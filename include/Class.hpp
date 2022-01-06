#ifndef CLASS_HPP
#define CLASS_HPP
#include "Random.hpp"
#include "Struct.hpp"
#include "Const.hpp"
using namespace std;

//Class as SNS
class SNS{
    public:
        vector<Message> msgdb;
        vector<struct_node> graph;
        int N;
        int E;
        SNS(int N, int E);
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
        void influence();
        void post(int time, SNS &sns);
        //void refollow();
        float diversity = 0;
    private:
        int myid;
        float o = random_uniform(-1.0, 1.0);
        bool confidence;
};

//Media Agent classs
class MediaAgent
{
    public:
        //UserAgent(int myid);
        void initialize(int myid);
        
        //void post();

    private:
        int myid;
        float o;
        float opinion_range[2];
};


#endif