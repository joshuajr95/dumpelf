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
    fprintf(stderr, "TODO: Implement stringification of 32-bit ELF header.\n");
    return NULL;
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
    char *class = "  Class:\t\t\t\t\t";
    char *data = "  Data:\t\t\t\t\t\t";
    char *version = "  Version:\t\t\t\t\t";
    char *osabi = "  OS/ABI:\t\t\t\t\t";
    char *abiver = "  ABI Version:\t\t\t\t";


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


