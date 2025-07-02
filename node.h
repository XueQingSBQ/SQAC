#ifndef NODE_H
#define NODE_H

#include<iostream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<string>
#include<algorithm>
#include<sstream>
#include<fstream>
#include<cstdint>
#include <string.h>
//#include <sys/resource.h>

using namespace std;

struct  label_node
{
    vector<uint32_t> label_vector;
    label_node* next=NULL;
};
struct  u_node
{
    unordered_set<int> uset;
    u_node* next=NULL;
};

template<typename T>
class CustomVector : public std::vector<T> {
public:
   void reserve(size_t n) {
       if (n > this->capacity()) {
           // 每次只增加1个位置，而不是翻倍
           std::vector<T>::reserve(this->capacity() + 1);
       }
   }
};
struct node_vlabel {
   CustomVector<node_vlabel*> children;
   char* label;
   bool isEnd; 

   
   node_vlabel() : label(nullptr), isEnd(false) {}
   
   node_vlabel(const std::string& lbl) : isEnd(false) {
       label = new char[lbl.length() + 1];
       strcpy(label, lbl.c_str());
   }
   
   ~node_vlabel() {
       if (label) delete[] label;
       for (auto child : children) {
           delete child;
       }
   }
    void add_child(node_vlabel* child) {
       // 在添加新元素前确保有足够空间
       if (children.size() == children.capacity()) {
           children.reserve(children.size() + 1);
       }
       
       children.push_back(child);
    //    for(int i = 0; i < children.size(); ++i)
    //    cout<<children[i]->label<<endl;
   }
    node_vlabel* add_or_get_child(const std::string& lbl) {
        for (auto child : children) {
            if (strcmp(child->label, lbl.c_str()) == 0) {
                return child;
            }
        }
        auto new_child = new node_vlabel(lbl);
        add_child(new_child);
        return new_child;
    }
      node_vlabel* find_child(const std::string& lbl) {
       for (auto child : children) {
           if (strcmp(child->label, lbl.c_str()) == 0) {
               return child;
           }
       }
       return nullptr;
   }
}
;

struct trie_vlabel {
   node_vlabel* root;
    trie_vlabel() : root(new node_vlabel()) {}
   
    void insert(const std::vector<std::string>& v_labels) {
       node_vlabel* p = root;
       for (const auto& label : v_labels) {
           node_vlabel* next = p->find_child(label);
           if (!next) {
               next = new node_vlabel(label);
               p->add_child(next);
           }
        
           p = next;

       }
       p->isEnd = true;
        //   for(int i = 0; i < root->children.size(); ++i){
        //     cout<<root->children[i]->label<<endl;
        //    }
   }
//     bool find_sonset(node_vlabel* p, const std::vector<std::string>& label_v_combination) {
//        if (!p) return false;
//         for (auto ne : p->children) {
//            const char* label_v = ne->label;
           
//            if (std::find_if(label_v_combination.begin(), label_v_combination.end(),
//                [label_v](const std::string& s) { 
//                    return strcmp(label_v, s.c_str()) == 0; 
//                }) != label_v_combination.end()) {
               
//                if (ne->isEnd) {
//                    return true;
//                }
//                if (find_sonset(ne, label_v_combination)) {
//                    return true;
//                }
//            }
//        }
//        return false;
//    }
    bool find_sonset(node_vlabel* p, const std::unordered_set<std::string>& label_v_combination) {
       if (!p) return false;
        for (auto ne : p->children) {
           std::string  label_v (ne->label);
           
           if ( label_v_combination.find(label_v)!= label_v_combination.end()) {
               
               if (ne->isEnd) {
                   return true;
               }
               if (find_sonset(ne, label_v_combination)) {
                   return true;
               }
           }
       }
       return false;
   }


 // 序列化 trie_vlabel 到文件

    // 序列化
    void serialize(ofstream& outFile, node_vlabel* node, string prefix = "") {
         //cout<<"ser"<<endl;
        if (node == nullptr) return;
       if (node->label != nullptr) {
        if (node->isEnd) {
         
           // cout<<prefix << node->label << "(end)" << endl;
            outFile << prefix << node->label << "(end)" << endl;
        } else {
           // cout<<prefix << node->label << "(end)" << endl;
            outFile << prefix << node->label << endl;
        }
       }
        // 递归序列化子节点
        for (const auto& child : node->children) {

            serialize(outFile, child, prefix + "   ");
        }
    }

};

struct node_ulabel {
    CustomVector<std::shared_ptr<node_ulabel>> children;
    std::unordered_map<int, trie_vlabel> b_trie_vlabel;
    char* label;
    bool isEnd;

    // 默认构造函数
    node_ulabel() : label(nullptr), isEnd(false) {}

