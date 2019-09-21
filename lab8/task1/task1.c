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
    if (fd == -1) {
        perror("open");
    }

    int fileLen = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    myMap = mmap (myMap, fileLen, PROT_READ, MAP_SHARED, fd, 0);
    Elf32_Ehdr * elfHeader;
    elfHeader = myMap;

    if(elfHeader->e_ident[EI_CLASS] != 1){
	perror("the file is not ELF 32 bit\n");
	munmap(myMap ,fileLen);
	close(fd);
	return 0;
    }

    int totalSections = elfHeader->e_shnum;
    int i;

    Elf32_Shdr* sectionList = (myMap + (elfHeader-> e_shoff)); /* Section header table file offset */
    /* e_shstrndx Section header string table index */
    int offsetStrings = sectionList[elfHeader->e_shstrndx].sh_offset; /* sh_offset Section file offset */
    
    printf("\n%s\t%s\t\t%s\t%s\t%s\n", "[Nr]", "Name", "Addr", "Offset", "size");
    for (i = 0; i < totalSections; i++){
        Elf32_Shdr section = sectionList[i];
        char * sectionName = (myMap + offsetStrings + section.sh_name);
        printf ("[%d]\t%-12s\t%08x %06x\t%06x\n" , i, sectionName, section.sh_addr, section.sh_offset, section.sh_size);
    }
    printf("\n");
    munmap(myMap ,fileLen);
    close(fd);
    return 0;
}
