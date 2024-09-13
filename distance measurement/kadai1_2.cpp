#include "jikkenA_image.h"
#include "jikkenA_matrix.h"
#define n 4

// intでは上手くいかなかったのでdoubleにする
void calculateCamparam(double x[4], double y[4], double X[4], double Y[4]) {
	Mat B = matrixAlloc(8, 1);
	Mat A = matrixAlloc(8, 8);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			ELEM0(A, i, j) = 0;
		}
	}
	for (int i = 0; i < n; i++) {
		ELEM0(A, 2 * i, 0) = X[i];
		ELEM0(A, 2 * i, 1) = Y[i];
		ELEM0(A, 2 * i, 2) = 1;

		ELEM0(A, 2 * i + 1, 3) = X[i];
		ELEM0(A, 2 * i + 1, 4) = Y[i];
		ELEM0(A, 2 * i + 1, 5) = 1;
	}
	for (int i = 0; i < n; i++) {
		ELEM0(A, 2 * i, 6) = -X[i] * x[i];
		ELEM0(A, 2 * i + 1, 6) = -X[i] * y[i];
		ELEM0(A, 2 * i, 7) = -Y[i] * x[i];
		ELEM0(A, 2 * i + 1, 7) = -Y[i] * y[i];
	}
	for (int i = 0; i < n; i++) {
		ELEM0(B, 2 * i, 0) = x[i];
		ELEM0(B, 2 * i + 1, 0) = y[i];
	}
	matrixDisp(A);
	/* Aを逆行列に変換しA自身に代入 */
	matrixSelfInv(A);
	Mat c = matrixAllocMul(A, B);

	matrixDisp(c);
}

int main(void) {
	// はじめはintにしていたが上手くいかなかった
	// 世界座標の編の長さが4.0cmくらいになってしまって、そこからintになっているというミスに気がついた. doubleにして直った
	double X[n] = { 0, 4.8, 0, 4.8 }, Y[n] = { 0, 0, 4.8, 4.8 }, x[n] = { 260, 346, 262, 344 }, y[n] = { 249, 250, 192, 194 };
	calculateCamparam(x, y, X, Y);
	return 0;
}
