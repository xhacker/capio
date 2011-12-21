#ifndef _command_h
#define _command_h

#include <map>
#include "common.h"

using namespace std;

typedef type_of_return(*type_of_command_function)();
typedef pair<string, type_of_command_function> type_of_command;
typedef map<string, type_of_command_function> type_of_command_map;

void init_command();

void main_processor();
type_of_return MAKE_processor();
type_of_return THING_processor();
type_of_return PRINT_processor();
type_of_return READLIST_processor();

type_of_return get_word();
type_of_return get_list();
type_of_return get_value();

#endif
