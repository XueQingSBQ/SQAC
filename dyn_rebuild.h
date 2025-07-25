#pragma once
#ifndef DYN_REBUILD_H
#define DYN_REBUILD_H
#include<iostream>
#include<queue>
#include<chrono>
#include <climits>
#include<stack>
#include<unordered_set>
#include<unordered_map>
#include<map>
#include <vector>
#include <algorithm>
#include "node.h"
using namespace std;

typedef  int vid_t;
struct traversal_info_insertion {
	int current_supports = 0;
	//		int necessary_sup = 0;
	std::vector<vid_t> neighbors_in_candidates;
};

struct traversal_info_removal {
	int current_supports = 0;
	//		int necessary_sup = 0;
	std::vector<vid_t> neighbors_not_in_candidates; // not in candidates but in visited nodes
	std::vector<vid_t> nodes_to_be_visited; // in case this node is selected into candidates after other nodes selected into candidates
};

// 定义自定义哈希函数，不能完全避免哈希冲突
struct pair_hash {
	template <class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2>& pair) const {
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};

class TopX {
public:
	TopX(int x) : x(x) {}

	// 插入新元素
	void insert(int num) {
		if (minHeap.size() < x) {
			minHeap.push(num);  // 堆未满时直接插入
		} else if (num > minHeap.top()) {
			minHeap.pop();       // 弹出堆顶（最小值）
			minHeap.push(num);   // 插入新值
		}
	}

	// 获取第x大的元素
	int get_xth_largest() {
		if (minHeap.size() == x) {
			return minHeap.top();  // 堆顶是第x大的元素
		} else {
			return 0;
		}
	}

	// 清空当前的堆
	void clear() {
		while (!minHeap.empty()) {
			minHeap.pop();  // 清空堆
		}
	}

	// 调整x的大小，清空堆
	void resize(int new_x) {
		if (new_x < 1) {
			throw std::invalid_argument("Size must be at least 1.");
		}
		x = new_x;
		clear();  // 清空堆，等待新的插入
	}

	// 删除堆顶元素，获得下一个更大的元素
	void pop_top() {
		if (!minHeap.empty()) {
			minHeap.pop();  // 移除堆顶（第x大的元素）
			x--;
		} else {
			throw std::runtime_error("Heap is empty.");
		}
	}

	// 判断堆是否为空
	bool is_empty() const {
		return minHeap.empty();
	}


private:
	int x;  // 要维护的第x大的元素
	std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;  // 使用最小堆
};
struct CascadeFrame {
	vid_t node;
	bool left_side;
};
struct skyline_block {
	std::map<int,int> mapset;
	//bicore_index_block* next = NULL;
};
int upgrade_vbeta_removal(node_graph& g, vid_t v, int alpha, int beta) ;
void dyn_update_removal_dfs(node_graph& g, vid_t node, int alpha, int beta, bool left_side,
	std::unordered_set<vid_t>& visited_nodes_left, std::unordered_set<vid_t>& visited_nodes_right,
	std::unordered_set<vid_t>& candidate_nodes_left, std::unordered_set<vid_t>& candidate_nodes_right,
	std::unordered_map<vid_t, traversal_info_removal>& visited_nodes_2_info_left, std::unordered_map<vid_t, traversal_info_removal>& visited_nodes_2_info_right);
void casacade_insert_candidate_nodes(node_graph& g, vid_t node, int alpha, int beta, bool left_side,
	unordered_set<vid_t>& visited_nodes_left, unordered_set<vid_t>& visited_nodes_right,
	unordered_set<vid_t>& candidate_nodes_left, unordered_set<vid_t>& candidate_nodes_right,
	unordered_map<vid_t, traversal_info_removal>& visited_nodes_2_info_left, unordered_map<vid_t, traversal_info_removal>& visited_nodes_2_info_right) ;
void dyn_update_removal_dfs(node_graph& g, vid_t node, int alpha, int beta, bool left_side,
	std::unordered_set<vid_t>& visited_nodes_left, std::unordered_set<vid_t>& visited_nodes_right,
	std::unordered_set<vid_t>& candidate_nodes_left, std::unordered_set<vid_t>& candidate_nodes_right,
	std::unordered_map<vid_t, traversal_info_removal>& visited_nodes_2_info_left, std::unordered_map<vid_t, traversal_info_removal>& visited_nodes_2_info_right);
  
