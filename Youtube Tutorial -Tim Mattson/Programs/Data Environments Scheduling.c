#include <stdio.h>
#include <omp.h>

#define MAX 100000

int a[MAX];
int b[MAX];

int main() {
	int i;
	for(i=0; i<MAX; ++i) {
		a[i] = i;
	}
	int temp = 0;
	// example of first private
	// #pragma omp parallel for firstprivate(temp)
	// for(i=0; i<MAX; ++i) {
	// 	if (a[i]%2==1) {
	// 		temp += 1;
	// 	}
	// 	b[i] = temp;
	// }
	/*
	By default schedule is static, so every thread gets exaclty same amount of numbers 
	almost in round robin fashion. So temp is always displayed as floor(n/4).

	If we set schedule to dynamic, we get differnt answers for temp everytime we execute it
	This is because every thread has a wait queue associated with it and gets random indices
	from the array and thus final temp value is not fixed.

	We can also check here the best behaviour decided by compiler using auto in schedule.
	If on average most of the times it gives output as floor(n/4) or close to that, it means
	it tries to almost equally divide the work among the threads.

	In guided it is sure we will get answer very close to floor(n/4) everytime as chunk size 
	decreases with each iteration which is not the case with dynamic.
	*/
	#pragma omp parallel for firstprivate(temp) lastprivate(temp) schedule(guided)
	for(i=0; i<MAX; ++i) {
		if (a[i]%2==1) {
			temp += 1;
		}
		b[i] = temp;
	}
	printf("There are %d odd numbers.\n", temp);
}