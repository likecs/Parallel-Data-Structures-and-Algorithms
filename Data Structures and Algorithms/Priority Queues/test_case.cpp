#include <bits/stdc++.h>
using namespace std;

const int mod = 1e6;

int main() {
	freopen("inp.txt", "w", stdout);
	srand(unsigned(time(0)));
	int n = 20;
	printf("%d\n", n);
	int cnt[2] = {0, 0};
	for(int i=1; i<=n; ++i) {
		int type = rand() % 2;
		cnt[type] += 1;
		if (cnt[0] > cnt[1]) {
			//
		}
		else {
			cnt[0] += 1;
			cnt[1] -= 1;
			type = 0;
		}
		printf("%d ", type + 1);
		if (type == 0) {
			int val = rand() % mod + 1;
			printf("%d\n", val);
		}
		else {
			printf("\n");
		}
	}
	return 0;
}