#include "jikkenA_image.h"

void thinning(unsigned char inputImage[HEIGHT][WIDTH], unsigned char dst[HEIGHT][WIDTH]) {

	/* ピクセルの移動に使う配列 */
	int offset[9][2] = { {0, 0}, {0,-1}, {1, -1}, {1,0}, {1,1}, {0, 1}, {-1,1}, {-1,0}, {-1,-1} };
	int px, py;

	/* すでに2値化されている事実を利用し0, 1に変換 */
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (inputImage[y][x] == 255) {
				inputImage[y][x] = 1;
			}
		}
	}
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			/* α, βの2つの処理を行う */
			for (int pattern = 0; pattern < 2; pattern++) {
				/* 配列pはp1...p9の色情報を格納 */
				int p[9];
				for (int i = 0; i < 9; i++) {
					px = x + offset[i][0];
					py = y + offset[i][1];
					if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
						p[i] = inputImage[py][px];
					}
				}
				/* ピクセル値が「0 から 1」に変化した回数
				あまり細線化されなかった時は配列のindexが間違っていた. また(p[8] == 0 && p[1] == 1)を忘れていた */
				/* (p[0] == 0 && p[1] == 1)は足さなくて良い */
				int A = (p[1] == 0 && p[2] == 1) + (p[2] == 0 && p[3] == 1) +
					(p[3] == 0 && p[4] == 1) + (p[4] == 0 && p[5] == 1) +
					(p[5] == 0 && p[6] == 1) + (p[6] == 0 && p[7] == 1) +
					(p[7] == 0 && p[8] == 1) + (p[8] == 0 && p[1] == 1);
				int m1 = 0, m2 = 0, b = 0;

				/* p[0]を含まない. ずっと含めていて上手くいかなかった（線が完全に消えてしまっていた）.  */
				for (int i = 1; i < 9; i++) {
					b += p[i];
				}

				if (pattern == 0) {
					m1 = (p[1] * p[3] * p[5]);
					m2 = (p[3] * p[5] * p[7]);
				}
				else if (pattern == 1) {
					m1 = (p[1] * p[3] * p[7]);
					m2 = (p[1] * p[5] * p[7]);
				}

				if (A == 1 && (b >= 2 && b <= 6) && m1 == 0 && m2 == 0) {
					inputImage[y][x] = 0;
				}
			}

		}
	}
	/* 最後に0, 1の2値から0, 255に直す */
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (inputImage[y][x] == 1) {
				dst[y][x] = 255;
			}
			else {
				dst[y][x] = 0;
			}
		}
	}
}

int main(void) {
	static unsigned char inputImage[HEIGHT][WIDTH];
	static unsigned char outputImage[HEIGHT][WIDTH];

	readImage("binary_kadai2_1.png", inputImage);
	thinning(inputImage, outputImage);
	writeImage("thinning_kadai2_1.png", outputImage);

	readImage("binary_kadai2_2.png", inputImage);
	thinning(inputImage, outputImage);
	writeImage("thinning_kadai2_2.png", outputImage);

	readImage("binary_kadai2_3.png", inputImage);
	thinning(inputImage, outputImage);
	writeImage("thinning_kadai2_3.png", outputImage);

	readImage("binary_kadai2_4.png", inputImage);
	thinning(inputImage, outputImage);
	writeImage("thinning_kadai2_4.png", outputImage);

	readImage("binary_kadai2_5.png", inputImage);
	thinning(inputImage, outputImage);
	writeImage("thinning_kadai2_5.png", outputImage);

	readImage("binary_kadai2_6.png", inputImage);
	thinning(inputImage, outputImage);
	writeImage("thinning_kadai2_6.png", outputImage);

	readImage("binary_kadai2_7.png", inputImage);
	thinning(inputImage, outputImage);
	writeImage("thinning_kadai2_7.png", outputImage);

	readImage("binary_kadai2_8.png", inputImage);
	thinning(inputImage, outputImage);
	writeImage("thinning_kadai2_8.png", outputImage);

	readImage("binary_kadai2_9.png", inputImage);
	thinning(inputImage, outputImage);
	writeImage("thinning_kadai2_9.png", outputImage);

	readImage("binary_kadai2_10.png", inputImage);
	thinning(inputImage, outputImage);
	writeImage("thinning_kadai2_10.png", outputImage);

	readImage("binary_kadai2_11.png", inputImage);
	thinning(inputImage, outputImage);
	writeImage("thinning_kadai2_11.png", outputImage);


	return 0;
}