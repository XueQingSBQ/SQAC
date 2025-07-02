#ifndef BICORE_H
#define BICORE_H

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
                              unordered_set<int> v_set,int n1,int n2)
   {
        vector<int>degree_u(n1+1,0);
        vector<int>degree_v(n2+1,0);
        
        vector<int>del_u(n1+1,0);
        vector<int>del_v(n2+1,0);
         for(auto &it:edge_mp_u)
         {
            degree_u[it.first]=it.second.size();
         }
         for(auto &it:edge_mp_v)
         {
            degree_v[it.first]=it.second.size();
         }
          int r=0;
         while(!u_set.empty()||!v_set.empty())
         {   r++;
            while(!u_set.empty()||!v_set.empty())
            {
              vector< int>temp_u_set;
              vector< int>temp_v_set;
                
               for(auto &u:u_set)
               {
                
                 if(degree_u[u]<r&&del_u[u]==0)  //degree[u]<a
                 {
                   del_u[u]=1;       //删除标zhi符
                   for(auto v:edge_mp_u[u])  //删除u的边，将v的度减一
                   {  if(del_v[v]==0)
                      degree_v[v]--;
                   }
                   temp_u_set.emplace_back(u); //添加要删除degree[u]<a的点
                 }
               }
               
               for(auto &v:v_set)
               {
                 if(degree_v[v]<r&&del_v[v]==0) //degree[v]<b
                 {
                   del_v[v]=1;  //删除标志符=1
                   for(auto u:edge_mp_v[v]) //删除v的边，将u的度减一
                   {  if(del_u[u]==0)
                      degree_u[u]--;
                     
                   }
                    temp_v_set.emplace_back(v);//添加要删除degree[v]<b的点
                 }
               }
                for(auto u:temp_u_set) //删除degree[u]<a的点
               {
                  u_set.erase(u);
                }
                for(auto v:temp_v_set)
                {
                  v_set.erase(v); //删除degree[v]<b的点
                }

               if(temp_u_set.size()+temp_v_set.size()==0)
               {
                break; // 没有需要删除的点，退出循环
               }


            }
           
              
         }
         return r-1;
   }
   void computeBmax(         unordered_map<int,vector<int>> edge_mp_u,
                             unordered_map<int,vector<int>> edge_mp_v,
                              unordered_set<int> u_set,
                              unordered_set<int> v_set,
                              vector<int>degree_u,
                              vector<int>degree_v,
                              vector<unordered_map<int,int>> &Bmax_a_u,
                              vector<unordered_map<int,int>> &Amax_b_v,int a,int n1,int n2)
   {

      unordered_set< int>del_u;
      queue<int>del_v;
      int minB_v=INT32_MAX;
      vector<int>del1(n1+1,0);
      vector<int>del2(n2+1,0);
      for(const auto&u:u_set)
      {
         if(degree_u[u]<a)
         {
            del1[u]=1;
            del_u.insert(u);
            for(auto v:edge_mp_u[u])  //删除u的边，将v的度减一
            { 
               degree_v[v]--;
            }
         }
      }
      for(auto u:del_u) //删除degree[u]<a的点
      {
         u_set.erase(u);
      }
      del_u.clear();
      while(!u_set.empty()||!v_set.empty())
      {
          
          minB_v=INT32_MAX;
         //keyouhua
       for(const auto&v:v_set)
      {
         minB_v=min(minB_v,degree_v[v]);
      }

       for(const auto&v:v_set)
      {
           if(degree_v[v]==minB_v)
           {
            del_v.push(v);
            del2[v]=1;
           }
      }
      //
      while(!del_v.empty())
      {
         auto v=del_v.front();
         del_v.pop();
         
         v_set.erase(v);
            for(auto u:edge_mp_v[v])
            {  
               if(del1[u]==0)
               { degree_u[u]--;
                 if(degree_u[u]<a)
                 del_u.insert(u);
               }
            }
            for(int i=1;i<=minB_v;i++)
            {
               if(Amax_b_v[i][v]<a)
               {
                  Amax_b_v[i][v]=a;
               }
            }
         for(auto u:del_u)
         {
            Bmax_a_u[a][u]=minB_v;
            del1[u]=1;
            for(auto v:edge_mp_u[u])  //删除u的边，将v的度减一
            {  if(del2[v]==0)
               {degree_v[v]--;
                if(degree_v[v]<=minB_v)
                {
                  del_v.push(v);
                  del2[v]=1;
                }
                
               }

            }
            u_set.erase(u);
         }
          del_u.clear();
         

      }
      }      

   }
   void computeAmax(          unordered_map<int,vector< int>> edge_mp_u,
                              unordered_map<int,vector<int>> edge_mp_v,
                              unordered_set< int> u_set,
                              unordered_set< int> v_set,
                              vector<int>degree_u,
                              vector<int>degree_v,
                              vector<unordered_map< int,int>> &Bmax_a_u,
                              vector<unordered_map< int,int>> &Amax_b_v,int b,int n1,int n2)
   {

      queue< int>del_u;
      unordered_set< int>del_v;
      int minA_v=INT32_MAX;
      vector<int>del1(n1+1,0);
      vector<int>del2(n2+1,0);
      for(const auto&v:v_set)
      {
         if(degree_v[v]<b)
         {
            del2[v]=1;
            del_v.insert(v);
            for(auto u:edge_mp_v[v])  //删除u的边，将v的度减一
            { 
               degree_u[u]--;
            }
         }
      }
      for(auto v:del_v) //删除degree[u]<a的点
      {
         v_set.erase(v);
      }
      del_v.clear();
      while(!u_set.empty()||!v_set.empty())
      {
          
          minA_v=INT32_MAX;
         //keyouhua
         for(const auto&u:u_set)
         {
            minA_v=min(minA_v,degree_u[u]);
         }

         for(const auto&u:u_set)
         {
            if(degree_u[u]==minA_v)
            {
               del_u.push(u);
               del1[u]=1;
            }
         }
         //
         while(!del_u.empty())
         {
            auto u=del_u.front();
            del_u.pop();
           
            u_set.erase(u);
            for(auto v:edge_mp_u[u])
            {  
               if(del2[v]==0)
               { degree_v[v]--;
               if(degree_v[v]<b)
                del_v.insert(v);
               }
            }
            for(int i=1;i<=minA_v;i++)
            {
               if(Bmax_a_u[i][u]<b)
               {
                  Bmax_a_u[i][u]=b;
               }
            }
            for(auto v:del_v)
            {
               Amax_b_v[b][v]=minA_v;
               del2[v]=1;
               for(auto u:edge_mp_v[v])  //删除u的边，将v的度减一
               {  if(del1[u]==0)
                  {degree_u[u]--;
                   if(degree_u[u]<=minA_v)
                   {
                     del_u.push(u);
                      del1[u]=1;
                   }
                  }
               }
               v_set.erase(v);
            }
            del_v.clear();

          }    
      }      
   }
   void computeBmax_VE(         unordered_map<int,vector< int>> edge_mp_u,
                             unordered_map<int,vector<int>> edge_mp_v,
                              unordered_set<int> u_set,
                              unordered_set<int> v_set,
                              vector<int>degree_u,
                              vector<int>degree_v,
                              unordered_map<int,vector<int>>&u_a_bmax,
                             unordered_map< int,vector<int>>&v_b_amax,int a,int n1,int n2)
   {

      vector<int>del_u;
      vector<int>del_v;
      int minB_v=INT32_MAX;
      vector<bool>delu(n1+1,false);
      vector<bool>delv(n2+1,false);
 
      for(const auto&u:u_set)
      {
         if(degree_u[u]<a)
         {
            delu[u]=true;
            del_u.push_back(u);
            for(auto &v:edge_mp_u[u])  //删除u的边，将v的度减一
            { 
               degree_v[v]--;
            }
         }
      }
      for(auto u:del_u) //删除degree[u]<a的点
      {
         u_set.erase(u);
      }
      del_u.clear();
      
      while(!u_set.empty()||!v_set.empty())
      {
          
          minB_v=INT32_MAX;
       for(const auto&v:v_set)
      {
         minB_v=min(minB_v,degree_v[v]);
      }

       for(const auto&v:v_set)
      {
           if(degree_v[v]==minB_v)
           {
            del_v.push_back(v);
            delv[v]=true;
           }
      }
     
      while(!del_v.empty()||!del_u.empty()){
        
         for(int i=0;i<del_v.size();i++){
            int v=del_v[i];
            v_set.erase(del_v[i]);
            for(int i=0;i<edge_mp_v[v].size();i++) 
            {
               int u=edge_mp_v[v][i];
               if(!delu[u])
               {
                  degree_u[u]--;
                  if(degree_u[u]<a)
                  {
                    del_u.push_back(u);
                    delu[u]=true;
                  }
               }
            }
            for(int i=1;i<=minB_v;i++)
            {
               if(v_b_amax[v][i]<a)
               {
                  v_b_amax[v][i]=a;
               }
            }
         }
         del_v.clear();
         for(int i=0;i<del_u.size();i++){
            int u=del_u[i];
            u_set.erase(u);
            u_a_bmax[u][a]=minB_v;
            for(int i=0;i<edge_mp_u[u].size();i++) 
            {
                int v=edge_mp_u[u][i];
                if(!delv[v])
                {
                  degree_v[v]--;
                  if(degree_v[v]<=minB_v)
                  {
                    del_v.push_back(v);
                    delv[v]=true;
                  }
                }
            }  
         }
         del_u.clear();
      }
      
      }      

   }
