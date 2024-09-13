#include "jikkenA_image.h"
#include "jikkenA_matrix.h"

int main(void) {
	static unsigned char leftImage[HEIGHT][WIDTH][3];
	static unsigned char rightImage[HEIGHT][WIDTH][3];
	
	/* Sは類似度. uが50まで動くよということ */
	/*static unsigned char S[HEIGHT][WIDTH];*/
	/* 視差マップ（モノクロ） */
	static unsigned char parallax[HEIGHT][WIDTH] = { {0} };

	/* 画像の読み込みと配列への格納 */
	readImageColor("image_from_left_camera.png", leftImage);
	readImageColor("image_from_right_camera.png", rightImage);

	for (int i = 0; i < HEIGHT - 10; i+=5) {
		for (int j = 0; j < WIDTH - 10; j+=5) {
			/* indexは一番類似度の高いブロックまでの距離 */
			int index = 0;
			double tmp_sad;
			double min = 1000;
			double sum;
			for (int u = 0; u + j < WIDTH - 10; u++) {
				tmp_sad = 0;
				/* 10×10のブロックを見る */
				for (int m = 0; m < 10; m++) {
					for (int n = 0; n < 10; n++) {
						sum = 0;
						/* 各チャンネルについて */
						for (int k = 0; k < 3; k++) {
							/* RGB全ての差を足して初めて視差 */						
							sum += abs(rightImage[i + m][j + n][k] - leftImage[i + m][j + u + n][k]) / 3;
						}
						tmp_sad += sum;
					}
				}
				if (tmp_sad < min) {
					min = tmp_sad;
					index = u;
				}
			}
			for (int m1 = 0; m1 < 5; m1++) {
				for (int m2 = 0; m2 < 5; m2++) {
					parallax[i + m1][j + m2] = index;
				}
			}
		}
	}
	// 画像を保存
	writeImage("parallax.png", parallax);
	return 0;
}