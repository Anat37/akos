#include "complex_matrix.h"
#include <stdio.h>
#include <stdlib.h>

using namespace prac_matrices;

int main()
{
    try
    {
        const Complex_Matrix a("example_text");
        Complex_Matrix b;
        
        /*
        Complex c;
        Complex d=1;

        c=a[1][1];
        a.print();
        a[2][2]=c;
        a[1][1]=d;
        a.print(); 
        */ 

        //b = ~a * a * 2;
        b.print();
        a.print();

        //Complex_Matrix b(10,10,ONE);
        //b.print();

        //a.save_to_binary((char*)"example_bin");
        //a.save_to_text((char*)"example_text1");
        //Complex_Matrix b((char*)"example_text");
        //Complex_Matrix b(a.get_m(),a.get_n());
        //printf("%i\n",a == a/2);
        //b.print();
        //a.print();

    }
    catch(char const * buff)
    {
        printf("%s\n", buff);
    };
    return 0;
}