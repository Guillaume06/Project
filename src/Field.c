#include <stdio.h>

/*
** @author: Guillaume FILIOL DE RAIMOND-MICHEL
*/

// Typedefs
typedef int Field[512][128];
typedef struct Entity{
    int x;
    int y;
}Entity;

// Global definitions
Field f;

// Movement functions

// 0 for delete, 1 for add
void EntityMod(Entity e, int mod){
    for (int i = 0; i <= 3; i++){
        f[e.x+i][e.y] = mod;
        f[e.x][e.y+i] = mod;
        f[e.x+i][e.y+i] = mod;
    }
    f[e.x+1][e.y+2] = mod;
    f[e.x+2][e.y+1] = mod;

    f[e.x+3][e.y+1] = mod;
    f[e.x+1][e.y+3] = mod;

    f[e.x+3][e.y+2] = mod;
    f[e.x+2][e.y+3] = mod;
}


void init(){

    // Global init
    for (int i = 0; i<512; i++){
        for (int j = 0; j< 128; j++){
            f[i][j] = 0;
        }
    }

    // Left wall + exit init
    for (int i=0; i <= 59 ; i++){
        f[0][i] = 2;
    }
    for (int i=68; i <= 127 ; i++){
        f[0][i] = 2;
    }
    for (int i=60; i <=  67 ; i++){
        f[0][i] = 3;
    }

    // Right wall init
    for (int i=0; i< 56 ; i++){
        f[127][i] = 2;
    }
    for (int i=72; i< 128 ; i++){
        f[127][i] = 2;
    }
}

void print(Field f){
    for (int j = 127; j >= 0; j--){
        for (int i = 0; i< 512; i++){
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