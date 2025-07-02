#ifndef GRAPH_H
#define GRAPH_H

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

#include <stack>
struct graphinc
{
     unordered_set<int> u_set;
     unordered_set<int> v_set;
 };
class Graph {
public:
    int n1;//顶点数
    int n2;
    int m;//边数
    
    vector<unordered_set<string>>ulabel; //每个上层顶点u对应的label集合
    vector<unordered_set<string>>vlabel; //每个下层顶点v对应的label集合
    vector<uint32_t>umask; //每个上层顶点u对应的label集合的掩码
    vector<uint32_t>vmask; //每个下层顶点v对应的label集合的掩码
    unordered_map<string,unordered_set<int>>labelu;//每个标签对应的u顶点集合
    unordered_map<string,unordered_set<int>>labelv;//每个标签对应的v顶点集合
    vector<vector<int>>edges_u;  //邻接表（存的边）
    vector<vector<int>>edges_v;
    unordered_set<string>LABEL_U;       //总的上层标签集合
    unordered_set<string>LABEL_V;       //总的下层标签集合
    int max_degree_u=0;
    int max_degree_v=0;
    unordered_map<uint32_t, node_graph> ulabel_bicore;
    unordered_map<uint32_t, node_graph> vlabel_bicore;
    
     //索引1的数据结构：
      
        std::unordered_map<
        std::string, 
        std::unordered_map<std::string, 
        std::unordered_map<int,
        std::unordered_map<int, 
        std::unordered_set<int>>>>> index_core_u;
        std::unordered_map<
        std::string, 
        std::unordered_map<std::string, 
        std::unordered_map<int,
        std::unordered_map<int, 
        std::unordered_set<int>>>>> index_core_v;

        std::unordered_map<int, 
        std::unordered_map<int, 
        std::vector<label_node*>>> VC_uindex;
        std::unordered_map<int, 
        std::unordered_map<int, 
        std::vector<label_node*>>> VC_vindex;

        unordered_map<uint32_t,
        unordered_map<int,
        vector<u_node*>>> AC_u_uindex;
        unordered_map<uint32_t,
        unordered_map<int,
        vector<u_node*>>> AC_u_vindex;

        unordered_map<uint32_t,
        unordered_map<int,
        vector<u_node*>>> AC_v_vindex;
        unordered_map<uint32_t,
        unordered_map<int,
        vector<u_node*>>> AC_v_uindex;

        unordered_map<uint32_t,
        unordered_map<int,
        vector<u_node*>>> MAC_u_uindex;
        unordered_map<uint32_t,
        unordered_map<int,
        vector<u_node*>>> MAC_u_vindex;
        unordered_map<uint32_t,
        unordered_map<int,
        vector<u_node*>>> MAC_v_uindex;
        unordered_map<uint32_t,
        unordered_map<int,
        vector<u_node*>>> MAC_v_vindex;
    
    Graph();
    void createGraph(string filename);
    void  subgraph_Extraction(const unordered_set<string>&LU,const unordered_set<string>&LV,
                              unordered_map< int,vector< int>> &edge_mp_u,
                              unordered_map< int,vector< int>> &edge_mp_v,
                              unordered_set< int>&u_set,
                              unordered_set<int>&v_set );
    void  subgraph_Extraction_ve(const vector<string>&LU,const vector<string>&LV,
                              unordered_map<  int,vector<int>> &edge_mp_u,
                              unordered_map< int,vector<int>> &edge_mp_v,

                              unordered_set< int>&u_set,
                              unordered_set<int>&v_set );
    void  subgraph_Extraction_u_ve(const vector<string>&LU,
      unordered_map<  int,vector<int>> &edge_mp_u,
      unordered_map< int,vector<int>> &edge_mp_v,

      unordered_set< int>&u_set,
      unordered_set<int>&v_set );
    void  subgraph_Extraction_v_ve(const vector<string>&LV,
      unordered_map<  int,vector<int>> &edge_mp_u,
      unordered_map< int,vector<int>> &edge_mp_v,

      unordered_set< int>&u_set,
      unordered_set<int>&v_set );
     void labels_to_mask(unordered_set<string>& LU ,unordered_set<string>& LV,uint32_t&u_mask, uint32_t&v_mask);
     void labels_to_mask_init();
     void creat_VC_index();
     void VC_index_opt();
     void creat_AC_Uindex();
     void creat_AC_Vindex();
     void creat_MAC_Uindex();
     void creat_MAC_Vindex();
     double  online_core_label(unordered_set<string>&LU,unordered_set<string>&LV ,int a,int b);
     double online_core_label_graph(unordered_set<string>&LU,unordered_set<string>&LV ,int a,int b);
     void index_core_query(unordered_set<string>&LU,unordered_set<string>&LV,int a,int b);
     double VC_query (unordered_set<string>&LU,unordered_set<string>&LV ,int a,int b);
     double AC_query (unordered_set<string>&LU,unordered_set<string>&LV ,int a,int b);
     double MAC_query (unordered_set<string>&LU,unordered_set<string>&LV ,int a,int b);
     graphinc online_core(int q,unordered_set<int> &u_set,unordered_set<int> &v_set, unordered_map< int,vector<int>> &edge_u,unordered_map< int,vector< int>> &edge_v,int a,int b);
     void MAC_to_bicore();
     double  Edge_add_MAC_maintenance(string filename);
     void MAC_to_bicore_delete();
     double  Edge_delete_MAC_maintenance(string filename);
     void load_AC_index(const string& filename, unordered_map<uint32_t,unordered_map<int, vector<u_node*>>>& label_index_batch);
     void load_VC_index(const string& filename, unordered_map<int, unordered_map<int, vector<label_node*>>>& index_batch);
     void insert_u_to_AC_index(unordered_map<uint32_t,unordered_map<int,vector<u_node*> > >&label_index_batch,int u,int a,int b,uint32_t label_mask);
     void AC_index_opt(unordered_map<uint32_t,unordered_map<int,vector<u_node*> > >&label_index_batch);
     void save_AC_index(const string& filename, const unordered_map<uint32_t, unordered_map<int, vector<u_node*>>>& label_index_batch);
     void save_VC_index(const string& filename, const unordered_map<int, unordered_map<int, vector<label_node*>>>& index_batch);
    void insert_u_to_MAC_index(unordered_map<uint32_t,unordered_map<int,vector<u_node*> > >&label_index_batch,int u,int a,int b,uint32_t label_mask);
     // void insert_umask(std::unordered_map<int, 
  // std::unordered_map<int, 
  // std::vector<label_node*>>> &indexu_batch, int u,int a, int b, uint32_t label_mask);
};

#endif