# All Targets
bcmp: clean task0c

task2: task0c.o
	gcc -m32 -g -Wall -o bcmp task0c.o

task2.o: task0c.c
	gcc -g -Wall -m32 -ansi -c -o task0c.o task0c.c 



#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o bcmp
