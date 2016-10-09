#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/resource.h>
#include <pthread.h>
#include <math.h>

/*
** @author: Guillaume FILIOL DE RAIMOND-MICHEL, Thibaut GONNIN
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
int azimuthY = 63;
Entity list[512];
int initList = 0;
int bool_time = 0;
double timeTmp;

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
    if (e.x > 511) return 1;
    if (e.y > 127) return 1;
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

// http://www.commentcamarche.net/forum/affich-1063120-debutant-en-c-trier-tableau
void triCroissant(double tableau[], long tailleTableau){
    long i,t,k=0;

    for(t = 1; t < tailleTableau; t++){
        for(i=0; i < tailleTableau - 1; i++){
            if(tableau[i] > tableau[i+1]){
                k= tableau[i] - tableau[i+1];
                tableau[i] -= k;
                tableau[i+1] += k;
            }
        }
    }
}

// 0 if ok, 1 if not
int checkMoveUp(Entity e){
    if (e.y + 4 > 127) return 1;
    int ret = 0;
    if ((f[e.x][e.y+4] != 0) && (f[e.x][e.y+4] != 3)) ret = 1;
    if ((f[e.x+1][e.y+4] != 0) && (f[e.x+1][e.y+4] != 3)) ret = 1;
    if ((f[e.x+2][e.y+4] != 0) && (f[e.x+2][e.y+4] != 3)) ret = 1;
    if ((f[e.x+3][e.y+4] != 0) && (f[e.x+3][e.y+4] != 3)) ret = 1;
    return ret;
}

// 0 if ok, 1 if not

int checkMoveUpperLeft(Entity e){
    if (e.x - 1 < 0) return 1;
    if (e.y + 4 > 127) return 1;
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
    if (e.x -1 < 0) return 1;
    int ret = 0;
    if ((f[e.x-1][e.y] != 0) && (f[e.x-1][e.y] != 3)) ret = 1;
    if ((f[e.x-1][e.y+1] != 0) && (f[e.x-1][e.y+1] != 3)) ret = 1;
    if ((f[e.x-1][e.y+2] != 0) && (f[e.x-1][e.y+2] != 3)) ret = 1;
    if ((f[e.x-1][e.y+3] != 0) && (f[e.x-1][e.y+3] != 3)) ret = 1;
    return ret;
}

// 0 if ok, 1 if not

int checkMoveLowerLeft(Entity e){
    if (e.x -1 < 0) return 1;
    if (e.y - 1 < 0) return 1;
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
    if (e.y - 1 < 0) return 1;
    int ret = 0;
    if ((f[e.x][e.y-1] != 0) && (f[e.x][e.y-1] != 3)) ret = 1;
    if ((f[e.x+1][e.y-1] != 0) && (f[e.x+1][e.y-1] != 3)) ret = 1;
    if ((f[e.x+2][e.y-1] != 0) && (f[e.x+2][e.y-1] != 3)) ret = 1;
    if ((f[e.x+2][e.y-1] != 0) && (f[e.x+3][e.y-1] != 3)) ret = 1;
    return ret;
}

// 0 if she has moved, 1 otherwise
int moveUp(Entity* e){
    if (e->y + 4 > 127) return 1;
    if (checkMoveUp(*e) == 0){
        entityMod(*e, 0);
        e->y+=1;
        if (f[e->x][e->y] != 3){
            entityMod(*e, 1);
        }else{
                     e->y = 0;
                     e->x = 0;
                 }
        return 0;
    }else{
        return 1;
    }
}

// 0 if she has moved, 1 otherwise
int moveUpperLeft(Entity* e){
    if (e->x- 1 < 0) return 1;
    if (e->y + 4 > 127) return 1;
    if (checkMoveUpperLeft(*e) == 0){
        entityMod(*e, 0);
        e->x-=1;
        e->y+=1;
        if (f[e->x][e->y] != 3){
            entityMod(*e, 1);
        }else{
                     e->y = 0;
                     e->x = 0;
                 }
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
        if (f[e->x][e->y] != 3){
            entityMod(*e, 1);
        }else{
                     e->y = 0;
                     e->x = 0;
                 }
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
        if (f[e->x][e->y] != 3){
            entityMod(*e, 1);
        }else{
                     e->y = 0;
                     e->x = 0;
                 }
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
        if (f[e->x][e->y] != 3){
            entityMod(*e, 1);
        }else{
            e->y = 0;
            e->x = 0;
        }
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
    for (int j = 0; j <16; j++){
        for (int i=0; i <= 59 ; i++){
            f[j][i] = 2;
        }
        for (int i=68; i <= 127 ; i++){
            f[j][i] = 2;
        }
    }
    for (int i=60; i <=  67 ; i++){
        f[0][i] = 3;
    }

    // Right wall init
        for (int j = 0; j< 16; j++){
        for (int i=0; i< 56 ; i++){
        f[128+j+16][i] = 2;
        }
        for (int i=72; i< 128 ; i++){
            f[128+j+16][i] = 2;
        }
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

void entityMovement(Entity* e){
    if (e->y == 0 && e->x == 0)return;
    if (e->y > azimuthY){
        if (moveLowerLeft(e) != 0) {
            if (moveDown(e) != 0) moveLeft(e);
        }
    }else if (e->y < azimuthY){
        if (moveUpperLeft(e) != 0){
            if (moveUp(e) != 0) moveLeft(e);
        }
    }else{
        moveLeft(e);
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
    list[initList++] = ret;
}

// Will generate randomly nb entities
void generateXRandomEntities(int nb){

    for (int i = 1; i <= nb; i++){
        generateRandomEntity(nb, i);
    }
    initList = 0;

}

int end(){
    for (int i = 0; i < people; i++){
        if ( (list[i].x != 0) || (list[i].y != 0) ) return 0;
    }
    return 1;
}

//just use to know how many entities we have to create
int power(int number){
    int result = 1;
    while(number>0){
        result *= 2;
        number--;
    }
    return result;
}

void parser(int argc, char const *argv[]){
    int number;
    for (int i = 1; i < argc; ++i){
        if (*argv[i]++ == '-'){

            if(*argv[i]=='p'){
                number = argv[i][1]-'0';
                if(0 <= number && number < 10 && strlen(argv[i]) == 2)
                    people = power(number);
                else
                    printf("%s\n","Issue with the number given with the parameters -p. This number should be between 0 and 9" );
            }
            else if(*argv[i]=='t'){
                number = argv[i][1]-'0';
                if(0 <= number && number < 3 && strlen(argv[i]) == 2)
                    thread = number;
                else
                    printf("%s\n","Issue with the number given with the parameters -t. This number should be 0,1 or 2" );
            }
            else if(*argv[i]=='m'){
                bool_time = 1;
            }
            else{
                printf("%s\n","you didn't give a good parameters(-t -m or -p)");
            }
        }
        else{
            printf("%s\n","you didn't give a good parameters(-t -m or -p)");

        }
    }
}


// Main method for the field
void startField(int entityNumber, int printed){
    init();
    generateXRandomEntities(entityNumber);
    if (printed == 1)print(f);
}


void run_global(int printed,void (*function)(int),int arg){
    if (bool_time == 1){
        int timeInit;
        int timeInitUser;
        double timeT[5];
        long int timeUser[5];
        int size[5];
        double response[5];
        struct rusage rUsage;
        for(int i = 0; i< 5; i++){
            getrusage(1, &rUsage);
            timeInitUser = rUsage.ru_utime.tv_usec;
            timeInit = (double)clock()/(double)CLOCKS_PER_SEC;
            startField(people, 0);
            timeInit = (double)clock()/(double)CLOCKS_PER_SEC - timeInit;
            timeInitUser = rUsage.ru_utime.tv_usec - timeInitUser;
            getrusage(1, &rUsage);
            
            (*function)(arg);
            
            double time_in_seconds=(double)clock()/(double)CLOCKS_PER_SEC;
            timeT[i] = time_in_seconds - timeTmp - timeInit;
            timeTmp = time_in_seconds;
            getrusage(1, &rUsage);
            size[i]=rUsage.ru_maxrss;
            timeUser[i]=rUsage.ru_utime.tv_usec - timeInitUser;
        }


        triCroissant(&timeT, 5);
        triCroissant(&size, 5);
        triCroissant(&timeUser, 5);
        printf ("Average CPU time used : %lf\n", (timeT[1]+timeT[2]+timeT[3])/3);
        printf ("Average user time used : %lf\n", (timeUser[1]+timeUser[2]+timeUser[3])/3);
        printf("Average max resident set size : %d\n", (size[1]+size[2]+size[3])/3);

    }
    else{
            (*function)(arg);
    }
        
}

/*
**function used for a thread in the t1 case
*/
void *t1_method(int area){

    while (end() == 0){
        for (int j = 0; j< people; j++){
            if ((list[j].x != 0) && (list[j].y != 0)){
                if ((list[j].x <= area*128) && (list[j].x <= area*128+127)){
                    entityMovement(&list[j]);

                }
            }
        }

    }

    pthread_exit(NULL);
}

