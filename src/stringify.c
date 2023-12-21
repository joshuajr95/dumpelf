/* Author: Joshua Jacobs-Rebhun
 * Date: December 14, 2023
 *
 * 
 * Implementation file for turning the ELF file data structures into strings
 * to be printed to stdout for the user.
 */

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>


#include "stringify.h"





static char *section_types[] = {
    [SHT_NULL] = "NULL",
    [SHT_PROGBITS] = "PROGBITS",
    [SHT_SYMTAB] = "SYMTAB",
    [SHT_STRTAB] = "STRTAB",
    [SHT_RELA] = "RELA",
    [SHT_HASH] = "HASH",
    [SHT_DYNAMIC] = "DYNAMIC",
    [SHT_NOTE] = "NOTE",
    [SHT_NOBITS] = "NOBITS",
    [SHT_REL] = "REL",
    [SHT_SHLIB] = "SHLIB",
    [SHT_DYNSYM] = "DYNSYM"
};



/*
 * Segment types
 */
static char *segment_types[] = {
    [PT_NULL] = "NULL",
    [PT_LOAD] = "LOAD",
    [PT_DYNAMIC] = "DYNAMIC",
    [PT_INTERP] = "INTERP",
    [PT_NOTE] = "NOTE",
    [PT_SHLIB] = "SHLIB",
    [PT_PHDR] = "PHDR"
};





/************************************
 * This section contains the 32-bit *
 * stringification routines.        *
 ************************************/

