
#ifndef COMMANDS_H
#define COMMANDS_H


/*
 * This enumerated type is used to define the type
 * of command issued to dumpelf. A command is specified
 * on the command line by a given option or flag, for
 * example --sections, which tells dumpelf to dump a given
 * item in the ELF file (the section header table in the
 * above example).
 */
typedef enum
{
    CMD_DUMP_ELF_HEADER,
    CMD_DUMP_SECTION_HEADERS,
    CMD_DUMP_PROGRAM_HEADERS,
    CMD_DUMP_ALL_HEADERS,
    CMD_DUMP_SYMBOL_TABLE,
    CMD_DUMP_RELOCATION_INFO,
    CMD_HEX_DUMP_SECTION,
    CMD_STRING_DUMP_SECTION,
    CMD_DUMP_DEBUG_INFO

} command_type;



typedef enum
{
    DBG_CMD_DUMP_FRAMES

} debug_command_subtype;



typedef struct
{
    // type of the given command
    command_type type;

    /*
     * This is the subtype of command
     * the debug dump command. This is
     * only used if the type is CMD_DUMP_DEBUG_INFO
     */
    debug_command_subtype subtype;

    /*
     * These are only used if the command to
     * dump a given section (hex or string dump)
     * is given to dumpelf.
     */
    int section_number;
    char *section_name;

} command_t;



/*
 * This struct contains a list of commands issued by the
 * user to dumpelf.
 */
typedef struct
{

    int num_commands;
    int array_size;
    
    command_t **command_array;

} command_list_t;


/*
 * Functions for initializing, releasing, and manipulating
 * commands and command lists.
 */
void init_command_list(command_list_t *list, int max_size);
void add_command(command_list_t *list, command_t *command);
command_t *remove_command(command_list_t *list, int index);
void release_command(command_t *command);
void release_command_list(command_list_t *list);



/*
 * Functions for implementing the actual commands.
 */
int dump_elf_header();
int dump_section_headers();
int dump_program_headers();
int dump_symbol_table();
int dump_relocation_info();
int hex_dump_section(int section_number, char *section_name);
int string_dump_section(int section_number, char *section_name);




#endif
