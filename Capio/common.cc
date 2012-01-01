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

bool is_word(string str)
{
    int point_count = 0;
    bool all_number = true;
    bool has_digit = false;
    for (string::iterator iter = str.begin(); iter != str.end(); ++iter)
    {
        if (*iter == ' ')
        {
            return false;
        }
        else if (*iter == '.')
        {
            ++point_count;
        }
        else if (!isdigit(*iter))
        {
            all_number = false;
        }
        else
        {
            // is digit
            has_digit = true;
        }
    }
    
    if (str.size() > 0 && all_number && has_digit && point_count <= 1)
    {
        return false;
    }
    else
    {
        return true;
    }
    
    return false;
}

bool is_bool(string str)
{
    if (str == "TRUE" || str == "FALSE")
    {
        return true;
    }
    
    return false;
}

bool is_list(string str)
{
    if (str[0] == '[')
    {
        /* a variable must be a valid variable. */
        return true;
    }
    
    return false;
}

bool is_empty(string str)
{
    if (str == "[ ]" || str == "\"")
    {
        return true;
    }
    
    return false;
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