char *stringify_ELF32_header(ELF32_Header_t *elf_header)
{
    /*
     * Temporary buffer to store strings before they
     * are concatenated onto the output string.
     */
    char buffer[256];


    /*
     * For creating output string need to dynamically allocate
     * memory. However, the ELF header string is continually
     * growing, so there is a need to allocate enough memory
     * and then some for the current size of the string. Thus,
     * max_size gives the size allocated, and current_size gives
     * the current size of the string, i.e. current_size is how
     * much of the buffer of max_size is actually taken up by
     * the string. When adding new text to the string causes
     * current_size to become greater than max_size, then the
     * string must be reallocated.
     */
    int max_size = 64, current_size = 0;


    char *header_string = "ELF header:\n";
    char *ident = "  ELF Identification bits:\t";
    char *class = "  Class:\t\t\t\t";
    char *data = "  Data:\t\t\t\t\t";
    char *version = "  Version:\t\t\t\t";
    char *osabi = "  OS/ABI:\t\t\t\t";
    char *type = "  Type:\t\t\t\t\t";
    char *machine = "  Machine:\t\t\t\t";
    char *entry = "  Entry point address:\t\t\t";
    char *section_headers = "  Start of section headers:\t\t";
    char *program_headers = "  Start of program headers:\t\t";
    char *flags = "  Flags:\t\t\t\t";
    char *header_size = "  Size of this header:\t\t\t";
    char *program_header_size = "  Size of the program headers:\t\t";
    char *num_program_headers = "  Number of program headers:\t\t";
    char *section_header_size = "  Size of the section headers:\t\t";
    char *num_section_headers = "  Number of section headers:\t\t";
    char *string_table_index = "  Section header string table index:\t";


    // allocate an initial buffer of 64 bytes
    char *output_string = (char*) malloc(max_size*sizeof(char));
    
    
    CONCATENATE_DYNAMIC_STRING(output_string, header_string, max_size, current_size);
    CONCATENATE_DYNAMIC_STRING(output_string, ident, max_size, current_size)


    // add the identification bits to the output string
    for(int i = 0; i < EI_NIDENT; i++)
    {
        char temp[8];

        if(i < EI_NIDENT - 1)
            sprintf(temp, "%02x ", elf_header->e_ident[i]);
        else
            sprintf(temp, "%02x\n", elf_header->e_ident[i]);
        
        CONCATENATE_DYNAMIC_STRING(output_string, temp, max_size, current_size);
    }


    // add ELF file class to output string
    CONCATENATE_DYNAMIC_STRING(output_string, class, max_size, current_size);

    switch(elf_header->e_ident[EI_CLASS])
    {
        case ELFCLASS32:
            sprintf(buffer, "ELF32\n");
            break;
        
        case ELFCLASS64:
            sprintf(buffer, "ELF64\n");
            break;
        
        default:
            sprintf(buffer, "Unrecognized\n");
    }
    
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);



    // add ELF data format to output string
    CONCATENATE_DYNAMIC_STRING(output_string, data, max_size, current_size);

    switch (elf_header->e_ident[EI_DATA])
    {
    case ELFDATA2LSB:
        sprintf(buffer, "2's complement, little-endian\n");
        break;
    
    case ELFDATA2MSB:
        sprintf(buffer, "2's complement, big-endian");
        break;
    
    default:
        sprintf(buffer, "Unrecognized format");
        break;
    }

    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add ELF version to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, version, max_size, current_size);

    switch(elf_header->e_version)
    {
        case EV_CURRENT:
            sprintf(buffer, "Current\n");
            break;
        
        default:
            sprintf(buffer, "None\n");
            break;
    }

    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);



    // add OS/ABI to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, osabi, max_size, current_size);

    switch(elf_header->e_ident[EI_OSABI])
    {
        case ELFOSABI_SYSV:
            sprintf(buffer, "Unix - System V\n");
            break;
        
        case ELFOSABI_HPUX:
            sprintf(buffer, "HP-UX\n");
            break;
        
        case ELFOSABI_STANDALONE:
            sprintf(buffer, "Standalone (embedded) application\n");
            break;
        
        default:
            sprintf(buffer, "Unrecognized\n");
            break;
    }

    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);



    // add type of object file to output string
    CONCATENATE_DYNAMIC_STRING(output_string, type, max_size, current_size);

    switch(elf_header->e_type)
    {
        case ET_NONE:
            sprintf(buffer, "None\n");
            break;
        
        case ET_REL:
            sprintf(buffer, "REL (Relocatable File)\n");
            break;
        
        case ET_EXEC:
            sprintf(buffer, "EXEC (Executable file)\n");
            break;

        case ET_DYN:
            sprintf(buffer, "DYN (Dynamically-linker shared library)\n");
            break;

        case ET_CORE:
            sprintf(buffer, "CORE (Core dump file)\n");
            break;

        default:
            sprintf(buffer, "Unkown or processor-specific type\n");
            break;
    }

    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);



    // add machine type information to output string
    CONCATENATE_DYNAMIC_STRING(output_string, machine, max_size, current_size);

    switch (elf_header->e_machine)
    {
    case EM_NONE:
        sprintf(buffer, "No machine type\n");
        break;

    case EM_M32:
        sprintf(buffer, "AT&T WE 32100\n");
        break;

    case EM_SPARC:
        sprintf(buffer, "SPARC\n");
        break;
    
    case EM_386:
        sprintf(buffer, "Intel Architecture\n");
        break;
    
    case EM_68K:
        sprintf(buffer, "Motorola 68000\n");
        break;
    
    case EM_88K:
        sprintf(buffer, "Motorola 88000\n");
        break;
    
    case EM_860:
        sprintf(buffer, "Intel 80860\n");
        break;
    
    case EM_MIPS:
        sprintf(buffer, "MIPS RS3000 Big-Endian\n");
        break;
    
    case EM_MIPS_RS4_BE:
        sprintf(buffer, "MIPS RS4000 Big-Endian\n");
        break;
    
    case EM_X86_64:
        sprintf(buffer, "Advanced Micro Devices X86-64\n");
        break;
    
    default:
        sprintf(buffer, "%d: Unknown machine architecture\n", elf_header->e_machine);
        break;
    }

    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);



    // add the entry point address to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, entry, max_size, current_size);
    sprintf(buffer, "0x%x\n", elf_header->e_entry);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the section header offset to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, section_headers, max_size, current_size);
    sprintf(buffer, "%u (bytes into the file)\n", elf_header->e_shoff);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the program header offset to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, program_headers, max_size, current_size);
    sprintf(buffer, "%u (bytes into the file)\n", elf_header->e_phoff);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);

    
    // add flags to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, flags, max_size, current_size);
    sprintf(buffer, "0x%x\n", elf_header->e_flags);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the ELF header size to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, header_size, max_size, current_size);
    sprintf(buffer, "%d (bytes)\n", elf_header->e_ehsize);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the section header size to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, section_header_size, max_size, current_size);
    sprintf(buffer, "%d (bytes)\n", elf_header->e_shentsize);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the number of section headers to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, num_section_headers, max_size, current_size);
    sprintf(buffer, "%d\n", elf_header->e_shnum);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the program header size to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, program_header_size, max_size, current_size);
    sprintf(buffer, "%d (bytes)\n", elf_header->e_phentsize);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the number of program headers to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, num_program_headers, max_size, current_size);
    sprintf(buffer, "%d\n", elf_header->e_phnum);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the section header string table index to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, string_table_index, max_size, current_size);
    sprintf(buffer, "%d\n", elf_header->e_shstrndx);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);

    
    return output_string;
}


