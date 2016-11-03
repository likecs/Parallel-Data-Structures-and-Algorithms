#include <omp.h>
#include <stdio.h>

static long num_steps = 100000000;
double step;

int main() {
	int i;
	double x, pi;
	double sum = 0.0;
	step = 1.0/(double)num_steps;
	double start = omp_get_wtime();
	for(i=0; i<num_steps; i++) {
		x = (i+0.5)*step;
		sum = sum + 4.0/(1.0 + x*x);
	}
	pi = step * sum;
	double end = omp_get_wtime();
	printf("%lf\n", pi);
	printf("Time taken : %lf\n", end - start);
}