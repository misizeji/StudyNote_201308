#!/bin/bash
name=teacher
string1="good morning $name"
string2='good morning $name'
echo $string1
echo $string2

echo "today is `date`"
echo 'today is `date`'


echo -e "nihao\nnihao\tnihao\n\rnihao"
( name=student;echo "1 $name" )
echo 2:$name 
{ name=student; echo "3 $name"; }
echo 4:$name 



