#include"Graph.h"
#include"bicore.h"
#include"dyn_rebuild.h"
#include"GraphUtils.h"
#include"node.h"
using namespace std;




    // 构造函数初始化图
   Graph:: Graph() : n1(0),n2(0),m(0) {}

   void Graph::createGraph(string filename) {
        ifstream file(filename);
        string line;
        if(!file.is_open())
        {
            cout << "Failed to open file!" << endl;
            return;
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
        //    cout<<u<<" "<<v<<" "<<ulabel_temp<<" "<<vlabel_temp<<endl;

            // 将解析出的字符串存储到 C++ string 对象中
            tempulabel = ulabel_temp;
            tempvlabel = vlabel_temp;

            if(u>=ulabel.size())
            {
                ulabel.resize(u+1);
            }
            if(v>=vlabel.size())
            {
                vlabel.resize(v+1);
            }
            std::stringstream ss_u(tempulabel);  // 对v的标签字符串进行分割
            std::string token_u;
            while (std::getline(ss_u, token_u, '|')) {
            ulabel[u].insert(token_u);  // 将每个标签插入v的标签集合
            labelu[token_u].insert(u);  // 将v插入到该标签的反向映射集合
            if(LABEL_U.find(token_u) == LABEL_U.end()) {
               LABEL_U.insert(token_u);  // 将标签加入总集合
             }
             
            }

            std::stringstream ss_v(tempvlabel);  // 对v的标签字符串进行分割
            std::string token_v;
            while (std::getline(ss_v, token_v, '|')) {
            vlabel[v].insert(token_v);  // 将每个标签插入v的标签集合
            labelv[token_v].insert(v);  // 将v插入到该标签的反向映射集合
            if(LABEL_V.find(token_v) == LABEL_V.end()) {
               LABEL_V.insert(token_v);  // 将标签加入总集合
             }
             
            }
            
         // 添加边
            if(u>=edges_u.size())
            {
                edges_u.resize(u+1);
            }
            if(v>=edges_v.size())
            {
                edges_v.resize(v+1);
            }
           edges_u[u].emplace_back(v);
           edges_v[v].emplace_back(u);
           m++;
        }
        n1=edges_u.size();
        n2=edges_v.size();
       file.close();
    }
        
    void  Graph:: subgraph_Extraction(const unordered_set<string>&LU,const unordered_set<string>&LV,
                              unordered_map< int,vector< int>> &edge_mp_u,
                              unordered_map< int,vector< int>> &edge_mp_v,
                              unordered_set< int>&u_set,
                              unordered_set<int>&v_set )
    {
         
         for(auto label1:LU)
         {
            for(auto u:labelu[label1])
            {
               if(u_set.find(u)!=u_set.end())
               continue;
                for(auto v:edges_u[u])
                {
                   for(auto label2:vlabel[v])
                   { 
                     if(LV.find(label2)!=LV.end())
                     {
                      edge_mp_u[u].emplace_back(v);
                      edge_mp_v[v].emplace_back(u);
                      if(u_set.find(u)==u_set.end())
                      u_set.insert(u);
                      if(v_set.find(v)==v_set.end())
                      v_set.insert(v);
                      break;
                     }
                      
                   }
                }
            }
         }
    }
    void  Graph::subgraph_Extraction_ve(const vector<string>&LU,const vector<string>&LV,
                              unordered_map<  int,vector<int>> &edge_mp_u,
                              unordered_map< int,vector<int>> &edge_mp_v,

                              unordered_set< int>&u_set,
                              unordered_set<int>&v_set )
    {
         
         for(auto label1:LU)
         {
            
           
           for(auto u:labelu[label1])
            {

               if(u_set.find(u)!=u_set.end())
               continue;
                for(auto v:edges_u[u])
                {
                 
                   for(auto label2:vlabel[v])
                   { 
                     if(find(LV.begin(),LV.end(),label2)!=LV.end())
                     {
                      edge_mp_u[u].emplace_back(v);
                      edge_mp_v[v].emplace_back(u);
                      if(u_set.find(u)==u_set.end())
                      u_set.insert(u);
                      if(v_set.find(v)==v_set.end())
                      v_set.insert(v);
                      break;
                     }
                      
                   }
                }
            }
         }
    }
   void  Graph::subgraph_Extraction_u_ve(const vector<string>&LU,
      unordered_map<  int,vector<int>> &edge_mp_u,
      unordered_map< int,vector<int>> &edge_mp_v,

      unordered_set< int>&u_set,
      unordered_set<int>&v_set )
  {

      for(auto label1:LU)
      {


         for(auto u:labelu[label1])
         {

         if(u_set.find(u)!=u_set.end())
         continue;
            for(auto v:edges_u[u])
            {

            edge_mp_u[u].emplace_back(v);
            edge_mp_v[v].emplace_back(u);
            if(u_set.find(u)==u_set.end())
            u_set.insert(u);
            if(v_set.find(v)==v_set.end())
            v_set.insert(v);

            }
         }
      }
   }
   void Graph:: subgraph_Extraction_v_ve(const vector<string>&LV,
      unordered_map<  int,vector<int>> &edge_mp_u,
      unordered_map< int,vector<int>> &edge_mp_v,

      unordered_set< int>&u_set,
      unordered_set<int>&v_set )
  {

      for(auto label1:LV)
      {


         for(auto v:labelv[label1])
         {

            if(v_set.find(v)!=v_set.end())
            continue;
            for(auto u:edges_v[v])
            {

               edge_mp_u[u].emplace_back(v);
               edge_mp_v[v].emplace_back(u);
               if(u_set.find(u)==u_set.end())
               u_set.insert(u);
               if(v_set.find(v)==v_set.end())
               v_set.insert(v);

            }
         }
      }
   }


   
     

// bool find_a_b_labels1(trie_vlabel & p, int b, const std::unordered_set<std::string>& label_set) {
//    node_vlabel* pb = p.root;
//    if(pb!=nullptr && p.find_sonset(pb,label_set))
//    {
//       return true;
//    }
//    else{
//       return false;
//    }

