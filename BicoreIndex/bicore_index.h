#pragma once

#include<iostream>
#include<vector>
#include<unordered_set>
#include<unordered_map>
#include<string>
#include<algorithm>
#include<queue>
using namespace std;
int  online_calculate_delta(unordered_map< int,vector<int>> edge_mp_u,
                              unordered_map< int,vector<int>> edge_mp_v,
                              unordered_set< int> u_set,
                              unordered_set<int> v_set,int n1,int n2);

   void computeBetaMax(       unordered_map<int,vector<int>> edge_mp_u,
                              unordered_map<int,vector<int>> edge_mp_v,
                              unordered_set<int> u_set,
                              unordered_set<int> v_set,
                              vector<int>degree_u,
                              vector<int>degree_v,
                              vector<unordered_map<int,int>> &Bmax_a_u,
                              vector<unordered_map<int,int>> &Amax_b_v,int a,int n1,int n2);
   
   void computeAlphaMax(      unordered_map<int,vector< int>> edge_mp_u,
                              unordered_map<int,vector<int>> edge_mp_v,
                              unordered_set< int> u_set,
                              unordered_set< int> v_set,
                              vector<int>degree_u,
                              vector<int>degree_v,
                              vector<unordered_map< int,int>> &Bmax_a_u,
                              vector<unordered_map< int,int>> &Amax_b_v,int b,int n1,int n2);
   
   void computeBetaMax_VE(    unordered_map<int,vector< int>> edge_mp_u,
                              unordered_map<int,vector<int>> edge_mp_v,
                              unordered_set<int> u_set,
                              unordered_set<int> v_set,
                              vector<int>degree_u,
                              vector<int>degree_v,
                              unordered_map<int,vector<int>>&u_a_bmax,
                              unordered_map< int,vector<int>>&v_b_amax,int a,int n1,int n2);
   
void computeAlphaMax_VE(unordered_map<int,vector<int>> &edge_u,
                              unordered_map<int,vector<int>>& edge_v,
                              unordered_set<int> u_set,
                              unordered_set< int> v_set,
                              vector<int>degree_u,
                              vector<int>degree_v,
                              unordered_map< int,vector<int>>&u_a_bmax,
                              unordered_map< int,vector<int>>&v_b_amax,int b,int n1,int n2);

