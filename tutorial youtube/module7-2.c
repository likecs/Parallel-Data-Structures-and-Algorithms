#include <omp.h>
#include <stdio.h>

static long num_steps = 100000000;
double step;

#define NUM_THREADS 3

int main() {
	int i;
	double sum = 0.0, pi, x;
	step = 1.0/(double)num_steps;
	omp_set_num_threads(NUM_THREADS);
	double start = omp_get_wtime();
	#pragma omp parallel for private(x) reduction(+:sum)
		for(i=0; i<num_steps; ++i) {
			x = (i+0.5)*step;
			sum = sum + 4.0/(1.0 + x*x);
		}
	pi = sum * step;
	double end = omp_get_wtime();
	printf("%lf\n", pi);
	printf("Time taken : %lf\n", end - start);
}