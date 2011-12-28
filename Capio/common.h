#ifndef _common_h
#define _common_h

using namespace std;

#define INF 0x7fffffff

bool is_number(string);
bool is_word(string);
bool is_bool(string);
bool is_list(string);
bool is_empty(string);
string num_to_string(int);
string num_to_string(double);

typedef int type_of_state;
typedef pair<type_of_state, string> type_of_return;

#define STATE_OK 0
#define STATE_EOF 1
#define STATE_ERROR 2
#define STATE_OK_WITH_VALUE 100
#define STATE_OK_WITH_OUTPUT 200

#endif