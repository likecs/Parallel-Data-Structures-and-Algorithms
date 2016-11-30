#!/bin/bash
g++ -std=c++11 -g -fopenmp -Wall -Wextra -Wpedantic -Wno-sign-compare -Wno-unused-result -Wno-unused-but-set-variable ./Sequential\ Skip\ List/skip_list.cpp -o ./Sequential\ Skip\ List/skip_list
g++ -std=c++11 -g -fopenmp -Wall -Wextra -Wpedantic -Wno-sign-compare -Wno-unused-result -Wno-unused-but-set-variable ./Lazy\ Skip\ List/skip_list.cpp -o ./Lazy\ Skip\ List/skip_list
g++ -std=c++11 ./Generators/test_generator.cpp -o ./Generators/test_generator
counter=$1
correct_sequential=0
correct_lazy=0
i=1

while [ $counter -gt 0 ]; do
	./Generators/test_generator > ./Generators/inp$2/$i'.in' $2
	
	timeout 2s ./Sequential\ Skip\ List/skip_list < ./Generators/inp$2/$i'.in' > ./Sequential\ Skip\ List/out$2/$i'.out'
	if [[ $? -ne 0 ]]; then
		echo error
	else
		correct_sequential=`expr $correct_sequential + 1`
	fi

	timeout 2s ./Lazy\ Skip\ List/skip_list < ./Generators/inp$2/$i'.in' > ./Lazy\ Skip\ List/out$2/$i'.out'
	if [[ $? -ne 0 ]]; then
		echo error
	else
		correct_lazy=`expr $correct_lazy + 1`
	fi

	counter=`expr $counter - 1`
	i=`expr $i + 1`
done

echo $correct_sequential
echo $correct_lazy