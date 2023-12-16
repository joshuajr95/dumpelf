/* Author: Joshua Jacobs-Rebhun
 * Date: December 7, 2023
 * 
 * 
 * Implementation file for reading the ELF file.
 */


#include <stdio.h>


#include "elf.h"
#include "readelf.h"




/*
 * Read the identification bits of the file.
 */
int read_ELF_identification(FILE *input_file, unsigned char *buffer)
{
    // set cursor to begining of file
    fseek(input_file, 0, SEEK_SET);


    // read the first EI_NIDENT bytes of file into buffer
    if(fread(buffer, EI_NIDENT, 1,input_file) < 1)
    {
        return RET_NOT_OK;
    }

    return RET_OK;
}


/*
 * Gets the file's class from the identification
 * bits of the ELF header. Class is either 32-bit,
 * 64-bit, or none.
 */
int get_file_class(FILE *input_file)
{
    unsigned char e_ident[EI_NIDENT];

    read_ELF_identification(input_file, e_ident);

    return (int) e_ident[EI_CLASS];
}



int read_ELF32_header(FILE *input_file, ELF32_Header_t *header)
{
    // jump to the beginning of the file where the ELF header is
    fseek(input_file, 0, SEEK_SET);

    if(fread(header, sizeof(ELF32_Header_t), 1, input_file) != 1)
    {
        return RET_NOT_OK;
    }
    
    return RET_OK;
}


int read_ELF64_header(FILE *input_file, ELF64_Header_t *header)
{
    // jump to the beginning of the file where the ELF header is
    fseek(input_file, 0, SEEK_SET);

    if(fread(header, sizeof(ELF64_Header_t), 1, input_file) != 1)
    {
        return RET_NOT_OK;
    }

    return RET_OK;
}


int read_ELF32_section_header_table(FILE *input_file, ELF32_Section_Header_t *header_table, int num_sections)
{
    fprintf(stderr, "TODO: Implement reading ELF32 section header table.\n");
    return RET_NOT_OK;
}


int read_ELF64_section_header_table(FILE *input_file, ELF64_Section_Header_t *header_table, int num_sections)
{
    fprintf(stderr, "TODO: Implement reading ELF64 section header table.\n");
    return RET_NOT_OK;
}


int read_ELF32_program_header_table(FILE *input_file, ELF32_Program_Header_t *header_table, int num_segments)
{
    fprintf(stderr, "TODO: Implement reading ELF32 program header table.\n");
    return RET_NOT_OK;
}


int read_ELF64_program_header_table(FILE *input_file, ELF64_Program_Header_t *header_table, int num_segments)
{
    fprintf(stderr, "TODO: Implement reading ELF64 program header table.\n");
    return RET_NOT_OK;
}





