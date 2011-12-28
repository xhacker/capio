#ifndef _variable_h
#define _variable_h

#include <vector>
#include <map>

typedef pair<string, string> type_of_variable;
typedef map<string, string> type_of_variable_map;
typedef struct
{
    vector<string> parameters;
    int start_position;
} type_of_function;
typedef map<string, type_of_function> type_of_function_map;
typedef pair<type_of_state, type_of_function> type_of_function_return;

void init_variable();
void set_variable(type_of_variable_map &, string, string);
void set_function(string, vector<string>, int);
type_of_return get_variable(type_of_variable_map &, string);
type_of_function_return get_function(string);

#endif
