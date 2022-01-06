#include "include/Prottype.hpp"

void UserAgent::post(int time, SNS &sns){
    Message msg = {"post", time, myid, myid, o};
    sns.push(msg);
}

void UserAgent::influence(){

}

void UserAgent::refollow(){
    
}

void MediaAgent::initialize(int index)
{
    this->myid = index + N_user;
    this->opinion_range[0] = opinion_ranges[index][0];
    this->opinion_range[1] = opinion_ranges[index][1];
    this->o = random_uniform(opinion_range[0], opinion_range[1]);
}