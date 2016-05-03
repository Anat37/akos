//sample for copy \ empty constructors

#import <stdio.h>
#import <stdlib.h>

class Rect
{
public:
    Rect()
    {
        printf("Rect: empty constructor\n");
    }
    Rect(const Rect& sample)
    {
        printf("Rect: copy constructor\n");
    }
};

class NextRect:Rect
{
public:
    NextRect()
    {
        printf("NextRect: empty constructor\n");
    }
    NextRect(const NextRect& sample)
    {
        printf("NextRect: copy constructor\n");
    }
};

int main()
{
    NextRect a;
    NextRect b(a);
}