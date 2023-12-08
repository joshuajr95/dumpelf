/* Author: Joshua Jacobs-Rebhun
 * Date: December 7, 2023
 *
 * 
 * Declarations for functions that convert information
 * read from ELF file into string to be printed to screen.
 */

#ifndef STRINGIFY_H
#define STRINGIFY_H


#include "elf.h"

char *stringify_ELF32_header(ELF32_Header_t *elf_header);
char *stringify_ELF32_section_headers(ELF32_Section_Header_t *section_header_table, int num_sections);


char *stringify_ELF64_header(ELF64_Header_t *elf_header);
char *stringify_ELF64_section_headers(ELF64_Section_Header_t *section_header_table, int num_sections);

#endif