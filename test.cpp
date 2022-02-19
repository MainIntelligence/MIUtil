#include "Src/Vector.h"
#include <cstdio>

int main(int argc, char * argv[]) {
   Vector<int> vec;
   vec.AddEnd(0);
   vec.AddEnd(1);
   vec.AddEnd(2);
   for (int i = 0; i < 3; i++) {
      if (vec[i] != i) {
        printf("ERROR: vec[%d] != %d\n", i, i);
        return 1;
      }
   }
   
   
   return 0;
}
