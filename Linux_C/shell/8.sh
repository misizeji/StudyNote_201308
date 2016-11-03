#!/bin/bash

echo "Please input your choice:"
read choice


case "$choice" in
	Y | yes | Yes | YES)
		echo "It's right"
		;;
	N* | n*)
		echo "It's wrong"
		;;
	*)
		exit 1
		;;
esac
