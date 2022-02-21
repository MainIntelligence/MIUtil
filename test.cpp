#include "Src/Vector.h"
#include "Src/FullComparor.h"
#include "Src/SubProcess.h"

#include <cstdio>

struct TestComparor : public FullComparor<TestComparor> {
  int n;
  TestComparor( int n ) : n{n} {}
  inline bool operator==(const int o) const { return n == o; }
  inline bool operator<(const int o) const { return n < o; }
  inline bool operator>(const int o) const { return n > o; }
};



template <char c>
void FillArr(SharedData<char[32]> & sdat) {
   WaitAccessor lock (sdat);
   for (unsigned int i = 0; i < 31; i++) { sdat.data[i] = c; }
};






int main(int argc, char * argv[]) {
   //***************** Vector test *************************
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
   
   //***************** FullComparor test ************************
   TestComparor tc(1);
   if ( tc != 1 || tc <= 0 || tc >= 2 ) { return 1; }
   
   //*********** SubProcess, SubProcesses, and SortedArray tests ********************
   SubProcesses<8> sps;
   SharedData<char[32]> results [8];
   
   sps.Add(FillArr<'A'>, std::ref(results[0]));
   sps.Add(FillArr<'B'>, std::ref(results[1]));
   sps.Add(FillArr<'C'>, std::ref(results[2]));
   sps.Add(FillArr<'D'>, std::ref(results[3]));
   sps.Add(FillArr<'E'>, std::ref(results[4]));
   sps.Add(FillArr<'F'>, std::ref(results[5]));
   sps.Add(FillArr<'G'>, std::ref(results[6]));
   sps.Add(FillArr<'H'>, std::ref(results[7]));
   
   bool foundcs [8] {false};
   unsigned int nfound = 0;
   
   while (nfound < 8) {
	for (unsigned int i = 0; i<8; i++) {
	   if (foundcs[i] == true) { continue; }
	   BusyAccessor lock (results[i]);
	   
	   //Check that the thread has done its work
	   if (results[i].data[0] != 'A' + i) { continue; }
	   foundcs[i] = true;
	   nfound++;
	}
   }
   
   for (unsigned int i = 0; i < 8; i++) { 
      if (foundcs[i] != true) {
   	fprintf(stderr, "Not all subprocesses recorded results!?! %u\n", i); 
   	return 1; 
      }
   }
   /*****************************************************/
   /*****************************************************/
   /*****************************************************/
   return 0;
}
