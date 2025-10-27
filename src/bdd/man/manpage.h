#ifndef MANPAGE_H
#define MANPAGE_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "../utils.h"


#define MAX_LINES 150
#define LINE_LENGTH 80


extern const char* manual[];
extern const char* help_select[];
extern const char* help_from[];
extern const char* help_where[];
extern const char* help_order_by[];
extern const char* help_limit[];
extern const char* help_offset[];
extern const char* help_createtable[];
extern const char* help_alter_table[];
extern const char* help_operateurslogiques[];
extern const char* help_groupby[];
extern const char* help_having[];


void manpage(const char* manual_text[]);
void submenu();

#endif
