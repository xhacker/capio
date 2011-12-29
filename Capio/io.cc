#include <iostream>
#include <fstream>
#include <vector>
#include "io.h"
#include "common.h"

vector<string> input_buf;
int input_buf_pos = 0;

string get_input()
{
    string ret = "";
    if (input_buf_pos == input_buf.size())
    {
        string command = "";
        cin >> command;
        
        if (command[0] == ':')
        {
            input_buf.push_back("THING");
            input_buf.push_back('"' + command.substr(1));
            ret = "THING";
        }
        else
        {
            input_buf.push_back(command);
            ret = command;
        }
    }
    else
    {
        ret = input_buf[input_buf_pos];
    }
    
    ++input_buf_pos;

    return ret;
}

void load_from_stream(ifstream &filestream)
{
    string elem = "";
    filestream >> elem;
    while (!filestream.eof())
    {
        input_buf.push_back(elem);
        filestream >> elem;
    }
}

void rollback_input()
{
    if (input_buf_pos > 0)
    {
        --input_buf_pos;
    }
}

int get_input_position()
{
    return input_buf_pos;
}

void rollback_input_to_position(int position)
{
    input_buf_pos = position;
}

type_of_state skip_list()
{
    string parenthesis_first = get_input();
    if (parenthesis_first != "[")
    {
        return STATE_ERROR;
    }
    
    int parentheses_l = 1;
    while (parentheses_l)
    {
        string str = get_input();
        if (str == "[")
        {
            ++parentheses_l;
        }
        else if (str == "]")
        {
            --parentheses_l;
        }
    }
    
    return STATE_OK;
}

void print_error(string message)
{
    cout << "[ERROR] " << message << endl;
    cin.ignore(INF, '\n');
}

void print_log(string message)
{
    cout << "[LOG]   " << message << endl;
}

void print_info(string message)
{
    cout << "[INFO]  " << message << endl;
}

void print_output(string message)
{
    cout << message << endl;
}

void print_br()
{
    cout << endl;
}