/* File: elf.h
 * Author: Joshua Jacobs-Rebhun
 * Date: November 20, 2023
 * 
 * 
 * This file holds a number of definitions and types that are 
 * necessary to parse and dump the Executable and Linking Format (ELF)
 * object file format. It contains structures and types to dump both
 * 32-bit and 64-bit ELF files.
 */


#ifndef ELF_H
#define ELF_H

#include <stdint.h>



typedef uint8_t BYTE



/***************************
 * 32-bit type definitions *
 ***************************/



/* 
 * All types defined here have the expected alignment properties.
 * Thus, a 32-bit (4 byte) integer will naturally be aligned on a
 * 4 byte boundary. This means that data structures will not
 * necessarily be packed together in the file and will contain some
 * natural spacing so that each data type will have proper alignment
 */


/* 32-bit unsigned address */
typedef uint32_t ELF32_Addr_t;


/* unsigned 2-byte data type */
typedef uint16_t ELF32_Half_t;


/* unsigned 32-bit file offset */
typedef uint32_t ELF32_Off_t;


typedef int32_t ELF32_Sword_t;

typedef uint32_t ELF32_Word_t;


// size of initial file bytes
#define EI_NIDENT 16



typedef struct HEADER
{
    /* 
     * Initial bytes mark the file as an object file
     * and provide information that will help with
     * decoding the file.
     */
    unsigned char e_ident[EI_NIDENT];


    /*
     * Identifies the type of the object file, for
     * example, executable, relocatable, etc.
     */
    ELF32_Half_t e_type;


    /*
     * Identifies the processor architecture
     */
    ELF32_Half_t e_machine;


    /*
     * Identifies the version of the file.
     */
    ELF32_Word_t e_version;

    
    /*
     * Gives the virtual address to which the system first
     * transfers control when running the process. This is
     * called the entry point of the program. If the file
     * has no entry point then this is 0.
     */
    ELF32_Addr_t e_entry;


    /*
     * Holds the offset of the program header table into the
     * file. If the file has no program headers then this
     * field is 0.
     */
    ELF32_Off_t e_phoff;


    /*
     * Holds the offset of the section header table into the
     * file. If the file has no section headers then this
     * field is 0.
     */
    ELF32_Off_t e_shoff;


    /*
     * Holds processor specific flags associated with the file.
     */
    ELF32_Word_t e_flags;


    /*
     * This is the ELF header's size in bytes.
     */
    ELF32_Half_t e_ehsize;


    /*
     * This holds the size, in bytes, of one entry in
     * the file's program header table. All entries
     * are the same size.
     */
    ELF32_Half_t e_phentsize;


    /*
     * Total number of program headers in the program
     * header table. This multiplied by e_phentsize
     * gives the total size, in bytes, of the file's
     * program header table. If there are no program
     * headers, then this field is 0.
     */
    ELF32_Half_t e_phnum;


    /*
     * This is the size, in bytes, of a single entry
     * in the file's section header table. All entries
     * are the same size.
     */
    ELF32_Half_t e_shentsize;


    /*
     * Total number of section headers in the section
     * header table. This multiplied by e_phentsize
     * gives the total size, in bytes, of the file's
     * section header table. If there are no section
     * headers, then this field is 0.
     */
    ELF32_Half_t e_shnum;


    /*
     * This holds the index of the section header table
     * entry associated with the section name string
     * table.
     */
    ELF32_Half_t e_shstrndx;



} ELF32_Header_t;




/***************************
 * 64-bit type definitions *
 ***************************/



/* 
 * All types defined here have the expected alignment properties.
 * Thus, a 32-bit (4 byte) integer will naturally be aligned on a
 * 4 byte boundary. This means that data structures will not
 * necessarily be packed together in the file and will contain some
 * natural spacing so that each data type will have proper alignment
 */


/* 32-bit unsigned address */
typedef uint64_t ELF64_Addr_t;


/* unsigned 2-byte data type */
typedef uint16_t ELF64_Half_t;


/* unsigned 32-bit file offset */
typedef uint64_t ELF64_Off_t;


