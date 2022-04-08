//fin.
// Implements a dictionary's functionality

#include <stdbool.h>
#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 100;

// Hash table
// N個のノードポインタの配列である「table」を作成
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    //Hash indexを獲得
    int hash_index = hash(word);
    
    //獲得したHash indexへアクセス（table[hash_index]＝連結リストの先頭ノードを示すポインタ）
    //cursolの初期位置を、連結リストの先頭にする
    
    node *cursol = table[hash_index];
    
    //
    while (cursol != NULL)
    {
        //引数の言葉が連結リスト内の言葉と一致するならば(辞書内に探してる言葉あり)
        if (strcasecmp(cursol->word, word) == 0)
        {
            return true;    
        }
        
        //引数の言葉が連結リスト内の言葉と一致しないならば(辞書内に探してる言葉なし)
        else
        {
            cursol = cursol->next;    
        }
        
        
    }
    
    return false;
    
}

// Hashes word to a number（単語をインデックス(1~(N-1))に変換する）
//unsigned int hash(const string word)
//単語の各文字をASCIIに変換して全て足す。
//最後に戻り値のインデックスがHashテーブルのバケット数N以下の値になるようにNで割る
unsigned int hash(const char *word)
{
    // TODO
    long total = 0; 
    for (int i = 0; i < strlen(word); i++)
    {
        total += tolower(word[i]);
    }
        
    unsigned int index = total % N; 
    return index;
}

// Loads dictionary into memory, returning true if successful, else false
/*引数である辞書ファイルを受け取り、それを開き、hash()関数を使って
適切なインデックスで辞書中の各単語をハッシュテーブルに追加することである。*/
//fopenで辞書ファイルを開き、FILE変数に代入
/*
辞書に存在する、とある単語を取得する
その単語はノードの中に入れる。よって新しくノードを作る
①まず新しく作るノードのためにメモリを割り当てる
②その後、ノードの中に単語を入れる
③その後、１つ前のノードが新しく作ったノードを指すようにする（連結）
*/

//fscanf:ファイルから文字列を読み込んで変数に値を格納
//文字列の終わりはNULL, ファイルの終端はEOF

int words = 0; //辞書の中にある単語数(whileが繰り返されるたびに+1)

bool load(const char *dictionary)
{
    // TODO
    FILE *fp;
    fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        return false;
    }
    
    //NULL文字のために+1しておく
    //次のwhileループで辞書ファイルから読み取る単語達をこの配列に入れる
    char newword [LENGTH + 1]; 
    
    //ファイルから読み取った単語をnewwordという配列に%s形式で格納する！
    //第３引数以降はポインタ型でないとダメだが、配列はポインタだからok
    while (fscanf(fp, "%s", newword) != EOF)
    {
        //①まず新しく作るノードのためにメモリを割り当てる
        node *n = malloc(sizeof(node)); 
        if (n == NULL)
        {
            return false;
        }
    
        //②ノードの中に単語を入れる
        strcpy(n->word, newword);  //strcpyじゃないとダメかも
        
        // ??
        int hash_index = hash(newword);
        
        //③１つ前のノードが新しく作ったノードを指すようにする（連結）
        //新しいノードであるblueのノード.nextがbatのポインタを指すようにする（ケツ）
        n -> next = table[hash_index]; 
        
        //hashtableのheadが新しいノード(blue)のポインタを指すようにする
        table[hash_index] = n; 
        
        words++;
        
    }
    fclose(fp);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        
        node *cursol = table[i];
        
        
        while (cursol != NULL)
        {
            node *tmp = cursol;
            cursol = cursol->next;
            free(tmp);
        
        }
        
        if (i == N - 1 && cursol == NULL)
        {
            return true;
        }
    }
    
    
    return false;
}