char *stringify_ELF32_section_header_table(ELF32_Section_Header_t *section_header_table, ELF32_Header_t *file_header, char **section_header_names)
{
    fprintf(stderr, "TODO: Implement stringification of 32-bit ELF section header table.\n");
    return NULL;
}


char *stringify_ELF32_program_header_table(ELF32_Program_Header_t *program_header_table, ELF32_Header_t *file_header, char ***section_to_segment_mapping)
{
    fprintf(stderr, "TODO: Implement stringification of 32-bit ELF program header table.\n");
    return NULL;
}




/************************************
 * This section contains the 64-bit *
 * stringification routines.        *
 ************************************/


char *stringify_ELF64_header(ELF64_Header_t *elf_header)
{

    /*
     * Temporary buffer to store strings before they
     * are concatenated onto the output string.
     */
    char buffer[256];


    /*
     * For creating output string need to dynamically allocate
     * memory. However, the ELF header string is continually
     * growing, so there is a need to allocate enough memory
     * and then some for the current size of the string. Thus,
     * max_size gives the size allocated, and current_size gives
     * the current size of the string, i.e. current_size is how
     * much of the buffer of max_size is actually taken up by
     * the string. When adding new text to the string causes
     * current_size to become greater than max_size, then the
     * string must be reallocated.
     */
    int max_size = 64, current_size = 0;


    char *header_string = "ELF header:\n";
    char *ident = "  ELF Identification bits:\t";
    char *class = "  Class:\t\t\t\t";
    char *data = "  Data:\t\t\t\t\t";
    char *version = "  Version:\t\t\t\t";
    char *osabi = "  OS/ABI:\t\t\t\t";
    char *type = "  Type:\t\t\t\t\t";
    char *machine = "  Machine:\t\t\t\t";
    char *entry = "  Entry point address:\t\t\t";
    char *section_headers = "  Start of section headers:\t\t";
    char *program_headers = "  Start of program headers:\t\t";
    char *flags = "  Flags:\t\t\t\t";
    char *header_size = "  Size of this header:\t\t\t";
    char *program_header_size = "  Size of the program headers:\t\t";
    char *num_program_headers = "  Number of program headers:\t\t";
    char *section_header_size = "  Size of the section headers:\t\t";
    char *num_section_headers = "  Number of section headers:\t\t";
    char *string_table_index = "  Section header string table index:\t";


    // allocate an initial buffer of 64 bytes
    char *output_string = (char*) malloc(max_size*sizeof(char));
    
    
    CONCATENATE_DYNAMIC_STRING(output_string, header_string, max_size, current_size);
    CONCATENATE_DYNAMIC_STRING(output_string, ident, max_size, current_size)


    // add the identification bits to the output string
    for(int i = 0; i < EI_NIDENT; i++)
    {
        char temp[8];

        if(i < EI_NIDENT - 1)
            sprintf(temp, "%02x ", elf_header->e_ident[i]);
        else
            sprintf(temp, "%02x\n", elf_header->e_ident[i]);
        
        CONCATENATE_DYNAMIC_STRING(output_string, temp, max_size, current_size);
    }


    // add ELF file class to output string
    CONCATENATE_DYNAMIC_STRING(output_string, class, max_size, current_size);

    switch(elf_header->e_ident[EI_CLASS])
    {
        case ELFCLASS32:
            sprintf(buffer, "ELF32\n");
            break;
        
        case ELFCLASS64:
            sprintf(buffer, "ELF64\n");
            break;
        
        default:
            sprintf(buffer, "Unrecognized\n");
    }
    
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);



    // add ELF data format to output string
    CONCATENATE_DYNAMIC_STRING(output_string, data, max_size, current_size);

    switch (elf_header->e_ident[EI_DATA])
    {
    case ELFDATA2LSB:
        sprintf(buffer, "2's complement, little-endian\n");
        break;
    
    case ELFDATA2MSB:
        sprintf(buffer, "2's complement, big-endian");
        break;
    
    default:
        sprintf(buffer, "Unrecognized format");
        break;
    }

    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add ELF version to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, version, max_size, current_size);

    switch(elf_header->e_version)
    {
        case EV_CURRENT:
            sprintf(buffer, "Current\n");
            break;
        
        default:
            sprintf(buffer, "None\n");
            break;
    }

    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);



    // add OS/ABI to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, osabi, max_size, current_size);

    switch(elf_header->e_ident[EI_OSABI])
    {
        case ELFOSABI_SYSV:
            sprintf(buffer, "Unix - System V\n");
            break;
        
        case ELFOSABI_HPUX:
            sprintf(buffer, "HP-UX\n");
            break;
        
        case ELFOSABI_STANDALONE:
            sprintf(buffer, "Standalone (embedded) application\n");
            break;
        
        default:
            sprintf(buffer, "Unrecognized\n");
            break;
    }

    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);



    // add type of object file to output string
    CONCATENATE_DYNAMIC_STRING(output_string, type, max_size, current_size);

    switch(elf_header->e_type)
    {
        case ET_NONE:
            sprintf(buffer, "None\n");
            break;
        
        case ET_REL:
            sprintf(buffer, "REL (Relocatable File)\n");
            break;
        
        case ET_EXEC:
            sprintf(buffer, "EXEC (Executable file)\n");
            break;

        case ET_DYN:
            sprintf(buffer, "DYN (Dynamically-linker shared library)\n");
            break;

        case ET_CORE:
            sprintf(buffer, "CORE (Core dump file)\n");
            break;

        default:
            sprintf(buffer, "Unkown or processor-specific type\n");
            break;
    }

    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);



    // add machine type information to output string
    CONCATENATE_DYNAMIC_STRING(output_string, machine, max_size, current_size);

    switch (elf_header->e_machine)
    {
    case EM_NONE:
        sprintf(buffer, "No machine type\n");
        break;

    case EM_M32:
        sprintf(buffer, "AT&T WE 32100\n");
        break;

    case EM_SPARC:
        sprintf(buffer, "SPARC\n");
        break;
    
    case EM_386:
        sprintf(buffer, "Intel Architecture\n");
        break;
    
    case EM_68K:
        sprintf(buffer, "Motorola 68000\n");
        break;
    
    case EM_88K:
        sprintf(buffer, "Motorola 88000\n");
        break;
    
    case EM_860:
        sprintf(buffer, "Intel 80860\n");
        break;
    
    case EM_MIPS:
        sprintf(buffer, "MIPS RS3000 Big-Endian\n");
        break;
    
    case EM_MIPS_RS4_BE:
        sprintf(buffer, "MIPS RS4000 Big-Endian\n");
        break;
    
    case EM_X86_64:
        sprintf(buffer, "Advanced Micro Devices X86-64\n");
        break;
    
    default:
        sprintf(buffer, "%d: Unknown machine architecture\n", elf_header->e_machine);
        break;
    }

    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);



    // add the entry point address to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, entry, max_size, current_size);
    sprintf(buffer, "0x%lx\n", elf_header->e_entry);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the section header offset to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, section_headers, max_size, current_size);
    sprintf(buffer, "%lu (bytes into the file)\n", elf_header->e_shoff);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the program header offset to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, program_headers, max_size, current_size);
    sprintf(buffer, "%lu (bytes into the file)\n", elf_header->e_phoff);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);

    
    // add flags to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, flags, max_size, current_size);
    sprintf(buffer, "0x%x\n", elf_header->e_flags);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the ELF header size to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, header_size, max_size, current_size);
    sprintf(buffer, "%d (bytes)\n", elf_header->e_ehsize);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the section header size to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, section_header_size, max_size, current_size);
    sprintf(buffer, "%d (bytes)\n", elf_header->e_shentsize);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the number of section headers to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, num_section_headers, max_size, current_size);
    sprintf(buffer, "%d\n", elf_header->e_shnum);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the program header size to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, program_header_size, max_size, current_size);
    sprintf(buffer, "%d (bytes)\n", elf_header->e_phentsize);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the number of program headers to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, num_program_headers, max_size, current_size);
    sprintf(buffer, "%d\n", elf_header->e_phnum);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the section header string table index to the output string
    CONCATENATE_DYNAMIC_STRING(output_string, string_table_index, max_size, current_size);
    sprintf(buffer, "%d\n", elf_header->e_shstrndx);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);

    
    return output_string;
}



