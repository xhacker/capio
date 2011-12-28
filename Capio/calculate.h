#ifndef _calculate_h
#define _calculate_h

#include <iostream>
#include <vector>
#include "common.h"
#include "variable.h"

#define NOT_OP      0
#define OP_EQUAL    11
#define OP_LESS     12
#define OP_MORE     13
#define OP_ADD      21
#define OP_MINUS    22
#define OP_MULTI    23
#define OP_DIVIDE   24

#define NOT_PARENTHESIS 0
#define PARENTHESIS_L   1
#define PARENTHESIS_R   2

int is_operator(string);
int is_parenthesis(string);
type_of_return calc(type_of_variable_map &);
type_of_return calc_list(vector<string>);
type_of_return calc_list(vector<string>, int, int);
string do_operate(type_of_return, string, type_of_return);
int find_parenthesis_r(vector<string>, int);

#endif
