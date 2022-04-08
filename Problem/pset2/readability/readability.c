#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//prototype
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
void formula(int letters, int words, int sentences);

//①ユーザ入力の取得
int main(void)
{
    string text = get_string("Text: ");
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);
    formula(letters, words, sentences);
    //printf("letters= %i, words= %i, sentences= %i\n", letters, words, sentences);
}


//②文字

int count_letters(string text)
{
    int letters = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isupper(text[i]) || islower(text[i]))
        {
            letters ++;
        }

    }
    
    return letters;
}


//③単語
int count_words(string text)
{
    int space = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isspace(text[i]))
        {
            space ++;
        }

    }
    int words = space + 1;
    
    
    return words;
}

//④文章
int count_sentences(string text)
{
    int sentences = 0;
    
    for (int i = 0; text[i] != '\0'; i++) //text[i]はchar型  \０＝NULL文字
    {
        if ((int)text[i] == (int)'.' || (int)text[i] == (int)'!' || (int)text[i] == (int)'?')
        {
            sentences ++;
        }
    }
    
   
    return sentences;
}

//⑤公式
void formula(int letters, int words, int sentences)
{
    float L = (100 * letters / (float)words);
    float S = (100 * sentences / (float)words);
    //int L = (int)(round((float)(letters/words)*100));
    //int S = round((float)(100 * sentences / words));
    //int S = (int)(round((float)(sentences/words)*100));
    /*float L_pre = (float)(letters/words);
    int L = (int)r(L_pre*100);
    float S_pre = (float)(sentences/words);
    int S = (int)round(S_pre*100);*/
    
    int index = (int)round(0.0588 * L - 0.296 * S - 15.8);
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }    
}
