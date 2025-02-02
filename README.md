# Personal-Products

本リポジトリには、これまで私が個人的に作成したプログラムが保存されています。  
以下に概要を記します。  

## rotation-tetris
2017.10.26  
ゲーム『回転テトリス』で遊べます。  
高専の学校祭の学科展示用に作成し、多くの来場者にプレイしていただき、大変な好評を博しました。  
	講義でC言語とOpenGL(2D)を用いてごく簡単なゲームを作成する実験があり、そこで配布されたテンプレート(ウィンドウを表示するだけ)を参考に『回転テトリス』を発案し一人で作成しました。 
	私自身が作成/編集した箇所は以下の通りです:  
	```
		main_simple.c  
		ActiveBlock_LotateDefine.h (落ちてくるブロックを回転させるためのプログラム)  
		ColorSamples.h のcolor_array[12][4]の定義  
		(VS2015で動作を確認)  
	```
ゲームのルールについてはwordファイルを参照してください。  
	(2017.10.26の物です)  

## merge-internet-shortcut  
2019.8.20  
ローカルに置かれた複数のインターネットショートカットを一つのhtmlファイルに統合できます。  
[(詳細)](https://github.com/apln-n/Personal-Products/tree/main/merge-internet-shortcut)


## AMT-page-transition-sample
2021.9.20  
Amazon Mechanical Turk (AMT)  上にて、ページ遷移を伴うクラウドソーシングを実施する際のサンプルプログラム。  
	修士1年の研究で作成したプログラムを元に作成。  
	(研究に関する機密が含まれないようにしています)  
使用する際は、AMTのRequester側のプログラムを書く欄(Design Layout)にそのままコピペしてください。  


## twitter-recommended-topic-remover
2022.2.8  
Twitterのタイムラインに表示されるおすすめトピックを削除します。  
	具体的には、指定されたユーザ(ブラウザでログイン済み)がフォローしている「おすすめトピック」を、PyAutoGUIを用いて一つずつ取り外すことで実現しています。  
実行方法:  
	source.exeを実行し、開かれた黒い画面でTwitter IDを指定。  
	ブラウザでTwitter IDに紐づいた「おすすめトピック」の設定画面が開かれ、自動的におすすめトピックが削除されていく。  
	黒い画面に「Please press Enter」と表示されるまで放置。  
制約:  
	Twitterをダークテーマに設定していたり、フォントサイズを変更しているとうまく動作しない可能性があります。
	動作しない場合は、arrow.png、footer.png等を自身の環境のものに置き換えてください。(スクショ→トリミング→リネーム)  

## マイうたDAMcsv化
[(詳細)](https://github.com/apln-n/Personal-Products/tree/main/%E3%83%9E%E3%82%A4%E3%81%86%E3%81%9FDAMcsv%E5%8C%96)

## ChieImages
[(詳細)](https://github.com/apln-n/Personal-Products/tree/main/ChieImages)