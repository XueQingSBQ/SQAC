#include<iostream>
#include <random>
#include <cstdlib>  // for rand()
#include <ctime>    // for time()
#include"Graph.h"
#include"casestudy.h"

using namespace std;
// vector<pair<int, int>> test_ab1 = {
//     {5,5},
//     {180,14}
// };
// unordered_set<string> get_random_subset(const unordered_set<string>& original_set, double ratio) {
//     vector<string> elements(original_set.begin(), original_set.end());
    
//     // 四舍五入计算样本数量，保证不超过原始集合大小
//     int sample_size = static_cast<int>(elements.size() * ratio + 0.5);
//     sample_size = min(static_cast<int>(elements.size()), sample_size);

//     // 使用随机数引擎进行洗牌
//     random_device rd;
//     mt19937 gen(rd());
//     shuffle(elements.begin(), elements.end(), gen);

//     // 构造子集
//     unordered_set<string> subset;
//     for (int i = 0; i < sample_size; ++i) {
//         subset.insert(elements[i]);
//     }

//     return subset;
// }
// void test2(Graph &g) {
//     int a, b;
//     unordered_set<string> LU_origin = g.LABEL_U;
//     unordered_set<string> LV_origin = g.LABEL_V;
//     vector<double> ratios = {0.2, 0.4, 0.6, 0.8};

//     for (int i = 0; i < test_ab1.size(); i++) {
//         a = test_ab1[i].first;
//         b = test_ab1[i].second;
//         cout << "a=" << a << " b=" << b << endl;

//         for (double ratio : ratios) {
//             double t1 = 0.0, t2 = 0.0, t3 = 0.0, t4 = 0.0, t5 = 0.0;

//             for (int j = 0; j < 50; j++) {
//                 // 随机采样LU和LV
//                 unordered_set<string> LU = get_random_subset(LU_origin, ratio);
//                 unordered_set<string> LV = get_random_subset(LV_origin, ratio);

//                 t1 += g.online_core_label_graph(LU, LV, a, b);
//                 t2 += g.online_core_label(LU, LV, a, b);
//                 t3 += g.VC_query(LU, LV, a, b);
//                 t4 += g.AC_query(LU, LV, a, b);
//                 t5 += g.MAC_query(LU, LV, a, b);
//             }

//             cout << "Ratio: " << ratio << endl;
//             cout << "ave_online_query_graph: " << t1 / 50 << "s" << endl;
//             cout << "ave_online_query: " << t2 / 50 << "s" << endl;
//             cout << "ave_index_batch_query: " << t3 / 50 << "s" << endl;
//             cout << "ave_label_index_batch_query: " << t4 / 50 << "s" << endl;
//             cout << "ave_label_index_batch_opt_query: " << t5 / 50 << "s" << endl;
//             cout << endl;
//         }
//     }
// }
void test3(Graph &g) {
   g.load_AC_index("MAC_u_uindex.txt", g.MAC_u_uindex);
   g.load_AC_index("MAC_u_vindex.txt", g.MAC_u_vindex);
   g.load_AC_index("MAC_v_vindex.txt", g.MAC_v_vindex);
   g.load_AC_index("MAC_v_uindex.txt", g.MAC_v_uindex);
   g.AC_index_opt(g.MAC_u_uindex);
   g.AC_index_opt(g.MAC_u_vindex);
   g.AC_index_opt(g.MAC_v_uindex);
   g.AC_index_opt(g.MAC_v_vindex);

    double t1 = 0.0, t2 = 0.0, t3 = 0.0, t4 = 0.0;
    int max_degree_u = g.max_degree_u;
    int max_degree_v = g.max_degree_v;
    
    int a = 2, b = 2;
    //vector<int> uv=g.lable_index_batch_opt_query_print(g.LABEL_U,g.LABEL_V,a,b);
    
    int n1 = g.n1 - 1;
  //  n1=uv.size();
    srand(time(0));  // 设置随机种子
    
    const int TEST_COUNT = 300;

    for (int i = 0; i < TEST_COUNT; ++i) {
        // 随机选取 u ∈ [1, n1-1]
 
        int u = 1 + rand() % (n1 - 1);
        cout << "u=" << u << endl;
        t1 += inc(u, a, b,g);
        t2 += inc_opt(u, a, b,g);
        t3 += Dec(u, a, b,g);
        t4 += Dec_opt(u, a, b,g);
    }

    // 计算平均值
    t1 /= TEST_COUNT;
    t2 /= TEST_COUNT;
    t3 /= TEST_COUNT;
    t4 /= TEST_COUNT;

    // 打印结果
    cout << "Average time for inc: " << t1 << endl;
    cout << "Average time for inc_opt: " << t2 << endl;
    cout << "Average time for Dec: " << t3 << endl;
    cout << "Average time for Dec_opt: " << t4 << endl;
}
// // 工具函数：从 unordered_set 中随机选取比例为 ratio 的子集（带四舍五入和安全控制）

