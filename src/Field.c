#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/resource.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <semaphore.h> 

/*
 * @author: Guillaume FILIOL DE RAIMOND-MICHEL, Thibaut GONNIN
 */

/*********************************************
 *                                           *
 *        Global Definitions                 *
 *                                           *
 *********************************************/
typedef int Field       [512][128];
typedef sem_t Semaphore   [512][128];
typedef struct Entity{
    int x;
    int y;
}Entity;

Field f;
Semaphore s;
int people, thread, t = 0, azimuthY = 63, initList = 0, bool_time = 0, etape=1;
Entity list[512];
double timeTmp;

/*********************************************
 *                                           *
 *            Semaphore Functions            *
 *                                           *
 *********************************************/

/*
 * Will initialize the semaphore
 */
 void initSemaphore(){
    for (int i = 0; i <512; i++){
         for (int j = 0; j < 128; j++){
             sem_init(&s[i][j],0,0);
         }
    }
 }

/*
 * Will lock the target semaphore
 */
 void up(int x, int y){
    sem_post(&s[x][y]);
 }



/*
 * Will unlock the target semaphore
 */
 void down(int x, int y){
    sem_wait(&s[x][y]);
 }


 /*
  * Will lock the lower part of the entity
  */
 void upMovementDown(int x, int y){
    up(x    , y - 1);
    up(x + 1, y - 1);
    up(x + 2, y - 1);
    up(x + 3, y - 1);
 }

 /*
  * Will lock the upper part of the entity
  */
 void upMovementUp(int x, int y){
    up(x    , y - 4);
    up(x + 1, y - 4);
    up(x + 2, y - 4);
    up(x + 3, y - 4);
 }

 /*
  * Will lock the left part of the entity
  */
 void upMovementLeft(int x, int y){
    up(x - 1, y     );
    up(x - 1, y + 1);
    up(x - 1, y + 2);
    up(x - 1, y + 3);
 }
/*
 * Will lock the place needed for the movement algorithm
 */
 void upMovement(int x, int y){
    up(x, y);
    for (int i = 1; i <= 3; i++){
        up(x + i, y     );
        up(x    , y + i);
        up(x + i, y + i);
    }
    up (x + 1, y + 2);
    up (x + 2, y + 1);
    up (x + 3, y + 1);
    up (x + 1, y + 3);
    up (x + 3, y + 2);
    up (x + 2, y + 3);

    upMovementDown  (x, y);
    upMovementLeft  (x, y);
    upMovementUp    (x, y);
    up(x - 1, y  + 4);
    up(x - 1, y  - 1);
 }

  /*
   * Will unlock the lower part of the entity
   */
  void downMovementDown(int x, int y){
     down(x    , y - 1);
     down(x + 1, y - 1);
     down(x + 2, y - 1);
     down(x + 3, y - 1);
  }

  /*
   * Will unlock the upper part of the entity
   */
  void downMovementUp(int x, int y){
     down(x    , y - 4);
     down(x + 1, y - 4);
     down(x + 2, y - 4);
     down(x + 3, y - 4);
  }

  /*
   * Will unlock the left part of the entity
   */
  void downMovementLeft(int x, int y){
     down(x - 1, y     );
     down(x - 1, y + 1);
     down(x - 1, y + 2);
     down(x - 1, y + 3);
  }

 /*
  * Will unlock the place needed for the movement algorithm
  */
  void downMovement(int x, int y){
     down(x, y);
     for (int i = 1; i <= 3; i++){
         down(x + i , y     );
         down(x     , y + i );
         down(x + i , y + i );
     }
     down(x + 1, y + 2);
     down(x + 2, y + 1);
     down(x + 3, y + 1);
     down(x + 1, y + 3);
     down(x + 3, y + 2);
     down(x + 2, y + 3);

     downMovementDown  (x, y);
     downMovementLeft  (x, y);
     downMovementUp    (x, y);
     down(x - 1, y  + 4);
     down(x - 1, y  - 1);
  }

