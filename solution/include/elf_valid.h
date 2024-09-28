#ifndef VALID_H
#define VALID_H

#include <stdint.h>
#include <elf.h>
#include <stddef.h>
#include <stdbool.h>

#define ARGS_HEADER 4

int header_checker(Elf64_Ehdr * header);


#endif