#!/bin/bash

declare -i s=0
declare -i i=0
#while [ "$i" != "101" ]
while [ $i -le 100 ]
do
	s=s+i;
	i=i+1;
done
echo "sum=$s"

