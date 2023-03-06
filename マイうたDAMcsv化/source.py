import csv, pykakasi, requests, json

# gooラボひらがな化APIを用いる場合
appid = "153ad61328b5976d85422ad7e1ba421274f096a8d3ac4a17e0043b0f3fdbefcc"

def convertKana(s):
	headers = {
  	  'Content-Type': 'application/json'
	}
	data = {
		"app_id": appid,
		"sentence": s,
		"output_type": "hiragana"
	}
	response = requests.post('https://labs.goo.ne.jp/api/hiragana', headers=headers, data=json.dumps(data))
	response = response.json()
	#print(response)
	return response["converted"]


# マイうたに登録できる曲
def getSongs1():
	with open ("songs1.txt", "r") as ft:
		li = ft.readlines()
		# マイうたページ全体をコピーしてくる
		li = li[8:-12]
		li = [s.replace("\n", "") for s in li]
		tmp = []
		# 曲数n
		n = int(len(li)/8)
		print("songs = {}".format(n))
		for i in range(0,n):
			j = i*8
			tmp.append( [(n-i), li[j+2], li[j+4] ] )
			# いま追加された曲
			# print(tmp[-1])
		return tmp
		# return tmp.reverse()

# マイうたに登録できない曲
# JOYSOUNDで1曲ずつ直接調べて持ってくる
def getSongs2():
	with open ("songs2.txt", "r") as ft:
		li = ft.readlines()
		#改行のみの要素を削除
		li = [s for s in li if s != "\n" ]
		#要素中の改行(文字)、余計な文字列を削除
		li = [s.replace("\n", "") for s in li]
		n = len(li)
		print("songs = {}".format(n))
		# 各要素の文字列を"／"で分解し、それらをまとめたリストの先頭にidを追加、これを曲ごとにリストにする
		li = [ [(n-i), *( li[i].split("／") )] for i,s in enumerate(li) ]
		return li

# csv書き込み
def writeSongs(songlists,n):
	# encoding="cp932" はインポートする時不要。
	# with open("result"+str(n+1)+".csv", "w", newline="", encoding="cp932") as fc:
	with open("result"+str(n+1)+".csv", "w", newline="") as fc:
		writer = csv.writer(fc)
		writer.writerow( ["id", "曲名", "歌手名"] )
		writer.writerows(songlists)

def func():
	for n in [0,1]:
		print("----------------------------------- song {} -----------------------------------".format(n+1))
		getSongs = [getSongs1, getSongs2][n]
		songlists = getSongs()
		# サンプル
		for li in songlists[:10]:
			print(li)
		if(len(songlists) > 10):
			print("...")
		writeSongs(songlists, n)
	print("completed.")

func()