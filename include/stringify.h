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




/*
 * Concatenates a new NULL-terminated string onto an existing dynamically-allocated string.
 * The _output_string argument must be a heap-allocated string (i.e. with malloc) or else
 * the program will crash, and _new_string must be NULL-terminated or this will not work.
 * The _max_size and _current_size variables keep track of how much memory is allocated
 * and how much of the allocated memory is being used, respectively.
 */
#define CONCATENATE_DYNAMIC_STRING(_output_string, _new_string, _max_size, _current_size)       \
    while(strlen(_new_string) > _max_size - _current_size)                                      \
    {                                                                                           \
        _max_size = _max_size*2;                                                                \
        _output_string = (char*) realloc(_output_string, _max_size);                            \
    }                                                                                           \
    memcpy(_output_string + _current_size, _new_string, strlen(_new_string)*sizeof(char));      \
    _current_size += strlen(_new_string);





char *stringify_ELF32_header(ELF32_Header_t *elf_header);
char *stringify_ELF32_section_header_table(ELF32_Section_Header_t *section_header_table, ELF32_Header_t *file_header, char **section_header_names);
char *stringify_ELF32_program_header_table(ELF32_Program_Header_t *program_header_table, ELF32_Header_t *file_header, char ***section_to_segment_mapping);



char *stringify_ELF64_header(ELF64_Header_t *elf_header);
char *stringify_ELF64_section_header_table(ELF64_Section_Header_t *section_header_table, ELF64_Header_t *file_header, char **section_header_names);
char *stringify_ELF64_program_header_table(ELF64_Program_Header_t *program_header_table, ELF64_Header_t *file_header, char ***section_to_segment_mapping);

#endif