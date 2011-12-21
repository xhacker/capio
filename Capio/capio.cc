#include <iostream>
#include "command.h"
#include "variable.h"
#include "io.h"

using namespace std;

int main()
{
	print_info("Hello, I am a CapIO LOGO interpreter.");
    print_info("I became operational at Zhejiang University");
    print_info("on the 21st of September 2011.");
    print_info("My instructor was Xhacker Liu.\n");
    print_info("Write something, press Enter to execute.");

    init_command();
    init_variable();
    main_processor();
	
    print_br();
    print_info("Bye!");
	
	return 0;
}

extern type_of_command_map map_of_builtin_command;

void main_processor()
{
    while (true)
    {
        string command = get_input();

        if (command == "")
        {
            break;
        }

        type_of_command_map::iterator it;
        it = map_of_builtin_command.find(command);
        if (it == map_of_builtin_command.end())
        {
            print_error("\"" + command + "\" is not a valid command.");
        }
        else
        {
            (*(it->second))();
        }
    }
}
