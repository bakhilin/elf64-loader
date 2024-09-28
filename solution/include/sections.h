#ifndef SECTIONS_H
#define SECTIONS_H

#include <elf.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

Elf64_Shdr read_section_header(int fd, Elf64_Off offset);
Elf64_Off find_section_header_by_name(Elf64_Ehdr *header, char *name, Elf64_Off strtab, int32_t fd);

#endif