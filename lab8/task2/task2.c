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

    myMap = mmap (myMap, fileLen, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    Elf32_Ehdr * elfHeader;
    elfHeader = (Elf32_Ehdr*) myMap;
    if(elfHeader->e_ident[EI_CLASS] != 1){
        perror("the file is not ELF 32 bit\n");
        munmap(myMap ,fileLen);
        close(fd);
        exit(EXIT_FAILURE);
    }

    int totalSections = elfHeader->e_shnum;
    int i;
    Elf32_Shdr* sectionList = (myMap + elfHeader-> e_shoff);
    int offsetStrings = sectionList[elfHeader->e_shstrndx].sh_offset;

    for (i = 0; i < totalSections; i++){
        Elf32_Shdr section = sectionList[i];
        if (section.sh_type == SHT_SYMTAB || section.sh_type == SHT_DYNSYM) {
            Elf32_Sym* symbolList = (myMap + section.sh_offset);
            int totalSymbols = section.sh_size / sizeof(Elf32_Sym);
            int symbolIndex;
            char* sectionName = (myMap + offsetStrings + section.sh_name);
            int symbolStrings = sectionList[section.sh_link].sh_offset;
            printf ("\nSymbol table '%s' contains %d entries:\n", sectionName, totalSymbols);
            printf ("%s\t%s\t%s\t%-12s\t%-12s\n" , "Num:","Value","Section_index", "Section_name", "Symbol_name");
            for (symbolIndex = 0; symbolIndex < totalSymbols; symbolIndex++) {
                Elf32_Sym symbol = symbolList[symbolIndex];
                char * symbolName = (myMap + symbolStrings + symbol.st_name);
                printf ("%d:\t%08x\t%d\t%-12s\t%-12s\n" , symbolIndex,symbol.st_value, i, sectionName, symbolName);
            }
        }
    }

    munmap(myMap ,fileLen);
    close(fd);
    return 0;
}
