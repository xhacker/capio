#ifndef _command_h
#define _command_h

#include <map>
#include "common.h"

using namespace std;

typedef type_of_return(*type_of_command_function)();
typedef pair<string, type_of_command_function> type_of_command;
typedef map<string, type_of_command_function> type_of_command_map;

void init_command();
type_of_command_function find_command(string command);

type_of_return get_word();
type_of_return get_list();
type_of_return get_number();
type_of_return get_bool();
type_of_return get_value();

void main_processor();
type_of_return MAKE_processor();
type_of_return THING_processor();
type_of_return PRINT_processor();
type_of_return READLIST_processor();
type_of_return AND_processor();
type_of_return OR_processor();
type_of_return NOT_processor();
type_of_return IF_processor();
type_of_return REPEAT_processor();
type_of_return RANDOM_processor();
type_of_return SQRT_processor();
type_of_return INT_processor();
type_of_return SLEEP_processor();

#endif
