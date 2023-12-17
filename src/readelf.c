/* Author: Joshua Jacobs-Rebhun
 * Date: December 7, 2023
 * 
 * 
 * Implementation file for reading the ELF file.
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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



static char **get_ELF32_section_names(FILE *input_file)
{


    return NULL;
}



static char **get_ELF64_section_names(FILE *input_file)
{
    ELF64_Header_t file_header;
    ELF64_Section_Header_t *section_header_table;
    char **section_names;
    ELF64_Off_t string_table_offset;
    ELF64_Word_t string_table_size;
    char *buffer;


    /*
     * Read the file header. The file header contains
     * the index into the section header table of the
     * section header string table section
     */
    if(read_ELF64_header(input_file, &file_header) != RET_OK)
    {
        fprintf(stderr, "Unable to read ELF header.\n");
        return NULL;
    }


    if((section_header_table = read_ELF64_section_header_table(input_file)) == NULL)
    {
        return NULL;
    }


    /*
     * Get the offset into the ELF file of the section
     * header string table from the section header at
     * the index given by the section header string table
     * index member of the ELF header.
     */
    string_table_offset = section_header_table[file_header.e_shstrndx].sh_offset;


    /*
     * Must dynamically allocate one extra string since
     * the final element in the string array is a NULL
     * pointer.
     */
    section_names = (char**) malloc(sizeof(char*) * (file_header.e_shnum + 1));
    section_names[file_header.e_shnum] = NULL;


    /*
     * Read the entire section header string table
     * into a memory buffer for easier access when
     * extracting strings from it.
     */
    string_table_size = section_header_table[file_header.e_shstrndx].sh_size;
    buffer = (char*) malloc(string_table_size);
    fseek(input_file, string_table_offset, SEEK_SET);
    fread(buffer, string_table_size, 1, input_file);


    /*
     * Extract the section names from the string
     * table.
     */
    for(int i = 0; i < file_header.e_shnum; i++)
    {
        // index into string table gives name of section
        ELF64_Word_t index = section_header_table[i].sh_name;

        // allocate new string based on length in string table
        section_names[i] = (char*) malloc(strlen(buffer+index));

        // copy from string table into newly allocated string
        strcpy(section_names[i], buffer+index);
    }


    free(buffer);


    return section_names;
}





