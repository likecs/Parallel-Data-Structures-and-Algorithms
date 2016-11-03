#include <omp.h>
#include <stdio.h>

int fib(int n) {
	int x, y;
	if (n < 2) {
		return n;
	}
	x = fib(n-1);
	y = fib(n-2);
	return x + y;
}

int main() {
	double start = omp_get_wtime();
	int ans = fib(40);
	double end = omp_get_wtime();
	printf("The 40th Fibonacci number is %d\n", ans);
	printf("Time taken : %lf\n", end - start);
}