#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include "command.h"
#include "io.h"
#include "common.h"
#include "variable.h"
#include "calculate.h"

using namespace std;

type_of_command_map map_of_builtin_command;
extern type_of_variable_map map_of_variable;

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
    map_of_builtin_command.insert(type_of_command("NUMBERP", NUMBERP_processor));
    map_of_builtin_command.insert(type_of_command("WORDP", WORDP_processor));
    map_of_builtin_command.insert(type_of_command("LISTP", LISTP_processor));
    map_of_builtin_command.insert(type_of_command("EQUALP", EQUALP_processor));
    map_of_builtin_command.insert(type_of_command("NAMEP", NAMEP_processor));
    map_of_builtin_command.insert(type_of_command("WORD", WORD_processor));
    map_of_builtin_command.insert(type_of_command("SENTENCE", SENTENCE_processor));
    map_of_builtin_command.insert(type_of_command("FIRST", FIRST_processor));
    map_of_builtin_command.insert(type_of_command("BUTFIRST", BUTFIRST_processor));
    map_of_builtin_command.insert(type_of_command("TO", TO_processor));
    map_of_builtin_command.insert(type_of_command("ERASE", ERASE_processor));
    map_of_builtin_command.insert(type_of_command("WAIT", WAIT_processor));
    map_of_builtin_command.insert(type_of_command("LOAD", LOAD_processor));
    map_of_builtin_command.insert(type_of_command("SAVE", SAVE_processor));
    
    /* aliases */
    map_of_builtin_command.insert(type_of_command("RAND", RANDOM_processor));
    map_of_builtin_command.insert(type_of_command("SLEEP", WAIT_processor));
    
    /* misc */
    map_of_builtin_command.insert(type_of_command(";", SEPARATOR_processor));
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

type_of_return get_word(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    ret = get_value(map_of_local_variable);
    if (ret.first == STATE_OK_WITH_VALUE)
    {
        if (!is_word(ret.second))
        {
            ret.first = STATE_ERROR;
        }
    }
    return ret;
}

