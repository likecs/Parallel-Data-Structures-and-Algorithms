#include <iostream>
#include <cstdio>
#include <cstring>
#include <malloc.h>
#include <cassert>
#include <climits>
#include <cmath>
#include <omp.h>
 
using namespace std;

int val[21];

int data_set[10001][2];

int main() {
	int ctr = 0;
	while(1) {
		scanf("%d", &data_set[ctr][0]);
		if (data_set[ctr][0] == 1 || data_set[ctr][0] == 2) {
			scanf("%d", &data_set[ctr][1]);
			ctr += 1;
		}
		else{
			break;
		}
	}
	for(int i=0; i<ctr; ++i) {
		if (data_set[i][0] == 1) {
			val[data_set[i][1]] += 1;
		}
		else {
			if (val[data_set[i][1]] > 0) {
				val[data_set[i][1]] -= 1;	
			}
		}
	}
	printf("Contents : ");
	printf("%d ", INT_MIN);
	for(int i=1; i<=20; ++i) {
		for(int j=1; j<=val[i]; ++j) {
			printf("%d ", i);
		}
	}
	printf("%d \n", INT_MAX);
	return 0;
}