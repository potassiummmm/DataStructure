#!/bin/bash
int=1
for file in *.cpp
do
	mv $file $int
    let "int++"
done