type_of_return get_list(type_of_variable_map &map_of_local_variable)
{
    string command = get_input();
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    
    if (command == "")
    {
        ret.first = STATE_EOF;
    }
    else if (command == "[")
    {
        ret.second = "[";
        type_of_return list_item(STATE_OK_WITH_VALUE, "");
        while (true) {
            list_item = get_value(map_of_local_variable);
            if (list_item.first == STATE_OK_WITH_VALUE) {
                ret.second += (" " + list_item.second);
            }
            else if (list_item.first == STATE_ERROR)
            {
                if (list_item.second == "]") {
                    ret.second += " ]";
                    ret.first = STATE_OK_WITH_VALUE;
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

type_of_return get_number(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    ret = get_value(map_of_local_variable);
    if (ret.first == STATE_OK_WITH_VALUE)
    {
        if (!is_number(ret.second))
        {
            ret.first = STATE_ERROR;
        }
    }
    return ret;
}

type_of_return get_bool(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    ret = get_value(map_of_local_variable);
    if (ret.first == STATE_OK_WITH_VALUE)
    {
        if (!is_bool(ret.second))
        {
            ret.first = STATE_ERROR;
        }
    }
    return ret;
}

type_of_return get_value(type_of_variable_map &map_of_local_variable)
{
    string command = get_input();
    type_of_return ret(STATE_OK_WITH_VALUE, "");

    if (command == "")
    {
        ret.first = STATE_EOF;
    }
    else if (command[0] == '"')
    {
        if (command == "\"")
        {
            ret.second = "\"";
        }
        else
        {
            ret.second = command.substr(1);
        }
    }
    else if (command == "[")
    {
        ret.second = "[";
        type_of_return list_item(STATE_OK_WITH_VALUE, "");
        while (true) {
            list_item = get_value(map_of_local_variable);
            if (list_item.first == STATE_OK_WITH_VALUE) {
                ret.second += (" " + list_item.second);
            }
            else if (list_item.first == STATE_ERROR)
            {
                if (list_item.second == "]") {
                    ret.second += " ]";
                    ret.first = STATE_OK_WITH_VALUE;
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
        ret = calc(map_of_local_variable);
    }

    return ret;
}

string main_processor(type_of_variable_map &map_of_local_variable)
{
    string ret = "";
    
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
        
        if (command == stop_flag || command == "END")
        {
            break;
        }
        
        if (command == "OUTPUT")
        {
            type_of_return output_ret = get_value(map_of_local_variable);
            if (output_ret.first == STATE_OK_WITH_VALUE)
            {
                ret = output_ret.second;
                //print_log("RETURN: " + ret);
            }
            else
            {
                print_error("\"OUTPUT\": Invalid argument, should be a value.");
            }
        }
        else
        {
            type_of_command_function command_processor = find_command(command);
            type_of_function_return func_ret = get_function(command);
            if (command_processor)
            {
                type_of_return command_ret = command_processor(map_of_local_variable);
                if (command_ret.first == STATE_OK_WITH_OUTPUT)
                {
                    ret = command_ret.second;
                }
            }
            else if (func_ret.first == STATE_OK_WITH_VALUE)
            {
                function_processor(map_of_local_variable, func_ret.second, command);
            }
            else
            {
                print_error("\"" + command + "\" is not a valid command.");
            }
        }
    }
    
    return ret;
}

string function_processor(type_of_variable_map &map_of_local_variable, type_of_function func, string func_name)
{
    string ret = "";
    
    type_of_variable_map new_map_of_local_variable;
    unsigned argi;
    bool arg_valid = true;
    for (argi = 0; argi != func.parameters.size(); ++argi)
    {
        type_of_return parameter = get_value(map_of_local_variable);
        if (parameter.first == STATE_OK_WITH_VALUE)
        {
            new_map_of_local_variable.insert(type_of_variable(
                func.parameters[argi],
                parameter.second));
        }
        else
        {
            print_error("\"" + func_name + "\": Invalid argument.");
            arg_valid = false;
            break;
        }
    }
    
    if (arg_valid)
    {
        //print_log("Entering function...");
        
        int current_position = get_input_position();
        rollback_input_to_position(func.start_position);
        ret = main_processor(new_map_of_local_variable);
        rollback_input_to_position(current_position);
        
        //print_log("Exiting function...");
    }
    
    
    return ret;
}

type_of_return MAKE_processor(type_of_variable_map &map_of_local_variable)
{
    string name = "";
    string value = "";
    type_of_return ret(STATE_OK, "");

    type_of_return first_argument_ret = get_word(map_of_local_variable);
    if (first_argument_ret.first == STATE_OK_WITH_VALUE)
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

    type_of_return second_argument_ret = get_value(map_of_local_variable);
    if (second_argument_ret.first == STATE_OK_WITH_VALUE)
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

    if (get_variable(map_of_local_variable, name).first == STATE_OK_WITH_VALUE)
    {
        /* if local has this variable, use local */
        set_variable(map_of_local_variable, name, value);
    }
    else
    {
        if (get_variable(map_of_variable, name).first == STATE_OK_WITH_VALUE)
        {
            /* local does not have this variable but global has, use global */
            set_variable(map_of_variable, name, value);
        }
        else
        {
            /* both local and global do not have this variable, use local */
            set_variable(map_of_local_variable, name, value);
        }
    }
    //print_log("MAKE: " + name + " = " + value);

    ret.first = STATE_OK;
    return ret;
}

type_of_return THING_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");

    type_of_return argument_ret = get_value(map_of_local_variable);
    string value = "";
    if (argument_ret.first == STATE_OK_WITH_VALUE)
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
    
    type_of_return global_ret = get_variable(map_of_variable, value);
    
    if (&map_of_local_variable != &map_of_variable) 
    {
        /* if scope is local, try to get variable from local */
        type_of_return local_ret = get_variable(map_of_local_variable, value);
        if (local_ret.first == STATE_OK_WITH_VALUE)
        {
            ret.second = local_ret.second;
            goto THING_RETURN;
        }
    }
    
    /* if scope is global or local does not have this variable, get variable from global */
    if (global_ret.first == STATE_OK_WITH_VALUE)
    {
        ret.second = global_ret.second;
        goto THING_RETURN;
    }
    else
    {
        ret.first = STATE_ERROR;
        print_error("\"THING\": Variable \"" + value + "\" not defined.");
        return ret;
    }

    
THING_RETURN:
    if (ret.second == "\"")
    {
        ret.second = "";
    }
    ret.first = STATE_OK_WITH_VALUE;

    return ret;
}

type_of_return PRINT_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK, "");
    type_of_return argument_ret = get_value(map_of_local_variable);
    string value = "";
    if (argument_ret.first == STATE_OK_WITH_VALUE)
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

type_of_return READLIST_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    type_of_return list_ret = get_list(map_of_local_variable);
    if (list_ret.first == STATE_OK_WITH_VALUE) {
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

type_of_return AND_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    
    type_of_return first_argument_ret = get_bool(map_of_local_variable);
    if (first_argument_ret.first != STATE_OK_WITH_VALUE)
    {
        print_error("\"AND\": Invalid first argument, should be a bool value.");
        ret.first = STATE_ERROR;
        return ret;
    }

    type_of_return second_argument_ret = get_bool(map_of_local_variable);
    if (second_argument_ret.first != STATE_OK_WITH_VALUE)
    {
        print_error("\"AND\": Invalid second argument, should be a bool value.");
        ret.first = STATE_ERROR;
        return ret;
    }
    
    if (first_argument_ret.second == "TRUE" && second_argument_ret.second == "TRUE")
    {
        ret.second = "TRUE";
    }
    else
    {
        ret.second = "FALSE";
    }
    
    return ret;
}

type_of_return OR_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    
    type_of_return first_argument_ret = get_bool(map_of_local_variable);
    if (first_argument_ret.first != STATE_OK_WITH_VALUE)
    {
        print_error("\"OR\": Invalid first argument, should be a bool value.");
        ret.first = STATE_ERROR;
        return ret;
    }
    
    type_of_return second_argument_ret = get_bool(map_of_local_variable);
    if (second_argument_ret.first != STATE_OK_WITH_VALUE)
    {
        print_error("\"OR\": Invalid second argument, should be a bool value.");
        ret.first = STATE_ERROR;
        return ret;
    }
    
    if (first_argument_ret.second == "TRUE" || second_argument_ret.second == "TRUE")
    {
        ret.second = "TRUE";
    }
    else
    {
        ret.second = "FALSE";
    }
    
    return ret;
}

type_of_return NOT_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    
    type_of_return argument_ret = get_bool(map_of_local_variable);
    if (argument_ret.first != STATE_OK_WITH_VALUE)
    {
        print_error("\"NOT\": Invalid argument, should be a bool value.");
        ret.first = STATE_ERROR;
        return ret;
    }
    
    if (argument_ret.second == "TRUE")
    {
        ret.second = "FALSE";
    }
    else
    {
        ret.second = "TRUE";
    }
    
    return ret;
}

type_of_return IF_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK, "");
    
    type_of_return argument_ret = get_bool(map_of_local_variable);
    if (argument_ret.first != STATE_OK_WITH_VALUE)
    {
        print_error("\"IF\": Invalid first argument, should be a bool value.");
        ret.first = STATE_ERROR;
        return ret;
    }
    
    if (argument_ret.second == "TRUE")
    {
        string ret_value = main_processor(map_of_local_variable);
        if (ret_value != "")
        {
            ret.first = STATE_OK_WITH_OUTPUT;
            ret.second = ret_value;
        }
        if (skip_list() == STATE_ERROR)
        {
            print_error("\"IF\": Invalid third argument, should be a list.");
        }
    }
    else
    {
        if (skip_list() == STATE_ERROR)
        {
            print_error("\"IF\": Invalid second argument, should be a list.");
        }
        string ret_value = main_processor(map_of_local_variable);
        if (ret_value != "")
        {
            ret.first = STATE_OK_WITH_OUTPUT;
            ret.second = ret_value;
        }
    }
    
    return ret;
}

type_of_return REPEAT_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK, "");
    
    type_of_return argument_ret = get_number(map_of_local_variable);
    if (argument_ret.first == STATE_OK_WITH_VALUE)
    {
        int repeat = (int)atof(argument_ret.second.c_str());
        int ri = 0;
        int input_position = get_input_position();
        for (ri = 1; ri <= repeat; ++ri)
        {
            rollback_input_to_position(input_position);
            string ret_value = main_processor(map_of_local_variable);
            if (ret_value != "")
            {
                ret.first = STATE_OK_WITH_OUTPUT;
                ret.second = ret_value;
            }
        }
    }
    else
    {
        print_error("\"REPEAT\": Invalid first argument, should be a number.");
        ret.first = STATE_ERROR;
    }
    
    return ret;
}

