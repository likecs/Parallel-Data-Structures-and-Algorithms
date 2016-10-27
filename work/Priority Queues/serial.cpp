#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <cassert>
#include <omp.h>
using namespace std;

const int MAX = 4e5 + 5;
const int NIL = -1;

int last_elem;
int full_level;
int heap[MAX];
int type[MAX];
int val[MAX];
omp_lock_t heap_lock;

void swap(int &a, int &b) {
	a ^= b;
	b ^= a;
	a ^= b;
}

int MIN(int i) {
	if (heap[i*2] <= heap[i*2+1]) {
		return i*2;
	}
	else {
		return i*2+1;
	}
}

void insert_heap(int key) {
	omp_set_lock(&heap_lock);
	last_elem += 1;
	if (last_elem >= full_level*2) {
		full_level = last_elem;
	}
	int i = last_elem;
	heap[i] = key;
	while (i != 1 && heap[i] < heap[i/2]) {
		swap(heap[i], heap[i/2]);
		i = i/2;
	}
	omp_unset_lock(&heap_lock);
}

int delete_heap() {
	omp_set_lock(&heap_lock);
	if (last_elem == 0) {
		omp_unset_lock(&heap_lock);
		return NIL;
	}
	int least = heap[1];
	int i = 1;
	int j = last_elem;
	last_elem -= 1;
	if (last_elem < full_level) {
		full_level /= 2;
	}
	if (j==1) {
		heap[1] = INT_MAX;
		omp_unset_lock(&heap_lock);
		return least;
	}
	heap[i] = heap[j];
	heap[j] = INT_MAX;
	int k = MIN(i);
	while(heap[i] > heap[k]) {
		swap(heap[i], heap[k]);
		i = k;
		k = MIN(i);
	}
	omp_unset_lock(&heap_lock);
	return least;
}

void init() {
	last_elem = 0;
	full_level = 0;
	omp_init_lock(&heap_lock);
}

void over() {
	omp_destroy_lock(&heap_lock);
}

int main() {
	init();
	int n;
	scanf("%d", &n);
	double start = omp_get_wtime();
	for(int i=1; i<=n; ++i) {
		scanf("%d", &type[i]);
		if (type[i] == 1) {
			scanf("%d", &val[i]);
		}
		else {
			val[i] = -1;
		}
	}
	#pragma omp parallel
	{
		int id, nthrds;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		for(int i=id+1; i<=n; i+=nthrds) {
			if (type[i]==1) {
				insert_heap(val[i]);
			}
			else {
				delete_heap();
			}
		}
	}
	double end = omp_get_wtime();
	printf("Time : taken %lf\n", end - start);
	over();
	return 0;
}