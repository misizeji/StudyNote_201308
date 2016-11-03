#!/bin/bash
is_it_directory()
{
	if [ $# -lt 1 ];then 
		echo "I need an agreement"
		return 1
	fi
	if [ ! -d $1 ];then 
		return 2
	else
		return 3
	fi
}
echo -n "enter destination directory:"
read direct
is_it_directory $direct
echo "the result is $?"
