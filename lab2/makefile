# All Targets
menu: clean task2a task2b  task3

task2a: task2a.o
	gcc -m32 -g -Wall -o menu task2a.o

task2a.o: task2a.c
	gcc -g -Wall -m32 -ansi -c -o task2a.o task2a.c 

task2b: task2b.o
	gcc -m32 -g -Wall -o menu task2b.o

task2b.o: task2b.c
	gcc -g -Wall -m32 -ansi -c -o task2b.o task2b.c


task3: task3.o
	gcc -m32 -g -Wall -o menu task3.o

task3.o: task3.c
	gcc -g -Wall -m32 -ansi -c -o task3.o task3.c 	

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o menu
