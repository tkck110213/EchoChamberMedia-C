#include "include/Prottype.hpp"
//UserAgent::UserAgent(unsigned short int myid)
void UserAgent::initialize(int myid)
{
    myid = myid;
    //ensure capacity of screen to size l 
    screen.reserve(l);

    if (myid < (int)(N_user * confidence_level)){
        confidence = true;
    }
    else{
        confidence = false;
    }
}

//divide screen posts follow bounded confidence(ε)
tuple<vector<Message>, vector<Message>> UserAgent:: divide_post(SNS &sns){
    vector<int> &follows = sns.network[myid].follow;
    vector<Message> &msgdb = sns.msgdb;
    vector<Message> similar_post;
    vector<Message> unsimilar_post;

    for(auto msg = msgdb.end(); msg != msgdb.begin(); --msg){
        //followsの中にいる人が投稿していたらsimilarかunsimilarに追加
        //find関数は見つからなかった時、最後のポインタを返す
        if(find(follows.begin(), follows.end(), msg->post_user) != follows.end()){
            //screen.push_back(msg);
            if(abs(msg->opinion - EP)){
                similar_post.push_back(*msg);
            }
            else{
                //メディアを信頼しているかつ元投稿者がメディアなら許容範囲外でもsimilar
                if(confidence == true && msg->original_user >= N_user){
                    similar_post.push_back(*msg);
                }
                else{
                    unsimilar_post.push_back(*msg);
                }
            }
            screen.push_back(*msg);
            //スクリーンのサイズ追加されたら抜ける
            if(similar_post.size() + unsimilar_post.size() >= l && screen.size() >= l){
                break;
            }
        }
        
    }
    return forward_as_tuple(similar_post, unsimilar_post);
}

//User posts message which user's opinion or similar opinion to database of sns 
void UserAgent::post(int time, SNS &sns, vector<Message> &similar_post){
    Message msg;

    if(random_uniform(0.0, 1.0) < p){//repost
        int repost_msg = random_int(0, similar_post.size());
        msg = {"repost", time, myid, similar_post[repost_msg].post_user, similar_post[repost_msg].opinion};
    }
    else{//post
        msg = {"post", time, myid, myid, o};
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

void UserAgent::refollow(SNS &sns, vector<Message> &unsimilar_post){
    if(random_uniform(0.0, 1.0) < q){
        //投稿を一つランダムに選んで、その投稿者をアンフォローする
        int remove_user;
        int follow_user;
        char screen_size = screen.size();
        vector<int> &follow = sns.network[myid].follow;
        vector<int> follow_candidates;
        follow_candidates.reserve(l);
        //アンフォローするユーザを選択
        remove_user = unsimilar_post[random_int(0, unsimilar_post.size() - 1)].post_user;
        

        //新しくフォローするユーザ候補を作成
        if(follow_method == "repost"){
            //フォローしていない再投稿元のユーザを候補に追加
            for(char i = 0; i < screen_size; i++){
                if(screen[i].post_type == "repost"){
                    if(find(follow.begin(), follow.end(), screen[i].original_user) != follow.end()){
                        follow_candidates.push_back(screen[i].original_user);
                    }
                }
            }
        }
        else if(follow_method == "recomendation"){
            //msg databaseから近い意見のユーザをl人候補者を選ぶ
            int msgdb_size = sns.msgdb.size();
            vector<Message> &msgdb = sns.msgdb;

            for(int i = msgdb_size - 1; i >= 0; ++i){
                if(abs(msgdb[i].opinion - o) < EP){
                    follow_candidates.push_back(msgdb[i].post_user);
                    if(follow_candidates.size() >= l){
                        break;
                    }
                }
            }
        }

        //フォローするユーザを選択
        //ランダムもしくはフォロー候補がいなかったとき
        if(follow_method == "random" || follow_candidates.empty() == true){
             while(true){
                 follow_user = random_int(0, N);
                 //フォロー中にfollow_userがいなかったら抜ける
                 if(find(follow.begin(), follow.end(), follow_user) == follow.end()){
                     break;
                 }
             }
        }
        else{//フォロー候補の中から1人選択
            follow_user = follow_candidates[random_int(0, follow_candidates.size() - 1)];
        }

        //選択したユーザをアンフォロー
        sns.remove_edge(myid, remove_user);
        //選択したユーザをフォロー
        sns.add_edge(myid, follow_user);
    }
}

void MediaAgent::initialize(int myid)
{
    myid = myid;
    opinion_range[0] = opinion_ranges[myid][0];
    opinion_range[1] = opinion_ranges[myid][1];
    o = random_uniform(opinion_range[0], opinion_range[1]);
}

void MediaAgent::post(int time, SNS &sns){
    if(random_int(0.0, 1.0) < p_media){
        o = random_uniform(opinion_range[0], opinion_range[1]);
        Message msg = {"post", time, myid, myid, o};
        sns.push(msg);
    }   
}