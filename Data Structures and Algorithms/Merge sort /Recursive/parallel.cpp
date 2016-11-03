#include <iostream>
#include <cstdio>
#include <omp.h>

using namespace std;

void mergesort(int *data_set, int start, int finish) {
	if (start == finish) {
		return ;
	}
	int mid = (start + finish) / 2;
	#pragma omp task
	{
		mergesort(data_set, start, mid);
	}
	#pragma omp task
	{
		mergesort(data_set, mid + 1, finish);
	}
	#pragma omp taskwait
	if (data_set[mid] > data_set[mid+1]) {
		int length = finish - start + 1;
		int *buffer = new int[length];
		int u = start, v = mid + 1, cnt = 0;
		while(u <= mid && v <= finish) {
			if (data_set[u] <= data_set[v]) {
				buffer[cnt++] = data_set[u++];
			}
			else {
				buffer[cnt++] = data_set[v++];
			}
		}
		while(u <= mid) {
			buffer[cnt++] = data_set[u++];
		}
		while(v <= finish) {
			buffer[cnt++] = data_set[v++];
		}
		for(int k = 0; k < length; ++k) {
			data_set[start + k] = buffer[k];
		}
		delete[] buffer;
	}
}

int main() {
	int n;
	scanf("%d", &n);
	int *data_set = new int[n];
	for(int i = 0; i < n; ++i) {
		scanf("%d", &data_set[i]);
	}
	double start_time = omp_get_wtime();
	#pragma omp parallel
	{
		#pragma omp single
		{
			mergesort(data_set, 0, n - 1);
		}
	}
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