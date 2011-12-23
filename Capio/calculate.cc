#include <iostream>
#include <vector>
#include "calculate.h"
#include "common.h"
#include "command.h"
#include "io.h"

int is_operator(string str)
{
    if (str == "+")
    {
        return OP_ADD;
    }
    else if (str == "-")
    {
        return OP_MINUS;
    }
    else if (str == "*")
    {
        return OP_MULTI;
    }
    else if (str == "/")
    {
        return OP_DIVIDE;
    }
    else
    {
        return NOT_OP;
    }
    return NOT_OP;
}

type_of_return calc()
{
    type_of_return ret(STATE_OK, "");
    
    vector<string> elem_list;
    while (true)
    {
        string elem = get_input();
        if (is_number(elem))
        {
            elem_list.push_back(elem);
        }
        else
        {
            type_of_command_function command_processor = find_command(elem);
            if (command_processor == NULL)
            {
                ret.first = STATE_ERROR;
                break;
            }
            else
            {
                type_of_return command_ret = command_processor();
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
        }
 
        string op = get_input();
        if (!is_operator(op))
        {
            rollback_input();
            break;
        }
        else
        {
            elem_list.push_back(op);
        }
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
    double result = 0;
    
    if (op == "+")
    {
        result = aval + bval;
    }
    else if (op == "-")
    {
        result = aval - bval;
    }
    else if (op == "*")
    {
        result = aval * bval;
    }
    else if (op == "/")
    {
        result = aval / bval;
    }
    else
    {
        ;
    }
    
    return num_to_string(result);
}
