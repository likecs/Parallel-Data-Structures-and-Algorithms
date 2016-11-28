#include <iostream>
#include <cstdio>
#include <omp.h>

using namespace std;

int main() {
	int n;
	scanf("%d", &n);
	int *data_set = new int[n];
	for(int i = 0; i < n; ++i) {
		scanf("%d", &data_set[i]);
	}
	double start_time = omp_get_wtime();
	for(int i = 1; i < n; i <<= 1) {
		int step = i << 1;
		for(int j = 0; j < n; j += step) {
			int start = j, finish = min(n, j + step);
			int length = finish - start;
			int *buffer = new int[length];
			int mid = min(n, start + (step / 2));
			int u = start, v = mid, cnt = 0;
			if (data_set[mid-1] <= data_set[mid]) {
				continue;
			}
			while(u < mid && v < finish) {
				if (data_set[u] <= data_set[v]) {
					buffer[cnt++] = data_set[u++];
				}
				else {
					buffer[cnt++] = data_set[v++];
				}
			}
			while(u < mid) {
				buffer[cnt++] = data_set[u++];
			}
			while(v < finish) {
				buffer[cnt++] = data_set[v++];
			}
			for(int k = 0; k < length; ++k) {
				data_set[start + k] = buffer[k];
			}
			delete[] buffer;
		}
	}
	double end_time = omp_get_wtime();
	bool sorted = true;
	for(int i=1; i<n; ++i) {
		sorted &= (data_set[i] >= data_set[i-1]);
	}
	if (sorted) {
		printf("Array is sorted.\n");
		printf("Sequential Time taken : %lf\n", end_time - start_time);
	}
	else {
		printf("Error in sorting\n");
	}
	delete[] data_set;
	return 0;
}