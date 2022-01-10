#include "include/prottype.hpp"


//SNSクラスのコンストラクタ
SNS::SNS(int N, int E){
  this->N = N;
  this->E = E;

  //グラフのノード数を人数分にする(最初は一つのノードだけを宣言している)
  network.resize(N);

  //ノードに番号を付与する
  for(int i = 0; i < N; i++){
    network[i].node_id = i;
  }
  //SNSネットワークを作成
  er_directed_network_generator(network, N, E);
}

//クラスのメソッド定義
void SNS::push(Message msg){
    msgdb.push_back(msg);
}

void SNS::remove_edge(int start, int end){
  vector<int> &follow_StartUser = network[start];
  vector<int> &follow_EndUser = network[end];

  for(auto itr = follow_StartUser.begin(); itr != follow_StartUser.end(); ++itr) {
      //*itr でイテレータの指す要素を参照
    if(*itr == end){
      follow_StartUser.erase(itr);
      break;
    }
  }

  for(auto itr = follower_EndUser.begin(); itr != follower_EndUser.end(); ++itr) 
  {
      //*itr でイテレータの指す要素を参照
    if(*itr == start){
      follower_EndUser.erase(itr);
      break;
    }
  }
}

void SNS::add_edge(int start, int end){
  network[start].follow.emplace_back(end);
  network[end].follower.emplace_back(start);
}


//ネットワークの定義関数
void er_directed_network_generator(vector<node> &network, int vertex, int edge){
  int start_node, end_node;

  //ノードのペアの組み合わせテーブル size = [vertex][vertex]
  vector<vector<char>> selected_node(vertex, vector<char>(vertex, 0));
  
  for(int i = 0; i < edge; i++){
    //ノードa(始点となるノード)を指定
    while(1){
      start_node = random_int(0, vertex-1);
      //自分以外のすべてのノードとすでに接続されている場合はもう一度start_nodeを選択する
      //自分以外のノード数 = vertex - 1
      if(network[start_node].follow.size() < vertex - 1){
        break;
      }
    }
    //ノードaと同じでないかつ、まだフォローしていないノードb(終点ノード)を決定
    while(1){
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