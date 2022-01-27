#include "include/prottype.hpp"


//SNSクラスのコンストラクタ
SNS::SNS(int N, int E, int MF){
	this->N = N;
	this->E = E;

	//グラフのノード数を人数分にする(最初は一つのノードだけを宣言している)
	network.resize(N);

	//ノードに番号と色(意見)を付与する
	for(int i = 0; i < N; i++){
		network[i].node_id = i;
	}
	//一般ユーザのみのSNSネットワークを作成
	er_directed_network_generator(network, N_user, E);

	//メディアをネットワークに追加
	for(int medium = N_user; medium < N; ++medium){
		int mf = 0;
		while(mf < MF){
			int follower = random_int(0, N_user - 1);
			if(find(network[follower].follow.begin(), network[follower].follow.end(), medium) == network[follower].follow.end()){
				network[follower].follow.push_back(medium);
				network[medium].follower.push_back(follower);
				//cout << "media " << medium << " get " << follower << endl; 
				++mf;
			}
		}
	}
}

//クラスのメソッド定義
void SNS::push(Message msg){
    msgdb.push_back(msg);
}

//start_userからend_userの線を削除
void SNS::remove_edge(int start_user, int end_user){
	vector<int> &follow_StartUser = network[start_user].follow;
	vector<int> &follow_EndUser = network[end_user].follow;

	for(auto itr = follow_StartUser.begin(); itr != follow_StartUser.end(); ++itr) {
		//*itr でイテレータの指す要素を参照
	if(*itr == end_user){
		follow_StartUser.erase(itr);
		break;
		}
	}

	for(auto itr = follow_EndUser.begin(); itr != follow_EndUser.end(); ++itr){
		//*itr でイテレータの指す要素を参照
	if(*itr == start_user){
		follow_EndUser.erase(itr);
		break;
		}
	}
}

//start_userからend_userの線を追加
void SNS::add_edge(int start_user, int end_user){
  network[start_user].follow.push_back(end_user);
  network[end_user].follower.push_back(start_user);
}


//ネットワークの定義関数
void er_directed_network_generator(vector<node> &network, int vertex, int edge){
  int start_node, end_node;

  //ノードのペアの組み合わせテーブル size = [vertex][vertex]
  vector<vector<char>> selected_node(vertex, vector<char>(vertex, 0));
  
  for(int i = 0; i < edge; i++){
    //ノードa(始点となるノード)を指定
    while(true){
      start_node = random_int(0, vertex-1);
      //自分以外のすべてのノードとすでに接続されている場合はもう一度start_nodeを選択する
      //自分以外のノード数 = vertex - 1
      if(network[start_node].follow.size() < vertex - 1){
        break;
      }
    }
    //ノードaと同じでないかつ、まだフォローしていないノードb(終点ノード)を決定
    while(true){
      end_node = random_int(0, vertex-1);
      if(start_node != end_node && selected_node[start_node][end_node] == 0){
        break;
      }
    }
    //ノードaのoutノード一覧にbを追加
    network[start_node].follow.push_back(end_node);
    //ノードbのinノード一覧にaを追加
    network[end_node].follower.push_back(start_node);
    //ノードテーブルを更新
    selected_node[start_node][end_node] = 1;
  }
  return;
}