/*********************************************
 *                                           *
 *        Diverse calculation Functions      *
 *                                           *
 *********************************************/

 /*
  * Will make a ascending sort on the board
  * Source : http://www.commentcamarche.net/forum/affich-1063120-debutant-en-c-trier-tableau
  */
 void triCroissantDouble(double tableau[], long tailleTableau){
     long i,t,k=0;
     for(t = 1; t < tailleTableau; t++){
         for(i = 0; i < tailleTableau - 1; i++){
             if(tableau[i] > tableau[i+1]){
                 k                = tableau[i] - tableau[i+1];
                 tableau[i]      -= k;
                 tableau[i+1]    += k;
             }
         }
     }
 }

 /*
  * Will make a ascending sort on the board
  * Source : http://www.commentcamarche.net/forum/affich-1063120-debutant-en-c-trier-tableau
  */
 void triCroissantInt(long int tableau[], long tailleTableau){
     long i,t,k=0;

     for(t = 1; t < tailleTableau; t++){
         for(i=0; i < tailleTableau - 1; i++){
             if(tableau[i] > tableau[i+1]){
                 k                = tableau[i] - tableau[i+1];
                 tableau[i]      -= k;
                 tableau[i+1]    += k;
             }
         }
     }
 }

 /*

  * Will calculate 2 power number
  * needed so we can know how many entity we needed to know
  */
 int power(int number){
     int result = 1;
     while(number > 0){
         result *= 2;
         number--;
     }
     return result;
 }

/*********************************************
 *                                           *
 *          Movement Functions               *
 *                                           *
 *********************************************/

/*
 * Will modify the value of the field
 * 0 for delete, 1 for add
 */
void entityMod(Entity e, int mod){
    for (int i = 0; i <= 3; i++){
        f[e.x+i][e.y]   = mod;
        f[e.x]  [e.y+i] = mod;
        f[e.x+i][e.y+i] = mod;
    }
    f[e.x+1][e.y+2]     = mod;
    f[e.x+2][e.y+1]     = mod;

    f[e.x+3][e.y+1]     = mod;
    f[e.x+1][e.y+3]     = mod;

    f[e.x+3][e.y+2]     = mod;
    f[e.x+2][e.y+3]     = mod;
}

/*
 * Will check if the entity can be placed here
 */
int entityCheck(Entity e){
    if (e.x > 511) return 1;
    if (e.y > 127) return 1;
    for (int i = 0; i <= 3; i++){
        if (f[e.x+i][e.y] != 0)     return 1;
        if (f[e.x]  [e.y+i] != 0)   return 1;
        if (f[e.x+i][e.y+i] != 0)   return 1;
    }
    if (f[e.x+1][e.y+2] != 0)       return 1;
    if (f[e.x+2][e.y+1] != 0);

    if (f[e.x+3][e.y+1])            return 1;
    if (f[e.x+1][e.y+3])            return 1;

    if (f[e.x+3][e.y+2])            return 1;
    if (f[e.x+2][e.y+3])            return 1;
    return 0;
}

/*
 * Will check if the entity can go up
 * 0 if yes, 1 otherwise
 */
int checkMoveUp(Entity e){
    if (e.y + 4 > 127) return 1;
    int ret = 0;
    if ((f[e.x]  [e.y+4] != 0)  && (f[e.x]  [e.y+4] != 3))  ret = 1;
    if ((f[e.x+1][e.y+4] != 0)  && (f[e.x+1][e.y+4] != 3))  ret = 1;
    if ((f[e.x+2][e.y+4] != 0)  && (f[e.x+2][e.y+4] != 3))  ret = 1;
    if ((f[e.x+3][e.y+4] != 0)  && (f[e.x+3][e.y+4] != 3))  ret = 1;
    return ret;
}
/*
 * Will check if the entity can go Upper left
 * 0 if yes, 1 otherwise
 */
int checkMoveUpperLeft(Entity e){
    if (e.x - 1 < 0)    return 1;
    if (e.y + 4 > 127)  return 1;
    int ret = 0;
    if ((f[e.x]  [e.y+4] != 0)  && (f[e.x]  [e.y+4] != 3))  ret = 1;
    if ((f[e.x+1][e.y+4] != 0)  && (f[e.x+1][e.y+4] != 3))  ret = 1;
    if ((f[e.x+2][e.y+4] != 0)  && (f[e.x+2][e.y+4] != 3))  ret = 1;

    if ((f[e.x-1][e.y]   != 0)  && (f[e.x-1][e.y]   != 3))  ret = 1;
    if ((f[e.x-1][e.y+1] != 0)  && (f[e.x-1][e.y+1] != 3))  ret = 1;
    if ((f[e.x-1][e.y+2] != 0)  && (f[e.x-1][e.y+2] != 3))  ret = 1;

    if ((f[e.x-1][e.y+4] != 0)  && (f[e.x-1][e.y+4] != 3))  ret = 1;
    return ret;
}