// }




void Graph::labels_to_mask(unordered_set<string>& LU ,unordered_set<string>& LV,uint32_t&u_mask, uint32_t&v_mask) {
      vector<string> label_u_vec(LABEL_U.begin(), LABEL_U.end());
      vector<string> label_v_vec(LABEL_V.begin(), LABEL_V.end());
      sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
         return this->labelu[a].size() > this->labelu[b].size();
      });
     // swap(label_u_vec[1],label_u_vec[2]);
      sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
         return this->labelv[a].size() > this->labelv[b].size();
      });
      unordered_map<string, int> ulabel_to_index;
      unordered_map<string, int> vlabel_to_index;
      int index = 0;
      for (const auto& label : label_u_vec) {
           ulabel_to_index[label] = index++;
       }
       index=0;
       for (const auto& label : label_v_vec) {
           vlabel_to_index[label] = index++;
       }
       for (const auto& label : LU) {
           int index = ulabel_to_index[label];
           u_mask |= (1U << index);  // 使用32位掩码设置
       }
       for (const auto& label : LV) {
           int index = vlabel_to_index[label];
           v_mask |= (1U << index);  // 使用32位掩码设置
       }
}
void Graph::labels_to_mask_init() {
      vector<string> label_u_vec(LABEL_U.begin(), LABEL_U.end());
      vector<string> label_v_vec(LABEL_V.begin(), LABEL_V.end());
      sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
         return this->labelu[a].size() > this->labelu[b].size();
      });
      sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
         return this->labelv[a].size() > this->labelv[b].size();
      });

      unordered_map<string, int> ulabel_to_index;
      unordered_map<string, int> vlabel_to_index;
      int index = 0;
      for (const auto& label : label_u_vec) {
           ulabel_to_index[label] = index++;
       }
       index=0;
       for (const auto& label : label_v_vec) {
           vlabel_to_index[label] = index++;
       }

       // 将 ulabel 转换为 umask
        umask.resize(ulabel.size(),0);
       for (size_t u = 1; u < ulabel.size(); ++u) {
           for (const auto& label : ulabel[u]) {
               if (ulabel_to_index.find(label) != ulabel_to_index.end()) {
                   umask[u] |= (1U << ulabel_to_index[label]);  // 设置对应位
               }
           }
       }
       vmask.resize(vlabel.size(), 0);
       for (size_t v = 0; v < vlabel.size(); ++v) {
           for (const auto& label : vlabel[v]) {
               if (vlabel_to_index.find(label) != vlabel_to_index.end()) {
                   vmask[v] |= (1U << vlabel_to_index[label]);  // 设置对应位
               }
           }
      }

}