    // 带标签的构造函数
    node_ulabel(const std::string& lbl) : isEnd(false) {
        if (!lbl.empty()) {
            label = new char[lbl.length() + 1];
            strcpy(label, lbl.c_str());
        } else {
            label = nullptr;
        }
    }

    // 析构函数只需要处理 label
    ~node_ulabel() {
        delete[] label;
    }

    void add_child(std::shared_ptr<node_ulabel> child) {
     if (children.size() == children.capacity()) {
           children.reserve(children.size() + 1);
       }
        children.push_back(child);
    }

    std::shared_ptr<node_ulabel> find_child(const std::string& lbl) {
        if (lbl.empty()) return nullptr;
        
        for (const auto& child : children) {
            if (child && child->label && 
                strcmp(child->label, lbl.c_str()) == 0) {
                return child;
            }
        }
        return nullptr;
    }
};
// void print_memory_usage() {
//     struct rusage usage;
//     getrusage(RUSAGE_SELF, &usage);
//     std::cout << "Memory usage: " << usage.ru_maxrss << " KB" << std::endl;
// }
struct trie_ulabel { //上层字典树
      std::shared_ptr<node_ulabel> root;

    trie_ulabel() : root(std::make_shared<node_ulabel>()) {}


    // 插入 ulabel 和 b_trie_vlabel
   void insert_ulabel_btrie(int sb, int tb, const std::vector<std::string>& u_labels, const std::vector<std::string>& v_labels) {
        auto p = root;

        for (const auto& label : u_labels) {
            cout << label << endl;
            auto next = p->find_child(label);
            if (!next) {
                cout << 1 << endl;
                next = std::make_shared<node_ulabel>(label);
                p->add_child(next);
                cout << 1 << endl;
            }
            cout << 2 << endl;
            p = next;
        }
        p->isEnd = true;
       
        for (int i = sb; i <= tb; ++i) {
            p->b_trie_vlabel[i].insert(v_labels);
        }
        cout<<3<<endl;
    }

    void serialize(ofstream& outFile, node_ulabel* node, string prefix = "") {
        if (node == nullptr) return;

        if (node->label != nullptr) {  // 添加空指针检查
            if (node->isEnd) {
                outFile << prefix << node->label << "(end)" << endl;
            } else {
                outFile << prefix << node->label << endl;
            }
        }

        // 序列化下层字典树
        for (auto& it : node->b_trie_vlabel) {
            outFile << prefix << "  alpha:" << it.first << " (a_trie_vlabel)" << endl;
            it.second.serialize(outFile, it.second.root, prefix + "     ");
        }

        // 序列化子节点
        for (const auto& child : node->children) {
            serialize(outFile, child.get(), prefix + "   ");
        }
    }
};
// 计算 unordered_map 容器的内存占用
template <typename K, typename V>
size_t get_unordered_map_memory(const unordered_map<K, V*>& umap) {
    size_t total_size = sizeof(umap); // unordered_map 本身的静态内存大小

    for (const auto& kv : umap) {
        total_size += sizeof(kv.first) + sizeof(kv.second); // 键和值本身的内存大小
        total_size += sizeof(*kv.second); // 对象指针指向的节点的内存大小
    }

    return total_size;
}

// // 递归计算 node_vlabel 的内存占用
// size_t get_node_vlabel_memory(node_vlabel* node) {
//     if (!node) return 0;

//     size_t node_size = sizeof(*node); // 当前节点的内存占用
//     size_t next_size = 0; // 递归计算 next 的内存占用
    
//     // 遍历并计算 next 的内存占用
//     for (const auto& kv : node->children) {
//         next_size += sizeof(kv.first) + sizeof(kv.second); // 键和值的内存大小
//         next_size += get_node_vlabel_memory(kv.second); // 递归计算子节点的内存占用
//     }

//     return node_size + next_size;
// }

// // 递归计算 node_ulabel 的内存占用
// size_t get_node_ulabel_memory(node_ulabel* node) {
//     if (!node) return 0;

//     size_t node_size = sizeof(*node); // 当前节点的内存占用
//     size_t next_size = get_unordered_map_memory(node->next); // 递归计算 next 的内存占用
//     size_t btrie_size = 0;

//     // 遍历并计算 b_trie_vlabel 的内存占用
//     for (const auto& kv : node->b_trie_vlabel) {
//         btrie_size += sizeof(kv.first) + sizeof(kv.second); // 键和值的内存大小
//         btrie_size += get_node_vlabel_memory(kv.second.root); // 递归计算 b_trie_vlabel 中节点的内存占用
//     }

//     return node_size + next_size + btrie_size;
// }

// // 打印 node_vlabel 的内存占用
// void print_node_vlabel_memory(node_vlabel* node) {
//     size_t memory = get_node_vlabel_memory(node);
//     cout << "Node vlabel memory usage: " << memory << " bytes" << endl;
// }

