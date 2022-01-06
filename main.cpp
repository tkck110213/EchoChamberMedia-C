#include <iostream>
#include "include/Prottype.hpp"

void EchoChamber_Dynamics(SNS &sns, UserAgent &users, MediaAgent &media){
    int user = random_int(0, N_user);
    int medium = random_int(0, N_media);

    users[user].influence();
    users[user].post();
    users[user].refollow();

    media[medium].post();

}

int main(void) {
    random_seed(seed);
    SNS sns(N, E);
    UserAgent users[N_user];
    MediaAgent media[N_media]


    return 0;
}
