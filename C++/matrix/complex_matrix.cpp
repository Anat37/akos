#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "complex_matrix.h"

#define SIZE 255

using namespace prac_matrices;

Complex_Matrix::Complex_Matrix()
{
    n = 0;
    m = 0;
    matrix = NULL;
}

Complex_Matrix::Complex_Matrix(uint64_t n, uint64_t m, matrix_types type)
{
    create_matrix(n,m);
    for(uint64_t i = 0; i<n; i++)
    {
        for(uint64_t j = 0; j<m; j++)
        {
            if (type == ONE)
            {
                matrix[i][j].re = 1;
                matrix[i][j].im = 0;
            }
            else
            {
                matrix[i][j].re = 0;
                matrix[i][j].im = 0;
            }
        }
    }

    if ((type == E)&&(n == m))
    {
        for (uint64_t i = 0; i<n; i++)
            matrix[i][i].re = 1;
    }

    if ((type == I)&&(n == m))
    {
        for (uint64_t i = 0; i<n; i++)
            matrix[i][i].im = 1;
    }
}

Complex_Matrix::Complex_Matrix(const char* filename)
{
    char buff[SIZE];
    FILE* f = fopen(filename, "r");
    
    fread(buff, sizeof(char), 14, f);
    buff[14] = '\0';

    if (!strcmp(buff,"Complex Matrix"))
    {
        printf("Text file\n");
        fgets(buff, SIZE, f);
        sscanf(buff, " %llu x %llu", &n, &m);
        create_matrix(n,m);
        read_from_text(f);
    }
    else
    {
        if (!strcmp(buff,"Complex_Matrix"))
        {
            printf("Binary file\n");
            fread(&n, sizeof(uint64_t), 1, f);
            fread(&m, sizeof(uint64_t), 1, f);
            create_matrix(n,m);
            read_from_binary(f);
        }
        else
        {
            fclose(f);
            throw "Can't understand file!\n";
        }
    }
    fclose(f);
}

Complex_Matrix::Complex_Matrix(Complex_Matrix const& sample)
{
    n = sample.n;
    m = sample.m;
    create_matrix(n,m);
    *this = sample;
}

Complex_Matrix::~Complex_Matrix()
{
    for(uint64_t i = 0; i<n; i++)
        free(matrix[i]);
    free(matrix);
}

uint64_t Complex_Matrix::get_n() const
{
    return n;
}

uint64_t Complex_Matrix::get_m() const
{
    return m;
}

void Complex_Matrix::create_matrix(uint64_t n, uint64_t m)
{
    this->n = n;
    this->m = m;
    if ((n==0)&&(m!=0))
        throw "shape mismatch";
    matrix = (Complex**)malloc(sizeof(Complex*)*n);
    if (matrix == NULL)
        throw "memory error";
    for(uint64_t i = 0; i<n; i++)
    {
        matrix[i] = (Complex*)malloc(sizeof(Complex)*m);
        if (matrix[i] == NULL)
            throw "memory error";
    }
}

void Complex_Matrix::resize(uint64_t n, uint64_t m)
{
    if ((this->n == 0)||(this->m == 0))
    {
        create_matrix(n,m);
        return;
    }

    this->n = n;
    this->m = m;
    matrix = (Complex**)realloc(matrix, sizeof(Complex*)*n);
    if (matrix == NULL)
        throw "memory error";
    
    for(uint64_t i = 0; i<n; i++)
    {
        matrix[i] = (Complex*)realloc(matrix[i], sizeof(Complex)*m);
        if (matrix[i] == NULL)
            throw "memory error";
    }
}

void Complex_Matrix::read_from_text(FILE* f)
{
    char buff[SIZE];
    char* tmp;
    uint64_t i = 0, 
        j = 0;

    for( i = 0; i<n; i++)
    {
        fgets(buff, SIZE, f);    
        tmp = strtok(buff,")\n");
        if (!strcmp(tmp,""))
        {
            continue;
        }
        for( j = 0; j<m; j++)
        {
            sscanf(tmp," (%lf, %lf", &matrix[i][j].re, &matrix[i][j].im);
            tmp = strtok(NULL,")\n");
        }
    }
}

