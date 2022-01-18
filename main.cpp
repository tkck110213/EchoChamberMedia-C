#include <iostream>
#include "include/Prottype.hpp"
#include <time.h>
void export_2nd_csv(int n, vector<vector<float>> &data, string ResultPath, string filename);

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

void export_2nd_csv(int n, vector<vector<float>> &data, string ResultPath, string filename){
    stringstream Result2ndCsvPath;
    Result2ndCsvPath << ResultPath << "data/";
    Result2ndCsvPath << follow_method << "_";
    Result2ndCsvPath << "c" << confidence_level << "_";
    Result2ndCsvPath << "mf" << media_follower << "_";
    Result2ndCsvPath << "n" << n << "_";
    Result2ndCsvPath << filename << ".csv";

    ofstream Result2ndCsvData(Result2ndCsvPath.str());
    int RowSize = data.size();
    int ColSize = data.at(0).size();

    for(int i = 0; i < RowSize; ++i){
        for(int j = 0; j < ColSize; ++j){
            Result2ndCsvData << data.at(i).at(j) << ",";
        }
        Result2ndCsvData << endl;
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
    clock_t start = clock();
    random_seed(seed); 
    string ResultPath = "./result/" + get_date() + "/";

    //結果を格納するresultフォルダの作成
    filesystem::create_directories(ResultPath);
    filesystem::create_directories(ResultPath + "graph/");
    filesystem::create_directories(ResultPath + "data/");
    filesystem::create_directories(ResultPath + "gexf/");

    //max_n回シミュレーションを実行する
    for(int n = 0; n < max_n; ++n){
        cout << "n " << n << endl;
        SNS sns(N, E);
        array<UserAgent, N_user> users;
        array<MediaAgent, N_media> media;
        //全ステップの全ユーザの意見
        vector<vector<float>> all_step_opinions(N, vector<float>(T));
        float opinion_range[amount_type_opinion_range*media_per_opinion_range][2];
        
        //人数分のユーザエージェントを作成
        for(int i = 0; i < N_user; ++i){
            users[i].initialize(i);
        }
        //人数分のopinion rangeの作成
        //opinion rangeの各種類を順番に渡していく
        for(int i = 0; i < N_media; i += media_per_opinion_range){
            for(int j = 0; j < amount_type_opinion_range; ++j){
                opinion_range[i + j][0] = opinion_ranges[i][0];
                opinion_range[i + j][1] = opinion_ranges[i][1];
            }
        }
        

        //人数分のメディアエージェントを作成
        for(int i = 0; i < N_media; ++i){
            media[i].initialize(i + N_user, opinion_range[i]);  
        }
        
        //シミュレーションをstep Tまで実行
        for(int time = 0; time < T; ++time){
            
            //グラフの出力
            if(time % 2000 == 0){
                export_graph(n, time, sns, users, media, ResultPath);
            }
            //各エージェントのopinionの履歴を保存
            
            for(int i = 0; i < N_user; ++i){//usersの添え字はN_userまで
                all_step_opinions.at(i).at(time) = users[i].o;
            }
            for(int i = 0; i < N_media; ++i){
                all_step_opinions.at(i + N_user).at(time) = media[i].o;
            }
            
            //シミュレーション開始
            int user = random_int(0, N_user - 1);
            int medium = random_int(0, N_media - 1);
            vector<Message> similar_post;
            vector<Message> unsimilar_post;
            
            users[user].divide_post(sns, similar_post, unsimilar_post);
            users[user].influence(similar_post);
            users[user].post(time, sns, similar_post);
            users[user].refollow(sns, unsimilar_post);
            
            //シミュレーションの折り返し地点でメディアのopinion rangeを変更
            if(time == T/2){
                for(int m = 0; m < N_media; ++m){
                    media[m].change_opinion_range(-0.1, 0.1);
                }
            }
            media[medium].post(time, sns);
            printf("flag\n");
        }
        
        export_graph(n, T, sns, users, media, ResultPath);
        
        //export_2nd_csv(n, all_step_opinions, ResultPath, "all_step_opinions");
        
    }
    clock_t end = clock();
    cout << "duration = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";
    return 0;
}
