#ifndef _calculate_h
#define _calculate_h

#include <iostream>
#include <vector>
#include "common.h"

#define NOT_OP 0
#define OP_ADD 1
#define OP_MINUS 2
#define OP_MULTI 3
#define OP_DIVIDE 4

int is_operator(string);
type_of_return calc();
type_of_return calc_list(vector<string>);
type_of_return calc_list(vector<string>, int, int);
string do_operate(type_of_return, string, type_of_return);

#endif
