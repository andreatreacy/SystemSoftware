# save a list of the objects and headers
objects = main.o areaOfRectangle.o areaOfSquare.o areaOfCircle.o area_of_triangle.o
headers = areaOfSquare.h areaOfRectangle.h area_of_triangle.h areaOfCircle.h

# build the main using the objects of the other programs as they are included in the main program
# -lm imports the math library
build : $(objects)
	gcc -o main $(objects) -lm

# -c means compile
# don't specify output like (gcc -o program program.c) or it will expect each program to have a main method
# the output is not needed, just the objects
main.o : main.c $(headers)
	gcc -c main.c
areaOfRectangle.o : areaOfRectangle.c areaOfRectangle.h
	gcc -c areaOfRectangle.c
areaOfSquare.o : areaOfSquare.c areaOfSquare.h areaOfRectangle.o
	gcc -c areaOfSquare.c
areaOfCircle.o : areaOfCircle.c areaOfCircle.h
	gcc -c areaOfCircle.c
area_of_triangle.o : area_of_triangle.c area_of_triangle.h
	gcc -c area_of_triangle.c -lm