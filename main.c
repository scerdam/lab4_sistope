#include "funciones.c"



int main(int argc, char **argv){

	int index;
	int c;


	opterr = 0;

	while ((c = getopt (argc, argv, "i:s:g:n:m:o:d")) != -1)
		switch (c)
			{
			case 'i':
				file_name_in =  optarg;
				break;
			case 's':
				file_name_out_1 =  optarg;
				break;
			case 'g':
				file_name_out_2 =  optarg;
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

	printf("> Inicio del programa\n");

	pixelesAsignados = 0;

  	obtenerInfoImagen();
  	calcularPadding();

    tiempoMetodo1 = 0;
    tiempoMetodo2 = 0;

  	matrizImagen = cargarMatrizImagen();

    if(flag_show_time_execution == 1)
    {
      mostrarInfo();
      printf("Padding imagen original: %d\n\n", padding);
    }

  	if(o == 1)
  	{
      matrizReducida = crearMatrizImagenReducida();
  		metodo1();
  		escribirSalida();

      free(matrizReducida);
      matrizReducida = NULL;

      if(flag_show_time_execution == 1)
      {
        printf("> Tiempo total metodo 1: %.16g milisegundos\n", tiempoMetodo1 * 1000.0);
      }

  	}
  	if(o == 2)
  	{
      matrizReducida = crearMatrizImagenReducida();
  		metodo2();
  		escribirSalida();

      free(matrizReducida);
      matrizReducida = NULL;

      if(flag_show_time_execution == 1)
      {
        printf("> Tiempo total metodo 2: %.16g milisegundos\n", tiempoMetodo2 * 1000.0);
      }

  	}
  	if(o == 3)
  	{
  		o = 1;
  		matrizReducida = crearMatrizImagenReducida();
  		metodo1();
  		escribirSalida();

      if(flag_show_time_execution == 1)
      {
        printf("\n> Tiempo total metodo 1: %.16g milisegundos\n", tiempoMetodo1 * 1000.0);
      }

  		free(matrizReducida);
  		matrizReducida = NULL;

  		printf("\n");

  		o = 2;
  		matrizReducida = crearMatrizImagenReducida();
  		metodo2();
  		escribirSalida();

      if(flag_show_time_execution == 1)
      {
        printf("\n> Tiempo total metodo 2: %.16g milisegundos\n" , tiempoMetodo2 * 1000.0);
      }

  		free(matrizReducida);
  		matrizReducida = NULL;

      if(flag_show_time_execution == 1)
      {
        printf("\n> Tiempo total metodo 3: %.16g milisegundos\n", (tiempoMetodo1 + tiempoMetodo2) * 1000.0);
      }

  	}

  	free(matrizImagen);
  	matrizImagen = NULL;

  	printf("\n> Fin del programa\n");

  	//imprimirMatrizImagen();
  	//imprimirMatrizImagenReducida();

}
