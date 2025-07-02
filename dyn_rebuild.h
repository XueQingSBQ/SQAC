#pragma once
#ifndef DYN_REBUILD_H
#define DYN_REBUILD_H
#include<iostream>
#include<queue>
#include<chrono>
#include <climits>
#include<stack>
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

int upgrade_vbeta_removal(node_graph& g, vid_t v, int alpha, int beta) {
	int a1 = g.v_b_Amax[v][beta];
	int a2 = -1;
	if (beta + 1 < g.v_b_Amax[v].size()) {
		a2 = g.v_b_Amax[v][beta + 1];
	}
	if (a1 < alpha) {
		return -1;
	}
	else if (a1 >= alpha && a2 < alpha) {
		return beta;
	}
	else {
		return 1e9;
	}
}	
void dyn_update_removal_dfs(node_graph& g, vid_t node, int alpha, int beta, bool left_side,
	std::unordered_set<vid_t>& visited_nodes_left, std::unordered_set<vid_t>& visited_nodes_right,
	std::unordered_set<vid_t>& candidate_nodes_left, std::unordered_set<vid_t>& candidate_nodes_right,
	std::unordered_map<vid_t, traversal_info_removal>& visited_nodes_2_info_left, std::unordered_map<vid_t, traversal_info_removal>& visited_nodes_2_info_right);
void casacade_insert_candidate_nodes(node_graph& g, vid_t node, int alpha, int beta, bool left_side,
	unordered_set<vid_t>& visited_nodes_left, unordered_set<vid_t>& visited_nodes_right,
	unordered_set<vid_t>& candidate_nodes_left, unordered_set<vid_t>& candidate_nodes_right,
	unordered_map<vid_t, traversal_info_removal>& visited_nodes_2_info_left, unordered_map<vid_t, traversal_info_removal>& visited_nodes_2_info_right) {
	if (left_side) {
		candidate_nodes_left.insert(node);
		for (auto v : visited_nodes_2_info_left[node].neighbors_not_in_candidates) {
			if (candidate_nodes_right.find(v) == candidate_nodes_right.end()) {
				visited_nodes_2_info_right[v].current_supports--;
				if (visited_nodes_2_info_right[v].current_supports < beta) {
					casacade_insert_candidate_nodes(g, v, alpha, beta, !left_side,
						visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right,
						visited_nodes_2_info_left, visited_nodes_2_info_right);
				}
			}
		}
		for (auto v : visited_nodes_2_info_left[node].nodes_to_be_visited) {
			if (visited_nodes_right.find(v) == visited_nodes_right.end()) {
				dyn_update_removal_dfs(g, v, alpha, beta, !left_side,
					visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right,
					visited_nodes_2_info_left, visited_nodes_2_info_right);
			}
		}
	}
	else {
		candidate_nodes_right.insert(node);
		for (auto u : visited_nodes_2_info_right[node].neighbors_not_in_candidates) {
			if (candidate_nodes_left.find(u) == candidate_nodes_left.end()) {
				visited_nodes_2_info_left[u].current_supports--;
				if (visited_nodes_2_info_left[u].current_supports < alpha) {
					casacade_insert_candidate_nodes(g, u, alpha, beta, !left_side,
						visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right,
						visited_nodes_2_info_left, visited_nodes_2_info_right);
				}
			}
		}
		for (auto u : visited_nodes_2_info_right[node].nodes_to_be_visited) {
			if (visited_nodes_left.find(u) == visited_nodes_left.end()) {
				dyn_update_removal_dfs(g, u, alpha, beta, !left_side,
					visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right,
					visited_nodes_2_info_left, visited_nodes_2_info_right);
			}
		}
	}
}
void dyn_update_removal_dfs(node_graph& g, vid_t node, int alpha, int beta, bool left_side,
	std::unordered_set<vid_t>& visited_nodes_left, std::unordered_set<vid_t>& visited_nodes_right,
	std::unordered_set<vid_t>& candidate_nodes_left, std::unordered_set<vid_t>& candidate_nodes_right,
	std::unordered_map<vid_t, traversal_info_removal>& visited_nodes_2_info_left, std::unordered_map<vid_t, traversal_info_removal>& visited_nodes_2_info_right)
  {     if (!left_side && node == 4) {
			int kkkkkk = 0;// for debug
		}
		if (left_side) {
			visited_nodes_left.insert(node);
			vector<vid_t>& neigh = g.edge_mp_u[node];
			for (auto v : neigh) {
				// could be a candidate only if v's deg >= beta
				// aim at finding whether v still belongs to alpha-beta-core
				if (g.degree_v[v] >= beta) {
					// could be optimized???
					int vbeta = upgrade_vbeta_removal(g, v, alpha, beta);
					if (vbeta > beta) {
						visited_nodes_2_info_left[node].current_supports++;
					}
					else if (vbeta == beta) {
						if (visited_nodes_right.find(v) == visited_nodes_right.end()) {
							visited_nodes_2_info_left[node].current_supports++;
							visited_nodes_2_info_left[node].nodes_to_be_visited.push_back(v);
						}
						else if (visited_nodes_right.find(v) != visited_nodes_right.end() && candidate_nodes_right.find(v) == candidate_nodes_right.end()) {
							visited_nodes_2_info_left[node].current_supports++;
							visited_nodes_2_info_left[node].neighbors_not_in_candidates.push_back(v);
							visited_nodes_2_info_right[v].neighbors_not_in_candidates.push_back(node);
						}
						else {
							//do nothing
						}
					}
					else {
						// do nothing
					}
				}
			}
			// expand current node
			if (visited_nodes_2_info_left[node].current_supports < alpha) {
				casacade_insert_candidate_nodes(g, node, alpha, beta, left_side,
					visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right,
					visited_nodes_2_info_left, visited_nodes_2_info_right);
				for (auto v : visited_nodes_2_info_left[node].nodes_to_be_visited) {
					if (visited_nodes_right.find(v) == visited_nodes_right.end()) {
						dyn_update_removal_dfs(g, v, alpha, beta, !left_side,
							visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right,
							visited_nodes_2_info_left, visited_nodes_2_info_right);
					}
				}
			}
		}
		else {
			visited_nodes_right.insert(node);
			vector<vid_t>& neigh = g.edge_mp_v[node];
			for (auto u : neigh) {
				if (g.degree_u[u] >= alpha) {
					int ubeta = g.u_a_Bmax[u][alpha];
					if (ubeta > beta) {
						visited_nodes_2_info_right[node].current_supports++;
					}
					else if (ubeta == beta) {
						if (visited_nodes_left.find(u) == visited_nodes_left.end()) {
							visited_nodes_2_info_right[node].current_supports++;
							visited_nodes_2_info_right[node].nodes_to_be_visited.push_back(u);
						}
						// can be simplified
						else if (visited_nodes_left.find(u) != visited_nodes_left.end() && candidate_nodes_left.find(u) == candidate_nodes_left.end()) {
							visited_nodes_2_info_right[node].current_supports++;
							// this node is doomed! must be the end point of insertion edge otherwise it can not be visited!
							if (g.v_b_Amax[node][beta] < alpha || g.degree_v[node] < beta) continue;
							visited_nodes_2_info_right[node].neighbors_not_in_candidates.push_back(u);
							visited_nodes_2_info_left[u].neighbors_not_in_candidates.push_back(node);
						}
						else {
							//do nothing
						}
					}
					else {
						// do nothing
					}
				}
			}
			if (visited_nodes_2_info_right[node].current_supports < beta) {
				casacade_insert_candidate_nodes(g, node, alpha, beta, left_side,
					visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right,
					visited_nodes_2_info_left, visited_nodes_2_info_right);
				for (auto u : visited_nodes_2_info_right[node].nodes_to_be_visited) {
					if (visited_nodes_left.find(u) == visited_nodes_left.end()) {
						dyn_update_removal_dfs(g, u, alpha, beta, !left_side,
							visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right,
							visited_nodes_2_info_left, visited_nodes_2_info_right);
					}
				}
			}
		}
	}
