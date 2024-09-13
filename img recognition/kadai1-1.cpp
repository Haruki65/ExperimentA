#include "jikkenA.h"  /* 実験に用いる各種関数の定義 */

/*
* カメラ映像のウインドウ表示中に 'q' を押すとプログラムは終了する
*/

int main(void) {
    static unsigned char img[WIDTH][HEIGHT];  /* 画像を読み込むためのメモリ領域 */

    /* カメラ撮影による画像取り込み */
    captureOpen(0);                           /* カメラからの画像取り込みを初期化 */
    while (1) {                               /* 無限ループ */
        capture(img);                         /* カメラ撮影画像を img に格納 */
        /* 2値化処理 */
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (img[j][i] > 128) {
                    img[j][i] = 0;
                }
                else if (img[j][i] <= 128) {
                    img[j][i] = 255;
                }
            }
        }
        dispImage(img);                       /* img を別ウインドウで表示 */
    }

    /* 以降の処理は実行されない */
    return(0);
}
