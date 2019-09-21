all: myshell
myshell: myshell.o
	gcc -g -m32 -Wall -o myshell myshell.o
myshell.o: myshell.c
	gcc -m32 -g -Wall -ansi -c -o myshell.o myshell.c

clean:
	rm -f *o myshell
