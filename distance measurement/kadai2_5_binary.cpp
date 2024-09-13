#include "jikkenA_image.h"

void binarization(unsigned char src[HEIGHT][WIDTH][3], unsigned char dst[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            /* 最初は128でやったが、茶色の板の部分が反応して白になってしまったため、150にして閾値を大きくした */
            if (src[i][j][2] < 150) {
                dst[i][j] = 0;
            }
            else if (src[i][j][2] >= 150) {
                dst[i][j] = 255;
            }

        }
    }
}

int main(void) {
    static unsigned char inputImage[HEIGHT][WIDTH][3];
    static unsigned char binaryImage[HEIGHT][WIDTH] = { {0} };

    /* 画像の読み込みと配列への格納 */
    readImageColor("kadai2_1.png", inputImage);
    binarization(inputImage, binaryImage);
    // 二値化された画像を保存
    writeImage("binary_kadai2_1.png", binaryImage);

    readImageColor("kadai2_2.png", inputImage);
    binarization(inputImage, binaryImage);
    writeImage("binary_kadai2_2.png", binaryImage);

    readImageColor("kadai2_3.png", inputImage);
    binarization(inputImage, binaryImage);
    writeImage("binary_kadai2_3.png", binaryImage);

    readImageColor("kadai2_4.png", inputImage);
    binarization(inputImage, binaryImage);
    writeImage("binary_kadai2_4.png", binaryImage);

    readImageColor("kadai2_5.png", inputImage);
    binarization(inputImage, binaryImage);
    writeImage("binary_kadai2_5.png", binaryImage);

    readImageColor("kadai2_6.png", inputImage);
    binarization(inputImage, binaryImage);
    writeImage("binary_kadai2_6.png", binaryImage);

    readImageColor("kadai2_7.png", inputImage);
    binarization(inputImage, binaryImage);
    writeImage("binary_kadai2_7.png", binaryImage);

    readImageColor("kadai2_8.png", inputImage);
    binarization(inputImage, binaryImage);
    writeImage("binary_kadai2_8.png", binaryImage);

    readImageColor("kadai2_9.png", inputImage);
    binarization(inputImage, binaryImage);
    writeImage("binary_kadai2_9.png", binaryImage);

    readImageColor("kadai2_10.png", inputImage);
    binarization(inputImage, binaryImage);
    writeImage("binary_kadai2_10.png", binaryImage);

    readImageColor("kadai2_11.png", inputImage);
    binarization(inputImage, binaryImage);
    writeImage("binary_kadai2_11.png", binaryImage);

    return 0;
}