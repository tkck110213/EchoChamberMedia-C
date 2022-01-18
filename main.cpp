#include <iostream>
#include "include/Prottype.hpp"

void export_graph(int n, int time, SNS &sns, array<UserAgent, N_user> &users, array<MediaAgent, N_media> &media, string ResultPath){
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

void export_2d_data(int n, vector<vector<float>> &data, string ResultPath, string FileName){
    stringstream ResultDataPath;

    ResultDataPath << ResultPath << "data/";
    ResultDataPath << follow_method << "_";
    ResultDataPath << "c" << confidence_level << "_";
    ResultDataPath << "mf" << media_follower << "_";
    ResultDataPath << "n" << n << "_";
    ResultDataPath << FileName << ".csv";

    ofstream ResultData(ResultDataPath.str());
    int RowSize = data.size();
    int ColSize = data[0].size();

    for(int i = 0; i < RowSize; ++i){
        for(int j = 0; j < ColSize; ++j){
            ResultData << data[i][j] << ",";
        }
        ResultData << endl;
    }
}

float shannon_entropy(vector<float> &data, int bins){
    vector<float> pk(bins);//各事象の発生確率
    vector<int> hist(bins);
    int interval = 2 / bins;
    float entropy = 0.0;

    //ヒストグラムを作成
    //個数が0だった時の対策で、個数に+1をする
    for(int i = 0; i < bins; ++i){
        if(i + 1 != bins){
            hist.at(i) = count_if(data.begin(), data.end(),[interval](int i)->bool{return(-1.0 + interval * i <= i < -1.0 + interval * (i+1));}) + 1;
        }
        else{
            //最後は階級の不等号が変わる
            hist.at(i) = count_if(data.begin(), data.end(),[interval](int i)->bool{return(-1.0 + interval * i <= i <= -1.0 + interval * (i+1));}) + 1;
        }
    }

    int data_sum = accumulate(hist.begin(), hist.end(), 0);

    //階級ごとの発生確率を計算
    for(int i = 0; i < bins; ++i){
        pk.at(i) = float(hist.at(i)) / float(data_sum);
        //cout << pk.at(i) << endl;
    }

    for(int i = 0; i < bins; ++i){
        entropy += (pk.at(i) * -1.0 * log2f(pk.at(i)));
    }
    
    return entropy;
}

float calc_diversity(vector<Message> &screen){
    int screen_size = screen.size();
    vector<float> screen_opinions(screen_size);

    for(int i = 0; i < screen_size; ++i){
        screen_opinions.at(i) = screen.at(i).opinion;
    }
    
    return shannon_entropy(screen_opinions, 10);
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
        cout << n + 1 << "/" << max_n << " Done" << endl;
        SNS sns(N, E);
        array<UserAgent, N_user> users;
        array<MediaAgent, N_media> media;
        vector<vector<float>> all_opinions(N_user, vector<float>(T));
        vector<vector<float>> all_diversity(N_user, vector<float>(T));
        
        //人数分のユーザエージェントを作成
        for(int i = 0; i < N_user; ++i){
            users[i].initialize(i);
        }
        //人数分のメディアエージェントを作成
        for(int i = 0; i < N_media; ++i){
            media[i].initialize(i + N_user);
        }
        
        //シミュレーション開始
        for(int time = 0; time < T; ++time){
            
            //グラフの出力と現時点での意見を保存
            if(time % 2000 == 0){
                export_graph(n, time, sns, users, media, ResultPath);
            }
            for(int i = 0; i < N_user; ++i){
                all_opinions[i][time] = users[i].o;
            }
            

            int user = random_int(0, N_user - 1);
            int medium = random_int(0, N_media - 1);
            vector<Message> similar_post;
            vector<Message> unsimilar_post;
            
            //ランダムに選んだユーザが行動する
            users[user].divide_post(sns, similar_post, unsimilar_post);
            users[user].influence(similar_post);
            users[user].post(time, sns, similar_post);
            users[user].refollow(sns, unsimilar_post);
            
            //opinion rangeを変更する
            if(opinion_change == true && time == T/2){
                for(int m = 0; m < N_media; ++m){
                    media[m].change_opinion_range(-0.1, 0.1);
                }
            }
            media[medium].post(time, sns);
        }

        export_graph(n, T, sns, users, media, ResultPath);
        export_2d_data(n, all_opinions, ResultPath, "all_opinions");
        export_2d_data(n, all_diversity, ResultPath, "all_diversity");
    }

    return 0;
}
