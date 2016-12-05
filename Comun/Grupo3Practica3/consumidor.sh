#!/bin/bash
while true
do
	cat /proc/modlist
	echo cleanup > /proc/modlist
	sleep 0.1
	reset
done

