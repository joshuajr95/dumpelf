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
    sprintf(buffer, "%x\n", elf_header->e_entry);
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
    sprintf(buffer, "%x\n", elf_header->e_flags);
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


char *stringify_ELF32_section_headers(ELF32_Section_Header_t *section_header_table, int num_sections)
{
    fprintf(stderr, "TODO: Implement stringification of 32-bit ELF section header table.\n");
    return NULL;
}


char *stringify_ELF32_program_headers(ELF32_Program_Header_t *program_header_table, int num_segments)
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
    sprintf(buffer, "%lx\n", elf_header->e_entry);
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
    sprintf(buffer, "%x\n", elf_header->e_flags);
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


char *stringify_ELF64_section_headers(ELF64_Section_Header_t *section_header_table, int num_sections)
{
    fprintf(stderr, "TODO: Implement stringification of 64-bit ELF section header table.\n");
    return NULL;
}


char *stringify_ELF64_program_headers(ELF64_Program_Header_t *program_header_table, int num_segments)
{
    fprintf(stderr, "TODO: Implement stringification of 64-bit ELF program header table.\n");
    return NULL;
}


