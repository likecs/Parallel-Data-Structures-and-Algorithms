#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <cassert>
#include <omp.h>

using namespace std;

const int SIZE = 4e5 + 5;

#define NIL			-1
#define PRESENT		3
#define WANTED		2
#define PENDING		1
#define ABSENT		0

int last_elem;
int full_level;
int heap[SIZE];
int type[SIZE];
int val[SIZE];
int status[SIZE];
omp_lock_t heap_elem[SIZE];

void swap(int &a, int &b) {
	a ^= b;
	b ^= a;
	a ^= b;
}

int LSON(int i) {
	return i*2;
}

int RSON(int i) {
	return i*2+1;
}

int MIN(int i) {
	if (heap[LSON(i)] <= heap[RSON(i)]) {
		return LSON(i);
	}
	else {
		return RSON(i);
	}
}

int MAX(int i) {
	if (heap[LSON(i)] >= heap[RSON(i)]) {
		return LSON(i);
	}
	else {
		return RSON(i);
	}
}

void concurrent_insert(int key) {
	omp_set_lock(&heap_elem[1]);
	last_elem += 1;
	int target = last_elem;
	if (last_elem >= full_level*2) {
		full_level = last_elem;
	}
	int i = target - full_level;
	int j = full_level / 2;
	int k = 1;
	status[target] = PENDING;
	while (j != 0) {
		if (status[target] == WANTED) {
			break;
		} 
		if (heap[k] > key) {
			swap(key, heap[k]);
		}
		if (i >= j) {
			omp_set_lock(&heap_elem[RSON(k)]);
			omp_unset_lock(&heap_elem[k]);
			k = RSON(k);
			i = i - j;
		}
		else {
			omp_set_lock(&heap_elem[LSON(k)]);
			omp_unset_lock(&heap_elem[k]);
			k = LSON(k);
		}
		j = j / 2;
	}
	if (status[target] == WANTED) {
		heap[1] = key;
		status[target] = ABSENT;
		status[1] = PRESENT;
	}
	else {
		heap[target] = key;
		status[target] = PRESENT;
	}
	omp_unset_lock(&heap_elem[k]);
}

int concurrent_delete() {
	omp_set_lock(&heap_elem[1]);
	if (last_elem == 0) {
		omp_unset_lock(&heap_elem[1]);
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
		status[1] = ABSENT;
		omp_unset_lock(&heap_elem[1]);
		return least;
	}
	omp_set_lock(&heap_elem[j]);
	if (status[j] == PRESENT) {
		heap[1] = heap[j];
		status[j] = ABSENT;
		heap[j] = INT_MAX;
	}
	else {
		status[1] = ABSENT;
		status[j] = WANTED;
	}
	omp_unset_lock(&heap_elem[j]);
	while(status[i] == ABSENT) {
		//
	}
	omp_set_lock(&heap_elem[LSON(i)]);
	omp_set_lock(&heap_elem[RSON(i)]);
	while(heap[i] > heap[MIN(i)]) {
		swap(heap[i], heap[MIN(i)]);
		omp_unset_lock(&heap_elem[i]);
		omp_unset_lock(&heap_elem[MAX(i)]);
		i = MIN(i);
		omp_set_lock(&heap_elem[LSON(i)]);
		omp_set_lock(&heap_elem[RSON(i)]);
	}
	omp_unset_lock(&heap_elem[i]);
	omp_unset_lock(&heap_elem[LSON(i)]);
	omp_unset_lock(&heap_elem[RSON(i)]);
	return least;
}

void init() {
	last_elem = 0;
	full_level = 0;
	for(int i=0; i<SIZE; ++i) {
		omp_init_lock(&heap_elem[i]);
	}
}

void over() {
	for(int i=0; i<SIZE; ++i) {
		omp_destroy_lock(&heap_elem[i]);
	}
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
				concurrent_insert(val[i]);
			}
			else {
				concurrent_delete();
			}
		}
	}
	double end = omp_get_wtime();
	printf("Time : taken %lf\n", end - start);
	over();
	return 0;
}