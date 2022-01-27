#ifndef CLASS_HPP
#define CLASS_HPP
#include "Random.hpp"
#include "Struct.hpp"
#include "Const.hpp"
using namespace std;
#include <tuple>
#include <cmath>

//Class as SNS
class SNS{
    public:
        vector<Message> msgdb;
        vector<node> network;
        int N;
        int E;
        SNS(int N, int E, int MF);
        vector<int> get_follow(int user_id);
        void push(Message msg);
        void remove_edge(int start, int end);
        void add_edge(int start, int end);
};

//Ordinary agent class
class UserAgent
{
    public:
        //UserAgent(int myid);
        void initialize(int myid, float confidence_level);
        void divide_post(SNS &sns, vector<Message> &similar_post, vector<Message> &unsimilar_post); 
        void influence(vector<Message> &similar_post);
        void post(int time, SNS &sns, vector<Message> &similar_post);
        void refollow(SNS &sns, vector<Message> &unsimilar_post);
        void renew_screen(SNS &sns);
        float o = random_uniform(-1.0, 1.0);
        vector<Message> screen;
        float diversity = 0;
        bool confidence;
    private:
        int myid;
};

//Media Agent classs
class MediaAgent
{
    public:
        //MediaAgent(int myid);
        void add_id(int myid);
        void post(int time, SNS &sns);
        void set_opinion_range(float range0, float range1);
        float o;
        float opinion_range[2];
    private:
        int myid;
};


#endif