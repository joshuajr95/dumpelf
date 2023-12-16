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





/*
 * Reads the ELF file header for 32-bit ELF files. The
 * header parameter must either be a pointer to heap
 * allocated memory or to a struct on the stack and
 * must not be null.
 */
int read_ELF32_header(FILE *input_file, ELF32_Header_t *header)
{

    /*
     * Defensive check to prevent a segmentation fault
     * from NULL pointer being passed to the function.
     */
    if(header == NULL)
    {
        fprintf(stderr, "NULL pointer passed to read_ELF32_header.\n");
        return RET_NOT_OK;
    }

    // jump to the beginning of the file where the ELF header is
    fseek(input_file, 0, SEEK_SET);

    if(fread(header, sizeof(ELF32_Header_t), 1, input_file) != 1)
    {
        return RET_NOT_OK;
    }
    
    return RET_OK;
}





/*
 * Reads the ELF file header for 64-bit ELF files. The
 * header parameter must either be a pointer to heap
 * allocated memory or to a struct on the stack and
 * must not be null.
 */
int read_ELF64_header(FILE *input_file, ELF64_Header_t *header)
{

    /*
     * Defensive check to prevent a segmentation fault
     * from NULL pointer being passed to the function.
     */
    if(header == NULL)
    {
        fprintf(stderr, "NULL pointer passed to read_ELF64_header.\n");
        return RET_NOT_OK;
    }


    // jump to the beginning of the file where the ELF header is
    fseek(input_file, 0, SEEK_SET);

    if(fread(header, sizeof(ELF64_Header_t), 1, input_file) != 1)
    {
        return RET_NOT_OK;
    }

    return RET_OK;
}




/*
 * Reads the section header table for 32-bit ELF files.
 * The input file stream is passed as a parameter to read
 * as well as a pointer to a previously allocated memory
 * buffer large enough to store the entire section header
 * table. The number of section headers is also passed as
 * a parameter, meaning it must be known before calling
 * this function.
 */
int read_ELF32_section_header_table(FILE *input_file, ELF32_Section_Header_t *header_table, ELF32_Half_t num_sections)
{
    ELF32_Header_t file_header;

    /*
     * Defensive check to prevent a
     * segmentation fault from a NULL
     * pointer being passed in.
     */
    if(header_table == NULL)
    {
        fprintf(stderr, "NULL pointer passed to read_ELF32_section_header_table.\n");
        return RET_NOT_OK;
    }


    /*
     * Read the ELF header to get the offset
     * and entry size of the section header
     * table.
     */
    fseek(input_file, 0, SEEK_SET);
    if(fread(&file_header, sizeof(ELF32_Header_t), 1, input_file) != 1)
    {
        return RET_NOT_OK;
    }


    /*
     * Read the ELF file starting at the offset to
     * the section header table into the pointer
     * to the previously allocated section header
     * table passed to the function.
     */
    fseek(input_file, file_header.e_shoff, SEEK_SET);
    if(fread(header_table, file_header.e_shentsize, num_sections, input_file) != num_sections)
    {
        return RET_NOT_OK;
    }

    return RET_OK;
}





/*
 * Reads the section header table for 64-bit ELF files.
 * The input file stream is passed as a parameter to read
 * as well as a pointer to a previously allocated memory
 * buffer large enough to store the entire section header
 * table. The number of section headers is also passed as
 * a parameter, meaning it must be known before calling
 * this function.
 */
int read_ELF64_section_header_table(FILE *input_file, ELF64_Section_Header_t *header_table, ELF32_Half_t num_sections)
{
    ELF64_Header_t file_header;

    /*
     * Defensive check to prevent a
     * segmentation fault from a NULL
     * pointer being passed in.
     */
    if(header_table == NULL)
    {
        fprintf(stderr, "NULL pointer passed to read_ELF64_section_header_table.\n");
        return RET_NOT_OK;
    }


    /*
     * Read the ELF header to get the offset
     * and entry size of the section header
     * table.
     */
    fseek(input_file, 0, SEEK_SET);
    if(fread(&file_header, sizeof(ELF64_Header_t), 1, input_file) != 1)
    {
        return RET_NOT_OK;
    }


    /*
     * Read the ELF file starting at the offset to
     * the section header table into the pointer
     * to the previously allocated section header
     * table passed to the function.
     */
    fseek(input_file, file_header.e_shoff, SEEK_SET);
    if(fread(header_table, file_header.e_shentsize, num_sections, input_file) != num_sections)
    {
        return RET_NOT_OK;
    }

    return RET_OK;
}




/*
 * Reads the program header table for 32-bit ELF files.
 * The input file stream is passed as a parameter to read
 * as well as a pointer to a previously allocated memory
 * buffer large enough to store the entire program header
 * table. The number of program headers or segments is also
 * passed as a parameter, meaning it must be known before
 * calling this function.
 */
int read_ELF32_program_header_table(FILE *input_file, ELF32_Program_Header_t *header_table, ELF32_Half_t num_segments)
{
    ELF32_Header_t file_header;


    /*
     * Defensive check to prevent a
     * segmentation fault from a NULL
     * pointer being passed in.
     */
    if(header_table == NULL)
    {
        fprintf(stderr, "NULL pointer passed to read_ELF32_program_header_table.\n");
        return RET_NOT_OK;
    }


    /*
     * Read the ELF header to get the offset
     * and entry size of the program header
     * table.
     */
    fseek(input_file, 0, SEEK_SET);
    if(fread(&file_header, sizeof(ELF32_Header_t), 1, input_file) != 1)
    {
        return RET_NOT_OK;
    }


    /*
     * Read the ELF file starting at the offset to
     * the program header table into the pointer
     * to the previously allocated program header
     * table passed to the function.
     */
    fseek(input_file, file_header.e_phoff, SEEK_SET);
    if(fread(header_table, file_header.e_phentsize, num_segments, input_file) != num_segments)
    {
        return RET_NOT_OK;
    }

    return RET_OK;
}




/*
 * Reads the program header table for 64-bit ELF files.
 * The input file stream is passed as a parameter to read
 * as well as a pointer to a previously allocated memory
 * buffer large enough to store the entire program header
 * table. The number of program headers or segments is also
 * passed as a parameter, meaning it must be known before
 * calling this function.
 */
int read_ELF64_program_header_table(FILE *input_file, ELF64_Program_Header_t *header_table, ELF64_Half_t num_segments)
{
    ELF64_Header_t file_header;


    /*
     * Defensive check to prevent a
     * segmentation fault from a NULL
     * pointer being passed in.
     */
    if(header_table == NULL)
    {
        fprintf(stderr, "NULL pointer passed to read_ELF64_program_header_table.\n");
        return RET_NOT_OK;
    }


    /*
     * Read the ELF header to get the offset
     * and entry size of the program header
     * table.
     */
    fseek(input_file, 0, SEEK_SET);
    if(fread(&file_header, sizeof(ELF64_Header_t), 1, input_file) != 1)
    {
        return RET_NOT_OK;
    }
    

    /*
     * Read the ELF file starting at the offset to
     * the program header table into the pointer
     * to the previously allocated program header
     * table passed to the function.
     */
    fseek(input_file, file_header.e_phoff, SEEK_SET);
    if(fread(header_table, file_header.e_phentsize, num_segments, input_file) != num_segments)
    {
        return RET_NOT_OK;
    }

    return RET_OK;
}





