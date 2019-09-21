#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){
    int fd;
    char* myMap = NULL;
    fd = open(argv[1], O_RDWR);
    int fileLen = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    myMap = mmap (myMap, fileLen, PROT_READ, MAP_SHARED, fd, 0);
    Elf32_Ehdr * elfheader;
    elfheader = (Elf32_Ehdr*) myMap;

    printf("Magic:\t\t\t\t %x,%x,%x\n",elfheader->e_ident[0], elfheader->e_ident[1] , elfheader->e_ident[2] );
    printf("Data:\t\t\t\t %d's complement, little endian\n", elfheader->e_type);
    printf("Entry point address:\t\t 0x%x\n" , elfheader->e_entry);
    printf("Start of section headers:\t %d\n" , elfheader->e_shoff);
    printf("Number of sections headers:\t %d\n" , elfheader->e_shnum);
    printf("Size of section headers:\t %d\n" , elfheader->e_shentsize);
    printf("Index of the shstrtab:\t\t %d\n", elfheader->e_shstrndx);
    
   munmap(myMap ,fileLen);
    close(fd);
    return 0;
}
