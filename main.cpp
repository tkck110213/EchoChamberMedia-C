#include "include/Prottype.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int seeds[max_n];
int origin_seed;
int MF;
float confidence_level;
float opinion_ranges[N_media][2];
bool opinion_change;

void export_graph(int n, int time, SNS &sns, array<UserAgent, N_user> &users, array<MediaAgent, N_media> &media, string ResultPath){
    stringstream ResultGraphRelationPath;
    ResultGraphRelationPath << ResultPath << "tmp/";
    ResultGraphRelationPath << follow_method << "_";
    ResultGraphRelationPath << fixed << setprecision(1) << "c" << confidence_level << "_";
    ResultGraphRelationPath << "mf" << MF << "_";
    ResultGraphRelationPath << "n" << n << "_";
    ResultGraphRelationPath << time << "step";
    ResultGraphRelationPath << "_Relation" << ".csv";

    stringstream ResultGraphMetaPath;
    ResultGraphMetaPath << ResultPath << "tmp/";
    ResultGraphMetaPath << follow_method << "_";
    ResultGraphMetaPath << fixed << setprecision(1) << "c" << confidence_level << "_";
    ResultGraphMetaPath << "mf" << MF << "_";
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
        //Agentid, opinion, label(cofidence) 
        if(i < N_user){//一般エージェント
            Meta << i << ","<< users[i].o << ",";
            if(users[i].confidence == true){
                Meta << "c" << endl;
            }
            else{
                Meta << "" << endl;
            }
        }
        else{//メディア
            Meta << i << ","<< (media[i-N_user].opinion_range[0] + media[i-N_user].opinion_range[1]) / 2.0 << "," << "media" << endl;
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

void export_parameter(string ResultPath, string abstract){
    string ParameterPath = ResultPath + "parameter.txt";
    ofstream parameter(ParameterPath);

    parameter << setprecision(2) << "# Experiment abstract" << endl;
    parameter << abstract << endl << endl;

    parameter << "# parameter " << endl;    
    parameter << "max_n : " << max_n << endl;
    parameter << "origin seed : " << origin_seed << endl;
    parameter << "seeds : [";
    for(int i = 0; i < max_n; ++i){
        parameter << seeds[i] << ", ";
    }
    parameter << "]" << endl;

    parameter << "confidence level : " << confidence_level << endl;
    parameter << "media follower : " << MF << endl;
    parameter << "opinion range : " << endl;
    for(int i = 0; i < N_media; ++i){
        parameter << "[" << opinion_ranges[i][0] << ", " << opinion_ranges[i][1] << "]" << endl;
    }
    parameter << "opinion_change : " << opinion_change << endl << endl;

    parameter << "N : " << N << endl;
    parameter << "N_user : " << N_user << endl;
    parameter << "N_media : " << N_media << endl;
    parameter << "E : " << E << endl;
    parameter << "p : " << p << endl;
    parameter << "p_media : " << p_media << endl;
    parameter << "q : " << q << endl;
    //parameter << "p_media : " << p_media << endl;
    parameter << "l : " << l << endl;
    parameter << "T : " << T << endl;
    parameter << "EP : " << EP << endl;
    parameter << "M : " << M << endl;
    
    parameter << "follow_method : " << follow_method << endl;
    
}

void export_log(int n, int t, string ResultPath, array<MediaAgent, N_media> &media){
    string LogPath = ResultPath + "log.txt";
    ofstream Log(LogPath, ios_base::app);

    Log << "n=" << n << " " << t << "step" << endl;
    for(int m = 0; m < N_media; ++m){
        Log << "    media " << m << " change opinion range" << endl;
        Log << "    " << media[m].opinion_range[0] << " " << media[m].opinion_range[1] << endl;
        Log << "    media " << m << " opinion " << media[m].o << endl  << endl;
    }
}

void export_2d_data(int n, vector<vector<float>> &data, string ResultPath, string FileName){
    stringstream ResultDataPath;

    ResultDataPath << fixed << setprecision(1) << ResultPath << "data/";
    ResultDataPath << follow_method << "_";
    ResultDataPath << "c" << confidence_level << "_";
    ResultDataPath << "mf" << MF << "_";
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
    vector<int> hist(bins, 0);
    float interval = 2.0 / float(bins); //intになってた
    //cout << "interval " << interval << endl;
    float entropy = 0.0;

    //ヒストグラムを作成
    //個数が0だった時の対策で、個数に+1をする
    for(int i = 0; i < bins; ++i){
        float range1 = -1.0 + interval * i;
        float range2 = -1.0 + interval * (i+1);
        if(i < bins - 1){
            //dがopinionを表すのにintになっていた
            //条件文がpythonの書き方になっていた
            hist.at(i) = count_if(data.begin(), data.end(),[range1, range2](float d)->bool{return(range1 <= d && d < range2);}) + 1;            
        }
        else{
            //最後は階級の不等号が変わる
            hist.at(i) = count_if(data.begin(), data.end(),[range1, range2](float d)->bool{return(range1 <= d && d <= range2);}) + 1;
        }
        
    }

    int data_sum = accumulate(hist.begin(), hist.end(), 0);

    //階級ごとの発生確率を計算
    for(int i = 0; i < bins; ++i){
        pk.at(i) = float(hist.at(i)) / float(data_sum);
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

string get_resultpath(string date){
    stringstream ResultPath;
    ResultPath  << "./result/" << date << "/c" << fixed << setprecision(1) << confidence_level << "mf" << MF << "/";

    return ResultPath.str();
}

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        cerr << "No program parameters provided !" << endl;
        exit(EXIT_FAILURE);
    }
    //パラメータが書かれたjsonファイルを引数に取る
    ifstream ifs("./parameter.json");
    json parameter;
    ifs >> parameter;

    origin_seed = parameter["origin_seed"];

    string abstract = parameter["abstract"];
    string date = get_date();

    //opinion rangeをグローバル変数に代入
    if(parameter["opinion_range"].size() != N_media){
        cerr << "Need same size opinion range and media" << endl;
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < N_media; ++i){
        opinion_ranges[i][0] = parameter["opinion_range"][i][0];
        opinion_ranges[i][1] = parameter["opinion_range"][i][1];
    }
    opinion_change = parameter["opinion_change"];

    random_seed(origin_seed);
    
    for (const float &c : parameter["confidence_level"]){
        for (const float &mf : parameter["media_follower"]){
            confidence_level = c;
            MF = mf;
            
            string ResultPath = get_resultpath(date);

            //結果を格納するresultフォルダの作成
            filesystem::create_directories(ResultPath);
            filesystem::create_directories(ResultPath + "tmp/");
            filesystem::create_directories(ResultPath + "data/");
            filesystem::create_directories(ResultPath + "graph/");

            //各試行のシード値作成
            for(int i = 0; i < max_n; ++i){
                seeds[i] = random_int(10, 100000);
            }

            //各種パラメータの出力
            //フォルダを作る前から出力しようとしていた
            export_parameter(ResultPath, abstract);

            for(int n = 0; n < max_n; ++n){
                //試行ごとにシード値を設定
                random_seed(seeds[n]);
                cout << "c" << fixed << setprecision(1) << confidence_level << " mf" << MF << endl;
                cout << n + 1 << "/" << max_n << " Execute..." << endl;
                SNS sns(N, E, MF);
                array<UserAgent, N_user> users;
                array<MediaAgent, N_media> media;
                vector<vector<float>> all_opinions(N_user, vector<float>(T));
                vector<vector<float>> all_diversity(N_user, vector<float>(T));
                
                //人数分のユーザエージェントを作成
                for(int i = 0; i < N_user; ++i){
                    users[i].initialize(i, confidence_level);
                    users[i].diversity = calc_diversity(users[i].screen);
                }
                //人数分のメディアエージェントを作成
                for(int i = 0; i < N_media; ++i){
                    media[i].add_id(i + N_user);
                    media[i].set_opinion_range(opinion_ranges[i][0], opinion_ranges[i][1]);
                }
                
                //シミュレーション開始
                for(int time = 0; time < T; ++time){
                    
                    //グラフの出力と現時点での意見を保存
                    if(time % 1000 == 0){
                        export_graph(n, time, sns, users, media, ResultPath);
                    }
                    for(int i = 0; i < N_user; ++i){
                        all_opinions[i][time] = users[i].o;
                        all_diversity[i][time] = users[i].diversity;
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
                    if(opinion_change == true && time % 1000 == 0 && time <= 10000){
                        float abs_min, abs_max, interval_range;
                        for(int m = 0; m < N_media; ++m){
                            //意見の変化幅を決める
                            if(abs(media[m].opinion_range[0]) < abs(media[m].opinion_range[1])){
                                abs_max = media[m].opinion_range[1];
                                abs_min = media[m].opinion_range[0];
                            }
                            else{
                                abs_max = media[m].opinion_range[0];
                                abs_min = media[m].opinion_range[1];
                            }
                            interval_range = (abs_min - abs_max) / 2.0;
                            
                            media[m].set_opinion_range(media[m].opinion_range[0] + interval_range, media[m].opinion_range[1] + interval_range);
                        }
                        
                        export_log(n, time, ResultPath, media);
                    }
                    media[medium].post(time, sns);

                    //screenを更新するフォロワーを格納するsetを作成
                    set<int> renew_screen_user{};
                    vector<int> &follower = sns.network[user].follower;
                    vector<int> &media_follower = sns.network[medium + N_user].follower;
                    int follower_size = follower.size();
                    int media_follower_size = media_follower.size();

                    renew_screen_user.insert(user);
                    for(int i = 0; i < follower_size; ++i){
                        renew_screen_user.insert(follower[i]);
                    }
                    for(int i = 0; i < media_follower_size; ++i){
                        renew_screen_user.insert(media_follower[i]);
                    }
                    
                    //screenとエントロピーを更新
                    for(auto f : renew_screen_user) {
                        users[f].renew_screen(sns);                
                        users[f].diversity = calc_diversity(users[f].screen);
                    }
                }

                export_graph(n, T, sns, users, media, ResultPath);
                export_2d_data(n, all_opinions, ResultPath, "all_opinions");
                export_2d_data(n, all_diversity, ResultPath, "all_diversity");

                cout << n + 1 << "/" << max_n << " Done!!" << endl << endl;
            }
            stringstream GraphConvertCommand;
            string GraphConvertCommandPath = "./" + date + "-ExportGraphSVG.bat";
            ofstream CommandBat(GraphConvertCommandPath, ios_base::app);
            GraphConvertCommand << "python ExportGraphSVG.py " << N << " " << fixed << setprecision(1) << confidence_level << " " << MF << " " << ResultPath << endl;
            
            CommandBat << GraphConvertCommand.str();

        }
    }
    return 0;
}
