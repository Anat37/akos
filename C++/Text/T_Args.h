#ifndef __T_ARGS__
#define __T_ARGS__

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>

using namespace std;

class T_Args{
    char *f_v, m_v;
    int w_v, t_v, r_v;
public:
    T_Args(const int &argc, char** argv);
    friend ostream& operator << (ostream &os, const T_Args& tmp);
};

#endif