/*
 * Will check if the entity can go left
 * 0 if yes, 1 otherwise
 */
int checkMoveLeft(Entity e){
    if (e.x -1 < 0) return 1;
    int ret = 0;
    if ((f[e.x-1][e.y]   != 0)  && (f[e.x-1][e.y]   != 3))  ret = 1;
    if ((f[e.x-1][e.y+1] != 0)  && (f[e.x-1][e.y+1] != 3))  ret = 1;
    if ((f[e.x-1][e.y+2] != 0)  && (f[e.x-1][e.y+2] != 3))  ret = 1;
    if ((f[e.x-1][e.y+3] != 0)  && (f[e.x-1][e.y+3] != 3))  ret = 1;
    return ret;
}

/*
 * Will check if the entity can go Lower left
 * 0 if yes, 1 otherwise
 */
 int checkMoveLowerLeft(Entity e){
    if (e.x -1 < 0)     return 1;
    if (e.y - 1 < 0)    return 1;
    int ret = 0;
    if ((f[e.x]  [e.y-1] != 0)  && (f[e.x]  [e.y-1] != 3))  ret = 1;
    if ((f[e.x+1][e.y-1] != 0)  && (f[e.x+1][e.y-1] != 3))  ret = 1;
    if ((f[e.x+2][e.y-1] != 0)  && (f[e.x+2][e.y-1] != 3))  ret = 1;

    if ((f[e.x-1][e.y]   != 0)  && (f[e.x-1][e.y]   != 3))  ret = 1;
    if ((f[e.x-1][e.y+1] != 0)  && (f[e.x-1][e.y+1] != 3))  ret = 1;
    if ((f[e.x-1][e.y+2] != 0)  && (f[e.x-1][e.y+2] != 3))  ret = 1;

    if ((f[e.x-1][e.y-1] != 0)  && (f[e.x-1][e.y-1] != 3))  ret = 1;
    return ret;
}

/*
 * Will check if the entity can go Down
 * 0 if yes, 1 otherwise
 */
int checkMoveDown(Entity e){
    if (e.y - 1 < 0) return 1;
    int ret = 0;
    if ((f[e.x]  [e.y-1] != 0)  && (f[e.x]  [e.y-1] != 3))  ret = 1;
    if ((f[e.x+1][e.y-1] != 0)  && (f[e.x+1][e.y-1] != 3))  ret = 1;
    if ((f[e.x+2][e.y-1] != 0)  && (f[e.x+2][e.y-1] != 3))  ret = 1;
    if ((f[e.x+2][e.y-1] != 0)  && (f[e.x+3][e.y-1] != 3))  ret = 1;
    return ret;
}

