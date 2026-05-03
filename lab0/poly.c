#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct point {
	double x;
	double y;
} POINT;

typedef struct poly {
	int n;
	POINT *p;
} POLY;

double dist (POINT p1, POINT p2)
 {
  register double t;
  return sqrt( ( t=(p2.x-p1.x) )*t + ( t=(p2.y-p1.y) )*t);
 }

double perimeter ( POLY p)
 {
  double s;
  s=0;
    for (int i=0; i < p.n; i++)
	    s+= dist(p.p[(i+1)%p.n],p.p[i]);
   return s;
 }










// lab 0 - find for area and diameter [
double area ( POLY p)
{
    double s = 0;
    for (int i = 0; i < p.n; i++) {
        int j = (i + 1) % p.n;
        s += p.p[i].x * p.p[j].y - p.p[j].x * p.p[i].y;
    }
    return fabs(s) / 2.0;
}

double diameter ( POLY p)
{
    double max_dist = 0;
    for (int i = 0; i < p.n; i++) {
        for (int j = i + 1; j < p.n; j++) {
            double d = dist(p.p[i], p.p[j]);
            if (d > max_dist) {
                max_dist = d;
            }
        }
    }
    return max_dist;
}
//]

int main( int argc, char *argv[]   )
 {
  FILE *F;
  POLY p;

   F=fopen("C:/Users/Evgenii/Desktop/Repos/labs-c-vvsu/lab0/poly_examples/105.txt","r");

 //  Здесь код чтения из файла
 fscanf(F,"%d",&p.n);
 p.p=malloc(p.n*sizeof(POINT));
   for (int i=0; i<p.n; i++)
   fscanf(F,"%lf%lf",&p.p[i].x,&p.p[i].y);
   printf("%lf %lf %lf\n",dist( p.p[0], p.p[1]), dist(p.p[1],p.p[2]),perimeter(p));
    
  printf("Area: %lf\n", area(p));
  printf("Diameter: %lf\n", diameter(p));

  free(p.p);
  fclose(F);

  return 0;
 }
