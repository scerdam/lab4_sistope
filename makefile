all: main.c main.h
	gcc -o ejecutable main.c main.h
	./ejecutable -i cuadro.bmp -s out1.bmp -g out1.bmp -n 5 -m 300 -o 1 -d