vector<int> findDominantPairs_u(const vector<int>& nums, vector<int>& result) {
		int maxY = INT_MIN;  // 当前的最大y值
		result.reserve(nums.size()); // 预留空间，避免频繁的扩容

		// 从右向左遍历vector
		for (int i = nums.size() - 1; i >= 0; --i) {
			// 如果当前元素比maxY大，说明该元素不被支配
			if (nums[i] > maxY) {
				// 将该元素的值和下标加入结果
				result.insert(result.begin(), {i, nums[i]});
				maxY = nums[i];             // 更新最大y值
			}
		}
		result.shrink_to_fit(); // 将预留空间缩减为实际使用的大小
		return result;
	}

	vector<int> findDominantPairs_v(const vector<int>& nums, vector<int>& result) {
		int maxY = INT_MIN;
		result.reserve(nums.size()); 

		for (int i = nums.size() - 1; i >= 0; --i) {
			if (nums[i] > maxY) {
				result.push_back(nums[i]);
				result.push_back(i);
				maxY = nums[i]; 
			}
		}
		result.shrink_to_fit();
		return result;
	}
int compute_vbeta_regarding_specific_alpha(node_graph& g, vid_t v, int alpha) {
		int vbeta = -1;
		bool changed = false;
		for (int b = 1; b <= g.v_b_Amax[v].size() - 1; b++) {
			if (g.v_b_Amax[v][b] < alpha) {
				vbeta = b - 1;
				changed = true;
				break;
			}
		}
		if (!changed) {
			vbeta = g.v_b_Amax[v].size() - 1;
		}
		if (vbeta < 0) {
			//cout << "error: vbeta" << endl;
		}
		return vbeta;
	}
	int compute_ualpha_regarding_specific_beta(node_graph& g, vid_t u, int beta) {
		int ualpha = -1;
		bool changed = false;
		for (int b = 1; b <= g.u_a_Bmax[u].size() - 1; b++) {
			if (g.u_a_Bmax[u][b] < beta) {
				ualpha = b - 1;
				changed = true;
				break;
			}
		}
		if (!changed) {
			ualpha = g.u_a_Bmax[u].size() - 1;
		}
		if (ualpha < 0) {
			//cout << "error: vbeta" << endl;
		}
		return ualpha;
	}
		void dyn_crossUpdate_addition_right(node_graph& g, int alpha, int k_x, vid_t v) {
		int oldalpha;
		for (int beta = k_x; beta > 0; beta--) {
			oldalpha = g.v_b_Amax[v][beta];
			if (oldalpha < alpha) {
				g.v_b_Amax[v][beta] = alpha;
			}else {
				break;
			}
		}
	}

	void dyn_crossUpdate_addition_left(node_graph& g, int beta, int k_x, vid_t u) {
		int oldbeta;
		for (int alpha = k_x; alpha > 0; alpha--) {
			oldbeta = g.u_a_Bmax[u][alpha];
			if (oldbeta < beta){
				g.u_a_Bmax[u][alpha] = beta;
			}else{
				break;
			}
		}
	}
	int compute_lower_bound_left(node_graph& g, vid_t u, int alpha, TopX& topX){
		// 计算left side节点在alpha下beta的lower bound
		topX.resize(alpha);
		vector<vid_t>& neigh = g.edge_mp_u[u];
		int ubeta = g.u_a_Bmax[u][alpha];
		for (vid_t nbr_v : neigh) {
			// 筛选掉不可能属于topX的nbr
			if (ubeta == 0 || g.v_b_Amax[nbr_v][ubeta] >= alpha){
				topX.insert(compute_vbeta_regarding_specific_alpha(g, nbr_v, alpha));
			}
		}
		int bound = topX.get_xth_largest();
		if (ubeta < bound) g.u_a_Bmax[u][alpha] = bound;
		// cout << "alpha: " << alpha << " bound: " << bound << endl;
		return bound;
	}

	int compute_lower_bound_right(node_graph& g, vid_t v, int beta, TopX& topX){
		// 计算right side节点在beta下alpha的lower bound
		topX.resize(beta);
		vector<vid_t>& neigh = g.edge_mp_v[v];
		int valpha = g.v_b_Amax[v][beta];
		for (vid_t nbr_u : neigh) {
			// 筛选掉不可能属于topX的nbr
			if (valpha == 0 || g.u_a_Bmax[nbr_u][valpha] >= beta){
				topX.insert(compute_ualpha_regarding_specific_beta(g, nbr_u, beta));
			}
		}
		int bound = topX.get_xth_largest();
		if (valpha < bound) g.v_b_Amax[v][beta] = bound;
		return bound;
	}
	int upgrade_vbeta_insertion(node_graph& g, vid_t v, int alpha, int beta) {
		int a1 = g.v_b_Amax[v][beta];
		if (beta == 0) {
			a1 = alpha;
		}
		int a2 = g.v_b_Amax[v][beta + 1];
		if (a1 < alpha) {
			return -1;
		}
		else if (a1 >= alpha && a2 < alpha) {
			return beta;
		}
		else {
			return 1e9;
		}
	}
	// void casacade_remove_candidate_nodes(vid_t node, int alpha, int beta, bool left_side,
	// 	unordered_set<vid_t>& visited_nodes_left, unordered_set<vid_t>& visited_nodes_right,
	// 	unordered_set<vid_t>& candidate_nodes_left, unordered_set<vid_t>& candidate_nodes_right,
	// 	unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_left, unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_right) {
	// 	if (left_side) {
	// 		if (candidate_nodes_left.find(node) == candidate_nodes_left.end()) return;
	// 		candidate_nodes_left.erase(node);
	// 		for (auto v : visited_nodes_2_info_left[node].neighbors_in_candidates) {
	// 			if (candidate_nodes_right.find(v) != candidate_nodes_right.end()) {
	// 				visited_nodes_2_info_right[v].current_supports--;
	// 				if (visited_nodes_2_info_right[v].current_supports <= beta) {
	// 					casacade_remove_candidate_nodes(v, alpha, beta, !left_side,
	// 						visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right,
	// 						visited_nodes_2_info_left, visited_nodes_2_info_right);
	// 				}
	// 			}
	// 		}
	// 	}
	// 	else {
	// 		if (candidate_nodes_right.find(node) == candidate_nodes_right.end()) return;
	// 		candidate_nodes_right.erase(node);
	// 		for (auto u : visited_nodes_2_info_right[node].neighbors_in_candidates) {
	// 			if (candidate_nodes_left.find(u) != candidate_nodes_left.end()) {
	// 				visited_nodes_2_info_left[u].current_supports--;
	// 				if (visited_nodes_2_info_left[u].current_supports < alpha) {
	// 					casacade_remove_candidate_nodes(u, alpha, beta, !left_side,
	// 						visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right,
	// 						visited_nodes_2_info_left, visited_nodes_2_info_right);
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	struct CascadeFrame {
		vid_t node;
		bool left_side;
	};
	void casacade_remove_candidate_nodes(
		vid_t start_node, int alpha, int beta, bool start_left_side,
		unordered_set<vid_t>& visited_nodes_left,
		unordered_set<vid_t>& visited_nodes_right,
		unordered_set<vid_t>& candidate_nodes_left,
		unordered_set<vid_t>& candidate_nodes_right,
		unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_left,
		unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_right)
	{
		stack<CascadeFrame> stk;
		stk.push({start_node, start_left_side});
	
		while (!stk.empty()) {
			CascadeFrame frame = stk.top();
			stk.pop();
	
			vid_t node = frame.node;
			bool left_side = frame.left_side;
	
			if (left_side) {
				if (candidate_nodes_left.find(node) == candidate_nodes_left.end()) continue;
				candidate_nodes_left.erase(node);
				// std::cout << "Deleted left: " << node << std::endl;
	
				auto it = visited_nodes_2_info_left.find(node);
				if (it != visited_nodes_2_info_left.end()) {
					for (auto v : it->second.neighbors_in_candidates) {
						if (candidate_nodes_right.find(v) != candidate_nodes_right.end()) {
							auto it_r = visited_nodes_2_info_right.find(v);
							if (it_r != visited_nodes_2_info_right.end()) {
								it_r->second.current_supports--;
								if (it_r->second.current_supports <= beta) {
									stk.push({v, false});  // 递归右边
								}
							}
						}
					}
				}
			} else {
				if (candidate_nodes_right.find(node) == candidate_nodes_right.end()) continue;
				candidate_nodes_right.erase(node);
				// std::cout << "Deleted right: " << node << std::endl;
	
				auto it = visited_nodes_2_info_right.find(node);
				if (it != visited_nodes_2_info_right.end()) {
					for (auto u : it->second.neighbors_in_candidates) {
						if (candidate_nodes_left.find(u) != candidate_nodes_left.end()) {
							auto it_l = visited_nodes_2_info_left.find(u);
							if (it_l != visited_nodes_2_info_left.end()) {
								it_l->second.current_supports--;
								if (it_l->second.current_supports < alpha) {
									stk.push({u, true});  // 递归左边
								}
							}
						}
					}
				}
			}
		}
	}
	
