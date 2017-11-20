#include "main.h"


float **grid; // float **H;
int N,T;


int main(int argc, char **argv) {

      int index;
      int c;

      char *file_name_in = NULL, *file_name_out_1 = NULL, *file_name_out_2 = NULL;
      int n, m, o, flag_show_time_execution = 0;

      opterr = 0;

      while ((c = getopt (argc, argv, "i:s:g:n:m:o:")) != -1)
        switch (c)
          {
          case 'i':
            file_name_in =  optarg;
            break;
          case 's':
            file_name_out_1 =  optarg;
            break;
          case 'g':
            nfile_name_out_2 =  optarg;
            break;
          case 'n':
            n  = atoi(optarg);
            break;
          case 'm':
            m  = atoi(optarg);
            break;
          case 'o':
            o  = atoi(optarg);
            break;
          case 'd':
            flag_show_time_execution = 1;
            break;
          case '?':
            if (optopt == 'c')
              fprintf (stderr, "Opcion -%c requie re un argumento.\n", optopt);
            else if (isprint (optopt))
              fprintf (stderr, "Opcion desconocida `-%c'.\n", optopt);
            else
              fprintf (stderr,
                       "Opcion con caracter desconocido `\\x%x'.\n",optopt);
            return 1;
          default:
            abort ();
          }


      for (index = optind; index < argc; index++) printf ("No existe opcion para agumento: %s\n", argv[index]);



      return 0;
}



// escribe la matriz(grid) en el archivo de salida
void writeGrid(int N, char *file_name_out){
    FILE *f = fopen(file_name_out , "wb");
    int i;
    for(i=0;i<N;i++){

        fwrite(grid[i],N,sizeof(float),f);

    }
    fclose(f);
}

void createGrid(int N,int T){

    int i,j,k;
    grid = (float***)malloc(sizeof(float**)*N);
    for(i=0;i<N;i++){
        grid[i] = (float**)malloc(sizeof(float*)*N);
        for(j=0;j<N;j++){
            grid[i][j] = (float*)malloc(sizeof(float)*T);
            for(k=0;k<=T;k++){  /// Hay que llegar hasta T  (k<=T)
                grid[i][j][k] = 12345.12345;
            }
        }
    }
}

void printGrid(int T){
    int i,j;
    printf("\n");
    for(i=0;i<N;i++){

        printf(" ");
        for(j=0;j<N;j++){
            printf("%10f " ,grid[i][j][T]);
        }
        printf("\n");
    }
    printf("\n");
}
