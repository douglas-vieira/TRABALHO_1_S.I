prog: prog.c prog.h
	gcc -ansi -Wall -g -std=c99 prog.c prog.h -o prog -lm