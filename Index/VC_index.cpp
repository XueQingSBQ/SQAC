#include "VC_index.h"


void VC_index::insert_umask(std::unordered_map<int, 
   std::unordered_map<int, 
   std::vector<label_node*>>> &index, int u,int a, int b, uint32_t label_mask){

      vector<label_node*> &b_label_nodes=index[u][a];
      if(b_label_nodes.size()<=b){
         b_label_nodes.resize(b+1,nullptr);
      }
      if(b_label_nodes[b]==nullptr){
         b_label_nodes[b]= new label_node;
      }
      label_node *label_node_ptr = b_label_nodes[b];
      int flag=0;
      for(int i=0;i<label_node_ptr->label_vector.size();i++)
      {
         if((label_node_ptr->label_vector[i]&label_mask)==label_node_ptr->label_vector[i]){
            flag=1;
            break;
         }
      }
      if(flag==0){
         label_node_ptr->label_vector.push_back(label_mask);
      }


}
void VC_index::creat_VC_index(string filename)
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
    for(int i=0;i<label_u_vec.size();i++){
      cout<<label_u_vec[i]<<" "<<g.labelu[label_u_vec[i]].size()<<endl;

    }
     // 生成所有可能的  LABEL_V ，U子集组合
    vector<uint32_t> label_u_combinations =generate_32bit_combinations(label_u_vec);
    vector<uint32_t> label_v_combinations =generate_32bit_combinations(label_v_vec);
    
     int cnt=0;
     unordered_map<uint32_t, node_graph> ulabel_node_graph;
     unordered_map<uint32_t, node_graph> vlabel_node_graph;
     

       
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
               ulabel_node_graph[label_mask]=node_data;
                auto end= chrono::system_clock::now();
                cout<<"insert_ulabel_node"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               cout<<"end"<<endl;                       
            
         }
         else
         {
            uint32_t label_mask_fa;
            label_mask_fa= get_parent_mask_gcc(label_mask);
             
            node_data= ulabel_node_graph[label_mask_fa];
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
            ulabel_node_graph[label_mask]=node_data;
         }
         for(auto it:node_data.u_a_Bmax)
            {
                vector<int>& u_a_bmax = it.second;
                int size = u_a_bmax.size()-1;
                int u=it.first;
               auto start = chrono::system_clock::now();
          
               for(int a=1;a<=size;a++)
               {
                  int b=u_a_bmax[a];
                 // cout<<"b"<<b<<endl;
                  auto start = chrono::system_clock::now();
                // 使用新的合并方式

                   insert_umask(VC_uindex,u,a,b,label_mask);
                  auto end= chrono::system_clock::now();
                //  cout<<"insert_label_to_index_add"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               
               } 
                auto end= chrono::system_clock::now();
               // cout<<"insert_label_to_index"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;

               
            }
               for(auto it:node_data.v_b_Amax)
            {
                vector<int>& v_b_amax = node_data.v_b_Amax[it.first];
                int size = v_b_amax.size()-1;
                int v=it.first;
               auto start = chrono::system_clock::now();
     
               for(int b=1;b<=size;b++)
               {
                  int a=v_b_amax[b];
                  insert_umask(VC_u_vindex,v,b,a,label_mask);
                 
               } 
               
            }

      }          
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
               auto start = chrono::system_clock::now();
     
               for(int b=1;b<=size;b++)
               {
                  int a=v_b_amax[b];
                  auto start = chrono::system_clock::now();
                // 使用新的合并方式

                   insert_umask(VC_vindex,v,b,a,label_mask);
                  auto end= chrono::system_clock::now();
                //  cout<<"insert_label_to_index_add"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               
               } 
                auto end= chrono::system_clock::now();
               // cout<<"insert_label_to_index"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;

               
            }
            for(auto it:node_data.u_a_Bmax)
            {
                vector<int>& u_a_bmax = it.second;
                int size = u_a_bmax.size()-1;
                int u=it.first;
               auto start = chrono::system_clock::now();
          
               for(int a=1;a<=size;a++)
               {
                  int b=u_a_bmax[a];
                 // cout<<"b"<<b<<endl;
                  auto start = chrono::system_clock::now();
                // 使用新的合并方式

                   insert_umask(VC_v_uindex,u,a,b,label_mask);
                  auto end= chrono::system_clock::now();
                //  cout<<"insert_label_to_index_add"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               
               } 
                auto end= chrono::system_clock::now();
               // cout<<"insert_label_to_index"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;

               
            }

      } 
      cout<<"index_batch_opt"<<endl;
    save_VC_index("VC_u_uindex_"+filename+".txt",VC_uindex);
    save_VC_index("VC_v_vindex_"+filename+".txt",VC_vindex);
    save_VC_index("VC_v_uindex_"+filename+".txt",VC_v_uindex);
    save_VC_index("VC_u_vindex_"+filename+".txt",VC_u_vindex);
