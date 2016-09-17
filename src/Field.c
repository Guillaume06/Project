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
void entityMod(Entity e, int mod){
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

// 0 if ok, 1 if not
int checkMoveUp(Entity e){
    int ret = 0;
    if ((f[e.x][e.y+4] != 0) && (f[e.x][e.y+4] != 3)) ret = 1;
    if ((f[e.x+1][e.y+4] != 0) && (f[e.x+1][e.y+4] != 3)) ret = 1;
    if ((f[e.x+2][e.y+4] != 0) && (f[e.x+2][e.y+4] != 3)) ret = 1;
    if ((f[e.x+3][e.y+4] != 0) && (f[e.x+3][e.y+4] != 3)) ret = 1;
    return ret;
}

// 0 if ok, 1 if not

int checkMoveUpperLeft(Entity e){
    int ret = 0;
    if ((f[e.x][e.y+4] != 0) && (f[e.x][e.y+4] != 3)) ret = 1;
    if ((f[e.x+1][e.y+4] != 0) && (f[e.x+1][e.y+4] != 3)) ret = 1;
    if ((f[e.x+2][e.y+4] != 0) && (f[e.x+2][e.y+4] != 3)) ret = 1;

    if ((f[e.x-1][e.y] != 0) && (f[e.x-1][e.y] != 3)) ret = 1;
    if ((f[e.x-1][e.y+1] != 0) && (f[e.x-1][e.y+1] != 3)) ret = 1;
    if ((f[e.x-1][e.y+2] != 0) && (f[e.x-1][e.y+2] != 3)) ret = 1;

    if ((f[e.x-1][e.y+4] != 0) && (f[e.x-1][e.y+4] != 3)) ret = 1;
    return ret;
}

// 0 if ok, 1 if not

int checkMoveLeft(Entity e){
    int ret = 0;
    if ((f[e.x-1][e.y] != 0) && (f[e.x-1][e.y] != 3)) ret = 1;
    if ((f[e.x-1][e.y+1] != 0) && (f[e.x-1][e.y+1] != 3)) ret = 1;
    if ((f[e.x-1][e.y+2] != 0) && (f[e.x-1][e.y+2] != 3)) ret = 1;
    if ((f[e.x-1][e.y+3] != 0) && (f[e.x-1][e.y+3] != 3)) ret = 1;
    return ret;
}

// 0 if ok, 1 if not

int checkMoveLowerLeft(Entity e){
    int ret = 0;
    if ((f[e.x][e.y-1] != 0) && (f[e.x][e.y-1] != 3)) ret = 1;
    if ((f[e.x+1][e.y-1] != 0) && (f[e.x+1][e.y-1] != 3)) ret = 1;
    if ((f[e.x+2][e.y-1] != 0) && (f[e.x+2][e.y-1] != 3)) ret = 1;

    if ((f[e.x-1][e.y] != 0) && (f[e.x-1][e.y] != 3)) ret = 1;
    if ((f[e.x-1][e.y+1] != 0) && (f[e.x-1][e.y+1] != 3)) ret = 1;
    if ((f[e.x-1][e.y+2] != 0) && (f[e.x-1][e.y+2] != 3)) ret = 1;

    if ((f[e.x-1][e.y-1] != 0) && (f[e.x-1][e.y-1] != 3)) ret = 1;
    return ret;
}

// 0 if ok, 1 if not

int checkMoveDown(Entity e){
    int ret = 0;
    if ((f[e.x][e.y-1] != 0) && (f[e.x][e.y-1] != 3)) ret = 1;
    if ((f[e.x+1][e.y-1] != 0) && (f[e.x+1][e.y-1] != 3)) ret = 1;
    if ((f[e.x+2][e.y-1] != 0) && (f[e.x+2][e.y-1] != 3)) ret = 1;
    if ((f[e.x+2][e.y-1] != 0) && (f[e.x+3][e.y-1] != 3)) ret = 1;
    return ret;
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

    Entity e1;
    e1.x = 10;
    e1.y = 10;

    Entity e2;
    e2.x = 6;
    e2.y = 6;

    init();

    entityMod(e1, 1);
    entityMod(e2, 1);

    print(f);

    printf("Move Up : %d\n", checkMoveUp(e1));
    printf("Move Upper Left : %d\n", checkMoveUpperLeft(e1));
    printf("Move Left : %d\n", checkMoveLeft(e1));
    printf("Move Lower Left : %d\n", checkMoveLowerLeft(e1));
    printf("Move Down : %d\n", checkMoveDown(e1));
    return 0;
}