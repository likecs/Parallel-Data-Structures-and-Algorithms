#!/bin/bash
g++ -std=c++11 -g -fopenmp -Wall -Wextra -Wpedantic -Wno-sign-compare -Wno-unused-result -Wno-unused-but-set-variable ./Sequential/linked_list.cpp -o ./Sequential/linked_list
g++ -std=c++11 -g -fopenmp -Wall -Wextra -Wpedantic -Wno-sign-compare -Wno-unused-result -Wno-unused-but-set-variable ./Lazy\ Synchronisation/linked_list.cpp -o ./Lazy\ Synchronisation/linked_list
g++ -std=c++11 -g -fopenmp -Wall -Wextra -Wpedantic -Wno-sign-compare -Wno-unused-result -Wno-unused-but-set-variable ./Optimistic\ Synchronisation/linked_list.cpp -o ./Optimistic\ Synchronisation/linked_list
g++ -std=c++11 ./Generators/test_generator.cpp -o ./Generators/test_generator
counter=$1
correct_sequential=0
correct_optimistic=0
correct_lazy=0
i=1

while [ $counter -gt 0 ]; do
	./Generators/test_generator > ./Generators/inp$2/$i'.in' $2
	
	timeout 2s ./Sequential/linked_list < ./Generators/inp$2/$i'.in' > ./Sequential/out$2/$i'.out'
	if [[ $? -ne 0 ]]; then
		echo error
	else
		correct_sequential=`expr $correct_sequential + 1`
	fi

	timeout 5s ./Optimistic\ Synchronisation/linked_list < ./Generators/inp$2/$i'.in' > ./Optimistic\ Synchronisation/out$2/$i'.out'
	if [[ $? -ne 0 ]]; then
		echo error
	else
		correct_optimistic=`expr $correct_optimistic + 1`
	fi

	timeout 2s ./Lazy\ Synchronisation/linked_list < ./Generators/inp$2/$i'.in' > ./Lazy\ Synchronisation/out$2/$i'.out'
	if [[ $? -ne 0 ]]; then
		echo error
	else
		correct_lazy=`expr $correct_lazy + 1`
	fi

	counter=`expr $counter - 1`
	i=`expr $i + 1`
done

echo $correct_sequential
echo $correct_optimistic
echo $correct_lazy