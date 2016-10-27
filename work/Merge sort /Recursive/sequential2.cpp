#include <iostream>
#include <cstdio>
#include <omp.h>

using namespace std;

void merge(int *data_set, int n, int *buffer) {
	int mid = n / 2;
	if (data_set[mid-1] <= data_set[mid]) {
		return ;
	}
	int u = 0, v = mid, cnt = 0;
	while(u < mid && v < n) {
		if (data_set[u] < data_set[v]) {
			buffer[cnt++] = data_set[u++];
		}
		else {
			buffer[cnt++] = data_set[v++];
		}
	}
	while(u < mid) {
		buffer[cnt++] = data_set[u++];
	}
	while(v < n) {
		buffer[cnt++] = data_set[v++];
	}
	for(int i = 0; i < cnt; ++i) {
		data_set[i] = buffer[i];
	}
}

void mergesort(int *data_set, int n, int *buffer) {
	if (n == 1) {
		return ;
	}
	int mid = n / 2;
	mergesort(data_set, mid, buffer);
	mergesort(data_set + mid, n - mid, buffer);
	merge(data_set, n, buffer);
}

int main() {
	int n;
	scanf("%d", &n);
	int *data_set = new int[n];
	for(int i = 0; i < n; ++i) {
		scanf("%d", &data_set[i]);
	}
	double start_time = omp_get_wtime();
	int *buffer = new int[n];
	mergesort(data_set, n, buffer);
	delete[] buffer;
	double end_time = omp_get_wtime();
	bool sorted = true;
	for(int i=1; i<n; ++i) {
		sorted &= (data_set[i] >= data_set[i-1]);
	}
	if (sorted) {
		printf("Array is sorted\n");
		printf("Time taken : %lf\n", end_time - start_time);
	}
	else {
		printf("Error in sorting\n");
	}
	delete[] data_set;
	return 0;
}