void stringify_ELF64_flags(char *flag_buffer, ELF64_Xword_t flags)
{
    int current_index = 0;

    if(flags & SHF_WRITE )
    {
        flag_buffer[current_index] = 'W';
        current_index++;
    }

    if((flags & SHF_ALLOC) >> 1)
    {
        flag_buffer[current_index] = 'A';
        current_index++;
    }

    if((flags & SHF_EXECINSTR) >> 2)
    {
        flag_buffer[current_index] = 'X';
        current_index++;
    }

    flag_buffer[current_index] = '\0';
}



static void stringify_ELF64_section_header(char *buffer, ELF64_Section_Header_t *section_header_table, int section_number, char *section_name, int max_len)
{

    ELF64_Section_Header_t section_header = section_header_table[section_number];

    char buf[256];
    sprintf(buf, " ");

    int num_spaces = max_len - strlen(section_name) + 5;
    for(int i = 1; i < num_spaces; i++)
    {
        strcat(buf, " ");
    }


    char flags[16];

    stringify_ELF64_flags(flags, section_header.sh_flags);


    char *section_type = (section_header.sh_type <= SHT_DYNSYM) ? section_types[section_header.sh_type] : "UNKNOWN";


    sprintf(buffer, "[  %d]\t\t%s%s%s\t\t%016lx\t%08lx\t%016lx\t%016lx\t%s\t%d\t%d\t%lu\n",
                section_number,
                section_name,
                buf,
                section_type,
                section_header.sh_addr,
                section_header.sh_offset,
                section_header.sh_size,
                section_header.sh_entsize,
                flags,
                section_header.sh_link,
                section_header.sh_info,
                section_header.sh_addralign);
}




