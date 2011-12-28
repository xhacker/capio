#include <iostream>
#include "command.h"
#include "variable.h"
#include "io.h"

using namespace std;

extern type_of_variable_map map_of_variable;

int main()
{
	print_info("Hello, I am a CapIO LOGO interpreter.");
    print_info("I became operational at Zhejiang University");
    print_info("on the 21st of September 2011.");
    print_info("My instructor was Xhacker Liu.\n");
    print_info("Write something, press Enter to execute.");

    init_command();
    init_variable();
    main_processor(map_of_variable);
	
    print_br();
    print_info("Bye!");
	
	return 0;
}