typedef int32_t ELF64_Sword_t;

typedef uint32_t ELF64_Word_t;


/* ELF 64 has extra 64-bit words as well */
typedef uint64_t ELF64_Xword_t;

typedef int64_t ELF64_Sxword_t;

// size of initial file bytes
#define EI_NIDENT 16



typedef struct HEADER
{
    /* 
     * Initial bytes mark the file as an object file
     * and provide information that will help with
     * decoding the file.
     */
    unsigned char e_ident[EI_NIDENT];


    /*
     * Identifies the type of the object file, for
     * example, executable, relocatable, etc.
     */
    ELF64_Half_t e_type;


    /*
     * Identifies the processor architecture
     */
    ELF64_Half_t e_machine;


    /*
     * Identifies the version of the file.
     */
    ELF64_Word_t e_version;

    
    /*
     * Gives the virtual address to which the system first
     * transfers control when running the process. This is
     * called the entry point of the program. If the file
     * has no entry point then this is 0.
     */
    ELF64_Addr_t e_entry;


    /*
     * Holds the offset of the program header table into the
     * file. If the file has no program headers then this
     * field is 0.
     */
    ELF64_Off_t e_phoff;


    /*
     * Holds the offset of the section header table into the
     * file. If the file has no section headers then this
     * field is 0.
     */
    ELF64_Off_t e_shoff;


    /*
     * Holds processor specific flags associated with the file.
     */
    ELF64_Word_t e_flags;


    /*
     * This is the ELF header's size in bytes.
     */
    ELF64_Half_t e_ehsize;


    /*
     * This holds the size, in bytes, of one entry in
     * the file's program header table. All entries
     * are the same size.
     */
    ELF64_Half_t e_phentsize;


    /*
     * Total number of program headers in the program
     * header table. This multiplied by e_phentsize
     * gives the total size, in bytes, of the file's
     * program header table. If there are no program
     * headers, then this field is 0.
     */
    ELF64_Half_t e_phnum;


    /*
     * This is the size, in bytes, of a single entry
     * in the file's section header table. All entries
     * are the same size.
     */
    ELF64_Half_t e_shentsize;


    /*
     * Total number of section headers in the section
     * header table. This multiplied by e_phentsize
     * gives the total size, in bytes, of the file's
     * section header table. If there are no section
     * headers, then this field is 0.
     */
    ELF64_Half_t e_shnum;


    /*
     * This holds the index of the section header table
     * entry associated with the section name string
     * table.
     */
    ELF64_Half_t e_shstrndx;



} ELF64_Header_t;


/******************************************************
 * The following definitions relate to the indices of
 * different information in the e_ident field in the
 * ELF file header. Each of the definitions is
 * explained individually.
 ******************************************************/



/*
 * EI_MAG0-3 give the indices of the "magic number" in
 * the ELF header. This number identifies the file as
 * an ELF object file.
 */
#define EI_MAG0         0
#define EI_MAG1         1
#define EI_MAG2         2
#define EI_MAG3         3


/*
 * These give the values of the data that must be at
 * the indices of e_ident given above.
 */
#define ELFMAG0         0x7f        // at e_ident[EI_MAG0]
#define ELFMAG1         'E'         // at e_ident[EI_MAG1]
#define ELFMAG2         'L'         // at e_ident[EI_MAG2]
#define ELFMAG3         'F'         // at e_ident[EI_MAG3]



/*
 * This is the index of the next byte in the e_ident
 * array.
 */
#define EI_CLASS        4


/*
 * The class of the file can either be none, 32-bit,
 * or 64-bit. Other classes may be added in the future.
 */
#define ELFCLASSNONE    0
#define ELFCLASS32      1
#define ELFCLASS64      2


/*
 * Index of the next byte in the e_ident array.
 */
#define EI_DATA         5



