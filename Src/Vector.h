#pragma once
#include "Utils.h"
//#include <iostream>

template <typename T>
class Vector {

public:
  size_t capElements = 0;
  size_t numElements = 0;
  T* data = nullptr;

  void Resize(size_t newCap);
  //check to resize the vector before adding a number of elements
  void CheckResizeAdd(const unsigned int toadd);
  
  
//Constructors
  Vector() : capElements{ 0 }, numElements{ 0 }, data{ nullptr } {}
  Vector(size_t startCap) : capElements{ startCap }, numElements{ 0 }, data{ new T[startCap] } {}
  Vector(const T* dat, size_t len) : capElements{ len }, numElements{ len }, data{ new T[len] } { pcpy(data, len, dat); }
  Vector(const T* dat, size_t len, size_t cap) : capElements{ cap }, numElements{ len }, data{ new T[cap] } { pcpy(data, len, dat); }
  ~Vector() { delete data; }
  
  //named/simple accessors
  size_t Size() const noexcept { return numElements; }
  size_t Capacity() const noexcept { return capElements; }
  
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
template <typename T>
void Vector<T>::Resize(size_t newCap) {
  T* newData = new T[newCap];
  if (data) {
    pcpy(newData, numElements, data);
    delete data;
  }
  capElements = newCap; //newSize should never be less than the number of elements
  data = newData;
}

template <typename T>
void Vector<T>::CheckResizeAdd(const unsigned int toadd) {
  unsigned int needed = numElements + toadd;
  unsigned int newcap = capElements;
  if (newcap == 0) { newcap = 1; } //should just make 1 the minimum size, but whatever
  
  while (newcap < needed) { newcap <<= 1; }
  if (newcap == capElements) { return; }
  
  T * newmem = new T [newcap];
  //copy data to new memory
  for ( unsigned int i = 0; i < numElements ; i++ ) { newmem[i] = data[i]; }
  //free and swap
  delete data;
  data = newmem;
  capElements = newcap;
}

template <typename T>
void Vector<T>::AddEnd(const T& obj) {
  if (numElements == capElements) { Resize(2 * ((capElements >= 1) ? capElements : 1)); }
  *(data + numElements) = obj;
  numElements++;
}

template <typename T>
void Vector<T>::AddEnd(const T* objs, const unsigned int n) {
 CheckResizeAdd(n);
 for (unsigned int i = 0; i < n ; i++) {
   data[numElements + i] = objs[i];
 }
 numElements += n;
}


template <typename T>
void Vector<T>::ShiftRemoveIndex(const int i)  {
  for(T* pdata = data + i; pdata < pdata + numElements - 1; pdata++) {
    *pdata = *(pdata + 1);
  }
  numElements--;
  *(data + numElements) = T{};
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& vec) {
  if (data != nullptr) {
    if (capElements < vec.numElements) { //needs resize
      delete data;
      capElements = vec.capElements;
      data = new T[capElements]{};
    }
  }
  
  numElements = vec.numElements;
  pcpy(data, numElements, vec.data);
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& vec) {
  Swap(capElements, vec.capElements);
  Swap(numElements, vec.numElements);
  Swap(data, vec.data);
  return *this;
}