void Complex_Matrix::read_from_binary(FILE* f)
{
    double tmp[2];
    uint64_t i,j;
    for(i=0; i<n; i++)
    {
        for(j=0; j<m; j++)
        {
            fread(tmp, sizeof(double), 2, f);
            matrix[i][j].re = tmp[0];
            matrix[i][j].im = tmp[1];
        }
    }
}

void Complex_Matrix::save_to_binary(char* filename) const
{
    char buff[SIZE];
    uint64_t i,j;
    FILE* f = fopen(filename,"wb");
    
    strcpy(buff,"Complex_Matrix");
    fwrite(buff, sizeof(char), 14, f);
    fwrite(&n, sizeof(uint64_t), 1, f);
    fwrite(&m, sizeof(uint64_t), 1, f);

    
    for(i=0; i<n; i++)
    {
        for(j=0; j<m; j++)
        {
            fwrite(&matrix[i][j].re, sizeof(double), 1, f);
            fwrite(&matrix[i][j].im, sizeof(double), 1, f);
        }
    }

    fclose(f);
}

void Complex_Matrix::save_to_text(char* filename) const
{
    char buff[SIZE];
    uint64_t i,
        j;
    FILE* f = fopen(filename, "w");
    sprintf(buff, "Complex Matrix %llu x %llu\n", n, m);
    fputs(buff, f);
    for(i = 0; i<n; i++)
    {
        for(j = 0; j<m-1; j++)
        {
            sprintf(buff, "(%.1lf, %.1lf) ", matrix[i][j].re, matrix[i][j].im);
            fputs(buff, f);
        }
        sprintf(buff, "(%.1lf, %.1lf)", matrix[i][j].re, matrix[i][j].im);
        fputs(buff, f);
        fputs("\n", f);
    }
    fclose(f);
}

void Complex_Matrix::print() const
{
    for (uint64_t i = 0; i<n; i++)
    {
        for(uint64_t j = 0; j<m ; j++)
            printf("(%2.2lf, %2.2lf) ", matrix[i][j].re, matrix[i][j].im);
        printf("\n");
    }
}

Proxy Complex_Matrix::operator [] (uint64_t i)
{
    //if (i>n)
        //throw "Shape mismatch"
    return Proxy(matrix[i],m);
}

Proxy Complex_Matrix::operator[] (uint64_t i) const
{
    throw "You cant change const matrix!\n";
}

Complex_Matrix& Complex_Matrix::operator = (Complex_Matrix const& b)
{
    if ((this->n != b.get_n())||(this->m != b.get_m()))
        resize(b.get_n(), b.get_m());

    for (uint64_t i = 0; i<n; i++)
        for (uint64_t j = 0; j<m; j++)
            (*this)[i][j] = b.matrix[i][j];

    return *this;
}

Complex_Matrix& Complex_Matrix::operator = (Complex_Matrix const& b) const
{
    throw "You cant change const matrix!\n";
}

Complex_Matrix Complex_Matrix::operator + (Complex_Matrix b) const
{
    if ((n!=b.n)||(m!=b.m))
        throw "Shape mismatch";

    Complex_Matrix tmp(n,m);
    for (uint64_t i = 0; i<n; i++)
        for (uint64_t j = 0; j<m; j++)
            tmp[i][j] = (*this).matrix[i][j]+b[i][j];
    return tmp;
}

Complex_Matrix Complex_Matrix::operator += (Complex_Matrix b)
{
    if ((n!=b.n)||(m!=b.m))
        throw "Shape mismatch";

    Complex_Matrix c(n,m);
    for (uint64_t i = 0; i<n; i++)
        for (uint64_t j = 0; j<m; j++)
            (*this)[i][j] += b[i][j];
    return *this;
}

Complex_Matrix Complex_Matrix::operator += (Complex_Matrix b) const
{
    throw "You cant change const matrix!\n";
}


