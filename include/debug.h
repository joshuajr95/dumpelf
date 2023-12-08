/* Author: Joshua Jacobs-Rebhun
 * Date: December 7, 2023
 *
 * 
 * Contains definitions of types and declarations of
 * functions relating to dumping debug info.
 */


#ifndef DEBUG_H
#define DEBUG_H

/*
 * Subtype of the command. This is used for
 * only commands that dump the debugging info.
 * Since there are many different debugging sections,
 * you need to specify which debug info you want to dump.
 */
typedef enum
{
    DBG_CMD_DUMP_ABBREV,
    DBG_CMD_DUMP_ADDR,
    DBG_CMD_DUMP_FRAMES,
    DBG_CMD_DUMP_NAMES,
    DBG_CMD_DUMP_INFO,
    DBG_CMD_DUMP_ARANGES

} debug_command_subtype;



#endif