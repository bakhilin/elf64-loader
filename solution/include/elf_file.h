#ifndef ELF_FILE_H
#define ELF_FILE_H

#include <elf.h>
#include <stdbool.h>
#include <stddef.h>

Elf64_Ehdr read_header(int fd);
Elf64_Shdr read_section_header(int fd, Elf64_Off offset);

int load_segment(int fd, Elf64_Ehdr *header);

Elf64_Off elf_name_table_header_offset(Elf64_Ehdr *header);

Elf64_Off find_section_header_by_name(Elf64_Ehdr *header, char *name, Elf64_Off strtab, int fd);


#endif
