#include <math.h>
#include "Src/Math/Complex.h"
#include "Src/Math/Rotator3.h"
#include "Src/Math/NotchWave.h"


#include <cstdio>

double EPSILON = 0.000001;
bool IsClose( const double x, const double y ) {
   return (x >= y - EPSILON) && (x <= y + EPSILON);
}


int main(int argc, char * argv[]) {
   Complex<double> complex0(1,0);
   Complex<double> complex1(0,1);
   Complex<double> complex2 = complex0 + complex1;
   complex2 = complex2.Inverse();
   if (complex2.Re() != 0.5f || complex2.Im() != -0.5f) {
      printf("unexpected result 0\n"); return 1;
   }
   complex2 = -complex2;
   complex2 += -complex2;
   if (complex2.Re() != 0.0f || complex2.Im() != 0.0f) {
      printf("unexpected result 1\n"); return 1;
   }
   complex2 += 2*complex0 + 2*complex1;
   complex2 = -complex2;
   if (complex2.Re() != -2.0f || complex2.Im() != -2.0f) {
      printf("unexpected result 2\n"); return 1;
   }
   
   //NotchWave test
   ComposedNotchWave<double> w (1000); // 1000 ms / notch
   w.AddWave(500, 1); //500Hz = (1/2) cycles / 0.001 sec = (1/2) cycles / 1000ms
   w.Progress();
   if ( w.Magnitude() != 1.0 ) { printf("unexpected result 3 %f\n", w.Magnitude()); return 1; }
   w.Progress();
   if ( w.Magnitude() != -1.0 ) { printf("unexpected result 4 %f\n", w.Magnitude()); return 1; }
   
   //Quaternion rotation test
   Rotator3 rotquat;
   //Rotate (0, 1, 0) 90 deg to the right, while you are currently looking at (1, 0, 0) -> r = (0,-1,0)
   //	exp: p -> (1.0, 0.0, 0.0)
   rotquat.Update(0, -3.141592 / 2.0, 0.0, -1.0);
   double point [3] {0.0, 1.0, 0.0};
   rotquat.Rotate(point);
   if ( !IsClose(point[0], 1.0f) || !IsClose(point[1], 0.0) || !IsClose(point[2], 0.0) ) {
     printf("unexpected result 5 (%f %f %f)\n", point[0], point[1], point[2]); return 1;
   }
   
   return 0;
};

