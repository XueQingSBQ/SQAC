#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<queue>
#include <functional>
#include <algorithm>
#include<sstream>
#include<set>
#include <memory>
#include <map>
#include <chrono>
#include <cstdint>
#include <bitset>  // 包含 bitset 头文件
#include"node.h"
#include"../BicoreIndex/bicore_index.h"
#include"dyn_rebuild.h"
#include"../Utils/graph_utils.h"
#include"Graph.h"
#include <stack>

using namespace std;

class VC_index {
    public:
        Graph&g;
        std::unordered_map<int, 
        std::unordered_map<int, 
        std::vector<label_node*>>> VC_uindex;
        std::unordered_map<int, 
        std::unordered_map<int, 
        std::vector<label_node*>>> VC_vindex;

        std::unordered_map<int, 
        std::unordered_map<int, 
        std::vector<label_node*>>> VC_u_vindex;
        std::unordered_map<int, 
        std::unordered_map<int, 
        std::vector<label_node*>>> VC_v_uindex; 
        VC_index(Graph& graph) : g(graph) {}
        ~VC_index() {}
        void insert_umask(std::unordered_map<int, 
            std::unordered_map<int, 
            std::vector<label_node*>>> &index, int u,int a, int b, uint32_t label_mask);
        void creat_VC_index(string filename);
        void VC_index_opt();
        void load_VC_index(const string& filename, unordered_map<int, unordered_map<int, vector<label_node*>>>& index_batch);
        void save_VC_index(const string& filename, const unordered_map<int, unordered_map<int, vector<label_node*>>>& index_batch);
        double VC_query (unordered_set<string>&LU,unordered_set<string>&LV ,int a,int b);
};