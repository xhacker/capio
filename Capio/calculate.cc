#include <iostream>
#include <vector>
#include "calculate.h"
#include "common.h"
#include "command.h"
#include "variable.h"
#include "io.h"

int is_operator(string op)
{
    if (op == "+")
    {
        return OP_ADD;
    }
    else if (op == "-")
    {
        return OP_MINUS;
    }
    else if (op == "*")
    {
        return OP_MULTI;
    }
    else if (op == "/")
    {
        return OP_DIVIDE;
    }
    else if (op == "=")
    {
        return OP_EQUAL;
    }
    else if (op == "<")
    {
        return OP_LESS;
    }
    else if (op == ">")
    {
        return OP_MORE;
    }
    else
    {
        return NOT_OP;
    }
    return NOT_OP;
}

int is_parenthesis(string op)
{
    if (op == "(")
    {
        return PARENTHESIS_L;
    }
    else if (op == ")")
    {
        return PARENTHESIS_R;
    }
    else
    {
        return NOT_PARENTHESIS;
    }
    return NOT_PARENTHESIS;
}

type_of_return calc(type_of_variable_map &map_of_local_variable)
{
    type_of_return ret(STATE_OK, "");
    
    vector<string> elem_list;
    int parenthesis_l = 0;
    while (true)
    {
        string elem = get_input();
        int parenthesis_test = is_parenthesis(elem);
        if (is_number(elem))
        {
            elem_list.push_back(elem);
        }
        else if (parenthesis_test)
        {
            elem_list.push_back(elem);
            if (parenthesis_test == PARENTHESIS_L)
            {
                ++parenthesis_l;
                continue;
            }
            else if (parenthesis_test == PARENTHESIS_R)
            {
                if (parenthesis_l > 0)
                {
                    --parenthesis_l;
                }
                else
                {
                    ret.first = STATE_ERROR;
                    return ret;
                }
            }
        }
        else
        {
            type_of_command_function command_processor = find_command(elem);
            type_of_function_return func_ret = get_function(elem);
            if (command_processor)
            {
                type_of_return command_ret = command_processor(map_of_local_variable);
                if (command_ret.first == STATE_OK_WITH_VALUE)
                {
                    elem_list.push_back(command_ret.second);
                }
                else
                {
                    ret.first = STATE_ERROR;
                    break;
                }
            }
            else if (func_ret.first == STATE_OK_WITH_VALUE)
            {
                string ret_value = function_processor(map_of_local_variable, func_ret.second, elem);
                if (ret_value != "")
                {
                    elem_list.push_back(ret_value);
                }
                else
                {
                    print_error("\"" + elem + "\" has no return value.");
                }
            }
            else
            {
                print_error("\"" + elem + "\" is not a valid command.");
            }
        }
 
        string op = get_input();
        if (is_parenthesis(op)) {
            rollback_input();
        }
        else if (is_operator(op))
        {
            elem_list.push_back(op);
        }
        else
        {
            rollback_input();
            break;
        }
    }
    
    if (parenthesis_l || elem_list.size() == 0)
    {
        print_error("Invalid expression.");
        ret.first = STATE_ERROR;
        return ret;
    }
    
    ret = calc_list(elem_list);
    
    return ret;
}

type_of_return calc_list(vector<string> list)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    ret = calc_list(list, 0, (int)list.size() - 1);
    return ret;
}

type_of_return calc_list(vector<string> list, int start, int end)
{
    type_of_return ret(STATE_OK_WITH_VALUE, "");
    
    if (start == end)
    {
        ret.second = list[start];
        return ret;
    }
    
    if (is_parenthesis(list[start]) == PARENTHESIS_L &&
        find_parenthesis_r(list, start) == end)
    {
        ret = calc_list(list, start + 1, end - 1);
        return ret;
    }
    
    int lowest = INF;
    int lowest_i = -1;
    for (int i = start; i <= end; ++i)
    {
        int op_test = is_operator(list[i]);
        if (op_test) {
            if (op_test < lowest)
            {
                lowest = op_test;
                lowest_i = i;
            }
        }
        int parenthesis_test = is_parenthesis(list[i]);
        if (parenthesis_test)
        {
            int r = find_parenthesis_r(list, i);
            i = r;
        }
    }
    
    ret.second = do_operate(calc_list(list, start, lowest_i - 1),
                            list[lowest_i],
                            calc_list(list, lowest_i + 1, end));
    
    return ret;
}

string do_operate(type_of_return a, string op, type_of_return b)
{
    /* calculate `a op b` */
    double aval = atof(a.second.c_str());
    double bval = atof(b.second.c_str());
    string result = "";
    
    if (op == "+")
    {
        result = num_to_string(aval + bval);
    }
    else if (op == "-")
    {
        result = num_to_string(aval - bval);
    }
    else if (op == "*")
    {
        result = num_to_string(aval * bval);
    }
    else if (op == "/")
    {
        result = num_to_string(aval / bval);
    }
    else if (op == "=")
    {
        result = (aval == bval) ? "TRUE" : "FALSE";
    }
    else if (op == "<")
    {
        result = (aval < bval) ? "TRUE" : "FALSE";
    }
    else if (op == ">")
    {
        result = (aval > bval) ? "TRUE" : "FALSE";
    }
    else
    {
        ;
    }
    
    return result;
}

int find_parenthesis_r(vector<string> list, int left)
{
    int parentheses_l = 1;
    int i, r;
    for (i = left + 1; ; ++i)
    {
        int parenthesis_test = is_parenthesis(list[i]);
        if (parenthesis_test == PARENTHESIS_L)
        {
            ++parentheses_l;
        }
        else if (parenthesis_test == PARENTHESIS_R)
        {
            --parentheses_l;
        }
        if (parentheses_l == 0)
        {
            r = i;
            break;
        }
    }
    return r;
}
