#include <iostream>
#include "include/Prottype.hpp"

void EchoChamber_Dynamics(int time, SNS &sns, UserAgent &users, MediaAgent &media){
    int user = random_int(0, N_user);
    int medium = random_int(0, N_media);
    vector<Message> similar_post;
    vector<Message> unsimilar_post;

    tie(similar_post, unsimilar_post) = users[user].divide_post(SNS sns);
    users[user].influence(similar_post);
    users[user].post(time, sns, similar_post);
    users[user].refollow();

    media[medium].post();

}

int main(void) {
    random_seed(seed);
    SNS sns(N, E);
    UserAgent users[N_user];
    MediaAgent media[N_media];


    return 0;
}
