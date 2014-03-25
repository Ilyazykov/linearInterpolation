#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//////////////////
struct floatWithNumber
{
	float val;
	int num;
};

int comparator(const void* v1, const void* v2)
{
	return ((floatWithNumber*)v1)->val > ((floatWithNumber*)v2)->val;
}

int interpolate(int n, float *x, float *y, int m, float *s, float *r) //O(m*log(m) + n)
{
	floatWithNumber *sortS = new floatWithNumber[m];
	for (int i=0; i<m; i++)
	{
		sortS[i].num = i;
		sortS[i].val = s[i];
	}
	qsort(sortS, m, sizeof(floatWithNumber), comparator);

	int currentN = 0;
	for (int i=0; i<m; i++)
	{
		// находим промежуток
		while (sortS[i].val > x[currentN+1]) currentN++;

		// получаем параметры линейного уравнения ax+b=y
		float a = (y[currentN+1]-y[currentN]) / (x[currentN+1]-x[currentN]);
		float b = (y[currentN] - a*x[currentN]);

		//вычисляем r
		r[sortS[i].num] = a*sortS[i].val+b;
	}

	delete[] sortS;
	return 0;
}


/////////////////
int getInterval(float *x, float s, int leftNumber, int rightNumber)
{
	if (leftNumber+1 == rightNumber) return leftNumber;

	int averageNumber = (rightNumber + leftNumber)/2;
	if (s < averageNumber) return getInterval(x, s, leftNumber, averageNumber);
	else return getInterval(x, s, averageNumber, rightNumber);
}

int interpolate2(int n, float *x, float *y, int m, float *s, float *r) //O(m*log(n))
{
	for (int i=0; i<m; i++)
	{
		int currentN = getInterval(x, s[i], 0, n);

		float a = (y[currentN+1]-y[currentN]) / (x[currentN+1]-x[currentN]);
		float b = (y[currentN] - a*x[currentN]);

		r[i] = a*s[i]+b;
	}

	return 0;
}
//////////////////

int interpolate3(int n, float *x, float *y, int m, float *s, float *r) //O(m*log(n))
{
	for (int i=0; i<m; i++)
	{
		int begin = 0;
		int end = n;
		
		while (end-begin>1)
		{
			int average = (begin+end)/2;
			if (s[i] < x[average]) end = average;
			else begin = average;
		}

		int currentN = begin;

		float a = (y[currentN+1]-y[currentN]) / (x[currentN+1]-x[currentN]);
		float b = (y[currentN] - a*x[currentN]);

		r[i] = a*s[i]+b;
	}

	return 0;
}
///////////////

void main()
{
	int numberOfPoints = 10;
	float *x = new float[numberOfPoints];
	float *y = new float[numberOfPoints];

	float startX = 2.0f;
	for (int i=0; i<numberOfPoints; i++)
	{
		x[i] = startX + i*1.1f;
		y[i] = 2*x[i];
	}

	int numberOfValues = 5;
	float *s = new float[numberOfValues];
	s[0] = 4.0f; s[1] = 6.0f; s[2] = 3.0f; s[3] = 8.0f; s[4] = 7.5f;
	float *r = new float[numberOfValues];

	interpolate3(numberOfPoints, x, y, numberOfValues, s, r);

	for (int i=0; i<numberOfValues; i++)
	{
		 printf("%f\n", r[i]);
	}

	delete[] x;
	delete[] y;
	delete[] s;
	delete[] r;
}