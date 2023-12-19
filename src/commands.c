/* Author: Joshua Jacobs-Rebhun
 * Date: December 7, 2023
 *
 * 
 * Implementation file for the commands as well
 * as for the command struct manipulation methods.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "commands.h"
#include "readelf.h"
#include "stringify.h"



/*
 * Initializes the command list pointed to by the list parameter
 * by allocating space for a number of command_t structs given
 * by the max_size parameter. Max_size is not the number of
 * commands given but is the maximum possible number of commands.
 */
void init_command_list(command_list_t *list, int max_size)
{
    // allocate array of command_t pointers
    list->command_array = (command_t**) malloc(sizeof(command_t*)*max_size);
    list->array_size = max_size;
    list->num_commands = 0;
}


/*
 * Adds a given command
 */
void add_command(command_list_t *list, command_t *command)
{
    list->command_array[list->num_commands] = command;
    list->num_commands++;
}


/*
 * Remove the command at the specified index and return it
 * to the caller. The number of commands is decremented by
 * one and each command after the given index is moved forward
 * by one.
 */
command_t *remove_command(command_list_t *list, int index)
{
    command_t *removed_command;
    removed_command = list->command_array[index];

    // shift every command after the index forward by one
    for(int i = index+1; i < list->num_commands; i++)
    {
        list->command_array[i-1] = list->command_array[i];
    }
    
    /*
     * Since command at the last index is now at the second
     * to last index, the last index can be cleared.
     */ 
    list->command_array[list->num_commands-1] = 0;

    list->num_commands--;

    return removed_command;
}


/*
 * Basically just recursively frees the memory
 * allocated for the command.
 */
void release_command(command_t *command)
{
    if(command->invoking_option != 0)
        free(command->invoking_option);
    
    if(command->section_name != 0)
        free(command->section_name);
    
    free(command);
}


/*
 * Frees any commands that are not already freed
 * and then frees the command array itself.
 */
void release_command_list(command_list_t *list)
{
    for(int i = 0; i < list->array_size; i++)
    {
        if(list->command_array[i] != 0)
            release_command(list->command_array[i]);
    }

    free(list->command_array);
}


/******************************************
 * This section has all of the functions 
 * implementing the commands given to
 * dumpelf.
 ******************************************/



static char *dump_ELF32_header(FILE *input_file)
{
    ELF32_Header_t file_header;

    if(read_ELF32_header(input_file, &file_header) != RET_OK)
    {
        fprintf(stderr, "Unable to read ELF32 file header.\n");
        return NULL;
    }

    return stringify_ELF32_header(&file_header);
}



static char *dump_ELF64_header(FILE *input_file)
{
    ELF64_Header_t file_header;

    if(read_ELF64_header(input_file, &file_header) != RET_OK)
    {
        fprintf(stderr, "Unable to read ELF64 file header.\n");
        return NULL;
    }

    return stringify_ELF64_header(&file_header);
}


int dump_elf_header(FILE *input_file)
{
    char *output_string;
    int file_class = get_file_class(input_file);


    switch (file_class)
    {
    case ELFCLASS32:
        output_string = dump_ELF32_header(input_file);
        break;
    
    case ELFCLASS64:
        output_string = dump_ELF64_header(input_file);
        break;

    default:
        printf("ELF file has no class.\n");
        return RET_NOT_OK;
        break;
    }

    if(output_string == NULL)
    {
        return RET_NOT_OK;
    }


    fputs(output_string, stdout);
    free(output_string);

    return RET_OK;
}





static char *dump_ELF32_section_headers(FILE *input_file)
{
    ELF32_Header_t file_header;
    ELF32_Section_Header_t *section_header_table;
    char **section_names;
    char *output_string;


    /*
     * Get the ELF file header which is needed by
     * the stringify function.
     */
    if(read_ELF32_header(input_file, &file_header) != RET_OK)
    {
        fprintf(stderr, "Unable to read ELF32 section headers.\n");
        return NULL;
    }


    /*
     * Read the section header table from the ELF
     * file and return pointer to heap-allocated
     * memory containing the section header table.
     */
    if((section_header_table = read_ELF32_section_header_table(input_file)) == NULL)
    {
        fprintf(stderr, "Unable to read ELF32 section headers.\n");
        return NULL;
    }


    /*
     * Get the section names from the section header
     * string table, since they are not stored in
     * the section header table themselves.
     */
    if((section_names = get_section_names(input_file)) == NULL)
    {
        fprintf(stderr, "Unable to find section names.\n");
        free(section_header_table);
        return NULL;
    }


    /*
     * Stringify the section headers. Output string
     * will be printed to user.
     */
    output_string = stringify_ELF32_section_header_table(section_header_table, &file_header, section_names);
    

    // free dynamically-allocated memory
    free(section_header_table);
    for(int i = 0; section_names[i] != NULL; i++)
    {
        free(section_names[i]);
    }
    free(section_names);


    return output_string;
}


