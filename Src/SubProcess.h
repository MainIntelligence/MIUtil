#pragma once
#include <thread>
#include <mutex>

//Process thread wrapper - lifetime of SubProcess == lifetime of worker thread

template <typename RT>
struct SubProcess {
   
   std::thread thread;
   std::mutex mtx;
   RT result;
   
   //CallT must accept RT& and std::mutex& parameter, makes new thread executing func
   template <typename CallT>
   SubProcess(CallT func) : thread(func, std::ref(result), std::ref(mtx)) {};
   ~SubProcess() { thread.join(); }
   
   //use when you have other stuff to do/don't want to wait for results
   //If return is true, you must call EndCheck / unlock the thread
   bool BeginCheckBusy();
   //use when you can't do anything until you obtain result
   void BeginCheckWait();
   //unlock so that the other thread can continue accessing result
   void EndCheck();
};

template <typename RT>
bool SubProcess<RT>::BeginCheckBusy() {
  return mtx.try_lock();
}

template <typename RT>
void SubProcess<RT>::BeginCheckWait() {
  mtx.lock();
}

template <typename RT>
void SubProcess<RT>::EndCheck() {
  mtx.unlock();
}


#include "SortedArray.h"
//NOTE: NO SAFETY IN MAKING SURE YOU DON'T ADD MORE THAN "CAP" SubProcesses
template <unsigned int CAP, typename RT>
struct SubProcesses {
   //Making this an explicit array of SubProcesses might need automatically constructing them (which is bad)
   alignas(SubProcess<RT>) char spdat [CAP * sizeof(SubProcess<RT>)];
   unsigned int spend = 0;
   //A sorted array of removed indices ( fill these when adding into spdat )
   SortedArray<int, CAP - 1> remIDs;
   
   void Clear();
   SubProcesses() {}
   ~SubProcesses() { Clear(); }
   SubProcess<RT> * GetSubProc(const unsigned int i) { return reinterpret_cast<SubProcess<RT>*>(spdat + sizeof(SubProcess<RT>) * i); }
   
   template <typename CallT>
   unsigned int Add(CallT func);
   
   void Remove(const unsigned int i);
   
   /*returns the first subprocess (over or at the given index) with an unlocked result, 
   	and locks it (if an unlocked result is found), index is return value, given pointer reference 
   	is filled with the subprocess address. if no unlocked results are found the pointer is nullified */
   unsigned int BeginCheckAnyFrom(SubProcess<RT>*& pref, unsigned int i = 0);

protected:
   //Get the first index not in use by a SubProcess + remove it from remIDs if obtained from there, or increment spend
   unsigned int FindUnusedIndex(); 
};

template <unsigned int CAP, typename RT>
unsigned int SubProcesses<CAP, RT>::FindUnusedIndex() {
  if (remIDs.total > 0) { //maybe take from end of remIDs to reduce shifting time
     int i = remIDs[0];
     remIDs.RemoveIndex(0);
     return i; 
  }
  spend++;
  return spend - 1;
}

template <unsigned int CAP, typename RT>
template <typename CallT>
unsigned int SubProcesses<CAP, RT>::Add(CallT func) { 
  unsigned int i = FindUnusedIndex();
  new (spdat + sizeof(SubProcess<RT>) * i) SubProcess<RT> (func);
  return i;
}

template <unsigned int CAP, typename RT>
void SubProcesses<CAP, RT>::Remove(const unsigned int i) {
  GetSubProc(i)->~SubProcess<RT>();
  
  if (i == spend - 1) { 
     spend--;
     while ((spend > 0) && spend - 1 == remIDs.Last()) {
       spend--;
       remIDs.RemoveEnd();
     }
  }
  else { remIDs.Add(i); }
}

template <unsigned int CAP, typename RT>
void SubProcesses<CAP, RT>::Clear() {
  unsigned int remi = 0;
  for (unsigned int i = 0; i < spend ; i++) {
    if (i == remIDs[remi]) { remi++; continue; }
    GetSubProc(i)->~SubProcess<RT>();
  }
  spend = 0;
  remIDs.Clear();
}

template <unsigned int CAP, typename RT>
unsigned int SubProcesses<CAP, RT>::BeginCheckAnyFrom(SubProcess<RT>*& pref, unsigned int i) {
  unsigned int remi = remIDs.BinarySearch(i);
  if ( (i > remi) && (remIDs.total - 1 != remi) ) { remi++; }
  
  for (; i < spend ; i++) {
    if (i == remIDs[remi]) { remi++; continue; }
    pref = GetSubProc(i);
    if (pref->BeginCheckBusy()) { return i; }
  }
  
  pref = nullptr;
  return i;
}


