#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

/*
~fread関数~
#include <stdio.h>
size_t fread(void *buf, size_t size, size_t n, FILE *fp);

第一引数には格納先のバッファ
第二引数は読み込むデータ１つのバイト数
第三引数は読み込むデータの個数
第四引数はファイルポインタ
*/

 
int main(int argc, char *argv[])
{   
    //プログラムが正確に1つのコマンドライン引数で実行されていない場合
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    
    //Open ファイル
    //argv[1]のファイルを読み取り専用で開く。FILE構造体へのポインタfile*を取得する。
    FILE *file = fopen(argv[1], "r"); 
    
    
    //フォレンジックイメージを開いて読み取ることができない場合
    if (file == NULL)
    {
        printf("Usage: ./recover image\n");
        return 2;
    }
    
    

    // Read first three bytes
    //BYTE bytes[4];
    //fread(bytes, sizeof(BYTE), 4, file);
    
    
    
    BYTE buffer [512]; //512バイトのバッファを用意。このバッファにfileのデータをストリームしていく。
    
    //fileの名前(013.jpg)を保存するための領域を確保。領域先頭アドレスをfilenameとする
    char *filename = malloc(8 * sizeof(char)); 
    
    int count = 0; //いま何個めのbufferかを数えて、番号をつけるための変数
    FILE *output_file = NULL; //output_fileの先頭構造体のアドレスをNULLにする
    

    // Check first four bytes and check whether a file is JPEG. 
    //JPEGの署名があれば、名付ける・outputファイルを開く・outputファイルにコピー。別の署名が見つかったら古いファイルは閉じる
    
    //512byteを確認してbufferに格納する。メモリカードの最後まで行ったら終了
    while (fread(&buffer, 512, 1, file) == 1) 
    {
        //もしJPEGならば名付けて・outputfileを // (buffer[3] >= 0xe0 && buffer[3] <= 0xef
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (!(count == 0))
            {
                fclose(output_file);
            }

            //名付ける
            //「013.jpg」という名前の羅列(013.jpg自体とは別の場所)がfilenameというアドレス領域へ書き込まれる
            sprintf(filename, "%03i.jpg", count); 
            
            //filenameとは"013.jpg"のこと。この画像ファイルの情報が格納された構造体へのアドレスがoutput_fileに格納される
            output_file = fopen(filename, "w"); 
            count++;
        }
        
        //bufferにoutputする
        if (output_file != NULL)
        {
            fwrite(&buffer, 1, 512, output_file);
        }
    }

    free(filename);
    // Close file
    fclose(file);
    fclose(output_file);
    
    return 0;

}