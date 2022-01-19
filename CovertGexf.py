import networkx as nx
import csv
import sys
import glob
import os
import shutil

def convert_gexf(N, ResultPath):
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

		Graph = nx.DiGraph()

		#ノードを作成
		for i in range(N):
			Graph.add_node(GraphMeta[i][0], color=GraphMeta[i][1], label=GraphMeta[i][2])

		#エッジの追加
		Graph.add_edges_from(GraphRelation)

		FileName = os.path.splitext(os.path.basename(MetaFilePath))[0]
		nx.write_gexf(Graph, "{}graph/{}{}".format(ResultPath, FileName.strip("_Meta.csv"), ".gexf"))
		#print(r"{}\gexf\{}{}".format(ResultPath, FileName.strip(r"_Meta.csv"), r".gexf"))

N = int(sys.argv[1])
ResultPath = sys.argv[2]
convert_gexf(N, ResultPath)
shutil.rmtree(ResultPath + "tmp/")