//    save_index_batch("VC_ulabel_uindex.txt",label_u_vec,label_u_combinations,VC_uindex);
//    save_index_batch("VC_ulabel_vindex.txt",label_u_vec,label_u_combinations,VC_u_vindex);
//    save_index_batch("VC_vlabel_vindex.txt",label_v_vec,label_v_combinations,VC_vindex);
//    save_index_batch("VC_vlabel_uindex.txt",label_v_vec,label_v_combinations,VC_v_uindex);

   VC_index_opt();
  // cout<<indexu_batch[1][2][2]<<endl;
   auto end= chrono::system_clock::now();
    // 计算时间间隔（以秒为单位，结果是 double 类型）
    std::chrono::duration<double> elapsed_seconds = end - start;

   cout<<"creat_VC_index"<<elapsed_seconds.count()<<"s"<<endl;
}
void VC_index::VC_index_opt(){
   int cnt=0;
   for(auto &it1: VC_uindex){
      int u=it1.first;
      //cout<<u<<endl;
      for(auto &it2:it1.second){
         int a=it2.first;
         vector<label_node*>& b_label_nodes= it2.second;
         int size=b_label_nodes.size()-1;
        // cout<<"u="<<u<<endl;
         for(int b=size-1;b>=1;b--){
           // cout<<"b="<<b<<endl;
            if(b_label_nodes[b]==nullptr){
               //cnt++;
                b_label_nodes[b]=b_label_nodes[b+1];
               // cout<<"b_label_nodes[b]="<<b_label_nodes[b]<<endl;
               // cout<<"indexu_batch[u][a][b]="<<indexu_batch[u][a][b]<<endl;
            }
            else {
                b_label_nodes[b]->next=b_label_nodes[b+1];
            }
         }
      }
      
   }
  for(auto &it1: VC_v_uindex){
      int u=it1.first;
      //cout<<u<<endl;
      for(auto &it2:it1.second){
         int a=it2.first;
         vector<label_node*>& b_label_nodes= it2.second;
         int size=b_label_nodes.size()-1;
        // cout<<"u="<<u<<endl;
         for(int b=size-1;b>=1;b--){
           // cout<<"b="<<b<<endl;
            if(b_label_nodes[b]==nullptr){
               //cnt++;
                b_label_nodes[b]=b_label_nodes[b+1];
               // cout<<"b_label_nodes[b]="<<b_label_nodes[b]<<endl;
               // cout<<"indexu_batch[u][a][b]="<<indexu_batch[u][a][b]<<endl;
            }
            else {
                b_label_nodes[b]->next=b_label_nodes[b+1];
            }
         }
      }
      
   }
   //cout<<cnt<<"indexu_batch[u][a][b]="<<indexu_batch[1][2][2]<<endl;
   for(auto &it1: VC_vindex){
      int v=it1.first;
      for(auto &it2:it1.second){
         int b=it2.first;
         vector<label_node*>& a_label_nodes= it2.second;
         int size=a_label_nodes.size()-1;
         for(int a=size-1;a>=1;a--){
            if(a_label_nodes[a]==nullptr){
                a_label_nodes[a]=a_label_nodes[a+1];
            }
            else{
               a_label_nodes[a]->next=a_label_nodes[a+1];
            }
         }
      }
   }
   for(auto &it1: VC_u_vindex){
      int v=it1.first;
      for(auto &it2:it1.second){
         int b=it2.first;
         vector<label_node*>& a_label_nodes= it2.second;
         int size=a_label_nodes.size()-1;
         for(int a=size-1;a>=1;a--){
            if(a_label_nodes[a]==nullptr){
                a_label_nodes[a]=a_label_nodes[a+1];
            }
            else{
               a_label_nodes[a]->next=a_label_nodes[a+1];
            }
         }
      }
   }
}
void VC_index::load_VC_index(const string& filename, unordered_map<int, unordered_map<int, vector<label_node*>>>& index_batch) {
   ifstream file(filename);
   if (!file.is_open()) {
       cerr << "Error: Failed to open file " << filename << " for reading.\n";
       return;
   }

   auto parse_label_vector = [](const string& data) -> vector<label_node*> {
       vector<label_node*> vec;
       if (data.front() != '[' || data.back() != ']') return vec;

       istringstream ss(data.substr(1, data.size() - 2));
       string item;
       while (getline(ss, item, '|')) {
           if (item == "null") {
               vec.push_back(nullptr);
           } else {
               auto* node = new label_node;
               istringstream ls(item.substr(1, item.size() - 2));
               string num;
               while (getline(ls, num, ',')) {
                   node->label_vector.push_back(stoul(num));
               }
               vec.push_back(node);
           }
       }
       return vec;
   };

   string line;
   while (getline(file, line)) {
       size_t colon_pos = line.find(':');
       if (colon_pos == string::npos) continue;

       string key_part = line.substr(0, colon_pos);
       string value_part = line.substr(colon_pos + 1);

       int outer_key = stoi(key_part);
       auto& imap = index_batch[outer_key];

       size_t brace_pos = value_part.find('{');
       if (brace_pos == string::npos) continue;

       istringstream entries(value_part.substr(brace_pos + 1,value_part.size()-2));
       string entry;
       while (getline(entries, entry, ';')) {
           size_t inner_colon = entry.find(':');
           
           if (inner_colon == string::npos) continue;
           cout<<entry<<endl;
           int inner_key = stoi(entry.substr(0, inner_colon));
           imap[inner_key] = parse_label_vector(entry.substr(inner_colon + 1));
       }
   }

   file.close();
}
void VC_index::save_VC_index(const string& filename, const unordered_map<int, unordered_map<int, vector<label_node*>>>& index_batch) {
   ofstream file(filename);
   if (!file.is_open()) {
       cerr << "Error: Failed to open file " << filename << " for writing.\n";
       return;
   }

   auto write_label_vector = [&](const vector<label_node*>& vec) {
       file << "[";
       for (size_t i = 0; i < vec.size(); ++i) {
           if (vec[i]) {
               file << "{";
               for (auto& lbl : vec[i]->label_vector) {
                   file << lbl << ",";
               }
               if (!vec[i]->label_vector.empty()) file.seekp(-1, ios_base::cur); // 删除最后一个逗号
               file << "}";
           } else {
               file << "null";
           }
           if (i != vec.size() - 1) file << "|";
       }
       file << "]";
   };

   for (auto& [okey, imap] : index_batch) {
       file << okey << ":{";
       for (auto& [ikey, vec] : imap) {
           file << ikey << ":";
           write_label_vector(vec);
           file << ";";
       }
       if (!imap.empty()) file.seekp(-1, ios_base::cur); // 删除最后一个分号
       file << "}\n";
   }

   file.close();
}
double VC_index::VC_query (unordered_set<string>&LU,unordered_set<string>&LV ,int a,int b)
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
       for(auto &it1 : VC_uindex){

         int u=it1.first;

         int flag=0;
         //判断顶点u的标签集是否和集合LU有交集
         if((g.umask[u]&u_mask)==0){
            continue;
         }
        unordered_map<int,vector<label_node*>> &a_b_mask=it1.second;
         if(a_b_mask.find(a)!=a_b_mask.end()){
            vector<label_node*> b_label_nodes=a_b_mask[a];
            if(b_label_nodes.size()>b){
               
               label_node* b_label_node=b_label_nodes[b];
               while(b_label_node!=nullptr){
                  for(uint32_t mask:b_label_node->label_vector)
                  {
                      if((mask&u_mask)==mask){
                         tmp_u_set.insert(u);
                         flag=1;
                         break;
                      }
                  }
                  if(flag==1){
                     break;
                  }
                  b_label_node=b_label_node->next;
               }
            }
         }
         
       }
       for(auto &it1 : VC_v_uindex){

         int u=it1.first;

         int flag=0;
         //判断顶点u的标签集是否和集合LU有交集
        unordered_map<int,vector<label_node*>> &a_b_mask=it1.second;
         if(a_b_mask.find(a)!=a_b_mask.end()){
            vector<label_node*> b_label_nodes=a_b_mask[a];
            if(b_label_nodes.size()>b){
               
               label_node* b_label_node=b_label_nodes[b];
               while(b_label_node!=nullptr){
                  for(uint32_t mask:b_label_node->label_vector)
                  {
                      if((mask&v_mask)==mask&&tmp_u_set.find(u)!=tmp_u_set.end()){
                         u_set.insert(u);
                         flag=1;
                         break;
                      }
                  }
                  if(flag==1){
                     break;
                  }
                  b_label_node=b_label_node->next;
               }
            }
         }
         
       }
      // cout<<"u_set size:"<<u_set.size()<<endl;
       for(auto &it2 : VC_vindex){
         int v=it2.first;
         int flag=0;
         if((g.vmask[v]&v_mask)==0){
            continue;
         }
         unordered_map<int,vector<label_node*>> &b_a_mask=it2.second;
         if(b_a_mask.find(b)!=b_a_mask.end()){
            vector<label_node*> a_label_nodes=b_a_mask[b];
            if(a_label_nodes.size()>a){
               label_node* a_label_node=a_label_nodes[a];
               while(a_label_node!=nullptr){
                 
                  for(auto mask:a_label_node->label_vector)
                  {
                     //std::cout << "Binary: " << std::bitset<32>(mask) << std::endl;
                      if((mask&v_mask)==mask){
                         tmp_v_set.insert(v);
                         flag=1;
                         break;
                      }

                  }
                  if(flag==1){
                     break;
                  }
                  a_label_node=a_label_node->next;
               }
            }
         }

       }
       for(auto &it2 : VC_u_vindex){
         int v=it2.first;
         int flag=0;
         unordered_map<int,vector<label_node*>> &b_a_mask=it2.second;
         if(b_a_mask.find(b)!=b_a_mask.end()){
            vector<label_node*> a_label_nodes=b_a_mask[b];
            if(a_label_nodes.size()>a){
               label_node* a_label_node=a_label_nodes[a];
               while(a_label_node!=nullptr){
                 
                  for(auto mask:a_label_node->label_vector)
                  {
                     //std::cout << "Binary: " << std::bitset<32>(mask) << std::endl;
                      if((mask&u_mask)==mask&&tmp_v_set.find(v)!=tmp_v_set.end()){
                         v_set.insert(v);
                         flag=1;
                         break;
                      }

                  }
                  if(flag==1){
                     break;
                  }
                  a_label_node=a_label_node->next;
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
      
       return elapsed_seconds.count();

}