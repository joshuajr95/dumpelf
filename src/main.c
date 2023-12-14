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
#include <string.h>
#include <stdlib.h>


#include "elf.h"
#include "readelf.h"
#include "debug.h"
#include "commands.h"



void finish_up_and_free_things()
{
    // free pointers that are dynamically allocated
}



/*
 * Checks whether EITHER string1 is a substring
 * of string2 OR string2 is a substring of string1.
 */
int is_substring(char *string1, char *string2)
{
    int index = 0;
    while( string1[index] != '\0' || string2[index] != '\0' )
    {
        if(string1[index] != string2[index])
        {
            return false;
        }
    }

    return true;
}


/*
 * Checks whether a given string can be converted to
 * an integer using atoi(). This is used because atoi's
 * behavior when passed a non-convertable string such as
 * "hello3" is undefined.
 */
int is_int(char *string)
{
    while(*string != '\0')
    {
        // if char is out of range 0-9, it is not an integer
        if( (int)(*string) - (int)'0' >= 10 )
        {
            return false;
        }

        string++;
    }

    return true;
}


/*
 * Prints the usage message to the screen. This
 * is invoked whenever the user passes incorrect
 * input or commands to dumpelf.
 */
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
        return RET_NOT_OK;
    }
}


int parse_command_line_options(int argc, char *argv[], command_list_t *commands, char *filename)
{

    
    for(int i = 1; i < argc; i++)
    {
        command_t *new_command = (command_t*)malloc(sizeof(command_t));

        if( (strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--file-header") == 0) )
        {
            new_command->type = CMD_DUMP_ELF_HEADER;
            new_command->invoking_option = argv[i];
            add_command(commands, new_command);
        }

        else if( (strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "--segments") == 0) || (strcmp(argv[i], "--program-headers") == 0) )
        {
            new_command->type = CMD_DUMP_PROGRAM_HEADERS;
            new_command->invoking_option = argv[i];
            add_command(commands, new_command);
        }

        else if( (strcmp(argv[i], "-S") == 0) || (strcmp(argv[i], "--sections") == 0) || (strcmp(argv[i], "--section-headers") == 0) )
        {
            new_command->type = CMD_DUMP_SECTION_HEADERS;
            new_command->invoking_option = argv[i];
            add_command(commands, new_command);
        }

        else if( (strcmp(argv[i], "-e") == 0) || (strcmp(argv[i], "--headers") == 0) )
        {
            new_command->type = CMD_DUMP_ALL_HEADERS;
            new_command->invoking_option = argv[i];
            add_command(commands, new_command);
        }

        else if( (strcmp(argv[i], "-s") == 0) || (strcmp(argv[i], "--syms") == 0) || (strcmp(argv[i], "--symbols") == 0) )
        {
            new_command->type = CMD_DUMP_SYMBOL_TABLE;
            new_command->invoking_option = argv[i];
            add_command(commands, new_command);
        }

        else if( (strcmp(argv[i], "-r") == 0) || (strcmp(argv[i], "--relocs") == 0) )
        {
            new_command->type = CMD_DUMP_RELOCATION_INFO;
            new_command->invoking_option = argv[i];
            add_command(commands, new_command);
        }

        else if(is_substring("--hex-dump=", argv[i]))
        {
            if(strlen("--hex-dump=") == strlen(argv[i]))
            {
                fprintf(stderr, "Unspecified section for --hex-dump.\n");
                print_usage_message();
                finish_up_and_free_things();
                return RET_NOT_OK;
            }

            char *ptr;
            new_command->type = CMD_HEX_DUMP_SECTION;
            new_command->invoking_option = argv[i];
            
            ptr = argv[i] + strlen("--hex-dump=");

            if(is_int(ptr))
            {
                new_command->section_number = atoi(ptr);
            }
            else
            {
                new_command->section_name = (char *)malloc(strlen(ptr)*sizeof(char));
                strncpy(new_command->section_name, ptr, strlen(ptr)+1);
            }

            add_command(commands, new_command);
        }

        else if(is_substring("--string-dump=", argv[i]))
        {
            if(strlen("--string-dump=") == strlen(argv[i]))
            {
                fprintf(stderr, "Unspecified section for --string-dump.\n");
                print_usage_message();
                finish_up_and_free_things();
                return RET_NOT_OK;
            }

            char *ptr;
            new_command->type = CMD_STRING_DUMP_SECTION;
            new_command->invoking_option = argv[i];
            
            ptr = argv[i] + strlen("--string-dump=");

            if(is_int(ptr))
            {
                new_command->section_number = atoi(ptr);
            }
            else
            {
                new_command->section_name = (char *)malloc(strlen(ptr)*sizeof(char));
                strncpy(new_command->section_name, ptr, strlen(ptr)+1);
            }

            add_command(commands, new_command);
        }

        else if(is_substring("--debug-dump=", argv[i]))
        {
            if(strlen("--debug-dump=") == strlen(argv[i]))
            {
                fprintf(stderr, "Unspecified section for --debug-dump.\n");
                print_usage_message();
                finish_up_and_free_things();
                return RET_NOT_OK;
            }

            char *ptr;
            new_command->type = CMD_DUMP_DEBUG_INFO;
            new_command->invoking_option = argv[i];

            ptr = argv[i] + strlen("--debug-dump=");

            if((new_command->subtype = get_debug_subtype(ptr)) < 0)
            {
                printf("Unrecognized option to --debug-dump: %s", ptr);
                print_usage_message();
                finish_up_and_free_things();
                return RET_NOT_OK;
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
            finish_up_and_free_things();
            return RET_NOT_OK;
        }
    }

    if (filename == NULL)
    {
        printf("Filename not specified.\n");
        print_usage_message();
        finish_up_and_free_things();
        return RET_NOT_OK;
    }


    return RET_OK;
}




int main(int argc, char *argv[])
{
    // file name and file pointer to
    // the specified file
    char *filename = NULL;
    FILE *file_handle = NULL;


    command_list_t commands;


    /*
     * initialize the command list with size argc since the number
     * of command-line arguments gives an upper bound on the number
     * of commands to dumpelf
     */
    init_command_list(&commands, argc);


    /*
     * parse the command-line options
     * if they are not used correctly, then print error message
     */
    if(parse_command_line_options(argc, argv, &commands, filename) != RET_OK)
    {
        return RET_NOT_OK;
    }


    // try to open file and return early if not found
    if((file_handle = fopen(filename, "r")) == NULL)
    {
        printf("%s\n", strerror(errno));
        return RET_NOT_OK;
    }


    // iterate over all of the different commands passed
    for(int i = 0; i < commands.num_commands; i++)
    {
        switch(commands.command_array[i]->type)
        {
            case CMD_DUMP_ELF_HEADER:
                dump_elf_header(file_handle);
                break;
            case CMD_DUMP_SECTION_HEADERS:
                dump_section_headers(file_handle);
                break;
            case CMD_DUMP_PROGRAM_HEADERS:
                dump_program_headers(file_handle);
                break;
            case CMD_DUMP_SYMBOL_TABLE:
                dump_symbol_table(file_handle);
                break;
            case CMD_DUMP_RELOCATION_INFO:
                dump_relocation_info(file_handle);
                break;
            case CMD_HEX_DUMP_SECTION:
                hex_dump_section(file_handle, commands.command_array[i]->section_number, commands.command_array[i]->section_name);
                break;
            case CMD_STRING_DUMP_SECTION:
                string_dump_section(file_handle, commands.command_array[i]->section_number, commands.command_array[i]->section_name);
                break;
            case CMD_DUMP_DEBUG_INFO:
                dump_debug_info(file_handle, commands.command_array[i]->subtype);
                break;
            default:
                print_usage_message();
                finish_up_and_free_things();
                return RET_NOT_OK;
                break;
        }
    }


    return RET_OK;
}