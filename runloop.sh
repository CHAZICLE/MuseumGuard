#!/usr/bin/env bash

while true; do
	reset
	make && gdb -x ./gdbrun
	read
done
