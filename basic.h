#ifndef BASIC_H
#define BASIC_H
#include "datastructure.h"
#include <map>
#include <vector>

extern data_node m_data[100000];

extern int readFromFile(char* filename);
extern data_node spiltline(string buffer);
extern void outPutNode(data_node Node);
extern void get_rhs_and_partition(map<int, int>& node_to_rhs, map<int, vector<vector<int>>>& partition, map<int, vector<int>>& divider, vector<graph_node>& current_layer, vector<graph_node>& last_layer, int layer, int attr_num);
extern void getFirstLayerRHS(map<int, int>& node_to_rhs);
extern void tane(data_node data[], int attr_num);
extern void generateNextLevel(map<int, int>& next_layer, vector<graph_node>& current_layer, vector<graph_node>& last_layer, int l, int attr_num);
extern void generateNextList(int* next_list, graph_node& node, int attr_num);
extern int calculateC(int a, int b);
extern void computeDependency(vector<graph_node>& current_layer, map<int, int>& node_to_rhs, int layer, map<int, vector<vector<int>>>& partition, map<int, vector<int>>& divider);
extern vector<vector<int>> getPartition(vector<int> tempDivider);
extern void getFirstLayerPartition(data_node data[], map<int, vector<vector<int>>>& partition, map<int, vector<int>>& divider, int attr_num);
extern void getUnion(vector<int>& vectorA, vector<int>& vectorB, vector<int>& vectorC);
bool cmp(int m_one, int m_two);
extern vector<int> getDivider(vector<int> node1, vector<int> node2);
extern bool judgeValid(vector<int> node1, vector<int> node2);
extern void output(int X_except_A, int A);
#endif
