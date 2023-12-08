/* Author: Joshua Jacobs-Rebhun
 * Date: December 7, 2023
 *
 * 
 * Implementation file for the commands as well
 * as for the command struct manipulation methods.
 */

#include <stdio.h>


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


int dump_elf_header(FILE *input_file)
{
    ELF32_Header_t header_32;
    ELF64_Header_t header_64;

    char *output_string;


    int file_class = get_file_class(input_file);

    switch (file_class)
    {
    case ELFCLASS32:
        read_ELF32_header(input_file, &header_32);
        output_string = stringify_ELF32_header(&header_32);
        break;
    
    case ELFCLASS64:
        read_ELF64_header(input_file, &header_64);
        output_string = stringify_ELF64_header(&header_64);
        break;

    default:
        printf("ELF file has no class.\n");
        return -1;
        break;
    }

    fprintf(stdout, output_string);
    free(output_string);

    return 0;
}


int dump_section_headers(FILE *input_file)
{
    ELF32_Header_t header_32;
    ELF64_Header_t header_64;

    ELF32_Section_Header_t *section_header_32;
    ELF64_Section_Header_t *section_header_64;

    char *output_string;

    int file_class = get_file_class(input_file);

    switch (file_class)
    {
    case ELFCLASS32:
        read_ELF32_header(input_file, &header_32);
        section_header_32 = (ELF32_Section_Header_t*) malloc(sizeof(ELF32_Section_Header_t)*header_32.e_shnum);

        read_ELF32_section_header_table(input_file, section_header_32);
        output_string = stringify_ELF32_section_headers(section_header_32, header_32.e_shnum);
        break;
    
    case ELFCLASS64:
        read_ELF64_header(input_file, &header_64);
        section_header_64 = (ELF64_Section_Header_t*) malloc(sizeof(ELF64_Section_Header_t)*header_64.e_shnum);

        read_ELF64_section_header_table(input_file, section_header_64);
        output_string = stringify_ELF64_section_headers(section_header_64, header_64.e_shnum);
        break;
    
    default:
        break;
    }

    fprintf(stdout, output_string);
    free(output_string);

    return 0;
}


