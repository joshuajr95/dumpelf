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



typedef uint8_t BYTE;



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



typedef struct
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



typedef struct
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

#define EM_X86_64       62          // X86 64-bit architecture




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
     * table section, which stores all of the names
     * of the sections in the file. This is distinct
     * from the general string table, which contains
     * names such as functions and the name of the file.
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
     * table section, which stores all of the names
     * of the sections in the file. This is distinct
     * from the general string table, which contains
     * names such as functions and the name of the file.
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


/*
 * The section contains program-specific information whose
 * format, meaning, and contents are defined by the program.
 */
#define SHT_PROGBITS    1


/*
 * This section holds a symbol table.
 */
#define SHT_SYMTAB      2


/*
 * This section holds a string table.
 */
#define SHT_STRTAB      3


/*
 * The section holds relocation entries with explicit
 * addends. An ELF file may have multiple relocation
 * sections.
 */
#define SHT_RELA        4


/*
 * The section holds a symbol hash table.
 */
#define SHT_HASH        5


/*
 * The section holds information for dynamic linking.
 */
#define SHT_DYNAMIC     6


/*
 * The section holds information that marks the file
 * in some way.
 */
#define SHT_NOTE        7


/*
 * The section resembles a section of type SHT_PROGBITS
 * except that it takes up no space in the file. The
 * sh_offset member still marks the location of the 
 * section in the file.
 */
#define SHT_NOBITS      8


/*
 * The section holds relocation entries without explicit
 * addends. The file may have multiple relocation sections.
 */
#define SHT_REL         9


/*
 * Reserved section type with unspecified semantics.
 */
#define SHT_SHLIB       10


/*
 * This section holds a symbol table.
 */
#define SHT_DYNSYM      11


/*
 * Values in this range are reserved for
 * processor-specific semantics.
 */
#define SHT_LOPROC      0x70000000
#define SHT_HIPROC      0x7fffffff


/*
 * Values in this range are reserved for application
 * programs to use without conflicting with current or
 * future system-defined section types.
 */
#define SHT_LOUSER      0x80000000
#define SHT_HIUSER      0xffffffff



/*
 * Masks for section header flags. The corresponding bit(s)
 * being set indicates the section has the given attribute.
 */
#define SHF_WRITE       0x1         // Flag marks section as writable during process execution
#define SHF_ALLOC       0x2         // Flag marks section as occupying memory in process image
#define SHF_EXECINSTR   0x4         // Flag marks section as containing executable machine instructions
#define SHF_MASKPROC    0xf0000000  // Processor-specific semantics


#define STN_UNDEF       0           // Undefined symbol index in the symbol table


typedef struct
{

    /*
     * Gives an index into one of the object file's
     * string tables. This index gives the name of
     * the symbol. For symbols of type STT_SECTION
     * the index is into the .shstrtab table, and
     * for others it is .strtab table.
     */
    ELF32_Word_t st_name;


    /*
     * Gives the value of the associated symbol. Depending
     * on the type of the object file and the type of the
     * section the symbol is in.
     * 
     * For relocatable object files, for symbols defined
     * against section index SHN_COMMON, st_value holds
     * alignment constraints. For symbols defined against
     * any other section in relocatable files, st_value
     * holds the offset from the beginning of the section
     * referenced by st_shndx.
     * 
     * For executable and shared object files, st_value
     * holds a virtual address.
     */
    ELF32_Addr_t st_value;


    /*
     * Gives the size of the data object referenced by the
     * symbol. Size is 0 if object takes up no space or the
     * size is unknown.
     */
    ELF32_Word_t st_size;


    /*
     * The upper 4 bits of this member contain the symbol's
     * binding characteristics, and the lower 4 bits contain
     * the symbol's type. Macros to manipulate this member
     * are given elsewhere.
     */
    unsigned char st_info;


    /*
     * Currently defined to be 0 and given no meaning.
     */
    unsigned char st_other;


    /*
     * Since every symbol is defined relative to some section
     * of the object file, this member gives the index into the
     * section header table of the relevant section. Some section
     * indexes have special meanings. See section index definitions
     * for details.
     */
    ELF32_Half_t st_shndx;

    
} ELF32_Sym_t;



typedef struct
{

    /*
     * Gives an index into one of the object file's
     * string tables. This index gives the name of
     * the symbol. For symbols of type STT_SECTION
     * the index is into the .shstrtab table, and
     * for others it is .strtab table.
     */
    ELF64_Word_t st_name;


    /*
     * Gives the value of the associated symbol. Depending
     * on the type of the object file and the type of the
     * section the symbol is in.
     * 
     * For relocatable object files, for symbols defined
     * against section index SHN_COMMON, st_value holds
     * alignment constraints. For symbols defined against
     * any other section in relocatable files, st_value
     * holds the offset from the beginning of the section
     * referenced by st_shndx.
     * 
     * For executable and shared object files, st_value
     * holds a virtual address.
     */
    ELF64_Addr_t st_value;


    /*
     * Gives the size of the data object referenced by the
     * symbol. Size is 0 if object takes up no space or the
     * size is unknown.
     */
    ELF64_Word_t st_size;


    /*
     * The upper 4 bits of this member contain the symbol's
     * binding characteristics, and the lower 4 bits contain
     * the symbol's type. Macros to manipulate this member
     * are given elsewhere.
     */
    unsigned char st_info;


    /*
     * Currently defined to be 0 and given no meaning.
     */
    unsigned char st_other;


    /*
     * Since every symbol is defined relative to some section
     * of the object file, this member gives the index into the
     * section header table of the relevant section. Some section
     * indexes have special meanings. See section index definitions
     * for details.
     */
    ELF64_Half_t st_shndx;

    
} ELF64_Sym_t;



