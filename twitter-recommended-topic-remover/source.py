"""
    PyCharmでプログラムを.exe化する方法
        https://coconala.com/blogs/1605628/102671
            pyinstaller source.py --onefile
        作った.exeは画像ファイルがあるディレクトリに置く必要あり
    既定のブラウザでWebページを開く
        https://tonari-it.com/python-webbrowser-open/#toc3
    画像のバイナリデータを画像化
        https://qiita.com/wasnot/items/be649f289073fb96513b
        https://techacademy.jp/magazine/30610
    PyAutoGUI
        https://rightcode.co.jp/blog/information-technology/python-automate-human-operations-with-pyautogui-image-recognition#PyAutoGui-2
        https://teratail.com/questions/266843
        おすすめが全て削除されるまで自動でWebページをリロードして処理を行う
"""

import sys
import pyautogui as pgui
import webbrowser
import time

def deleteRecoms(url,total):
    count = 0
    arrow = True

    if(pgui.locateCenterOnScreen("topic.png", confidence=0.90) is None):
        print("Failed to open {}.".format(url))
        return 0
    for i in range(0,100):
        if pgui.locateCenterOnScreen("recommend.png", confidence=0.9) is None:
            if pgui.locateCenterOnScreen("footer.png", confidence=0.9) is None:
                position = pgui.locateCenterOnScreen("topic.png", confidence=0.9)
                pgui.moveTo(position)
                pgui.scroll(-400)
                #print(i)
                time.sleep(0.15)
            else:
                #print("There are no topics.")
                return 0
        else:
            break

    if pgui.locateCenterOnScreen("recommend.png", confidence=0.9) is not None:
        x1, y1 = pgui.locateCenterOnScreen("topic.png", confidence=0.9)
        _, y2 = pgui.locateCenterOnScreen("recommend.png", confidence=0.9)
        diff = int(y2 - y1)
        #print("{} - {} = {}".format(y1,y2,diff))
        if(diff > 0):
            pgui.moveTo(x1, y1)
            pgui.scroll(-diff+200)
            time.sleep(1.0)
    else:
        #print("Failed to search Recommended Topic.")
        return 0

    for i in range(0,200):
        position = pgui.locateCenterOnScreen("img.png", confidence=0.85)
        if position is not None:
            pgui.click(position)
            arrow = True
            time.sleep(0.2)
            count += 1
            print("{:>3}: {} is clicked".format(total+count, position))
        else:
            #print("position is {}".format(position))
            if(arrow == True):
                position = pgui.locateCenterOnScreen("arrow.png", confidence=0.95)
                if position is not None:
                    #print("arrow".format(position))
                    pgui.click(position)
                    time.sleep(1.0)
                arrow = False
            else:
                break
    return count

userID = input("Twitter ID >> ")
# Twitter Recommended Topic URL
url = "https://twitter.com/"+userID+"/topics"
webbrowser.open(url)

total = 0
for i in range(0,10):
    time.sleep(5.0)
    tmp = deleteRecoms(url,total)
    if(tmp != 0):
        total += tmp
        pgui.hotkey('f5')
        print("Reloading now...")
    else:
        break

print("Total {} recommended topics are deleted.".format(total))
input("Please press Enter...")
sys.exit()
