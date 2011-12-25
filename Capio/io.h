#ifndef _io_h
#define _io_h

#include "common.h"

using namespace std;

string get_input();
void rollback_input();
type_of_state skip_list();

void print_log(string);
void print_error(string);
void print_info(string);
void print_output(string);
void print_br();

#endif