/*
 * e_ident[EI_DATA] identifies the data encoding of
 * all of the processor-specific data in the file.
 * ELFDATANONE obviously means there is no data
 * encoding.
 * 
 * ELFDATA2LSB means that in a multi-byte
 * object, the least significant byte (LSB) is stored
 * at the lowest address. This is also known as a
 * little endian byte-ordering.
 * 
 * ELFDATA2MSB means that in a multi-byte
 * object, the most significant byte (MSB) is stored
 * at the lowest address. This is also known as a
 * big endian byte-ordering.
 */
#define ELFDATANONE     0
#define ELFDATA2LSB     1
#define ELFDATA2MSB     2

/*
 * Index of version information in e_ident.
 */
#define EI_VERSION      6


/*
 * Possible version information stored in
 * both e_ident[EI_VERSION] and in e_version.
 */
#define EV_NONE         0
#define EV_CURRENT      1


/*
 * Index into e_ident of the ABI used in
 * the file.
 */
#define EI_OSABI        7


/*
 * Possible values of the OS/ABI information.
 */
#define ELFOSABI_SYSV           0       // System V ABI
#define ELFOSABI_HPUX           1       // HP-UX operating system
#define ELFOSABI_STANDALONE     255     // Standalone (embedded) application


/* ABI version not really used except in unusual circumstances */
#define EI_ABIVERSION   8

/* Beginning of padding bytes (set to 0) */
#define EI_PAD          9



/*
 * Possible values taken they the e_type field.
 */
#define ET_NONE         0           // no file type
#define ET_REL          1           // relocatable object file
#define ET_EXEC         2           // executable object file
#define ET_DYN          3           // dynamically-linker shared object file
#define ET_CORE         4           // core file (created during core dump)
#define ET_LOPROC       0xff00      // processor-specific
#define ET_HIPROC       0xffff      // processor-specific


/*
 * Possible values of the e_machine field identifying
 * the required architecture for the file.
 */
#define EM_NONE         0           // No machine architecture
#define EM_M32          1           // AT&T WE 32100
#define EM_SPARC        2           // SPARC
#define EM_386          3           // Intel Architecture
#define EM_68K          4           // Motorola 68000
#define EM_88K          5           // Motorola 88000
#define EM_860          7           // Intel 80860
#define EM_MIPS         8           // MIPS RS3000 Big-Endian
#define EM_MIPS_RS4_BE  10          // MIPS RS4000 Big-Endian




/*
 * The following are reserved indices in any ELF file's
 * section header table. Technically, the entry at index
 * 0 (SHN_UNDEF) is not undefined.
 */
#define SHN_UNDEF       0           // Undefined section header
#define SHN_LORESERVE   0xff00      // Lower bound on the range of reserved indices
#define SHN_LOPROC      0xff00      // Beginning of the range of indices reserved for processor-specific semantics
#define SHN_HIPROC      0xff1f      // End of the processor-specific range
#define SHN_ABS         0xfff1      // Defines absolute values for the corresponding reference.
#define SHN_COMMON      0xfff2      // Common symbols
#define SHN_HIRESERVE   0xffff      // Upper bound on the range of reserved indices




/***************************************************
 * Section header definitions for 32-bit ELF files *
 ***************************************************/


