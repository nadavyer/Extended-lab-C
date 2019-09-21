# All Targets
char2ascii: clean task0

task0: task0.o
	gcc -m32 -g -Wall -o char2ascii task0.o

task0.o: task0.c
	gcc -g -Wall -m32 -ansi -c -o task0.o task0.c 



#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o char2ascii
