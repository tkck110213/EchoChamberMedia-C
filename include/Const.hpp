#ifndef CONST_HPP
#define CONST_HPP
#include <string>
using namespace std;
const float confidence_level = 0.5;
const int N_user = 100;
const int E = 400;
const int seed = 125456;
const int media_per_opinion_range = 2;//opinionrange ごとのメディア数
const int amount_type_opinion_range = 3;//opinion rangeの種類数
const float opinion_ranges[amount_type_opinion_range][2] = {{-1.0, -0.8},{-0.1, 0.1},{0.8, 1.0}};
const int N_media = amount_type_opinion_range * media_per_opinion_range;
const int N = N_user + N_media;
const float p = 0.5;
const float q = 0.5;
const float p_media = 0.5;
const int l = 10;
const int T = 10000;
const float EP = 0.4;
const float M = 0.5;
const int max_n = 10;
const string follow_method = "random";
const int media_follower = 15;
#endif