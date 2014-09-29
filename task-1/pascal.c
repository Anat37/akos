#include <stdio.h>
#include <stdlib.h>

#define pascal_int unsigned long long int

int main(){
    int n,i,j;
    scanf("%i",&n);
    n++;
    pascal_int **d_array =(pascal_int**)malloc(n*sizeof(pascal_int*));

    d_array[0] = (pascal_int*)calloc(3,sizeof(pascal_int));
    d_array[0][1] = 1;
    
    for(i = 1;i<n;i++){
        d_array[i] = (pascal_int*)calloc(i+3,sizeof(pascal_int));
        for(j = 1;j<i+2;j++){
            d_array[i][j] = d_array[i-1][j-1]+d_array[i-1][j];
            printf("%llu ",d_array[i][j]);
        }
        printf("\n");
    }

    for(i = 0;i<n;i++)
        free(d_array[i]);
    free(d_array);
    return 0;
}
