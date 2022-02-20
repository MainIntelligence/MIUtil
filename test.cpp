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


//SubProcess(es) Test Utilities
struct SubProcResult {
   char carr [32] {'\0'};
};

template <char c>
void FillArr(SubProcResult& res, std::mutex& mtx) {
   mtx.lock();
   
   for (unsigned int i = 0; i < 31; i++) { res.carr[i] = c; }
   
   mtx.unlock();
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
   SubProcesses<8, SubProcResult> sps; 
   
   sps.Add(FillArr<'A'>);
   sps.Add(FillArr<'B'>);
   sps.Add(FillArr<'C'>);
   sps.Add(FillArr<'D'>);
   sps.Add(FillArr<'E'>);
   sps.Add(FillArr<'F'>);
   sps.Add(FillArr<'G'>);
   sps.Add(FillArr<'H'>);
   
   bool foundcs [8] {false};
   unsigned int found = 0;
   SubProcess<SubProcResult> * psp = nullptr;
   unsigned int foundid = 0;
   while (found < 8) {
     foundid = sps.BeginCheckAnyFrom(psp, 0);
     if (psp == nullptr) { continue; }
     if ( (psp->result.carr[0] - foundid) != 'A' ) { fprintf(stderr, "Unexpected FillArr result\n"); return 1; }
     
     foundcs[foundid] = true;
     found++;
     
     psp->EndCheck();
     sps.Remove(foundid);
   }
   
   for (unsigned int i = 0; i < 8; i++) { 
      if (foundcs[i] != true) { 
   	fprintf(stderr, "Not all subprocesses recorded results!?!\n"); 
   	return 1; 
      } 
   }
   /*****************************************************/
   /*****************************************************/
   /*****************************************************/
   return 0;
}
