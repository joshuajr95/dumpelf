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
        return -1;
    }

    return 0;
}


/*
 * Gets the file's class from the identification
 * bits of the ELF header. Class is either 32-bit,
 * 64-bit, or none.
 */
int get_file_class(FILE *input_file)
{
    unsigned char e_ident[EI_NIDENT];

    read_ELF_identification(input_file, &e_ident);

    return (int) e_ident[EI_CLASS];
}





