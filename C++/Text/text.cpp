#include <iostream>

#include "T_Args.h"
#include "T_String.h"
#include "Lists.h"

using namespace std;

int main(int argc, char** argv)
{
    T_Args args(argc, argv);
    Header tmp(args);
    tmp.append("qwejknrjkndfgjkn");
    tmp.next_level();
    tmp.append("asds");
    tmp.append("zxc");
    tmp.append("zxc");
    tmp.prev_level();
    tmp.append("zxc");
    tmp.append("zxc");
    cout<< tmp << endl;
    /*
    T_List *fp = &tmp;
    cout<< (*fp) << endl;
    */
    return 0;
}