void dyn_update_insertion_dfs(node_graph& g, vid_t node, int alpha, int beta, bool left_side,
		unordered_set<vid_t>& visited_nodes_left, unordered_set<vid_t>& visited_nodes_right,
		unordered_set<vid_t>& candidate_nodes_left, unordered_set<vid_t>& candidate_nodes_right,
		unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_left, unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_right) {

		vector<pair<pair<vid_t,vid_t>,bool>> stack; // pre cur left_side

		stack.reserve(g.degree_u.size()+g.degree_v.size()); // 提前分配栈的容量
		stack.emplace_back(make_pair(-1, node), left_side);
		bool start_root = true;

		
		while (!stack.empty()) {

			auto [pre_cur, is_left] = stack.back();
			stack.pop_back();

			vid_t pre = pre_cur.first;
			vid_t node = pre_cur.second;

			if(is_left){
				if (visited_nodes_left.count(node)) continue;
				if (!start_root && !candidate_nodes_right.count(pre)) continue;
				start_root = false;

				candidate_nodes_left.insert(node);
				visited_nodes_left.insert(node);
				auto& neigh = g.edge_mp_u[node];

				for (auto v : neigh) {
					if (g.degree_v[v] > beta) {
						int vbeta = upgrade_vbeta_insertion(g, v, alpha, beta);

						if (vbeta > beta) {
							visited_nodes_2_info_left[node].current_supports++;
						} else if (vbeta == beta) {
							if (visited_nodes_right.count(v) == 0) {
								visited_nodes_2_info_left[node].current_supports++;
								stack.push_back(make_pair(make_pair(node,v),false));
							} else if (candidate_nodes_right.count(v)) {
								visited_nodes_2_info_left[node].current_supports++;
								visited_nodes_2_info_left[node].neighbors_in_candidates.push_back(v);
								visited_nodes_2_info_right[v].neighbors_in_candidates.push_back(node);
							}
						}
					}
				}
				//
				if (visited_nodes_2_info_left[node].current_supports < alpha) {
					casacade_remove_candidate_nodes(node, alpha, beta, true,
						visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right,
						visited_nodes_2_info_left, visited_nodes_2_info_right);
				}

			}
			else {
				if (visited_nodes_right.count(node)) continue;
				if (!start_root && !candidate_nodes_left.count(pre)) continue;

				start_root = false;
				candidate_nodes_right.insert(node);
				visited_nodes_right.insert(node);
				auto& neigh = g.edge_mp_v[node];

				for (auto u : neigh) {
					if (g.degree_u[u] >= alpha) {
						int ubeta = g.u_a_Bmax[u][alpha];
						if (ubeta > beta) {
							visited_nodes_2_info_right[node].current_supports++;
						} else if (ubeta == beta) {
							if (visited_nodes_left.find(u) == visited_nodes_left.end()) {
								visited_nodes_2_info_right[node].current_supports++;
								stack.push_back(make_pair(make_pair(node, u), true));
							} else if (visited_nodes_left.find(u) != visited_nodes_left.end() && candidate_nodes_left.find(u) != candidate_nodes_left.end()) {
								visited_nodes_2_info_right[node].current_supports++;
								visited_nodes_2_info_right[node].neighbors_in_candidates.push_back(u);
								visited_nodes_2_info_left[u].neighbors_in_candidates.push_back(node);
							}
						}
					}
				}

				if (visited_nodes_2_info_right[node].current_supports <= beta) {
					casacade_remove_candidate_nodes(node, alpha, beta, false,
						visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right,
						visited_nodes_2_info_left, visited_nodes_2_info_right);
				}
			}
		}
}

