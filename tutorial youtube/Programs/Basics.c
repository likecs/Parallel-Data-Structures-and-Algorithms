#include <omp.h>
#include <stdio.h>

int main() {
	int max = omp_get_max_threads();
	printf("We can get a max of %d threads.\n", max);
	int processors = omp_get_num_procs();
	printf("You have %d processors on your system\n", processors);
	#pragma omp parallel
	{
		int id = omp_get_thread_num();
		printf("Hello(%d) ", id);
		printf("World(%d)\n", id);
	}
	printf("Your OpenMP version and spec dates to the implementation on %d\n", _OPENMP);
	return 0;
}