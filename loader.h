
#ifndef LOADER_H
#define LOADER_H

// Standard C Libraries
#include <stdio.h>      // For printf
#include <stdlib.h>     // For malloc, free, exit
#include <string.h>    

// System Call Libraries
#include <fcntl.h>      // For open() and file flags like O_RDONLY
#include <unistd.h>     // For read(), lseek(), close(), write()
#include <sys/mman.h>   // For mmap()
#include <sys/types.h>  // For off_t, size_t
#include <sys/stat.h>   

// ELF File Header
#include <elf.h>        // For Elf32_Ehdr, Elf32_Phdr, and ELF constants

// Signal Handling
#include <signal.h>     // For sigaction(), siginfo_t, SIGSEGV

// Precise Integer Types
#include <stdint.h>     // For uintptr_t, uint32_t

// Function prototypes from loader.c (except main and static functions)
void load_and_run_elf(char** exe);
void loader_cleanup();
void check_fd(int fd);
void check_size(off_t size_fd);
void check_fdread(ssize_t fd_read, off_t size_fd);
void check_elf_magic_bytes(Elf32_Ehdr *ehdr);
void check_mmap(void *segment);

#endif // LOADER_H

