#include <stdio.h>
#include <math.h>
#include "areaOfRectangle.h"
#include "areaOfCircle.h"
#include "areaOfSquare.h"
#include "area_of_triangle.h"


int main( int argc, char *argv[] )  {
   int aor;

   aor = areaOfRectangle(10,80);
   printf("Area of Recctangle: %d", aor);
}