char **get_section_names(FILE *input_file)
{
    char **section_names;
    int fileclass = get_file_class(input_file);


    switch (fileclass)
    {
    case ELFCLASS32:
        section_names = get_ELF32_section_names(input_file);
        break;
    
    case ELFCLASS64:
        section_names = get_ELF64_section_names(input_file);
        break;
    
    default:
        break;
    }


    return section_names;
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
 * The function reads the section header table from the
 * file and returns a pointer to a dynamically-allocated
 * array of section header structs which contain the
 * file section headers. Since this function does not
 * return the number of section headers, that must be known
 * beforehand or read from the ELF header.
 */
ELF32_Section_Header_t *read_ELF32_section_header_table(FILE *input_file)
{
    ELF32_Header_t file_header;
    ELF32_Section_Header_t *section_header_table;


    /*
     * Defensive check against NULL
     * file pointer being passed in.
     */
    if(input_file == NULL)
    {
        fprintf(stderr, "No file given to read_ELF32_section_header_table.\n");
        return NULL;
    }


    /*
     * Read the ELF header to get the offset
     * and entry size of the section header
     * table.
     */
    fseek(input_file, 0, SEEK_SET);
    if(fread(&file_header, sizeof(ELF32_Header_t), 1, input_file) != 1)
    {
        return NULL;
    }


    /*
     * Section header table must be dynamically allocated
     */
    section_header_table = (ELF32_Section_Header_t*) malloc(file_header.e_shentsize*file_header.e_shnum);


    /*
     * Read the ELF file starting at the offset to
     * the section header table into the pointer
     * to the previously allocated section header
     * table.
     */
    fseek(input_file, file_header.e_shoff, SEEK_SET);
    if(fread(section_header_table, file_header.e_shentsize, file_header.e_shnum, input_file) != file_header.e_shnum)
    {
        free(section_header_table);
        return NULL;
    }


    return section_header_table;
}





/*
 * Reads the section header table for 64-bit ELF files.
 * The function reads the section header table from the
 * file and returns a pointer to a dynamically-allocated
 * array of section header structs which contain the
 * file section headers. Since this function does not
 * return the number of section headers, that must be known
 * beforehand or read from the ELF header.
 */
ELF64_Section_Header_t *read_ELF64_section_header_table(FILE *input_file)
{
    ELF64_Header_t file_header;
    ELF64_Section_Header_t *section_header_table;


    /*
     * Defensive check in case NULL file
     * pointer is passed to function.
     */
    if(input_file == NULL)
    {
        fprintf(stderr, "NULL pointer passed to read_ELF64_section_header_table.\n");
        return NULL;
    }


    /*
     * Read the ELF header to get the offset
     * and entry size of the section header
     * table.
     */
    fseek(input_file, 0, SEEK_SET);
    if(fread(&file_header, sizeof(ELF64_Header_t), 1, input_file) != 1)
    {
        return NULL;
    }


    /*
     * Section header table must be dynamically allocated
     */
    section_header_table = (ELF64_Section_Header_t*) malloc(file_header.e_shentsize*file_header.e_shnum);


    /*
     * Read the ELF file starting at the offset to
     * the section header table into the pointer
     * to the previously allocated section header
     * table.
     */
    fseek(input_file, file_header.e_shoff, SEEK_SET);
    if(fread(section_header_table, file_header.e_shentsize, file_header.e_shnum, input_file) != file_header.e_shnum)
    {
        free(section_header_table);
        return NULL;
    }


    return section_header_table;
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
ELF32_Program_Header_t *read_ELF32_program_header_table(FILE *input_file)
{
    ELF32_Header_t file_header;
    ELF32_Program_Header_t *program_header_table;


    /*
     * Defensive check to prevent a
     * segmentation fault from a NULL
     * pointer being passed in.
     */
    if(input_file == NULL)
    {
        fprintf(stderr, "NULL pointer passed to read_ELF32_program_header_table.\n");
        return NULL;
    }


    /*
     * Read the ELF header to get the offset
     * and entry size of the program header
     * table.
     */
    fseek(input_file, 0, SEEK_SET);
    if(fread(&file_header, sizeof(ELF32_Header_t), 1, input_file) != 1)
    {
        return NULL;
    }


    /*
     * Program header table must be dynamically-allocated.
     */
    program_header_table = (ELF32_Program_Header_t*) malloc(file_header.e_phentsize*file_header.e_phnum);


    /*
     * Read the ELF file starting at the offset to
     * the program header table into the pointer
     * to the previously allocated program header
     * table passed to the function.
     */
    fseek(input_file, file_header.e_phoff, SEEK_SET);
    if(fread(program_header_table, file_header.e_phentsize, file_header.e_phnum, input_file) != file_header.e_phnum)
    {
        free(program_header_table);
        return NULL;
    }

    return program_header_table;
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
ELF64_Program_Header_t *read_ELF64_program_header_table(FILE *input_file)
{
    ELF64_Header_t file_header;
    ELF64_Program_Header_t *program_header_table;


    /*
     * Defensive check to prevent a
     * segmentation fault from a NULL
     * pointer being passed in.
     */
    if(input_file == NULL)
    {
        fprintf(stderr, "NULL pointer passed to read_ELF64_program_header_table.\n");
        return NULL;
    }


    /*
     * Read the ELF header to get the offset
     * and entry size of the program header
     * table.
     */
    fseek(input_file, 0, SEEK_SET);
    if(fread(&file_header, sizeof(ELF64_Header_t), 1, input_file) != 1)
    {
        return NULL;
    }


    program_header_table = (ELF64_Program_Header_t*) malloc(file_header.e_phentsize*file_header.e_phnum);
    

    /*
     * Read the ELF file starting at the offset to
     * the program header table into the pointer
     * to the previously allocated program header
     * table passed to the function.
     */
    fseek(input_file, file_header.e_phoff, SEEK_SET);
    if(fread(program_header_table, file_header.e_phentsize, file_header.e_phnum, input_file) != file_header.e_phnum)
    {
        free(program_header_table);
        return NULL;
    }

    return program_header_table;
}





