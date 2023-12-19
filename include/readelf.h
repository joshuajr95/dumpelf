/* Author: Joshua Jacobs-Rebhun
 * Date: December 5, 2023
 *
 * 
 * This file declares a number of functions for reading particular
 * data structures out of an ELF file.
 */
#ifndef READELF_H
#define READELF_H


#include <stdbool.h>


#include "elf.h"


#define RET_OK          0
#define RET_NOT_OK      -1


int read_ELF_identification(FILE *input_file, unsigned char *buffer);
int get_file_class(FILE *input_file);
char **get_section_names(FILE *input_file);



int read_ELF32_header(FILE *input_file, ELF32_Header_t *header);

ELF32_Section_Header_t *read_ELF32_section_header(FILE *input_file, int index);
ELF32_Section_Header_t *read_ELF32_section_header_table(FILE *input_file);

ELF32_Program_Header_t *read_ELF32_program_header(FILE *input_file, int index);
ELF32_Program_Header_t *read_ELF32_program_header_table(FILE *input_file);



int read_ELF64_header(FILE *input_file, ELF64_Header_t *header);

ELF64_Section_Header_t *read_ELF64_section_header(FILE *input_file, int index);
ELF64_Section_Header_t *read_ELF64_section_header_table(FILE *input_file);

ELF64_Program_Header_t *read_ELF64_program_header(FILE *input_file, int index);
ELF64_Program_Header_t *read_ELF64_program_header_table(FILE *input_file);



char ***get_section_to_segment_mapping(FILE *input_file);


#endif