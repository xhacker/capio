#ifndef _variable_h
#define _variable_h

typedef pair<string, string> type_of_variable;
typedef map<string, string> type_of_variable_map;

void init_variable();
void set_variable(string, string);
type_of_return get_variable(string);

#endif
