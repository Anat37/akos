#include <iostream>

#include "Types.h"

using namespace std;


int main(int argc, char** argv)
{ 
    Parser lex(argc, argv);
    try
    {
        lex.load();
        lex.print();
        cout<<endl;
        lex.collect();
    }
    catch(string e)
    {
        cout<< e << endl;
    }
    catch(const char* e)
    {
        cout<< e <<endl;
    }

    return 0;
}