/*
 * Will move up the entity
 * 0 if she moved, 1 otherwise
 */
 int moveUp(Entity* e){
    if (e->y + 4 > 127) return 1;
    if (checkMoveUp(*e) == 0){
        entityMod(*e, 0);
        e->y += 1;
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

/*
 * Will move upper left the entity
 * 0 if she moved, 1 otherwise
 */int moveUpperLeft(Entity* e){
    if (e->x - 1  < 0)   return 1;
    if (e->y + 4  > 127) return 1;
    if (checkMoveUpperLeft(*e) == 0){
        entityMod(*e, 0);
        e->x -= 1;
        e->y += 1;
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

/*
 * Will move left the entity
 * 0 if she moved, 1 otherwise
 */
 int moveLeft(Entity* e){
    if (checkMoveLeft(*e) == 0){
        entityMod(*e, 0);
        e->x -= 1;
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

/*
 * Will move lower Left the entity
 * 0 if she moved, 1 otherwise
 */
 int moveLowerLeft(Entity* e){
    if (checkMoveLowerLeft(*e) == 0){
        entityMod(*e, 0);
        e->x -= 1;
        e->y -= 1;
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

/*
 * Will move down the entity
 * 0 if she moved, 1 otherwise
 */
 int moveDown(Entity* e){
    if (checkMoveDown(*e) == 0){
        entityMod(*e, 0);
        e->y -= 1;
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

/*
 * Global movement function for the entities
 * Calculate the best option and move the entity
 */
void entityMovement(Entity* e){
    if (e->y == 0 && e->x == 0) return;
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

/*********************************************
 *                                           *
 *               Field Functions             *
 *                                           *
 *********************************************/

/*
 * Initialise the whole field (only init values and walls)
 * 0 if she moved, 1 otherwise
 */
void init(){
    // Global init
    for (int i = 0; i <512; i++){
        for (int j = 0; j < 128; j++){
            f[i][j] = 0;
        }
    }

    // Left wall + exit init
    for (int j = 0; j < 16; j++){
        for (int i=0; i <= 59 ; i++){
            f[j][i] = 2;
        }
        for (int i=68; i <= 127 ; i++){
            f[j][i] = 2;
        }
    }
    for (int i = 60; i <= 67; i++){
        f[0][i] = 3;
    }

    // Right wall init
    for (int j = 0; j < 16; j++){
        for (int i = 0; i < 56; i++){
        f[128 + j + 16][i] = 2;
        }
        for (int i = 72; i < 128; i++){
            f[128 + j + 16][i] = 2;
        }
    }
}

/*
 * Will print the field
 * 0 if she moved, 1 otherwise
 */
void print(Field f){
    for (int j = 127; j >= 0; j--){
        for (int i = 0; i < 512; i++){
            printf("%d", f[i][j]);
        }
        printf("\n");
    }
}

/*
 * Will generate an entity, optimized so we can place the new entity quickly
 */
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
        ret.x   = randomX;
        ret.y   = randomY;
    }
    entityMod(ret, 1);
    list[initList++] = ret;
}

/*
 * Will generate nb entities randomly placed on the field
 * 0 if she moved, 1 otherwise
 */
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

void parser(int argc, char const *argv[]){
    int number;
    for (int i = 1; i < argc; ++i){
        if (*argv[i]++ == '-'){

            if(*argv[i] == 'p'){
                number = argv[i][1]-'0';
                if(0 <= number && number < 10 && strlen(argv[i]) == 2){
                    people = power(number);
                    //printf("people = %d\n",people );
                  
                }
                else
                    printf("%s\n","Issue with the number given with the parameters -p. This number should be between 0 and 9" );
            }
            else if(*argv[i]=='t'){
                number = argv[i][1]-'0';
                if(0 <= number && number < 3 && strlen(argv[i]) == 2){
                    thread = number;
                    //printf("thread = %d\n",thread );
                
                }

                else
                    printf("%s\n","Issue with the number given with the parameters -t. This number should be 0,1 or 2" );
            }
            else if(*argv[i]=='m'){
                bool_time = 1;
            }
            else if (*argv[i]=='e'){
                number = argv[i][1]-'0';
                if(1 <= number && number < 4 && strlen(argv[i]) == 2){
                    etape = number;
                    //printf("etape = %d\n",etape );
                }

                else
                    printf("%s\n","Issue with the number given with the parameters -e. This number should be 1,2 or 3" );                

            }
            else{
                printf("%s\n","you didn't give a good parameters(-t, -e, -m or -p)");
            }
        }
        else{
            printf("%s\n","you didn't give a good parameters(-t, -e, -m or -p)");

        }
    }
}


// Main method for the field
void startField(int entityNumber, int printed){
    init();
    generateXRandomEntities(entityNumber);
    if (printed == 1)print(f);
}


void run_global(int printed,void (*function)(void)){
    struct tm* tm_info1, *tm_info2, *tm_info3;
    if (bool_time == 1){
        double      timeInit, timeInitUser;
        double      timeT[5];
        double      size[5];
        time_t      timeUser[5];
        struct      rusage rUsage;
        for(int i = 0; i< 5; i++){
            startField(people, 0);

            timeInit                = ((double)clock())/CLOCKS_PER_SEC;
            timeInitUser            = time(NULL);

            (*function)();

            double time_in_seconds  =((double)clock())/CLOCKS_PER_SEC;
            timeT[i]                = time_in_seconds - timeInit;
            getrusage(1, &rUsage);
            size[i]                 = rUsage.ru_maxrss;
            timeUser[i]             = time(NULL) - timeInitUser;
        }
        tm_info1                    = localtime(&timeUser[1]);
        tm_info2                    = localtime(&timeUser[2]);
        tm_info3                    = localtime(&timeUser[3]);
        tm_info1->tm_sec           += tm_info2->tm_sec;
        tm_info1->tm_sec           += tm_info3->tm_sec;
        tm_info1->tm_sec           /= 3;


        triCroissantDouble(timeT, 5);
        triCroissantDouble(size, 5);
        triCroissantInt(timeUser, 5);
        printf("Average CPU time used           : %f secondes\n",                  (timeT[1]+timeT[2]+timeT[3])/3);
        printf("Average user time used          : %d secondes\n",                  tm_info1-> tm_sec);
        printf("Average max resident set size   : %f kb\n",                        (size[1]+size[2]+size[3])/3);

    }
    else{
        (*function)();
    }
}

/*
 *function used for a thread in the t1 case
 */
void *t1_method(int area){

    while (end() == 0){
        for (int j = 0; j< people; j++){
            if ((list[j].x != 0) && (list[j].y != 0)){
                if ((list[j].x <= area*128) && (list[j].x <= area*128 + 127)){
                    entityMovement(&list[j]);
                }
            }
        }

    }
    pthread_exit(NULL);
}

/*
 *function used for a thread in the t2 case
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
        for (int j = 0; j < people; j++){
            if (list[j].x != 0 && list[j].y != 0){
                entityMovement(&list[j]);
            }
        }
        if (printed == 1) printf("\n");
        if (printed == 1) print(f);
    }
}

/*
 *general method for t1
 */
void run_t1(){
    pthread_t t1[4];

    for (int i = 0; i < 4; i++){
        if (pthread_create(&(t1[i]), NULL, (void*)t1_method, 4-i)) {
            perror("pthread_create");
        }
        if (pthread_join(t1[i], NULL)) {
            perror("pthread_join");
        }                                            
    }
}

/*
**general method for t2
*/
void run_t2(){
    pthread_t t2[people];

    for (int i = 0; i < people; i++){
        if (pthread_create(&(t2[i]), NULL, (void*)t2_method, i)) {
            perror("pthread_create");
        }
    }

    for (int i = 0; i < people; i++){
        if (pthread_join(t2[i], NULL)) {
            perror("pthread_join");
        }
    } 
}

/*
 *function used for a thread in the t1 case
 */
void *t1_method_semaphore(int area){

    while (end() == 0){
        for (int j = 0; j< people; j++){
            int xi=list[j].x;
            int yi=list[j].y;
            if ((xi != 0) && (yi != 0)){
                if ((xi <= area*128) && (xi <= area*128 + 127)){
                  upMovement(xi, yi);
                  entityMovement(&list[j]);
                  downMovement(xi, yi);
                }
            }
        }

    }
    pthread_exit(NULL);
}

/*
**general method for t1 with semaphores
*/
void run_t1_semaphore(){
    pthread_t t1[4];

    for (int i = 0; i < 4; i++){
        if (pthread_create(&(t1[i]), NULL, (void*)t1_method_semaphore, 4-i)) {
            perror("pthread_create");
        }
        if (pthread_join(t1[i], NULL)) {
            perror("pthread_join");
        }                                            
    }
}

void *t2_method_semaphore(int rank){
    
    while (list[rank].x != 0){
        int xi=list[rank].x;
        int yi=list[rank].y;
        upMovement(xi, yi);
        entityMovement(&list[rank]);
        downMovement(xi, yi);
    }

    pthread_exit(NULL);
}

/*
**general method for  with semaphores
*/
void run_t2_semaphore(){
    pthread_t t2[people];

    for (int i = 0; i < people; i++){
      if (pthread_create(&(t2[i]), NULL, (void*)t2_method_semaphore, i)) {
            perror("pthread_create");
      }
    }

    for (int i = 0; i < people; i++){
      if (pthread_join(t2[i], NULL)) {
        perror("pthread_join");
      }        
    }
}

int main(int argc, char const *argv[]){

    srand (time(NULL));

    parser(argc,argv);
    
    startField(people,0);
    switch(etape){
      case 1:
          switch(thread){
              case 0:
                  run_global(0, (void*)run_t0);
                  break;
              case 1:
                  run_global(0, run_t1);
                  break;
              case 2:
                  run_global(0, run_t2);
                  break;
          }
      break;

      case 2:
          switch(thread){
              case 0:
                  run_global(0, (void*)run_t0);
                  break;
              case 1:
                  run_global(0, run_t1_semaphore);
                  break;
              case 2:
                  run_global(0, run_t2_semaphore);
                  break;
          }        
      break;

      case 3:
        printf("not implemented yet\n" );
      break;        
    }

    return 0;
}