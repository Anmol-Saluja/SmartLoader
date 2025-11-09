#include "loader.h"

Elf32_Ehdr *ehdr;
Elf32_Phdr *phdr;
int fd;
char *heap_memory;
#define PAGE_SIZE 4096

long faultCount = 0;
long allocationCount = 0;
long bytes_copied = 0; 

void check_fd(int fd){
  if (fd<0){    // the function to check if file descriptor was opened successfully.
      printf("Failed to open file"); 
      loader_cleanup();
      exit(1);
  }
}

void check_size(off_t size_fd){
    if(size_fd<0){  // the function to check if file size was found.
        printf("Failed to determine the size of the ELF");
        loader_cleanup();
        exit(1);
    }
}

void check_fdread(ssize_t fd_read,off_t size_fd){
  if (fd_read<0 || (size_t)fd_read != (size_t)size_fd){
      printf("Could not read from file"); // the function to check if binary content was successfully loaded into heap memory.
      loader_cleanup();
      exit(1);
  }
}

void check_elf_magic_bytes(Elf32_Ehdr *ehdr){
  if (!(ehdr->e_ident[0]==0x7f &&
        ehdr->e_ident[1]=='E'  &&    // the function to check whether ELF file is of type ET_EXEC (executable).
        ehdr->e_ident[2]=='L'  &&
        ehdr->e_ident[3]=='F') || ehdr->e_type != ET_EXEC) {
      printf("Not a valid ELF file (executable file)\n");
      loader_cleanup();
      exit(1);
  }
}

void check_mmap(void *segment) {
    if (segment==MAP_FAILED) { // the function to check if the mmap mapping was successful.
      printf("mapping of segment failed");
      loader_cleanup();
      exit(1);
    }
}

// the function to find the address in which the address causing the segmentation fault lies
static Elf32_Phdr* findSegment(void* SIGSEV_addr) {
    int num_ph = ehdr->e_phnum;
    for (int i = 0; i < num_ph; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            if ((uint32_t)SIGSEV_addr >= phdr[i].p_vaddr && 
                (uint32_t)SIGSEV_addr < (phdr[i].p_vaddr + phdr[i].p_memsz)) {
                
                return &phdr[i]; // Found the segment
            }
        }
    }
    return NULL; // Not in any segment
}

// the function to find actually how any bytes we have to copy from the elf file to the allocated page.
static uint32_t copyBytes(uint32_t segmentPage, uint32_t segment_file_size) {
    if (segmentPage >= segment_file_size) {
        return 0;
    }
    if (segmentPage + PAGE_SIZE > segment_file_size) {
        return segment_file_size - segmentPage;
    }
    return PAGE_SIZE;
}

// the function to calculate bss section size 
static long BssSize() {
    long total_bss_size = 0;
    int num_ph = ehdr->e_phnum;

    for (int i = 0; i < num_ph; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            if (phdr[i].p_memsz > phdr[i].p_filesz) {
                total_bss_size += (phdr[i].p_memsz - phdr[i].p_filesz);
            }
        }
    }
    return total_bss_size;
}

void loader_cleanup() {
    if (fd>0) {
        close(fd);
    }
    if(!heap_memory){
        return;     // the function to close executable file and free heap memory.
    }else{
        free(heap_memory);
    }
}
