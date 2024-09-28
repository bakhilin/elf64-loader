#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include "elf_file.h"
#include "elf_valid.h"

void exit_program(int code, int fd){
    close(fd);
    _exit(code);
}

int main( int argc, char** argv ) {
    int fd = open(argv[1], O_RDONLY);

    if (fd <= 0) { exit_program(ENOENT, fd);}

    Elf64_Ehdr elf_header;
    Elf64_Off header_offset;

    Elf64_Off sec_offset;
    Elf64_Shdr sec_header;

    elf_header = read_header(fd);

    if (!header_checker(&elf_header)){ exit_program(EINVAL, fd);}

    header_offset = elf_header.e_shoff + (elf_header.e_shentsize * elf_header.e_shstrndx);

    sec_offset = find_section_header_by_name(&elf_header, argv[2], read_section_header(fd, header_offset).sh_offset, fd);
    sec_header = read_section_header(fd, sec_offset);
    
    if (!(SHF_EXECINSTR & sec_header.sh_flags)) 
    { 
        close(fd);
        exit_program(EINVAL, fd);
    }

    load_segment(fd, &elf_header);

    typedef void (*section_start)(void);
    section_start start = (section_start)sec_header.sh_addr; // NOLINT
    start();

    close(fd);

    return 0;
}
