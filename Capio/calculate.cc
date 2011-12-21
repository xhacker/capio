#include <iostream>
#include "calculate.h"
#include "common.h"
#include "command.h"
#include "io.h"

type_of_return calc()
{
    type_of_return ret(STATE_OK, "");
    
    string command = get_input();
    if (is_number(command))
    {
        ret.first = STATE_OK;
        ret.second = command;
    }
    else
    {
        type_of_command_function command_processor = find_command(command);
        if (command_processor == NULL)
        {
            ret.first = STATE_ERROR;
        }
        else
        {
            type_of_return command_ret = command_processor();
            if (command_ret.first == STATE_OK_WITH_VALUE)
            {
                ret.first = STATE_OK;
                ret.second = command_ret.second;
            }
            else
            {
                ret.first = STATE_ERROR;
            }
        }
    }
    
    return ret;
}
