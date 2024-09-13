#include "jikkenA_image.h"
#include "jikkenA_matrix.h"
// includeの順番注意
errno_t error;
/* 変更箇所 */
#define N 202

typedef struct {
	double x;
	double y;
} Point;

/* 比較関数重要. qsortでどうソートするかに関わる */
int compare(const void* a, const void* b) {
	Point* pointA = (Point*)a;
	Point* pointB = (Point*)b;
	if (pointA->x > pointB->x)
	{
		return 1;
	}
	else if (pointA->x < pointB->x)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int main(void) {
	FILE* p;
	Point XY[N];
	int tmp;
	double x1 = 0, x2 = 0, x3 = 0, x4 = 0, y1 = 0, y2 = 0, y3 = 0, y4 = 0;
	double topside, bottomside, height, h1, h2, S, V;
	/* 変更箇所 */
	if ((error = fopen_s(&p, "kadai2_5_11.txt", "r")) == NULL) {
		printf("ファイルオープンエラー\n");
	}
	for (int i = 0; i < N; i++)
	{
		fscanf_s(p, "%d\t%lf\t%lf\n", &tmp, &(XY[i].x), &(XY[i].y));
	}

	qsort(XY, N, sizeof(Point), compare);

	for (int i = 0; i < N; i++) {
		printf("%f\t", XY[i].x);
		printf("%f\n", XY[i].y);
	}

	int count = 0;
	/* i < Nではない. 注意する */
	for (int i = 0; i < N - 1; i++) {
		if (abs(XY[i + 1].y - XY[i].y) > 0.3) {
			if (count == 0) {
				x1 = XY[i].x;
				y1 = XY[i].y;
				x2 = XY[i + 1].x;
				y2 = XY[i + 1].y;
				count++;
			}
			else if (count == 1) {
				x3 = XY[i].x;
				y3 = XY[i].y;
				x4 = XY[i + 1].x;
				y4 = XY[i + 1].y;
			}
		}

	}
	topside = x3 - x2;
	bottomside = x4 - x1;

	/*printf("%f\n", topside);
	printf("%f\n", bottomside);*/

	h1 = (y2 + y3) / 2;
	h2 = (y1 + y4) / 2;
	height = h1 - h2;
	S = (topside + bottomside) * height / 2;
	/* kadai2_5_11.txtの時のみ0.1 */
	V = S * 0.1;
	printf("部分体積S=%f\n", V);
	fclose(p);
	return 0;
}