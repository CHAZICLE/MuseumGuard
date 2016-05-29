#!/usr/bin/env zsh

if [ $# -gt 0 ]; then
	runloop_target=$1
fi

tmux rename-window "Run"

while true; do
	clear
	make
	MAKE_RC=$?
	if [ -n "$runloop_target" ]; then
		[ $MAKE_RC -eq 0 ] && rsync assets.gz ./Three charles@sandybridge:~/in/ && ssh -t "$runloop_target" 'cd ~/in; DISPLAY=:0.0 gdb -q --eval-command=run --eval-command=quit ./Three'
	else
		[ $MAKE_RC -eq 0 ] && primusrun gdb -q --eval-command=run --eval-command=quit ./Three
	fi
	read
done
