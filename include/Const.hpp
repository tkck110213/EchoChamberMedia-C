#ifndef CONST_HPP
#define CONST_HPP

const float confidence_level = 0.6;
const int N_user = 100;
const int E = 400;
const int seed = 125456;
const int N_media = 6;
const int N = N_user + N_media;
const float p = 0.5;
const int l = 10;
const float EP = 0.4;
const float M = 0.5;
const float opinion_ranges[N_media][2] = {{-1.0, -8.0},{-0.1, 0.1},{0.8, 1.0},{-1.0, -8.0},{-0.1, 0.1},{0.8, 1.0}};
#endif