/*
 * Turns the section header table into a character string to be printed to the screen.
 * A pointer to the section header table array is passed so the function can read the
 * data from it, as well as a pointer to the ELF file header, which holds the number
 * of sections in the section header table, as well as the offset to the section header
 * table. The section header names need to be passed in separately since they reside
 * in the string table and this function does not have access to the string table. The
 * section header names list is terminated with a NULL pointer to signify the end of
 * the list.
 */
char *stringify_ELF64_section_header_table(ELF64_Section_Header_t *section_header_table, ELF64_Header_t *file_header, char **section_header_names)
{

    /*
     * Defensive check to prevent segmentation
     * fault from NULL pointer being passed.
     */
    if(section_header_table == NULL || file_header == NULL)
    {
        fprintf(stderr, "NULL pointer passed to stringify_ELF64_section_header_table.\n");
        return NULL;
    }


    /*
     * Temporary buffer to store strings before they
     * are concatenated onto the output string.
     */
    char buffer[1024];


    /*
     * For creating output string need to dynamically allocate
     * memory. However, the ELF header string is continually
     * growing, so there is a need to allocate enough memory
     * and then some for the current size of the string. Thus,
     * max_size gives the size allocated, and current_size gives
     * the current size of the string, i.e. current_size is how
     * much of the buffer of max_size is actually taken up by
     * the string. When adding new text to the string causes
     * current_size to become greater than max_size, then the
     * string must be reallocated.
     */
    int max_size = 64, current_size = 0;


    // allocate an initial buffer of 64 bytes
    char *output_string = (char*) malloc(max_size*sizeof(char));


    // add the number of section headers and the offset to the output
    sprintf(buffer, "There are %d section headers, starting at offset 0x%lx\n\n", file_header->e_shnum, file_header->e_shoff);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    // add the title to the output
    sprintf(buffer, "Section Headers:\n");
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    int max_len = 0;
    // get the length of the section header names
    for(int i = 0; i < file_header->e_shnum; i++)
    {        
        if(strlen(section_header_names[i]) > max_len)
            max_len = strlen(section_header_names[i]);
    }
    
    
    char buf[256];
    sprintf(buf, " ");

    int num_spaces = max_len - strlen("Name") + 5;
    for(int i = 1; i < num_spaces; i++)
    {
        strcat(buf, " ");
    }


    // add the headers of the table to be printed for the section headers
    sprintf(buffer, "[Number]\tName%sType\t\tAddress\t\t\tOffset\t\tSize\t\t\tEntSize\t\t\tFlags\tLink\tInfo\tAlign\n", buf);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    sprintf(buffer, "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);
    

    // for each section header add the section header data 
    for(int i = 0; i < file_header->e_shnum; i++)
    {
        stringify_ELF64_section_header(buffer, section_header_table, i, section_header_names[i], max_len);
        CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);
    }


    sprintf(buffer, "\nKey to flags:  (W) Write, (A) alloc, (X) execute.\n");
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    return output_string;
}




