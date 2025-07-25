#ifndef CASESTUDY_H
#define CASESTUDY_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <cassert>
#include"Graph.h"
#include"./Utils/graph_utils.h"
#include"./Index/VC_index.h"
#include"./Index/MAC_index_build_maintenance.h"


using namespace std;

// 用于将 unordered_set<string> 转成唯一 key，方便去重
string serialize_set(const unordered_set<string>& s) ;
int count_ones(uint32_t x) ;
bool cmp( pair<uint32_t, uint32_t> a,  pair<uint32_t, uint32_t> b) ;
std::vector<uint32_t> getNonEmptySubmasks(uint32_t mask);
double inc(int q,int a,int b,Graph&G,MAC_index & mac);
double inc_opt(int q,int a,int b,Graph&G,MAC_index& mac);
double Dec(int q,int a,int b,Graph&G,MAC_index& mac);
double Dec_opt(int q,int a,int b,Graph&G,MAC_index& mac);



#endif