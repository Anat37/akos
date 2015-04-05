#ifndef __COMPLEX_MATRIX__
#define __COMPLEX_MATRIX__

#include "complex.h"
#include <stdlib.h>
#include <stdio.h>

namespace prac_matrices
{
    enum matrix_types {E, I, ONE, ZERO};

    class Proxy
    {
        Complex* matrix;
    public:
        Proxy(Complex *sample, uint64_t size)
        {
            matrix = sample;
        }

        Complex& operator [](uint64_t i)
        {
            return matrix[i];
        }
    };

    class Complex_Matrix
    {
        Complex** matrix;
        uint64_t n;
        uint64_t m;

        void create_matrix(uint64_t n, uint64_t m);
        void resize(uint64_t n, uint64_t m);
        void read_from_text(FILE* f);
        void read_from_binary(FILE* f);
    public:
        Complex_Matrix();
        Complex_Matrix(uint64_t n, uint64_t m, matrix_types type = ZERO);
        Complex_Matrix(const char* filename);
        Complex_Matrix(Complex_Matrix const& sample);
        ~Complex_Matrix();
        uint64_t get_n() const;
        uint64_t get_m() const;
        void print() const;
        void save_to_binary(char* filename) const;
        void save_to_text(char* filename) const;
        Proxy operator[] (uint64_t i);
        Proxy operator[] (uint64_t i) const;
        Complex_Matrix& operator = (Complex_Matrix const& b);
        Complex_Matrix& operator = (Complex_Matrix const& b) const;
        Complex_Matrix operator + (Complex_Matrix b) const;
        Complex_Matrix operator - (Complex_Matrix b) const;
        Complex_Matrix operator += (Complex_Matrix b);
        Complex_Matrix operator += (Complex_Matrix b) const;
        Complex_Matrix operator -= (Complex_Matrix b);
        Complex_Matrix operator -= (Complex_Matrix b) const; 
        Complex_Matrix operator - () const;
        Complex_Matrix operator ~ () const;
        Complex_Matrix operator * (Complex a) const;
        Complex_Matrix operator / (Complex a) const;
        Complex_Matrix operator * (Complex_Matrix b) const;
        Complex_Matrix operator *= (Complex_Matrix b);
        Complex_Matrix operator *= (Complex_Matrix b) const;
        int operator == (Complex_Matrix b) const; 
        
        friend Complex_Matrix operator * (Complex a, Complex_Matrix b);
    };
}

#endif