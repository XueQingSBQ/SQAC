#ifndef CASESTUDY_H
#define CASESTUDY_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <cassert>
#include"Graph.h"
#include"GraphUtils.h"

using namespace std;

// 用于将 unordered_set<string> 转成唯一 key，方便去重
string serialize_set(const unordered_set<string>& s) {
   vector<string> temp(s.begin(), s.end());
   sort(temp.begin(), temp.end());
   string res;
   for (const string& str : temp) {
       res += str + "#";
   }
   return res;
}
int count_ones(uint32_t x) {
   return __builtin_popcount(x); // GCC/Clang专用，MSVC下可用手写版本
}

 bool cmp( pair<uint32_t, uint32_t> a,  pair<uint32_t, uint32_t> b) {
   int a_count = count_ones(a.first) + count_ones(a.second);
   int b_count = count_ones(b.first) + count_ones(b.second);
   return a_count > b_count; // 从大到小
}
double inc(int q,int a,int b,Graph&G)
{
  auto start= chrono::system_clock::now();
    unordered_set<string> SULabel,SVLabel;
    SULabel= G.ulabel[q];
    for(auto v:G.edges_u[q])
    {
        SVLabel.insert(G.vlabel[v].begin(),G.vlabel[v].end());
    }
    unordered_map<string,unordered_set<int>> P,Q;
    unordered_set<string> newSULabel;
    for(auto label:SULabel){
        if(G.labelu[label].size()>=b){
           P[label]=G.labelu[label];
           newSULabel.insert(label);
        }
        
    }
    SULabel = std::move(newSULabel);
    newSULabel.clear();
    unordered_set<string> newSVLabel;
    for(auto label:SVLabel){
        if(G.labelv[label].size()>=a){
           Q[label]=G.labelv[label];
           newSVLabel.insert(label);
        }
    }
    SVLabel = std::move(newSVLabel);
   int l=0;
   unordered_set<string> c;
  vector<vector<pair<unordered_set<string>,graphinc>>>ans;
  ans.resize(1);
   for(auto label:SULabel)
   {
     for(auto label2:SVLabel){
        
        unordered_set<int> u_set=P[label];
        unordered_set<int> v_set=Q[label2];
        unordered_map< int,vector<int>> edge_u;
        unordered_map< int,vector< int>> edge_v;
        
        for(auto u:u_set){
            for(auto v:G.edges_u[u]){
              if(v_set.find(v)!=v_set.end()){
                 edge_u[u].push_back(v);
                 edge_v[v].push_back(u);
              }
            }
        }
        
        graphinc g=G.online_core(q,u_set,v_set,edge_u,edge_v,a,b);
        if(g.u_set.size()>0)
        {
           
           if(ans.size()<=l)
           ans.resize(l+2);  
           c.insert(label);
           c.insert(label2);
           ans[l].push_back(make_pair(c,g));
           c.clear();
        }
     }
   }
   unordered_set<string> seen; // 记录已出现的组合 key
   while (ans[l].size()!=0)
   {
      ans.resize(l+2);
      for(int i=0;i<ans[l].size();i++){

        for(int j=i+1;j<ans[l].size();j++){
  
           pair<unordered_set<string>,graphinc> p1=ans[l][i];
           pair<unordered_set<string>,graphinc> p2=ans[l][j];
           unordered_set<string>c1=p1.first;
           unordered_set<string>c2=p2.first;
           c.insert(c1.begin(),c1.end());
           c.insert(c2.begin(),c2.end());
           graphinc g1=p1.second;
           graphinc g2=p2.second;
           graphinc g;
          
           for(const auto& it:g1.u_set){
              if(g2.u_set.find(it)!=g2.u_set.end()){
                 g.u_set.insert(it);
              }
           }
           for(const auto& it:g1.v_set){
              if(g2.v_set.find(it)!=g2.v_set.end()){
                 g.v_set.insert(it);
              }
           }
           string key = serialize_set(c); // 当前组合的唯一表示

           if (seen.count(key)) continue; // 跳过已处理的组合
           
           unordered_map< int,vector<int>> edge_u;
           unordered_map< int,vector< int>> edge_v;

           for(auto u:g.u_set){
              for(auto v:G.edges_u[u]){
                 
                 if(g.v_set.find(v)!=g.v_set.end()){
                    edge_u[u].push_back(v);
                    edge_v[v].push_back(u);
                 }
              }
           }
        
           g=G.online_core(q,g.u_set,g.v_set,edge_u,edge_v,a,b);

           if(g.u_set.size()!=0){
              ans[l+1].push_back(make_pair(c,g));
              seen.insert(key); // 记录已插入的组合
           }
           c.clear();
        }
      }
      l=l+1;
   }
   int max_size=0;
   graphinc g;
   for(int i=0;i<l;i++){
      for(const auto& p:ans[i]){
        if(p.first.size()>max_size){
           max_size=p.first.size();
          
        }
      }
   }
   //cout<<1<<endl;
  auto end= chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  
   return elapsed_seconds.count();
     

}
std::vector<uint32_t> getNonEmptySubmasks(uint32_t mask) {
  std::vector<uint32_t> submasks;
  for (uint32_t sub = mask; sub > 0; sub = (sub - 1) & mask) {
      submasks.push_back(sub);
  }
  return submasks;
}



