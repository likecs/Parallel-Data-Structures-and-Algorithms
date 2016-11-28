#!/bin/bash
g++ -std=c++11 -g -fopenmp -Wall -Wextra -Wpedantic -Wno-sign-compare -Wno-unused-result -Wno-unused-but-set-variable ./Iterative/mergesort_sequential.cpp -o ./Iterative/mergesort_sequential
g++ -std=c++11 -g -fopenmp -Wall -Wextra -Wpedantic -Wno-sign-compare -Wno-unused-result -Wno-unused-but-set-variable ./Iterative/mergesort_parallel.cpp -o ./Iterative/mergesort_parallel
g++ -std=c++11 ./Generators/test_generator.cpp -o ./Generators/test_generator
counter=$1
correct_sequential=0
correct_parallel=0
i=1

while [ $counter -gt 0 ]; do
	./Generators/test_generator > ./Generators/inp$2/$i'.in' $2
	
	sequential=$(timeout 5s ./Iterative/mergesort_sequential < ./Generators/inp$2/$i'.in')
	echo $sequential
	if [[ $? -ne 0 ]]; then
		echo error
	else
		if [[ ${sequential:0:1} != 'A' ]]; then
			echo error
		else
			correct_sequential=`expr $correct_sequential + 1`
		fi
	fi

	parallel=$(timeout 5s ./Iterative/mergesort_parallel < ./Generators/inp$2/$i'.in')
	echo $parallel
	if [[ $? -ne 0 ]]; then
		echo error
	else
		if [[ ${parallel:0:1} != 'A' ]]; then
			echo error
		else
			correct_parallel=`expr $correct_parallel + 1`
		fi
	fi

	counter=`expr $counter - 1`
	i=`expr $i + 1`
done

echo $correct_sequential
echo $correct_parallel
