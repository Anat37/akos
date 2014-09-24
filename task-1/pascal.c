#include <stdio.h>
#include <stdlib.h>

int main(){
    int n,i,j;
    scanf("%i",&n);
    int **d_array =(int**)malloc(n*sizeof(int*));

    d_array[0] = (int*)calloc(2,sizeof(int));
    d_array[0][1] = 1;
    
    for(i = 1;i<n;i++){
        d_array[i] = (int*)calloc(i+2,sizeof(int));   
        for(j = 1;j<i+2;j++){
            d_array[i][j] = d_array[i-1][j-1]+d_array[i-1][j];
            printf("%i ",d_array[i][j]);
        }
        printf("\n");
    }

    return 0;
}