double Dec(int q,int a,int b,Graph&G)
{
  auto start= chrono::system_clock::now();
    vector<uint32_t> Umasks = getNonEmptySubmasks(G.umask[q]);
    vector<uint32_t> Vmasks;
    unordered_map<uint32_t,int> mask_cnt;
    for(auto v:G.edges_u[q]){
        vector<uint32_t> Vmasks_tmp = getNonEmptySubmasks(G.vmask[v]);
        for(auto v_mask:Vmasks_tmp){
            mask_cnt[v_mask]++;
        }
    }
    for(auto mask:mask_cnt){
        if(mask.second>=a){
            Vmasks.push_back(mask.first);
        }
    }

    unordered_map<uint32_t,unordered_set<int>> P,Q;
    unordered_set<string> newSULabel;
    for(auto mask:Umasks){
        for(int i=1;i<G.n1;i++){
           if((mask&G.umask[i])==mask){
             
              P[mask].insert(i);
           }
        }
     }
     for(auto mask:Vmasks){
        for(int i=1;i<G.n2;i++){
           if((mask&G.vmask[i])==mask){
              Q[mask].insert(i);
           }
        }
     }
    unordered_set<uint32_t> newUmask;
    for(auto mask:Umasks){
        if(P[mask].size()>=b){
           newUmask.insert(mask);
        }
    }
    Umasks.clear();
    for(auto mask:newUmask){
        Umasks.push_back(mask);
    }

   int maxlen=0;
   vector<pair<pair<uint32_t,uint32_t>,graphinc>>ans;
   vector<pair<uint32_t,uint32_t>> S;
   for(auto umask:Umasks)
   {
     for(auto vmask:Vmasks)
     {
          S.push_back(make_pair(umask,vmask));
     }
   }
   sort(S.begin(),S.end(),cmp);
   for(auto &s:S){
     if(count_ones(s.first)+count_ones(s.second)<maxlen){
        break;
     }
     else{
        unordered_set<int> &u_set=P[s.first];
        unordered_set<int> &v_set=Q[s.second];
        unordered_map< int,vector<int>> edge_u;
        unordered_map< int,vector< int>> edge_v;
        for(auto u:u_set){
            for(auto v:G.edges_u[u]){
              if(v_set.find(v)!=v_set.end()){
                 edge_u[u].push_back(v);
                 edge_v[v].push_back(u);
              }
            }
        }
        graphinc g=G.online_core(q,u_set,v_set,edge_u,edge_v,a,b);
        if(g.u_set.size()>0)
        {
           ans.push_back(make_pair(s,g));
           maxlen=count_ones(s.first)+count_ones(s.second);
        }
     }
  }
  auto end= chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  return elapsed_seconds.count();
}
double Dec_opt(int q,int a,int b,Graph&G)
{
   auto start= chrono::system_clock::now();
    vector<uint32_t> Umasks = getNonEmptySubmasks(G.umask[q]);
    vector<uint32_t> Vmasks;
    unordered_map<uint32_t,int> mask_cnt;
    for(auto v:G.edges_u[q]){
        vector<uint32_t> Vmasks_tmp = getNonEmptySubmasks(G.vmask[v]);
        for(auto v_mask:Vmasks_tmp){
            mask_cnt[v_mask]++;
        }
    }
    for(auto mask:mask_cnt){
        if(mask.second>=a){
            Vmasks.push_back(mask.first);
        }
    }
    unordered_map<uint32_t,unordered_set<int>> P,Q;
    unordered_set<string> newSULabel;
    for(auto mask:Umasks){
        for(int i=1;i<G.n1;i++){
           if((mask&G.umask[i])==mask){
              P[mask].insert(i);
              if(P[mask].size()>=b){
                 break;
              }
           }
        }
     }
    unordered_set<uint32_t> newUmask;
    for(auto mask:Umasks){
        if(P[mask].size()>=b){
           newUmask.insert(mask);
        }
    }
    Umasks.clear();
    for(auto mask:newUmask){
        Umasks.push_back(mask);
    }
   int maxlen=0;
   vector<pair<pair<uint32_t,uint32_t>,graphinc>>ans;
   vector<pair<uint32_t,uint32_t>> S;
   for(auto umask:Umasks)
   {
     for(auto vmask:Vmasks)
     {
          S.push_back(make_pair(umask,vmask));
     }
   }
   sort(S.begin(),S.end(),cmp);
   for(auto &s:S){
     uint32_t mask1=s.first;
     uint32_t mask2=s.second;
     if(count_ones(s.first)+count_ones(s.second)<maxlen){
        break;
     }
     else{
        unordered_set<int> u_set;
        unordered_set<int> v_set;
        for(auto &it1:G.MAC_u_uindex){
           uint32_t mask=it1.first;
           if((mask&mask1)!=mask){
              continue;
           }
           if(it1.second.find(a)!=it1.second.end()){
              vector<u_node*>& u_label_nodes=it1.second[a];
              if(u_label_nodes.size()>b){
                 u_node* u_label_node=u_label_nodes[b];
                 while(u_label_node!=nullptr){
                    unordered_set<int>& uset=u_label_node->uset;
                    for(auto u:uset){
                        if(u_set.find(u)==u_set.end()){
                          if((G.umask[u]&mask1)==mask1)
                           u_set.insert(u);
                        }
                    }
                    u_label_node=u_label_node->next;
                 }
              }
           }
         }
         for(auto &it2:G.MAC_v_vindex){
           uint32_t mask=it2.first;
           if((mask&mask2)!=mask){
              continue;
           }
           if(it2.second.find(b)!=it2.second.end()){
              vector<u_node*>& v_label_nodes=it2.second[b];
              if(v_label_nodes.size()>a){
                 u_node* v_label_node=v_label_nodes[a];
                 while ( v_label_node!=nullptr)
                 {
                    /* code */
                    unordered_set<int>& vset=v_label_node->uset;
                    for(auto v:vset){
                        if(v_set.find(v)==v_set.end()){
                          if((G.vmask[v]&mask2)==mask2)
                          v_set.insert(v);
                        }
                    }
                    v_label_node=v_label_node->next;
                 }
                 
              }
           }
         }
        unordered_map< int,vector<int>> edge_u;
        unordered_map< int,vector<int>> edge_v;

        for(auto u:u_set){
            for(auto v:G.edges_u[u]){
              if(v_set.find(v)!=v_set.end()){
                 edge_u[u].push_back(v);
                 edge_v[v].push_back(u);
              }
            }
        }
        graphinc g=G.online_core(q,u_set,v_set,edge_u,edge_v,a,b);
        if(g.u_set.size()>0)
        {
           ans.push_back(make_pair(s,g));
           maxlen=count_ones(s.first)+count_ones(s.second);
        }
     }
  }
  auto end= chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  return elapsed_seconds.count();
}
double inc_opt(int q,int a,int b,Graph&G)
{
   auto start= chrono::system_clock::now();
    unordered_map<string,uint32_t> label_mask_u;
    unordered_map<string,uint32_t> label_mask_v;
    vector<string> lABEL_U(G.LABEL_U.begin(),G.LABEL_U.end()),lABEL_V(G.LABEL_V.begin(),G.LABEL_V.end());


    label_mask_u=generate_32bit_map_ucombinations(lABEL_U,G.labelu);
    label_mask_v=generate_32bit_map_vcombinations(lABEL_V,G.labelv);
    unordered_set<string> SULabel,SVLabel;
    SULabel= G.ulabel[q];
    for(auto v:G.edges_u[q])
    {
        SVLabel.insert(G.vlabel[v].begin(),G.vlabel[v].end());
    }
    unordered_map<string,unordered_set<int>> P,Q;
    unordered_set<string> newSULabel;
    for(auto label:SULabel){
        uint32_t umask=label_mask_u[label];
        if(G.MAC_u_uindex[umask].find(a)!=G.MAC_u_uindex[umask].end()){
           vector<u_node*>& u_label_nodes=G.MAC_u_uindex[umask][a];
           if(u_label_nodes.size()>b){
              u_node* u_label_node=u_label_nodes[b];
              while(u_label_node!=nullptr){
                 unordered_set<int>& uset=u_label_node->uset;
                 P[label].insert(uset.begin(),uset.end());
                 u_label_node=u_label_node->next;
              }
              newSULabel.insert(label);
           }
        }
       // cout<<label<<" "<<umask<<" "<<P[label].size()<<endl;
        
    }
    SULabel = std::move(newSULabel);
    newSULabel.clear();
    unordered_set<string> newSVLabel;
    for(auto label:SVLabel){
        uint32_t vmask=label_mask_v[label];
        
        if(G.MAC_v_vindex[vmask].find(b)!=G.MAC_v_vindex[vmask].end()){
           vector<u_node*>& v_label_nodes=G.MAC_v_vindex[vmask][b];
           if(v_label_nodes.size()>a){
              u_node* v_label_node=v_label_nodes[a];
              while ( v_label_node!=nullptr)
              {
                 /* code */
                 unordered_set<int>& vset=v_label_node->uset;
                 Q[label].insert(vset.begin(),vset.end());
                 v_label_node=v_label_node->next;
              }
              newSVLabel.insert(label);
           }
        }
    }
    SVLabel = std::move(newSVLabel);
   int l=0;
   unordered_set<string> c;
  vector<vector<pair<unordered_set<string>,graphinc>>>ans;
   ans.resize(1);
   for(auto label:SULabel)
   {
     for(auto label2:SVLabel){
        unordered_set<int> u_set=P[label];
        unordered_set<int> v_set=Q[label2];
        unordered_map< int,vector<int>> edge_u;
        unordered_map< int,vector< int>> edge_v;

        for(auto u:u_set){
            for(auto v:G.edges_u[u]){
              if(v_set.find(v)!=v_set.end()){
                 edge_u[u].push_back(v);
                 edge_v[v].push_back(u);
              }
            }
        }
        graphinc g=G.online_core(q,u_set,v_set,edge_u,edge_v,a,b);
        if(g.u_set.size()>0)
        {
           if(ans.size()<=l)
           ans.resize(l+2);  
           c.insert(label);
           c.insert(label2);
           ans[l].push_back(make_pair(c,g));
           c.clear();
        }
     }
   }
  
   unordered_set<string> seen; // 记录已出现的组合 key
   while (ans[l].size()!=0)
   {
     
      ans.resize(l+2);
      for(int i=0;i<ans[l].size();i++){

        for(int j=i+1;j<ans[l].size();j++){
  
           pair<unordered_set<string>,graphinc> p1=ans[l][i];
           pair<unordered_set<string>,graphinc> p2=ans[l][j];
           unordered_set<string>c1=p1.first;
           unordered_set<string>c2=p2.first;
           c.insert(c1.begin(),c1.end());
           c.insert(c2.begin(),c2.end());
           graphinc g1=p1.second;
           graphinc g2=p2.second;
           graphinc g;
          
           for(const auto& it:g1.u_set){
              if(g2.u_set.find(it)!=g2.u_set.end()){
                 g.u_set.insert(it);
              }
           }
           for(const auto& it:g1.v_set){
              if(g2.v_set.find(it)!=g2.v_set.end()){
                 g.v_set.insert(it);
              }
           }
           string key = serialize_set(c); // 当前组合的唯一表示

           if (seen.count(key)) continue; // 跳过已处理的组合
           
           unordered_map< int,vector<int>> edge_u;
           unordered_map< int,vector< int>> edge_v;

           for(auto u:g.u_set){
              for(auto v:G.edges_u[u]){
                 
                 if(g.v_set.find(v)!=g.v_set.end()){
                    edge_u[u].push_back(v);
                    edge_v[v].push_back(u);
                 }
              }
           }
        
           g=G.online_core(q,g.u_set,g.v_set,edge_u,edge_v,a,b);

           if(g.u_set.size()!=0){
              ans[l+1].push_back(make_pair(c,g));
              seen.insert(key); // 记录已插入的组合
           }
           c.clear();
        }
      }
      l=l+1;
   }
   int max_size=0;
   graphinc g;
   for(int i=0;i<l;i++){
      for(const auto& p:ans[i]){
        if(p.first.size()>max_size){
           max_size=p.first.size();
          
        }
      }
   }
   auto end= chrono::system_clock::now();
   std::chrono::duration<double> elapsed_seconds = end - start;

     return  elapsed_seconds.count();

}

#endif