import sys, csv, re

def __error(s):
	print(f"error: {s}")
	sys.exit(-1)
	
# マイうたに登録できる曲
def getJoySound1():
	with open ("JoySound1.txt", "r") as f:
		li = f.readlines()
		if(len(li)==0):
			print("JoySound1: 0")
			return []
		else:
			# マイうたページ全体をコピーしてくる
			try:
				s = [s for s in li if ("マイうた（" in s and "件）" in s)][0]
				size = int( s[len("マイうた（"): -(len("件）")+1)] )
				print(f"JoySound1: {size}")
				indexes = [i for i,x in enumerate(li) if "±0" in x]
				tmp =  [[li[index-5].replace("\n",""),li[index-3].replace("\n","")] for index in indexes]
				if(len(tmp) == size):
					return tmp
				else:
					__error("マイうたのコピーが正しく行われていません")
			except:
				__error("マイうたの曲数が取得できませんでした")

				
# getJoySound2(), Others()の本体
def getManuallyAdds(name):
	with open (f"{name}.txt", "r") as f:
		li = f.readlines()
		if(len(li)==0):
			print(f"{name}: 0")
			return []
		else:
			li = [s.replace("\n", "") for s in li if "／" in s]
			print(f"{name}: {len(li)}")
			# 各要素の文字列を"／"(スラッシュ)で分解して曲ごとにリストにする
			tmp = [re.split("[／]", s)[:2] for s in li]
			return tmp


# DAMとものマイリストを1つずつ、ページ全体をコピーして4つ貼り付ける
def getDAM1():
	with open ("DAM1.txt", "r") as f:
		li = f.readlines()
		if(len(li)==0):
			print("DAM1: 0")
			return []
		else:
			#要素中の改行(文字)、余計な文字列を削除
			li = [s.replace("\n", "") for s in li]
			indexes = [i for i,x in enumerate(li) if "…" in x]
			tmp =  [[li[index-3].replace("\n",""),li[index-2].replace("\n","")] for index in indexes]
			print(f"DAM1: {len(tmp)}")
			return tmp

# JOYSOUND2, DAM2, Others
# マイうたやうたスキに登録できない曲
# 1曲ずつ直接持ってくる
def getJoySound2():
	return getManuallyAdds("JoySound2")
def getDAM2():
	return getManuallyAdds("DAM2")
def getOthers():
	return getManuallyAdds("Others")

# [曲名, 歌手名]
# [JoySound1, JoySound2, Dam1, Dam2, Others]
def getSongLists():
	return [getJoySound1(), getJoySound2(), getDAM1(), getDAM2(), getOthers()]


def cleaning(songLists):
	brackets = [
		{"left":"《", "right":"》"},
		{"left":"(", "right":")"},
		{"left":"（", "right":"）"},
		{"left":"[", "right":"]"},
		#{"left":"〈", "right":"〉"},	
		#{"left":"-", "right":"-"},
		#{"left":"～", "right":"～"},
		]
	data = []
	# Othersはそのまま素通りさせる
	for lists in songLists[:-1]:
		data.append([])
		for li in lists:
			data[-1].append([])
			#曲名と歌手名
			#同じ名前の曲が無いなら歌手名を考慮しない方が良いかもしれない
			for s in li[:1]:
				i = 0
				while(i<len(brackets)):
					if(brackets[i]["left"] in s and brackets[i]["right"] in s):
						indexLeft = s.index(brackets[i]["left"])
						if indexLeft != 0:
							for j in range(indexLeft+1, len(s)):
								if(s[j] == brackets[i]["right"]):
									indexRight = j
									s = s[:indexLeft] + s[indexRight+1:]
									i = 0
									break
					i += 1
				if(s[-1] == " "):
					s = s[:-1]
				data[-1][-1].append(s)
				data[-1][-1].append(li[1])
			if(li[0] != s):
				#print(li)
				#print(data[-1][-1])
				continue
	# Others
	data.append([])
	for li in songLists[-1]:
		data[-1].append(li)
	return data


# li: [songname, songartist] 
# ※重要
def isEqualSong(li1, li2):
	if(len(li1) <= 2):
		flag = True
		#同じ名前でなくとも短い曲名が長い曲名にアナグラムとして含まれることがあるので、歌手名での比較もすると良い
		for i in range(0,len(li1)):
			# len(s1) <= len(s2)
			[s1, s2] = [li1[i],li2[i]] if (len(li1[i])<=len(li2[i])) else [li2[i],li1[i]]
			#リストにする
			s1 = list(s1.replace(" ","").replace("，","").replace(",","").lower())
			s2 = list(s2.replace(" ","").replace("，","").replace(",","").lower())
			if(i==1 and "(" in s1):
				n = s1.index("(")
				s1 = s1[:n]
			while(len(s1)>0):
				if(s1[0] in s2):
					index = s2.index(s1[0])
					s1.pop(0)
					s2.pop(index)
				else:
					flag = False
					break
			if(flag):
				continue
		if(flag):
			#同じ曲の場合
			#曲名、歌手名の差分の確認用
			#print(li1)
			#print(li2)
			return True
		else:
			#異なる曲の場合
			return False
	else:
		__error("リストに余計な情報が入っています")
		
