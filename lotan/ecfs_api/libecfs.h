#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <elf.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <errno.h>
#include <link.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <time.h>
#include <locale.h>
#include <signal.h>
#include <sys/user.h>
#include <sys/procfs.h>         /* struct elf_prstatus */
#include <sys/resource.h>

typedef struct ecfs_elf {
         uint8_t *mem;          /* raw memory pointer */
         char *shstrtab;        /* shdr string table */
         char *strtab;          /* .symtab string table */
         char *dynstr;          /* .dynstr string table */
         ElfW(Ehdr) * ehdr;     /* ELF Header pointer */
         ElfW(Phdr) * phdr;     /* Program header table pointer */
         ElfW(Shdr) * shdr;     /* Section header table pointer */
         ElfW(Nhdr) * nhdr;     /* ELF Notes section pointer */
         ElfW(Dyn)  *dyn;       /* Dynamic segment pointer */
         ElfW(Sym)  *symtab;    /* Pointer to array of symtab symbol structs */
         ElfW(Sym)  *dynsym;    /* Pointer to array of dynsym symbol structs */
         ElfW(Addr) textVaddr;  /* Text segment virtual address */
         ElfW(Addr) dataVaddr;  /* data segment virtual address */
         ElfW(Addr) dynVaddr;   /* dynamic segment virtual address */
         size_t filesize;       /* total file size              */
         size_t dataSize;       /* p_memsz of data segment      */
         size_t textSize;       /* p_memsz of text segment      */
         size_t dynSize;        /* p_memsz of dynamnic segment  */
         int fd;                /* A copy of the file descriptor to the file */
} ecfs_elf_t;

#define MAX_SYM_LEN 255

typedef struct ecfs_sym {
	ElfW(Addr) symval; /* Symbol value (address/offset) */
        size_t size;       /* size of object/function       */
        uint8_t type;      /* symbol type, i.e STT_FUNC, STT_OBJECT */
        uint8_t binding;   /* symbol bind, i.e STB_GLOBAL, STB_LOCAL */
        char *strtab; /* pointer to the symbols associated string table */
        int nameoffset;    /* Offset of symbol name into symbol strtab */
} ecfs_sym_t;

