#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <stdint.h>

 //
char *file_name_in = NULL, *file_name_out_1 = NULL, *file_name_out_2 = NULL;
int n, m, o, flag_show_time_execution = 0;

typedef struct fileheader{

  uint32_t size_file;        // No se usa
  uint16_t reserved_1;       // No se usa
  uint16_t reserved_2;       // No se usa
  uint32_t offset;      // Offset donde comiezan los datos de la foto

} fileHeader;

typedef struct infoheader{

  uint32_t headersize;
  uint32_t width;
  uint32_t height;
  uint16_t planes;
  uint16_t bpp;
  uint32_t compress;
  uint32_t imgsize;
  uint32_t bpmx;
  uint32_t bpmy;
  uint32_t colors;
  uint32_t imxtcolors;
} infoHeader;

typedef struct pixel
{
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} pixel;

infoHeader imagenInfoHeader;
fileHeader imagenFileHeader;

int padding, pixelesAsignados, widthReducida, heightReducida;

pixel** matrizImagen;
pixel** matrizReducida;

clock_t inicioMetodo1, inicioMetodo2, finMetodo1, finMetodo2;

double tiempoMetodo1, tiempoMetodo2;

void obtenerInfoImagen();
void calcularPadding();
pixel** cargarMatrizImagen();
pixel** crearMatrizImagenReducida();
void promediar(int i, int j, int modo);
void imprimirMatrizImagen();
void imprimirMatrizImagenReducida();
void metodo1();
void metodo2();
int escribirSalida();
void mostrarInfo();
