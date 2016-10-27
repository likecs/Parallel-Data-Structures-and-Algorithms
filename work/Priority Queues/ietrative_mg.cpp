#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

const int SIZE = 1e6 + 6;

int a[SIZE];
int temp[SIZE];

int main() {
	int n;
	scanf("%d", &n);
	for(int i=1; i<=n; ++i) {
		scanf("%d", &a[i]);
	}
	double start = omp_get_wtime();
	int lg = ceil(log2(n));
	int s1, e1, s2, e2, cnt, displace;
	for(int i=1; i<=lg; ++i) {
		displace = 1<<i;
		for(int j=1; j<=n; j+=displace) {
			s1 = j;
			e1 = min(n-1, j + displace / 2 - 1);
			s2 = min(n, j + displace / 2);
			e2 = min(n, j + displace - 1);
			cnt = 0;
			while(s1 <= e1 && s2 <= e2) {
				if (a[s1] <= a[s2]) {
					temp[cnt++] = a[s1++];
				}
				else {
					temp[cnt++] = a[s2++];
				}
			}
			while(s1 <= e1) {
				temp[cnt++] = a[s1++];
			}
			while(s2 <= e2) {
				temp[cnt++] = a[s2++];
			}
			for(int k=j, l=0; k<=e2; ++k) {
				a[k] = temp[l++];
			}
		}
	}
	double end = omp_get_wtime();
	bool done = true;
	for(int i=1; i<n; ++i) {
		done &= (a[i] <= a[i+1]);
	}
	assert(done);
	printf("Array is sorted\n");
	printf("Time taken : %lf\n", end - start);
	return 0;
}