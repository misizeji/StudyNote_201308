#!/bin/bash
if [ ! -e ~/test ]
then
	echo "press y/n to creat a test"
	read choice
	if [ "$choice" = "y" ];then
		mkdir ~/test
		cd ~/test
	elif [ "$choice" = "n" ];then
		exit 1
	fi
elif [ -d ~/test ]
then
	cd ~/test
	pwd
fi
	for (( i=1; i<=3; i=i+1 ))
	do
		echo "please input a string:"
		read string
		if [ -e $string ];then
			echo "It's wrong,please input again"
		else
			touch $string
			break
		fi
	done

	


	


