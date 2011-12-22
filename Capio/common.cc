#include <iostream>
#include <sstream>
#include <cctype>
#include "common.h"

using namespace std;

bool is_number(string str)
{
    bool has_point = false;
    bool ret = true;
    for (string::iterator iter = str.begin(); iter != str.end(); ++iter)
    {
        if (*iter == '.')
        {
            if (has_point)
            {
                ret = false;
                break;
            }
            else
            {
                has_point = true;
            }
        }
        else if (isdigit(*iter))
        {
        }
        else
        {
            ret = false;
            break;
        }
    }
    
    return ret;
}

string num_to_string(int number)
{
    stringstream ss;
    ss << number;
    return ss.str();
}

string num_to_string(double number)
{
    stringstream ss;
    ss << number;
    return ss.str();
}