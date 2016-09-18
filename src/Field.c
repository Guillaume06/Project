#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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
int people;
int thread;
int t = 0;

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

// Will check if the entity can be placed here
int entityCheck(Entity e){
    for (int i = 0; i <= 3; i++){
        if (f[e.x+i][e.y] != 0) return 1;
        if (f[e.x][e.y+i] != 0) return 1;
        if (f[e.x+i][e.y+i] != 0) return 1;
    }
    if (f[e.x+1][e.y+2] != 0) return 1;
    if (f[e.x+2][e.y+1] != 0);

    if (f[e.x+3][e.y+1]) return 1;
    if (f[e.x+1][e.y+3]) return 1;

    if (f[e.x+3][e.y+2]) return 1;
    if (f[e.x+2][e.y+3]) return 1;
    return 0;
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

// 0 if she has moved, 1 otherwise
int moveUp(Entity* e){
    if (checkMoveUp(*e) == 0){
        entityMod(*e, 0);
        e->y+=1;
        entityMod(*e, 1);
        return 0;
    }else{
        return 1;
    }
}

// 0 if she has moved, 1 otherwise
int moveUpperLeft(Entity* e){
    if (checkMoveUpperLeft(*e) == 0){
        entityMod(*e, 0);
        e->x-=1;
        e->y+=1;
        entityMod(*e, 1);
        return 0;
    }else{
        return 1;
    }
}

// 0 if she has moved, 1 otherwise
int moveLeft(Entity* e){
    if (checkMoveLeft(*e) == 0){
        entityMod(*e, 0);
        e->x-=1;
        entityMod(*e, 1);
        return 0;
    }else{
        return 1;
    }
}

// 0 if she has moved, 1 otherwise
int moveLowerLeft(Entity* e){
    if (checkMoveLowerLeft(*e) == 0){
        entityMod(*e, 0);
        e->x-=1;
        e->y-=1;
        entityMod(*e, 1);
        return 0;
    }else{
        return 1;
    }
}

// 0 if she has moved, 1 otherwise
int moveDown(Entity* e){
    if (checkMoveDown(*e) == 0){
        entityMod(*e, 0);
        e->y-=1;
        entityMod(*e, 1);
        return 0;
    }else{
        return 1;
    }
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

// Will generate random entity
void generateRandomEntity(int total, int current){

    int N1 = 0, M1 = 508, N2 = 0, M2 = 124;

    int randomX = M1 + rand() / (RAND_MAX / (N1 - M1 + 1) + 1);
    int randomY = M2 + rand() / (RAND_MAX / (N2 - M2 + 1) + 1);


    Entity ret;
    ret.x = randomX;
    ret.y = randomY;

    while (entityCheck(ret) != 0){

        randomX = M1 + rand() / (RAND_MAX / (N1 - M1 + 1) + 1);
        randomY = M2 + rand() / (RAND_MAX / (N2 - M2 + 1) + 1);
        ret.x = randomX;
        ret.y = randomY;
    }
    entityMod(ret, 1);
}

// Will generate randomly nb entities
void generateXRandomEntities(int nb){
    for (int i = 1; i <= nb; i++){
        generateRandomEntity(nb, i);
    }
}

//fonction de hashage de string pour pouvoir switcher facilement sur une string (inspiré de djb2)
int hash(const char *str) {
    int hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

void parser(int argc, char const *argv[]){
    int number;
    for (int i = 1; i < argc; ++i){
        
        switch(hash(argv[i])){ 
            case 5861503: // -m
                t = 1;
                break;
            case 5861506: // -p 
                //convert a char to the int
                number = *argv[++i]-'0';
                if(0 <= number && number < 10 && strlen(argv[i]) == 1){
                    people = number;
                }
                else{
                    printf("%s\n","Issue with the number given with the parameters -p. This number should be between 0 and 9" );
                }
                break;
            case 5861510: // -t
                number = *argv[++i]-'0';
                if(0 <= number && number < 3 && strlen(argv[i]) == 1){
                    thread = number;
                }
                else{
                    printf("%s\n","Issue with the number given with the parameters -t. This number should be 0,1 or 2" );
                }
                break;
            default:
                printf("%s\n","This parameters is not allowed in this program (only -t -p or -m)");


        }
    }
}


// Main method for the field
void startField(int entityNumber){
    init();
    generateXRandomEntities(entityNumber);
    print(f);
}

int main(int argc, char const *argv[]){

    srand (time(NULL));

    parser(argc,argv);
    startField(256);

    return 0;
}