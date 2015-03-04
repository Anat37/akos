#include <stdio.h>
#include <math.h>
#include "complex.h"

using namespace prac_matrices;

Complex::Complex(double re ,double im)
{
    this->re = re;
    this->im = im;
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