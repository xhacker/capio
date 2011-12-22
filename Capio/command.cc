#include <iostream>
#include <map>
#include <cmath>
#include <cstdlib>
#include "command.h"
#include "io.h"
#include "common.h"
#include "variable.h"
#include "calculate.h"

using namespace std;

type_of_command_map map_of_builtin_command;

void init_command()
{
    map_of_builtin_command.insert(type_of_command("MAKE", MAKE_processor));
    map_of_builtin_command.insert(type_of_command("THING", THING_processor));
    map_of_builtin_command.insert(type_of_command("PRINT", PRINT_processor));
    map_of_builtin_command.insert(type_of_command("READLIST", READLIST_processor));
    map_of_builtin_command.insert(type_of_command("AND", AND_processor));
    map_of_builtin_command.insert(type_of_command("OR", OR_processor));
    map_of_builtin_command.insert(type_of_command("NOT", NOT_processor));
    map_of_builtin_command.insert(type_of_command("IF", IF_processor));
    map_of_builtin_command.insert(type_of_command("REPEAT", REPEAT_processor));
    map_of_builtin_command.insert(type_of_command("RANDOM", RANDOM_processor));
    map_of_builtin_command.insert(type_of_command("SQRT", SQRT_processor));
    map_of_builtin_command.insert(type_of_command("INT", INT_processor));
    map_of_builtin_command.insert(type_of_command("WAIT", WAIT_processor));
}

type_of_command_function find_command(string command)
{
    type_of_command_map::iterator it;
    it = map_of_builtin_command.find(command);
    if (it == map_of_builtin_command.end())
    {
        return NULL;
    }
    return it->second;
}

type_of_return get_word()
{
    string command = get_input();
    type_of_return ret(STATE_OK, "");

    if (command == "")
    {
        ret.first = STATE_EOF;
    }
    else if (command[0] != '"')
    {
        ret.first = STATE_ERROR;
    }
    else
    {
        ret.first = STATE_OK;
        ret.second = command.substr(1);
    }
    return ret;
}

type_of_return get_list()
{
    string command = get_input();
    type_of_return ret(STATE_OK, "");
    
    if (command == "")
    {
        ret.first = STATE_EOF;
    }
    else if (command == "[")
    {
        ret.second = "[";
        type_of_return list_item(STATE_OK, "");
        while (true) {
            list_item = get_value();
            if (list_item.first == STATE_OK) {
                ret.second += (" " + list_item.second);
            }
            else if (list_item.first == STATE_ERROR)
            {
                if (list_item.second == "]") {
                    ret.second += " ]";
                    ret.first = STATE_OK;
                    break;
                }
                else
                {
                    ret.first = STATE_ERROR;
                    break;
                }
            }
            else
            {
                ;
            }
        }
    }
    else
    {
        ret.first = STATE_ERROR;
    }
    return ret;
}

// TODO
type_of_return get_number()
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    ret = get_value();
    if (ret.first == STATE_OK)
    {
        if (!is_number(ret.second))
        {
            ret.first = STATE_ERROR;
        }
    }
    return ret;
}

// TODO
type_of_return get_bool()
{
    type_of_return ret(STATE_OK, "");
    return ret;
}

type_of_return get_value()
{
    string command = get_input();
    type_of_return ret(STATE_OK, "");

    if (command == "")
    {
        ret.first = STATE_EOF;
    }
    else if (command[0] == '"')
    {
        ret.first = STATE_OK;
        ret.second = command.substr(1);
    }
    else if (command == "[")
    {
        ret.second = "[";
        type_of_return list_item(STATE_OK, "");
        while (true) {
            list_item = get_value();
            if (list_item.first == STATE_OK) {
                ret.second += (" " + list_item.second);
            }
            else if (list_item.first == STATE_ERROR)
            {
                if (list_item.second == "]") {
                    ret.second += " ]";
                    ret.first = STATE_OK;
                    break;
                }
                else
                {
                    ret.first = STATE_ERROR;
                    break;
                }
            }
            else
            {
                ;
            }
        }
    }
    else if (command == "]")
    {
        ret.first = STATE_ERROR;
        ret.second = "]";
    }
    else
    {
        rollback_input();
        ret = calc();
    }

    return ret;
}

void main_processor()
{
    string stop_flag = "";
    string start_flag = "";
    start_flag = get_input();
    if (start_flag == "[")
    {
        stop_flag = "]";
    }
    else
    {
        rollback_input();
    }
    
    while (true)
    {
        string command = get_input();
        
        if (command == stop_flag)
        {
            break;
        }
        
        type_of_command_map::iterator it;
        it = map_of_builtin_command.find(command);
        if (it == map_of_builtin_command.end())
        {
            print_error("\"" + command + "\" is not a valid command.");
        }
        else
        {
            (*(it->second))();
        }
    }
}

