# All Targets
bcmp: clean task1b

task1b: task1b.o
	gcc -m32 -g -Wall -o bcmp task1b.o

task1b.o: task1b.c
	gcc -g -Wall -m32 -ansi -c -o task1b.o task1b.c 



#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o bcmp
