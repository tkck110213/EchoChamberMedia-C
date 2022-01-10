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
        SNS(int N, int E);
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
        void initialize(int myid);
        tuple<vector<Message>, vector<Message>> divide_post(SNS &sns); 
        void influence(vector<Message> &similar_post);
        void post(int time, SNS &sns, vector<Message> &similar_post);
        void refollow(SNS &sns, vector<Message> &unsimilar_post);
        
    private:
        int myid;
        vector<Message> screen;
        float diversity = 0;
        float o = random_uniform(-1.0, 1.0);
        bool confidence;

};

//Media Agent classs
class MediaAgent
{
    public:
        //MediaAgent(int myid);
        void initialize(int myid);
        void post(int time, SNS &sns);

    private:
        int myid;
        float o;
        float opinion_range[2];
};


#endif