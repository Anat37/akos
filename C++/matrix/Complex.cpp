#include <stdio.h>
#include <math.h>

class Complex{
    double re,im;
public:
    Complex(double in_re = 0,double in_im = 0);
    void print();
    Complex neg();
    double module();

    void operator=(Complex sample);
    Complex operator+(double sample);
    Complex operator-(double sample);
    Complex operator*(double sample);
    Complex operator/(double sample);
    
    Complex operator+(Complex sample);
    Complex operator-(Complex sample);
    Complex operator*(Complex sample);
    Complex operator/(Complex sample);
};

Complex::Complex(double re,double im)
{
    this->re = re;
    this->im = im;
}

char* Complex::print()
{
    char tmp[128]
    printf("%.2lf + i*%.2lf\n",re,im);
}

Complex Complex::neg()
{
    Complex tmp(this->re,-this->im);
    return tmp;
}

double Complex::module()
{
    return sqrt(this->re*this->re+this->im*this->im);
}

void Complex::operator=(Complex sample)
{
    this->re = sample.re;
    this->im = sample.im;
}

Complex Complex::operator+(double sample)
{
    Complex tmp(this->re+sample, this->im);
    return tmp;
}

Complex Complex::operator-(double sample)
{
    Complex tmp(this->re - sample, this->im);
    return tmp;
}

Complex Complex::operator*(double sample)
{
    Complex tmp(this->re*sample,this->im*sample);
    return tmp;
}

Complex Complex::operator/(double sample)
{
    Complex tmp(this->re/sample, this->im/sample);
    return tmp;
}

Complex Complex::operator+(Complex sample)
{
    Complex tmp(this->re+sample.re, this->im+sample.im);
    return tmp;
}

Complex Complex::operator-(Complex sample)
{
    Complex tmp(this->re - sample.re, this->im - sample.im);
    return tmp;
}

Complex Complex::operator*(Complex sample)
{
    Complex tmp(this->re*sample.re-this->im*sample.im,this->im*sample.re+this->re*sample.im);
    return tmp;
}

Complex Complex::operator/(Complex sample)
{
    return (*this)*sample.neg()/(sample*sample.neg()).re;
}

int main()
{
    Complex a(1,2),b(3,4);
    Complex c = b;
    c.print();
    printf("%0.2lf\n",c.module());
    return 0;
}