void case_alpha_increase_left(node_graph& g, vid_t u, vid_t v, int current_alpha, int current_beta, unordered_set<std::pair<int, int>, pair_hash>& have_update, unordered_set<int>& have_update_alpha,
		unordered_set<vid_t>& visited_nodes_left, unordered_set<vid_t>& visited_nodes_right,
        unordered_set<vid_t>& candidate_nodes_left, unordered_set<vid_t>& candidate_nodes_right, 
        unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_left, unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_right, TopX& topX, int& bfs_times){
		int bound = compute_lower_bound_left(g, u, current_alpha + 1, topX);
		// 排除重复更新
		if (have_update.find({current_alpha + 1, bound + 1}) != have_update.end()) return;

		//清空容器
		visited_nodes_left.clear(); visited_nodes_right.clear();
		candidate_nodes_left.clear(); candidate_nodes_right.clear();
		visited_nodes_2_info_left.clear(); visited_nodes_2_info_right.clear();
		// 调用 DFS 计算候选集
		bfs_times++;
        dyn_update_insertion_dfs(g, u, current_alpha + 1, bound, true, visited_nodes_left, visited_nodes_right,
            candidate_nodes_left, candidate_nodes_right, visited_nodes_2_info_left, visited_nodes_2_info_right);

		// 更新候选集
		for (auto u : candidate_nodes_left) {
			if (g.u_a_Bmax[u][current_alpha + 1] < bound + 1) g.u_a_Bmax[u][current_alpha + 1] = bound + 1;
			// dyn_crossUpdate_addition_left(g, bound + 1, current_alpha + 1, u);
		}
		for (auto v : candidate_nodes_right) {
			dyn_crossUpdate_addition_right(g, current_alpha + 1, bound + 1, v);
		}

		have_update.insert({current_alpha + 1, bound + 1});
		have_update_alpha.insert(current_alpha + 1);
	}

	void case_beta_increase_right(node_graph& g, vid_t u, vid_t v, int current_alpha, int current_beta, unordered_set<std::pair<int, int>, pair_hash>& have_update, unordered_set<int>& have_update_beta,
		unordered_set<vid_t>& visited_nodes_left, unordered_set<vid_t>& visited_nodes_right,
        unordered_set<vid_t>& candidate_nodes_left, unordered_set<vid_t>& candidate_nodes_right, 
        unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_left, unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_right, TopX& topX, int& bfs_times){
		int bound = compute_lower_bound_right(g, v, current_beta + 1, topX);
		// 排除重复更新
		if (have_update.find({bound + 1, current_beta + 1}) != have_update.end()) return;

		//清空容器
		visited_nodes_left.clear(); visited_nodes_right.clear();
		candidate_nodes_left.clear(); candidate_nodes_right.clear();
		visited_nodes_2_info_left.clear(); visited_nodes_2_info_right.clear();
		// 调用 DFS 计算候选集
		bfs_times++;
        dyn_update_insertion_dfs(g, v, bound + 1, current_beta, false, visited_nodes_left, visited_nodes_right,
            candidate_nodes_left, candidate_nodes_right, visited_nodes_2_info_left, visited_nodes_2_info_right);

		// 更新候选集
		for (auto v : candidate_nodes_right){
			if (g.v_b_Amax[v][current_beta + 1] < bound + 1) g.v_b_Amax[v][current_beta + 1] = bound + 1;
			// dyn_crossUpdate_addition_right(g, bound + 1, current_beta + 1, v);
		}
		for (auto u : candidate_nodes_left){
			dyn_crossUpdate_addition_left(g, current_beta + 1, bound + 1, u);
		}

		have_update.insert({{bound + 1, current_beta + 1}});
		have_update_beta.insert(current_beta + 1);
	}

	void case_beta_increase_left(node_graph& g, vid_t u, vid_t v, int current_alpha, int current_beta, unordered_set<std::pair<int, int>, pair_hash>& have_update, unordered_set<int>& have_update_alpha,
		unordered_set<vid_t>& visited_nodes_left, unordered_set<vid_t>& visited_nodes_right,
        unordered_set<vid_t>& candidate_nodes_left, unordered_set<vid_t>& candidate_nodes_right, 
        unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_left, unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_right, TopX& topX, int& bfs_times){
		int bound = compute_lower_bound_left(g, u, current_alpha, topX);
		// 排除重复更新
		if (have_update.find({current_alpha, bound + 1}) != have_update.end()) return;

		//清空容器
		visited_nodes_left.clear(); visited_nodes_right.clear();
		candidate_nodes_left.clear(); candidate_nodes_right.clear();
		visited_nodes_2_info_left.clear(); visited_nodes_2_info_right.clear();
		// 调用 DFS 计算候选集
		bfs_times++;
        dyn_update_insertion_dfs(g, u, current_alpha, bound, true, visited_nodes_left, visited_nodes_right,
            candidate_nodes_left, candidate_nodes_right, visited_nodes_2_info_left, visited_nodes_2_info_right);
		
		// 更新候选集
		for (auto u : candidate_nodes_left) {
			// if (g.left_index[u][current_alpha] < bound + 1) g.left_index[u][current_alpha] = bound + 1;
			dyn_crossUpdate_addition_left(g, bound + 1, current_alpha, u);
		}
		for (auto v : candidate_nodes_right) {
			dyn_crossUpdate_addition_right(g, current_alpha, bound + 1, v);
		}

		have_update.insert({current_alpha, bound + 1});
		have_update_alpha.insert(current_alpha);
	}

	void case_alpha_increase_right(node_graph& g, vid_t u, vid_t v, int current_alpha, int current_beta, unordered_set<std::pair<int, int>, pair_hash>& have_update, unordered_set<int>& have_update_beta,
		unordered_set<vid_t>& visited_nodes_left, unordered_set<vid_t>& visited_nodes_right,
        unordered_set<vid_t>& candidate_nodes_left, unordered_set<vid_t>& candidate_nodes_right, 
        unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_left, unordered_map<vid_t, traversal_info_insertion>& visited_nodes_2_info_right, TopX& topX, int& bfs_times){
		int bound = compute_lower_bound_right(g, v, current_beta, topX);
		// 排除重复更新
		if (have_update.find({bound + 1, current_beta}) != have_update.end()) return;

		//清空容器
		visited_nodes_left.clear(); visited_nodes_right.clear();
		candidate_nodes_left.clear(); candidate_nodes_right.clear();
		visited_nodes_2_info_left.clear(); visited_nodes_2_info_right.clear();
		// 调用 DFS 计算候选集
		bfs_times++;
        dyn_update_insertion_dfs(g, v, bound + 1, current_beta - 1, false, visited_nodes_left, visited_nodes_right,
            candidate_nodes_left, candidate_nodes_right, visited_nodes_2_info_left, visited_nodes_2_info_right);
		
		// 更新候选集
		for (auto v : candidate_nodes_right){
			// if (g.right_index[v][current_beta] < bound + 1) g.right_index[v][current_beta] = bound + 1;
			dyn_crossUpdate_addition_right(g, bound + 1, current_beta, v);
		}
		for (auto u : candidate_nodes_left){
			dyn_crossUpdate_addition_left(g, current_beta, bound + 1, u);
		}

		have_update.insert({{bound + 1, current_beta}});
		have_update_beta.insert(current_beta);
	}
