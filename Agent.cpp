#include "include/Prottype.hpp"

//divide screen posts follow bounded confidence(ε)
tuple<vector, vector> UserAgent:: divide_post(SNS &sns){
    //get my screen post
    vector<int> &follows = sns.network[myid].follow;
    vector<Message> &msgdb = sns.msgdb;
    //vector<Message> screen;
    vector<Message> similar_post;
    vector<Message> unsimilar_post;
    //要素数の容量を確保。あくまでsizeは0
    //screen.reserve(l);

    for(auto msg = msgdb.end(); msg != msgdb.begin(); --msg){
        //followsの中にいる人が投稿していたらsimilarかunsimilarに追加
        if(find(follows.begin(), follows.end(), msg.post_user) != follows.end()){
            //screen.push_back(msg);
            if(abs(msg - msg.opinion) < EP){
                similar_post.push_back(msg);
            }
            else{
                //メディアを信頼しているかつ元投稿者がメディアなら許容範囲外でもsimilar
                if(confidence == true && msg.original_user >= N_user){
                    similar_post.push_back(msg);
                }
                else{
                    unsimilar_post.push_back(msg);
                }
            }
        }
        //スクリーンのサイズ追加されたら抜ける
        if(similar_post.size() + unsimilar_post.size() >= l){
            break;
        }
    }
    return forward_as_tuple(similar_post, unsimilar_post);
}

void UserAgent::post(int time, SNS &sns, vector<Message> &similar_post){
    if(random_uniform(0.0, 1.0) < p){//repost
        int repost_msg = random_int(0, similar_post.size());
        Message msg = {"repost", time, myid, similar_post[repost_msg].post_user, similar_post[repost_msg].opinion};
    }
    else{//post
        Message msg = {"post", time, myid, myid, o};
    }
    sns.push(msg);
}

void UserAgent::influence(vector<Message> &similar_post){
    vector<float> diffrence_opinion;
    float average_diffrence_opinion;

    //すべての意見差を求める
    for(int i = 0; i < similar_post.size(); i++){
        diffrence_opinion.push_back(similar_post[i].opinion - o);
    }

    //全意見差の平均を求める
    average_diffrence_opinion = accumulate(diffrence_opinion.begin(), diffrence_opinion.end(), 0.0) / diffrence_opinion.size();

    //影響を受ける
    o += (M * average_diffrence_opinion);
}

void UserAgent::refollow(SNS &sns, vector<Massage> &unsimilar_post){
    //投稿を一つランダムに選んで、その投稿者をアンフォローする
    int remove = rand_int(0, unsimilar_post.size());
    vector<int> &follow = sns.network[myid].follow;
    
    if(random_uniform(0.0, 1.0) < q){
        //アンフォロー
        for(auto f = follow.begin(); f != follow.end(); ++f){
            if(*f == unsimilar_post[remove].post_user){
                follow.erase(f);
                break;
            }
        }
        
        
    }
}

void MediaAgent::initialize(int index)
{
    this->myid = index + N_user;
    this->opinion_range[0] = opinion_ranges[index][0];
    this->opinion_range[1] = opinion_ranges[index][1];
    this->o = random_uniform(opinion_range[0], opinion_range[1]);
}