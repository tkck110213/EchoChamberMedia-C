import pathlib
import shutil
import re
import networkx as nx
import csv
import sys
import glob
import numpy as np
import os
import shutil
import matplotlib.pyplot as plt
import py4cytoscape as p4c


def change_suffix(file_name, from_suffix, to_suffix):
    # ファイルの拡張子を得る
    sf = pathlib.PurePath(file_name).suffix
    
    # 変更対象かどうか判定する
    if sf == from_suffix:
        # ファイル名(拡張子除く)を得る
        st = pathlib.PurePath(file_name).stem

        # 変更後のファイル名を得る
        to_name = os.path.dirname(file_name) + "/" + st + to_suffix

        # ファイル名を変更する
        shutil.move(file_name, to_name)


def export_graph(N, ResultPath):
	MetaFilesPath = glob.glob(ResultPath + "tmp/*Meta.csv")
	RelationFilesPath = glob.glob(ResultPath + "tmp/*Relation.csv")

	for MetaFilePath, RelationFilePath in zip(MetaFilesPath, RelationFilesPath):
		#print("{} {}".format(MetaFilePath, RelationFilePath))
		with open(RelationFilePath) as f:
			#GraphRelation = f.read().splitlines()
			GraphRelation = csv.reader(f)
			GraphRelation = [list(map(int, GR)) for GR in GraphRelation]#csv.readerで取得したファイルはwith文の中でしか開けない
			
		with open(MetaFilePath) as f:
			#GraphRelation = f.read().splitlines()
			GraphMeta = csv.reader(f)
			GraphMeta = [[int(GM[0]), float(GM[1]), GM[2]] for GM in GraphMeta]

	#NordCorlor = norm_color([GraphMeta[i][1] for i in range(N)])

		Graph = nx.DiGraph()


		#ノードを作成
		Graph.add_nodes_from([i for i in range(N)])
		#エッジの追加
		Graph.add_edges_from(GraphRelation)

		for i in range(N):
			Graph.nodes[i]["color"] = GraphMeta[i][1]
			Graph.nodes[i]["label"] = GraphMeta[i][2]
			Graph.nodes[i]["size"] = len(list(Graph.predecessors(i))) * 1.7 + 45
		

		FileName = os.path.splitext(os.path.basename(MetaFilePath))[0].strip("_Meta.csv")
		#nx.write_gexf(Graph, "{}graph/{}{}".format(ResultPath, FileName, ".gexf"))
		nx.write_graphml(Graph, "{}graph/{}{}".format(ResultPath, FileName, ".graphml"))

		#plt.figure(figsize=(15,15))
		#pos = nx.spring_layout(Graph, k=0.05)
		#ノードのサイズを、入次数に従って決定
		#nx.draw_networkx_nodes(Graph, pos, node_color=[Graph.nodes[n]["color"] for n in range(N)], alpha=0.8, cmap="jet", node_size=node_size)
		#nx.draw_networkx_labels(Graph, pos, labels={n:Graph.nodes[n]["label"] for n in range(N)}, fontsize=16, font_family="Yu Gothic", font_weight="bold")
		#nx.draw_networkx_edges(Graph, pos, alpha=0.4, width=0.3)
		#plt.savefig("{}graph/{}{}".format(ResultPath, FileName, ".png"))
		#plt.close()

		#print(r"{}\gexf\{}{}".format(ResultPath, FileName.strip(r"_Meta.csv"), r".gexf"))

N = int(sys.argv[1])
ResultPath = sys.argv[2]
export_graph(N, ResultPath)
#shutil.rmtree(ResultPath + "tmp/")