type_of_return MAKE_processor()
{
    string name = "";
    string value = "";
    type_of_return ret(STATE_OK, "");

    type_of_return first_argument_ret = get_word();
    if (first_argument_ret.first == STATE_OK)
    {
        name = first_argument_ret.second;
    }
    else if (first_argument_ret.first == STATE_ERROR)
    {
        print_error("\"MAKE\": Invalid first argument, should be a word.");
        ret.first = STATE_ERROR;
        return ret;
    }
    else if (first_argument_ret.first == STATE_EOF)
    {
        ret.first = STATE_EOF;
        return ret;
    }
    else
    {
        ;
    }

    type_of_return second_argument_ret = get_value();
    if (second_argument_ret.first == STATE_OK)
    {
        value = second_argument_ret.second;
    }
    else if (second_argument_ret.first == STATE_ERROR)
    {
        print_error("\"MAKE\": Invalid second argument, should be a value.");
        ret.first = STATE_ERROR;
        return ret;
    }
    else if (second_argument_ret.first == STATE_EOF)
    {
        ret.first = STATE_EOF;
        return ret;
    }
    else
    {
        ;
    }

    set_variable(name, value);
    print_log("MAKE: " + name + " = " + value);

    ret.first = STATE_OK;
    return ret;
}

type_of_return THING_processor()
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");

    type_of_return argument_ret = get_value();
    string value = "";
    if (argument_ret.first == STATE_OK)
    {
        value = argument_ret.second;
    }
    else if (argument_ret.first == STATE_ERROR)
    {
        print_error("\"THING\": Invalid argument, should be a value.");
        ret.first = STATE_ERROR;
        return ret;
    }
    else if (argument_ret.first == STATE_EOF)
    {
        ret.first = STATE_EOF;
        return ret;
    }
    else
    {
        ;
    }
    
    type_of_return result_ret = get_variable(value);
    if (result_ret.first == STATE_OK)
    {
        ret.second = result_ret.second;
    }
    else
    {
        print_error("\"THING\": Variable \"" + value + "\" not defined.");
        ret.first = STATE_ERROR;
        return ret;
    }
    
    ret.first = STATE_OK_WITH_VALUE;

    return ret;
}

type_of_return PRINT_processor()
{
    type_of_return ret(STATE_OK, "");
    type_of_return argument_ret = get_value();
    string value = "";
    if (argument_ret.first == STATE_OK)
    {
        value = argument_ret.second;
    }
    else if (argument_ret.first == STATE_ERROR)
    {
        print_error("\"PRINT\": Invalid argument, should be a value.");
        ret.first = STATE_ERROR;
        return ret;
    }
    else if (argument_ret.first == STATE_EOF)
    {
        ret.first = STATE_EOF;
        return ret;
    }
    else
    {
        ;
    }

    print_output(value);

    ret.first = STATE_OK;
    return ret;
}

type_of_return READLIST_processor()
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    type_of_return list_ret = get_list();
    if (list_ret.first == STATE_OK) {
        ret.first = STATE_OK_WITH_VALUE;
        ret.second = list_ret.second;
    }
    else
    {
        print_error("\"READLIST\": Invalid argument, should be a list.");
        ret.first = STATE_ERROR;
    }
    return ret;
}

// TODO
type_of_return AND_processor()
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    return ret;
}

// TODO
type_of_return OR_processor()
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    return ret;
}

// TODO
type_of_return NOT_processor()
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    return ret;
}

// TODO
type_of_return IF_processor()
{
    type_of_return ret(STATE_OK, "");
    return ret;
}

// TODO
type_of_return REPEAT_processor()
{
    type_of_return ret(STATE_OK, "");
    return ret;
}

type_of_return RANDOM_processor()
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    type_of_return argument_ret = get_number();
    if (argument_ret.first == STATE_OK) {
        ret.first = STATE_OK_WITH_VALUE;
        int rand_max = (int)atof(argument_ret.second.c_str());
        int rand_num = rand() % rand_max;
        ret.second = num_to_string(rand_num);
    }
    else
    {
        print_error("\"RAMDOM\": Invalid argument, should be a number.");
        ret.first = STATE_ERROR;
    }
    return ret;
}

type_of_return SQRT_processor()
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    type_of_return argument_ret = get_number();
    if (argument_ret.first == STATE_OK) {
        ret.first = STATE_OK_WITH_VALUE;
        double number = atof(argument_ret.second.c_str());
        double sqrted = sqrt(number);
        ret.second = num_to_string(sqrted);
    }
    else
    {
        print_error("\"SQRT\": Invalid argument, should be a number.");
        ret.first = STATE_ERROR;
    }
    return ret;
}

type_of_return INT_processor()
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    type_of_return argument_ret = get_number();
    if (argument_ret.first == STATE_OK) {
        ret.first = STATE_OK_WITH_VALUE;
        int number = (int)atof(argument_ret.second.c_str());
        ret.second = num_to_string(number);
    }
    else
    {
        print_error("\"INT\": Invalid argument, should be a number.");
        ret.first = STATE_ERROR;
    }
    return ret;
}

type_of_return WAIT_processor()
{
    type_of_return ret(STATE_OK, "");
    type_of_return argument_ret = get_number();
    if (argument_ret.first == STATE_OK) {
        ret.first = STATE_OK;
        /* 1 microsecond equals to 1x10E-6 second. */
        int ms = (int)atof(argument_ret.second.c_str());
        print_log("SLEEP: " + num_to_string(ms) + " ms.");
        usleep(ms);
    }
    else
    {
        print_error("\"SLEEP\": Invalid argument, should be a number.");
        ret.first = STATE_ERROR;
    }
    return ret;
}

/*
// TODO
type_of_return VOID_processor()
{
    type_of_return ret(STATE_OK, "");
    return ret;
}
*/