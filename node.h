#ifndef NODE_H
#define NODE_H

#include<iostream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<string>
#include<algorithm>
#include<sstream>
#include<fstream>
#include<cstdint>
#include <string.h>
#include <memory>
//#include <sys/resource.h>

using namespace std;

struct  label_node
{
    vector<uint32_t> label_vector;
    label_node* next=NULL;
};
struct  u_node
{
    unordered_set<int> uset;
    u_node* next=NULL;
};
struct node_graph
{
   /* data */
    unordered_map<int, vector< int>> edge_mp_u;
    unordered_map<int, vector< int>> edge_mp_v;
    unordered_set<int> u_set;
    unordered_set<int> v_set;
    vector<int> degree_u;
    vector<int>degree_v;
    
    unordered_map<int,vector<int>>u_a_Bmax;
    unordered_map<int,vector<int>>v_b_Amax;
    std::vector<bool> left_delete;
	std::vector<bool> right_delete;

	unordered_map<int,vector<int>> left_index_change;
	unordered_map<int,vector<int>> right_index_change;

	unordered_map<int,vector<int>> left_index_delete;
	unordered_map<int,vector<int>> right_index_delete;
	unordered_map<int,vector<int>> left_index_old;
	unordered_map<int,vector<int>> right_index_old;
};

#endif // NODE_H