vector<int> findDominantPairs_u(const vector<int>& nums, vector<int>& result) ;

vector<int> findDominantPairs_v(const vector<int>& nums, vector<int>& result) ;
int compute_vbeta_regarding_specific_alpha(node_graph& g, vid_t v, int alpha) ;
int compute_ualpha_regarding_specific_beta(node_graph& g, vid_t u, int beta);
void dyn_crossUpdate_addition_right(node_graph& g, int alpha, int k_x, vid_t v) ;
void dyn_crossUpdate_addition_left(node_graph& g, int beta, int k_x, vid_t u) ;
int compute_lower_bound_left(node_graph& g, vid_t u, int alpha, TopX& topX);
int compute_lower_bound_right(node_graph& g, vid_t v, int beta, TopX& topX);
int upgrade_vbeta_insertion(node_graph& g, vid_t v, int alpha, int beta) ;


void casacade_remove_candidate_nodes(
	vid_t start_node, int alpha, int beta, bool start_left_side,
	unordered_set<vid_t>& visited_nodes_left,
	unordered_set<vid_t>& visited_nodes_right,
	unordered_set<vid_t>& candidate_nodes_left,
	unordered_set<vid_t>& candidate_nodes_right,
	unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_left,
	unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_right);
void dyn_update_insertion_dfs(node_graph& g, vid_t node, int alpha, int beta, bool left_side,
		unordered_set<vid_t>& visited_nodes_left, unordered_set<vid_t>& visited_nodes_right,
		unordered_set<vid_t>& candidate_nodes_left, unordered_set<vid_t>& candidate_nodes_right,
		unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_left, unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_right) ;

void case_alpha_increase_left(node_graph& g, vid_t u, vid_t v, int current_alpha, int current_beta, unordered_set<std::pair<int, int>, pair_hash>& have_update, unordered_set<int>& have_update_alpha,
		unordered_set<vid_t>& visited_nodes_left, unordered_set<vid_t>& visited_nodes_right,
        unordered_set<vid_t>& candidate_nodes_left, unordered_set<vid_t>& candidate_nodes_right, 
        unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_left, unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_right, TopX& topX, int& bfs_times);	void case_beta_increase_right(node_graph& g, vid_t u, vid_t v, int current_alpha, int current_beta, unordered_set<std::pair<int, int>, pair_hash>& have_update, unordered_set<int>& have_update_beta,
		unordered_set<vid_t>& visited_nodes_left, unordered_set<vid_t>& visited_nodes_right,
        unordered_set<vid_t>& candidate_nodes_left, unordered_set<vid_t>& candidate_nodes_right, 
        unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_left, unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_right, TopX& topX, int& bfs_times);

void case_beta_increase_left(node_graph& g, vid_t u, vid_t v, int current_alpha, int current_beta, unordered_set<std::pair<int, int>, pair_hash>& have_update, unordered_set<int>& have_update_alpha,
		unordered_set<vid_t>& visited_nodes_left, unordered_set<vid_t>& visited_nodes_right,
        unordered_set<vid_t>& candidate_nodes_left, unordered_set<vid_t>& candidate_nodes_right, 
        unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_left, unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_right, TopX& topX, int& bfs_times);	
		void case_alpha_increase_right(node_graph& g, vid_t u, vid_t v, int current_alpha, int current_beta, unordered_set<std::pair<int, int>, pair_hash>& have_update, unordered_set<int>& have_update_beta,
		unordered_set<vid_t>& visited_nodes_left, unordered_set<vid_t>& visited_nodes_right,
        unordered_set<vid_t>& candidate_nodes_left, unordered_set<vid_t>& candidate_nodes_right, 
        unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_left, unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_right, TopX& topX, int& bfs_times);
double update_skyline_index_swap_with_bfs(node_graph& g, vid_t u, vid_t v,unordered_set<vid_t>& update_u_set,unordered_set<vid_t>& update_v_set);
skyline_block findDominantPairs_u_map(const vector<int>& nums, skyline_block&result) ;
skyline_block findDominantPairs_v_map(const vector<int>& nums, skyline_block& result) ;
int find_kth(vector<int> B, int n, int k);
double update_skyline_index_swap_with_bfs_delete(node_graph& g,  vid_t u, vid_t v,unordered_set<int> &update_u_set_end, unordered_set<int> &update_v_set_end);


#endif // !DYN_REBUILD_H