typedef struct
{

    /*
     * Gives the name of the section. The value is
     * an index into the section header string
     * table section, which stores all of the strings
     * in the file.
     */
    ELF32_Word_t sh_name;


    /*
     * Specifies the type, content, and semantics
     * of the section.
     */
    ELF32_Word_t sh_type;


    /*
     * Sections can have 1-bit flags that describe
     * miscellaneous attributes. The flag types and
     * meanings are described in other parts of this
     * file.
     */
    ELF32_Word_t sh_flags;


    /*
     * If this section is to appear in the process
     * image, then this member specifies the starting
     * address of the section in process memory. If
     * this section will not appear in memory, for 
     * example in relocatable object files where none
     * of the sections are to appear in memory.
     */
    ELF32_Addr_t sh_addr;


    /*
     * This member gives the byte offset from the 
     * beginning of the file to the first byte of
     * the section. For the section type SH_NOBITS,
     * described elsewhere in the file, the size of
     * the section is 0 and this simply marks the
     * logical placement of the section in the file.
     */
    ELF32_Off_t sh_offset;


    /*
     * This gives the size, in bytes, of the section
     * in the file, unless the section is of type
     * SH_NOBITS in which case sh_size may be non-zero
     * but the section nevertheless occupies no space
     * in the file.
     */
    ELF32_Word_t sh_size;


    /*
     * This gives a section header table index link.
     * The interpretation of this member depends on
     * the type of the section.
     */
    ELF32_Word_t sh_link;


    /*
     * Holds extra information about the section.
     * Meaning depends on the section type.
     */
    ELF32_Word_t sh_info;


    /*
     * Depending on the section, it may have an 
     * address alignment constraint, which is given
     * by this member. If this member is set, the sh_addr
     * member must be congruent to 0 modulo sh_addralign.
     * Currently, only positive integer powers of two are
     * allowed (and 0). A value of 1 is considered to be
     * identical to 0.
     */
    ELF32_Word_t sh_addralign;


    /*
     * Some sections contain a table of fixed-size entries,
     * such as a symbol table. If this is the case, then this
     * member gives the size, in bytes, of a single member
     * of the table. If it is 0, the section does not contain
     * such a table.
     */
    ELF32_Word_t sh_entsize;



} ELF32_Section_Header_t;




/***************************************************
 * Section header definitions for 64-bit ELF files *
 ***************************************************/


typedef struct
{

    /*
     * Gives the name of the section. The value is
     * an index into the section header string
     * table section, which stores all of the strings
     * in the file.
     */
    ELF64_Word_t sh_name;


    /*
     * Specifies the type, content, and semantics
     * of the section.
     */
    ELF64_Word_t sh_type;


    /*
     * Sections can have 1-bit flags that describe
     * miscellaneous attributes. The flag types and
     * meanings are described in other parts of this
     * file.
     */
    ELF64_Xword_t sh_flags;


    /*
     * If this section is to appear in the process
     * image, then this member specifies the starting
     * address of the section in process memory. If
     * this section will not appear in memory, for 
     * example in relocatable object files where none
     * of the sections are to appear in memory.
     */
    ELF64_Addr_t sh_addr;


    /*
     * This member gives the byte offset from the 
     * beginning of the file to the first byte of
     * the section. For the section type SH_NOBITS,
     * described elsewhere in the file, the size of
     * the section is 0 and this simply marks the
     * logical placement of the section in the file.
     */
    ELF64_Off_t sh_offset;


    /*
     * This gives the size, in bytes, of the section
     * in the file, unless the section is of type
     * SH_NOBITS in which case sh_size may be non-zero
     * but the section nevertheless occupies no space
     * in the file.
     */
    ELF64_Xword_t sh_size;


    /*
     * This gives a section header table index link.
     * The interpretation of this member depends on
     * the type of the section.
     */
    ELF64_Word_t sh_link;


    /*
     * Holds extra information about the section.
     * Meaning depends on the section type.
     */
    ELF64_Word_t sh_info;


    /*
     * Depending on the section, it may have an 
     * address alignment constraint, which is given
     * by this member. If this member is set, the sh_addr
     * member must be congruent to 0 modulo sh_addralign.
     * Currently, only positive integer powers of two are
     * allowed (and 0). A value of 1 is considered to be
     * identical to 0.
     */
    ELF64_Xword_t sh_addralign;


    /*
     * Some sections contain a table of fixed-size entries,
     * such as a symbol table. If this is the case, then this
     * member gives the size, in bytes, of a single member
     * of the table. If it is 0, the section does not contain
     * such a table.
     */
    ELF64_Xword_t sh_entsize;



} ELF64_Section_Header_t;




/***************************************************
 * The following section contains type definitions
 * and constants that are common to both the 32-bit
 * and 64-bit files.
 ***************************************************/




/*
 * Section types (possible values of sh_type) are defined
 * and explained below.
 */



/*
 * Marks the section header as inactive - meaning it has
 * no corresponding section. All of the other members of
 * the section header struct are undefined.
 */
#define SHT_NULL        0



#endif