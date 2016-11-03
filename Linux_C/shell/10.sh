#!/bin/bash

for name in `ls`
do
	if [ -f $name ];then
		echo "$name is file "
	elif [ -d $name ];then
		echo "$name is directory"
	else
		echo "^_^"
	fi
done