// vector<unordered_set<string>> test_data_u = {
//     {"actor", "composer"},
// };
// vector<unordered_set<string>> test_data_v = {
//     {"Drama", "War"},
// };
// vector<pair<int, int>> test_ab = {
//     {18, 1},
//     {36, 3},
//     {72, 6},
//     {108, 8},
//     {180, 14},
//     {252,19},
//     {324,24}
// };
// vector<unordered_set<string>> test_data_u = {
//     {"a", "b"},
// };
// vector<unordered_set<string>> test_data_v = {
//     {"x", "y"},
// };
// vector<pair<int, int>> test_ab = {
//     {1, 1},
//     {2, 2},
//     {1, 2},
//     {2, 3},
//     {3, 3},
// };

// // vector<unordered_set<string>> test_data_u1 = {
// //     {"actor"},
// //     {"actor", "archive_footage"},
// //     {"actor", "archive_footage", "composer","soundtrack"},

// //     {"composer"},
// //     {"archive_footage","composer"},
// //     {"archive_footage", "composer","actor","soundtrack"},
// //     {"actor", "archive_footage", "composer", "soundtrack","archive_sound"},
    
// // };
// // vector<unordered_set<string>> test_data_v1 = {
// //      {"Drama"},
// //      {"Western", "Drama"},
// //      {"Drama", "Western", "War","History"},
    
// //      {"Drama"},
// //     {"Western","Drama"},
// //     {"Drama","Western","War","History"},
// //     {"Drama", "Western", "War","History","Family"},
// // };
// // vector<pair<int, int>> test_ab1 = {
// //     {5,5},
// //     {180,14}
// // };

// // //766

// vector<unordered_set<string>> test_data_u = {
//     {"actor", "assistant_director"},

// };
// vector<unordered_set<string>> test_data_v = {
//     {"Drama", "Thriller"},
// };
// vector<pair<int, int>> test_ab = {
//     {17, 1},
//     {34, 2},
//     {68,4},
//     {102, 5},
//     {170, 9},
//     {239,13},
//     {307,16}
// };
// // vector<unordered_set<string>> test_data_u1 = {
// //     {"actor"},
// //     {"actor", "assistant_director"},
// //     {"actor", "assistant_director", "composer","archive_sound"},

// //     {"composer"},
// //     {"actor","composer"},
// //     {"assistant_director", "composer","actor","cinematographer"},
// //     {"actor", "assistant_director", "composer", "cinematographer","archive_sound"},
    
// // };
// // vector<unordered_set<string>> test_data_v1 = {
// //     {"Drama"},
// //     {"Drama", "Crime"},
// //     {"Drama", "Crime", "Drama","War"},
// //     {"Drama"},
// //     {"Drama","Crime"},
// //     {"Drama","Crime","Drama","War"},
// //     {"Drama", "Crime", "Drama","War","Western"},
// // };
// // vector<pair<int, int>> test_ab1 = {
// //     {6,6},
// //     {170,9}
// // };
// // // //


// vector<unordered_set<string>> test_data_u = {
//     {"click", "alipay"},

// };
// vector<unordered_set<string>> test_data_v = {
//     {"60", "625","5967"},
// };
// vector<pair<int, int>> test_ab = {
//     {6, 8},
//     {13,16},
//     {26,32},
//     {38,47},
//     {64,79},
//     {90,111},
//     {115,142}
// };
// // vector<unordered_set<string>> test_data_u1 = {
// //     {"click"},
// //     {"click", "alipay"},
// //     {"click", "cart", "alipay"},

// //     {"click"},
// //     {"click", "alipay"},
// //     {"click", "cart", "alipay"},

// //     {"click", "cart", "collect" "alipay"},
    
// // };
// // vector<unordered_set<string>> test_data_v1 = {
// //     {"5961","625"},
// //     {"60","625","5961"},
// //     {"60","5961","5967","625","3836"},

// //     {"60","625"},
// //     {"60","625","5961"},
// //     {"60","5961","5967","625","3836"},
// //     {"60","5961" ,"5967","625","3836","5120"},
// // };
// // vector<pair<int, int>> test_ab1 = {
// //     {5,5},
// //     {64,79}
// // };
//  //1047
// vector<unordered_set<string>> test_data_u = {
//     {"click", "alipay"},

