CC = clang
CFLAGS = -g  -std=c99 
RAYLIBFLAGS =  -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

main: main.o  image.o ppm.o
	$(CC) $(CFLAGS) $(RAYLIBFLAGS) main.o image.o ppm.o -o main

main.o: main.c color.h image.h
	$(CC) $(CFLAGS)  -c main.c

 

image.o: image.c image.h ppm.h
	$(CC) $(CFLAGS) -c image.c

ppm.o: ppm.c ppm.h
	$(CC) $(CFLAGS) -c ppm.c
 
clean:
	rm -f *.o main