/*
**function used for a thread in the t2 case
*/
void *t2_method(int rank){
    
    while (list[rank].x != 0){
        entityMovement(&list[rank]);
    }

    pthread_exit(NULL);
}

/*
**general method for t0
*/
void run_t0(int printed){
    while (end() == 0){
        for (int j = 0; j< people; j++){
            if (list[j].x != 0 && list[j].y != 0){
                entityMovement(&list[j]);
            }
        }
        if (printed == 1)printf("\n");
        if (printed == 1)print(f);
    }
}

/*
**general method for t1
*/
void run_t1(){
    pthread_t t1[4];

    for (int i = 0; i < 4; i++){
        if (pthread_create(&(t1[i]), NULL, t1_method, 4-i)) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
        
        if (pthread_join(t1[i], NULL)) {
            perror("pthread_join");
            return EXIT_FAILURE;
        }                                            
    }
}

/*
**general method for t2
*/
void run_t2(){
    pthread_t t2[people];
                  

    for (int i = 0; i < people; i++){
        if (pthread_create(&(t2[i]), NULL, t2_method, i)) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }



    for (int i = 0; i < people; i++){
        if (pthread_join(t2[i], NULL)) {
            perror("pthread_join");
            return EXIT_FAILURE;
        }
    }
    

    
}



int main(int argc, char const *argv[]){

    srand (time(NULL));

    parser(argc,argv);
    
    startField(people,0);

    switch(thread){
        case 0:
            run_global(0,run_t0,NULL);    
            break;

        case 1:
            run_global(0,run_t1,NULL);
            break;
        case 2:
            run_global(0,run_t2,NULL);
            break;
    }

    return 0;
}