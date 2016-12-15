#!/bin/bash
while true
do
	cat /proc/modlist
	echo cleanup > /proc/modlist
	sleep 1
	reset
done

