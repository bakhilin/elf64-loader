#include "elf_valid.h"


static bool elf64_header_validation(Elf64_Ehdr * header) {
    return header->e_ident[0] == ELFMAG0 &&
           header->e_ident[1] == ELFMAG1 &&
           header->e_ident[2] == ELFMAG2 && 
           header->e_ident[3] == ELFMAG3;        
}

static bool elf64_type_check(Elf64_Ehdr * header){
    return header->e_type != ET_DYN;
}

static bool elf64_section_tables(Elf64_Ehdr * header) {
    return header->e_shnum > 0;
}

int header_checker(Elf64_Ehdr * header){
    return elf64_header_validation(header) &&
           elf64_type_check(header) && 
           elf64_section_tables(header);
}
