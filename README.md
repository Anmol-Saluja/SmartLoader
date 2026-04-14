# 🚀 Assignment 4: SimpleSmartLoader

## 📌 Overview

SimpleSmartLoader is a user-level **demand paging ELF loader** implemented in C.
It replaces the traditional eager loading mechanism with a **lazy, page-fault–driven strategy**, similar to modern operating systems.

Instead of loading all ELF segments at once, memory is mapped **only when accessed**, triggering a `SIGSEGV` which is then handled as a page fault.

* Implemented entirely in a single C file
* Executes ELF binaries **without using the OS loader**
* Uses a custom signal handler to simulate demand paging

---

## 🧩 Error Checking Functions

1. **`check_fd(int fd)`**
   Ensures the ELF file opened correctly. If not, cleanup and exit.

2. **`check_size(off_t size_fd)`**
   Verifies that `lseek()` returned a valid file size.

3. **`check_fdread(ssize_t fd_read, off_t size_fd)`**
   Confirms the entire ELF file was read into memory.

4. **`check_elf_magic_bytes(Elf32_Ehdr *ehdr)`**
   Validates ELF magic bytes and ensures the file is executable.

5. **`check_mmap(void* segment)`**
   Checks whether `mmap()` successfully allocated memory.

---

## ⚙️ Loader & Execution Functions

6. **`loader_cleanup()`**
   Closes the ELF file and frees allocated memory.

7. **`load_and_run_elf(char** exe)`**
   Main loader routine:

   * Opens the ELF file
   * Loads it into memory
   * Sets up the `SIGSEGV` handler
   * Transfers control to the entry point
   * Prints execution statistics

8. **`main()`**

   * Validates input
   * Calls the loader
   * Performs final cleanup

---

## 📦 Demand Paging Helper Functions

9. **`findSegment(void* SIGSEV_addr)`**

   * Identifies the ELF segment containing the faulting address
   * If no segment matches → real segmentation fault

10. **`copyBytes(uint32_t segmentPage, uint32_t filesz)`**

* Computes bytes to copy from ELF into the page
* Handles:

  * Partial pages
  * `.bss` regions (returns 0 for `.bss`)

---

## ⚡ Page Fault Handler

11. **`SIGSEV_handler(int sig, siginfo_t *si, void *ucontext)`**
    Custom handler that simulates demand paging:

* Detects faulting segment
* Aligns address to page boundary
* Allocates a 4KB page using `mmap()`
* Copies required ELF data into memory
* Updates statistics

---

## 📊 Memory & Statistics Functions

12. **`static long BssSize()`**
    Calculates total size of all `.bss` sections.

* `.bss` stores zero-initialized global variables
* Not stored in ELF → allocated at runtime

**Computation:**

* Sum of `(p_memsz - p_filesz)` across all loadable segments

---

13. **`static void loaderINFO()`**
    Prints execution statistics:

* **Total Page Faults** → number of SIGSEGV events
* **Total Page Allocations** → number of 4KB pages allocated
* **Internal Fragmentation (Total)**

  ```
  Total Allocated - Total Bytes Copied
  ```
* **Internal Fragmentation (Net, excluding BSS)**

  ```
  Total Allocated - Total Bytes Copied - Total BSS Size
  ```

---

## 🤝 Contributions

### 👩‍💻 Hansika Singh (2024225)

* Error Checking Functions:

  * `check_fd`
  * `check_size`
  * `check_fdread`
  * `check_elf_magic_bytes`
  * `check_mmap`
* Page Fault Handler:

  * `SIGSEV_handler`
* Statistics:

  * `loaderINFO`

---

### 👨‍💻 Anmol Saluja (2024087)

* Demand Paging Helpers:

  * `findSegment`
  * `copyBytes`
* Loader Functions:

  * `loader_cleanup`
  * `load_and_run_elf`
  * `main`
* Memory Analysis:

  * `BssSize`

---

## 🎯 Key Concepts Demonstrated

* Demand Paging
* ELF File Parsing
* Signal Handling (`SIGSEGV`)
* Virtual Memory Management
* Internal Fragmentation Analysis

---
