import sys
import csv

if len(sys.argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    

def main():
    # strsという空リストを作成し、全てのSTR配列を格納する
    # のちにfor str in strs　として、各strへのアクセスを繰り返したいからである
    # まず1行目を抽出
    # その後1行１列目("name")を削除し、
    # STRs = ['AGATC', 'TTTTTTCT', 'AATG', 'TCTAG', 'GATA', 'TATC', 'GAAA', 'TCTG ']を得た
    
    STRs = []
   
    with open(sys.argv[1]) as csvfile:
        reader = csv.reader(csvfile)
        header = next(reader) 
        STRs = header[1:]
        
        # match関数で使う
        
    # DNAsequenceファイルを開く
    # あるSTR配列を考える。
    # そのsequenceにこのSTR配列が最大何回連続しているか調べる
    # この操作を全てのstrについて繰り返す(for str in strs)
    
    with open(sys.argv[2]) as txtfile:
        sequence = txtfile.read()
        
        # {key:STR名}-{value:最大連続回数 ←str_max関数の戻り値}という辞書を作成
        dict_STR_MAX = {}
        
        # 辞書に要素を追加する方法
        # ①profile["gender"] = "male"
        # ②profile.update({"gender":"male"})
        
        for STR in STRs:
            dict_STR_MAX[STR] = str_max(sequence, STR)
        
        # print(dict_STR_MAX) 
        # {'AGATC': 5, 'TTTTTTCT': 0, 'AATG': 2, 'TCTAG': 0, 'GATA': 2, 'TATC': 6, 'GAAA': 2, 'TCTG ': 0}を得た
        # {key:STR名}-{value:最大連続回数 ←str_max関数の戻り値}という辞書の作成完了
        
        # 犯人を見つける
        # この辞書とcsvファイルに記載の人々のSTR回数が一致しているか調べる
        # row = 1(Albusの行のとき)、
        
        with open(sys.argv[1]) as csvfile:    
            dict_reader = csv.DictReader(csvfile)
        # for row in dict_reader:
        #     print(row)→出力
        #     {'name': 'Albus', 'AGATC': '15', 'TTTTTTCT': '49', 'AATG': '38', 'TCTAG': '5', 'GATA': '14', 'TATC': '44', 'GAAA': '14', 'TCTG ': '12'}
        #     {'name': 'Cedric', 'AGATC': '31', 'TTTTTTCT': '21', 'AATG': '41', 'TCTAG': '28', 'GATA': '30', 'TATC': '9', 'GAAA': '36', 'TCTG ': '44'}
        
        #     print(row["name"])→出力
        #     Albus
        #     Cedric
            for row in dict_reader:
                if match(STRs, row, dict_STR_MAX):
                    print(f"{row['name']}")
                    sys.exit()
                
            print("No match")
            

# sequenceの中に含まれるstr最大連続回数を与える関数
def str_max(sequence, STR):
    maxi = 0
    for i in range(len(sequence)):
        if STR * (i + 1) in sequence:
            i += 1
        else:
            break
        
    maxi = i
    return maxi

    # i = 0     
    # while STR*(i + 1) in sequence:
    #     i += 1
    # return i
    

def match(STRs, row, dict_STR_MAX):
    
    # 引数のrow が1(Albus)のとき
    # AlbusのAGATC:15が、sequence辞書の最大連続回数と一致していないなら即False
    # AGATCが一致していたならば、次の塩基TTTCTを調べて一致していないなら即False
    # これを繰り返すと、全て一致(match)のときのみTrueを返す
    
    # AlbusがFalseならばmain関数に戻り次のrow（人物）へ
    # AlbusがTrueならばmain関数にTrueを返して名前をprint
    
    # dict_STR_MAX['AGATC']: 5　←整数
    # row['AGATC']: '15'　←文字列！！　int型へ型変換が必要！！
    # print(STRs)
    # print(dict_STR_MAX)
    
    for STR in STRs:
        if int(row[STR]) != dict_STR_MAX[STR]:
            # print("False")
            # print(int(row[STR]))
            return False
            
    # print("True")
    return True
    

main()

        
# 1行1列目(=name)を消す
# 1列目を消す
# 辞書(key~AGAT- valueのセット)のnameというkeyを持つやつを消す
# DictReaderはkey-valueセット前提だから2行目からアクセス
# del STR[0]["name"]
# print(STR[0])
    
        
# STR配列を取り出す
# 1行めの2列目から最後まで取り出す
    
# STRs = column[2:]
    
# 7番目から終点までの要素を取り出す
# a[7:] #[7, 8, 9] （拡張子の取り出しなどよく使う）
    
# TXTopen
    
# txtfile = open(argv[2], 'r')
# txtdata = txtfile.read()
# txtfile.close()
    
# 『操作』
# sequenceの長さの間この操作を行う（for）
# sequenceのi~j番目の配列=STR配列ならば(True)
# 1.continue_count++,2.i(j)+= len(STR)して繰り返し連続していないか調べる
    
# sequenceのi~j番目の配列=STR配列でないならば(False)
# 1.i(j) +1 して（1つ隣に移動して）また上の『操作』を繰り返す