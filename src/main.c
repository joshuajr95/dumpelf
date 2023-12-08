/* Author: Joshua Jacobs-Rebhun
 * Date: December 5, 2023
 *
 * 
 * This is the main file for the program that contains the
 * entry point and the highest-level code controlling the flow
 * of the program.
 */


#include <stdio.h>
#include <errno.h>


#include "elf.h"
#include "readelf.h"
#include "debug.h"
#include "commands.h"



// file name and file pointer to
// the specified file
static char *filename;
static FILE *file_handle;


int is_substring(char *string1, char *string2)
{
    int index = 0;
    while( string1[index] != '\0' || string2[index] != '\0' )
    {
        if(string1[index] != string2[index])
        {
            return 0;
        }
    }

    return 1;
}

int is_int(char *string)
{
    while(*string != '\0')
    {
        // if char is out of range 0-9, it is not an integer
        if( (int)(*string) - (int)'0' >= 10 )
        {
            return 0;
        }

        string++;
    }

    return 1;
}


void print_usage_message()
{
    // print the usage message
}


/*
 * Convert the argument in --debug-dump=<argument> to a
 * command subtype.
 */
int get_debug_subtype(char *string)
{
    if(strcmp(string, "abbrev") == 0)
    {
        return DBG_CMD_DUMP_ABBREV;
    }
    else if(strcmp(string, "addr") == 0)
    {
        return DBG_CMD_DUMP_ADDR;
    }
    else if(strcmp(string, "frames") == 0)
    {
        return DBG_CMD_DUMP_FRAMES;
    }
    else if(strcmp(string, "names") == 0)
    {
        return DBG_CMD_DUMP_NAMES;
    }
    else if(strcmp(string, "info") == 0)
    {
        return DBG_CMD_DUMP_INFO;
    }
    else if(strcmp(string, "aranges") == 0)
    {
        return DBG_CMD_DUMP_ARANGES;
    }
    else
    {
        return -1;
    }
}


int parse_command_line_options(int argc, char *argv[], command_list_t *commands)
{

    for(int i = 1; i < argc; i++)
    {
        command_t *new_command = (command_t*)malloc(sizeof(command_t));

        if( (strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--file-header") == 0) )
        {
            new_command->type = CMD_DUMP_ELF_HEADER;
            add_command(commands, new_command);
        }
        else if( (strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "--segments") == 0) || (strcmp(argv[i], "--program-headers") == 0) )
        {
            new_command->type = CMD_DUMP_SECTION_HEADERS;
            add_command(commands, new_command);
        }
        else if( (strcmp(argv[i], "-S") == 0) || (strcmp(argv[i], "--sections") == 0) || (strcmp(argv[i], "--section-headers") == 0) )
        {
            new_command->type = CMD_DUMP_PROGRAM_HEADERS;
            add_command(commands, new_command);
        }
        else if( (strcmp(argv[i], "-e") == 0) || (strcmp(argv[i], "--headers") == 0) )
        {
            new_command->type = CMD_DUMP_ALL_HEADERS;
            add_command(commands, new_command);
        }
        else if( (strcmp(argv[i], "-s") == 0) || (strcmp(argv[i], "--syms") == 0) || (strcmp(argv[i], "--symbols") == 0) )
        {
            new_command->type = CMD_DUMP_SYMBOL_TABLE;
            add_command(commands, new_command);
        }
        else if( (strcmp(argv[i], "-r") == 0) || (strcmp(argv[i], "--relocs") == 0) )
        {
            new_command->type = CMD_DUMP_RELOCATION_INFO;
            add_command(commands, new_command);
        }
        else if(is_substring("--hex-dump=", argv[i]))
        {
            char *ptr;
            new_command->type = CMD_HEX_DUMP_SECTION;
            
            ptr = argv[i] + strlen("--hex-dump=");

            if(is_int(ptr))
            {
                new_command->section_number = atoi(ptr);
            }
            else
            {
                new_command->section_name = (char *)malloc(strlen(ptr)*sizeof(char));
                strcpy(new_command->section_name, ptr, strlen(ptr));
            }

            add_command(commands, new_command);
        }
        else if(is_substring("--string-dump=", argv[i]))
        {
            char *ptr;
            new_command->type = CMD_STRING_DUMP_SECTION;
            
            ptr = argv[i] + strlen("--string-dump=");

            if(is_int(ptr))
            {
                new_command->section_number = atoi(ptr);
            }
            else
            {
                new_command->section_name = (char *)malloc(strlen(ptr)*sizeof(char));
                strcpy(new_command->section_name, ptr, strlen(ptr));
            }

            add_command(commands, new_command);
        }
        else if(is_substring("--debug-dump=", argv[i]))
        {
            char *ptr;
            new_command->type = CMD_DUMP_DEBUG_INFO;

            ptr = argv[i] + strlen("--debug-dump=");

            if((new_command->subtype = get_debug_subtype(ptr)) < 0)
            {
                printf("Unrecognized option to --debug-dump: %s", ptr);
                print_usage_message();
                return -1;
            }

            add_command(commands, new_command);
        }
        else if( argv[i][0] != '-' )
        {
            // any command-line argument without a begining '-'
            // is interpreted as a filename
            filename = argv[i];
        }
        else
        {
            printf("Unrecognized option: %s.\n", argv[i]);
            print_usage_message();
            return -1;
        }
    }

    if (filename == 0)
    {
        printf("Filename not specified.\n");
        print_usage_message();
        return -1;
    }


    return 0;
}




int main(int argc, char *argv[])
{

    command_list_t commands;

    // initialize the command list with size argc since the number
    // of command-line arguments gives an upper bound on the number
    // of commands to dumpelf
    init_command_list(&commands, argc);


    // parse the command-line options
    // if they are not used correctly, then print error message
    if(parse_command_line_options(argc, argv, &commands) != 0)
    {
        return -1;
    }


    // open file and return early if not found
    if((file_handle = fopen(filename, "r")) == 0)
    {
        printf("%s\n", strerror(errno));
        return -1;
    }


    // iterate over all of the different commands passed
    for(int i = 0; i < commands.num_commands; i++)
    {
        switch(commands.command_array[i]->type)
        {
            case CMD_DUMP_ELF_HEADER:
                dump_elf_header();
                break;
            case CMD_DUMP_SECTION_HEADERS:
                dump_section_headers();
                break;
            case CMD_DUMP_PROGRAM_HEADERS:
                dump_program_headers();
                break;
            case CMD_DUMP_SYMBOL_TABLE:
                dump_symbol_table();
                break;
            case CMD_DUMP_RELOCATION_INFO:
                dump_relocation_info();
                break;
            case CMD_HEX_DUMP_SECTION:
                hex_dump_section(commands.command_array[i]->section_number, commands.command_array[i]->section_name);
                break;
            case CMD_STRING_DUMP_SECTION:
                string_dump_section(commands.command_array[i]->section_number, commands.command_array[i]->section_name);
                break;
            case CMD_DUMP_DEBUG_INFO:
                dump_debug_info(commands.command_array[i]->subtype);
                break;
            default:
                printf("Unrecognized option.\n");       // add mapping from command to option specified
                print_usage_message();
                break;
        }
    }


    return 0;
}