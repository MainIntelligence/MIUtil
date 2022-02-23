#pragma once
#include <thread>
#include <mutex>


//Just using some desirable naming conventions here ;)
struct Lockable {
  std::mutex mtx;
  
  bool BeginUseBusy()   { return mtx.try_lock(); }
  void BeginUseWait()   { mtx.lock();     }
  void EndUse()         { mtx.unlock();   }
};

//A convenience thing using RAII to make proper lock handling slightly easier
struct BusyAccessor {
  Lockable& lock;
  bool accessed = false;
  
  BusyAccessor(Lockable& lock) : lock(lock) { accessed = lock.BeginUseBusy(); }
  ~BusyAccessor() { if (accessed) { lock.EndUse(); } }
};
struct WaitAccessor {
  Lockable& lock;
  
  WaitAccessor(Lockable& lock) : lock(lock) { lock.BeginUseWait(); }
  ~WaitAccessor() { lock.EndUse(); }
};


//Helper which may optionally be passed on creation of subprocess, a mutex is generally needed,
//	but may be passed without data (like when the threads need to share a character device)
template <typename T>
struct SharedData : public Lockable {
   T data;
};


//Process thread wrapper - lifetime of SubProcess == lifetime of worker thread
struct SubProcess { 
   std::thread thread;
   
   template <typename CallT, typename ... Args>
   SubProcess(CallT func, Args... args) : thread(func, std::ref(args...)) {};
   ~SubProcess() { thread.join(); }
};

#include "SortedArray.h"
//NOTE: NO SAFETY IN MAKING SURE YOU DON'T ADD MORE THAN "CAP" SubProcesses
template <unsigned int CAP>
struct SubProcesses {
   //Making this an explicit array of SubProcesses might need automatically constructing them (which is bad)
   alignas(SubProcess) char spdat [CAP * sizeof(SubProcess)];
   unsigned int spend = 0;
   //A sorted array of removed indices ( fill these when adding into spdat )
   SortedArray<int, CAP - 1> remIDs;
   
   void Clear();
   SubProcesses() {}
   ~SubProcesses() { Clear(); }
   SubProcess* GetSubProc(const unsigned int i) { return reinterpret_cast<SubProcess*>(spdat + sizeof(SubProcess) * i); }
   SubProcess& operator[]( const unsigned int i ) { return *GetSubProc(i);}
      
   template <typename CallT, typename ...Args>
   unsigned int Add(CallT func, Args... args);
   void Remove(const unsigned int i);
   
   /*returns the first subprocess (over or at the given index) with an unlocked result, 
   	and locks it (if an unlocked result is found), index is return value, given pointer reference 
   	is filled with the subprocess address. if no unlocked results are found the pointer is nullified */
   //unsigned int BeginCheckAnyFrom(SubProcess*& pref, unsigned int i = 0);

protected:
   //Get the first index not in use by a SubProcess + remove it from remIDs if obtained from there, or increment spend
   unsigned int FindUnusedIndex(); 
};

template <unsigned int CAP>
unsigned int SubProcesses<CAP>::FindUnusedIndex() {
  if (remIDs.total > 0) { //maybe take from end of remIDs to reduce shifting time
     int i = remIDs[0];
     remIDs.RemoveIndex(0);
     return i; 
  }
  spend++;
  return spend - 1;
}

template <unsigned int CAP>
template <typename CallT, typename ...Args>
unsigned int SubProcesses<CAP>::Add(CallT func, Args... args) { 
  unsigned int i = FindUnusedIndex();
  new (spdat + sizeof(SubProcess) * i) SubProcess (func, args...); //construct SubProcess
  return i;
}

template <unsigned int CAP>
void SubProcesses<CAP>::Remove(const unsigned int i) {
  GetSubProc(i)->~SubProcess();
  
  if (i == spend - 1) { 
     spend--;
     while ((spend > 0) && spend - 1 == remIDs.Last()) {
       spend--;
       remIDs.RemoveEnd();
     }
  }
  else { remIDs.Add(i); }
}

template <unsigned int CAP>
void SubProcesses<CAP>::Clear() {
  unsigned int remi = 0;
  for (unsigned int i = 0; i < spend ; i++) {
    if (i == remIDs[remi]) { remi++; continue; }
    GetSubProc(i)->~SubProcess();
  }
  spend = 0;
  remIDs.Clear();
}

/*
template <unsigned int CAP, typename RT>
unsigned int SubProcesses<CAP, RT>::BeginCheckAnyFrom(SubProcess*& pref, unsigned int i) {
  unsigned int remi = remIDs.BinarySearch(i);
  if ( (i > remi) && (remIDs.total - 1 != remi) ) { remi++; }
  
  for (; i < spend ; i++) {
    if (i == remIDs[remi]) { remi++; continue; }
    pref = GetSubProc(i);
    if (pref->BeginCheckBusy()) { return i; }
  }
  
  pref = nullptr;
  return i;
}*/


