CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g

project: project.o imageManip.o ppm_io.o
	$(CC) project.o imageManip.o ppm_io.o -o project -lm

project.o: project.c ppm_io.h imageManip.h
	$(CC) $(CFLAGS) -c project.c

imageManip.o: imageManip.c imageManip.h
	$(CC) $(CFLAGS) -c imageManip.c

ppm: ppm.o ppm_io.o
	$(CC) ppm.o ppm_io.o -o ppm

ppm.o: ppm.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm.c

ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c

clean:
	rm -f *.o ppm project
