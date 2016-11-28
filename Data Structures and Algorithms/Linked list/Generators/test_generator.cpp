#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <ctype.h>
#include <stdlib.h>
 
using namespace std;

const int mod = 500;

int main(int argc, char const *argv[]) {
	srand(unsigned(time(NULL)));
	int operations = atoi(argv[1]);
	int insert_count = 0;
	int remove_count = 0;
	int type, num;
	for(int i = 1; i <= operations; ++i) {
		type = rand() % 2;
		if (type == 1) {
			insert_count += 1;
		}
		else {
			if (insert_count < remove_count) {
				type = 1;
				insert_count += 1;
			}
			else {
				type = 2;
				remove_count += 1;
			}
		}
		num = rand() % mod + 1;
		cout << type << " " << num << "\n";
	}
	cout << "-1\n";
	return 0;
}