#ifndef CONST_HPP
#define CONST_HPP
#include <string>
using namespace std;
const float confidence_level = 0.0;
const int max_n = 10;
const int N_user = 100;
const int E = 400;
const int seeds[max_n] = {987, 654, 321, 123, 456, 789, 963, 852, 741, 1234567890};
const int N_media = 6;
const int N = N_user + N_media;
const float p = 0.5;
const float q = 0.5;
const float p_media = p;
const int l = 10;
const int T = 1000;
const float EP = 0.4;
const float M = 0.5;
const string follow_method = "random";
const bool opinion_change = true;
const int MF = 15;
const float opinion_ranges[N_media][2] = {{-1.0, -0.8},{-1.0, -0.8},{-1.0, -0.8}, {0.8, 1.0}, {0.8, 1.0} ,{0.8, 1.0}};
#endif