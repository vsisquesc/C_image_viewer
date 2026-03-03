CC = clang
CFLAGS = -g  -std=c99

main: main.o color.o ppm.o
	$(CC) $(CFLAGS) main.o color.o ppm.o -o ascii_image_viewer.out

main.o: main.c utils.h color.h ppm.h
	$(CC) $(CFLAGS) -c main.c

color.o: color.c color.h
	$(CC) $(CFLAGS) -c color.c


ppm.o: ppm.c ppm.h
	$(CC) $(CFLAGS) -c ppm.c

clean:
	rm -f *.o main
