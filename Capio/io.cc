#include <iostream>
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
        input_buf.push_back(command);
        ret = command;
    }
    else
    {
        ret = input_buf[input_buf_pos];
    }
    
    ++input_buf_pos;

    return ret;
}

void rollback_input()
{
    if (input_buf_pos > 0)
    {
        --input_buf_pos;
    }
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