type_of_return RANDOM_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    
    type_of_return argument_ret = get_number(map_of_local_variable);
    if (argument_ret.first == STATE_OK_WITH_VALUE) {
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

type_of_return SQRT_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    type_of_return argument_ret = get_number(map_of_local_variable);
    if (argument_ret.first == STATE_OK_WITH_VALUE) {
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

type_of_return INT_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    type_of_return argument_ret = get_number(map_of_local_variable);
    if (argument_ret.first == STATE_OK_WITH_VALUE) {
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

type_of_return NUMBERP_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    type_of_return argument_ret = get_value(map_of_local_variable);
    if (argument_ret.first == STATE_OK_WITH_VALUE) {
        if (is_number(argument_ret.second))
        {
            ret.second = "TRUE";
        }
        else
        {
            ret.second = "FALSE";
        }
    }
    else
    {
        print_error("\"NUMBERP\": Invalid argument, should be a value.");
        ret.first = STATE_ERROR;
    }
    return ret;
}

type_of_return WORDP_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    type_of_return argument_ret = get_value(map_of_local_variable);
    if (argument_ret.first == STATE_OK_WITH_VALUE) {
        if (is_word(argument_ret.second))
        {
            ret.second = "TRUE";
        }
        else
        {
            ret.second = "FALSE";
        }
    }
    else
    {
        print_error("\"WORDP\": Invalid argument, should be a value.");
        ret.first = STATE_ERROR;
    }
    return ret;
}

type_of_return LISTP_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    type_of_return argument_ret = get_value(map_of_local_variable);
    if (argument_ret.first == STATE_OK_WITH_VALUE) {
        if (is_list(argument_ret.second))
        {
            ret.second = "TRUE";
        }
        else
        {
            ret.second = "FALSE";
        }
    }
    else
    {
        print_error("\"LISTP\": Invalid argument, should be a value.");
        ret.first = STATE_ERROR;
    }
    return ret;
}

