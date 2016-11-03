#!/bin/bash

declare -i s=0
declare -i i=0

until [ "$i" = "101" ]
do
	s+=i;
	i=i+1;
done
echo "The count is $s"