/*
 * Extracts the binding information from the
 * st_info member of the symbol table entry.
 * Binding is the upper 4 bits of the member.
 */
#define ELF_ST_BIND(i) ((i)>>4)


/*
 * Extracts the type information from the st_info
 * member of the symbol table entry. Type information
 * is the lower 4 bits of the member.
 */
#define ELF_ST_TYPE(i) ((i)&0xf)


/*
 * Constructs the st_info member from the given binding
 * information b and type information t. Shifts b into
 * the upper 4 bits of st_info, applies bitwise AND mask
 * to t to clear upper 4 bits of t and bitwise ORs the
 * two together.
 */
#define ELF_ST_INFO(b,t)( ((b)<<4) | ((t)&0xf) )




/*******************************
 * Symbol binding definitions. *
 *******************************/



/*
 * Symbols with local binding are not visible outside the file they
 * are defined in. Local symbols of the same name may exist in different
 * files without interfering with each other.
 */
#define STB_LOCAL 0


/*
 * Global symbols are visible to all object files being combined.
 * One file's definition of a global symbol satisfies another file's
 * undefined reference to that global symbol.
 */
#define STB_GLOBAL 1


/*
 * Weak symbols are similar to global symbols but with lower precedence.
 * A weak symbol has the same visibility as a global symbol, but given
 * a global symbol and a weak symbol, the global symbol will be chosen.
 */
#define STB_WEAK 2


// Processor-specific semantics
#define STB_LOPROC 13
#define STB_HIPROC 15



/***************************
 * Symbol type definitions *
 ***************************/


/*
 * Symbol type not defined.
 */
#define STT_NOTYPE 0


/*
 * The symbol is associated with a data
 * object, like a variable, and array, a struct, etc.
 */
#define STT_OBJECT 1


/*
 * The symbol is associated with a function or
 * other executable code.
 */
#define STT_FUNC 2


/*
 * The symbol is associated with a given section of
 * the object file. These symbols are primarily used
 * for relocation and are usually given STB_LOCAL binding.
 */
#define STT_SECTION 3


/*
 * The symbol is associated with a filename, for example
 * the name of the file the object file was compiled from.
 */
#define STT_FILE 4


// Processor-specific semantics
#define STT_LOPROC 13
#define STT_HIPROC 15




/*******************************************************
 * This section defines the 32 and 64 bit relocation
 * entries. Relocation entries are used to keep track
 * of the places in the code where unresolved references
 * are used in the code. This allows the linker to
 * adjust the references to point to the right place.
 * For example, a function used in one file but defined
 * in another would compile to a call instruction. This
 * call instruction would have a reference to the
 * address of the function to be called, which is not
 * known when the file is compiled. The compiler would
 * then leave this address as zero and add a relocation
 * entry to the output ELF file. The process of 
 * relocation would then be left to the linker.
 ********************************************************/



/*
 * 32-bit Relocation entry without explicit addends.
 */
typedef struct
{

    /*
     * This gives the location at which to apply
     * the relocation. For relocatable object files
     * this value is the offset from the beginning
     * of the section to the location that is affected.
     * For executable and shared object files, this
     * value gives the virtual address of the location
     * that is affected.
     */
    ELF32_Addr_t r_offset;


    /*
     * This gives the symbol table index of the object
     * to apply the relocation to as well as the type
     * of relocation to apply. Relocation types are
     * processor-specific.
     */
    ELF32_Word_t r_info;


} ELF32_Rel_t;



/*
 * 32-bit relocation entry with explicit addends.
 */
typedef struct
{
    
    /*
     * Same as above.
     */
    ELF32_Addr_t r_offset;


    /*
     * Same as above.
     */
    ELF32_Word_t r_info;


    /*
     * Constant integer (positive or negative)
     * to be added to the relocation information.
     */
    ELF32_Sword_t r_addend;


} ELF32_Rela_t;


/*
 * Macro for extracting symbol table index from r_info.
 */
#define ELF32_R_SYM(i) ((i)>>8)


/*
 * Macro for extracting relocation type from r_info.
 */
#define ELF32_R_TYPE(i) ((unsigned char)i)      // could also use bitwise & mask with 0xff, but whatever


/*
 * Macro for constructing r_info from symbol table index
 * and relocation type.
 */
#define ELF32_R_INFO(s,t) (((s)<<8) + (unsigned char)(t))




/*
 * 64-bit Relocation entry without explicit addends.
 */
