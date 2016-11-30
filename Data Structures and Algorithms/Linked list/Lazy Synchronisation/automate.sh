#!/bin/bash
g++ -std=c++11 -g -fopenmp -Wall -Wextra -Wpedantic -Wno-sign-compare -Wno-unused-result -Wno-unused-but-set-variable linked_list.cpp -o linked_list
g++ -std=c++11 ../Generators/test_generator.cpp -o ../Generators/test_generator
counter=100
correct=0
while [ $counter -gt 0 ]; do
	timeout 2s ../Generators/test_generator > ../Generators/input_large.txt
	timeout 2s ./linked_list < ../Generators/input_large.txt > output_large.txt
	if [[ $? -ne 0 ]]; then
		echo error
	else
		correct=`expr $correct + 1`
	fi
	counter=`expr $counter - 1`
done
echo $correct