Complex_Matrix Complex_Matrix::operator - (Complex_Matrix b) const
{
    if ((n!=b.n)||(m!=b.m))
        throw "Shape mismatch";

    Complex_Matrix tmp(n,m);
    for (uint64_t i = 0; i<n; i++)
        for (uint64_t j = 0; j<m; j++)
            tmp[i][j] = (*this).matrix[i][j]-b[i][j];
    return tmp;
}

Complex_Matrix Complex_Matrix::operator -= (Complex_Matrix b)
{
    if ((n!=b.n)||(m!=b.m))
        throw "Shape mismatch";

    Complex_Matrix c(n,m);
    for (uint64_t i = 0; i<n; i++)
        for (uint64_t j = 0; j<m; j++)
            (*this)[i][j] -= b[i][j];
    return *this;
}

Complex_Matrix Complex_Matrix::operator -= (Complex_Matrix b) const
{
    throw "You cant change const matrix!\n";
}

Complex_Matrix Complex_Matrix::operator - () const
{
    Complex_Matrix tmp(n,m);
    for (uint64_t i = 0; i<n; i++)
        for (uint64_t j = 0; j<m; j++)
            tmp[i][j] = - (*this).matrix[i][j];
    return tmp;
}

Complex_Matrix Complex_Matrix::operator ~ () const
{
    Complex_Matrix tmp(m,n);
    for (uint64_t i = 0; i<n; i++)
        for(uint64_t j = 0; j<m; j++)
            tmp[j][i] = (*this).matrix[i][j];
    return tmp;
}

Complex_Matrix Complex_Matrix::operator * (Complex a) const
{
    Complex_Matrix tmp(n,m);
    for (uint64_t i = 0; i<n; i++)
        for(uint64_t j = 0; j<m; j++)  
            tmp[i][j] = (*this).matrix[i][j]*a;
    return tmp;
}

Complex_Matrix Complex_Matrix::operator / (Complex a) const
{
    Complex_Matrix tmp(n,m);
    for (uint64_t i = 0; i<n; i++)
        for(uint64_t j = 0; j<m; j++)  
            tmp[i][j] = (*this).matrix[i][j]/a;
    return tmp;
}

Complex_Matrix prac_matrices::operator * (Complex a, Complex_Matrix b)
{
    uint64_t n = b.get_n(), m = b.get_m();
    Complex_Matrix tmp(n, m);
    for (uint64_t i = 0; i<n; i++)
        for(uint64_t j = 0; j<m; j++)  
            tmp[i][j] = b[i][j]*a;
    return tmp;
}

Complex_Matrix Complex_Matrix::operator * (Complex_Matrix b) const
{
    if (m!=b.n)
        throw "Shape mismatch";

    Complex_Matrix tmp(n,b.m);
    for (uint64_t i = 0; i<n; i++)
        for (uint64_t j = 0; j<b.m; j++)
            for (uint64_t k = 0; k<m; k++)
                tmp[i][j]+= (*this).matrix[i][k]*b[k][j];
    return tmp;
}

Complex_Matrix Complex_Matrix::operator *= (Complex_Matrix b)
{
    if (m!=b.n)
        throw "Shape mismatch";

    Complex_Matrix tmp(n,b.m);
    for (uint64_t i = 0; i<n; i++)
        for (uint64_t j = 0; j<b.m; j++)
            for (uint64_t k = 0; k<m; k++)
                tmp[i][j]+= (*this)[i][k]*b[k][j];
    *this = tmp;
    return *this;
}

Complex_Matrix Complex_Matrix::operator *= (Complex_Matrix b) const
{
    throw "You cant change const matrix!\n";
}

int Complex_Matrix::operator == (Complex_Matrix b) const
{
    if ((n!=b.n)||(m!=b.m))
        return 0;

    for (uint64_t i = 0; i<n; i++)
        for (uint64_t j = 0; j<m; j++)
            if (!( (*this).matrix[i][j] == b[i][j] ))
                return 0;
    return 1;
}





