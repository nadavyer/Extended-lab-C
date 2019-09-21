# All Targets
bcmp: clean task1a

task1a: task1a.o
	gcc -m32 -g -Wall -o bcmp task1a.o

task1a.o: task1a.c
	gcc -g -Wall -m32 -ansi -c -o task1a.o task1a.c 



#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o bcmp