double update_skyline_index_swap_with_bfs(node_graph& g, vid_t u, vid_t v,unordered_set<vid_t>& update_u_set,unordered_set<vid_t>& update_v_set){
	auto start = chrono::system_clock::now();
	int bfs_times = 0;
	update_u_set.insert(u);
	update_v_set.insert(v);
	
	// 生成接下来分析所需的skyline pair
	vector<int> skyline_u, skyline_v;
	findDominantPairs_u(g.u_a_Bmax[u], skyline_u);
	findDominantPairs_v(g.v_b_Amax[v], skyline_v);

	// 初始化
	vector<vid_t>& nbr_u = g.edge_mp_u[u]; 
	vector<vid_t>& nbr_v = g.edge_mp_v[v];
	int u_deg = g.degree_u[u];
	int v_deg = g.degree_v[v];
	// cout << "deg u: " << u_deg << " " << skyline_u.size() << endl;

	//case, u or v's degree = 1
	if(u_deg == 1){
		g.u_a_Bmax[u].push_back(0);
		if(v_deg == 1)
		g.v_b_Amax[v].push_back(0);
		g.u_a_Bmax[u].push_back(v_deg);
		g.v_b_Amax[v].push_back(u_deg);
		for(auto nbr_node : nbr_v){
			if(g.u_a_Bmax[nbr_node][1] < v_deg) 
				{g.u_a_Bmax[nbr_node][1] = v_deg;
				 update_u_set.insert(nbr_node);
				}
		}
		auto end = chrono::system_clock::now();
		return chrono::duration<double>(end - start).count();
	}

	if(v_deg == 1){
		g.v_b_Amax[v].push_back(0);
		if(u_deg == 1)
		g.u_a_Bmax[u].push_back(0);
		g.v_b_Amax[v].push_back(u_deg);
		g.u_a_Bmax[u].push_back(v_deg);
		for (auto nbr_node : nbr_u){
			if(g.v_b_Amax[nbr_node][1] < u_deg) g.v_b_Amax[nbr_node][1] = u_deg;//update_v_set.insert(nbr_node);
		}
		auto end = chrono::system_clock::now();
		return chrono::duration<double>(end - start).count();
	}

	g.u_a_Bmax[u].push_back(0);
	g.v_b_Amax[v].push_back(0);

	// (0,0)也是一个skyline
	if (skyline_u[0] != 1) skyline_u.insert(skyline_u.begin(), {1, g.u_a_Bmax[u][1]});
	if (skyline_v.empty() || skyline_v.back() != 1) skyline_v.insert(skyline_v.end(), {g.v_b_Amax[v][1], 1});

	unordered_set<std::pair<int, int>, pair_hash> have_update; // 记录防止重复更新
	unordered_set<int> have_update_alpha;
	unordered_set<int> have_update_beta;
	unordered_set<vid_t> visited_nodes_left, visited_nodes_right;
	unordered_set<vid_t> candidate_nodes_left, candidate_nodes_right;
	unordered_map<vid_t, traversal_info_insertion> visited_nodes_2_info_left, visited_nodes_2_info_right;

	// 优先队列，用于找第X大
	TopX topX(1); 

	for(int i_iter = skyline_u.size()-2; i_iter >= 0; i_iter-=2){
		int current_alpha = skyline_u[i_iter];
		int current_beta = skyline_u[i_iter+1];
		// cout << "u: " << u << " " << current_alpha << " " << current_beta << endl;

		// if (current_alpha > current_beta) continue;

		// case: alpha increase
		int bv_alpha_p_one = compute_vbeta_regarding_specific_alpha(g, v, current_alpha + 1);
		int bu_alpha_p_one = g.u_a_Bmax[u][current_alpha + 1];
		// 更新条件：bv_alpha_p_one >= bu_alpha_p_one；current_alpha+1未被计算过
		if (bv_alpha_p_one >= bu_alpha_p_one && have_update_alpha.find(current_alpha + 1) == have_update_alpha.end()) 
		{
			case_alpha_increase_left(g, u, v, current_alpha, current_beta, have_update, have_update_alpha,
				visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right, visited_nodes_2_info_left, visited_nodes_2_info_right, topX, bfs_times);

			update_u_set.insert(candidate_nodes_left.begin(), candidate_nodes_left.end());
			update_v_set.insert(candidate_nodes_right.begin(), candidate_nodes_right.end());
		}	

		if (v_deg < current_beta) break;

		// case: beta increase
		int bv_alpha = compute_vbeta_regarding_specific_alpha(g, v, current_alpha);
		// 更新条件: bv_alpha >= current_beta; current_alpha未被计算过
		if (bv_alpha >= current_beta && have_update_alpha.find(current_alpha) == have_update_alpha.end())
			{
				case_beta_increase_left(g, u, v, current_alpha, current_beta, have_update, have_update_alpha,
				visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right, visited_nodes_2_info_left, visited_nodes_2_info_right, topX, bfs_times);
			    update_u_set.insert(candidate_nodes_left.begin(), candidate_nodes_left.end());
			    update_v_set.insert(candidate_nodes_right.begin(), candidate_nodes_right.end());
			}
	 }

	for (int i_iter = 1; i_iter <= skyline_v.size() - 1; i_iter+=2){
		int current_beta = skyline_v[i_iter];
		int current_alpha = skyline_v[i_iter-1];

		// cout << "v: " << v << " " << current_alpha << " " << current_beta << " " << v_deg << endl;

		// if (current_beta > current_alpha) continue;

		// case: beta increase
		int au_beta_p_one = compute_ualpha_regarding_specific_beta(g, u, current_beta + 1);
		int av_beta_p_one = g.v_b_Amax[v][current_beta + 1];
		// 更新条件: au_beta_p_one >= av_beta_p_one; current_beta+1未被计算过
		if (au_beta_p_one >= av_beta_p_one && have_update_beta.find(current_beta + 1) == have_update_beta.end()){
			case_beta_increase_right(g, u, v, current_alpha, current_beta, have_update, have_update_beta,
				visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right, visited_nodes_2_info_left, visited_nodes_2_info_right, topX, bfs_times);
		     update_u_set.insert(candidate_nodes_left.begin(), candidate_nodes_left.end());
			 update_v_set.insert(candidate_nodes_right.begin(), candidate_nodes_right.end());	
				}
		
		if (u_deg < current_alpha) break;

		// case: alpha increase
		int au_alpha = compute_ualpha_regarding_specific_beta(g, u, current_beta);
		// 更新条件: au_alpha >= current_alpha; current_beta未被计算过
		if (au_alpha >= current_alpha && have_update_beta.find(current_beta) == have_update_beta.end())
			{case_alpha_increase_right(g, u, v, current_alpha, current_beta, have_update, have_update_beta, 
				visited_nodes_left, visited_nodes_right, candidate_nodes_left, candidate_nodes_right, visited_nodes_2_info_left, visited_nodes_2_info_right, topX, bfs_times);
			    update_u_set.insert(candidate_nodes_left.begin(), candidate_nodes_left.end());
			    update_v_set.insert(candidate_nodes_right.begin(), candidate_nodes_right.end());
			}
	}
	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	return elapsed_seconds.count();
}

struct skyline_block {
	std::map<int,int> mapset;
	//bicore_index_block* next = NULL;
};

skyline_block findDominantPairs_u_map(const vector<int>& nums, skyline_block&result) {
	int maxY = INT_MIN;  // 当前的最大y值

	// 从右向左遍历vector
	for (int i = nums.size() - 1; i >= 0; --i) {
		// 如果当前元素比maxY大，说明该元素不被支配
		if (nums[i] > maxY) {
			// 将该元素的值和下标加入结果
			result.mapset[i]=nums[i];
			maxY = nums[i];             // 更新最大y值
		}
	}
	
	return result;
}

