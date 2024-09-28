#include "sections.h"

Elf64_Shdr read_section_header(int fd, Elf64_Off offset) {
    Elf64_Shdr header;
    if(lseek(fd, offset, 0) < 0) {
        exit(EIO);
    }

    if (read(fd, &header, sizeof(Elf64_Shdr)) < sizeof(Elf64_Ehdr))
    {
        exit(EIO);
    }
    
    return header;
}

