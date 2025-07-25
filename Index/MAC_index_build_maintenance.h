#pragma once

#include"Graph.h"

class MAC_index
{

public:
    Graph& g;  
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
    MAC_index(Graph& g) : g(g) {}
     ~MAC_index(){}

     void creat_MAC_Uindex(string filename);
     void creat_MAC_Vindex(string filename);


     double MAC_query (unordered_set<string>&LU,unordered_set<string>&LV ,int a,int b);


     void MAC_index_opt(unordered_map<uint32_t,unordered_map<int,vector<u_node*> > >&label_index_batch);
     void save_MAC_index(const string& filename, const unordered_map<uint32_t, unordered_map<int, vector<u_node*>>>& label_index_batch);
     void insert_u_to_MAC_index(unordered_map<uint32_t,unordered_map<int,vector<u_node*> > >&label_index_batch,int u,int a,int b,uint32_t label_mask);
     void load_MAC_index(const string& filename, unordered_map<uint32_t,unordered_map<int, vector<u_node*>>>& label_index_batch);

     graphinc online_core(int q,unordered_set<int> &u_set,unordered_set<int> &v_set, unordered_map< int,vector<int>> &edge_u,unordered_map< int,vector< int>> &edge_v,int a,int b);
     void MAC_to_bicore();
     double  Edge_add_MAC_maintenance(string filename);
     void MAC_to_bicore_delete();
     double  Edge_delete_MAC_maintenance(string filename);
     void  delete_u_to_MAC(unordered_map<uint32_t,unordered_map<int,vector<u_node*> > >&label_index_batch,int u,int a,uint32_t label_mask);
};

