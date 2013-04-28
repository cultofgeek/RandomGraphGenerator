#include <iostream>
#include <cmath>

using namespace std;


int roundme( double x ){
   return  static_cast<int> ( x >= 0 ? x + 0.5 : x - 0.5 );
}

int boundedRand(int lower, int upper) {
   // Generates a random integer in the given interval [lower, upper]. 
   // Avoids using rand()%n, since the low-end bits of the 
   // output of rand() are often not reliably random; Stroustrup p.685.

   return roundme( (double(rand())/double(RAND_MAX))*(upper-lower) + lower);
}