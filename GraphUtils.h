#ifndef GRAPHUTILS_H
#define GRAPHUTILS_H 
#include<iostream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
using namespace std;
// 生成给定标签集合的所有可能的非空子集组合
   vector<unordered_set<string>> generate_combinations(const vector<string>& labels);
    // 将 unordered_set 转换为 std::string，保证无序集合的一致性
   std::string to_string(const std::unordered_set<std::string>& uset);

      // 辅助函数：按照给定的分隔符，将字符串拆分为多个子字符串
   std::vector<std::string> split(const std::string& str, char delimiter);
   // 将 std::string 转换为 unordered_set<string>，按给定的分隔符拆分
   std::unordered_set<std::string> to_set(const std::string& str, char delimiter = ',');
   // 判断一个 pair 的两个集合是否是另一个 pair 的子集
   bool is_subset(const std::string& subset_str, const std::string& superset_str) ;


 vector<vector<string>> generate_combinations_1( vector<string> &labels);
 vector<uint32_t> generate_32bit_combinations(const vector<string>& labels) ;
unordered_map<string,uint32_t> generate_32bit_map_ucombinations( vector<string>& labels,const std::unordered_map<std::string, std::unordered_set<int>>& labelu) ;
unordered_map<string,uint32_t> generate_32bit_map_vcombinations( vector<string>& labels, const std::unordered_map<std::string, std::unordered_set<int>>& labelv);
// 掩码到标签的转换函数
vector<string> mask_to_labels(uint32_t mask, const vector<string>& labels) ;
// 使用GCC/Clang内置函数（高效）
uint32_t get_parent_mask_gcc(uint32_t label_mask);
#endif