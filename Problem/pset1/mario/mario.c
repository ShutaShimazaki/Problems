//ヘッダー
#include <cs50.h>
#include <stdio.h>

//プロトタイプ宣言
int Height_int(void) ;

int main(void)
{   
    int Height = Height_int() ;
    for (int i = 0; i < Height; i++)
  {
      for (int k = 0; k < Height - i- 1; k++)
      {
        printf(" ");
      }
      
      
      for (int j = 0; j < i + 1; j++)
      {   
        printf("#");
      }
      
      printf("\n");
  }

}


int Height_int(void) //高さの入力
{
  int n;
  do
   {
    n = get_int("Height number") ;
   }
  while (n < 1 || n > 8) ;
  return n;
}
