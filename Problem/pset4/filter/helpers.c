#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
//①RGBの平均=ave として、②RGBの値をaveで統一する。③ループを用いて全２次元へ拡張
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float ave_pre = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;
            int ave = round(ave_pre);

            image[i][j].rgbtBlue = ave ;
            image[i][j].rgbtGreen = ave ;
            image[i][j].rgbtRed = ave ;
        }
    }


    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sepiaRed = 0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue;
            float sepiaGreen = 0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue;
            float sepiaBlue = 0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue;

            image[i][j].rgbtBlue = round(sepiaBlue) ;
            image[i][j].rgbtGreen = round(sepiaGreen) ;
            image[i][j].rgbtRed = round(sepiaRed) ;


            if (sepiaBlue > 255.0)
            {
                image[i][j].rgbtBlue = 255;
            }
            if (sepiaGreen > 255.0)
            {
                image[i][j].rgbtGreen = 255;
            }
            if (sepiaRed > 255.0)
            {
                image[i][j].rgbtRed = 255;
            }
        }
    }
    return;
}

// Reflect image horizontally

//引数に配列型＝ポインタ型とみなされる　引数：RGBTRIPLE* image
void reflect(int height, int width, RGBTRIPLE image[height][width])

{
    if (width % 2 == 0) //１列が偶数ならば
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < (width / 2); j++)
            {
                int k = width - j - 1;
                RGBTRIPLE tmp = image[i][j]; //image[i][j]はポインタ型なのにRGBTRIPLE型に入れてOKなの　どちらもRGBTRIPLE
                image[i][j] = image[i][k];
                image[i][k] = tmp;

                /*
                RGBTRIPLE tmp = *image[i][j];  アドレス”image[i][j]”にあるものをtmpに格納する
                image[i][j] = *image[i][k];
                image[i][k] = tmp;
                */

            }
        }
    }

    if (width % 2 == 1) //１列が奇数ならば
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < (width / 2.0) - 1 ; j++)
            {
                int k = width - j - 1;
                RGBTRIPLE tmp = image[i][j];
                image[i][j] = image[i][k];
                image[i][k] = tmp;

            }
        }
    }

    return;
}

// Blur image
//image[height][width]の[0][0]を決める方法
//1. image[0][0](row=0,col=0)を中心に縦(tate)3つ、横(yoko)３つの計９個(cur_row=-1,0,1 cur_col=-1,0,1)のピクセルを調べる。（ピクセルをRGBTRIPLE型で定義）
//2.　9つの中の１つのピクセルがcur_row,cur_col>=0ならばその数をcount++し、そのピクセル.rgbtred,green,blueをTotal R,g,bにまとめて、9つのピクセルをループで最後まで調べる
//3. temp[][]にtotalの情報を格納して、Total R/countして色値を決める。
//4. temp[][]をimage[][]に移す。

//imege[height][width]を１つずつ調べていく。


void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //image[i][j]の色値決定のために、周りのピクセルの色値情報を足していく。
            int count = 0;
            int total_Blue = 0;
            int total_Green = 0;
            int total_Red = 0;

            for (int col = -1; col < 2; col++) //縦3マス
            {


                for (int row = -1; row < 2; row++) //横3マスの計9マスをチェック
                {
                    int cur_col = i + col;
                    int cur_row = j + row;
                    //cur_row=widthの時、[cur_row]はwidth+1になるから。
                    if (cur_col >= 0 && cur_row >= 0 && cur_col < height && cur_row < width)
                    {

                        RGBTRIPLE pixel = image[cur_col][cur_row];

                        total_Blue += image[cur_col][cur_row].rgbtBlue;
                        total_Green += image[cur_col][cur_row].rgbtGreen;
                        total_Red += image[cur_col][cur_row].rgbtRed;

                        count++;
                    }

                }

            }

            //元の画像の色値を変更すると、次のマスの色値を決めるときに影響するので,出来上がったこのマスの情報はtempに保存。

            temp[i][j].rgbtBlue = round((float) total_Blue / count) ;
            temp[i][j].rgbtGreen = round((float) total_Green / count) ;
            temp[i][j].rgbtRed = round((float) total_Red / count) ;

        }
    }

    //tempの情報をimageへ移す。

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }

    return;
}
