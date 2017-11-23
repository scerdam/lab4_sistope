all: main.c
	gcc -o ejecutable main.c -Wall -I.
	./ejecutable -i cuadro.bmp -s out_metodo1.bmp -g out_metodo2.bmp -n 5 -m 10 -o 2 -d
