#include <iostream>
#include <map>
#include <ctime>
#include <cstdlib>
#include "common.h"
#include "variable.h"

using namespace std;

type_of_variable_map map_of_variable;
type_of_function_map map_of_function;

void init_variable()
{
    srand((unsigned)time(NULL));
}

void set_variable(type_of_variable_map &map_of_local_variable, string name, string value)
{
    pair<type_of_variable_map::iterator, bool> ret;
    ret = map_of_local_variable.insert(type_of_variable(name, value));
    if (ret.second == false)
    {
        //exists already
        ret.first->second = value;
    }
}

void set_function(string name, vector<string> parameters, int start_position)
{
    pair<type_of_function_map::iterator, bool> ret;
    type_of_function func;
    func.parameters = parameters;
    func.start_position = start_position;
    ret = map_of_function.insert(pair<string, type_of_function>(name, func));
    if (ret.second == false)
    {
        //exists already
        ret.first->second = func;
    }
}

type_of_return get_variable(type_of_variable_map &map_of_local_variable, string name)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    
    type_of_variable_map::iterator it;
    it = map_of_local_variable.find(name);
    if (it == map_of_local_variable.end())
    {
        ret.first = STATE_ERROR;
    }
    else
    {
        ret.second = it->second;
    }
    
    return ret;
}

type_of_function_return get_function(string name)
{
    type_of_function_return ret;
    
    type_of_function_map::iterator it;
    it = map_of_function.find(name);
    if (it == map_of_function.end())
    {
        ret.first = STATE_ERROR;
        return ret;
    }
    ret.first = STATE_OK_WITH_VALUE;
    ret.second = it->second;
    return ret;
}
