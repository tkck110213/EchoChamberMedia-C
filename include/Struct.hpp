#ifndef STRUCT_HPP
#define STRUCT_HPP

#include <vector>
#include <string>
using namespace std;

//Agent node on sns network
struct node
{
  int node_id;
  vector<int> follower;//follower
  vector<int> follow;//follow
  float color;
  string label;
};

//Struct data as Masseage data
struct Message{
    string post_type;
    int post_time;
    int post_user;
    int original_user;
    float opinion;
};


#endif