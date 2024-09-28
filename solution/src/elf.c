#include "elf_file.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>


Elf64_Ehdr read_header(int fd){
    Elf64_Ehdr header;
    size_t size_header  = sizeof(Elf64_Ehdr);
    if (fd == 0){ exit(ENOENT);}
    size_t bytes_read = read(fd, &header, size_header);
    if (bytes_read != size_header){ exit(EIO);}
    
    return header;
}


int load_segment(int fd, Elf64_Ehdr * header) {
    Elf64_Off offset;
    Elf64_Phdr ph;
    Elf64_Addr addr;
    size_t memsz;
    size_t index = 0;

    if (lseek(fd, header->e_phoff, 0) < 0) {
        close(fd);
        exit(EINVAL);
    }

    while (header->e_phnum > index) {
        if (read(fd, &ph, header->e_phentsize) < header->e_phentsize) {
            exit(EINVAL);
        }
        addr = ph.p_vaddr;
        offset = ph.p_offset;
        if (ph.p_type == PT_LOAD) {

            addr -= addr % sysconf(_SC_PAGE_SIZE);
            offset -= offset % sysconf(_SC_PAGE_SIZE);
            memsz = ph.p_memsz + (ph.p_vaddr - addr);

            if (mmap(
                (void *) addr, // NOLINT
                memsz, 
                    (ph.p_flags & PF_X ? PROT_EXEC  : 0) |
                    (ph.p_flags & PF_W ? PROT_WRITE : 0) |
                    (ph.p_flags & PF_R ? PROT_READ  : 0),
                MAP_PRIVATE | MAP_FIXED | MAP_FIXED_NOREPLACE,
                fd,
                (off_t)offset
                ) == MAP_FAILED) {
                    exit(EIO);
                }
        }
        index++;
    }
    return 1;
}

static bool names_equal(int32_t fd, const char *s2) {
    char c;
    while (read(fd, &c, 1) == 1 && c == *s2++) {
		if (c == '\0')
			return true;
    }
	return false;
}

Elf64_Off find_section_header_by_name(Elf64_Ehdr *header, char *name, Elf64_Off strtab, int32_t fd) {
    size_t index = 0;
    Elf64_Shdr sh;
    while(index < header->e_shnum) {
        if (lseek(fd, (off_t)(header->e_shoff + index * header->e_shentsize), 0) < 0 ) {
            close(fd);
            exit(EIO);
        }
        
        if (read(fd, &sh, (off_t)header->e_shentsize) < header->e_shentsize) {
            close(fd);
            exit(EIO);
        }

        if (lseek(fd, (off_t)(strtab + sh.sh_name), 0) < 0 ) {
            close(fd);
            exit(EIO);
        }

        if (names_equal(fd, name)) {
            return header->e_shoff + index * header->e_shentsize;
        }
        index++;
    }

    close(fd);
    exit(EINVAL);
}
