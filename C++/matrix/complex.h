#ifndef __COMPLEX__
#define __COMPLEX__

namespace prac_matrices
{
    class Complex{
    public:
        double re,im;
        
        Complex(double re = 0,double im = 0);
        Complex neg();
        double module();

        void operator = (Complex sample);
        Complex operator / (double sample);
        Complex operator + (Complex sample);
        Complex operator += (Complex sample);
        Complex operator - (Complex sample);
        Complex operator -= (Complex sample);
        Complex operator * (Complex sample);
        Complex operator / (Complex sample);
        int operator == (Complex sample);

        Complex operator -();
    };
}

#endif