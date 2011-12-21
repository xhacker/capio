#include <iostream>
#include <map>
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