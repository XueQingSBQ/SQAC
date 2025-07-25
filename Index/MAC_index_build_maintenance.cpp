#include "MAC_index_build_maintenance.h"

void MAC_index::creat_MAC_Uindex(string filename)
{
    auto start= chrono::system_clock::now();
         // 获取所有可能的 LABEL_U 和 LABEL_V 的子集组合
    vector<string> label_u_vec(g.LABEL_U.begin(), g.LABEL_U.end());
    vector<string> label_v_vec(g.LABEL_V.begin(), g.LABEL_V.end());
   sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
      return this->g.labelu[a].size() > this->g.labelu[b].size();
   });
    sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
        return this->g.labelv[a].size() > this->g.labelv[b].size();
    });
    for(int i=0;i<label_v_vec.size();i++){
      cout<<label_v_vec[i]<<" "<<g.labelv[label_v_vec[i]].size()<<endl;
    }
     // 生成所有可能的  LABEL_V ，U子集组合
    vector<uint32_t> label_u_combinations =generate_32bit_combinations(label_u_vec);
    vector<uint32_t> label_v_combinations =generate_32bit_combinations(label_v_vec);
   //  savecombination(label_u_combinations,label_u_vec,"label_u_combinations.txt");
   //  savecombination(label_v_combinations,label_v_vec,"label_v_combinations.txt");
    
     int cnt=0;
     unordered_map<uint32_t, node_graph> label_u_bicore;
     //unordered_map<uint32_t, node_graph> vlabel_node_graph;
     

       
      for(auto label_mask : label_u_combinations )
      {
             // 使用 bitset 输出二进制格式
         std::cout << "Binary: " << std::bitset<32>(label_mask) << std::endl;
         node_graph node_data;
         unordered_set<int> update_u_set;
         unordered_set<int> update_v_set;
         vector<string> label_u_combination = mask_to_labels(label_mask, label_u_vec);

         if(__builtin_popcount(label_mask)==1)
         {  
            // 提取当前标签组合的子图
               g.subgraph_Extraction_u_ve(label_u_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
               node_data.degree_u.resize(g.n1+1, 0);
               node_data.degree_v.resize(g.n2+1, 0);

               // 计算度数
               for (auto& it : node_data.edge_mp_u) {
                  node_data.degree_u[it.first] = it.second.size();
               }
               for (auto& it : node_data.edge_mp_v) {
                  node_data.degree_v[it.first] = it.second.size();
               }

               int max_u_degree = 0;
               int max_v_degree = 0;

               for (auto& it : node_data.u_set) {
                  max_u_degree = max(max_u_degree, node_data.degree_u[it]);
               }

               for (auto& it : node_data.v_set) {
                  max_v_degree = max(max_v_degree, node_data.degree_v[it]);
               }

               // 计算子图 ??
               int r = online_calculate_delta(node_data.edge_mp_u,node_data.edge_mp_v ,node_data.u_set, node_data.v_set,g.n1,g.n2);
               unordered_map<int,vector<int>>&u_a_Bmax=node_data.u_a_Bmax;
               unordered_map<int,vector<int>>&v_b_Amax=node_data.v_b_Amax;
               for(auto u:node_data.u_set)
               {
                  u_a_Bmax[u].resize(node_data.degree_u[u]+1,0);
               }
               for(auto v:node_data.v_set)
               {
                  v_b_Amax[v].resize(node_data.degree_v[v]+1,0);
               }
               for(int i=1;i<=r;i++)
               {
                  auto start = chrono::system_clock::now();
                  computeBetaMax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,g.n1,g.n2);
                  auto end= chrono::system_clock::now();
                  cout<<"computeBmax_VE time:"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               }
               for(int i=1;i<=r;i++)
               {
                  auto start = chrono::system_clock::now();
                  computeAlphaMax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,g.n1,g.n2);
                  auto end= chrono::system_clock::now();
                  cout<<"computeAmax_VE time:"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               }    
              
               auto start = chrono::system_clock::now();
               label_u_bicore[label_mask]=node_data;
                auto end= chrono::system_clock::now();
                cout<<"insert_ulabel_node"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               cout<<"end"<<endl;                       
            
         }
         else
         {
            uint32_t label_mask_fa;
            label_mask_fa= get_parent_mask_gcc(label_mask);
             
            node_data= label_u_bicore[label_mask_fa];
            unordered_map< int, vector<int>> edge_add;
            
            //提取添加的边
            int cnt1=0;
            for(auto u:g.labelu[label_u_combination[label_u_combination.size()-1]])
            {
               if(node_data.u_set.find(u)!=node_data.u_set.end())
                  continue;
               for(auto v:g.edges_u[u])
               {

                  edge_add[u].emplace_back(v);
                  cnt1++;
                  if(node_data.u_set.find(u)==node_data.u_set.end())
                     node_data.u_set.insert(u);
                  if(node_data.v_set.find(v)==node_data.v_set.end())
                     node_data.v_set.insert(v);      
                  
               }
            }
            cout<<"edge_add size:"<<cnt1<<endl;
             double time=0 ;
            for(auto u_edge:edge_add)
            {
               for(auto edge:u_edge.second)
               {
                  
                  node_data.edge_mp_u[u_edge.first].emplace_back(edge);
                  node_data.edge_mp_v[edge].emplace_back(u_edge.first);
                  node_data.degree_u[u_edge.first]++;
                  node_data.degree_v[edge]++;
                  time +=update_skyline_index_swap_with_bfs(node_data,u_edge.first,edge,update_u_set,update_v_set);
               
                 
               }
            }
             std::cout << "update_uv_abmax time: " << time/cnt1 << " seconds" << std::endl;
            cout<<"---"<<endl; 
            label_u_bicore[label_mask]=node_data;
         }
         for(auto it:node_data.u_a_Bmax)
            {
                vector<int>& u_a_bmax = it.second;
                int size = u_a_bmax.size()-1;
                int u=it.first;
          
               for(int a=1;a<=size;a++)
               {
                  int b=u_a_bmax[a];
                   insert_u_to_MAC_index(MAC_u_uindex,u,a,b,label_mask);
               }
               
            }
            for(auto it:node_data.v_b_Amax)
            {
                vector<int>& v_b_amax = node_data.v_b_Amax[it.first];
                int size = v_b_amax.size()-1;
                int v=it.first;     
               for(int b=1;b<=size;b++)
               {
                  int a=v_b_amax[b];
                   insert_u_to_MAC_index(MAC_u_vindex,v,b,a,label_mask);
               } 

               
            }

      }          
      

   save_MAC_index("MAC_u_uindex_"+filename+".txt",MAC_u_uindex);
   save_MAC_index("MAC_u_vindex_"+filename+".txt",MAC_u_vindex);
   MAC_index_opt( MAC_u_uindex);
   MAC_index_opt( MAC_u_vindex);
  // cout<<indexu_batch[1][2][2]<<endl;
   auto end= chrono::system_clock::now();
    // 计算时间间隔（以秒为单位，结果是 double 类型）
    std::chrono::duration<double> elapsed_seconds = end - start;
   cout<<"creat_lable_index_opt_batch"<<elapsed_seconds.count()<<"s"<<endl;
}
void MAC_index::creat_MAC_Vindex(string filename)
{
    auto start= chrono::system_clock::now();
         // 获取所有可能的 LABEL_U 和 LABEL_V 的子集组合
    vector<string> label_u_vec(g.LABEL_U.begin(), g.LABEL_U.end());
    vector<string> label_v_vec(g.LABEL_V.begin(), g.LABEL_V.end());
   sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
      return this->g.labelu[a].size() > this->g.labelu[b].size();
   });
    sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
        return this->g.labelv[a].size() > this->g.labelv[b].size();
    });
    for(int i=0;i<label_v_vec.size();i++){
      cout<<label_v_vec[i]<<" "<<g.labelv[label_v_vec[i]].size()<<endl;
    }
     // 生成所有可能的  LABEL_V ，U子集组合
    vector<uint32_t> label_u_combinations =generate_32bit_combinations(label_u_vec);
    vector<uint32_t> label_v_combinations =generate_32bit_combinations(label_v_vec);
   //  savecombination(label_u_combinations,label_u_vec,"label_u_combinations.txt");
   //  savecombination(label_v_combinations,label_v_vec,"label_v_combinations.txt");
    
     int cnt=0;
     unordered_map<uint32_t, node_graph> ulabel_node_graph;
     unordered_map<uint32_t, node_graph> vlabel_node_graph;
     
      for(auto label_mask : label_v_combinations )
      {
         // 使用 bitset 输出二进制格式
         std::cout << "Binary: " << std::bitset<32>(label_mask) << std::endl;
         node_graph node_data;
         unordered_set<int> update_u_set;
         unordered_set<int> update_v_set;
         vector<string> label_v_combination = mask_to_labels(label_mask, label_v_vec);
         
         if(__builtin_popcount(label_mask)==1)
         {  

            // 提取当前标签组合的子图
               g.subgraph_Extraction_v_ve(label_v_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
               node_data.degree_u.resize(g.n1+1, 0);
               node_data.degree_v.resize(g.n2+1, 0);

               // 计算度数
               for (auto& it : node_data.edge_mp_u) {
                  node_data.degree_u[it.first] = it.second.size();
               }
               for (auto& it : node_data.edge_mp_v) {
                  node_data.degree_v[it.first] = it.second.size();
               }

               int max_u_degree = 0;
               int max_v_degree = 0;

               for (auto& it : node_data.u_set) {
                  max_u_degree = max(max_u_degree, node_data.degree_u[it]);
               }

               for (auto& it : node_data.v_set) {
                  max_v_degree = max(max_v_degree, node_data.degree_v[it]);
               }

               // 计算子图 ??
               int r = online_calculate_delta(node_data.edge_mp_u,node_data.edge_mp_v ,node_data.u_set, node_data.v_set,g.n1,g.n2);
               unordered_map<int,vector<int>>&u_a_Bmax=node_data.u_a_Bmax;
               unordered_map<int,vector<int>>&v_b_Amax=node_data.v_b_Amax;
               for(auto u:node_data.u_set)
               {
                  u_a_Bmax[u].resize(node_data.degree_u[u]+1,0);
               }
               for(auto v:node_data.v_set)
               {
                  v_b_Amax[v].resize(node_data.degree_v[v]+1,0);
               }
               for(int i=1;i<=r;i++)
               {
                  auto start = chrono::system_clock::now();
                  computeBetaMax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,g.n1,g.n2);
                  auto end= chrono::system_clock::now();
                  cout<<"computeBmax_VE time:"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               }
               for(int i=1;i<=r;i++)
               {
                  auto start = chrono::system_clock::now();
                  computeAlphaMax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,g.n1,g.n2);
                  auto end= chrono::system_clock::now();
                  cout<<"computeAmax_VE time:"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               }    
              
               auto start = chrono::system_clock::now();
               vlabel_node_graph[label_mask]=node_data;
                auto end= chrono::system_clock::now();
                cout<<"insert_ulabel_node"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               cout<<"end"<<endl;     
                           
            
         }
         else
         {
            uint32_t label_mask_fa;
            label_mask_fa= get_parent_mask_gcc(label_mask);
             
            node_data= vlabel_node_graph[label_mask_fa];
            unordered_map< int, vector<int>> edge_add;
            
            //提取添加的边
            int cnt1=0;
            for(auto v:g.labelv[label_v_combination[label_v_combination.size()-1]])
            {
               if(node_data.v_set.find(v)!=node_data.v_set.end())
                  continue;
               for(auto u:g.edges_v[v])
               {

                  edge_add[v].emplace_back(u);
                  cnt1++;
                  if(node_data.u_set.find(u)==node_data.u_set.end())
                     node_data.u_set.insert(u);
                  if(node_data.v_set.find(v)==node_data.v_set.end())
                     node_data.v_set.insert(v);      
                  
               }
            }
            cout<<"edge_add size:"<<cnt1<<endl;
             double time=0 ;
            for(auto v_edge:edge_add)
            {
               for(auto edge:v_edge.second)
               {
                  
                  node_data.edge_mp_v[v_edge.first].emplace_back(edge);
                  node_data.edge_mp_u[edge].emplace_back(v_edge.first);
                  node_data.degree_v[v_edge.first]++;
                  node_data.degree_u[edge]++;
                  time +=update_skyline_index_swap_with_bfs(node_data,edge,v_edge.first,update_u_set,update_v_set);
               
                 
               }
            }
             std::cout << "update_uv_abmax time: " << time/cnt1 << " seconds" << std::endl;
            cout<<"---"<<endl; 
            vlabel_node_graph[label_mask]=node_data;
         }
         for(auto it:node_data.v_b_Amax)
            {
                vector<int>& v_b_amax = node_data.v_b_Amax[it.first];
                int size = v_b_amax.size()-1;
                int v=it.first;
     
               for(int b=1;b<=size;b++)
               {
                  int a=v_b_amax[b];
                insert_u_to_MAC_index(MAC_v_vindex,v,b,a,label_mask);
  
               } 
  
               
            }
     for(auto it:node_data.u_a_Bmax)
      {
          vector<int>& u_a_bmax = it.second;
          int size = u_a_bmax.size()-1;
          int u=it.first;
    
         for(int a=1;a<=size;a++)
         {
            int b=u_a_bmax[a];
             insert_u_to_MAC_index(MAC_v_uindex,u,a,b,label_mask);
         }
         
      }

      } 


   save_MAC_index("MAC_v_vindex_"+filename+".txt",MAC_v_vindex);
   save_MAC_index("MAC_v_uindex_"+filename+".txt",MAC_v_uindex);
   MAC_index_opt( MAC_v_vindex);
   MAC_index_opt( MAC_v_uindex);
  // cout<<indexu_batch[1][2][2]<<endl;
   auto end= chrono::system_clock::now();
    // 计算时间间隔（以秒为单位，结果是 double 类型）
    std::chrono::duration<double> elapsed_seconds = end - start;
   cout<<"creat_lable_index_opt_batch"<<elapsed_seconds.count()<<"s"<<endl;
}
void MAC_index::MAC_index_opt(unordered_map<uint32_t,unordered_map<int,vector<u_node*> > >&label_index_batch) {

   for (auto& label_u_pair : label_index_batch) {
      uint32_t label_mask = label_u_pair.first;
      for (auto& a_pair : label_u_pair.second) {
         int a = a_pair.first;
         vector<u_node*>& u_node_list = a_pair.second;
         int size = u_node_list.size() - 1;
         for (int b = size - 1; b >= 1; b--) {
            if (u_node_list[b] == nullptr) {
               u_node_list[b] = u_node_list[b + 1];
            }
            else {
               u_node_list[b]->next = u_node_list[b + 1];
            }
         }
         
      }
   }
}
void MAC_index:: insert_u_to_MAC_index(unordered_map<uint32_t,unordered_map<int,vector<u_node*> > >&label_index_batch,int u,int a,int b,uint32_t label_mask){
   vector<u_node*>& u_node_list=label_index_batch[label_mask][a];
   if(u_node_list.size()<=b){
      u_node_list.resize(b+1,nullptr);
   }
   int flag=0;
   for(auto &it1: label_index_batch){
      uint32_t label_mask_son=it1.first;
      if((label_mask_son & label_mask)!=label_mask_son || label_mask_son==label_mask){
         continue;
      }
      
      if(it1.second.find(a)!=it1.second.end()){
         vector<u_node*>& u_node_list=it1.second[a];
         if(u_node_list.size()>b&&u_node_list[b]!=nullptr){
            if(u_node_list[b]->uset.find(u)!=u_node_list[b]->uset.end()){
               flag=1;
               break;
            }
         }
      }

   }
   if(flag==1){
      return;
   }
   if(u_node_list[b]==nullptr){
      u_node_list[b]= new u_node;
   }
   u_node *u_node_ptr = u_node_list[b];
   u_node_ptr->uset.insert(u);

}
void MAC_index:: save_MAC_index(const string& filename, const unordered_map<uint32_t, unordered_map<int, vector<u_node*>>>& label_index_batch) {
   ofstream file(filename);
   
   auto write_u_vector = [&](const vector<u_node*>& vec) {
       file << "[";
       for (size_t i = 0; i < vec.size(); ++i) {
           if (vec[i]) {
               file << "{";
               for (auto& u : vec[i]->uset) {
                   file << u << ",";
               }
               if (!vec[i]->uset.empty()) file.seekp(-1, ios_base::cur);
               file << "}";
           } else {
               file << "null";
           }
           if (i != vec.size()-1) file << "|";
       }
       file << "]";
   };

   for (auto& [mask, imap] : label_index_batch) {
       file << mask << ":{";
       for (auto& [key, vec] : imap) {
           file << key << ":";
           write_u_vector(vec);
           file << ";";
       }
       if (!imap.empty()) file.seekp(-1, ios_base::cur);
       file << "}\n";
   }

   file.close();
}
// 加载 label_uindex_batch
void MAC_index::load_MAC_index(const string& filename, unordered_map<uint32_t,unordered_map<int, vector<u_node*>>>& label_index_batch) {
   ifstream file(filename);
   string line;

   auto parse_u_vector = [](const string& data) -> vector<u_node*> {
       vector<u_node*> vec;
       if (data.front() != '[' || data.back() != ']') return vec;
       
       istringstream ss(data.substr(1, data.size()-2));
       string item;
       while (getline(ss, item, '|')) {
           if (item == "null") {
               vec.push_back(nullptr);
           } else {
               auto* node = new u_node;
               istringstream ls(item.substr(1, item.size()-2));
               string num;
               while (getline(ls, num, ',')) {
                   node->uset.insert(stoi(num));
               }
               vec.push_back(node);
           }
       }
       return vec;
   };

   while (getline(file, line)) {
       size_t colon_pos = line.find(':');
       if (colon_pos == string::npos) continue;

       string key_part = line.substr(0, colon_pos);
       string value_part = line.substr(colon_pos+1);

       uint32_t mask = stoul(key_part);
       auto& imap = label_index_batch[mask];
       
       size_t brace_pos = value_part.find('{');
       if (brace_pos == string::npos) continue;
       
       istringstream entries(value_part.substr(brace_pos+1,value_part.size()-2));
       string entry;
       while (getline(entries, entry, ';')) {
           size_t inner_colon = entry.find(':');
           if (inner_colon == string::npos) continue;
           
           int inner_key = stoi(entry.substr(0, inner_colon));
           imap[inner_key] = parse_u_vector(entry.substr(inner_colon+1));
       }
   }

   file.close();
}