# songlists: [JoySound1(2次元リスト), JoySound2, DAM1, DAM2]
# con: [ [曲名, 歌手名, JoySound(シンボル), DAM(シンボル)] ]
# songlistsは3次元リスト
def merge(songLists, symbols):
	if(len(songLists) > 0):
		# 2次元リストとして連結
		con = []
		for i,lists in enumerate(songLists):
			for li in lists:
				con.append( [*li[:2], "-", "-", "-"] )
				if(i == 0 or i == 1):
					con[-1][2] = symbols[i]
				elif(i==2 or i== 3):
					con[-1][3] = symbols[i]
				elif(i==4):
					con[-1][4] = symbols[i]
				else:
					__error("以下は、JoySound1, 2, DAM1, 2, Othersのどれにも保存されていない曲です")
					print(li[:2])
					print(f"Concatenated: {len(con)}")

		# i,j: 曲の行
		merged = []
		while(len(con)>0):
			# con[0]を軸とする(重要)
			# Othersの曲でない場合は比較
			if(con[0][-1] != symbols[-1]):
				#con[i=1]から見る
				for i in range(1,len(con)):
					# 同じ曲だった場合
					if( isEqualSong(con[0][:2], con[i][:2]) ):
						#JoySound, DAMの2つ
						#JoySound2にある曲がJoySound1にもあるなら、「○?」は要らない
						#DAMも同様
						for j in range(0,2):
							if(con[i][j+2]!="-" and con[0][j+2]=="-"):
								con[0][j+2] = con[i][j+2]
							con.pop(i)
						#print(con[0])
						break
					else:
						continue
			# con[0]は毎回appendされるのでpopしておく、実質の「i--」と似た働き
			merged.append( con[0] )
			con.pop(0)
		print(f"Merged: {len(merged)}")
		#id付加
		merged = [[(len(merged)-i), *row] for i,row in enumerate(merged)]
		return merged
	else:
		__error("曲が無いです")
		

# csv書き込み
def writeSongs(mergedLists, colNames):
	print("Write songs to CSV file...")
	with open("result.csv", "w", newline="") as f:
		writer = csv.writer(f)
		writer.writerow( colNames )
		writer.writerows(mergedLists)
	# encodingはデバッグ用
	"""
	with open("確認用.csv", "w", newline="", encoding="cp932") as f:
		writer = csv.writer(f)
		writer.writerow( colNames )
		writer.writerows(mergedLists)
	"""


def checkSongLists(songLists):
	names = ["JoySound1", "JoySound2", "DAM1", "DAM2", "Others"]
	print("Check song lists...")
	if(len(songLists) == len(names)):
		for i,name in enumerate(names):
			print(f"----------------------------------- {name} -----------------------------------")
			# サンプル
			for li in songLists[i][:5]:
				print(li)
			if(len(songLists[i]) > 10):
				print("...")
		return
	else:
		__error("適切なデータを取得できませんでした")
		

#2次元リスト
def checkMergedLists(mergedLists, colNames):
	print("Check merged list...")
	#idはマージする時に付加した
	if(len(mergedLists[0])  == len(colNames)):
		for li in mergedLists:
			if(len(li[2:]) == len([s for s in li[2:] if s=="-"])):
				__error(f"以下の曲の情報を確認してください\n{li}")
		# デバッグ用
		"""
		print(", ".join(colNames))
		# サンプル
		for li in mergedLists[:5]:
			print(li)
		if(len(mergedLists) > 5):
			print("...")
		"""
		return
	else:
		__error("適切にデータがマージされていません")
		
def func():
	#3次元リスト
	songLists = getSongLists()
	songLists = cleaning(songLists)
	checkSongLists(songLists)
	symbols = ["J.", "J.?", "D.", "D.?", "O."]
	#2次元リスト
	mergedLists = merge(songLists, symbols)
	colNames = ["id", "曲名", "歌手名", "JoySound", "DAM", "Others"]
	checkMergedLists(mergedLists, colNames)
	'''
		Othersにある曲と、それ以外の曲を比較して「重複かも？」リストをプロンプトに表示するのも良さそう
	'''
	writeSongs(mergedLists, colNames)
	print("Completed.")
func()