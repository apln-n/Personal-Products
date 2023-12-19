# マイうたDAMcsv化

以下の最大4つの**テキストファイルに曲を保存し**、  
本ディレクトリで**`source.py`を実行する**ことで、曲の重複を(~~ほぼ~~)除いて結合させた`result.csv`が出力されます。  
`result.csv`はWordPressのプラグイン「TablePress」にインポートできる形式になっており、  
`確認用.csv`は`result.csv`の文字コードをcp932(Shift_JIS)にした物です(雑)  
---

## `JoySound1.txt`
[JOYSOUNDのマイうた](https://utasuki.joysound.com/utasuki/mypage/mysong/)に保存されている曲。  
PCで↑を開き、全選択(ctrl+a)して`JoySound1.txt`に上書きで貼り付けて保存。  
このときブラウザである程度スクロールしておくと、全選択時の「漏れ」が無くて済む。曲数要確認。

## `JoySound2.txt`
JOYSOUNDのマイうたには無いが検索すると出てくる曲。  
あれば手動で、「曲名／歌手名」の形式で保存。  
[※サンプルURL](https://github.com/apln-n/Personal-Products/blob/main/%E3%83%9E%E3%82%A4%E3%81%86%E3%81%9FDAMcsv%E5%8C%96/JoySound2.txt)

## `DAM.txt`
[DAMのMYデンモク](https://www.clubdam.com/app/damtomo/mydenmoku/mylistListUp.do?listNo=1)に保存されている曲。  
PCで↑を開き、MYリスト1の画面で全選択して`DAM.txt`に上書きで貼り付ける。  
その後、MYリスト2、3、4と順番に全選択して、↑で貼り付けた物の後ろに追加する形で順次保存。  

## `Others.txt`
JOYSOUND、DAMのどちらの機種にも存在しない曲。  
`JoySound2.txt`と同様に、あれば手動で「曲名／歌手名」の形式で保存。  
[※サンプルURL](https://github.com/apln-n/Personal-Products/blob/main/%E3%83%9E%E3%82%A4%E3%81%86%E3%81%9FDAMcsv%E5%8C%96/Others.txt)
