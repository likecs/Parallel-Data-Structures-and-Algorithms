#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX 1000000
#define MOD 1000000

int hist[MAX];
int arr[MAX];

int main() {
	freopen("inp.txt", "r", stdin);
	int i;
	for(i=0; i<MAX; ++i) {
		scanf("%d", &arr[i]);
	}
	double start = omp_get_wtime();
	for(i=0; i<MAX; ++i) {
		hist[arr[i]] += 1;
	}
	double end = omp_get_wtime();
	printf("Time taken : %lf\n", end - start);
}