static void stringify_ELF64_program_header(char *buffer, ELF64_Program_Header_t *program_header_table, int index)
{
    ELF64_Program_Header_t program_header = program_header_table[index];



    /*
     * Some types are very large, i.e. 0xffff0000.
     * Thus need to check for out-of-bounds indexing
     * to prevent bus error or segmentation fault.
     */ 
    char *segment_type = (program_header.p_type <= PT_PHDR) ? segment_types[program_header.p_type] : "UNKNOWN";


    sprintf(buffer, "%d\t%s\t\t%016lx\t%016lx\t%016lx\t%016lx\t%016lx\t%d\t%lu\n", 
                index,
                segment_type,
                program_header.p_offset,
                program_header.p_vaddr,
                program_header.p_paddr,
                program_header.p_filesz,
                program_header.p_memsz,
                program_header.p_flags,
                program_header.p_align);
}



char *stringify_ELF64_program_header_table(ELF64_Program_Header_t *program_header_table, ELF64_Header_t *file_header, char ***section_to_segment_mapping)
{


    /*
     * Defensive check to prevent segmentation
     * fault from NULL pointer being passed.
     */
    if(program_header_table == NULL || file_header == NULL || section_to_segment_mapping == NULL)
    {
        fprintf(stderr, "NULL pointer passed to stringify_ELF64_section_header_table.\n");
        return NULL;
    }


    /*
     * Temporary buffer to store strings before they
     * are concatenated onto the output string.
     */
    char buffer[2048];


    /*
     * For creating output string need to dynamically allocate
     * memory. However, the ELF header string is continually
     * growing, so there is a need to allocate enough memory
     * and then some for the current size of the string. Thus,
     * max_size gives the size allocated, and current_size gives
     * the current size of the string, i.e. current_size is how
     * much of the buffer of max_size is actually taken up by
     * the string. When adding new text to the string causes
     * current_size to become greater than max_size, then the
     * string must be reallocated.
     */
    int max_size = 64, current_size = 0;


    // allocate an initial buffer of 64 bytes
    char *output_string = (char*) malloc(max_size*sizeof(char));



    char *file_types[] = {
        [ET_NONE] = "NONE",
        [ET_REL] = "REL (Relocatable File)",
        [ET_EXEC] = "EXEC (Executable File)",
        [ET_DYN] = "DYN (Position Independent executable file)",
        [ET_CORE] = "CORE (Core Dump file)"
    };


    sprintf(buffer, "\nELF file type is %s\n", file_types[file_header->e_type]);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    sprintf(buffer, "Entry point is 0x%lx\n", file_header->e_entry);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);

    sprintf(buffer, "There are %d section headers starting at offset %lu\n\n", file_header->e_phnum, file_header->e_phoff);
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);

    sprintf(buffer, "Program Headers:\n");
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    sprintf(buffer, "Number\tType\t\tOffset\t\t\tVirtAddr\t\tPhysAddr\t\tFileSize\t\tMemSize\t\t\tFlags\tAlign\n");
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    sprintf(buffer, "-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    for(int i = 0; i < file_header->e_phnum; i++)
    {
        stringify_ELF64_program_header(buffer, program_header_table, i);
        CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);
    }


    sprintf(buffer, "\n\nSection to Segment mapping:\n\n");
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);

    sprintf(buffer, "Segment Number\tSection Name\n");
    CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);


    for(int i = 0; i < file_header->e_phnum; i++)
    {
        sprintf(buffer, "%d\t\t", i);
        CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);

        for(int j = 0; section_to_segment_mapping[i][j] != NULL; j++)
        {
            CONCATENATE_DYNAMIC_STRING(output_string, section_to_segment_mapping[i][j], max_size, current_size);
            
            sprintf(buffer, " ");
            CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);
        }

        sprintf(buffer, "\n");
        CONCATENATE_DYNAMIC_STRING(output_string, buffer, max_size, current_size);
    }


    return output_string;
}


