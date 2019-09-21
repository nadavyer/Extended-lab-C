# All Targets
bcmp: clean task1

task2: task1.o
	gcc -m32 -g -Wall -o bcmp task1.o

task2.o: task1.c
	gcc -g -Wall -m32 -ansi -c -o task1.o task1.c 



#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o bcmp
