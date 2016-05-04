#!/usr/bin/env zsh

while true; do
	reset
	make && gdb -x ./gdbrun
	read
done
