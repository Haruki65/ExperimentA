#include "jikkenA_image.h"
#include "jikkenA_matrix.h"
// includeの順番注意
errno_t error;

Mat convertCamToReal(Mat invC, int CamX, int CamY) {
	double h;

	//h = 1 / (ELEM0(invC, 2, 0) * CamX + ELEM0(invC, 2, 1) * CamY + ELEM0(invC, 2, 2));

	Mat B = matrixAlloc(3, 1);
	ELEM0(B, 0, 0) = CamX;
	ELEM0(B, 0, 1) = CamY;
	ELEM0(B, 0, 2) = 1;

	Mat XY = matrixAllocMul(invC, B);
	ELEM0(XY, 0, 0) = ELEM0(XY, 0, 0) / ELEM0(XY, 0, 2);
	ELEM0(XY, 0, 1) = ELEM0(XY, 0, 1) / ELEM0(XY, 0, 2);
	ELEM0(XY, 0, 2) = ELEM0(XY, 0, 2) / ELEM0(XY, 0, 2);


	return XY;
}

int main(void) {
	FILE* p;
	if ((error = fopen_s(&p, "kadai2_5_11.txt", "w")) == NULL) {
		printf("ファイルオープンエラー");
	}
	/* カメラパラメータ配列 */
	double c[9] = {
	19.3307,
	3.10538,
	260,
	1.23002,
	-9.90464,
	249,
	0.00408675,
	0.0102623,
	1
	};
	// カメラパラメータ配列を行列に
	Mat C = matrixAlloc(3, 3);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			ELEM0(C, i, j) = c[i * 3 + j];
		}
	}
	/* Cを逆行列に */
	matrixSelfInv(C);

	/* invCとカメラ座標を引数に */
	unsigned char inputImage[HEIGHT][WIDTH];
	readImage("thinning_kadai2_11.png", inputImage);

	/* 500で十分 */
	int laserpoints[500][2] = { {0} };
	/* レーザーポインタの数にもなる */
	int idx = 0;
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (inputImage[i][j] == 255) {
				laserpoints[idx][0] = i;
				laserpoints[idx][1] = j;
				idx++;
			}
		}
	}


	Mat XY[500];
	for (int i = 0; i < idx; i++) {
		XY[i] = convertCamToReal(C, laserpoints[i][1], laserpoints[i][0]);
		
		fprintf(p, "%d\t", i);
		fprintf(p, "%f\t", ELEM0(XY[i], 0, 0));
		fprintf(p, "%f\n", ELEM0(XY[i], 0, 1));
	}

	Mat tmp = convertCamToReal(C, 262, 193);

	fclose(p);
	return 0;
}

