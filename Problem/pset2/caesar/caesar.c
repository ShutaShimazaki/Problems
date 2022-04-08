#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


bool valid_key(string s);

int main(int argc, string argv[])
{
    if (argc != 2 || valid_key(argv[1]) == 0)
    {
        printf("Usage: ./caesar key\n");
        return 1;
        
    }
    
    int key = atoi(argv[1]);
    
    //userから平文の入力
    string plaintext = get_string("plaintext: ");
    printf("ciphertext:");
    
    for (int i = 0; i < strlen(plaintext); i++)
    {
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z') //大文字なら
        {
            int pi = plaintext[i] - 65; //'A'=65 A=0にする
            int Ci = ((pi + key) % 26) + 65;
            //int Ci = ((plaintext[i] + key) % 26) + 65;
            printf("%c", Ci);
        }
        
        if (plaintext[i] >= 'a' && plaintext[i] <= 'z') //小文字なら
        {
            int pi = plaintext[i] - 97;  //'a'=97　a=0にする
            int Ci = ((pi + key) % 26) + 97;
            printf("%c", Ci);
        }
        
        
        if (isalpha(plaintext[i]) == 0) //大文字や小文字ではないなら（！など）
        {
            printf("%c", plaintext[i]);
        }
        
    }
    //入力された文字をずらす。
    //1.平文の各文字をASCIIに変換
    //2.平文ASCII+ key　→暗号文ASCIIに変換
    //3.暗号文ASCIIを各文字に変換
    
    
    
    printf("\n");  
    
    
    
    return 0;
    
}

bool valid_key(string s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        //char c = s[i];
        if (isdigit(s[i]) == 0)
        {
            return 0;
        }
    }
    
    return 1;
}



/*大文字小文字の区別してないからダメ。k>26以上になると区別しないと大文字→小文字になってしまう。
    
    for (int i = 0; i < strlen(plaintext); i++)
    {
        
        int Ascii_Pi = (int)plaintext[i]; //1
        int Ascii_Ci = Ascii_Pi + key; //2
       
        int Ci = (plaintext[i] + key) % 26;
         
        printf("%c%i",plaintext[i] ,plaintext[i]);
    }
    */