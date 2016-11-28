#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <ctype.h>
#include <stdlib.h>
 
using namespace std;

const int mod = 50000;

int main(int argc, char const *argv[]) {
	srand(unsigned(time(NULL)));
	int operations = atoi(argv[1]);
	printf("%d\n", operations);
	for(int i = 1; i <= operations; ++i) {
		int num = rand() % mod + 1;
		cout << num << "\n";
	}
	return 0;
}