#!/bin/bash

if [ $# -eq 2 ]; then
	for i in $(seq $1 $2); do
		if [ $(( i % 3 )) -eq 0 ]; then 
			echo 'FOO'
		elif [ $(( i % 5 )) -eq 0 ]; then 
			echo 'BAR'
		else echo $i
	fi
	done
else
	echo "Please enter only 2 args"
fi
