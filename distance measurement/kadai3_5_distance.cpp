#include "jikkenA_image.h"
#include "jikkenA_matrix.h"
#define fov 52.8
#define B 30
# define PI 3.14159265359

int main(void) {
	double Z, f, x;

	static unsigned char inputImage[HEIGHT][WIDTH];
	readImage("kadai3_3_ncc.png", inputImage);
	double sum = 0;
	for (int i = 235; i < 285; i++) {
		for (int j = 325; j < 375; j++) {
			sum += inputImage[i][j];
		}
	}

	x = sum / (50 * 50);
	printf("x=%f\n", x);
	
	double radian = fov * (PI / 180);
	f = WIDTH / tan(radian);
	Z = B * f / x;
	printf("距離%f", Z);
	return 0;
}