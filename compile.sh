#!/bin/bash

basepath=$(cd `dirname $0`; pwd)


if (which g++); then
	for dir in `ls $basepath`
	do
		if [[ -d $dir ]]; then
			cd $dir
			g++ -std=c++14 ${dir}.cpp -o ${dir} 
			cd ..
		fi
	done
fi

