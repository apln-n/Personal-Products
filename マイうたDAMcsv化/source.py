import sys, csv, json

# マイうたに登録できる曲
def getJoySound1():
	with open ("JoySound1.txt", "r") as f:
		li = f.readlines()
		# マイうたページ全体をコピーしてくる
		size = -1
		for s in li:
			if("マイうた（" in s and "件）" in s):
				size = int( s[len("マイうた（"): -(len("件）")+1)] )
				print("JoySound1: {}".format(size))
				break
		if(size != -1):
			li = li[8:-12]
			li = [s.replace("\n", "") for s in li]
			tmp = []
			# 曲数n
			n = int(len(li)/8)
			for i in range(0,n):
				j = i*8
				tmp.append( [li[j+2], li[j+4] ] )
				# いま追加された曲
				# print(tmp[-1])
			if(len(tmp) == size):
				return tmp
			else:
				print("error: マイうたのコピーが正しく行われていません")
				sys.exit(-1)
		else:
			print("error: マイうたの曲数が取得できませんでした")
			sys.exit(-1)

# マイうたに登録できない曲
# JOYSOUNDで1曲ずつ直接調べて持ってくる
def getJoySound2():
	with open ("JoySound2.txt", "r") as f:
		li = f.readlines()
		#改行のみの要素を削除
		li = [s for s in li if s != "\n" ]
		#要素中の改行(文字)、余計な文字列を削除
		li = [s.replace("\n", "") for s in li]
		print("JoySound2: {}".format(len(li)))
		# 各要素の文字列を"／"で分解して曲ごとにリストにする
		li = [ [*( li[i].split("／") )] for i,s in enumerate(li) ]
		return li

# DAMとものマイリストを1つずつ、ページ全体をコピーして4つ貼り付ける
def getDAM():
	with open ("DAM.txt", "r") as f:
		li = f.readlines()
		#要素中の改行(文字)、余計な文字列を削除
		li = [s.replace("\n", "") for s in li]
		data = []
		for i in range(0,len(li)):
			if(li[i] == "…"):
				tmp = [li[i-3], li[i-2]]
				if(tmp not in data):
					data.append( tmp )
		print("DAM: {}".format(len(data)))
		return data

# [曲名, 歌手名]
# [JoySound1, JoySound2, Dam]
def getSongLists():
	return [getJoySound1(), getJoySound2(), getDAM()]


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
	for lists in songLists:
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
				print(li)
				print(data[-1][-1])
				continue
	return data


# li: [songname, songartist] 
# ※重要
def isEqualSong(li1, li2):
	if(len(li1) <= 2):
		flag = True
		for i in range(0,len(li1)):
			# len(s1) <= len(s2)
			[s1, s2] = [li1[i],li2[i]] if (len(li1[i])<=len(li2[i])) else [li2[i],li1[i]]
			#リストにする
			s1 = list(s1.replace(" ","").replace("，","").replace(",","").lower())
			s2 = list(s2.replace(" ","").replace("，","").replace(",","").lower())
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
		print("error: リストに余計な情報が入っています")
		sys.exit(-1)

# songlists: [JoySound1(2次元リスト), JoySound2, DAM]
# con: [曲名, 歌手名, JoySound(シンボル), DAM]
# songlistsは3次元リスト
def merge(songLists, symbols):
	if(len(songLists) > 0):
		# 2次元リストとして連結
		con = []
		for i,lists in enumerate(songLists):
			for j,li in enumerate(lists):
				con.append( [*li[:2], "-", "-"] )
				if(i in [0,1]):
					con[-1][2] = symbols[i]
				else:
					con[-1][3] = symbols[i]
		print("Concatenated: {}".format(len(con)))
		# i,j: 曲の行
		merged = []
		while(len(con)>0):
			# con[0]を軸に比較する(重要)
			merged.append( con[0] )
			for i in range(1,len(con)):
				# 同じ曲だった場合
				if( isEqualSong(merged[-1][:2], con[i][:2]) ):
					#マイナス1してる
					for j in range(0,len(symbols)-1):
						if(con[i][j+2]!="-" and merged[-1][j+2]=="-"):
							merged[-1][j+2] = con[i][j+2]
							con.pop(i)
							break
					#print(merged[-1])
					break
				else:
					continue
			# con[0]は毎回appendされるのでpopしておく、実質の「i--」と似た働き
			con.pop(0)
		print("Merged: {}".format(len(merged)))
		#id付加
		merged = [[(len(merged)-i), *row] for i,row in enumerate(merged)]
		return merged
	else:
		print("error: 曲が無いです")
		sys.exit(-1)


# csv書き込み
def writeSongs(mergedLists):
	with open("result.csv", "w", newline="") as f:
		writer = csv.writer(f)
		writer.writerow( ["id", "曲名", "歌手名", "JoySound", "DAM"] )
		writer.writerows(mergedLists)
	# encodingはデバッグ用
	with open("確認用.csv", "w", newline="", encoding="cp932") as f:
		writer = csv.writer(f)
		writer.writerow( ["id", "曲名", "歌手名", "JoySound", "DAM"] )
		writer.writerows(mergedLists)


def checkSongLists(songLists):
	pflag = True
	name = ["JoySound1", "JoySound2", "DAM"]
	print("Check song lists...")
	if(len(songLists) == len(name)):
		if(pflag):
			for i in range(0,len(name)):
				print("----------------------------------- {} -----------------------------------".format(name[i]))
				# サンプル
				for li in songLists[i][:10]:
					print(li)
				if(len(songLists[i]) > 10):
					print("...")
		return
	else:
		print("error: 適切なデータを取得できませんでした")
		sys.exit(-1)


#2次元リスト
def checkMergedLists(mergedLists):
	pflag = True
	colNames = ["id", "曲名", "歌手名", "JoySound", "DAM"]
	print("Check merged list...")
	#idはマージする時に付加した
	if(len(mergedLists[0])  == len(colNames)):
		for li in mergedLists:
			flag = False
			for symbol in li[2:]:
				if(symbol != "-"):
					flag = True
					break
			if(not flag):
				print("error: 以下の曲の情報を確認してください")
				print(li)
				sys.exit(-1)
		if(pflag):
			print(", ".join(colNames))
			# サンプル
			for li in mergedLists[:10]:
				print(li)
			if(len(mergedLists) > 10):
				print("...")
		return
	else:
		print("error: 適切にデータがマージされていません")
		sys.exit(-1)

def func():
	#3次元リスト
	songLists = getSongLists()
	songLists = cleaning(songLists)
	checkSongLists(songLists)
	symbols = ["○", "○?", "○"]
	#2次元リスト
	mergedLists = merge(songLists, symbols)
	checkMergedLists(mergedLists)
	writeSongs(mergedLists)
	print("Completed.")
func()