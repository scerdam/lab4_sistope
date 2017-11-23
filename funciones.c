#include "funciones.h"

void obtenerInfoImagen()
{
	FILE *archivoImagen;
	uint16_t tipoFichero;

	archivoImagen = fopen (file_name_in, "r");

  	if (!archivoImagen)
  	{
  		printf("No existe el archivo de entrada.\n");
  		return;
  	}

  	fread(&tipoFichero, sizeof(uint16_t), 1, archivoImagen);

  	if (tipoFichero !=0x4D42)
   	{
   		printf("El archivo especificado no es .bmp\n");
    	fclose(archivoImagen);
    	return;
  	}

  	fread(&imagenFileHeader, sizeof(fileHeader), 1, archivoImagen);
  	fread(&imagenInfoHeader, sizeof(infoHeader), 1, archivoImagen);

  	fclose(archivoImagen);
}

void calcularPadding()
{
	padding = 0;
	int anchoTemp = imagenInfoHeader.width*3;
	while( ( anchoTemp % 4 ) != 0 )
	{
  		anchoTemp++;
  		padding++;
	}
}

pixel** cargarMatrizImagen()
{
  /*
      Los datos se guardan desde la esquina inferior izquierda hacia la derecha,
      y luego de abajo hacia arriba.
      Los bytes en el bmp se guardan así, y además en orden BGR y no RGB.
  */
  unsigned char* imagen;
  FILE *archivoImagen;

  archivoImagen = fopen (file_name_in, "r");

  if (!archivoImagen)
  {
    printf("No existe el archivo de entrada.\n");
    return NULL;
  }

  imagen = (unsigned char*)malloc(imagenInfoHeader.imgsize);
  fseek(archivoImagen, imagenFileHeader.offset, SEEK_SET);
  fread(imagen, imagenInfoHeader.imgsize,1, archivoImagen);

  fclose(archivoImagen);

  pixel** matriz;
  int i, j, k;
  matriz = (pixel**) calloc(sizeof(pixel*), imagenInfoHeader.height);

  for(i=0; i<imagenInfoHeader.height; i++)
  {
    matriz[i] = (pixel*) calloc(sizeof(pixel), imagenInfoHeader.width);
  }

  k = 0;

  /*
  for(i=0; i< imagenInfoHeader.imgsize; i++)
  {
    printf("#%d: %d\n", i, imagen[i]);
  }
  */

  for(i=imagenInfoHeader.height-1; i>=0 ; i--)
  {
    for(j=0; j<imagenInfoHeader.width; j++)
    {
      matriz[i][j].blue = imagen[k];
      matriz[i][j].green = imagen[k+1];
      matriz[i][j].red = imagen[k+2];
      k = k+3;
    }
    //Se saltan los bytes de padding en cada fila
    k = k + padding;
  }
  return matriz;
}

pixel** crearMatrizImagenReducida()
{
  pixel** matriz;
  int i;

  if(o == 1)
  {

    widthReducida = imagenInfoHeader.width/m + imagenInfoHeader.width%m;
    heightReducida = imagenInfoHeader.height;

    matriz = (pixel**) calloc(sizeof(pixel*), imagenInfoHeader.height);

    for(i=0; i<imagenInfoHeader.height; i++)
    {
      matriz[i] = (pixel*) calloc(sizeof(pixel), imagenInfoHeader.width/m + imagenInfoHeader.width%m);
    }
    return matriz;
  }
  else
  {
    widthReducida = imagenInfoHeader.width;
    heightReducida = imagenInfoHeader.height/m + imagenInfoHeader.height%m;

    matriz = (pixel**) calloc(sizeof(pixel*), imagenInfoHeader.height/m + imagenInfoHeader.height%m);

    for(i=0; i<imagenInfoHeader.height/m + imagenInfoHeader.height%m; i++)
    {
      matriz[i] = (pixel*) calloc(sizeof(pixel), imagenInfoHeader.width);
    }
    return matriz;
  }
}

