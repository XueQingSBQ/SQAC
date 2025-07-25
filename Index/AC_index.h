#pragma once
#include"Graph.h"

class AC_index
{
public:
    Graph& g;
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
    AC_index(Graph& graph) : g(graph) {}
    ~AC_index(){}
    void creat_AC_Uindex(string filename);
    void creat_AC_Vindex(string filename);
    double AC_query (unordered_set<string>&LU,unordered_set<string>&LV ,int a,int b);
    void load_AC_index(const string& filename, unordered_map<uint32_t,unordered_map<int, vector<u_node*>>>& label_index_batch);
    void save_AC_index(const string& filename, const unordered_map<uint32_t, unordered_map<int, vector<u_node*>>>& label_index_batch);
    void insert_u_to_AC_index(unordered_map<uint32_t,unordered_map<int,vector<u_node*> > >&label_index_batch,int u,int a,int b,uint32_t label_mask);
    void AC_index_opt(unordered_map<uint32_t,unordered_map<int,vector<u_node*> > >&label_index_batch);
};

