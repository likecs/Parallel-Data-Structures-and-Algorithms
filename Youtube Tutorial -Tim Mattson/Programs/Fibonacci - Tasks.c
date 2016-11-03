#include <omp.h>
#include <stdio.h>

//very slow even slower than sequential one
int fib(int n) {
	int x, y;
	if (n < 2) {
		return n;
	}
	#pragma omp task shared(x)
	x = fib(n-1);
	#pragma omp task shared(y)
	y = fib(n-2);
	#pragma omp taskwait
	return x + y;
}

int main() {
	double start = omp_get_wtime();
	int ans = fib(40);
	double end = omp_get_wtime();
	printf("The 40th Fibonacci number is %d\n", ans);
	printf("Time taken : %lf\n", end - start);
}