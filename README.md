Assignment 4: SimpleSmartLoader
This project implements a user-level demand paging ELF loader written in C. The loader replaces the traditional eager loading mechanism with a lazy, page-fault–driven strategy, similar to modern operating systems. Instead of mapping all ELF segments immediately, the loader maps and populates memory only when the running program touches an unmapped page, generating a SIGSEGV. The loader is implemented entirely in a single C file and executes an ELF executable without using the OS loader. The system sets up a custom signal handler to catch segmentation faults and handles them as page-fault events.

•Error Checking Functions:
1. check_fd(int fd):
Ensures the ELF file opened correctly. If not, cleanup and exit.
2. check_size(off_t size_fd):
Makes sure lseek() returned a valid file size.
3. check_fdread(ssize_t fd_read, off_t size_fd):
Verifies that the full ELF file was read into memory.
4. check_elf_magic_bytes(Elf32_Ehdr *ehdr):
Validates the ELF magic bytes and confirms the file is an executable.
5. check_mmap(void* segment):
Checks if mmap() successfully allocated a page.

•Loader & Execution Functions:
6. loader_cleanup():
Closes the ELF file and frees heap_memory.
7. load_and_run_elf(char** exe):
Main loader routine:
  • Opens the ELF
  • Loads it into memory
  • Sets up the SIGSEGV handler
  • Jumps to entry point
  • Prints statistics after the program finishes
8. main():
Validates input, calls the loader, performs final cleanup.

•Demand Paging Helper Functions:
9. findSegment(void* SIGSEV_addr):
Returns the ELF segment containing the faulting address.
If no segment matches, it’s an actual segmentation fault.
10. copyBytes(uint32_t segmentPage, uint32_t filesz):
Calculates how many bytes from the ELF file need to be copied into the current page.
Handles partial pages and .bss regions (returns 0 for .bss).

•Page Fault Handler:
11. SIGSEV_handler(int sig, siginfo_t *si, void *ucontext):
Custom SIGSEGV handler that simulates demand paging:
  • Identifies which segment the fault occurred in
  • Computes the page-aligned address
  • Allocates a 4 KB page with mmap()
  • Copies required bytes from the ELF into the page
  • Updates statistics
12. static long BssSize():
This function calculates the total size (in bytes) of all the .bss sections in the program.
The .bss section is  where global variables that are initialized to 0 (like int arr[100] = {0};) are stored. To save space, these zeros are not saved in the ELF file itself. However, the program still needs real memory for them when it runs.
This function finds the .bss size by looping through all loadable segments and adding up the difference between their size in memory (p_memsz) and their size in the file (p_filesz).
13. static void loaderINFO():
This function calculates and prints the final statistics report after the program finishes running.
  It prints four key pieces of information:
  1. Total Page Faults: How many times the program tried to access an unmapped memory page, forcing our SIGSEV_handler to run.
  2. Total Page Allocations: The total number of 4KB pages our loader had to allocate.
  3. Internal Fragmentation (Total): This calculation INCLUDES the .bss pages as part of the fragmentation, because no data was copied into them.
  o Formula: (Total Allocated) - (Total Bytes Copied)
  4. Internal Fragmentation (Net, excluding BSS): This calculation EXCLUDES the .bss pages, giving a "net" or "true waste" value.
  o Formula: (Total Allocated) - (Total Bytes Copied) - (Total BSS Size)

CONTRIBUTIONS:
HANSIKA SINGH (2024225):
• Error Checking Functions: check_fd(int fd),check_size(off_t size_fd), check_fdread(ssize_t fd_read, off_t size_fd), check_elf_magic_bytes(Elf32_Ehdr *ehdr), check_mmap(void* segment)
• Page Fault handler : SIGSEV_handler(int sig, siginfo_t *si, void *ucontext)
• static void loaderINFO()
Anmol Saluja (2024087):
• Demand paging helper functions : findSegment(void* SIGSEV_addr), copyBytes(uint32_t segmentPage, uint32_t filesz)
• Loader functions: loader_cleanup, load_and_run_elf(char** exe), main()
• static long BssSize()
