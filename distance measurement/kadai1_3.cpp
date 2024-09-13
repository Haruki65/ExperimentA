#include "jikkenA_image.h"
//#include "jikkenA_image_noCV.h"
#include "jikkenA_matrix.h"

void calculate(double c[9], double xy[3]) {
	// カメラパラメータ行列を逆行列に変換する
	Mat C = matrixAlloc(3, 3);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			ELEM0(C, i, j) = c[i * 3 + j];
		}
	}
	double h;
	matrixDisp(C);
	matrixSelfInv(C);

	h = 1 / (ELEM0(C, 2, 0) * xy[0] + ELEM0(C, 2, 1) * xy[1] + ELEM0(C, 2, 2) * xy[2]);
	
	Mat B = matrixAlloc(3, 1);
	ELEM0(B, 0, 0) = h * xy[0];
	ELEM0(B, 0, 1) = h * xy[1];
	ELEM0(B, 0, 2) = h * xy[2];

	matrixDisp(B);

	Mat XY = matrixAllocMul(C, B);
	matrixDisp(XY);
}

int main(void) {
	/* カメラパラメータ */
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
	// ここに任意の座標を入力
	double xy[3] = { 349, 194, 1 };
	calculate(c, xy);
	return 0;
}
