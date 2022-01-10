#include <iostream>
#include "include/Prottype.hpp"

void EchoChamber_Dynamics(int time, SNS &sns, array<UserAgent, l> &users, array<MediaAgent, l> &media){
    int user = random_int(0, N_user);
    int medium = random_int(0, N_media);
    vector<Message> similar_post;
    vector<Message> unsimilar_post;

    tie(similar_post, unsimilar_post) = users[user].divide_post(sns);
    users[user].influence(similar_post);
    users[user].post(time, sns, similar_post);
    users[user].refollow(sns, unsimilar_post);

    media[medium].post(time, sns);

}

int main(void) {
    random_seed(seed);
    SNS sns(N, E);
    array<UserAgent, N_user> users;
    array<MediaAgent, N_media> media; 
    
    //人数分のユーザエージェントを作成
    for(int i = 0; i < N_user; ++i){
        users[i].initialize(i);
    }
    //人数分のメディアエージェントを作成
    for(int i = N_user; i < N; ++i){
        media[i].initialize(i);
    }

    for(int time = 0; time < T; ++time){
        int user = random_int(0, N_user - 1);
        int medium = random_int(N_user, N - 1);
        vector<Message> similar_post;
        vector<Message> unsimilar_post;

        tie(similar_post, unsimilar_post) = users[user].divide_post(sns);
        users[user].influence(similar_post);
        users[user].post(time, sns, similar_post);
        users[user].refollow(sns, unsimilar_post);

        media[medium].post(time, sns);
    }

    return 0;
}
