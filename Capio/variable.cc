#include <iostream>
#include <map>
#include <ctime>
#include <cstdlib>
#include "common.h"
#include "variable.h"

using namespace std;

type_of_variable_map map_of_variable;

void init_variable()
{
    srand((unsigned)time(NULL));
}

void set_variable(string name, string value)
{
    pair<type_of_variable_map::iterator, bool> ret;
    ret = map_of_variable.insert(type_of_variable(name, value));
    if (ret.second == false)
    {
        //exists already
        ret.first->second = value;
    }
}

type_of_return get_variable(string name)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    
    type_of_variable_map::iterator it;
    it = map_of_variable.find(name);
    if (it == map_of_variable.end())
    {
        ret.first = STATE_ERROR;
    }
    else
    {
        ret.second = it->second;
    }
    
    return ret;
}