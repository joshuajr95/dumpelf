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
    fprintf(stderr, "TODO: Implement stringification of 64-bit ELF header.\n");
    return NULL;
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