typedef struct
{

    /*
     * This gives the location at which to apply
     * the relocation. For relocatable object files
     * this value is the offset from the beginning
     * of the section to the location that is affected.
     * For executable and shared object files, this
     * value gives the virtual address of the location
     * that is affected.
     */
    ELF64_Addr_t r_offset;


    /*
     * This gives the symbol table index of the object
     * to apply the relocation to as well as the type
     * of relocation to apply. Relocation types are
     * processor-specific.
     */
    ELF64_Xword_t r_info;


} ELF64_Rel_t;



/*
 * 32-bit relocation entry with explicit addends.
 */
typedef struct
{
    
    /*
     * Same as above.
     */
    ELF64_Addr_t r_offset;


    /*
     * Same as above.
     */
    ELF64_Xword_t r_info;


    /*
     * Constant integer (positive or negative)
     * to be added to the relocation information.
     */
    ELF64_Sxword_t r_addend;


} ELF64_Rela_t;



/*
 * Macro for extracting symbol table index from r_info.
 */
#define ELF64_R_SYM(i) ((i)>>32)


/*
 * Macro for extracting relocation type from r_info.
 */
#define ELF64_R_TYPE(i) ((i) & 0xffffffff)


/*
 * Macro for constructing r_info from symbol table index
 * and relocation type.
 */
#define ELF64_R_INFO(s,t) (((s)<<32) + ((t) & 0xffffffff))





/******************************************************
 * This section defines structs and macros relating to
 * program headers and information used in program
 * loading.
 ******************************************************/



typedef struct
{
    

    /*
     * This describes the type of the segment this program
     * header element refers to.
     */
    ELF32_Word_t p_type;


    /*
     * This member gives the offset from the beginning of the
     * file at which the first byte of the segment resides.
     */
    ELF32_Off_t p_offset;


    /*
     * This member gives the virtual address at which the
     * segment resides in memory.
     */
    ELF32_Addr_t p_vaddr;


    /*
     * If the target system supports physical addressing, then
     * this member gives the physical address at which the
     * segment resides.
     */
    ELF32_Addr_t p_paddr;


    /*
     * This member gives the size, in bytes, of the segment as
     * it resides in the file.
     */
    ELF32_Word_t p_filesz;


    /*
     * This member gives the size, in bytes, of the segment as
     * it appears in memory. This must be greater than or equal
     * to p_filesz.
     */
    ELF32_Word_t p_memsz;


    /*
     * Defines relevant flags for the segment.
     */
    ELF32_Word_t p_flags;


    /*
     * Gives the value to which the segment is aligned in memory.
     */
    ELF32_Word_t p_align;
    

} ELF32_Program_Header_t;




typedef struct
{
    

    /*
     * This describes the type of the segment this program
     * header element refers to.
     */
    ELF64_Word_t p_type;


    /*
     * Defines relevant flags for the segment.
     */
    ELF64_Word_t p_flags;


    /*
     * This member gives the offset from the beginning of the
     * file at which the first byte of the segment resides.
     */
    ELF64_Off_t p_offset;


    /*
     * This member gives the virtual address at which the
     * segment resides in memory.
     */
    ELF64_Addr_t p_vaddr;


    /*
     * If the target system supports physical addressing, then
     * this member gives the physical address at which the
     * segment resides.
     */
    ELF64_Addr_t p_paddr;


    /*
     * This member gives the size, in bytes, of the segment as
     * it resides in the file.
     */
    ELF64_Xword_t p_filesz;


    /*
     * This member gives the size, in bytes, of the segment as
     * it appears in memory. This must be greater than or equal
     * to p_filesz.
     */
    ELF64_Xword_t p_memsz;


    /*
     * Gives the value to which the segment is aligned in memory.
     */
    ELF64_Xword_t p_align;
    

} ELF64_Program_Header_t;



/*****************
 * Segment types *
 *****************/


/*
 * The given segment is unused and the other elements of the
 * program header struct are undefined. This allows the program
 * header table to have ignored entries.
 */
#define PT_NULL     0


/*
 * This defines a loadable segment, i.e. a segment to be loaded
 * into memory at the address specified by p_vaddr. The first,
 * p_filesz bytes of the segment in the file will be mapped into
 * the first p_filesz bytes in memory. The remaining p_memsz - p_filesz
 * bytes of the memory segment will be 0 initialized.
 */
#define PT_LOAD     1


/*
 * This segment specifies dynamic linking information.
 */
#define PT_DYNAMIC  2


/*
 * This segment specifies the size and pathname of an interpreter.
 */
#define PT_INTERP   3


/*
 * Gives the location and size of auxiliary information.
 */
#define PT_NOTE     4


/*
 * Reserved but with unspecified semantics.
 */
#define PT_SHLIB    5


/*
 * This segment specifies the location, both in the file and in
 * memory, of the program header table.
 */
#define PT_PHDR     6


/*
 * Low and high range of processor-specific semantics.
 */
#define PT_LOPROC   0x70000000
#define PT_HIPROC   0x7fffffff






#endif