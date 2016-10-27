#include <omp.h>
#include <stdio.h>

static long num_steps = 100000000;
double step;

#define NUM_THREADS 2

/*
Parameter 1 can be : 
omp_sched_static
omp_sched_dynamic
omp_sched_guided
omp_sched_auto

Parameter 2 specifies chunk size
If set to 0, default is used
*/

int main() {
	/*
	In below program dynmaic gives worst performance with around 4.4667 sec
	All the other methods almost roughly give the same runtime around 0.455 sec
	*/
	int i;
	double sum = 0.0, pi, x;
	step = 1.0/(double)num_steps;
	omp_set_num_threads(NUM_THREADS);
	omp_set_schedule(omp_sched_auto, 0);
	double start = omp_get_wtime();
	#pragma omp parallel for reduction(+:sum) schedule(runtime)
		for(i=0; i<num_steps; ++i) {
			x = (i+0.5)*step;
			sum = sum + 4.0/(1.0 + x*x);
		}
	pi = sum * step;
	double end = omp_get_wtime();
	printf("%lf\n", pi);
	printf("Time taken : %lf\n", end - start);
}