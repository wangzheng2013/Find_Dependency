#include "basic.h"
#include <set>
#include <vector>
#include <algorithm>

int* number_list;
int total_number = 0;

int main()
{
	total_number = readFromFile("data.txt");
	tane(m_data, 12);
	return 0;
}

int calculateC(int a, int b) {
	if ((b - a) < a) {
		a = b - a;
	}
	int result = 1;
	for (int i = 0; i < a; i++) {
		result *= (b - i);
	}
	for (int i = 1; i <= a; i++) {
		result /= i;
	}
	return result;
}
//更新候选列表，正确！！！
void generateNextList(int* next_list, graph_node& node, int attr_num) {
	int index = 0;
	int temp;
	for (int i = 0; i < attr_num; i++) {
		temp = node.get_this_node();
		temp &= number_list[i];
		if (!temp) {
			next_list[index] = node.get_this_node() | number_list[i];
			index += 1;
		}
	}
}
//生成下一层，正确！！！
void generateNextLevel(map<int, int>& next_layer, vector<graph_node>& current_layer, vector<graph_node>& last_layer, int l, int attr_num) {
	int* next_list = new int[attr_num - l];
	last_layer.clear();
	for (auto val : current_layer) {
		if (!val.get_delete()) {
			last_layer.push_back(val);
			generateNextList(next_list, val, attr_num);
			for (int i = 0; i < attr_num - l; i++) {
				if (next_layer.find(next_list[i]) == next_layer.end()) {
					next_layer[next_list[i]] = l + 1;
				}
				else {
					continue;
					//next_layer[next_list[i]] += 1;
				}
			}
		}
	}
	current_layer.clear();
	for (auto val : next_layer) {
		if (val.second == (l + 1)) {
			graph_node temp_node = graph_node();
			temp_node.set_this_node(val.first);
			temp_node.update_from_this();
			current_layer.push_back(temp_node);
		}
	}
	next_layer.clear();
	delete[] next_list;
}

void tane(data_node data[], int attr_num) {
	vector<graph_node> last_layer = vector<graph_node>();
	vector<graph_node> current_layer = vector<graph_node>();
	map<int, int> node_to_rhs;
	map<int, graph_node> node_index;
	map<int, int> next_layer;
	//partition可以说到每一个attribute的映射，经过and就能生成partition
	map<int, vector<vector<int>>> partition;
	map<int, vector<int>> divider;
	number_list = new int[attr_num];
	for (int i = 0; i < attr_num; i++) {
		number_list[i] = (1 << i);
	}
	//得到首层partition
	getFirstLayerPartition(data, partition, divider, attr_num);
	int l = 1;
	last_layer.resize(1);
	node_index[0] = last_layer[0];
	node_to_rhs[0] = 0xfff;
	current_layer.resize(attr_num);
	for (int i = 0; i < attr_num; i++) {
		current_layer[i].set_bit(i);
		current_layer[i].update_from_list();
	}
	getFirstLayerRHS(node_to_rhs);
	while (current_layer.size() != 0 && l != 12) {
		computeDependency(current_layer, node_to_rhs, l, partition, divider);
		generateNextLevel(next_layer, current_layer, last_layer, l, attr_num);
		l++;
	}
}
//根据上层得到当前层rhs和partition 正确！！！
void get_rhs_and_partition(map<int, int>& node_to_rhs, map<int, vector<vector<int>>>& partition, map<int, vector<int>>& divider, vector<graph_node>& current_layer, vector<graph_node>& last_layer, int layer, int attr_num) {
	for (auto val : current_layer) {
		int sum = 0xfff;
		vector<int> tempDivider;
		//求RHS
		for (int i = 0; i < attr_num; i++) {
			int temp = val.get_this_node() & number_list[i];
			if (temp) {
				sum &= (node_to_rhs[temp]);
			}
		}
		//求partition分割线
		for (int i = 0; i < attr_num; i++) {
			int temp = val.get_this_node() & number_list[i];
			int temp1;
			if (temp) {
				temp1 = 0xfff & (~temp);
				tempDivider = getDivider(divider[temp], divider[temp1]);
				break;
			}
		}
		node_to_rhs[val.get_this_node()] = sum;
		divider[val.get_this_node()] = tempDivider;
		//partition[val.get_this_node()] = getPartition(tempDivider);
	}
}
//得到首层partition 正确！！！
void getFirstLayerRHS(map<int, int>& node_to_rhs) {
	for (int i = 0; i < 12; i++) {
		node_to_rhs[number_list[i]] = 0xfff & (~(number_list[i]));
	}
}

