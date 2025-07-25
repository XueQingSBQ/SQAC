#include"graph_utils.h"

using namespace std;
// 生成给定标签集合的所有可能的非空子集组合
   vector<unordered_set<string>> generate_combinations(const vector<string>& labels) {
      vector<unordered_set<string>> combinations;
      int n = labels.size();

      // 使用二进制位生成子集
      for (int i = 1; i < (1 << n); ++i) {  // 从 1 开始，避免空集
         unordered_set<string> subset;
         for (int j = 0; j < n; ++j) {
               if (i & (1 << j)) {  // 如果第 j 位为 1，则将对应标签添加到子集
                  subset.insert(labels[j]);
               }
         }
         combinations.push_back(subset);
      }

      return combinations;
   }
    // 将 unordered_set 转换为 std::string，保证无序集合的一致性
   std::string to_string(const std::unordered_set<std::string>& uset) {
      std::vector<std::string> vec(uset.begin(), uset.end());
      std::sort(vec.begin(), vec.end());  // 排序
      std::stringstream ss;
      for (size_t i = 0; i < vec.size(); ++i) {
         if (i > 0) {
               ss << ",";
         }
         ss << vec[i];
      }
      return ss.str();
   }

      // 辅助函数：按照给定的分隔符，将字符串拆分为多个子字符串
   std::vector<std::string> split(const std::string& str, char delimiter) {
      std::vector<std::string> tokens;
      std::string token;
      std::istringstream tokenStream(str);
      while (std::getline(tokenStream, token, delimiter)) {
         tokens.push_back(token);
      }
      return tokens;
   }

   // 将 std::string 转换为 unordered_set<string>，按给定的分隔符拆分
   std::unordered_set<std::string> to_set(const std::string& str, char delimiter ) {
      std::unordered_set<std::string> result;
      std::vector<std::string> tokens = split(str, delimiter);
      for (const auto& token : tokens) {
         result.insert(token);
      }
      return result;
   }
   // 判断一个 pair 的两个集合是否是另一个 pair 的子集
   bool is_subset(const std::string& subset_str, const std::string& superset_str) {
      unordered_set<string> subset_u = to_set(subset_str);  // 使用 to_set 函数
      unordered_set<string> superset_u = to_set(superset_str);

      // 判断子集关系（提前剪枝）
      if (subset_u.size() > superset_u.size()) {
         return false;  // 剪枝：如果 subset 的大小比 superset 大，不可能是子集
      }

      // 判断是否是子集
      return std::all_of(subset_u.begin(), subset_u.end(),
                        [&](const std::string& label) { return superset_u.count(label) > 0; });
   }


 vector<vector<string>> generate_combinations_1( vector<string> &labels) {
   
   vector<vector<string>> combinations;
    int n = labels.size();
   vector<vector<vector<string>>> combination_indices(n);
  
   for(int i=n-1;i>=0;i--)
   {
      vector<string> combination;
      combination.push_back(labels[i]);
      combination_indices[i].push_back(combination);
      for(int j=i+1;j<n;j++)
      {
         for(auto combination_index : combination_indices[j])
         {  vector<string> combination1;
            combination1.push_back(labels[i]);
            
            for(auto label : combination_index)
            {
               combination1.push_back(label);
            }
            combination_indices[i].push_back(combination1);
         }
      }
   }
   for(int len=1;len<=n;len++)
   {
      for(int i=0;i<n;i++)
      {
         for(auto combination : combination_indices[i])
         {  if(combination.size()==len)
             combinations.push_back(combination);
         }
      }
   }
   
   return combinations;
}
vector<uint32_t> generate_32bit_combinations(const vector<string>& labels) {
   const int n = labels.size();
   if (n > 32) throw invalid_argument("标签数量超过32个，无法用32位掩码表示");

   vector<vector<uint32_t>> mask_groups(n);
   vector<uint32_t> result;

   // 逆序生成组合掩码
   for (int i = n-1; i >= 0; --i) {
       const uint32_t base_mask = 1U << i;  // 使用32位无符号整型
       mask_groups[i].push_back(base_mask);

       for (int j = i+1; j < n; ++j) {
           for (uint32_t mask : mask_groups[j]) {
               mask_groups[i].push_back(base_mask | mask);
           }
       }
   }

   // 按长度和顺序收集结果
   for (int len=1; len<=n; ++len) {
       for (int i=0; i<n; ++i) {
           for (uint32_t mask : mask_groups[i]) {
               if (__builtin_popcount(mask) == len) {  // 使用32位计数函数
                   result.push_back(mask);
               }
           }
       }
   }

   return result;
}
unordered_map<string,uint32_t> generate_32bit_map_ucombinations( vector<string>& labels,const std::unordered_map<std::string, std::unordered_set<int>>& labelu) {
    int n = labels.size();
    
   sort(labels.begin(), labels.end(), [&labelu]( string& a, string& b) {
      return labelu.at(a).size() > labelu.at(b).size();
   });
   if (n > 32) throw invalid_argument("标签数量超过32个，无法用32位掩码表示");

   unordered_map<string,uint32_t> result;

  
   // 逆序生成组合掩码
   for (int i = n-1; i >= 0; --i) {
       const uint32_t base_mask = 1U << i;  // 使用32位无符号整型
      result[labels[i]]=base_mask;
   }

   return result;
}
unordered_map<string,uint32_t> generate_32bit_map_vcombinations( vector<string>& labels, const std::unordered_map<std::string, std::unordered_set<int>>& labelv) {
   int n = labels.size();
   
  sort(labels.begin(), labels.end(), [&labelv]( string& a, string& b) {
     return labelv.at(a).size() > labelv.at(b).size();
  });
  if (n > 32) throw invalid_argument("标签数量超过32个，无法用32位掩码表示");

  unordered_map<string,uint32_t> result;

 
  // 逆序生成组合掩码
  for (int i = n-1; i >= 0; --i) {
      const uint32_t base_mask = 1U << i;  // 使用32位无符号整型
     result[labels[i]]=base_mask;
  }

  return result;
}
// 掩码到标签的转换函数
vector<string> mask_to_labels(uint32_t mask, const vector<string>& labels) {
   vector<string> combination;
   for (int i=0; i<labels.size(); ++i) {
       if (mask & (1U << i)) {  // 使用32位掩码检查
           combination.push_back(labels[i]);
       }
   }
   return combination;
}

// 使用GCC/Clang内置函数（高效）
uint32_t get_parent_mask_gcc(uint32_t label_mask) {
   if (label_mask == 0) return 0; // 处理全0情况
   int highest_bit = 31 - __builtin_clz(label_mask); // 计算最高位位置
   return label_mask & ~(1U << highest_bit); // 清除最高位的1
}
