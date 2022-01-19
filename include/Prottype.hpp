#ifndef PROTTYPE_HPP
#define PROTTYPE_HPP
#include "Class.hpp"
using namespace std;
#include <tuple>
#include <array>
#include <algorithm>
#include <queue>
#include <numeric>
//#include <direct.h>
#include <filesystem>
#include <iomanip> //時間を取得するため
#include <sstream> //値を簡単に文字列にするため
#include <fstream>
#include <iostream>
#include <set>


void er_directed_network_generator(vector<node> &network, int vertex, int edge);
string get_date(void);
//void EchoChamber_Dynamics(int time, SNS &sns, array<UserAgent, l> &users, array<MediaAgent, l> &media);
void export_graph(int n, int time, SNS &sns, array<UserAgent, N_user> &users, array<MediaAgent, N_media> &media, string ResultPath);
void export_2d_data(int n, vector<vector<float>> &data, string ResultPath, string FileName);
void export_parameter(string ResultPath, string abstract);
float shannon_entropy(vector<float> &data, int bins);
float calc_diversity(vector<Message> &screen);
#endif