// };
// vector<unordered_set<string>> test_data_v = {
//     {"10", "4057","6732","4776"},
// };
// vector<pair<int, int>> test_ab = {
//     {5, 3},
//     {10,7},
//     {20,14},
//     {30,20},
//     {51,34},
//     {71,48},
//     {91,61}
// };
// // vector<unordered_set<string>> test_data_u1 = {
// //     {"click"},
// //     {"click", "cart"},
// //     {"click", "cart", "alipay"},

// //     {"click"},
// //     {"click", "alipay"},
// //     {"click", "cart", "alipay"},

// //     {"click", "cart", "collect" "alipay"},
    
// // };
// // vector<unordered_set<string>> test_data_v1 = {
// //     {"6732","4057"},
// //     {"10","4057","6828","2171"},
// //     {"10","4057","6828","2171","6129"},

// //     {"10","4057"},
// //     {"10","4057","6828","2171"},
// //     {"10","4057","6828","2171","6129"},
// //     {"10","4057","6828","2171","6129","4776","3744"},
// // };
// // vector<pair<int, int>> test_ab1 = {
// //     {5,5},
// //     {50,34}
// // };
// // //1052
// vector<unordered_set<string>> test_data_u = {
//     {"actor", "writer"},

// };
// vector<unordered_set<string>> test_data_v = {
//     {"Drama", "Action","Romance"},
// };
// vector<pair<int, int>> test_ab = {
//     {31, 2},
//     {61, 5},
//     {122,9},
//     {183,14},
//     {306,23},
//     {428,32},
//     {550,41}
// };
// // vector<unordered_set<string>> test_data_u1 = {
// //     {"actor"},
// //     {"actor", "writer"},
// //     {"actor", "writer", "archive_footage"},

// //     {"actor"},
// //     {"actor", "writer"},
// //     {"actor", "writer", "archive_footage"},
// //     {"actor", "writer", "archive_footage", "miscellaneous"},
    
// // };
// // vector<unordered_set<string>> test_data_v1 = {
// //     {"Drama"},
// //     {"Drama", "Crime","Action"},
// //     {"Drama", "Crime", "Action","Romance"},
// //     {"Crime"},
// //     {"Action", "Crime","Romance"},
// //     {"Drama", "Crime", "Action","Romance"},
// //     {"Drama", "Crime", "Action","Romance","Thriller","Comedy"},
// // };
// // vector<pair<int, int>> test_ab1 = {
// //     {11,11},
// //     {305,23}
// // };
// // vector<unordered_set<string>> test_data_u = {
// //     {"1", "2","3"},

// // };
// // vector<unordered_set<string>> test_data_v = {
// //     {"a", "b","d"},
// // };
// // vector<pair<int, int>> test_ab = {
// //     {11,7},
// //     {21,15},
// //     {43,30},
// //     {64,45},
// //     {107,76},
// //     {150,107},
// //     {193,137}
// // };
// void test1(Graph &g){
//     int a,b;
//     unordered_set<string> LU;
//     unordered_set<string> LV;
//     for(int i=0;i<test_data_u.size();i++){
//         LU=test_data_u[i];
//         LV=test_data_v[i];
//         cout<<"di"<<i<<endl;
//         for(auto it:LU){
//             cout<<it<<" ";
//         }cout<<endl;
//         for(auto it:LV){
//             cout<<it<<" ";
//         }cout<<endl;
//         for(int j=0;j<test_ab.size();j++){
//             a=test_ab[j].first;
//             b=test_ab[j].second;
//             cout<<a<<" "<<b<<endl;
//             double t1=0.0 ,t2=0.0,t3=0.0,t4=0.0,t5=0.0;
//            // for(int i=0;i<30;i++){
//               //  t1+=g.online_core_label_graph(LU,LV,a,b);
//                 t2+= g.online_core_label(LU,LV,a,b);
               
//            // }
//             //for(int i=0;i<50;i++){
//             //    t3+= g.index_batch_query(LU,LV,a,b);
//                 t4+= g.AC_query(LU,LV,a,b);
//                t5+= g.MAC_query(LU,LV,a,b);
//           //  }
//             cout<<"ave_online_quiry_graph"<<t1*1.0/30<<"s"<<endl;
//             cout<<"ave_online_quiry"<<t2*1.0/30<<"s"<<endl;
//             cout<<"ave_index_batch_query"<<t3*1.0/50<<"s"<<endl;
//             cout<<"ave_lable_index_batch_query"<<t4*1.0/50<<"s"<<endl;
//             cout<<"ave_lable_index_batch_opt_query"<<t5*1.0/50<<"s"<<endl;