void computeDependency(vector<graph_node>& current_layer, map<int, int>& node_to_rhs, int layer, map<int, vector<vector<int>>>& partition, map<int, vector<int>>& divider) {
	if (layer == 1) return;
	get_rhs_and_partition(node_to_rhs, partition, divider, current_layer, current_layer, layer, 12);
	for (auto val : current_layer) {
		int *current_node = val.get_attr();
		int rhs_x = node_to_rhs[val.get_this_node()];
		for (int i = 0; i < 12; i++) {
			if (current_node[i] != 0) {
				int pre_layer_node = 0xfff;
				for (int j = 0; j < 12; j++) {
					if (i == j) {
						pre_layer_node &= (~(number_list[j]));
						continue;
					}
					if (current_node[j] == 0) pre_layer_node &= (~(number_list[j]));
				}
				if ((pre_layer_node != number_list[i]) && 
					(judgeValid(divider[pre_layer_node], divider[pre_layer_node | number_list[i]]))) {
					output(pre_layer_node, i);
					//remove A from C
					rhs_x &= (~(number_list[i]));
					//remove all B in R\X from C
					rhs_x &= (~(0xfff & (~(pre_layer_node | number_list[i]))));
					if (!rhs_x) break;
				}
			}
			node_to_rhs[val.get_this_node()] = rhs_x;
		}
		if (!rhs_x) {
			val.set_delete();
		}
	}
}

bool judgeValid(vector<int> node1, vector<int> node2) {
	if (node1.size() == node2.size())
		return true;
	return false;
}