void computeAmax_VE(unordered_map<int,vector<int>> &edge_u,
                              unordered_map<int,vector<int>>& edge_v,
                              unordered_set<int> u_set,
                              unordered_set< int> v_set,
                              vector<int>degree_u,
                              vector<int>degree_v,
                              unordered_map< int,vector<int>>&u_a_bmax,
                              unordered_map< int,vector<int>>&v_b_amax,int b,int n1,int n2)
{
   vector<int>del_u;
   vector<int>del_v;
   int minA_v=INT32_MAX;
   vector<bool>delu(n1+1,false);
   vector<bool>delv(n2+1,false);
   vector<vector<int>>edge_mp_u(n1+1);
   vector<vector<int>>edge_mp_v(n2+1);
   for(auto it : edge_u){
      edge_mp_u[it.first]=it.second;
   }
   for(auto it : edge_v){
      edge_mp_v[it.first]=it.second;
   }

   for(const auto&v:v_set)
   {
      if(degree_v[v]<b)
      {
         delv[v]=true;
         del_v.push_back(v);
         for(auto &u:edge_mp_v[v])  //删除u的边，将v的度减一
         { 
            degree_u[u]--;
         }
      }
   }
   for(auto v:del_v) //删除degree[u]<a的点
   {
      v_set.erase(v);
   }
   del_v.clear();
   
   while(!u_set.empty()||!v_set.empty())
   {
      
       minA_v=INT32_MAX;
    for(const auto&u:u_set)
   {
      minA_v=min(minA_v,degree_u[u]);
   }

    for(const auto&u:u_set)
   {
        if(degree_u[u]==minA_v)
        {
         del_u.push_back(u);
         delu[u]=true;
        }
   }
   
   while(!del_v.empty()||!del_u.empty()){
   
      for(int i=0;i<del_u.size();i++){
         int u=del_u[i];
         u_set.erase(del_u[i]);
         for(int i=0;i<edge_mp_u[u].size();i++) 
         {
            int v=edge_mp_u[u][i];
            if(!delv[v])
            {
               degree_v[v]--;
               if(degree_v[v]<b)
               {
                 del_v.push_back(v);
                 delv[v]=true;
               }
            }
         }
         for(int i=1;i<=minA_v;i++)
         {
            if(u_a_bmax[u][i]<b)
            {
               u_a_bmax[u][i]=b;
            }
         }
      }
      del_u.clear();
      for(int i=0;i<del_v.size();i++){
         int v=del_v[i];
         v_set.erase(v);
         v_b_amax[v][b]=minA_v;
         for(int i=0;i<edge_mp_v[v].size();i++) 
         {
             int u=edge_mp_v[v][i];
             if(!delu[u])
             {
               degree_u[u]--;
               if(degree_u[u]<=minA_v)
               {
                 del_u.push_back(u);
                 delu[u]=true;
               }
             }
         }  
      }
      del_v.clear();
   }
   
   }      
 
}
#endif 
