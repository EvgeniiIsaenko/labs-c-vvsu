#include <stdio.h>
#include <math.h>

#define EPSILON 1e-9

double ch(double x) 
{
//  ch(x)= 1 + x2/2! + x4/4! + x6/6! + x8/8! + ...
  double s; int k;
  s=0; k=0;

  double f = 1.0;

  while (fabs(f) >= EPSILON) {
        s += f;
        k++;
        // рекуррентный переход: term_{k-1} -> term_k
        // term_k = term_{k-1} * x^2 / ((2k-1)*(2k))
        f *= (x * x) / ((2*k - 1) * (2*k));

        if (k > 1000) break;
    }

  return s;
}

double sh(double x) 
{
    double s; 
    int k;
    double term;
    
    s = 0; k = 1;
    term = x;     // первый член: x^1/1! = x
    
    while (fabs(term) >= EPSILON) {
        s += term;
        k++;
        // рекуррентный переход: term_{k-1} -> term_k
        // term_k = term_{k-1} * x^2 / ((2k-2)*(2k-1))
        term *= (x * x) / ((2*k - 2) * (2*k - 1));
        if (k > 1000) break;
    }
    return s;
}

int main()
{
  double x;
   for (x=0; x<10; x=x+0.1)
//    printf("%16.8lf %16.8lf\n",sh(x), ch(x)); 
    printf("%16.8lf %16.8lf %16.8lf %16.8lf\n",sh(x), ch(x), sinh(x), cosh(x)); 


 return 0;
}