type_of_return EQUALP_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    
    type_of_return first_argument_ret = get_value(map_of_local_variable);
    if (first_argument_ret.first != STATE_OK_WITH_VALUE)
    {
        print_error("\"EQUALP\": Invalid first argument, should be a value.");
        ret.first = STATE_ERROR;
        return ret;
    }
    
    type_of_return second_argument_ret = get_value(map_of_local_variable);
    if (second_argument_ret.first != STATE_OK_WITH_VALUE)
    {
        print_error("\"EQUALP\": Invalid second argument, should be a value.");
        ret.first = STATE_ERROR;
        return ret;
    }
    
    if (first_argument_ret.second == second_argument_ret.second)
    {
        ret.second = "TRUE";
    }
    else
    {
        ret.second = "FALSE";
    }
    
    return ret;
}

type_of_return NAMEP_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    type_of_return argument_ret = get_word(map_of_local_variable);
    if (argument_ret.first == STATE_OK_WITH_VALUE) {
        type_of_return variable_ret = get_variable(map_of_local_variable, argument_ret.second);
        if (variable_ret.first == STATE_OK_WITH_VALUE)
        {
            ret.second = "TRUE";
        }
        else
        {
            ret.second = "FALSE";
        }
    }
    else
    {
        print_error("\"NAMEP\": Invalid argument, should be a word.");
        ret.first = STATE_ERROR;
    }
    return ret;
}

type_of_return WORD_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    
    type_of_return first_argument_ret = get_word(map_of_local_variable);
    if (first_argument_ret.first != STATE_OK_WITH_VALUE)
    {
        print_error("\"WORD\": Invalid first argument, should be a word.");
        ret.first = STATE_ERROR;
        return ret;
    }
    
    type_of_return second_argument_ret = get_word(map_of_local_variable);
    if (second_argument_ret.first != STATE_OK_WITH_VALUE)
    {
        print_error("\"WORD\": Invalid second argument, should be a word.");
        ret.first = STATE_ERROR;
        return ret;
    }
    
    ret.second = first_argument_ret.second + second_argument_ret.second;
    
    return ret;
}

type_of_return SENTENCE_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    
    type_of_return first_argument_ret = get_value(map_of_local_variable);
    if (first_argument_ret.first != STATE_OK_WITH_VALUE)
    {
        print_error("\"SENTENCE\": Invalid first argument, should be a value.");
        ret.first = STATE_ERROR;
        return ret;
    }
    
    type_of_return second_argument_ret = get_value(map_of_local_variable);
    if (second_argument_ret.first != STATE_OK_WITH_VALUE)
    {
        print_error("\"SENTENCE\": Invalid second argument, should be a value.");
        ret.first = STATE_ERROR;
        return ret;
    }
    
    ret.second = "[ " + first_argument_ret.second + " " + second_argument_ret.second + " ]";
    
    return ret;
}

type_of_return FIRST_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    type_of_return argument_ret = get_value(map_of_local_variable);
    if (argument_ret.first == STATE_OK_WITH_VALUE) {
        if (is_list(argument_ret.second))
        {
            if (is_empty(argument_ret.second))
            {
                ret.second = "\"";
            }
            else
            {
                stringstream ss;
                ss << argument_ret.second.substr(2);
                ss >> ret.second;
            }
        }
        else
        {
            ret.second = argument_ret.second[0];
        }
    }
    else
    {
        print_error("\"FIRST\": Invalid argument, should be a value.");
        ret.first = STATE_ERROR;
    }
    return ret;
}

