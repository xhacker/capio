#ifndef _common_h
#define _common_h

using namespace std;

#define INFINITY 0x7fffffff

bool is_number(string);

typedef int type_of_state;
typedef pair<type_of_state, string> type_of_return;

#define STATE_OK 0
#define STATE_EOF 1
#define STATE_ERROR 2
#define STATE_OK_WITH_VALUE 100

#endif