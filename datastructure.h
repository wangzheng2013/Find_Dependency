#ifndef DATASTRUCTURE_H_
#define DATASTRUCTURE_H_

#include <iostream>
#include <string>
using namespace std;

extern int* number_list;

struct data_node {
	int item1;
	int item2;
	int item3;
	string item4;
	char item5;
	string item6;
	short int item7;
	string item8;
	string item9;
	//char item9[4];
	string item10;
	//char item11[3];
	string item11;
	int item12;
};

class graph_node {
private:
	int attr_num;
	int* attr;
	int this_node;
	int subset_num;
	bool is_delete;
public:
	int* get_attr() {
		return attr;
	}
	int get_this_node() {
		return this_node;
	}
	int get_subset_num() {
		return subset_num;
	}
	void set_this_node(int s) {
		this_node = s;
	}
	void update_from_this() {
		for (int i = 0; i < attr_num; i++) {
			if (number_list[i] & this_node) {
				attr[i] = 1;
			}
			else {
				attr[i] = 0;
			}
		}
	}
	void update_from_list() {
		this_node = 0;
		for (int i = 0; i < attr_num; i++) {
			if (attr[i] == 1) {
				this_node += number_list[i];
			}
		}
	}
	void set_bit(int i) {
		attr[i] = 1;
	}
	void set_delete() {
		is_delete = true;
	}
	bool get_delete() {
		return is_delete;
	}
	void add_subset() {
		subset_num += 1;
	}
	graph_node() {
		attr_num = 12;
		attr = new int[attr_num];
		this_node = 0;
		for (int i = 0; i < attr_num; i++) {
			attr[i] = 0;
		}
		subset_num = 0;
		is_delete = false;
	}
	~graph_node() {
		//delete[] attr;
	}
};

#endif