void promediar(int i, int j, int modo)
{
  /*
    Modo = 0 -> Se recorre normalmente.
    Modo = 1 -> Se recorre hacia atrás.
  */
  int promedioR;
  int promedioG;
  int promedioB;

  promedioR = 0;
  promedioG = 0;
  promedioB = 0;

  int x;

  if(o == 1)
  {
    for(x=0; x<m; x++)
    {
      promedioR = promedioR + matrizImagen[i][j + x].red;
      promedioG = promedioG + matrizImagen[i][j + x].green;
      promedioB = promedioB + matrizImagen[i][j + x].blue;
    }
    if(modo == 0)
    {
      matrizReducida[i][pixelesAsignados].red = promedioR/m;
      matrizReducida[i][pixelesAsignados].green = promedioG/m;
      matrizReducida[i][pixelesAsignados].blue = promedioB/m;
      //printf("> Ubicar en %d, %d\n", i, pixelesAsignados);
      pixelesAsignados++;
    }
    else
    {
      matrizReducida[i][pixelesAsignados-1].red = promedioR/m;
      matrizReducida[i][pixelesAsignados-1].green = promedioG/m;
      matrizReducida[i][pixelesAsignados-1].blue = promedioB/m;
      //printf("> Ubicar en %d, %d\n", i, pixelesAsignados-1);
      pixelesAsignados--;
    }
  }
  else
  {
    for(x=0; x<m; x++)
    {
      promedioR = promedioR + matrizImagen[i + x][j].red;
      promedioG = promedioG + matrizImagen[i + x][j].green;
      promedioB = promedioB + matrizImagen[i + x][j].blue;
    }
    matrizReducida[pixelesAsignados][j].red = promedioR/m;
    matrizReducida[pixelesAsignados][j].green = promedioG/m;
    matrizReducida[pixelesAsignados][j].blue = promedioB/m;
    pixelesAsignados++;
  }

}

void imprimirMatrizImagen()
{
  /*
    La matriz se maneja como se hace normalmente.
  */
  printf("\n> IMAGEN ORIGINAL:\n");
  int i, j;
  for(i=0; i<imagenInfoHeader.height ; i++)
  {
    for(j=0; j<imagenInfoHeader.width; j++)
    {
      printf("%3d %3d %3d |", matrizImagen[i][j].red, matrizImagen[i][j].green, matrizImagen[i][j].blue);
    }
    printf("\n");
  }
}

void imprimirMatrizImagenReducida()
{
  /*
    La matriz se maneja como se hace normalmente.
  */
  int i, j;
  printf("\n> IMAGEN REDUCIDA:\n");
  if(o == 1)
  {
    for(i=0; i<imagenInfoHeader.height ; i++)
    {
      for(j=0; j<imagenInfoHeader.width/m + imagenInfoHeader.width%m; j++)
      {
        printf("%3d %3d %3d |", matrizReducida[i][j].red, matrizReducida[i][j].green, matrizReducida[i][j].blue);
      }
      printf("\n");
    }
  }
  else
  {
    for(i=0; i<imagenInfoHeader.height/m + imagenInfoHeader.height%m ; i++)
    {
      for(j=0; j<imagenInfoHeader.width; j++)
      {
        printf("%3d %3d %3d |", matrizReducida[i][j].red, matrizReducida[i][j].green, matrizReducida[i][j].blue);
      }
      printf("\n");
    }
  }
}

void metodo1()
{
  int i, j;

  int fin = 0;

  int altura, ancho;

  int vecesReducida = 0;

  int pixelesReducidos;

  int k;

  while(vecesReducida < n)
  {
    inicioMetodo1 = clock();
    //printf("\n> Traza metodo 1 reduccion #%d: \n\n", vecesReducida);
    for(i=0; i<imagenInfoHeader.height; i++)
    {
      pixelesReducidos = 0;
      altura = i;
      for(j=0; j<imagenInfoHeader.width; j++)
      {
        if(fin == 0)
        {
          ancho = j;
          k = ancho - (m - 1);
        }
        if(fin == 1)
        {
          ancho = imagenInfoHeader.width-1 - j;
          k = ancho;
        }

        pixelesReducidos++;

        //printf("%3d %3d %3d\n", matrizImagen[altura][ancho].red, matrizImagen[altura][ancho].green, matrizImagen[altura][ancho].blue);
        if(pixelesReducidos%m == 0)
        {
          //printf("TENGO %d PIXELES\n", m);
          //printf("> Parametros promediar: %d, %d, %d\n", altura, k, fin);
          promediar(altura, k, fin);
        }

        if(j == imagenInfoHeader.width-1)
        {
          switch(fin)
          {
            case 1:
              fin = 0;
              break;
            case 0:
              fin = 1;
              break;
          }
        }

      }
    }
    vecesReducida++;

    finMetodo1 = clock();
    tiempoMetodo1 = tiempoMetodo1 + (double) (finMetodo1 - inicioMetodo1)/CLOCKS_PER_SEC;

    if(flag_show_time_execution == 1)
    {
      printf("Iteracion #%d Metodo 1: %.16g milisegundos\n", vecesReducida, (double) (finMetodo1 - inicioMetodo1)/CLOCKS_PER_SEC * 1000.0);
    }

  }

}