//             // g.online_core_label1(LU,LV,a,b);   
//            // cout<<endl;         
//         }
//     }
// }
string joinLabels(const unordered_set<string>& labels) {
   stringstream ss;
   for (auto it = labels.begin(); it != labels.end(); ++it) {
       if (it != labels.begin()) ss << "|";
       ss << *it;
   }
   return ss.str();
}
void edge_ge_add(Graph&g){
   // 确保两个集合非空
   if (g.ulabel.empty() || g.vlabel.empty()) {
      cerr << "标签集合不能为空" << endl;
      return ;
  }

  // 初始化随机数
  srand((unsigned int)time(0));

  ofstream outfile("edge_add.txt");
  if (!outfile.is_open()) {
      cerr << "无法打开文件 edge.txt" << endl;
      return ;
  }

  int numEdges = 50;
  int usize = g.ulabel.size();
  int vsize = g.vlabel.size();

  for (int i = 0; i < numEdges; ++i) {
   int u = rand() % usize;
   int v = rand() % vsize;
      if(u==0||v==0){
         i--;
         continue;
      }

      string uLabels = joinLabels(g.ulabel[u]);
      string vLabels = joinLabels(g.vlabel[v]);
      outfile << u << " " << v << " " << uLabels << " " << vLabels << endl;
  }

  outfile.close();
  cout << "生成完毕，已写入 edge.txt" << endl;

  return ;
}
void edge_ge_delete(Graph&g){
   srand((unsigned int)time(0));

   ofstream outfile("edge_delete.txt");
   if (!outfile.is_open()) {
       cerr << "无法打开 edge.txt" << endl;
       return ;
   }

   int numEdges = 50;
   int usize =g.edges_u.size();
   int count = 0;
   int try_limit = 100000; // 防止死循环
   int tries = 0;

   while (count < numEdges && tries < try_limit) {
       int u = rand() % usize;

       if (g.edges_u[u].empty()) {
           ++tries;
           continue; // 跳过无出边的点
       }

       int vidx = rand() % g.edges_u[u].size();
       int v = g.edges_u[u][vidx];

       string uLabels = joinLabels(g.ulabel[u]);
       string vLabels = joinLabels(g.vlabel[v]);

       outfile << u  << " " << v << " " << uLabels << " " << vLabels << endl;
       ++count;
   }

   outfile.close();
}
void test5(Graph &g){
       //edge_ge_delete(g);
       g.load_AC_index("MAC_u_uindex.txt", g.MAC_u_uindex);
       g.load_AC_index("MAC_u_vindex.txt", g.MAC_u_vindex);
       g.load_AC_index("MAC_v_vindex.txt", g.MAC_v_vindex);
       g.load_AC_index("MAC_v_uindex.txt", g.MAC_v_uindex);
   
      cout<<"MAC_to_bicore"<<endl;
      g.MAC_to_bicore_delete();
      cout<<"MAC_to_bicore_opt"<<endl;
      cout<<g.Edge_delete_MAC_maintenance("edge.txt")/50.0<<endl;
}
void test4(Graph &g){
   edge_ge_add(g);
   g.load_AC_index("MAC_u_uindex.txt", g.MAC_u_uindex);
   g.load_AC_index("MAC_u_vindex.txt", g.MAC_u_vindex);
   g.load_AC_index("MAC_v_vindex.txt", g.MAC_v_vindex);
   g.load_AC_index("MAC_v_uindex.txt", g.MAC_v_uindex);


  cout<<"MAC_to_bicore"<<endl;
  g.MAC_to_bicore();
  cout<<"MAC_to_bicore_opt"<<endl;
  cout<<g.Edge_add_MAC_maintenance("edge_add.txt")/50.0<<endl;
}
int main()
{
     Graph g=Graph();
     g.createGraph("in.txt");
     g.labels_to_mask_init();

   // g.creat_MAC_Uindex();
    //g.creat_MAC_Vindex();
    // g.creat_index_batch();
    // g.creat_lable_index_batch();
    // g.creat_lable_index_batch_opt();
     // g.creat_bicore_index();
    //  g.load_index_batch("index_batch_u_546.txt", g.indexu_batch);
    //  g.load_index_batch("index_batch_v_546.txt", g.indexv_batch);
    //   g.index_batch_opt();
      //  g.load_label_index_batch("MAC_u_uindex.txt", g.MAC_u_uindex);
      //  g.load_label_index_batch("MAC_u_vindex.txt", g.MAC_u_vindex);
      //  g.load_label_index_batch("MAC_v_vindex.txt", g.MAC_v_vindex);
      //  g.load_label_index_batch("MAC_v_uindex.txt", g.MAC_v_uindex);
   

      // cout<<"MAC_to_bicore"<<endl;
      // g.MAC_to_bicore_delete();
      // cout<<"MAC_to_bicore_opt"<<endl;
      // g.Edge_delete_MAC_maintenance("edge.txt");

      //  g.label_index_batch_opt(g.MAC_u_uindex);
      //  g.label_index_batch_opt(g.MAC_u_vindex);
      //  g.label_index_batch_opt(g.MAC_v_vindex);
      //  g.label_index_batch_opt(g.MAC_v_uindex);
    //   g.label_index_batch_opt(g.label_uindex_batch);
    //   g.label_index_batch_opt(g.label_vindex_batch);
     // g.load_label_index_batch("label_index_batch_opt_u_546.txt", g.label_uindex_batch_opt);
     // g.load_label_index_batch("label_index_batch_opt_v_546.txt", g.label_vindex_batch_opt);

     // g.label_index_batch_opt(g.label_uindex_batch_opt);
     // g.label_index_batch_opt(g.label_vindex_batch_opt);

     

    //  g.inc(1,2,2);
    //  g.inc_opt(1,2,2);
    //  g.Dec(1,2,2);
    //  g.Dec_opt(1,2,2);
    // test5(g);
    test3(g);
    

}