void insert_umask(std::unordered_map<int, 
   std::unordered_map<int, 
   std::vector<label_node*>>> &indexu_batch, int u,int a, int b, uint32_t label_mask){

      vector<label_node*> &b_label_nodes=indexu_batch[u][a];
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
void Graph::creat_VC_index()
{
    auto start= chrono::system_clock::now();
         // 获取所有可能的 LABEL_U 和 LABEL_V 的子集组合
    vector<string> label_u_vec(LABEL_U.begin(), LABEL_U.end());
    vector<string> label_v_vec(LABEL_V.begin(), LABEL_V.end());
   sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
      return this->labelu[a].size() > this->labelu[b].size();
   });
    sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
        return this->labelv[a].size() > this->labelv[b].size();
    });
    for(int i=0;i<label_u_vec.size();i++){
      cout<<label_u_vec[i]<<" "<<labelu[label_u_vec[i]].size()<<endl;

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
               subgraph_Extraction_u_ve(label_u_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
               node_data.degree_u.resize(n1+1, 0);
               node_data.degree_v.resize(n2+1, 0);

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
               int r = online_calculate_delta(node_data.edge_mp_u,node_data.edge_mp_v ,node_data.u_set, node_data.v_set,n1,n2);
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
                  computeBmax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,n1,n2);
                  auto end= chrono::system_clock::now();
                  cout<<"computeBmax_VE time:"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               }
               for(int i=1;i<=r;i++)
               {
                  auto start = chrono::system_clock::now();
                  computeAmax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,n1,n2);
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
            for(auto u:labelu[label_u_combination[label_u_combination.size()-1]])
            {
               if(node_data.u_set.find(u)!=node_data.u_set.end())
                  continue;
               for(auto v:edges_u[u])
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
               subgraph_Extraction_v_ve(label_v_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
               node_data.degree_u.resize(n1+1, 0);
               node_data.degree_v.resize(n2+1, 0);

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
               int r = online_calculate_delta(node_data.edge_mp_u,node_data.edge_mp_v ,node_data.u_set, node_data.v_set,n1,n2);
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
                  computeBmax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,n1,n2);
                  auto end= chrono::system_clock::now();
                  cout<<"computeBmax_VE time:"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               }
               for(int i=1;i<=r;i++)
               {
                  auto start = chrono::system_clock::now();
                  computeAmax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,n1,n2);
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
            for(auto v:labelv[label_v_combination[label_v_combination.size()-1]])
            {
               if(node_data.v_set.find(v)!=node_data.v_set.end())
                  continue;
               for(auto u:edges_v[v])
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

      } 
      cout<<"index_batch_opt"<<endl;
   save_VC_index("VC_u.txt",VC_uindex);
   save_VC_index("VC_v.txt",VC_vindex);
   VC_index_opt();
  // cout<<indexu_batch[1][2][2]<<endl;
   auto end= chrono::system_clock::now();
    // 计算时间间隔（以秒为单位，结果是 double 类型）
    std::chrono::duration<double> elapsed_seconds = end - start;

   cout<<"creat_index_core_label3_batch"<<elapsed_seconds.count()<<"s"<<endl;
}
void Graph::VC_index_opt(){
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
}
void Graph::creat_AC_Uindex()
{
    auto start= chrono::system_clock::now();
         // 获取所有可能的 LABEL_U 和 LABEL_V 的子集组合
    vector<string> label_u_vec(LABEL_U.begin(), LABEL_U.end());
    vector<string> label_v_vec(LABEL_V.begin(), LABEL_V.end());
   sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
      return this->labelu[a].size() > this->labelu[b].size();
   });
    sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
        return this->labelv[a].size() > this->labelv[b].size();
    });
    for(int i=0;i<label_u_vec.size();i++){
      cout<<label_u_vec[i]<<" "<<labelu[label_u_vec[i]].size()<<endl;

    }
     // 生成所有可能的  LABEL_V ，U子集组合
    vector<uint32_t> label_u_combinations =generate_32bit_combinations(label_u_vec);
    vector<uint32_t> label_v_combinations =generate_32bit_combinations(label_v_vec);
    
     int cnt=0;
     unordered_map<uint32_t, node_graph> label_u_bicore;
     unordered_map<uint32_t, node_graph> label_v_bicore;
     
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
               subgraph_Extraction_u_ve(label_u_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
               node_data.degree_u.resize(n1+1, 0);
               node_data.degree_v.resize(n2+1, 0);

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
               int r = online_calculate_delta(node_data.edge_mp_u,node_data.edge_mp_v ,node_data.u_set, node_data.v_set,n1,n2);
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
                  computeBmax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,n1,n2);
                  auto end= chrono::system_clock::now();
                  cout<<"computeBmax_VE time:"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               }
               for(int i=1;i<=r;i++)
               {
                  auto start = chrono::system_clock::now();
                  computeAmax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,n1,n2);
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
            for(auto u:labelu[label_u_combination[label_u_combination.size()-1]])
            {
               if(node_data.u_set.find(u)!=node_data.u_set.end())
                  continue;
               for(auto v:edges_u[u])
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
               auto start = chrono::system_clock::now();
          
               for(int a=1;a<=size;a++)
               {
                  int b=u_a_bmax[a];
                 // cout<<"b"<<b<<endl;
                  auto start = chrono::system_clock::now();
                // 使用新的合并方式

                   insert_u_to_AC_index(AC_u_uindex,u,a,b,label_mask);
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
             
     
               for(int b=1;b<=size;b++)
               {
                  int a=v_b_amax[b];
                insert_u_to_AC_index(AC_u_vindex,v,b,a,label_mask);
                 
               } 
               
               
          }

      }          
      

   save_AC_index("AC_u_uindex.txt",AC_u_uindex);
   save_AC_index("AC_u_vindex.txt",AC_u_vindex);
   //save_label_index_batch("AC_v.txt",label_v_vec,label_vindex_batch);
   AC_index_opt( AC_u_uindex);
   AC_index_opt(AC_u_vindex);
  // cout<<indexu_batch[1][2][2]<<endl;
   auto end= chrono::system_clock::now();
    // 计算时间间隔（以秒为单位，结果是 double 类型）
    std::chrono::duration<double> elapsed_seconds = end - start;
   cout<<"creat_AC_U"<<elapsed_seconds.count()<<"s"<<endl;
}
void Graph::creat_AC_Vindex()
{
    auto start= chrono::system_clock::now();
         // 获取所有可能的 LABEL_U 和 LABEL_V 的子集组合
    vector<string> label_u_vec(LABEL_U.begin(), LABEL_U.end());
    vector<string> label_v_vec(LABEL_V.begin(), LABEL_V.end());
   sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
      return this->labelu[a].size() > this->labelu[b].size();
   });
    sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
        return this->labelv[a].size() > this->labelv[b].size();
    });
    for(int i=0;i<label_u_vec.size();i++){
      cout<<label_u_vec[i]<<" "<<labelu[label_u_vec[i]].size()<<endl;

    }
     // 生成所有可能的  LABEL_V ，U子集组合
    vector<uint32_t> label_u_combinations =generate_32bit_combinations(label_u_vec);
    vector<uint32_t> label_v_combinations =generate_32bit_combinations(label_v_vec);
    
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
               subgraph_Extraction_v_ve(label_v_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
               node_data.degree_u.resize(n1+1, 0);
               node_data.degree_v.resize(n2+1, 0);

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
               int r = online_calculate_delta(node_data.edge_mp_u,node_data.edge_mp_v ,node_data.u_set, node_data.v_set,n1,n2);
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
                  computeBmax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,n1,n2);
                  auto end= chrono::system_clock::now();
                  cout<<"computeBmax_VE time:"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               }
               for(int i=1;i<=r;i++)
               {
                  auto start = chrono::system_clock::now();
                  computeAmax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,n1,n2);
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
            for(auto v:labelv[label_v_combination[label_v_combination.size()-1]])
            {
               if(node_data.v_set.find(v)!=node_data.v_set.end())
                  continue;
               for(auto u:edges_v[v])
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

                insert_u_to_AC_index(AC_v_vindex,v,b,a,label_mask);
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
          
               for(int a=1;a<=size;a++)
               {
                  int b=u_a_bmax[a];
                  insert_u_to_AC_index(AC_v_uindex,u,a,b,label_mask);
               } 
                auto end= chrono::system_clock::now();

               
         }

      } 
     // cout<<"index_batch_opt"<<endl;

   //save_label_index_batch("AC_u.txt",label_u_vec,label_uindex_batch);
   save_AC_index("AC_v_vindex.txt",AC_v_vindex);
   save_AC_index("AC_v_uindex.txt",AC_v_uindex);
   AC_index_opt( AC_v_vindex);
   AC_index_opt( AC_v_uindex);
  // cout<<indexu_batch[1][2][2]<<endl;
   auto end= chrono::system_clock::now();
    // 计算时间间隔（以秒为单位，结果是 double 类型）
    std::chrono::duration<double> elapsed_seconds = end - start;
   cout<<"creat_AC_V"<<elapsed_seconds.count()<<"s"<<endl;
}
void Graph::insert_u_to_AC_index(unordered_map<uint32_t,unordered_map<int,vector<u_node*> > >&label_index_batch,int u,int a,int b,uint32_t label_mask){
   vector<u_node*>& u_node_list=label_index_batch[label_mask][a];
   if(u_node_list.size()<=b){
      u_node_list.resize(b+1,nullptr);
   }
   if(u_node_list[b]==nullptr){
      u_node_list[b]= new u_node;
   }
   u_node *u_node_ptr = u_node_list[b];
   u_node_ptr->uset.insert(u);

}
void Graph::AC_index_opt(unordered_map<uint32_t,unordered_map<int,vector<u_node*> > >&label_index_batch) {

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

void Graph::creat_MAC_Uindex()
{
    auto start= chrono::system_clock::now();
         // 获取所有可能的 LABEL_U 和 LABEL_V 的子集组合
    vector<string> label_u_vec(LABEL_U.begin(), LABEL_U.end());
    vector<string> label_v_vec(LABEL_V.begin(), LABEL_V.end());
   sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
      return this->labelu[a].size() > this->labelu[b].size();
   });
    sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
        return this->labelv[a].size() > this->labelv[b].size();
    });
    for(int i=0;i<label_v_vec.size();i++){
      cout<<label_v_vec[i]<<" "<<labelv[label_v_vec[i]].size()<<endl;
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
               subgraph_Extraction_u_ve(label_u_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
               node_data.degree_u.resize(n1+1, 0);
               node_data.degree_v.resize(n2+1, 0);

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
               int r = online_calculate_delta(node_data.edge_mp_u,node_data.edge_mp_v ,node_data.u_set, node_data.v_set,n1,n2);
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
                  computeBmax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,n1,n2);
                  auto end= chrono::system_clock::now();
                  cout<<"computeBmax_VE time:"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               }
               for(int i=1;i<=r;i++)
               {
                  auto start = chrono::system_clock::now();
                  computeAmax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,n1,n2);
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
            for(auto u:labelu[label_u_combination[label_u_combination.size()-1]])
            {
               if(node_data.u_set.find(u)!=node_data.u_set.end())
                  continue;
               for(auto v:edges_u[u])
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
      

   save_AC_index("MAC_u_uindex.txt",MAC_u_uindex);
   save_AC_index("MAC_u_vindex.txt",MAC_u_vindex);
   AC_index_opt( MAC_u_uindex);
   AC_index_opt( MAC_u_vindex);
  // cout<<indexu_batch[1][2][2]<<endl;
   auto end= chrono::system_clock::now();
    // 计算时间间隔（以秒为单位，结果是 double 类型）
    std::chrono::duration<double> elapsed_seconds = end - start;
   cout<<"creat_lable_index_opt_batch"<<elapsed_seconds.count()<<"s"<<endl;
}
void Graph::creat_MAC_Vindex()
{
    auto start= chrono::system_clock::now();
         // 获取所有可能的 LABEL_U 和 LABEL_V 的子集组合
    vector<string> label_u_vec(LABEL_U.begin(), LABEL_U.end());
    vector<string> label_v_vec(LABEL_V.begin(), LABEL_V.end());
   sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
      return this->labelu[a].size() > this->labelu[b].size();
   });
    sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
        return this->labelv[a].size() > this->labelv[b].size();
    });
    for(int i=0;i<label_v_vec.size();i++){
      cout<<label_v_vec[i]<<" "<<labelv[label_v_vec[i]].size()<<endl;
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
               subgraph_Extraction_v_ve(label_v_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
               node_data.degree_u.resize(n1+1, 0);
               node_data.degree_v.resize(n2+1, 0);

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
               int r = online_calculate_delta(node_data.edge_mp_u,node_data.edge_mp_v ,node_data.u_set, node_data.v_set,n1,n2);
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
                  computeBmax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,n1,n2);
                  auto end= chrono::system_clock::now();
                  cout<<"computeBmax_VE time:"<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<"us"<<endl;
               }
               for(int i=1;i<=r;i++)
               {
                  auto start = chrono::system_clock::now();
                  computeAmax_VE(node_data.edge_mp_u,node_data.edge_mp_v,node_data.u_set,node_data.v_set,node_data.degree_u,node_data.degree_v,u_a_Bmax,v_b_Amax,i,n1,n2);
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
            for(auto v:labelv[label_v_combination[label_v_combination.size()-1]])
            {
               if(node_data.v_set.find(v)!=node_data.v_set.end())
                  continue;
               for(auto u:edges_v[v])
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


   save_AC_index("MAC_v_vindex.txt",MAC_v_vindex);
   save_AC_index("MAC_v_uindex.txt",MAC_v_uindex);
   AC_index_opt( MAC_v_vindex);
   AC_index_opt( MAC_v_uindex);
  // cout<<indexu_batch[1][2][2]<<endl;
   auto end= chrono::system_clock::now();
    // 计算时间间隔（以秒为单位，结果是 double 类型）
    std::chrono::duration<double> elapsed_seconds = end - start;
   cout<<"creat_lable_index_opt_batch"<<elapsed_seconds.count()<<"s"<<endl;
}

void Graph:: insert_u_to_MAC_index(unordered_map<uint32_t,unordered_map<int,vector<u_node*> > >&label_index_batch,int u,int a,int b,uint32_t label_mask){
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

void  delete_u_to_MAC(unordered_map<uint32_t,unordered_map<int,vector<u_node*> > >&label_index_batch,int u,int a,uint32_t label_mask){
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




void Graph::save_VC_index(const string& filename, const unordered_map<int, unordered_map<int, vector<label_node*>>>& index_batch) {
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
void Graph::load_VC_index(const string& filename, unordered_map<int, unordered_map<int, vector<label_node*>>>& index_batch) {
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
void Graph:: save_AC_index(const string& filename, const unordered_map<uint32_t, unordered_map<int, vector<u_node*>>>& label_index_batch) {
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
void Graph::load_AC_index(const string& filename, unordered_map<uint32_t,unordered_map<int, vector<u_node*>>>& label_index_batch) {
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

      double Graph:: online_core_label(unordered_set<string>&LU,unordered_set<string>&LV ,int a,int b)
    {
       auto start= chrono::system_clock::now();
         unordered_map<int,vector<int>> edge_mp_u;
         unordered_map<int,vector<int>> edge_mp_v;
         unordered_set<int>u_set;
         unordered_set<int>v_set;
         for(auto label1:LU)
         {
            for(auto u:labelu[label1])
            {
               if(u_set.find(u)!=u_set.end())
               continue;
                for(auto v:edges_u[u])
                {
                   for(auto label2:vlabel[v])
                   { 
                     if(LV.find(label2)!=LV.end())
                     {
                      edge_mp_u[u].emplace_back(v);
                      edge_mp_v[v].emplace_back(u);
                      if(u_set.find(u)==u_set.end())
                      u_set.insert(u);
                      if(v_set.find(v)==v_set.end())
                      v_set.insert(v);
                      break;
                     }
                      
                   }
                }
            }
         }

         vector<int>delu(n1+1,0);
         vector<int>delv(n2+1,0);
         vector<int>degree_u(n1+1,0);
         vector<int>degree_v(n2+1,0);
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
        
         // cout<<"("<<a<<","<<b<<")-core:\n";
         //  cout<<"u:\n";
         vector< int>temp_u_set={u_set.begin(),u_set.end()};
         sort(temp_u_set.begin(),temp_u_set.end());
         vector< int>temp_v_set={v_set.begin(),v_set.end()};
         sort(temp_v_set.begin(),temp_v_set.end());
         for(auto u:temp_u_set)
         {
            cout<<u<<" ";
         }
         cout<<endl;
         cout<<"v:\n";
         for(auto v:temp_v_set)
         {
            cout<<v<<" ";
         }
         cout<<endl;
         cout<<"online_quiry"<<elapsed_seconds.count()<<"s"<<endl;
         return elapsed_seconds.count();
    }
   void Graph:: index_core_query(unordered_set<string>&LU,unordered_set<string>&LV,int a,int b)
   {
      auto start= chrono::system_clock::now();
      string label_u_str = to_string(LU);
      string label_v_str = to_string(LV);
      vector<int> u_set;
      vector<int> v_set;

      for(auto&it:index_core_u[label_u_str][label_v_str][a])
      {
             if(it.first>=b)
             {
               for(auto&it2:it.second)
               u_set.emplace_back(it2);
             }
      }
     
     
      for(auto&it:index_core_v[label_u_str][label_v_str][b])
      {
         if(it.first>=a)
             {
               for(auto&it2:it.second)
               v_set.emplace_back(it2);
             }
      }
    auto end= chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    
      sort(u_set.begin(),u_set.end());
      sort(v_set.begin(),v_set.end());
      for(auto&it:u_set)
      cout<<it<<" ";
      cout<<endl; 
      for(auto&it:v_set)
      cout<<it<<" ";
      cout<<endl;
      cout<<"index_1_query"<<elapsed_seconds.count()<<"s"<<endl;
   }
    double Graph::VC_query (unordered_set<string>&LU,unordered_set<string>&LV ,int a,int b)
    {
      
      uint32_t u_mask=0;
      uint32_t v_mask=0;
      labels_to_mask(LU,LV,u_mask, v_mask);
       auto start= chrono::system_clock::now();
       unordered_map<int,vector<int>> edge_mp_u;
       unordered_map<int,vector<int>> edge_mp_v;
       unordered_set<int>u_set;
       unordered_set<int>v_set;
       for(auto &it1 : VC_uindex){

         int u=it1.first;

         int flag=0;
         //判断顶点u的标签集是否和集合LU有交集
         if((umask[u]&u_mask)==0){
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
         if((vmask[v]&v_mask)==0){
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
         for(auto v:edges_u[u]){
            if(v_set.find(v)!=v_set.end()){
               edge_mp_u[u].emplace_back(v);
               edge_mp_v[v].emplace_back(u);
            }
         }
       }

       vector<int>delu(n1+1,0);
       vector<int>delv(n2+1,0);
       vector<int>degree_u(n1+1,0);
       vector<int>degree_v(n2+1,0);
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
    double Graph::AC_query (unordered_set<string>&LU,unordered_set<string>&LV ,int a,int b)
    {
      
      uint32_t u_mask=0;
      uint32_t v_mask=0;
      labels_to_mask(LU,LV,u_mask, v_mask);
       auto start= chrono::system_clock::now();
       unordered_map<int,vector<int>> edge_mp_u;
       unordered_map<int,vector<int>> edge_mp_v;
       unordered_set<int>tmp_u_set;
       unordered_set<int>tmp_v_set;
       unordered_set<int>u_set;
       unordered_set<int>v_set;
       vector<int>u1_set;
       vector<int>v1_set;
       

      unordered_map<int,vector<u_node*>>&a_b_u_unode= AC_u_uindex[u_mask];
      if(a_b_u_unode.find(a)!=a_b_u_unode.end()){
         vector<u_node*>& u_label_nodes=a_b_u_unode[a];
         if(u_label_nodes.size()>b){
            u_node* u_label_node=u_label_nodes[b];
            while(u_label_node!=nullptr){
               unordered_set<int>& uset=u_label_node->uset;
               for(auto u:uset){
                  tmp_u_set.insert(u);
               }
               u_label_node=u_label_node->next;

            }
            
         }
      }
      unordered_map<int,vector<u_node*>>&a_b_v_unode= AC_v_uindex[v_mask];
      if(a_b_v_unode.find(a)!=a_b_v_unode.end()){
         vector<u_node*>& u_label_nodes=a_b_v_unode[a];
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
      unordered_map<int,vector<u_node*>>&b_a_v_vnode= AC_v_vindex[v_mask];
         if(b_a_v_vnode.find(b)!=b_a_v_vnode.end()){
            vector<u_node*>& v_label_nodes=b_a_v_vnode[b];
            if(v_label_nodes.size()>a){
               u_node* v_label_node=v_label_nodes[a];
               while ( v_label_node!=nullptr)
               {
                  /* code */
                  unordered_set<int>& vset=v_label_node->uset;
                  for(auto v:vset){
                     tmp_v_set.insert(v);
                  }
                  v_label_node=v_label_node->next;
               }
               
            }
         }
         unordered_map<int,vector<u_node*>>&b_a_u_vnode= AC_u_vindex[u_mask];
         if(b_a_u_vnode.find(b)!=b_a_u_vnode.end()){
            vector<u_node*>& v_label_nodes=b_a_u_vnode[b];
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
       
      // cout<<"v_set size:"<<v_set.size()<<endl;
       for(auto u:u_set){
         for(auto v:edges_u[u]){
            if(v_set.find(v)!=v_set.end()){
               edge_mp_u[u].emplace_back(v);
               edge_mp_v[v].emplace_back(u);
            }
         }
       }

       vector<int>delu(n1+1,0);
       vector<int>delv(n2+1,0);
       vector<int>degree_u(n1+1,0);
       vector<int>degree_v(n2+1,0);
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
    double Graph:: MAC_query (unordered_set<string>&LU,unordered_set<string>&LV ,int a,int b)
    {
      
      uint32_t u_mask=0;
      uint32_t v_mask=0;
      labels_to_mask(LU,LV,u_mask, v_mask);
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
         for(auto v:edges_u[u]){
            if(v_set.find(v)!=v_set.end()){
               edge_mp_u[u].emplace_back(v);
               edge_mp_v[v].emplace_back(u);
            }
         }
       }

       vector<int>delu(n1+1,0);
       vector<int>delv(n2+1,0);
       vector<int>degree_u(n1+1,0);
       vector<int>degree_v(n2+1,0);
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
    double Graph:: online_core_label_graph(unordered_set<string>&LU,unordered_set<string>&LV ,int a,int b)
    {
       auto start= chrono::system_clock::now();
         unordered_map<int,vector<int>> edge_mp_u;
         unordered_map<int,vector<int>> edge_mp_v;
         unordered_set<int>u_set;
         unordered_set<int>v_set;
         vector<int>delu(n1+1,0);
         vector<int>delv(n2+1,0);
         vector<int>degree_u(n1+1,0);
         vector<int>degree_v(n2+1,0);
         for(int i=1;i<n1;i++)
         {
                  edge_mp_u[i]=edges_u[i];
                  u_set.insert(i);
         }
         for(int i=1;i<n2;i++)
         {
                  edge_mp_v[i]=edges_v[i];
                  v_set.insert(i);
         }

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
                 int flag=0;
                 for(auto it:ulabel[u]){
                  if(LU.find(it)!=LU.end()){
                     flag=1;
                     break;
                  }
                 }
                 
                 if(degree_u[u]<a|| flag==0)  //degree[u]<a
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
                  int flag=0;
                 for(auto it:vlabel[v]){
                  if(LV.find(it)!=LV.end()){
                     flag=1;
                     break;
                  }
                 }
                 if(degree_v[v]<b|| flag==0) //degree[v]<b
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
        
         // cout<<"("<<a<<","<<b<<")-core:\n";
         // cout<<"u:\n";
         vector< int>temp_u_set={u_set.begin(),u_set.end()};
         sort(temp_u_set.begin(),temp_u_set.end());
         vector< int>temp_v_set={v_set.begin(),v_set.end()};
         sort(temp_v_set.begin(),temp_v_set.end());
         // for(auto u:temp_u_set)
         // {
         //    cout<<u<<" ";
         // }
         // cout<<endl;
         // cout<<"v:\n";
         // for(auto v:temp_v_set)
         // {
         //    cout<<v<<" ";
         // }
         // cout<<endl;
         // cout<<"online_quiry"<<elapsed_seconds.count()<<"s"<<endl;
         return elapsed_seconds.count();
    }
    


    graphinc Graph::online_core(int q,unordered_set<int> &u_set,unordered_set<int> &v_set, unordered_map< int,vector<int>> &edge_u,unordered_map< int,vector< int>> &edge_v,int a,int b){
         graphinc g;
         vector<int>delu(n1+1,0);
         vector<int>delv(n2+1,0);
         vector<int>degree_u(n1+1,0);
         vector<int>degree_v(n2+1,0);
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
               return g;
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
         g.u_set=u_set;
         g.v_set=v_set;

         return g;

   }


  void Graph::MAC_to_bicore(){
   vector<string> label_u_vec(LABEL_U.begin(), LABEL_U.end());
   vector<string> label_v_vec(LABEL_V.begin(), LABEL_V.end());
   sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
      return this->labelu[a].size() > this->labelu[b].size();
   });
    sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
        return this->labelv[a].size() > this->labelv[b].size();
    });
    //swap(label_u_vec[1],label_u_vec[2]);
    for(int i=0;i<label_u_vec.size();i++){
      cout<<label_u_vec[i]<<" "<<labelu[label_u_vec[i]].size()<<endl;

    }
     // 生成所有可能的  LABEL_V ，U子集组合
    vector<uint32_t> label_u_combinations =generate_32bit_combinations(label_u_vec);
    vector<uint32_t> label_v_combinations =generate_32bit_combinations(label_v_vec);

    for(int i=0;i<label_u_combinations.size();i++){
      
      uint32_t mask=label_u_combinations[i];
      cout<<mask<<endl;
      node_graph node_data;
      vector<string> label_u_combination = mask_to_labels(mask, label_u_vec);
      subgraph_Extraction_u_ve(label_u_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
      node_data.degree_u.resize(n1+1, 0);
      node_data.degree_v.resize(n2+1, 0);

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
             unordered_map<int,vector<int>>&temp_u_a_Bmax=ulabel_bicore[fa_mask].u_a_Bmax;
             unordered_map<int,vector<int>>&temp_v_b_Amax=ulabel_bicore[fa_mask].v_b_Amax;
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
      ulabel_bicore[mask]=node_data;
    }


    for(int i=0;i<label_v_combinations.size();i++){
      
      uint32_t mask=label_v_combinations[i];
      cout<<mask<<endl;
      node_graph node_data;
      vector<string> label_v_combination = mask_to_labels(mask, label_v_vec);
      subgraph_Extraction_v_ve(label_v_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
      node_data.degree_u.resize(n1+1, 0);
      node_data.degree_v.resize(n2+1, 0);

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
             unordered_map<int,vector<int>>&temp_u_a_Bmax=vlabel_bicore[fa_mask].u_a_Bmax;
             unordered_map<int,vector<int>>&temp_v_b_Amax=vlabel_bicore[fa_mask].v_b_Amax;
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
      vlabel_bicore[mask]=node_data;
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
   double Graph:: Edge_add_MAC_maintenance(string filename){
      auto start= chrono::system_clock::now();
        unordered_map< int, vector<int>> u_edge_add;
        unordered_map< int, vector<int>> v_edge_add;
        unordered_map<int,unordered_set<string>> u_label;
        unordered_map<int,unordered_set<string>> v_label;
        unordered_set<string> LU,LV;
        vector<string> label_u_vec(LABEL_U.begin(), LABEL_U.end());
        vector<string> label_v_vec(LABEL_V.begin(), LABEL_V.end());
        // 按顶点集合大小排序（降序）
       sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
          return this->labelu[a].size() > this->labelu[b].size();
       });
        sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
            return this->labelv[a].size() > this->labelv[b].size();
        });
        unordered_map<string,uint32_t> label_mask_u;
        unordered_map<string,uint32_t> label_mask_v;
        vector<string> lABEL_U(LABEL_U.begin(),LABEL_U.end()),lABEL_V(LABEL_V.begin(),LABEL_V.end());
        label_mask_u=generate_32bit_map_ucombinations(lABEL_U,labelu);
        label_mask_v=generate_32bit_map_vcombinations(lABEL_V,labelv);
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
             if(u>=ulabel.size())
             {
                 ulabel.resize(u+1);
             }
             if(v>=vlabel.size())
             {
                 vlabel.resize(v+1);
             }
             std::stringstream ss_u(tempulabel);  // 对v的标签字符串进行分割
             std::string token_u;
             while (std::getline(ss_u, token_u, '|')) {
             ulabel[u].insert(token_u);  // 将每个标签插入v的标签集合
             
             LU.insert(token_u);
             labelu[token_u].insert(u);  // 将v插入到该标签的反向映射集合
             if(LABEL_U.find(token_u) == LABEL_U.end()) {
                LABEL_U.insert(token_u);  // 将标签加入总集合
              }              
             }
 
             std::stringstream ss_v(tempvlabel);  // 对v的标签字符串进行分割
             std::string token_v;
             while (std::getline(ss_v, token_v, '|')) {
             vlabel[v].insert(token_v);  // 将每个标签插入v的标签集合
             LV.insert(token_v);
             labelv[token_v].insert(v);  // 将v插入到该标签的反向映射集合
             if(LABEL_V.find(token_v) == LABEL_V.end()) {
                LABEL_V.insert(token_v);  // 将标签加入总集合
              }
             }
          // 添加边
             if(u>=edges_u.size())
             {
                 edges_u.resize(u+1);
             }
             if(v>=edges_v.size())
             {
                 edges_v.resize(v+1);
             }
            u_edge_add[u].emplace_back(v);
            v_edge_add[v].emplace_back(u);
            edges_u[u].emplace_back(v);
            edges_v[v].emplace_back(u);
            m++;
          }
          for(auto u:u_edge_add){
            u_label[u.first]=ulabel[u.first];
          }
          for(auto v:v_edge_add){
            v_label[v.first]=vlabel[v.first];
          }
     
           file.close();
        }

        vector<string> ulabel_delete_vec;
        vector<string> vlabel_delete_vec;
        for(auto u:LABEL_U){
            if(LU.find(u)==LU.end()){
               ulabel_delete_vec.push_back(u);
            }
        }
        for(auto v:LABEL_V){
            if(LV.find(v)==LV.end()){
               vlabel_delete_vec.push_back(v);
            }
        }


        sort(ulabel_delete_vec.begin(), ulabel_delete_vec.end(), [this](const string& a, const string& b) {
         return this->labelu[a].size() > this->labelu[b].size();
      });
       sort(vlabel_delete_vec.begin(), vlabel_delete_vec.end(), [this](const string& a, const string& b) {
           return this->labelv[a].size() > this->labelv[b].size();
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

            node_graph& node_data=ulabel_bicore[mask_u];

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

         node_graph &node_data=vlabel_bicore[mask_v];
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
     save_AC_index("MAC_v_vindex_1.txt",MAC_v_vindex);
     save_AC_index("MAC_v_uindex_1.txt",MAC_v_uindex);
     save_AC_index("MAC_u_vindex_1.txt",MAC_u_vindex);
     save_AC_index("MAC_u_uindex_1.txt",MAC_u_uindex);
     AC_index_opt( MAC_v_vindex);
     AC_index_opt( MAC_v_uindex);
     AC_index_opt( MAC_u_vindex);
     AC_index_opt( MAC_u_uindex);
     return elapsed_seconds.count();
   }
   void Graph::MAC_to_bicore_delete(){
      vector<string> label_u_vec(LABEL_U.begin(), LABEL_U.end());
      vector<string> label_v_vec(LABEL_V.begin(), LABEL_V.end());
      sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
         return this->labelu[a].size() > this->labelu[b].size();
      });
      
       sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
           return this->labelv[a].size() > this->labelv[b].size();
       });
       swap(label_u_vec[1],label_u_vec[2]);
       for(int i=0;i<label_u_vec.size();i++){
         cout<<label_u_vec[i]<<" "<<labelu[label_u_vec[i]].size()<<endl;
   
       }
        // 生成所有可能的  LABEL_V ，U子集组合
       vector<uint32_t> label_u_combinations =generate_32bit_combinations(label_u_vec);
       vector<uint32_t> label_v_combinations =generate_32bit_combinations(label_v_vec);
   
       for(int i=0;i<label_u_combinations.size();i++){
         
         uint32_t mask=label_u_combinations[i];
         cout<<mask<<endl;
         node_graph node_data;
         vector<string> label_u_combination = mask_to_labels(mask, label_u_vec);
         subgraph_Extraction_u_ve(label_u_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
         node_data.degree_u.resize(n1+1, 0);
         node_data.degree_v.resize(n2+1, 0);
   
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
                unordered_map<int,vector<int>>&temp_u_a_Bmax=ulabel_bicore[fa_mask].u_a_Bmax;
                unordered_map<int,vector<int>>&temp_v_b_Amax=ulabel_bicore[fa_mask].v_b_Amax;
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
         ulabel_bicore[mask]=node_data;
       }
   
   
       for(int i=0;i<label_v_combinations.size();i++){
         
         uint32_t mask=label_v_combinations[i];
         cout<<mask<<endl;
         node_graph node_data;
         vector<string> label_v_combination = mask_to_labels(mask, label_v_vec);
         subgraph_Extraction_u_ve(label_v_combination,node_data.edge_mp_u, node_data.edge_mp_v,node_data.u_set, node_data.v_set);
         node_data.degree_u.resize(n1+1, 0);
         node_data.degree_v.resize(n2+1, 0);
   
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
                unordered_map<int,vector<int>>&temp_u_a_Bmax=vlabel_bicore[fa_mask].u_a_Bmax;
                unordered_map<int,vector<int>>&temp_v_b_Amax=vlabel_bicore[fa_mask].v_b_Amax;
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
         vlabel_bicore[mask]=node_data;
       }
      }
      double Graph:: Edge_delete_MAC_maintenance(string filename){
         auto start= chrono::system_clock::now();
           unordered_map< int, vector<int>> u_edge_add;
           unordered_map< int, vector<int>> v_edge_add;
           unordered_map<int,unordered_set<string>> u_label;
           unordered_map<int,unordered_set<string>> v_label;
           unordered_set<string> LU,LV;
           vector<string> label_u_vec(LABEL_U.begin(), LABEL_U.end());
           vector<string> label_v_vec(LABEL_V.begin(), LABEL_V.end());
           // 按顶点集合大小排序（降序）
          sort(label_u_vec.begin(), label_u_vec.end(), [this](const string& a, const string& b) {
             return this->labelu[a].size() > this->labelu[b].size();
          });
           sort(label_v_vec.begin(), label_v_vec.end(), [this](const string& a, const string& b) {
               return this->labelv[a].size() > this->labelv[b].size();
           });
           swap(label_u_vec[1],label_u_vec[2]);
           unordered_map<string,uint32_t> label_mask_u;
           unordered_map<string,uint32_t> label_mask_v;
           vector<string> lABEL_U(LABEL_U.begin(),LABEL_U.end()),lABEL_V(LABEL_V.begin(),LABEL_V.end());
           label_mask_u=generate_32bit_map_ucombinations(lABEL_U,labelu);
           label_mask_v=generate_32bit_map_vcombinations(lABEL_V,labelv);
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
                if(u>=ulabel.size())
                {
                    ulabel.resize(u+1);
                }
                if(v>=vlabel.size())
                {
                    vlabel.resize(v+1);
                }
                std::stringstream ss_u(tempulabel);  // 对v的标签字符串进行分割
                std::string token_u;
                while (std::getline(ss_u, token_u, '|')) {
                ulabel[u].insert(token_u);  // 将每个标签插入v的标签集合
                
                LU.insert(token_u);
                labelu[token_u].insert(u);  // 将v插入到该标签的反向映射集合
                if(LABEL_U.find(token_u) == LABEL_U.end()) {
                   LABEL_U.insert(token_u);  // 将标签加入总集合
                 }              
                }
    
                std::stringstream ss_v(tempvlabel);  // 对v的标签字符串进行分割
                std::string token_v;
                while (std::getline(ss_v, token_v, '|')) {
                vlabel[v].insert(token_v);  // 将每个标签插入v的标签集合
                LV.insert(token_v);
                labelv[token_v].insert(v);  // 将v插入到该标签的反向映射集合
                if(LABEL_V.find(token_v) == LABEL_V.end()) {
                   LABEL_V.insert(token_v);  // 将标签加入总集合
                 }
                }
             // 添加边
                if(u>=edges_u.size())
                {
                    edges_u.resize(u+1);
                }
                if(v>=edges_v.size())
                {
                    edges_v.resize(v+1);
                }
               u_edge_add[u].emplace_back(v);
               v_edge_add[v].emplace_back(u);
               edges_u[u].emplace_back(v);
               edges_v[v].emplace_back(u);
               m++;
             }
             for(auto u:u_edge_add){
               u_label[u.first]=ulabel[u.first];
             }
             for(auto v:v_edge_add){
               v_label[v.first]=vlabel[v.first];
             }
        
              file.close();
           }
   
           vector<string> ulabel_delete_vec;
           vector<string> vlabel_delete_vec;
           for(auto u:LABEL_U){
               if(LU.find(u)==LU.end()){
                  ulabel_delete_vec.push_back(u);
               }
           }
           for(auto v:LABEL_V){
               if(LV.find(v)==LV.end()){
                  vlabel_delete_vec.push_back(v);
               }
           }
   
   
           sort(ulabel_delete_vec.begin(), ulabel_delete_vec.end(), [this](const string& a, const string& b) {
            return this->labelu[a].size() > this->labelu[b].size();
         });
          sort(vlabel_delete_vec.begin(), vlabel_delete_vec.end(), [this](const string& a, const string& b) {
              return this->labelv[a].size() > this->labelv[b].size();
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
   
               node_graph&node_data=ulabel_bicore[mask_u];
   
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
   
            node_graph&node_data=vlabel_bicore[mask_v];
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

   

