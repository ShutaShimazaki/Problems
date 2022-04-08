//ヘッダー
#include <cs50.h>
#include <stdio.h>
#include <math.h>


//プロトタイプ宣言
float Change_float(void) ;


int main(void)
{   
    float dollars = Change_float();    //関数を呼び出してfloat型のdollarsへ格納
    int cents = round(dollars * 100); //float型のdollarsをint型にし、単位もセントにする
    int coins = 0;                     //求めるべきコイン枚数をcoinsとして宣言。初期値は０
    
    //以下枚数を数える
    if(cents > 0)
  {
    while(cents >= 25)
    {
        cents = cents - 25 ;
        coins++ ;
    } 
    
     while(cents >= 10)
     {
        cents = cents - 10 ;
        coins++ ;
    } 
    while(cents >= 5)
    {
        cents = cents - 5 ;
        coins++ ;
    } 
    
    while(cents >= 1)
    {
        cents = cents - 1 ;
        coins++ ;
    } 
     printf("%d\n", coins) ;
  }
  else
  {
   printf("0\n") ;
  }
  
}





float Change_float(void) //両替額の入力
{
   float n;
  do
   {
    n = get_float("Change owed (dollars)") ;
   }
  while (n < 0) ;
  return n;
}