// void test2(Graph &g){
//     int a,b;
//     unordered_set<string> LU=g.LABEL_U;
//     unordered_set<string> LV=g.LABEL_V;
//     for(int i=0;i<test_ab1.size();i++){
//         a=test_ab1[i].first;
//         b=test_ab1[i].second;
//         cout<<"a="<<a<<"b="<<b<<endl;
//         for(int j=0;j<test_data_u1.size();j++){
//             LU=test_data_u1[j];
//             LV=test_data_v1[j];
//             for(auto it:LU){
//                 cout<<it<<" ";
//             }cout<<endl;
//             for(auto it:LV){
//                 cout<<it<<" ";
//             }cout<<endl;
            
//             double t1=0.0 ,t2=0.0,t3=0.0,t4=0.0,t5=0.0;
//             for(int i=0;i<30;i++){
//                 t1+=g.online_core_label_graph(LU,LV,a,b);
//                 t2+= g.online_core_label(LU,LV,a,b);
               
//             }
//             for(int i=0;i<50;i++){
//                 t3+= g.index_batch_query(LU,LV,a,b);
//                 t4+= g.lable_index_batch_query(LU,LV,a,b);
//                 t5+= g.lable_index_batch_opt_query(LU,LV,a,b);
//             }
//             cout<<"ave_online_quiry_graph"<<t1*1.0/30<<"s"<<endl;
//             cout<<"ave_online_quiry"<<t2*1.0/30<<"s"<<endl;
//             cout<<"ave_index_batch_query"<<t3*1.0/50<<"s"<<endl;
//             cout<<"ave_lable_index_batch_query"<<t4*1.0/50<<"s"<<endl;
//             cout<<"ave_lable_index_batch_opt_query"<<t5*1.0/50<<"s"<<endl;

           
//             cout<<endl;       
             
//         }
//     }
// }
// void test3(Graph &g){
    
//             double t1=0.0 ,t2=0.0,t3=0.0,t4=0.0,t5=0.0;
//             int a=3,b=3;
//             int n1=g.n1-1;
//             int n2=g.n2-1;
//             int u;
//             t1+=g.inc(u,a,b);
//             t2+=g.inc_opt(u,a,b);
//             t3+=g.Dec(u,a,b);
//             t4+=g.Dec_opt(u,a,b);

           
//         //     for(int i=0;i<20;i++){
//         //         t1+=g.inc()
//         //     }
//         //     cout<<"ave_online_quiry_graph"<<t1*1.0/30<<"s"<<endl;
//         //     cout<<"ave_online_quiry"<<t2*1.0/30<<"s"<<endl;
//         //     cout<<"ave_index_batch_query"<<t3*1.0/50<<"s"<<endl;
//         //     cout<<"ave_lable_index_batch_query"<<t4*1.0/50<<"s"<<endl;
//         //     cout<<"ave_lable_index_batch_opt_query"<<t5*1.0/50<<"s"<<endl;

//         //     // g.online_core_label1(LU,LV,a,b);   
//         //    // cout<<endl;         
//      }
    
