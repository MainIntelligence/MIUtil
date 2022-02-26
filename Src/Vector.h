#pragma once
#include "Utils.h"
//#include <iostream>

template <typename T, typename ALIGNT = T>
class Vector {

public:
  unsigned int capElements = 0;
  unsigned int numElements = 0;
  alignas(ALIGNT) T* data = nullptr;

  void Resize(unsigned int newCap);
  //check to resize the vector before adding a number of elements
  void CheckResizeAdd(const unsigned int toadd);
  
  
//Constructors
  Vector() : capElements{ 0 }, numElements{ 0 }, data{ nullptr } {}
  Vector(unsigned int startCap) : capElements{ startCap }, numElements{ 0 }, data{ new T[startCap]{} } {}
  Vector(const T* dat, unsigned int len) : capElements{ len }, numElements{ len }, data{ new T[len]{} } { pcpy(data, len, dat); }
  Vector(const T* dat, unsigned int len, unsigned int cap) : capElements{ cap }, numElements{ len }, data{ new T[cap] } { pcpy(data, len, dat); }
  constexpr Vector(const Vector&);
  
  ~Vector() { if (data) { delete [] data; } }
  
  //named/simple accessors
  unsigned int Size() const noexcept { return numElements; }
  unsigned int Capacity() const noexcept { return capElements; }
  
  const T& operator[](const int i) const { return data[i]; }
  T& operator[](const int i) { return data[i]; }
  T& LastItem() { return data[Size() - 1]; }
  T& End() { return data[Size()]; }
  //Shrinks the capacity (allocated space) to just enough to hold the current data.
  void FinalizeSize(unsigned int fincap) noexcept { Resize(fincap); }

  //Adds an element to the end and resizes if necessary.
  void AddEnd(const T& obj);
  //Adds n objects, added sequentially from the given memory. Not safe when you have no alloc memory
  void AddEnd(const T* objs, const unsigned int n);
  void ShiftRemoveIndex(const int i);

  //Hard Copy (frees and allocates if cannot contain the other vectors data)
  Vector& operator=(const Vector& vec);
  //Move operator (swaps the data pointers and sizes/capacities)
  Vector& operator=(Vector&& vec);

};

//**************** Implementations ********************
template <typename T, typename ALIGNT>
constexpr Vector<T, ALIGNT>::Vector(const Vector& v) {
   if (capElements < v.numElements) { 
       if ( data != nullptr ) { delete[] data; }
       if ( capElements == 0 ) { capElements = 1; }
       
       do { 
          capElements <<= 1;
       } while (capElements < v.numElements);
       
       data = new T[capElements]{};
   }
   
   numElements = v.numElements;
   for ( unsigned int i = 0; i < v.numElements ; i++) {
      *(data + i) = *( v.data + i );
   }
}


template <typename T, typename ALIGNT>
void Vector<T, ALIGNT>::Resize(unsigned int newCap) {
  T* newData = new T[newCap]{};
  if (data) {
    for ( unsigned int i = 0; i < numElements ; i++ ) { *(newData + i) = *(data + i); }
    delete [] data;
  }
  capElements = newCap; //newSize should never be less than the number of elements
  data = newData;
}

template <typename T, typename ALIGNT>
void Vector<T, ALIGNT>::CheckResizeAdd(const unsigned int toadd) {
  unsigned int needed = numElements + toadd;
  unsigned int newcap = capElements;
  if ( needed <= capElements ) { return; }
  
  if (newcap == 0) { newcap = 1; } //should just make 1 the minimum size, but whatever
  
  while (newcap < needed) { newcap <<= 1; }
  
  T * newmem = new T [newcap]{};
  //copy data to new memory
  if (data != nullptr) {
    for ( unsigned int i = 0; i < numElements ; i++ ) { newmem[i] = data[i]; }
    delete [] data;
  }
  
  data = newmem;
  capElements = newcap;
}

template <typename T, typename ALIGNT>
void Vector<T, ALIGNT>::AddEnd(const T& obj) {
  if (numElements == capElements) { CheckResizeAdd(1); }
  *(data + numElements) = obj;
  numElements++;
}

template <typename T, typename ALIGNT>
void Vector<T, ALIGNT>::AddEnd(const T* objs, const unsigned int n) {
 CheckResizeAdd(n);
 for (unsigned int i = 0; i < n ; i++) {
   data[numElements + i] = objs[i];
 }
 numElements += n;
}


template <typename T, typename ALIGNT>
void Vector<T, ALIGNT>::ShiftRemoveIndex(const int i)  {
  for(T* pdata = data + i; pdata < pdata + numElements - 1; pdata++) {
    *pdata = *(pdata + 1);
  }
  numElements--;
  *(data + numElements) = T{};
}


template <typename T, typename ALIGNT>
Vector<T, ALIGNT>& Vector<T, ALIGNT>::operator=(const Vector<T, ALIGNT>& v) {
   if (capElements < v.numElements) { 
       if ( data != nullptr ) { delete [] data; }
       if ( capElements == 0 ) { capElements = 1; }
       
       do { 
          capElements <<= 1;
       } while (capElements < v.numElements);
       
       data = new T[capElements]{};
   }
   
   numElements = v.numElements;
   for ( unsigned int i = 0; i < v.numElements ; i++) {
      *(data + i) = *( v.data + i );
   }
   return *this;
}

template <typename T, typename ALIGNT>
Vector<T, ALIGNT>& Vector<T, ALIGNT>::operator=(Vector<T, ALIGNT>&& vec) {
  Swap(capElements, vec.capElements);
  Swap(numElements, vec.numElements);
  Swap(data, vec.data);
  return *this;
}