//第一次分partition，扫1次，存入所有的映射中 正确！！！
void getFirstLayerPartition(data_node data[], map<int, vector<vector<int>>>& partition, map<int, vector<int>>& divider, int attr_num) {
	
	int data_length = total_number;
	for (int i = 0; i < attr_num; i++) {
		if (i == 0) {
			vector<int> temp;
			for (int j = 0; j < data_length; j++) {
				vector<int>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item1);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item1);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					if (partition[number_list[i]].size() > 1) {
						divider[number_list[i]].push_back(j);
					}
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
				}
			}
		} else if (i == 1) {
			vector<int> temp;
			for (int j = 0; j < data_length; j++) {
				vector<int>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item2);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item2);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					if (partition[number_list[i]].size() > 1) {
						divider[number_list[i]].push_back(j);
					}
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
				}
			}
		} else if (i == 2) {
			vector<int> temp;
			for (int j = 0; j < data_length; j++) {
				vector<int>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item3);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item3);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					if (partition[number_list[i]].size() > 1) {
						divider[number_list[i]].push_back(j);
					}
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
				}
			}
		} else if (i == 3) {
			vector<string> temp;
			for (int j = 0; j < data_length; j++) {
				vector<string>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item4);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item4);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					if (partition[number_list[i]].size() > 1) {
						divider[number_list[i]].push_back(j);
					}
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
				}
			}
		} else if (i == 4) {
			vector<char> temp;
			for (int j = 0; j < data_length; j++) {
				vector<char>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item5);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item5);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					if (partition[number_list[i]].size() > 1) {
						divider[number_list[i]].push_back(j);
					}
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
				}
			}
		} else if (i == 5) {
			vector<string> temp;
			for (int j = 0; j < data_length; j++) {
				vector<string>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item6);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item6);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					if (partition[number_list[i]].size() > 1) {
						divider[number_list[i]].push_back(j);
					}
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
				}
			}
		} else if (i == 6) {
			vector<short int> temp;
			for (int j = 0; j < data_length; j++) {
				vector<short int>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item7);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item7);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					if (partition[number_list[i]].size() > 1) {
						divider[number_list[i]].push_back(j);
					}
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
				}
			}
		} else if (i == 7) {
			vector<string> temp;
			for (int j = 0; j < data_length; j++) {
				vector<string>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item8);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item8);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					if (partition[number_list[i]].size() > 1) {
						divider[number_list[i]].push_back(j);
					}
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
				}
			}
		} else if (i == 8) {
			vector<string> temp;
			for (int j = 0; j < data_length; j++) {
				vector<string>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item9);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item9);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					if (partition[number_list[i]].size() > 1) {
						divider[number_list[i]].push_back(j);
					}
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
				}
			}
		} else if (i == 9) {
			vector<string> temp;
			for (int j = 0; j < data_length; j++) {
				vector<string>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item10);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item10);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					if (partition[number_list[i]].size() > 1) {
						divider[number_list[i]].push_back(j);
					}
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
				}
			}
		} else if (i == 10) {
			vector<string> temp;
			for (int j = 0; j < data_length; j++) {
				vector<string>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item11);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item11);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					if (partition[number_list[i]].size() > 1) {
						divider[number_list[i]].push_back(j);
					}
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
				}
			}
		} else if (i == 11) {
			vector<int> temp;
			for (int j = 0; j < data_length; j++) {
				vector<int>::iterator tempIterator;
				tempIterator = find(temp.begin(), temp.end(), data[j].item12);
				if (tempIterator == temp.end()) {
					temp.push_back(data[j].item12);
					vector<int> temp_j;
					temp_j.push_back(j);
					partition[number_list[i]].push_back(temp_j);
					if (partition[number_list[i]].size() > 1) {
						divider[number_list[i]].push_back(j);
					}
				}
				else {
					partition[number_list[i]][tempIterator - temp.begin()].push_back(j);
				}
			}
		}
	}
}
//比较函数
bool cmp(int m_one, int m_two)
{
	return (m_one < m_two);//定义了大于，就是按从小到大排序 //可以定义任何你想要的比较操作  
}
//根据上一层节点求下一层分割线
vector<int> getDivider(vector<int> node1, vector<int> node2) {
	vector<int> temp;
	//getUnion(node1, node2, temp);
	/*求并集*/
	for (int i = 0; i < node1.size(); i++)
	{
		temp.push_back(node1[i]);
	}
	for (int j = 0; j < node2.size(); j++)
	{
		vector<int>::iterator k = find(temp.begin(), temp.end(), node2[j]);
		if (k == temp.end())
		{
			temp.push_back(node2[j]);
		}
	}
	sort(temp.begin(), temp.end(), cmp);
	return temp;
}
//之后根据上一层节点求下一层节点的partition 正确！！！
vector<vector<int>> getPartition(vector<int> tempDivider) {
	vector<vector<int>> result;
	for (vector<int>::iterator i = tempDivider.begin(); i < tempDivider.end(); i++) {
		vector<int> temp;
		if (i == tempDivider.begin()) {
			for (int j = 0; j < (*i); j++) {
				temp.push_back(j);
			}
			result.push_back(temp);
		}
		else {
			for (int j = (*(i - 1)); j < (*i); j++) {
				temp.push_back(j);
			}
			result.push_back(temp);
		}
	}
	vector<int> temp;
	for (int j = tempDivider.back(); j < total_number; j++) {
		temp.push_back(j);
	}
	result.push_back(temp);
	return result;
	/*vector<vector<int>>::iterator i;
	vector<vector<int>>::iterator j;
	vector<vector<int>> result;

	for (i = node1_partition.begin(); i < node1_partition.end(); i++) {
		for (j = node2_partition.begin(); j < node2_partition.end(); j++) {
			vector<int> temp;
			getIntersection(*i, *j, temp);
			vector<vector<int>>::iterator tempResult = find(result.begin(), result.end(), temp);
			if ((temp.size() > 0) && (tempResult == result.end()))
				result.push_back(temp);
		}
	}
	return result;*/
}
//求并集函数 正确！！！
void getUnion(vector<vector<int>>& vectorA, vector<vector<int>>& vectorB, vector<vector<int>>& vectorC)
{
	set<vector<int>> s;
	for (int i = 0; i < vectorA.size(); i++)
	{
		s.insert(vectorA[i]);
	}
	for (int j = 0; j < vectorB.size(); j++)
	{
		if (s.find(vectorB[j]) == s.end())
		{
			vectorC.push_back(vectorB[j]);
		}
	}
}

void output(int X_except_A, int A) {
	bool flag = true;
	for (int i = 0; i < 12; i++)
	{
		if (flag && (X_except_A & number_list[i])) {
			cout << (i + 1);
			flag = false;
		}
		else if (!flag && (X_except_A & number_list[i])) {
			cout << ", ";
			cout << (i + 1);
		}
	}

	if (flag) {
		return;
	}
	cout << "->" << (A + 1) << endl;
}
