#include <stdio.h>

typedef int Field[512][128];

Field f;

void init(){

    for (int i = 0; i<512; i++){
        for (int j = 0; j< 128; j++){
            f[i][j] = 0;
        }
    }
    for (int i=0; i<= 60 ; i++){
        f[0][i] = 2;
    }
    for (int i=69; i<= 128 ; i++){
            f[0][i] = 2;
    }
}

void print(Field f){
    for (int i = 0; i< 512; i++){
        for (int j = 0; j< 128; j++){
            printf("%d", f[i][j]);
        }
        printf("\n");
    }
}

int main(){
    init();
    print(f);
    return 0;
}