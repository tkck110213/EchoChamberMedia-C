#include "include/Prottype.hpp"
//UserAgent::UserAgent(unsigned short int myid)
void UserAgent::initialize(int myid)
{
    this->myid = myid;//thisをつけ忘れてた
    //ensure capacity of screen to size l 
    this->screen.reserve(l);

    if (myid < (int)(N_user * confidence_level)){
        confidence = true;
    }
    else{
        confidence = false;
    }
}

//divide screen posts follow bounded confidence(ε)
void UserAgent:: divide_post(SNS &sns, vector<Message> &similar_post, vector<Message> &unsimilar_post){
    
    vector<int> &follows = sns.network[myid].follow;
    vector<Message> &msgdb = sns.msgdb;

    //screenに新しく追加するから、初期化する
    screen.clear();

    for(auto msg = msgdb.rbegin(); msg != msgdb.rend(); ++msg){//逆イテレータをちゃんと使おう
        //followsの中にいる人が投稿していたらsimilarかunsimilarに追加
        //find関数は見つからなかった時、最後のポインタを返す
        if(find(follows.begin(), follows.end(), msg->post_user) != follows.end()){
            //screen.push_back(msg);
            if(abs(msg->opinion - o) < EP){//条件文の書き忘れ
				//cout << abs(msg->opinion - o) << endl;
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
            //cout << msg->opinion << endl;
            screen.push_back(*msg);
            
        }
		//スクリーンのサイズ追加されたら抜ける
		if(screen.size() >= l){
			break;
		}
    }
    //printf("flag divede\n");
}

//User posts message which user's opinion or similar opinion to database of sns 
void UserAgent::post(int time, SNS &sns, vector<Message> &similar_post){
    Message msg;
    //printf("flag post\n");
    if(random_uniform(0.0, 1.0) <= p && similar_post.empty() != true){//repost
        int repost_msg = random_int(0, similar_post.size() - 1);
		//cout << "repost msg" << repost_msg << endl;
         msg.post_type = "repost";
         msg.post_time = time;
         msg.post_user = myid;
         msg.original_user = similar_post[repost_msg].post_user;
         msg.opinion = similar_post[repost_msg].opinion;
         //{"repost", time, myid, similar_post[repost_msg].post_user, similar_post[repost_msg].opinion};
    }
    else{//post
         msg.post_type = "post";
         msg.post_time = time;
         msg.post_user = myid;
         msg.original_user = myid;
         msg.opinion = o;
    }
    
    sns.push(msg);
}

void UserAgent::influence(vector<Message> &similar_post){
	if(similar_post.empty() != true){
        //printf("flag\n");
        //cout << "similar size " << similar_post.size() << endl;
        vector<float> diffrence_opinion;
        float average_diffrence_opinion;
        //cout << "my opinion " << o << endl;

        //すべての意見差を求める
        for(int i = 0; i < similar_post.size(); i++){
            //cout << "similar opinion " << similar_post[i].opinion << endl;
            diffrence_opinion.push_back(similar_post[i].opinion - o);
        }
        /*for(int i = 0; i < diffrence_opinion.size(); i++){
            cout << "diffrence value" << i << " " << diffrence_opinion[i] << endl;
        }*/
		//cout << "diffrence_size " << diffrence_opinion.size() << endl;
        //全意見差の平均を求める
        average_diffrence_opinion = accumulate(diffrence_opinion.begin(), diffrence_opinion.end(), 0.0) / diffrence_opinion.size();
        //cout << "average " << average_diffrence_opinion << endl;
        //影響を受ける
        //cout << "before o" << o << endl;
        o += (M * average_diffrence_opinion);
        //cout << "after o" << o << endl << endl;
        
    }
    //printf("flag influence\n");
}

void UserAgent::refollow(SNS &sns, vector<Message> &unsimilar_post){
    if(random_uniform(0.0, 1.0) <= q && unsimilar_post.empty() != true){
        //投稿を一つランダムに選んで、その投稿者をアンフォローする
        int remove_user;
        int follow_user;
        int screen_size = screen.size();
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
                    //Nになっていた
                    //Nも含んでしまうから1を引かないといけない
                    follow_user = random_int(0, N - 1);
                    //フォロー中にfollow_userがいないかつ、自分でなければ抜ける
                    if(find(follow.begin(), follow.end(), follow_user) == follow.end() && follow_user != myid){
                        break;
                    }
                }
        }
        else{//フォロー候補の中から1人選択
            follow_user = follow_candidates[random_int(0, follow_candidates.size() - 1)];
        }

        //選択したユーザをアンフォロー
        //printf("remove user: %d\n", remove_user);
        sns.remove_edge(myid, remove_user);
        //printf("follow user: %d\n", follow_user);
        sns.add_edge(myid, follow_user);
        /*
        printf("remove user: %d\n", remove_user);
        sns.remove_edge(myid, remove_user);
        for(int i = 0; i < sns.network[myid].follow.size(); ++i){
            cout << sns.network[myid].follow[i] << ", ";
        }
        cout << endl;
        //選択したユーザをフォロー
        printf("follow user: %d\n", follow_user);
        
        for(int i = 0; i < sns.network[myid].follow.size(); ++i){
            cout << sns.network[myid].follow[i] << ", ";
        }
        cout << endl;*/
        //printf("flag refollow\n");
    }
}

void MediaAgent::initialize(int myid)
{
    this->myid = myid;
    opinion_range[0] = opinion_ranges[myid - N_user][0];
    opinion_range[1] = opinion_ranges[myid - N_user][1];
    o = random_uniform(opinion_range[0], opinion_range[1]);
}

void MediaAgent::post(int time, SNS &sns){
    if(random_uniform(0.0, 1.0) < p_media){//uniformなのにintになってた
        o = random_uniform(opinion_range[0], opinion_range[1]);
        Message msg = {"post", time, myid, myid, o};
        sns.push(msg);
    }   
}