skyline_block findDominantPairs_v_map(const vector<int>& nums, skyline_block& result) {
	int maxY = INT_MIN;

	for (int i = nums.size() - 1; i >= 0; --i) {
		if (nums[i] > maxY) {
			result.mapset[nums[i]]=i;
			maxY = nums[i]; 
		}
	}

	return result;
}
int find_kth(vector<int> B, int n, int k){
	if (k < 0||k > n) return 0;
	sort(B.begin(), B.end());
	return B[n - k];
}
double update_skyline_index_swap_with_bfs_delete(node_graph& g,  vid_t u, vid_t v,unordered_set<int> &update_u_set_end, unordered_set<int> &update_v_set_end){
		auto start = chrono::system_clock::now();
		for (int i = 0; i < g.degree_u[u]; ++i)
		{
			int vv = g.edge_mp_u[u][i];
			if (vv == v)
			{
				swap(g.edge_mp_u[u][i], g.edge_mp_u[u][g.degree_u[u] - 1]);
				--g.degree_u[u];
				g.edge_mp_u[u].pop_back();
				//num_edges--;//only once!!!
				break;
			}
		}
		for (int i = 0; i < g.degree_v[v]; ++i)
		{
			int uu = g.edge_mp_v[v][i];
			if (uu == u)
			{
				swap(g.edge_mp_v[v][i], g.edge_mp_v[v][g.degree_v[v] - 1]);
				--g.degree_v[v];
				g.edge_mp_v[v].pop_back();
				break;
			}
		}
	cout<<"delete"<<"u:"<<u<<"v"<<v<<endl;

	    unordered_set<int> updateu;
		unordered_set<int> updatev;
		unordered_set<int> update_u_set;
		unordered_set<int> update_v_set;
		map<int,int> pair_to_be_visit;
		map<int,int> pair_to_be_visit_v;
		skyline_block skyline_index_u;
		skyline_block skyline_index_v;
		skyline_block skyline_index_v_reverse;
		skyline_block skyline_index_u_reverse;
		skyline_index_u = findDominantPairs_u_map(g.u_a_Bmax[u], skyline_index_u);
		skyline_index_v = findDominantPairs_v_map(g.v_b_Amax[v], skyline_index_v);
		for(auto it : skyline_index_v.mapset){
			skyline_index_v_reverse.mapset[it.second] =it.first;
		}
		for(auto it : skyline_index_u.mapset){
			int alpha = it.first; int beta = it.second;
			//if(find_skyline(skyline_index_v, v, alpha, g.num_v2) >= beta){
			if(compute_vbeta_regarding_specific_alpha(g, v, alpha) >= beta){
				pair_to_be_visit[alpha] = beta;
			}
		}
		for(auto it : skyline_index_v.mapset){
			int alpha = it.first; int beta = it.second;
			//if(find_skyline(skyline_index_u, u, alpha,g.num_v1) >= beta){
						if(g.u_a_Bmax[u].size()>alpha&&g.u_a_Bmax[u][alpha] >= beta){
			//if(g.left_index[u][alpha] >= beta){
				//if(pair_to_be_visit.count(alpha) != 0){
				//    if (pair_to_be_visit[alpha] == beta) continue;
				//}
				pair_to_be_visit_v[alpha] = beta;
			}
		}

		if(pair_to_be_visit.size() == 0){
			pair_to_be_visit[g.degree_u[u] + 1] = skyline_index_u.mapset[g.degree_u[u] + 1];
		}
		else{
			if(pair_to_be_visit.count(g.degree_u[u] + 1) == 0){
				//pair_to_be_visit[g.degree_v1[u] + 1] = find_skyline(skyline_index_u, u, g.degree_v1[u] + 1,g.num_v1);
				pair_to_be_visit[g.degree_u[u] + 1] = g.u_a_Bmax[u][g.degree_u[u] + 1];
			}
		}
		if(pair_to_be_visit_v.size() == 0){
			pair_to_be_visit_v[skyline_index_v_reverse.mapset[g.degree_v[v] + 1]] = g.degree_v[v] + 1;
		}
		else{
			if(pair_to_be_visit_v.count(g.degree_v[v] + 1) == 0){
				//pair_to_be_visit_v[find_skyline(skyline_index_v_reverse, v, g.degree_v2[v] + 1,g.num_v2)] = g.degree_v2[v] + 1;
				pair_to_be_visit_v[g.v_b_Amax[v][g.degree_v[v] + 1]] = g.degree_v[v] + 1;
			}
		}
		cout<<"pair_to_be_visit_v"<<endl;
		unordered_set<vid_t> visited_nodes_left; unordered_set<vid_t> visited_nodes_right;
		unordered_set<vid_t> candidate_nodes_left; unordered_set<vid_t> candidate_nodes_right;
		unordered_map<vid_t, traversal_info_removal> visited_nodes_2_info_left; unordered_map<vid_t, traversal_info_removal> visited_nodes_2_info_right;
		
		vector<int> vector_A;
		vector<vid_t>& neigh_1 = g.edge_mp_u[u];
		vector<vid_t>& neigh_2 = g.edge_mp_v[v];
        for (auto iter = pair_to_be_visit.rbegin(); iter != pair_to_be_visit.rend(); iter++){
            visited_nodes_left.clear(); visited_nodes_right.clear();
            candidate_nodes_left.clear(); candidate_nodes_right.clear();
            visited_nodes_2_info_left.clear(); visited_nodes_2_info_right.clear();
            int current_alpha = iter->first;
            int current_beta = iter->second;
			//<<"current_alpha"<<current_alpha<<"current_beta"<<current_beta<<endl;
			if (current_alpha == 0 || current_beta == 0) continue;
			dyn_update_removal_dfs(g, u, current_alpha, current_beta, true, visited_nodes_left, visited_nodes_right,
                                                                candidate_nodes_left, candidate_nodes_right, visited_nodes_2_info_left, visited_nodes_2_info_right);
			if(candidate_nodes_left.size() == 0 ) {
                pair_to_be_visit_v[current_alpha] = current_beta;
                continue;
            }
			if(pair_to_be_visit_v.count(current_alpha) != 0){
				if(candidate_nodes_right.find(v) == candidate_nodes_right.end()){
					dyn_update_removal_dfs(g, v, current_alpha, current_beta, false, visited_nodes_left, visited_nodes_right,
                                                                candidate_nodes_left, candidate_nodes_right, visited_nodes_2_info_left, visited_nodes_2_info_right);
				}
				pair_to_be_visit_v[current_alpha] = 0;
			}
            int beta_ = 0;
			int beta_2 = 0;
            vector<int> vector_B;
			vector<int> vector_C;
            if(current_alpha - 1 != 0){
                for (vid_t nbr_node:neigh_1){
                    //vector_B.push_back(find_skyline(skyline_index_v, nbr_node, current_alpha - 1,g.num_v2));
					vector_B.push_back(compute_vbeta_regarding_specific_alpha(g, nbr_node, current_alpha - 1));
					vector_C.push_back(compute_vbeta_regarding_specific_alpha(g, nbr_node, current_alpha));
                }
                beta_ = max(find_kth(vector_B,vector_B.size(),current_alpha - 1),current_beta);
                vector_B.clear();
				beta_2 = find_kth(vector_C,vector_C.size(),current_alpha);
            	vector_C.clear();
            }
			else{
				for (vid_t nbr_node:neigh_1){
					//vector_B.push_back(find_skyline(skyline_index_v, nbr_node, current_alpha,g.num_v2));
					vector_B.push_back(compute_vbeta_regarding_specific_alpha(g, nbr_node, current_alpha));
				}
				beta_2 = find_kth(vector_B,vector_B.size(),current_alpha);
				vector_B.clear();
			}

			for (auto w : candidate_nodes_left){
				if(g.left_index_delete[w].size()>current_alpha)
				  g.left_index_delete[w][current_alpha] = 0,updateu.insert(w);
				if (w == u){
					int i = current_alpha;
					while (i < g.u_a_Bmax[w].size() && g.left_index_change[w][i] < beta_2){
						if( beta_2 < g.u_a_Bmax[w][i]) {
							g.left_index_change[w][i] = beta_2;
							updateu.insert(w);
							if(g.left_index_delete[w][i] == 0) g.left_index_old[w][i] = beta_2,update_u_set.insert(w);
						}
						i++;
					}
					if(g.left_index_delete[w].size()<i&&g.left_index_delete[w][i] == 0 && g.left_index_old[w][i] == g.u_a_Bmax[w][i]) g.left_index_old[w][i] = g.left_index_change[w][i],update_u_set.insert(w);
				}
				else{
					int i = current_alpha;
					while (i < g.u_a_Bmax[w].size() && g.left_index_change[w][i] < current_beta - 1){
						if(current_beta - 1 < g.u_a_Bmax[w][i]) {
							g.left_index_change[w][i] = current_beta - 1;
							updateu.insert(w);
							if(g.left_index_delete[w][i] == 0) g.left_index_old[w][i] = current_beta - 1,update_u_set.insert(w);
						}
						i++;
					}
					if(g.left_index_delete[w].size()<i&&g.left_index_delete[w][i] == 0 && g.left_index_old[w][i] == g.u_a_Bmax[w][i]) g.left_index_old[w][i] = g.left_index_change[w][i],update_u_set.insert(w);
				}
				if ((g.left_index_change[w].size()<current_alpha - 1)&&g.left_index_change[w][current_alpha - 1] < beta_){
					if(beta_ < g.u_a_Bmax[w][current_alpha - 1]) {
						g.left_index_change[w][current_alpha - 1] = beta_;
						updateu.insert(w);
						if(g.left_index_delete[w][current_alpha - 1] == 0) g.left_index_old[w][current_alpha - 1] = beta_,update_u_set.insert(w);
					}
				}
				else{
					if((g.left_index_delete[w].size()<current_alpha - 1)&& g.left_index_delete[w][current_alpha - 1] == 0 && g.left_index_old[w][current_alpha - 1] == g.u_a_Bmax[w][current_alpha - 1])
					g.left_index_old[w][current_alpha - 1] = g.left_index_change[w][current_alpha - 1],update_u_set.insert(w);
				} 
			}
			for (auto w : candidate_nodes_right) {
				if(g.right_index_delete[w].size()>current_beta)
				 g.right_index_delete[w][current_beta] = 0,updatev.insert(w);
				if ((g.right_index_change[w].size()<current_beta - 1)&& g.right_index_change[w][current_beta - 1] < current_alpha){
					if(current_alpha < g.v_b_Amax[w][current_beta - 1]) {
						g.right_index_change[w][current_beta - 1] = current_alpha;
						updatev.insert(w);
						if(g.right_index_delete[w][current_beta - 1] == 0) g.right_index_old[w][current_beta - 1] = current_alpha,update_v_set.insert(w);
					}
				}
				else{
					if((g.right_index_delete[w].size()<current_beta - 1)&&g.right_index_delete[w][current_beta - 1] == 0 && g.right_index_old[w][current_beta - 1] == g.v_b_Amax[w][current_beta - 1])
					g.right_index_old[w][current_beta - 1] = g.right_index_change[w][current_beta - 1],update_v_set.insert(w);
				}
				if (w == v){
					int alpha_2 = 0;
					for (vid_t nbr_node:neigh_2){
						//vector_B.push_back(comput_u_alpha(skyline_index_u, skyline_index_u_reverse, nbr_node, current_beta, g.num_v1));
						vector_B.push_back(compute_ualpha_regarding_specific_beta(g, nbr_node, current_beta));
					}
					alpha_2 = min(find_kth(vector_B,vector_B.size(),current_beta), current_alpha - 1);
					
					vector_B.clear();
					int i = current_beta;
					while (i < g.v_b_Amax[w].size() &&g.right_index_change[w][i] < alpha_2 ){
						if(alpha_2 < g.v_b_Amax[w][i]) {
							g.right_index_change[w][i] = alpha_2;
							updatev.insert(w);
							if(g.right_index_delete[w][i] == 0) g.right_index_old[w][i] = alpha_2,update_v_set.insert(w);
						}
						i++;
					}
					if(g.right_index_delete[w].size()<i&&g.right_index_delete[w][i] == 0 && g.right_index_old[w][i] == g.v_b_Amax[w][i])
					g.right_index_old[w][i] = g.right_index_change[w][i],update_v_set.insert(w);
				}
				else{
					for (int i = current_beta; i <= beta_; i++){ 
						if ((g.right_index_change[w].size()<i)&&g.right_index_change[w][i] < current_alpha - 1 && current_alpha - 1 < g.v_b_Amax[w][i]) {
							g.right_index_change[w][i] = current_alpha - 1;
							updatev.insert(w);
							if(g.right_index_delete[w][i] == 0) g.right_index_old[w][i] = current_alpha - 1,update_v_set.insert(w); 
						}
						else{
							if((g.right_index_delete[w].size()<i)&&g.right_index_delete[w][i] == 0 && g.right_index_old[w][i] == g.v_b_Amax[w][i])
							g.right_index_old[w][i] = g.right_index_change[w][i],update_v_set.insert(w);
						}
					}
					//dyn_crossUpdate_deletion_v(g, alpha, beta, v);
				}

			}
        }
		cout<<"finish1"<<endl;
        vector_A.clear();
        for (auto iter = pair_to_be_visit_v.begin(); iter != pair_to_be_visit_v.end(); iter++){
            int current_alpha = iter->first;
            int current_beta = iter->second;
			//cout<<"current_alpha:"<<current_alpha<<" current_beta:"<<current_beta<<endl;
			if (current_alpha == 0 || current_beta == 0) continue;
            //if(already_u.count(current_alpha) != 0) continue;
            visited_nodes_left.clear(); visited_nodes_right.clear();
            candidate_nodes_left.clear(); candidate_nodes_right.clear();
            visited_nodes_2_info_left.clear(); visited_nodes_2_info_right.clear();
            dyn_update_removal_dfs(g, v, current_alpha, current_beta, false, visited_nodes_left, visited_nodes_right,
                                                                candidate_nodes_left, candidate_nodes_right, visited_nodes_2_info_left, visited_nodes_2_info_right);
			if(candidate_nodes_right.size() == 0 ) continue;
            int alpha_ = 0;
			int alpha_2 = 0;
			vector<int> vector_B;
			vector<int> vector_C;
            if(current_beta - 1 != 0){
                for (vid_t nbr_node:neigh_2){
                    //vector_B.push_back(comput_u_alpha(skyline_index_u, skyline_index_u_reverse, nbr_node, current_beta - 1, g.num_v1));
					vector_B.push_back(compute_ualpha_regarding_specific_beta(g, nbr_node, current_beta - 1));
					vector_C.push_back(compute_ualpha_regarding_specific_beta(g, nbr_node, current_beta));
                }
                alpha_ = max(find_kth(vector_B,vector_B.size(),current_beta - 1),current_alpha);
                vector_B.clear();
				alpha_2 = find_kth(vector_C,vector_C.size(),current_beta);
            	vector_C.clear();
            }
			else{
				for (vid_t nbr_node:neigh_2){
					//vector_B.push_back(comput_u_alpha(skyline_index_u, skyline_index_u_reverse, nbr_node, current_beta, g.num_v1));
					vector_B.push_back(compute_ualpha_regarding_specific_beta(g, nbr_node, current_beta));
				}
				alpha_2 = find_kth(vector_B,vector_B.size(),current_beta);
				vector_B.clear();
			}

			 
			for (auto w : candidate_nodes_right){
				if(g.right_index_delete[w].size()>current_beta)
				g.right_index_delete[w][current_beta] = 0,updatev.insert(w);
				if (w == v){
					int i = current_beta;
					while (i < g.v_b_Amax[w].size() && g.right_index_change[w][i] < alpha_2){
						if(alpha_2 < g.v_b_Amax[w][i]) {
							g.right_index_change[w][i] = alpha_2;
							updatev.insert(w);
							if(g.right_index_delete[w][i] == 0) g.right_index_old[w][i] = alpha_2,update_v_set.insert(w);
						}
						i++;
					}
					if (g.right_index_delete[w].size()<i&& g.right_index_delete[w][i] == 0 && g.right_index_old[w][i] == g.v_b_Amax[w][i]) g.right_index_old[w][i] = g.right_index_change[w][i],update_v_set.insert(w);
				}
				else{
					int i = current_beta;
					while (i < g.v_b_Amax[w].size()&&g.right_index_change[w][i] < current_alpha - 1  ){
						if(current_alpha - 1 < g.v_b_Amax[w][i]) {
							g.right_index_change[w][i] = current_alpha - 1;
							updatev.insert(w);
							if(g.right_index_delete[w][i] == 0) g.right_index_old[w][i] = current_alpha - 1,update_v_set.insert(w);
						}
						i++;
					}
					if (g.right_index_delete[w].size()<i&&g.right_index_delete[w][i] == 0 && g.right_index_old[w][i] == g.v_b_Amax[w][i]) g.right_index_old[w][i] = g.right_index_change[w][i],update_v_set.insert(w);
				}
				if ((g.right_index_change[w].size()<current_beta - 1)&&g.right_index_change[w][current_beta - 1] < alpha_){
					if(alpha_ < g.v_b_Amax[w][current_beta - 1]){
						g.right_index_change[w][current_beta - 1] = alpha_;
						updatev.insert(w);
						if(g.right_index_delete[w][current_beta - 1] == 0) g.right_index_old[w][current_beta - 1] = alpha_,update_v_set.insert(w);
					}
				}
				else{
					if((g.right_index_delete[w].size()<current_beta - 1)&&g.right_index_delete[w][current_beta - 1] == 0 && g.right_index_old[w][current_beta - 1] == g.v_b_Amax[w][current_beta - 1])
					g.right_index_old[w][current_beta - 1] = g.right_index_change[w][current_beta - 1],update_v_set.insert(w);
				}
			}
			for (auto w : candidate_nodes_left) {
				if(g.left_index_delete[w].size()>current_alpha)
				g.left_index_delete[w][current_alpha] = 0,updateu.insert(w);
				if ( (g.left_index_change[w].size()<current_alpha-1)&&g.left_index_change[w][current_alpha - 1] < current_beta){
					if(current_beta < g.u_a_Bmax[w][current_alpha - 1]) {
						g.left_index_change[w][current_alpha - 1] = current_beta;
						updateu.insert(w);
						if(g.left_index_delete[w][current_alpha - 1] == 0) g.left_index_old[w][current_alpha - 1] = current_beta,update_u_set.insert(w);
					}
				}
				else{
					if((g.left_index_delete[w].size()<current_alpha-1)&&g.left_index_delete[w][current_alpha - 1] == 0 && g.left_index_old[w][current_alpha - 1] == g.u_a_Bmax[w][current_alpha - 1])
					g.left_index_old[w][current_alpha - 1] = g.left_index_change[w][current_alpha - 1],update_u_set.insert(w);
				}
				for (int i = current_alpha; i <= alpha_; i++){ 
					if ((g.left_index_change[w].size()<i)&&g.left_index_change[w][i] < current_beta - 1 && current_beta - 1 < g.u_a_Bmax[w][i]) {
						g.left_index_change[w][i] = current_beta - 1; 
						updateu.insert(w);
						if(g.left_index_delete[w][i] == 0) g.left_index_old[w][i] = current_beta - 1,update_u_set.insert(w); 
					}
					else{
						if((g.left_index_delete[w].size()<i)&&g.left_index_delete[w][i] == 0 && g.left_index_old[w][i] == g.u_a_Bmax[w][i])
						g.left_index_old[w][i] = g.left_index_change[w][i],update_u_set.insert(w);
					}
				}
			}
        }
		//cout<<"finish2"<<endl;
		// for(auto it: g.left_index_old){
		// 	cout<<"u:"<<it.first<<endl;
		// 	for(int a=1;a<it.
				
		// 		second.size();a++){
        //         cout<<"alpha:"<<a<<"beta:"<<it.second[a]<<endl;
		// 	}
		// }
		// cout<<"old"<<endl;

		// for(auto it: g.u_a_Bmax){
		// 	cout<<"u:"<<it.first<<endl;
		// 	for(int a=1;a<it.second.size();a++){
        //         cout<<"alpha:"<<a<<"beta:"<<it.second[a]<<endl;
		// 	}
		// }
		// swap(g.left_index_old, g.u_a_Bmax);
		// swap(g.right_index_old, g.v_b_Amax);
		cout<<"finis"<<endl;
		update_u_set.insert(u);
		update_v_set.insert(v);
		updateu.insert(update_u_set.begin(),update_u_set.end());
		updatev.insert(update_v_set.begin(),update_v_set.end());
		update_u_set_end.insert(updateu.begin(),updateu.end());
		update_v_set_end.insert(updatev.begin(),updatev.end());
		for(auto it:update_u_set){
			g.u_a_Bmax[it]=g.left_index_old[it];

		}
		for(auto it:update_v_set){
			g.v_b_Amax[it]=g.right_index_old[it];

		}
		g.u_a_Bmax[u].pop_back();
		g.v_b_Amax[v].pop_back();
		g.left_index_old[u].pop_back();
		g.right_index_old[v].pop_back();
		for(auto it:updateu){
			g.left_index_change[it]=g.left_index_old[it];
			fill(g.left_index_delete[it].begin(),g.left_index_delete[it].end(),0);
			g.left_index_delete[it]=g.left_index_old[it];
		}
		for(auto it:updatev){
			g.right_index_change[it]=g.right_index_old[it];
			fill(g.right_index_delete[it].begin(),g.right_index_delete[it].end(),0);
			g.right_index_delete[it]=g.right_index_old[it];
		}


		cout<<"finish3"<<endl;
        auto end = chrono::system_clock::now();
		//rebuild_skyline(g, skyline_index_u, skyline_index_u_reverse, skyline_index_v, skyline_index_v_reverse);
        chrono::duration<double> elapsed_seconds = end - start;
		// for (auto it:g.u_a_Bmax) {
        //     int u=it.first;
        //     int size=it.second.size();
		// 	if(size==0){
		// 		g.left_index_change[u]=vector<int>();
		// 		continue;
		// 	}
        //     g.left_index_change[u]=vector<int>(size,0);
        // }
        // for(auto it: g.v_b_Amax){
        //    int v=it.first;
        //    int size=it.second.size();
		//    if(size==0){
		// 	   g.right_index_change[v]=vector<int>();
		// 	   continue;
		//    }
        //    g.right_index_change[v]=vector<int>(size,0);
        // }
		// 直接复制 u_a_Bmax 和 v_b_Amax 到 left_index_change 和 right_index_change
		// g.left_index_change = g.u_a_Bmax;
		// g.right_index_change = g.v_b_Amax;

		// // 仅需遍历左侧索引
		// for (auto& it : g.left_index_change) {
		// 	int u = it.first;
		// 	// 直接初始化左侧旧索引为0
		// 	std::fill(g.left_index_change[u].begin(), g.left_index_change[u].end(), 0);
		// }

		// // 仅需遍历右侧索引
		// for (auto& it : g.right_index_change) {
		// 	int v = it.first;
		// 	// 直接初始化右侧旧索引为0
		// 	std::fill(g.right_index_change[v].begin(), g.right_index_change[v].end(), 0);
		// }

        // cout<<"finish4"<<endl;
        // g.left_index_old = g.u_a_Bmax;
        // g.right_index_old = g.v_b_Amax;
     
		// g.left_index_delete = g.u_a_Bmax;
		// g.right_index_delete = g.v_b_Amax;
		// cout<<"finish5"<<endl;
		// g.left_index_change.clear();
		// g.right_index_change.clear();
		// g.left_index_old.clear();
		// g.right_index_old.clear();
		// g.left_index_delete.clear();
		// g.right_index_delete.clear();
        
		// for (auto& [u, vec] : g.u_a_Bmax) {
		// 	if(vec.size()==0){
		// 		continue;
		// 	}
		// 	g.left_index_change[u] = std::vector<int>(vec.size(), 0);
		// }

		// for (auto& [v, vec] : g.v_b_Amax) {
		// 	if(vec.size()==0){
		// 		continue;
		// 	}
		// 	g.right_index_change[v] = std::vector<int>(vec.size(), 0);
		
		// }
		// cout<<"finish4"<<endl;
		// g.left_index_old=g.u_a_Bmax;
		// g.right_index_old=g.v_b_Amax;
		// g.left_index_delete=g.u_a_Bmax;
		// g.right_index_delete=g.v_b_Amax;

        // cout<<"finish6"<<endl;

        return elapsed_seconds.count();
    }

#endif // !DYN_REBUILD_H
