#include "T_Args.h"

T_Args::T_Args(const int &argc, char** argv)
{
    f_v = NULL;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    w_v = w.ws_col;
    t_v = 2;
    m_v = '*';
    r_v = 2;
    
    int c;
    while( (c = getopt(argc,argv,"f:w:t:m:r:hv")) != -1)
    {
        switch(c)
        {
            case 'f':
                f_v = optarg;
                break;
            case 'w':
                w_v = atoi(optarg);    
                break;
            case 't':
                t_v = atoi(optarg);
                break;
            case 'm':
                m_v = optarg[0];
                break;
            case 'r':
                r_v = atoi(optarg);
                break;
            case 'h':
                cout<< "You won\'t find help here, go away!" << endl;
                break;
            case 'v':
                cout<< "version: 1.0" << endl;
                break;
            case '?':
                break;
        }
    }

    if (!f_v)
    {
        cerr << "Requires file name argument!" << endl;
        exit(1);
    }
}

ostream& operator << (ostream &os, const T_Args& tmp)
{
    os << "file name = \'" << tmp.f_v << "\'" << endl;
    os << "screen width = " << tmp.w_v << endl;
    os << "indent = " << tmp.t_v << endl;
    os << "marker = " << tmp.m_v << endl;
    os << "red line = " << tmp.r_v;
    return os;
}