double MAC_index:: MAC_query (unordered_set<string>&LU,unordered_set<string>&LV ,int a,int b)
    {
      
      uint32_t u_mask=0;
      uint32_t v_mask=0;
      g.labels_to_mask(LU,LV,u_mask, v_mask);
       auto start= chrono::system_clock::now();
       unordered_map<int,vector<int>> edge_mp_u;
       unordered_map<int,vector<int>> edge_mp_v;
       unordered_set<int>tmp_u_set;
       unordered_set<int>tmp_v_set;
       unordered_set<int>u_set;
       unordered_set<int>v_set;
       vector<int>u1_set;
       vector<int>v1_set;
       
       for(auto &it1:MAC_u_uindex){
         uint32_t mask=it1.first;
         if((mask&u_mask)!=mask){
            continue;
         }

         if(it1.second.find(a)!=it1.second.end()){
            vector<u_node*>& u_label_nodes=it1.second[a];
            if(u_label_nodes.size()>b){
               u_node* u_label_node=u_label_nodes[b];
               while(u_label_node!=nullptr){
                  unordered_set<int>& uset=u_label_node->uset;
                  for(auto u:uset){
                      if(tmp_u_set.find(u)==tmp_u_set.end()){
                        tmp_u_set.insert(u);
                      }
                  }
                  u_label_node=u_label_node->next;

               }
              
            }
         }
       }
       for(auto &it1:MAC_v_uindex){
         uint32_t mask=it1.first;
         if((mask&v_mask)!=mask){
            continue;
         }

         if(it1.second.find(a)!=it1.second.end()){
            vector<u_node*>& u_label_nodes=it1.second[a];
            if(u_label_nodes.size()>b){
               u_node* u_label_node=u_label_nodes[b];
               while(u_label_node!=nullptr){
                  unordered_set<int>& uset=u_label_node->uset;
                  for(auto u:uset){
                      if(tmp_u_set.find(u)!=tmp_u_set.end()){
                        u_set.insert(u);
                      }
                  }
                  u_label_node=u_label_node->next;

               }
              
            }
         }
       }
       for(auto &it2:MAC_v_vindex){
         uint32_t mask=it2.first;
         if((mask&v_mask)!=mask){
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
                      if(tmp_v_set.find(v)==tmp_v_set.end()){
                        tmp_v_set.insert(v);
                      }
                  }
                  v_label_node=v_label_node->next;
               }
               
            }
         }
       }
       for(auto &it2:MAC_u_vindex){
         uint32_t mask=it2.first;
         if((mask&u_mask)!=mask){
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
                      if(tmp_v_set.find(v)!=tmp_v_set.end()){
                          v_set.insert(v);
                      }
                  }
                  v_label_node=v_label_node->next;
               }
               
            }
         }
       }
      // cout<<"v_set size:"<<v_set.size()<<endl;
       for(auto u:u_set){
         for(auto v:g.edges_u[u]){
            if(v_set.find(v)!=v_set.end()){
               edge_mp_u[u].emplace_back(v);
               edge_mp_v[v].emplace_back(u);
            }
         }
       }

       vector<int>delu(g.n1+1,0);
       vector<int>delv(g.n2+1,0);
       vector<int>degree_u(g.n1+1,0);
       vector<int>degree_v(g.n2+1,0);
       for(auto &it:edge_mp_u)
       {
          degree_u[it.first]=it.second.size();
       }
       for(auto &it:edge_mp_v)
       {
          degree_v[it.first]=it.second.size();
       }
        
       while(!u_set.empty()||!v_set.empty())
       {    
           vector< int>temp_u_set;
           vector< int>temp_v_set;
              
             for(auto &u:u_set)
             {
              
               if(degree_u[u]<a)  //degree[u]<a
               {
                 delu[u]=1;       //删除标zhi符
                 for(auto v:edge_mp_u[u])  //删除u的边，将v的度减一
                 {  if(delv[v]==0)
                    degree_v[v]--;
                 }
                 temp_u_set.emplace_back(u); //添加要删除degree[u]<a的点
               }
             }
             
             for(auto &v:v_set)
             {
               if(degree_v[v]<b) //degree[v]<b
               {
                 delv[v]=1;  //删除标志符=1
                 for(auto u:edge_mp_v[v]) //删除v的边，将u的度减一
                 {  if(delu[u]==0)
                    degree_u[u]--;
                 }
                  temp_v_set.emplace_back(v);//添加要删除degree[v]<b的点
               }
             }
             //cout<<"du461:"<<degree[461]<<" "<<degree[464]<<endl;
              for(auto u:temp_u_set) //删除degree[u]<a的点
             {
             //cout<<u<<" "<<degree[u]<<endl;
                u_set.erase(u);
              }
              for(auto v:temp_v_set)
              {
                //cout<<v<<" "<<degree[v]<<endl;
                v_set.erase(v); //删除degree[v]<b的点
              }

             if(temp_u_set.size()+temp_v_set.size()==0)
             {
              break; // 没有需要删除的点，退出循环
             }

       }
       auto end= chrono::system_clock::now();
       std::chrono::duration<double> elapsed_seconds = end - start;
      
      //  cout<<"("<<a<<","<<b<<")-core:\n";
      //  cout<<"u:\n";
      //  vector< int>temp_u_set={u_set.begin(),u_set.end()};
      //  sort(temp_u_set.begin(),temp_u_set.end());
      //  vector< int>temp_v_set={v_set.begin(),v_set.end()};
      //  sort(temp_v_set.begin(),temp_v_set.end());
      //  for(auto u:temp_u_set)
      //  {
      //     cout<<u<<" ";
      //  }
      //  cout<<endl;
      //  cout<<"v:\n";
      //  for(auto v:temp_v_set)
      //  {
      //     cout<<v<<" ";
      //  }
      //  cout<<endl;
      //  cout<<"lable_index_batch_opt_quiry"<<elapsed_seconds.count()<<"s"<<endl;
       return elapsed_seconds.count();
 


    }
