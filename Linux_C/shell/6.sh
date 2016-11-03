#!/bin/bash
echo "please input y/n!"
read yn
if [ $yn = "y" ]
then
	echo "you input is right"
elif [ $yn = "n"]
then
	echo "you input is wrong" 
else 
	echo "input wrong"
fi
