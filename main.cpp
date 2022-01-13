#include <iostream>
#include "include/Prottype.hpp"

void export_graph(int n, int time, SNS &sns, array<UserAgent, N_user> users, array<MediaAgent, N_media> media, string ResultPath){
    stringstream ResultGraphRelationPath;
    ResultGraphRelationPath << ResultPath << "graph/";
    ResultGraphRelationPath << follow_method << "_";
    ResultGraphRelationPath << "c" << confidence_level << "_";
    ResultGraphRelationPath << "mf" << media_follower << "_";
    ResultGraphRelationPath << "n" << n << "_";
    ResultGraphRelationPath << time << "step";
    ResultGraphRelationPath << "_Relation" << ".csv";

    stringstream ResultGraphMetaPath;
    ResultGraphMetaPath << ResultPath << "graph/";
    ResultGraphMetaPath << follow_method << "_";
    ResultGraphMetaPath << "c" << confidence_level << "_";
    ResultGraphMetaPath << "mf" << media_follower << "_";
    ResultGraphMetaPath << "n" << n << "_";
    ResultGraphMetaPath << time << "step";
    ResultGraphMetaPath << "_Meta" << ".csv";

    ofstream Relation(ResultGraphRelationPath.str());  // ファイルパスを指定する
    ofstream Meta(ResultGraphMetaPath.str());
    /*ノードaからb、ノードcからdの時は以下のように表記される
    a, b
    c, d*/
    for(int i = 0; i < N; ++i){
        vector<int> &follow = sns.network[i].follow;
        int amount_follow = follow.size();
        for(int j = 0; j < amount_follow; ++j){
            Relation << i << ", "<< follow[j] << endl;
        }

        //メタデータ(意見など)を付与
        if(i < N_user){
            Meta << i << ","<< users[i].o << ",";
            if(users[i].confidence == true){
                Meta << "c" << endl;
            }
            else{
                Meta << "" << endl;
            }
        }
        else{
            Meta << i << ","<< media[i - N_user].o << "," << "media" << endl;
        }
    }
}

string get_date(void){
    //現在日時を取得する
    time_t t = time(nullptr);
    
    //形式を変換する    
    const tm* lt = localtime(&t);
    
    //sに独自フォーマットになるように連結していく
    stringstream s;
    s << "20";
    s << lt->tm_year-100; //100を引くことで20xxのxxの部分になる
    s << "-";
    s << setw(2) << setfill('0') << lt->tm_mon+1; //月を0からカウントしているため
    s << setw(2) << setfill('0') << lt->tm_mday; //
    s << "-";
    s << setw(2) << setfill('0') << lt->tm_hour;
    s << setw(2) << setfill('0') << lt->tm_min;
    s << setw(2) << setfill('0') << lt->tm_sec;
    return s.str();
}

int main(void) {
    random_seed(seed); 
    string ResultPath = "./result/" + get_date() + "/";

    //結果を格納するresultフォルダの作成
    filesystem::create_directories(ResultPath);
    filesystem::create_directories(ResultPath + "graph/");
    filesystem::create_directories(ResultPath + "data/");
    filesystem::create_directories(ResultPath + "gexf/");

    for(int n = 0; n < max_n; ++n){
        cout << "n " << n << endl;
        SNS sns(N, E);
        array<UserAgent, N_user> users;
        array<MediaAgent, N_media> media;
        
        //人数分のユーザエージェントを作成
        for(int i = 0; i < N_user; ++i){
            users[i].initialize(i);
        }
        //人数分のメディアエージェントを作成
        for(int i = 0; i < N_media; ++i){
            media[i].initialize(i + N_user);
        }
        
        for(int time = 0; time < T; ++time){
            
            if(time % 2000 == 0){
                export_graph(n, time, sns, users, media, ResultPath);
            }

            int user = random_int(0, N_user - 1);
            int medium = random_int(0, N_media - 1);
            vector<Message> similar_post;
            vector<Message> unsimilar_post;
            
            users[user].divide_post(sns, similar_post, unsimilar_post);
            users[user].influence(similar_post);
            users[user].post(time, sns, similar_post);
            users[user].refollow(sns, unsimilar_post);
            
            if(time == T/2){
                for(int m = 0; m < N_media; ++m){
                    media[m].change_opinion_range(-0.1, 0.1);
                    //printf("range0 %f\n", media[m].opinion_range[0]);
                    //printf("range1 %f\n", media[m].opinion_range[1]);
                    //printf("opinion %f\n", media[m].o);
                }
            }

            media[medium].post(time, sns);

        }

        export_graph(n, T, sns, users, media, ResultPath);
    }

    return 0;
}
