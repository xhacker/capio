#include <iostream>
#include "io.h"
#include "common.h"

string get_input()
{
    string command = "";
    cin >> command;
    return command;
}

void print_error(string message)
{
    cout << "[ERROR] " << message << endl;
    cin.ignore(INFINITY, '\n');
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