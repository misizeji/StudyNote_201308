#!/bin/bash
num=10
i=$num

echo $num
echo $i

unset num
echo $num


echo "This is the var script"
name="sunplus edu"
echo "$name is me"
echo '$name is me'

echo "看看他们有什么不同"

echo "please input a string "
read string
echo "the string of your input is $string"

readonly var=1000
#var=200

export public=300


