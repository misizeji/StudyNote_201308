#!/bin/bash
echo "Please input 1 2 3"
case "$1" in
	"one")
		echo "your choice is one!"
		;;
	"two")
		echo "your choice is two!"
		;;
	"three")
		echo "your choice is three!"
		;;
	*)
		echo "input wrong"
		exit 1
		;;
esac

