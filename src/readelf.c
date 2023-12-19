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
    ELF32_Header_t file_header;
    ELF32_Section_Header_t *section_header_table;
    char **section_names;
    ELF32_Off_t string_table_offset;
    ELF32_Word_t string_table_size;
    char *buffer;


    /*
     * Read the file header. The file header contains
     * the index into the section header table of the
     * section header string table section
     */
    if(read_ELF32_header(input_file, &file_header) != RET_OK)
    {
        fprintf(stderr, "Unable to read ELF header.\n");
        return NULL;
    }


    if((section_header_table = read_ELF32_section_header_table(input_file)) == NULL)
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
        ELF32_Word_t index = section_header_table[i].sh_name;

        // allocate new string based on length in string table
        section_names[i] = (char*) malloc(strlen(buffer+index));

        // copy from string table into newly allocated string
        strcpy(section_names[i], buffer+index);
    }


    free(buffer);


    return section_names;
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





/*
 * Given the input file, returns a NULL pointer
 * terminated list of strings, each of which gives
 * the name of a given section in the file. The NULL
 * termination is to allow for finding the end of
 * the list without explicitly passing the length.
 */
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




char ***get_ELF32_section_to_segment_mapping(FILE *input_file)
{
    return NULL;        // TODO: Implement 32-bit version
}



char ***get_ELF64_section_to_segment_mapping(FILE *input_file)
{
    ELF64_Header_t file_header;
    ELF64_Section_Header_t *section_header_table;
    ELF64_Program_Header_t *program_header_table;

    char **section_names;
    char ***section_to_segment_mapping;
    

    // ELF header has important information
    fseek(input_file, 0, SEEK_SET);
    fread(&file_header, sizeof(file_header), 1, input_file);


    /*
     * Get the section headers, program headers, and section
     * header names to create the mapping.
     */
    section_header_table = read_ELF64_section_header_table(input_file);
    program_header_table = read_ELF64_program_header_table(input_file);
    section_names = get_section_names(input_file);


    // allocate space for mapping
    section_to_segment_mapping = malloc(sizeof(*section_to_segment_mapping));


    /*
     * To keep track of which sections
     * have already been added to a
     * segment.
     */
    bool already_added[file_header.e_shnum];


    // build the mapping one program header at a time
    for(int i = 0; i < file_header.e_phnum; i++)
    {
        ELF64_Off_t segment_offset = program_header_table[i].p_offset;
        ELF64_Xword_t segment_size = program_header_table[i].p_filesz;


        /* 
         * To avoid repeated reallocation, simply
         * allocate per program header an array
         * of char* pointers of size equal to the
         * number of section headers (plus 1 for
         * the terminating NULL pointer), since the
         * number of sections in a given segment
         * cannot be more than the total number of
         * sections in the file.
         */
        section_to_segment_mapping[i] = malloc(sizeof(*(section_to_segment_mapping[i]))*(file_header.e_shnum + 1));


        /*
         * Index of the next element in the section
         * to segment mapping array to add a section
         * name to.
         */
        int current_segment_index = 0;


        for(int j = 0; j < file_header.e_shnum; j++)
        {
            if(already_added[j] == true)
                continue;
            
            ELF64_Off_t section_offset = section_header_table[j].sh_offset;
            ELF64_Xword_t section_size = section_header_table[j].sh_size;

            if( (section_offset >= segment_offset) && (section_offset + section_size <= segment_offset + segment_size) )
            {
                char *section_name = section_names[j];

                section_to_segment_mapping[i][current_segment_index] = malloc(sizeof(char)*(strlen(section_name)+1));

                strcpy(section_to_segment_mapping[i][current_segment_index], section_name);
                current_segment_index++;
            }
        }

        section_to_segment_mapping[i][current_segment_index] = NULL;
    }


    free(section_header_table);
    free(program_header_table);

    for(int i = 0; i < file_header.e_shnum; i++)
    {
        free(section_names[i]);
    }

    free(section_names);


    return section_to_segment_mapping;
}



/*
 * Gets the section to segment mapping. It is returned in the form of
 * a list of NULL pointer terminated lists of (NULL character terminated)
 * strings (kinda confusing, right?). The returned data type is an array
 * with one element per program header, where each element is itself an array
 * of strings, where each string is the name of a section contained
 * in the given segment. Thus, index 4 of the array would have an array of
 * section names that are to be placed in segment 4 at run time.
 * 
 * The NULL termination allows the user to find the end of the list without
 * having to have an explicitlength for each index of the list. The top-level
 * list (list of string lists) does not have any NULL termination since
 * its length is the same as the number of program headers (segments),
 * which is readily available to the user.
 */
char ***get_section_to_segment_mapping(FILE *input_file)
{
    char ***section_to_segment_mapping;
    int file_class = get_file_class(input_file);


    switch (file_class)
    {
    case ELFCLASS32:
        section_to_segment_mapping = get_ELF32_section_to_segment_mapping(input_file);
        break;
    
    case ELFCLASS64:
        section_to_segment_mapping = get_ELF64_section_to_segment_mapping(input_file);
        break;
    
    default:
        break;
    }


    return section_to_segment_mapping;
}




