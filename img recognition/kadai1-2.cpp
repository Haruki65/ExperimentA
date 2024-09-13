#include "jikkenA.h"  /* 実験に用いる各種関数の定義 */
#include "math.h"
#define PI 3.141592653589793

/*
* サンプルプログラム
*
* カメラ撮影による画像取り込みと文字列描画のサンプル
*
* 正しく動作すれば、カメラからの撮影画像（動画）に、文字列"Sample"を重畳描画した画像が表示される
*
* カメラ映像のウインドウ表示中に 'q' を押すとプログラムは終了する
*/



void binarization(static unsigned char img[WIDTH][HEIGHT]) {
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
}

int area(static unsigned char f[WIDTH][HEIGHT]) {
    int S = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (f[j][i] == 1) {
                S++;
            }
        }
    }
    return S;
}

void convolution(int m[3][3], static unsigned char inputimg[WIDTH][HEIGHT], double output[WIDTH][HEIGHT]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int sum = 0;
            for (int j = 0; j < 3; j++) {
                for (int i = 0; i < 3; i++) {
                    sum += m[i][j] * inputimg[x + i][y + j];
                }
            }
            output[x][y] = sum;
        }
    }
}

void sobel_operator(int mx[3][3], int my[3][3], static unsigned char img[WIDTH][HEIGHT], static unsigned char Le[WIDTH][HEIGHT]) {
    static double Lex[WIDTH][HEIGHT];
    static double Ley[WIDTH][HEIGHT];
    convolution(mx, img, Lex);
    convolution(my, img, Ley);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            double tmp = sqrt(Lex[x][y] * Lex[x][y] + Ley[x][y] * Ley[x][y]);
            if (tmp > 255) {
                Le[x][y] = 255;
            }
            else if (Le[x][y] <= 255) {
                Le[x][y] = 0;
            }
        }
    }
}

double perimeter(static unsigned char Le[WIDTH][HEIGHT], int xg, int yg) {
    double coordinates[360] = {0};
    double length = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (Le[x][y] == 255) {
                int degree = 0;
                
                if ((x - xg) < 0 && (y - yg) == 0) {
                    degree = 180;
                } 
                else {
                    degree = (int)(atan2(y - yg, x - xg) * (180 / PI));
                }
                if (degree < 0) {
                    degree += 360;
                }
                if (sqrt((x - xg) * (x - xg) + (y - yg) * (y - yg)) > coordinates[degree]) {
                    double r = sqrt((x - xg) * (x - xg) + (y - yg) * (y - yg));
                    coordinates[degree] = r;
                }
            }
        }
    }
    for (int i = 0; i < 359; i++) {
        length += sqrt(
            pow(coordinates[i] * cos(i * PI / 180) - coordinates[i + 1] * cos((i + 1) * PI / 180), 2)
            + pow(coordinates[i] * sin(i * PI / 180) - coordinates[i + 1] * sin((i + 1) * PI / 180), 2));
    }
    length += sqrt(
        pow(coordinates[359] * cos(359 * PI / 180) - coordinates[0] * cos(0 * PI / 180), 2)
        + pow(coordinates[359] * sin(359 * PI / 180) - coordinates[0] * sin(0 * PI / 180), 2));
    return length;
}

int xcentroid(static unsigned char f[WIDTH][HEIGHT]) {
    int dsum = 0, nsumx = 0, nsumy = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            dsum += f[j][i];
            nsumx += j * f[j][i];
            nsumy += i * f[j][i];
        }
    }
    int xg = nsumx / dsum;
    int yg = nsumy / dsum;
    //printf("G(%d, %d)", xg, yg);
    if (f[xg][yg] == 1) {
        //printf("穴なし\n");
    }
    else if (f[xg][yg] == 0) {
        //printf("穴あり\n");
    }
    return xg;
}

int ycentroid(static unsigned char f[WIDTH][HEIGHT]) {
    int dsum = 0, nsumx = 0, nsumy = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            dsum += f[j][i];
            nsumx += j * f[j][i];
            nsumy += i * f[j][i];
        }
    }
    int xg = nsumx / dsum;
    int yg = nsumy / dsum;
    printf("重心G(%d, %d)", xg, yg);
    if (f[xg][yg] == 1) {
        printf("穴なし\n");
    }
    else if (f[xg][yg] == 0) {
        printf("穴あり\n");

    }
    return yg;
}

int main(void) {
    static unsigned char img[WIDTH][HEIGHT];  /* 画像を読み込むためのメモリ領域 */
    static unsigned char f[WIDTH][HEIGHT];
    int mx[3][3] = { {1, 0, -1}, {2, 0, -2}, {1, 0, -1} };
    int my[3][3] = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
    /* カメラ撮影による画像取り込み．1にすると外カメ */
    captureOpen(1);                        /* カメラからの画像取り込みを初期化 */
    while (1) {                               /* 無限ループ */
        capture(img);                         /* カメラ撮影画像を img に格納 */
        binarization(img);
        /* 図形の面積Sを定義 */
        int xg = 0, yg = 0;
        int S = 0;

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (img[j][i] == 0) {
                    f[j][i] = 0;
                }
                else if (img[j][i] == 255) {
                    f[j][i] = 1;
                }
            }
        }

        /* 面積 */
        S = area(f);/* img を別ウインドウで表示 */

        /* 重心 */
        xg = xcentroid(f);
        yg = ycentroid(f);

        sobel_operator(mx, my, img, img);
        double length = perimeter(img, xg, yg);

        printf("面積S=%d, ", S);
        printf("周囲長length=%f, ", length);
        printf("円形度%f\n", 4 * PI * (double)S / (length*length));
        double c = 4 * PI * (double)S / (length * length);
        if (f[xg][yg] == 0) {
            addCharImage(img, xg, yg, "Donut");
        }
        else if (f[xg][yg] == 1) {
            if (c >= 0.95) {
                addCharImage(img, xg, yg, "Circle");
            }
            else if (c >= 0.75) {
                addCharImage(img, xg, yg, "Square");
            }
            else if (c >= 0.65) {
                addCharImage(img, xg, yg, "Rectangle");
            } 
            else {
                addCharImage(img, xg, yg, "Triangle");
            }
        }

        dispImage(img);
    }

    /* 以降の処理は実行されない */
    return(0);
}