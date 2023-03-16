CC=gcc
objects = main.o areaOfRectangle.o areaOfSquare.o areaOfCircle.o area_of_triangle.o 
headers = areaOfRectangle.h areaOfSquare.h areaOfCircle.h area_of_triangle.h 

build: createReport.c
	gcc -o createReport createReport.c

run: createReport.o
	./createReport


build-b: createReport.c
	gcc -o createReport createReport.c

run-b: createReport.o
	./createReport

clean:
	rm go