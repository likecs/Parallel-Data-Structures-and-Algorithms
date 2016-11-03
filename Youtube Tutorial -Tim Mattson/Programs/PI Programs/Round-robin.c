#include <omp.h>
#include <stdio.h>

static long num_steps = 100000000;
double step;

#define NUM_THREADS 2

int main() {
	double pi;
	step = 1.0/(double)num_steps;
	omp_set_num_threads(NUM_THREADS);
	double start = omp_get_wtime();
	#pragma omp parallel
	{
		int i, id, nthrds;
		double x, sum;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		for(i=id, sum = 0.0; i<num_steps; i=i+nthrds) {
			x = (i+0.5)*step;
			sum = sum + 4.0/(1.0 + x*x);
		}
		#pragma omp critical
			pi += sum * step;
	}
	double end = omp_get_wtime();
	printf("%lf\n", pi);
	printf("Time taken : %lf\n", end - start);
}