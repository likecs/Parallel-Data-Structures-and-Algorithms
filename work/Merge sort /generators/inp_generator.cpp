#include <iostream>
#include <cstdio>
#include <cassert>

const int mod = 1e9;

int main() {
	srand(unsigned(time(NULL)));
	int n;	
	scanf("%d", &n);
	printf("%d\n", n);
	for(int i=0; i<n; ++i) {
		int num = rand() % mod + 1;
		printf("%d ", num);
	}
	printf("\n");
	return 0;
}