static char *dump_ELF64_section_headers(FILE *input_file)
{
    ELF64_Header_t file_header;
    ELF64_Section_Header_t *section_header_table;
    char *output_string;
    char **section_names;


    /*
     * Get the ELF file header which is needed by
     * the stringify function.
     */
    if(read_ELF64_header(input_file, &file_header) != RET_OK)
    {
        fprintf(stderr, "Unable to read ELF64 section header table.\n");
        return NULL;
    }


    /*
     * Read the section header table from the ELF
     * file and return pointer to heap-allocated
     * memory containing the section header table.
     */
    if((section_header_table = read_ELF64_section_header_table(input_file)) == NULL)
    {
        fprintf(stderr, "Unable to read ELF64 section header table.\n");
        return NULL;
    }


    /*
     * Get the section names from the section header
     * string table, since they are not stored in
     * the section header table themselves.
     */
    if((section_names = get_section_names(input_file)) == NULL)
    {
        fprintf(stderr, "Unable to get section names.\n");
        free(section_header_table);
        return NULL;
    }


    /*
     * Stringify the section headers. Output string
     * will be printed to user.
     */
    output_string = stringify_ELF64_section_header_table(section_header_table, &file_header, section_names);


    // free dynamically allocated memory
    free(section_header_table);
    for(int i = 0; section_names[i] != NULL; i++)
    {
        free(section_names[i]);
    }
    free(section_names);


    return output_string;
}



int dump_section_headers(FILE *input_file)
{
    char *output_string;
    int file_class = get_file_class(input_file);


    /*
     * Since the data structures for 32-bit
     * and 64-bit ELF files differ slightly
     * there are different handler functions
     * for the two different classes of ELF
     * file. The handler functions are almost
     * identical except for 32-bit vs. 64-bit.
     */
    switch (file_class)
    {
    case ELFCLASS32:
        output_string = dump_ELF32_section_headers(input_file);
        break;
    
    case ELFCLASS64:
        output_string = dump_ELF64_section_headers(input_file);
        break;
    
    default:
        printf("ELF file has no class.\n");
        return RET_NOT_OK;
        break;
    }


    /*
     * The handler functions will return
     * a NULL pointer on failure.
     */
    if(output_string == NULL)
    {
        return RET_NOT_OK;
    }


    /*
     * Print to screen and free the
     * output string.
     */
    fputs(output_string, stdout);
    free(output_string);

    return RET_OK;
}



static char *dump_ELF32_program_headers(FILE *input_file)
{
    ELF32_Header_t file_header;
    ELF32_Program_Header_t *program_header_table;
    char *output_string;


    char ***section_to_segment_mapping;


    /*
     * Get the ELF file header which is needed by
     * the stringify function.
     */
    if(read_ELF32_header(input_file, &file_header) != RET_OK)
    {
        fprintf(stderr, "Unable to read ELF32 program header table.\n");
        return NULL;
    }

    
    /*
     * Read the program header table into a dynamically
     * allocated array of program headers. 
     */
    if((program_header_table = read_ELF32_program_header_table(input_file)) == NULL)
    {
        fprintf(stderr, "Unable to read ELF32 program header table.\n");
        return NULL;
    }


    section_to_segment_mapping = get_section_to_segment_mapping(input_file);


    /*
     * Stringify the program headers and free
     * the dynamically-allocated memory.
     */
    output_string = stringify_ELF32_program_header_table(program_header_table, &file_header, section_to_segment_mapping);
    free(program_header_table);

    return output_string;
}



static char *dump_ELF64_program_headers(FILE *input_file)
{
    ELF64_Header_t file_header;
    ELF64_Program_Header_t *program_header_table;
    char *output_string;
    
    
    char ***section_to_segment_mapping;


    /*
     * Get the ELF file header which is needed by
     * the stringify function.
     */
    if(read_ELF64_header(input_file, &file_header) != RET_OK)
    {
        fprintf(stderr, "Unable to read ELF64 program header table.\n");
        return NULL;
    }


    if((program_header_table = read_ELF64_program_header_table(input_file)) == NULL)
    {
        fprintf(stderr, "Unable to read ELF64 program header table.\n");
        return NULL;
    }


    /*
     * Get the section to segment mapping 
     */
    section_to_segment_mapping = get_section_to_segment_mapping(input_file);


    output_string = stringify_ELF64_program_header_table(program_header_table, &file_header, section_to_segment_mapping);
    free(program_header_table);

    return output_string;
}



int dump_program_headers(FILE *input_file)
{
    char *output_string;
    int file_class = get_file_class(input_file);


    switch(file_class)
    {
        case ELFCLASS32:
            output_string = dump_ELF32_program_headers(input_file);
            break;
        
        case ELFCLASS64:
            output_string = dump_ELF64_program_headers(input_file);
            break;
        
        default:
            printf("ELF file has no class.\n");
            return RET_NOT_OK;
            break;
    }

    if(output_string == NULL)
    {
        return RET_NOT_OK;
    }

    fputs(output_string, stdout);
    free(output_string);

    return RET_OK;
}



int dump_symbol_table(FILE *input_file)
{
    fprintf(stderr, "TODO: Dump the symbol table.\n");
    return RET_NOT_OK;
}



int dump_relocation_info(FILE *input_file)
{
    fprintf(stderr, "TODO: Dump the relocation info.\n");
    return RET_NOT_OK;
}



int hex_dump_section(FILE *input_file, int section_number, char *section_name)
{
    fprintf(stderr, "TODO: Hex dump a particular section.\n");
    return RET_NOT_OK;
}



int string_dump_section(FILE *input_file, int section_number, char *section_name)
{
    fprintf(stderr, "TODO: String dump a particular section.\n");
    return RET_NOT_OK;
}



int dump_debug_info(FILE *input_file, debug_command_subtype subtype)
{
    fprintf(stderr, "TODO: Dump a section of debugging symbols.\n");
    return RET_NOT_OK;
}