type_of_return BUTFIRST_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    type_of_return argument_ret = get_value(map_of_local_variable);
    if (argument_ret.first == STATE_OK_WITH_VALUE) {
        if (is_list(argument_ret.second))
        {
            if (is_empty(argument_ret.second))
            {
                ret.second = "[ ]";
            }
            else
            {
                /* 0 should be '[', 1 should be ' '. */
                unsigned i = 0;
                for (i = 2; i != argument_ret.second.size(); ++i)
                {
                    if (argument_ret.second[i] == ' ')
                    {
                        break;
                    }
                }
                ret.second = "[" + argument_ret.second.substr(i);
            }
        }
        else
        {
            ret.second = argument_ret.second.substr(1);
        }
    }
    else
    {
        print_error("\"BUTFIRST\": Invalid argument, should be a value.");
        ret.first = STATE_ERROR;
    }
    return ret;
}

type_of_return TO_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    type_of_return argument_ret = get_word(map_of_local_variable);
    if (argument_ret.first == STATE_OK_WITH_VALUE) {
        vector<string> parameters;
        string left_parenthesis = get_input();
        if (left_parenthesis != "[") {
            print_error("\"TO\": Invalid second argument, should be a parameter list.");
            ret.first = STATE_ERROR;
            return ret;
        }
        while (true)
        {
            string parameter = get_input();
            if (parameter == "]")
            {
                break;
            }
            if (!is_word(parameter))
            {
                print_error("\"TO\": Invalid second argument, should be a parameter list.");
                ret.first = STATE_ERROR;
                return ret;
            }
            parameters.push_back(parameter);
        }
        
        int start = 0;
        start = get_input_position();
        string command;
        do {
            command = get_input();
        } while (command != "END");
        
        set_function(argument_ret.second, parameters, start);
    }
    else
    {
        print_error("\"TO\": Invalid first argument, should be a word.");
        ret.first = STATE_ERROR;
    }
    return ret;
}

type_of_return ERASE_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK, "");
    type_of_return argument_ret = get_word(map_of_local_variable);
    if (argument_ret.first == STATE_OK_WITH_VALUE) {
        remove_variable(map_of_local_variable, argument_ret.second);
    }
    else
    {
        print_error("\"ERASE\": Invalid argument, should be a word.");
        ret.first = STATE_ERROR;
    }
    return ret;
}

type_of_return WAIT_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK, "");
    type_of_return argument_ret = get_number(map_of_local_variable);
    if (argument_ret.first == STATE_OK_WITH_VALUE) {
        ret.first = STATE_OK;
        /* 1 microsecond equals to 1x10E-6 second. */
        int ms = (int)atof(argument_ret.second.c_str());
        print_log("WAIT: " + num_to_string(ms) + " ms.");
        usleep(ms);
    }
    else
    {
        print_error("\"WAIT\": Invalid argument, should be a number.");
        ret.first = STATE_ERROR;
    }
    return ret;
}

type_of_return LOAD_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK, "");
    type_of_return argument_ret = get_word(map_of_local_variable);
    if (argument_ret.first == STATE_OK_WITH_VALUE) {
        string filename = argument_ret.second;
        ifstream file(filename.c_str());
        if (file)
        {
            load_from_stream(file);
            file.close();
        }
        else
        {
            print_error("\"LOAD\": Specified file not exist.");
            ret.first = STATE_ERROR;
        }
    }
    else
    {
        print_error("\"LOAD\": Invalid argument, should be a word.");
        ret.first = STATE_ERROR;
    }
    return ret;
}

type_of_return SAVE_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK, "");
    
    int current_position, save_end_position;
    save_end_position = get_input_position() - 1;
    type_of_return argument_ret = get_word(map_of_local_variable);
    if (argument_ret.first == STATE_OK_WITH_VALUE) {
        string filename = argument_ret.second;
        ofstream file(filename.c_str(), ios::out);
        
        current_position = get_input_position();
        rollback_input_to_position(0);
        for (int i = 0; i < save_end_position; ++i)
        {
            file << get_input() << " ";
        }
        file.close();
        rollback_input_to_position(current_position);
    }
    else
    {
        print_error("\"SAVE\": Invalid argument, should be a word.");
        ret.first = STATE_ERROR;
    }
    
    return ret;
}

type_of_return SEPARATOR_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK, "");
    /* yep it does nothing. */
    return ret;
}

/*
type_of_return FUZZY_processor(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK, "");
    return ret;
}
*/