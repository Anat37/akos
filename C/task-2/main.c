#include <stdio.h>
#include <stdlib.h>

#define LEN 26+26+10+1
#define LOWER 0
#define UPPER 26
#define NUMBER 52
#define SPACE 62
#define STR_SIZE 256

int main(){
    char *c = (char*)malloc(STR_SIZE);  
    int min_i,shift,i = 0;
    int *times_repeated = (int*)calloc(LEN,sizeof(int));

    scanf ("%[^\n]%*c", c);

    while(c[i]!='\0'){
        if ((c[i]>='A')&&(c[i]<='Z')){
            times_repeated[UPPER + (c[i] - (int)'A')]++;
        }
        if ((c[i]>='a')&&(c[i]<='z')){
            times_repeated[LOWER + (c[i] - (int)'a')]++;
        }
        if ((c[i]>='0')&&(c[i]<='9')){
            times_repeated[NUMBER + (c[i] -(int)'0')]++;
        }
        if (c[i]==' '){
            times_repeated[SPACE]++;
        }
        i++;
    }
    
    i = 0;
    while(i<LEN){
        if (times_repeated[i]){
            min_i = i;
            i++;
            break;
        }
        i++;
    }
    for(;i<LEN;i++){
        if ((times_repeated[i])&&(times_repeated[i]<times_repeated[min_i])){
            min_i = i;
        }
    }
    
    shift = (min_i<26)?(int)'a'-LOWER:
        (min_i<52)?(int)'A'-UPPER:
            (min_i<62)?(int)'0'-NUMBER:(int)' '-SPACE;
    printf("Most rare symbol is: '%c'\n", min_i+shift);
    free(c);
    free(times_repeated);
    return 0;
}