void  MAC_index::delete_u_to_MAC(unordered_map<uint32_t,unordered_map<int,vector<u_node*> > >&label_index_batch,int u,int a,uint32_t label_mask){
   vector<u_node*>& u_node_list=label_index_batch[label_mask][a];
   int b=0;
   int flag=0;
   for(int i=0;i<u_node_list.size();i++){
      if(u_node_list[i]!=nullptr){
         if(u_node_list[i]->uset.find(u)!=u_node_list[i]->uset.end()){
            u_node_list[i]->uset.erase(u);
            flag=1;
            b=i;
            break;
         }
      }
   }
}

graphinc MAC_index::online_core(int q,unordered_set<int> &u_set,unordered_set<int> &v_set, unordered_map< int,vector<int>> &edge_u,unordered_map< int,vector< int>> &edge_v,int a,int b){
         graphinc graph;
         vector<int>delu(g.n1+1,0);
         vector<int>delv(g.n2+1,0);
         vector<int>degree_u(g.n1+1,0);
         vector<int>degree_v(g.n2+1,0);
         for(auto &it: edge_u)
         {
            degree_u[it.first]=it.second.size();
         }
         for(auto &it:edge_v)
         {
            degree_v[it.first]=it.second.size();
         }
          
         while(!u_set.empty()||!v_set.empty())
         {    
            if(u_set.find(q)==u_set.end()){
               return graph;
            }
             vector< int>temp_u_set;
             vector< int>temp_v_set;
                
               for(auto &u:u_set)
               {
                
                 if(degree_u[u]<a)  //degree[u]<a
                 {
                   delu[u]=1;       //删除标zhi符
                   for(auto v:edge_u[u])  //删除u的边，将v的度减一
                   {  if(delv[v]==0)
                      degree_v[v]--;
                   }
                   temp_u_set.emplace_back(u); //添加要删除degree[u]<a的点
                 }
               }
               
               for(auto &v:v_set)
               {
                 if(degree_v[v]<b) //degree[v]<b
                 {
                   delv[v]=1;  //删除标志符=1
                   for(auto u:edge_v[v]) //删除v的边，将u的度减一
                   {  if(delu[u]==0)
                      degree_u[u]--;
                   }
                    temp_v_set.emplace_back(v);//添加要删除degree[v]<b的点
                 }
               }
               //cout<<"du461:"<<degree[461]<<" "<<degree[464]<<endl;
                for(auto u:temp_u_set) //删除degree[u]<a的点
               {
               //cout<<u<<" "<<degree[u]<<endl;
                  u_set.erase(u);
                }
                for(auto v:temp_v_set)
                {
                  //cout<<v<<" "<<degree[v]<<endl;
                  v_set.erase(v); //删除degree[v]<b的点
                }

               if(temp_u_set.size()+temp_v_set.size()==0)
               {
                break; // 没有需要删除的点，退出循环
               }

         }
         graph.u_set=u_set;
         graph.v_set=v_set;

         return graph;

   }


  void MAC_index::MAC_to_bicore(){
   vector<string> label_u_vec(g.LABEL_U.begin(), g.LABEL_U.end());
   vector<string> label_v_vec(g.LABEL_V.begin(), g.LABEL_V.end());
   sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
      return this->g.labelu[a].size() > this->g.labelu[b].size();
   });
    sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
        return this->g.labelv[a].size() > this->g.labelv[b].size();
    });
    //swap(label_u_vec[1],label_u_vec[2]);
    for(int i=0;i<label_u_vec.size();i++){
      cout<<label_u_vec[i]<<" "<<g.labelu[label_u_vec[i]].size()<<endl;

    }
     // 生成所有可能的  LABEL_V ，U子集组合
    vector<uint32_t> label_u_combinations =generate_32bit_combinations(label_u_vec);
    vector<uint32_t> label_v_combinations =generate_32bit_combinations(label_v_vec);

    for(int i=0;i<label_u_combinations.size();i++){
      
      uint32_t mask=label_u_combinations[i];
      cout<<mask<<endl;
      node_graph node_data;
      vector<string> label_u_combination = mask_to_labels(mask, label_u_vec);
      g.subgraph_Extraction_u_ve(label_u_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
      node_data.degree_u.resize(g.n1+1, 0);
      node_data.degree_v.resize(g.n2+1, 0);

      // 计算度数
      for (auto& it : node_data.edge_mp_u) {
         node_data.degree_u[it.first] = it.second.size();
      }
      for (auto& it : node_data.edge_mp_v) {
         node_data.degree_v[it.first] = it.second.size();
      }
      //计算前置节点的Bmax和Amax
      for (int i = 0; i < 32; i++) {
         if (mask & (1U << i)) {
             uint32_t fa_mask = mask & ~(1U << i); // 清除第i位
             if(fa_mask == 0){
                 break;
             }
             unordered_map<int,vector<int>>&temp_u_a_Bmax=g.ulabel_bicore[fa_mask].u_a_Bmax;
             unordered_map<int,vector<int>>&temp_v_b_Amax=g.ulabel_bicore[fa_mask].v_b_Amax;
             for(auto& it:temp_u_a_Bmax){
                int u=it.first;
                vector<int>& a_b=it.second;
                int max_a= a_b.size()-1;
                if(node_data.u_a_Bmax[u].size()<=max_a){
                    node_data.u_a_Bmax[u].resize(max_a+1);
                }
                for(int a=1;a<=max_a;a++){
                  if(node_data.u_a_Bmax[u][a]<a_b[a]){
                    node_data.u_a_Bmax[u][a]=a_b[a];
                  }
                }
             }
             for(auto it:temp_v_b_Amax){
                int v=it.first;
                vector<int>& b_a=it.second;
                int max_b= b_a.size()-1;
                if(node_data.v_b_Amax[v].size()<=max_b){
                    node_data.v_b_Amax[v].resize(max_b+1);
                }
                for(int b=1;b<=max_b;b++){
                    if(node_data.v_b_Amax[v][b]<b_a[b]){
                        node_data.v_b_Amax[v][b]=b_a[b];
                    }
                }
             }
         }
      }
      //cout<<"node_data.u_a_Bmax"<<endl;
      unordered_map<int,vector<int>> temp_u_a_Bmax;
      unordered_map<int,vector<int>> temp_v_b_Amax;
      unordered_map<int,vector<u_node*>> temp_u_bicore =MAC_u_uindex[mask];
      unordered_map<int,vector<u_node*>> temp_v_bicore =MAC_u_vindex[mask];
      for(auto & it:temp_u_bicore){
         int a=it.first;
         vector<u_node*> &b_usets=it.second;
         int b_max=b_usets.size()-1;
         u_node* fa=b_usets[b_max];

         for(int b=b_max;b>=1;b--){
            if((b!=b_max&&fa==b_usets[b]) || b_usets[b]==NULL){
               continue;
            }
            //cout<<a<<"b"<<b<<" "<<fa<<" "<<b_usets[b]<<endl;
            unordered_set<int>&uset=b_usets[b]->uset;
            for(auto &u:uset){
                if(node_data.u_a_Bmax[u].size()<=a){
                  node_data.u_a_Bmax[u].resize(a+1);
                }
                if(node_data.u_a_Bmax[u][a]<b){
                  node_data.u_a_Bmax[u][a]=b;
                }
            } 
            fa=b_usets[b];
         }
      }
     // cout<<"node_data.v_b_Amax"<<endl;
      for(auto & it:temp_v_bicore){
         int b=it.first;
         vector<u_node*> &a_vsets=it.second;
         int a_max=a_vsets.size()-1;
         u_node* fa=a_vsets[a_max];

         for(int a=a_max;a>=1;a--){
            if((a!=a_max&&fa==a_vsets[a])|| a_vsets[a]==NULL){
               continue;
            }
            unordered_set<int>&vset=a_vsets[a]->uset;
            for(auto &v:vset){
                if(node_data.v_b_Amax[v].size()<=b){
                  node_data.v_b_Amax[v].resize(b+1);
                }
                if(node_data.v_b_Amax[v][b]<a){
                  node_data.v_b_Amax[v][b]=a;
                }
            }
            fa=a_vsets[a];
            
         }

      }
      g.ulabel_bicore[mask]=node_data;
    }


    for(int i=0;i<label_v_combinations.size();i++){
      
      uint32_t mask=label_v_combinations[i];
      cout<<mask<<endl;
      node_graph node_data;
      vector<string> label_v_combination = mask_to_labels(mask, label_v_vec);
      g.subgraph_Extraction_v_ve(label_v_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
      node_data.degree_u.resize(g.n1+1, 0);
      node_data.degree_v.resize(g.n2+1, 0);

      // 计算度数
      for (auto& it : node_data.edge_mp_u) {
         node_data.degree_u[it.first] = it.second.size();
      }
      for (auto& it : node_data.edge_mp_v) {
         node_data.degree_v[it.first] = it.second.size();
      }
      //计算前置节点的Bmax和Amax
      for (int i = 0; i < 32; i++) {
         if (mask & (1U << i)) {
             uint32_t fa_mask = mask & ~(1U << i); // 清除第i位
             if(fa_mask == 0){
                 break;
             }
             unordered_map<int,vector<int>>&temp_u_a_Bmax=g.vlabel_bicore[fa_mask].u_a_Bmax;
             unordered_map<int,vector<int>>&temp_v_b_Amax=g.vlabel_bicore[fa_mask].v_b_Amax;
             cout<<"fa_mask"<<fa_mask<<endl;
             for(auto& it:temp_u_a_Bmax){
                int u=it.first;
                vector<int>& a_b=it.second;
                int max_a= a_b.size()-1;
                if(node_data.u_a_Bmax[u].size()<=max_a){
                    node_data.u_a_Bmax[u].resize(max_a+1);
                }
                for(int a=1;a<=max_a;a++){
                  if(node_data.u_a_Bmax[u][a]<a_b[a]){
                    node_data.u_a_Bmax[u][a]=a_b[a];
                  }
                }
             }
             for(auto it:temp_v_b_Amax){
                int v=it.first;
                vector<int>& b_a=it.second;
                int max_b= b_a.size()-1;
                if(node_data.v_b_Amax[v].size()<=max_b){
                    node_data.v_b_Amax[v].resize(max_b+1);
                }
                for(int b=1;b<=max_b;b++){
                    if(node_data.v_b_Amax[v][b]<b_a[b]){
                        node_data.v_b_Amax[v][b]=b_a[b];
                    }
                }
             }
         }
      }
      cout<<"node_data.u_a_Bmax"<<endl;
      unordered_map<int,vector<int>> temp_u_a_Bmax;
      unordered_map<int,vector<int>> temp_v_b_Amax;
      unordered_map<int,vector<u_node*>> temp_u_bicore =MAC_v_uindex[mask];
      unordered_map<int,vector<u_node*>> temp_v_bicore =MAC_v_vindex[mask];
      for(auto & it:temp_u_bicore){
         int a=it.first;
         vector<u_node*> &b_usets=it.second;
         int b_max=b_usets.size()-1;
         u_node* fa=b_usets[b_max];

         for(int b=b_max;b>=1;b--){
            if((b!=b_max&&fa==b_usets[b]) || b_usets[b]==NULL){
               continue;
            }
            //cout<<a<<"b"<<b<<" "<<fa<<" "<<b_usets[b]<<endl;
            unordered_set<int>&uset=b_usets[b]->uset;
            for(auto &u:uset){
                if(node_data.u_a_Bmax[u].size()<=a){
                  node_data.u_a_Bmax[u].resize(a+1);
                }
                if(node_data.u_a_Bmax[u][a]<b){
                  node_data.u_a_Bmax[u][a]=b;
                }
            } 
            fa=b_usets[b];
         }
      }
      cout<<"node_data.v_b_Amax"<<endl;
      for(auto & it:temp_v_bicore){
         int b=it.first;
         vector<u_node*> &a_vsets=it.second;
         int a_max=a_vsets.size()-1;
         u_node* fa=a_vsets[a_max];

         for(int a=a_max;a>=1;a--){
            if((a!=a_max&&fa==a_vsets[a])|| a_vsets[a]==NULL){
               continue;
            }
            unordered_set<int>&vset=a_vsets[a]->uset;
            for(auto &v:vset){
                if(node_data.v_b_Amax[v].size()<=b){
                  node_data.v_b_Amax[v].resize(b+1);
                }
                if(node_data.v_b_Amax[v][b]<a){
                  node_data.v_b_Amax[v][b]=a;
                }
            }
            fa=a_vsets[a];
            
         }

      }
      g.vlabel_bicore[mask]=node_data;
    }
   }
vector<vector<string>> filter_combinations(
       vector<vector<string>>& combinations,
       vector<vector<string>>& delete_combinations) {
  
      set<vector<string>> delete_set;
  
      // 对 delete_combinations 里的每一个组合排序并加入 set
      for (auto comb : delete_combinations) {
          delete_set.insert(comb);
      }
  
      vector<vector<string>> result;
  
      for (auto comb : combinations) {
          if (delete_set.find(comb) == delete_set.end()) {
              result.push_back(comb);
          }
      }
  
      return result;
  }
   double MAC_index:: Edge_add_MAC_maintenance(string filename){
      auto start= chrono::system_clock::now();
        unordered_map< int, vector<int>> u_edge_add;
        unordered_map< int, vector<int>> v_edge_add;
        unordered_map<int,unordered_set<string>> u_label;
        unordered_map<int,unordered_set<string>> v_label;
        unordered_set<string> LU,LV;
        vector<string> label_u_vec(g.LABEL_U.begin(), g.LABEL_U.end());
        vector<string> label_v_vec(g.LABEL_V.begin(), g.LABEL_V.end());
        // 按顶点集合大小排序（降序）
       sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
          return this->g.labelu[a].size() > this->g.labelu[b].size();
       });
        sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
            return this->g.labelv[a].size() > this->g.labelv[b].size();
        });
        unordered_map<string,uint32_t> label_mask_u;
        unordered_map<string,uint32_t> label_mask_v;
        vector<string> lABEL_U(g.LABEL_U.begin(),g.LABEL_U.end()),lABEL_V(g.LABEL_V.begin(),g.LABEL_V.end());
        label_mask_u=generate_32bit_map_ucombinations(lABEL_U,g.labelu);
        label_mask_v=generate_32bit_map_vcombinations(lABEL_V,g.labelv);
        {
         ifstream file(filename);
         string line;
         if(!file.is_open())
         {
             cout << "Failed to open file!" << endl;
             return 0;
         }
 
         int u,v;
         string tempulabel,tempvlabel;
         int cnt=1;
 
         while(file.good()&&!file.eof())
         {   cnt++;
             char ulabel_temp[100];  // 创建一个临时的字符数组
             char vlabel_temp[100];  // 创建另一个临时的字符数组
             getline(file,line);
             if(line.empty()) continue;
             sscanf(line.c_str(), "%d %d %s %s", &u, &v, ulabel_temp, vlabel_temp);
             tempulabel = ulabel_temp;
             tempvlabel = vlabel_temp;
             if(u>=g.ulabel.size())
             {
                 g.ulabel.resize(u+1);
             }
             if(v>=g.vlabel.size())
             {
                 g.vlabel.resize(v+1);
             }
             std::stringstream ss_u(tempulabel);  // 对v的标签字符串进行分割
             std::string token_u;
             while (std::getline(ss_u, token_u, '|')) {
             g.ulabel[u].insert(token_u);  // 将每个标签插入v的标签集合
             
             LU.insert(token_u);
             g.labelu[token_u].insert(u);  // 将v插入到该标签的反向映射集合
             if(g.LABEL_U.find(token_u) == g.LABEL_U.end()) {
                g.LABEL_U.insert(token_u);  // 将标签加入总集合
              }              
             }
 
             std::stringstream ss_v(tempvlabel);  // 对v的标签字符串进行分割
             std::string token_v;
             while (std::getline(ss_v, token_v, '|')) {
             g.vlabel[v].insert(token_v);  // 将每个标签插入v的标签集合
             LV.insert(token_v);
             g.labelv[token_v].insert(v);  // 将v插入到该标签的反向映射集合
             if(g.LABEL_V.find(token_v) == g.LABEL_V.end()) {
                g.LABEL_V.insert(token_v);  // 将标签加入总集合
              }
             }
          // 添加边
             if(u>=g.edges_u.size())
             {
                 g.edges_u.resize(u+1);
             }
             if(v>=g.edges_v.size())
             {
                 g.edges_v.resize(v+1);
             }
            u_edge_add[u].emplace_back(v);
            v_edge_add[v].emplace_back(u);
            g.edges_u[u].emplace_back(v);
            g.edges_v[v].emplace_back(u);
            g.m++;
          }
          for(auto u:u_edge_add){
            u_label[u.first]=g.ulabel[u.first];
          }
          for(auto v:v_edge_add){
            v_label[v.first]=g.vlabel[v.first];
          }
     
           file.close();
        }

        vector<string> ulabel_delete_vec;
        vector<string> vlabel_delete_vec;
        for(auto u:g.LABEL_U){
            if(LU.find(u)==LU.end()){
               ulabel_delete_vec.push_back(u);
            }
        }
        for(auto v:g.LABEL_V){
            if(LV.find(v)==LV.end()){
               vlabel_delete_vec.push_back(v);
            }
        }


        sort(ulabel_delete_vec.begin(), ulabel_delete_vec.end(), [this](const string& a, const string& b) {
         return this->g.labelu[a].size() > this->g.labelu[b].size();
      });
       sort(vlabel_delete_vec.begin(), vlabel_delete_vec.end(), [this](const string& a, const string& b) {
           return this->g.labelv[a].size() > this->g.labelv[b].size();
       });

         // 生成所有可能的  LABEL_V ，U子集组合
        vector<vector<string>> label_u_combinations =generate_combinations_1(label_u_vec);
        vector<vector<string>> label_v_combinations =generate_combinations_1(label_v_vec);
        vector<vector<string>> label_u_delete_combinations =generate_combinations_1(ulabel_delete_vec);
        vector<vector<string>> label_v_delete_combinations =generate_combinations_1(vlabel_delete_vec);
        label_u_combinations=filter_combinations(label_u_combinations,label_u_delete_combinations);
        label_v_combinations=filter_combinations(label_v_combinations,label_v_delete_combinations);

        
      
        for(int i=0;i<label_u_combinations.size();i++){
         
            vector<string> label_u_combination = label_u_combinations[i];
            for(int j=0;j<label_u_combination.size();j++){
               cout<<label_u_combination[j]<<" ";
            }
            cout<<endl;
            uint32_t mask_u=0;
            for(int j=0;j<label_u_combination.size();j++){
                mask_u=mask_u|label_mask_u[label_u_combination[j]];
            }
            cout<<mask_u<<endl;

            node_graph& node_data=g.ulabel_bicore[mask_u];

            double time=0 ;
            int cnt1=0;
            unordered_map<int,vector<int>> edge_add;
            for(auto it :u_edge_add){
               int u=it.first;
               int flag=0;
               for(auto label:u_label[u]){
                  if((label_mask_u[label]& mask_u)!=0){
                     flag=1;
                     break;
                  }
               }
               if(flag==1){
                  edge_add[u]=it.second;
                  cnt1+=it.second.size();
               }
            }
            unordered_set<int> update_u_set;
            unordered_set<int> update_v_set;
            for(auto u_edge:edge_add)
            {
               for(auto edge:u_edge.second)
               {
                  
                  node_data.edge_mp_u[u_edge.first].emplace_back(edge);
                  node_data.edge_mp_v[edge].emplace_back(u_edge.first);
                  node_data.degree_u[u_edge.first]++;
                  node_data.degree_v[edge]++;
                  time +=update_skyline_index_swap_with_bfs(node_data,u_edge.first,edge,update_u_set,update_v_set); 
               }
            }
             std::cout << "update_uv_abmax time: " << time/cnt1 << " seconds" << std::endl;
             //MAC_u_uindex[mask_u].clear();
             //MAC_u_vindex[mask_u].clear();
             for(auto it:update_u_set)
             {   int u=it;
                 vector<int>& u_a_bmax = node_data.u_a_Bmax[u];
                 int size = u_a_bmax.size()-1;
                 //int u=it.first;
           
                for(int a=1;a<=size;a++)
                {
                   int b=u_a_bmax[a];
                    delete_u_to_MAC(MAC_u_uindex,u,a,mask_u);
                    insert_u_to_MAC_index(MAC_u_uindex,u,a,b,mask_u);
                }
                
             }
             for(auto it:update_v_set)
             {    int v=it;    
                 vector<int>& v_b_amax = node_data.v_b_Amax[v];
                 int size = v_b_amax.size()-1;
                  
                for(int b=1;b<=size;b++)
                {
                   int a=v_b_amax[b];
                   delete_u_to_MAC(MAC_u_vindex,v,b,mask_u);
                   insert_u_to_MAC_index(MAC_u_vindex,v,b,a,mask_u);
                }  
             }

        }
        //v
        for(int i=0;i<label_v_combinations.size();i++){
         vector<string> label_v_combination = label_v_combinations[i];
         uint32_t mask_v=0;
         for(int j=0;j<label_v_combination.size();j++){
             mask_v=mask_v|label_mask_v[label_v_combination[j]];
         }

         node_graph &node_data=g.vlabel_bicore[mask_v];
         double time=0 ;
         int cnt1=0;
         unordered_map<int,vector<int>> edge_add;
         for(auto it :v_edge_add){
            int v=it.first;
            int flag=0;
            for(auto label:v_label[v]){
               if((label_mask_v[label]& mask_v)!=0){
                  flag=1;
                  break;
               }
            }
            if(flag==1){
               edge_add[v]=it.second;
               cnt1+=it.second.size();
            }
         }
         unordered_set<int> update_u_set;
         unordered_set<int> update_v_set;
         for(auto v_edge:edge_add)
         {
            for(auto edge:v_edge.second)
            {
               
               node_data.edge_mp_v[v_edge.first].emplace_back(edge);
               node_data.edge_mp_u[edge].emplace_back(v_edge.first);
               node_data.degree_v[v_edge.first]++;
               node_data.degree_u[edge]++;
               time +=update_skyline_index_swap_with_bfs(node_data,edge,v_edge.first,update_u_set,update_v_set);
            
              
            }
         }
          std::cout << "update_uv_abmax time: " << time/cnt1 << " seconds" << std::endl;
          //MAC_v_uindex[mask_v].clear();
          //MAC_v_vindex[mask_v].clear();
          for(auto it: update_v_set )
          {
              vector<int>& v_b_amax = node_data.v_b_Amax[it];
              int size = v_b_amax.size()-1;
              int v=it;
   
             for(int b=1;b<=size;b++)
             {
                int a=v_b_amax[b];
                delete_u_to_MAC(MAC_v_vindex,v,b,mask_v);
               insert_u_to_MAC_index(MAC_v_vindex,v,b,a,mask_v);

             } 

             
          }
         for(auto it:update_u_set)
         {
            int u=it;
            vector<int>& u_a_bmax = node_data.u_a_Bmax[u];
            int size = u_a_bmax.size()-1;
            
      
            for(int a=1;a<=size;a++)
            {
               int b=u_a_bmax[a];
               delete_u_to_MAC(MAC_v_uindex,u,a,mask_v);
               insert_u_to_MAC_index(MAC_v_uindex,u,a,b,mask_v);
            }
            
         }

     }
     auto end= chrono::system_clock::now();
     std::chrono::duration<double> elapsed_seconds = end - start;
     save_MAC_index("MAC_v_vindex_1.txt",MAC_v_vindex);
     save_MAC_index("MAC_v_uindex_1.txt",MAC_v_uindex);
     save_MAC_index("MAC_u_vindex_1.txt",MAC_u_vindex);
     save_MAC_index("MAC_u_uindex_1.txt",MAC_u_uindex);
     MAC_index_opt( MAC_v_vindex);
     MAC_index_opt( MAC_v_uindex);
     MAC_index_opt( MAC_u_vindex);
     MAC_index_opt( MAC_u_uindex);
     return elapsed_seconds.count();
   }
   void MAC_index::MAC_to_bicore_delete(){
      vector<string> label_u_vec(g.LABEL_U.begin(), g.LABEL_U.end());
      vector<string> label_v_vec(g.LABEL_V.begin(), g.LABEL_V.end());
      sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
         return this->g.labelu[a].size() > this->g.labelu[b].size();
      });
      
       sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
           return this->g.labelv[a].size() > this->g.labelv[b].size();
       });
       swap(label_u_vec[1],label_u_vec[2]);
       for(int i=0;i<label_u_vec.size();i++){
         cout<<label_u_vec[i]<<" "<<g.labelu[label_u_vec[i]].size()<<endl;
   
       }
        // 生成所有可能的  LABEL_V ，U子集组合
       vector<uint32_t> label_u_combinations =generate_32bit_combinations(label_u_vec);
       vector<uint32_t> label_v_combinations =generate_32bit_combinations(label_v_vec);
   
       for(int i=0;i<label_u_combinations.size();i++){
         
         uint32_t mask=label_u_combinations[i];
         cout<<mask<<endl;
         node_graph node_data;
         vector<string> label_u_combination = mask_to_labels(mask, label_u_vec);
         g.subgraph_Extraction_u_ve(label_u_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
         node_data.degree_u.resize(g.n1+1, 0);
         node_data.degree_v.resize(g.n2+1, 0);
   
         // 计算度数
         for (auto& it : node_data.edge_mp_u) {
            node_data.degree_u[it.first] = it.second.size();
         }
         for (auto& it : node_data.edge_mp_v) {
            node_data.degree_v[it.first] = it.second.size();
         }
         //计算前置节点的Bmax和Amax
         for (int i = 0; i < 32; i++) {
            if (mask & (1U << i)) {
                uint32_t fa_mask = mask & ~(1U << i); // 清除第i位
                if(fa_mask == 0){
                    break;
                }
                unordered_map<int,vector<int>>&temp_u_a_Bmax=g.ulabel_bicore[fa_mask].u_a_Bmax;
                unordered_map<int,vector<int>>&temp_v_b_Amax=g.ulabel_bicore[fa_mask].v_b_Amax;
                cout<<"fa_mask"<<fa_mask<<endl;
                for(auto& it:temp_u_a_Bmax){
                   int u=it.first;
                   vector<int>& a_b=it.second;
                   int max_a= a_b.size()-1;
                   if(node_data.u_a_Bmax[u].size()<=max_a){
                       node_data.u_a_Bmax[u].resize(max_a+1);
                   }
                   for(int a=1;a<=max_a;a++){
                     if(node_data.u_a_Bmax[u][a]<a_b[a]){
                       node_data.u_a_Bmax[u][a]=a_b[a];
                     }
                   }
                }
                for(auto it:temp_v_b_Amax){
                   int v=it.first;
                   vector<int>& b_a=it.second;
                   int max_b= b_a.size()-1;
                   if(node_data.v_b_Amax[v].size()<=max_b){
                       node_data.v_b_Amax[v].resize(max_b+1);
                   }
                   for(int b=1;b<=max_b;b++){
                       if(node_data.v_b_Amax[v][b]<b_a[b]){
                           node_data.v_b_Amax[v][b]=b_a[b];
                       }
                   }
                }
            }
         }
         cout<<"node_data.u_a_Bmax"<<endl;
         unordered_map<int,vector<int>> temp_u_a_Bmax;
         unordered_map<int,vector<int>> temp_v_b_Amax;
         unordered_map<int,vector<u_node*>> temp_u_bicore =MAC_u_uindex[mask];
         unordered_map<int,vector<u_node*>> temp_v_bicore =MAC_u_vindex[mask];
         for(auto & it:temp_u_bicore){
            int a=it.first;
            vector<u_node*> &b_usets=it.second;
            int b_max=b_usets.size()-1;
            u_node* fa=b_usets[b_max];
   
            for(int b=b_max;b>=1;b--){
               if((b!=b_max&&fa==b_usets[b]) || b_usets[b]==NULL){
                  continue;
               }
               //cout<<a<<"b"<<b<<" "<<fa<<" "<<b_usets[b]<<endl;
               unordered_set<int>&uset=b_usets[b]->uset;
               for(auto &u:uset){
                   if(node_data.u_a_Bmax[u].size()<=a){
                     node_data.u_a_Bmax[u].resize(a+1);
                   }
                   if(node_data.u_a_Bmax[u][a]<b){
                     node_data.u_a_Bmax[u][a]=b;
                   }
               } 
               fa=b_usets[b];
            }
         }
         cout<<"node_data.v_b_Amax"<<endl;
         for(auto & it:temp_v_bicore){
            int b=it.first;
            vector<u_node*> &a_vsets=it.second;
            int a_max=a_vsets.size()-1;
            u_node* fa=a_vsets[a_max];
   
            for(int a=a_max;a>=1;a--){
               if((a!=a_max&&fa==a_vsets[a])|| a_vsets[a]==NULL){
                  continue;
               }
               unordered_set<int>&vset=a_vsets[a]->uset;
               for(auto &v:vset){
                   if(node_data.v_b_Amax[v].size()<=b){
                     node_data.v_b_Amax[v].resize(b+1);
                   }
                   if(node_data.v_b_Amax[v][b]<a){
                     node_data.v_b_Amax[v][b]=a;
                   }
               }
               fa=a_vsets[a];
               
            }
   
         }
         for (auto it:node_data.u_a_Bmax) {
             int u=it.first;
             int size=it.second.size();
             node_data.left_index_change[u]=vector<int>(size,0);
         }
         for(auto it: node_data.v_b_Amax){
            int v=it.first;
            int size=it.second.size();
            node_data.right_index_change[v]=vector<int>(size,0);
         }
      
         node_data.left_index_old = node_data.u_a_Bmax;
         node_data.right_index_old = node_data.v_b_Amax;
      
         node_data.left_index_delete = node_data.u_a_Bmax;
         node_data.right_index_delete = node_data.v_b_Amax;
         g.ulabel_bicore[mask]=node_data;
       }
   
   
       for(int i=0;i<label_v_combinations.size();i++){
         
         uint32_t mask=label_v_combinations[i];
         cout<<mask<<endl;
         node_graph node_data;
         vector<string> label_v_combination = mask_to_labels(mask, label_v_vec);
         g.subgraph_Extraction_u_ve(label_v_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
         node_data.degree_u.resize(g.n1+1, 0);
         node_data.degree_v.resize(g.n2+1, 0);
   
         // 计算度数
         for (auto& it : node_data.edge_mp_u) {
            node_data.degree_u[it.first] = it.second.size();
         }
         for (auto& it : node_data.edge_mp_v) {
            node_data.degree_v[it.first] = it.second.size();
         }
         //计算前置节点的Bmax和Amax
         for (int i = 0; i < 32; i++) {
            if (mask & (1U << i)) {
                uint32_t fa_mask = mask & ~(1U << i); // 清除第i位
                if(fa_mask == 0){
                    break;
                }
                unordered_map<int,vector<int>>&temp_u_a_Bmax=g.vlabel_bicore[fa_mask].u_a_Bmax;
                unordered_map<int,vector<int>>&temp_v_b_Amax=g.vlabel_bicore[fa_mask].v_b_Amax;
                cout<<"fa_mask"<<fa_mask<<endl;
                for(auto& it:temp_u_a_Bmax){
                   int u=it.first;
                   vector<int>& a_b=it.second;
                   int max_a= a_b.size()-1;
                   if(node_data.u_a_Bmax[u].size()<=max_a){
                       node_data.u_a_Bmax[u].resize(max_a+1);
                   }
                   for(int a=1;a<=max_a;a++){
                     if(node_data.u_a_Bmax[u][a]<a_b[a]){
                       node_data.u_a_Bmax[u][a]=a_b[a];
                     }
                   }
                }
                for(auto it:temp_v_b_Amax){
                   int v=it.first;
                   vector<int>& b_a=it.second;
                   int max_b= b_a.size()-1;
                   if(node_data.v_b_Amax[v].size()<=max_b){
                       node_data.v_b_Amax[v].resize(max_b+1);
                   }
                   for(int b=1;b<=max_b;b++){
                       if(node_data.v_b_Amax[v][b]<b_a[b]){
                           node_data.v_b_Amax[v][b]=b_a[b];
                       }
                   }
                }
            }
         }
         cout<<"node_data.u_a_Bmax"<<endl;
         unordered_map<int,vector<int>> temp_u_a_Bmax;
         unordered_map<int,vector<int>> temp_v_b_Amax;
         unordered_map<int,vector<u_node*>> temp_u_bicore =MAC_v_uindex[mask];
         unordered_map<int,vector<u_node*>> temp_v_bicore =MAC_v_vindex[mask];
         for(auto & it:temp_u_bicore){
            int a=it.first;
            vector<u_node*> &b_usets=it.second;
            int b_max=b_usets.size()-1;
            u_node* fa=b_usets[b_max];
   
            for(int b=b_max;b>=1;b--){
               if((b!=b_max&&fa==b_usets[b]) || b_usets[b]==NULL){
                  continue;
               }
               //cout<<a<<"b"<<b<<" "<<fa<<" "<<b_usets[b]<<endl;
               unordered_set<int>&uset=b_usets[b]->uset;
               for(auto &u:uset){
                   if(node_data.u_a_Bmax[u].size()<=a){
                     node_data.u_a_Bmax[u].resize(a+1);
                   }
                   if(node_data.u_a_Bmax[u][a]<b){
                     node_data.u_a_Bmax[u][a]=b;
                   }
               } 
               fa=b_usets[b];
            }
         }
         cout<<"node_data.v_b_Amax"<<endl;
         for(auto & it:temp_v_bicore){
            int b=it.first;
            vector<u_node*> &a_vsets=it.second;
            int a_max=a_vsets.size()-1;
            u_node* fa=a_vsets[a_max];
   
            for(int a=a_max;a>=1;a--){
               if((a!=a_max&&fa==a_vsets[a])|| a_vsets[a]==NULL){
                  continue;
               }
               unordered_set<int>&vset=a_vsets[a]->uset;
               for(auto &v:vset){
                   if(node_data.v_b_Amax[v].size()<=b){
                     node_data.v_b_Amax[v].resize(b+1);
                   }
                   if(node_data.v_b_Amax[v][b]<a){
                     node_data.v_b_Amax[v][b]=a;
                   }
               }
               fa=a_vsets[a];
               
            }
   
         }
         for (auto it:node_data.u_a_Bmax) {
            int u=it.first;
            int size=it.second.size();
            node_data.left_index_change[u]=vector<int>(size,0);
        }
        for(auto it: node_data.v_b_Amax){
           int v=it.first;
           int size=it.second.size();
           node_data.right_index_change[v]=vector<int>(size,0);
        }
     
        node_data.left_index_old = node_data.u_a_Bmax;
        node_data.right_index_old = node_data.v_b_Amax;
     
         node_data.left_index_delete = node_data.u_a_Bmax;
         node_data.right_index_delete = node_data.v_b_Amax;
         g.vlabel_bicore[mask]=node_data;
       }
      }
      double MAC_index:: Edge_delete_MAC_maintenance(string filename){
         auto start= chrono::system_clock::now();
           unordered_map< int, vector<int>> u_edge_add;
           unordered_map< int, vector<int>> v_edge_add;
           unordered_map<int,unordered_set<string>> u_label;
           unordered_map<int,unordered_set<string>> v_label;
           unordered_set<string> LU,LV;
           vector<string> label_u_vec(g.LABEL_U.begin(), g.LABEL_U.end());
           vector<string> label_v_vec(g.LABEL_V.begin(), g.LABEL_V.end());
           // 按顶点集合大小排序（降序）
          sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
             return this->g.labelu[a].size() > this->g.labelu[b].size();
          });
           sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
               return this->g.labelv[a].size() > this->g.labelv[b].size();
           });
           swap(label_u_vec[1],label_u_vec[2]);
           unordered_map<string,uint32_t> label_mask_u;
           unordered_map<string,uint32_t> label_mask_v;
           vector<string> lABEL_U(g.LABEL_U.begin(),g.LABEL_U.end()),lABEL_V(g.LABEL_V.begin(),g.LABEL_V.end());
           label_mask_u=generate_32bit_map_ucombinations(lABEL_U,g.labelu);
           label_mask_v=generate_32bit_map_vcombinations(lABEL_V,g.labelv);
           {
            ifstream file(filename);
            string line;
            if(!file.is_open())
            {
                cout << "Failed to open file!" << endl;
                return 0;
            }
    
            int u,v;
            string tempulabel,tempvlabel;
            int cnt=1;
    
            while(file.good()&&!file.eof())
            {   cnt++;
                char ulabel_temp[100];  // 创建一个临时的字符数组
                char vlabel_temp[100];  // 创建另一个临时的字符数组
                getline(file,line);
                if(line.empty()) continue;
                sscanf(line.c_str(), "%d %d %s %s", &u, &v, ulabel_temp, vlabel_temp);
                tempulabel = ulabel_temp;
                tempvlabel = vlabel_temp;
                if(u>=g.ulabel.size())
                {
                    g.ulabel.resize(u+1);
                }
                if(v>=g.vlabel.size())
                {
                    g.vlabel.resize(v+1);
                }
                std::stringstream ss_u(tempulabel);  // 对v的标签字符串进行分割
                std::string token_u;
                while (std::getline(ss_u, token_u, '|')) {
                g.ulabel[u].insert(token_u);  // 将每个标签插入v的标签集合
                
                LU.insert(token_u);
                g.labelu[token_u].insert(u);  // 将v插入到该标签的反向映射集合
                if(g.LABEL_U.find(token_u) == g.LABEL_U.end()) {
                   g.LABEL_U.insert(token_u);  // 将标签加入总集合
                 }              
                }
    
                std::stringstream ss_v(tempvlabel);  // 对v的标签字符串进行分割
                std::string token_v;
                while (std::getline(ss_v, token_v, '|')) {
                g.vlabel[v].insert(token_v);  // 将每个标签插入v的标签集合
                LV.insert(token_v);
                g.labelv[token_v].insert(v);  // 将v插入到该标签的反向映射集合
                if(g.LABEL_V.find(token_v) == g.LABEL_V.end()) {
                   g.LABEL_V.insert(token_v);  // 将标签加入总集合
                 }
                }
             // 添加边
                if(u>=g.edges_u.size())
                {
                    g.edges_u.resize(u+1);
                }
                if(v>=g.edges_v.size())
                {
                    g.edges_v.resize(v+1);
                }
               u_edge_add[u].emplace_back(v);
               v_edge_add[v].emplace_back(u);
               g.edges_u[u].emplace_back(v);
               g.edges_v[v].emplace_back(u);
               g.m++;
             }
             for(auto u:u_edge_add){
               u_label[u.first]=g.ulabel[u.first];
             }
             for(auto v:v_edge_add){
               v_label[v.first]=g.vlabel[v.first];
             }
        
              file.close();
           }
   
           vector<string> ulabel_delete_vec;
           vector<string> vlabel_delete_vec;
           for(auto u:g.LABEL_U){
               if(LU.find(u)==LU.end()){
                  ulabel_delete_vec.push_back(u);
               }
           }
           for(auto v:g.LABEL_V){
               if(LV.find(v)==LV.end()){
                  vlabel_delete_vec.push_back(v);
               }
           }
   
   
           sort(ulabel_delete_vec.begin(), ulabel_delete_vec.end(), [this](const string& a, const string& b) {
            return this->g.labelu[a].size() > this->g.labelu[b].size();
         });
          sort(vlabel_delete_vec.begin(), vlabel_delete_vec.end(), [this](const string& a, const string& b) {
              return this->g.labelv[a].size() > this->g.labelv[b].size();
          });
   
            // 生成所有可能的  LABEL_V ，U子集组合
           vector<vector<string>> label_u_combinations =generate_combinations_1(label_u_vec);
           vector<vector<string>> label_v_combinations =generate_combinations_1(label_v_vec);
           vector<vector<string>> label_u_delete_combinations =generate_combinations_1(ulabel_delete_vec);
           vector<vector<string>> label_v_delete_combinations =generate_combinations_1(vlabel_delete_vec);
           label_u_combinations=filter_combinations(label_u_combinations,label_u_delete_combinations);
           label_v_combinations=filter_combinations(label_v_combinations,label_v_delete_combinations);
   
           
         
           for(int i=0;i<label_u_combinations.size();i++){
            
               vector<string> label_u_combination = label_u_combinations[i];
               for(int j=0;j<label_u_combination.size();j++){
                  cout<<label_u_combination[j]<<" ";
               }
               cout<<endl;
               uint32_t mask_u=0;
               for(int j=0;j<label_u_combination.size();j++){
                   mask_u=mask_u|label_mask_u[label_u_combination[j]];
               }
               cout<<mask_u<<endl;
   
               node_graph&node_data=g.ulabel_bicore[mask_u];
   
               double time=0 ;
               int cnt1=0;
               unordered_map<int,vector<int>> edge_add;
               for(auto it :u_edge_add){
                  int u=it.first;
                  int flag=0;
                  for(auto label:u_label[u]){
                     if((label_mask_u[label]& mask_u)!=0){
                        flag=1;
                        break;
                     }
                  }
                  if(flag==1){
                     edge_add[u]=it.second;
                     cnt1+=it.second.size();
                  }
               }
               unordered_set<int> update_u_set;
               unordered_set<int> update_v_set;

               for(auto u_edge:edge_add)
               {
                  for(auto edge:u_edge.second)
                  {
   
                     time +=update_skyline_index_swap_with_bfs_delete(node_data,u_edge.first,edge,update_u_set,update_v_set);
   

                     
                  }
               }
                std::cout << "update_uv_abmax time: " << time/cnt1 << " seconds" << std::endl;
               
               for(auto it:update_v_set){ 
                  int v=it;
                  vector<int>& v_b_amax = node_data.v_b_Amax[v];
                  int size = v_b_amax.size()-1;
                  for(int b=1;b<=size;b++)
                   {
                     delete_u_to_MAC(MAC_u_vindex,v,b,mask_u);
                     int a=v_b_amax[b];
                     insert_u_to_MAC_index(MAC_u_vindex,v,b,a,mask_u);
      
                   } 
                }
                for(auto it:update_u_set){
                  int u=it;
                  vector<int>& u_a_bmax = node_data.u_a_Bmax[u];
                  int size = u_a_bmax.size()-1;
                  for(int a=1;a<=size;a++)
                   {
                     delete_u_to_MAC(MAC_u_uindex,u,a,mask_u);
                     int b=u_a_bmax[a];
                     insert_u_to_MAC_index(MAC_u_uindex,u,a,b,mask_u);
                   }
                }
   
           }
           //v
           for(int i=0;i<label_v_combinations.size();i++){
            vector<string> label_v_combination = label_v_combinations[i];
            uint32_t mask_v=0;
            cout<<"mask_v:"<<mask_v<<endl;
            for(int j=0;j<label_v_combination.size();j++){
                mask_v=mask_v|label_mask_v[label_v_combination[j]];
            }
   
            node_graph&node_data=g.vlabel_bicore[mask_v];
            double time=0 ;
            int cnt1=0;
            unordered_map<int,vector<int>> edge_add;
            for(auto it :v_edge_add){
               int v=it.first;
               int flag=0;
               for(auto label:v_label[v]){
                  if((label_mask_v[label]& mask_v)!=0){
                     flag=1;
                     break;
                  }
               }
               if(flag==1){
                  edge_add[v]=it.second;
                  cnt1+=it.second.size();
               }
            }
            unordered_set<int> update_v_set;
            unordered_set<int> update_u_set;
            for(auto v_edge:edge_add)
            {
               for(auto edge:v_edge.second)
               {
                  
                  time +=update_skyline_index_swap_with_bfs_delete(node_data,edge,v_edge.first,update_u_set,update_v_set);
               
               
                 
               }
            }
             std::cout << "update_uv_abmax time: " << time/cnt1 << " seconds" << std::endl;
             for(auto it:update_v_set){ 
               int v=it;
               vector<int>& v_b_amax = node_data.v_b_Amax[v];
               int size = v_b_amax.size()-1;
               for(int b=1;b<=size;b++)
                {
                  delete_u_to_MAC(MAC_v_vindex,v,b,mask_v);
                  int a=v_b_amax[b];
                  insert_u_to_MAC_index(MAC_v_vindex,v,b,a,mask_v);
   
                } 
             }
             for(auto it:update_u_set){
               int u=it;
               vector<int>& u_a_bmax = node_data.u_a_Bmax[u];
               int size = u_a_bmax.size()-1;
               for(int a=1;a<=size;a++)
                {
                 
                  delete_u_to_MAC(MAC_v_uindex,u,a,mask_v);
                  int b=u_a_bmax[a];
                 
                  insert_u_to_MAC_index(MAC_v_uindex,u,a,b,mask_v);
                }
             }
             cout<<"update_uv_abmax"<<endl;
   
        }
        auto end= chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
      //   save_label_index_batch("MAC_v_vindex_2.txt",MAC_v_vindex);
      //   save_label_index_batch("MAC_v_uindex_2.txt",MAC_v_uindex);
      //   save_label_index_batch("MAC_u_vindex_2.txt",MAC_u_vindex);
      //   save_label_index_batch("MAC_u_uindex_2.txt",MAC_u_uindex);
      //   label_index_batch_opt( MAC_v_vindex);
      //   label_index_batch_opt( MAC_v_uindex);
      //   label_index_batch_opt( MAC_u_vindex);
      //   label_index_batch_opt( MAC_u_uindex);
        return elapsed_seconds.count();
      }
