#!/bin/bash

echo "Number of args $#"

# {1..$#} - doesn't work in some versions of bash

for i in $(seq 1 $#); do
	echo $1
	shift
done
