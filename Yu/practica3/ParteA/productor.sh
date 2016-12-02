#!/bin/bash
i=0
while true
do
	echo add $i  > /proc/modlist
	((i=i+1))
	sleep 0.5

done
