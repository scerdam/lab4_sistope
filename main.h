#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


typedef struct threads{

    int row_start, row_end;
    //pthread_mutex_t * mutexs;

}ParamsThread;

//void writeGrid(int, char *);
//int getSchroedingerValue(int ,int ,int ,int);

void writeGrid(int N, char *file_name_out);
void createGrid(int N,int T);

void printGrid();