// // 打印 node_ulabel 的内存占用
// void print_node_ulabel_memory(node_ulabel* node) {
//     size_t memory = get_node_ulabel_memory(node);
//     cout << "Node ulabel memory usage: " << memory << " bytes" << endl;
// }

struct node_graph
{
   /* data */
    unordered_map<int, vector< int>> edge_mp_u;
    unordered_map<int, vector< int>> edge_mp_v;
    unordered_set<int> u_set;
    unordered_set<int> v_set;
    vector<int> degree_u;
    vector<int>degree_v;
    
    unordered_map<int,vector<int>>u_a_Bmax;
    unordered_map<int,vector<int>>v_b_Amax;
    std::vector<bool> left_delete;
	std::vector<bool> right_delete;
	// std::vector<int> degree_of_left_node_initial;
    // std::vector<int> degree_of_right_node_initial;
	// for dynamic update
	unordered_map<int,vector<int>> left_index_change;
	unordered_map<int,vector<int>> right_index_change;

	unordered_map<int,vector<int>> left_index_delete;
	unordered_map<int,vector<int>> right_index_delete;
	unordered_map<int,vector<int>> left_index_old;
	unordered_map<int,vector<int>> right_index_old;
};
struct node_ulabel_graph {  //上层字典树节点
    std::string label;
    std::unordered_map<std::string, node_ulabel_graph*> next; // 使用指针避免深拷贝
    bool isEnd;
    node_graph node_data;
    // 默认构造函数

    
    node_ulabel_graph() : isEnd(false) {}
    node_ulabel_graph(const std::string& lbl) : label(lbl), isEnd(false) {}

    // 带标签的构造函数
    
};

struct trie_ulabels_graph { //子图字典树
    node_ulabel_graph *root=new node_ulabel_graph();


    // 插入 ulabel 和 b_trie_vlabel
    void insert_ulabel(const std::vector<std::string>& u_labels) {
        node_ulabel_graph *p =root;
        for (const auto& label : u_labels) {
            if (p->next.find(label) == p->next.end()) {
                p->next[label] =new node_ulabel_graph(label);
            }
            p = p->next[label];
        }
        p->isEnd = true;
    }
    void insert_ulabel_node(const std::vector<std::string>& u_labels,node_graph &node_data) {
        node_ulabel_graph *p =root;
        for (const auto& label : u_labels) {
            p = p->next[label];
        }
        p->node_data=node_data;
    }
    node_graph find_ulabel(const std::vector<std::string>& u_labels) {
        node_ulabel_graph *p =root;
        for (int i=0;i<u_labels.size()-1;i++) {
            std::string label = u_labels[i];
            p = p->next[label];
        }
        return p->node_data;
    }
};


struct node_vlabel_graph {  //xia层字典树节点
    std::string label;
    std::unordered_map<std::string, node_vlabel_graph*> next; // 使用指针避免深拷贝
    bool isEnd;
    std::unordered_map<std::string, node_graph> ulabel_graph; 
    // 默认构造函数


    
    node_vlabel_graph() : isEnd(false) {}
    node_vlabel_graph(const std::string& lbl) : label(lbl), isEnd(false) {}

    // 带标签的构造函数
    
};

struct trie_vlabels_graph { //子图字典树
    node_vlabel_graph *root=new node_vlabel_graph();


    // 插入 ulabel 和 b_trie_vlabel
    void insert_vlabel(const std::vector<std::string>& v_labels) {
        node_vlabel_graph *p =root;
        for (const auto& label : v_labels) {
            if (p->next.find(label) == p->next.end()) {
                p->next[label] =new node_vlabel_graph(label);
            }
            p = p->next[label];
        }
        p->isEnd = true;
    }
    void insert_vlabel_node(const std::vector<std::string>& v_labels,string ulabel,node_graph &node_data) {
        node_vlabel_graph *p =root;
        for (const auto& label : v_labels) {
            p = p->next[label];
        }
        p->ulabel_graph[ulabel]=node_data;
    }
    node_graph find(const std::vector<std::string>& v_labels,string ulabel)
    { 
       node_vlabel_graph *p =root;
        for (int i=0;i<v_labels.size()-1;i++) {
            const auto& label = v_labels[i];
            p = p->next[label];
        }
        return p->ulabel_graph[ulabel];
    }

    // 析构函数，递归删除所有节点
    ~trie_vlabels_graph() {
        delete_node(root);
    }

private:
    void delete_node(node_vlabel_graph* node) {
        if (node == nullptr) return;
        
        // 递归删除所有子节点
        for (auto& pair : node->next) {
            delete_node(pair.second);
        }
        
        delete node;
    }
};

#endif // NODE_H