void metodo2()
{
  int i, j;

  int vecesReducida = 0;

  int pixelesReducidos;

  int k;

  while(vecesReducida < n)
  {
    inicioMetodo2 = clock();

    //printf("\n> Traza metodo 2 reduccion #%d: \n\n", vecesReducida);

    for(j=0; j<imagenInfoHeader.width; j++)
    {
      i = 0;
      pixelesReducidos = 0;
      pixelesAsignados = 0;
      while(i < imagenInfoHeader.height)
      {
        pixelesReducidos++;
        //printf("%3d %3d %3d\n", matrizImagen[i][j].red, matrizImagen[i][j].green, matrizImagen[i][j].blue);
        if(pixelesReducidos%m == 0)
        {
          k = i - (m - 1);
          //printf("TENGO %d PIXELES\n", m);
          //printf("> Parametros promediar: %d, %d\n", k, j);
          promediar(k, j, 0);
        }

        i++;
      }
    }
    vecesReducida++;

    finMetodo2 = clock();

    tiempoMetodo2 = tiempoMetodo2 + (double) (finMetodo2 - inicioMetodo2)/CLOCKS_PER_SEC;

    if(flag_show_time_execution == 1)
    {
      printf("Iteracion #%d Metodo 2: %.16g milisegundos\n", vecesReducida, (double) (finMetodo2 - inicioMetodo2)/CLOCKS_PER_SEC * 1000.0);
    }

  }

}

/*
  https://gist.github.com/lbsx/1723715
*/
//supply an array of pixels[height][width] <- notice that height comes first
int escribirSalida()
{
  char* filename;

  if(o == 1)
  {
    filename = file_name_out_1;
  }
  else
  {
    filename = file_name_out_2;
  }
  unsigned int width = widthReducida;
  unsigned int height = heightReducida;
  pixel** pixels = matrizReducida;

  int fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
  static unsigned char header[54] = {66,77,0,0,0,0,0,0,0,0,54,0,0,0,40,0,0,0,0,0,0,0,0,0,0,0,1,0,24}; //rest is zeroes
  unsigned int pixelBytesPerRow = width*sizeof(pixel);
  unsigned int paddingBytesPerRow = (4-(pixelBytesPerRow%4))%4;
  unsigned int* sizeOfFileEntry = (unsigned int*) &header[2];
  *sizeOfFileEntry = 54 + (pixelBytesPerRow+paddingBytesPerRow)*height;
  unsigned int* widthEntry = (unsigned int*) &header[18];
  *widthEntry = width;
  unsigned int* heightEntry = (unsigned int*) &header[22];
  *heightEntry = height;
  write(fd, header, 54);

  static unsigned char zeroes[3] = {0,0,0}; //for padding
  //for (int row = 0; row < height; row++)
  for (int row = height-1; row >= 0; row--)
  {
    write(fd,pixels[row],pixelBytesPerRow);
    write(fd,zeroes,paddingBytesPerRow);
  }
  close(fd);
  return 0;
}

void mostrarInfo()
{
  printf("\n> Datos imagen entrada:\n");
  printf("Inicio datos: %u\n", imagenFileHeader.offset);
  printf("Tamaño de la cabecera: %u\n", imagenInfoHeader.headersize);
  printf("Anchura: %d\n", imagenInfoHeader.width);
  printf("Altura: %d\n", imagenInfoHeader.height);
  printf("Planos (1): %d\n", imagenInfoHeader.planes);
  printf("Bits por pixel: %d\n", imagenInfoHeader.bpp);
  printf("Compresión: %d\n", imagenInfoHeader.compress);
  printf("Tamaño de datos de imagen: %u\n", imagenInfoHeader.imgsize);
  printf("Resolucón horizontal: %u\n", imagenInfoHeader.bpmx);
  printf("Resolucón vertical: %u\n", imagenInfoHeader.bpmy);
  printf("Colores en paleta: %d\n", imagenInfoHeader.colors);
  printf("Colores importantes: %d\n", imagenInfoHeader.imxtcolors);
}
