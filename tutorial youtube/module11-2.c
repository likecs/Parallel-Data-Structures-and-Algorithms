#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX 1000000
#define MOD 1000000

int hist[MAX];
int hist_lock[MAX];
int arr[MAX];

int main() {
	freopen("inp.txt", "r", stdin);
	int i;
	for(i=0; i<MAX; ++i) {
		scanf("%d", &arr[i]);
	}
	#pragma omp parallel for
	for(i=0; i<MAX; ++i) {
		omp_init_lock(&hist_lock[i]);
	}
	double start = omp_get_wtime();
	#pragma omp parallel for
	for(i=0; i<MAX; ++i) {
		omp_set_lock(&hist_lock[arr[i]]);
		hist[arr[i]] += 1;
		omp_unset_lock(&hist_lock[arr[i]]);
	}
	double end = omp_get_wtime();
	for(i=0; i<MAX; ++i) {
		omp_destroy_lock(&hist_lock[i]);
	}
	printf("Time taken : %lf\n", end - start);
}