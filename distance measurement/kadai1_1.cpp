#include "jikkenA_image.h"
//#include "jikkenA_image_noCV.h"
#include "jikkenA_matrix.h"
#define n 4


void calculateCamparam(int x[4], int y[4], int X[4], int Y[4]) {
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
	/* Aを逆行列に変換しAに代入 */
	matrixSelfInv(A);
	Mat c = matrixAllocMul(A, B);

	matrixDisp(c);
}

int main(void) {
	int X[n] = { 0, 2, 0, 4 }, Y[n] = { 0, 0, 2, 4 }, x[n] = { 100, 60, 50, 28 }, y[n] = { 100, 50, 60, 28 };
	calculateCamparam(x, y, X, Y);
	return 0;
}
