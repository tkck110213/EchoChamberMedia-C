import pathlib
import shutil
import re
import networkx as nx
import csv
import sys
import glob
import os
import shutil
import py4cytoscape as p4c
from tqdm import tqdm


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


def export_graph(N, c, ResultPath):
	MetaFilesPath = glob.glob(ResultPath + "tmp/*Meta.csv")
	RelationFilesPath = glob.glob(ResultPath + "tmp/*Relation.csv")
	GraphSVGPath = []
	
	for path in ["random_c{}_mf15_n{}_0step", "random_c{}_mf15_n{}_5000step", "random_c{}_mf15_n{}_10000step", "random_c{}_mf15_n{}_12000step", "random_c{}_mf15_n{}_14000step", "random_c{}_mf15_n{}_16000step", "random_c{}_mf15_n{}_18000step", "random_c{}_mf15_n{}_20000step"]:
		for i in range(10):
			GraphSVGPath.append(path.format(c, i))

	for MetaFilePath, RelationFilePath in tqdm(zip(MetaFilesPath, RelationFilesPath)):
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
		Graph.add_nodes_from([i for i in range(N)])
		#エッジの追加
		Graph.add_edges_from(GraphRelation)

		for i in range(N):
			Graph.nodes[i]["color"] = GraphMeta[i][1]
			Graph.nodes[i]["label"] = GraphMeta[i][2]
			Graph.nodes[i]["size"] = len(list(Graph.predecessors(i))) * 1.7 + 45
		

		FileName = os.path.splitext(os.path.basename(MetaFilePath))[0].strip("_Meta.csv")
		FilePath = "{}graph/{}".format(ResultPath, FileName)
		FileTmpPath = "{}tmp/{}".format(ResultPath, FileName)
		#nx.write_gexf(Graph, "{}graph/{}{}".format(ResultPath, FileName, ".gexf"))
		nx.write_graphml(Graph, FilePath + ".graphml")

		#cytoscapeでSVG出力
		if FileName in GraphSVGPath:
			p4c.networks.create_network_from_networkx(Graph)
			p4c.styles.set_visual_style("myStyle3")
			p4c.layouts.set_layout_properties("force-directed", {'defaultSpringCoefficient':0.00001})
			p4c.layouts.layout_network("force-directed")
			#p4c.network_views.fit_content()
			p4c.network_views.export_image(FileTmpPath, type="svg")
			#p4c.commands.command_sleep(1)
			p4c.networks.delete_network()
			change_suffix(FileTmpPath+".svg", '.svg', '.txt')

			with open(FileTmpPath+".txt") as f:
				a = f.read()

			with open(FilePath + ".svg", mode='w') as f:
				f.write(re.sub("<desc>.*</desc>\n", "", a))

			os.remove(FileTmpPath+".txt")
		



N = int(sys.argv[1])
c = float(sys.argv[2])
ResultPath = sys.argv[3]
print(p4c.cytoscape_ping())
export_graph(N,c, ResultPath)
#shutil.rmtree(ResultPath + "tmp/")