#ifndef CONST_HPP
#define CONST_HPP
#include <string>
using namespace std;
const int max_n = 10;
const int N_user = 100;
const int E = 400;
//const int origin_seed = 4155;
const int N_media = 6;
const int N = N_user + N_media;
const float p = 0.5;
const float q = 0.5;
const float p_media = p;
const int l = 10;
const int T = 10000;
const float EP = 0.4;
const float M = 0.5;
const string follow_method = "random";


//const float opinion_ranges[N_media][2] = {{-1.0, 0.1},{-1.0, 0.1},{-1.0, 0.1}, {-0.1, 1.0}, {-0.1, 1.0} ,{-0.1, 1.0}};
#endif