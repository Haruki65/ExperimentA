#include "jikkenA.h"  /* 実験に用いる各種関数の定義 */
#include "math.h"
#define PI 3.141592653589793

/*
* カメラ映像のウインドウ表示中に 'q' を押すとプログラムは終了する
*/

static int label[WIDTH][HEIGHT];

/* 2値化を行う関数 */
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

/* 畳み込み演算を行う関数 */
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

/* ソーベルオペレータ */
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

/* 周囲長を行う関数 */
double perimeter(static unsigned char Le[WIDTH][HEIGHT], int xg, int yg) {
    double coordinates[360] = { 0 };
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

/* 8近傍のラベルの最小値を求める関数 */
int neighber(int label[WIDTH][HEIGHT], int x, int y) {
    int min = INT_MAX;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                if (label[nx][ny] != 0 && label[nx][ny] < min) {
                    min = label[nx][ny];
                }
            }
        }
    }
    return (min == INT_MAX) ? 0 : min;
}

/* 各図形のパラメータを計算し，図形を判別するプログラム */
void param_cal(static unsigned char img[WIDTH][HEIGHT], static unsigned char img2[WIDTH][HEIGHT], int shape_label) {
    /* 図形の面積Sを定義 */
    int xg = 0, yg = 0;
    int S = 0, Sx = 0, Sy = 0;
    int mx[3][3] = { {1, 0, -1}, {2, 0, -2}, {1, 0, -1} };
    int my[3][3] = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };

    // 図形の重心を計算
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            if (label[x][y] == shape_label) {
                S++;
                Sx += x;
                Sy += y;
            }
        }
    }
    if (S == 0) return; // 図形がない場合はリターン

    xg = Sx / S;
    yg = Sy / S;
    // printf("G(%d, %d)\n", xg, yg);

    sobel_operator(mx, my, img, img);
    double length = perimeter(img, xg, yg);

    /* 判別木 */
    double c = 4 * PI * (double)S / (length * length);
    if (img2[xg][yg] == 0) {
        addCharImage(img2, xg, yg, "Donut");
    }
    else {
        if (c >= 0.95) {
            addCharImage(img2, xg, yg, "Circle");
        }
        else if (c >= 0.75) {
            addCharImage(img2, xg, yg, "Square");
        }
        else if (c >= 0.65) {
            addCharImage(img2, xg, yg, "Rectangle");
        }
        else {
            addCharImage(img2, xg, yg, "Triangle");
        }
    }


}

int main(void) {
    static unsigned char img[WIDTH][HEIGHT];  /* 画像を読み込むためのメモリ領域 */
    static unsigned char img2[WIDTH][HEIGHT];
    static int unique_labels[1000];
    /* カメラ撮影による画像取り込み．1にすると外カメ */
    captureOpen(0);                        /* カメラからの画像取り込みを初期化 */
    while (1) {
        // 初期化はここで行う
        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                label[x][y] = 0;
                img2[x][y] = 0;
            }
        }
        /* 無限ループ */
        capture(img);                         /* カメラ撮影画像を img に格納 */
        binarization(img);
        int index = 1;
        int changed = 0;

        do {
            changed = 0;
            for (int y = 0; y < HEIGHT; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    // 注目画素が255の場合（物体領域）
                    if (img[x][y] == 255) {
                        int neighber_label = neighber(label, x, y);
                        if (neighber_label == 0) {
                            label[x][y] = index++;
                            changed++;
                        }
                        else if (label[x][y] != neighber_label) {
                            label[x][y] = neighber_label;
                            changed++;
                        }
                    }
                    img2[x][y] = label[x][y] * 50;
                }
            }
            
        } while (changed > 500);

        int label_max = 0;

        /* ラベルの最大値を求める */
        for (int x = 0; x < HEIGHT; x++) {
            for (int y = 0; y < WIDTH; y++) {
                if (label[x][y] > label_max) {
                    label_max = label[x][y];
                }
            }
        }
        for (int i = 0; i < 1000; i++) {
            unique_labels[i] = 0;
        }

        for (int x = 0; x < HEIGHT; x++) {
            for (int y = 0; y < WIDTH; y++) {
                if (label[x][y] > 0) {
                    unique_labels[label[x][y]] = 1;
                }
            }
        }

        /* 画面映る図形の数を計算 */
        int shape_count = 0;
        for (int i = 1; i < 1000; i++) {
            if (unique_labels[i] == 1) {
                shape_count++;
            }
        }

        // printf("図形の数: %d\n", shape_count);

        for (int i = 1; i <= shape_count; i++) {
            param_cal(img, img2, i);
        }
        dispImage(img2);

    }
    free(img);
    free(img2);
    return(0);
}