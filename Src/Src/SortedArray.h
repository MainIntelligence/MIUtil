#pragma once

template <typename T, unsigned int CAP>
struct SortedArray {
   unsigned int total = 0;
   T arr [CAP] {};
   
   T& operator[]( const unsigned int i ) { return arr[i]; }
   
   
   //Returns the index of the item if it exists, otherwise returns an index close to where the item would be
   unsigned int BinarySearch(const T item) const noexcept;
   
   //starts shifting values from end of array, then inserts at valid location
   template <typename ...Args>
   void Add(Args... args);
   
   //removes the value at the given index
   void RemoveIndex(unsigned int i);
   
   T& Last() { return arr[total - 1]; }
   void RemoveEnd() { (arr[total - 1]).~T(); total--; }
   
   void Clear() { total = 0; }
};

template <typename T, unsigned int CAP>
unsigned int SortedArray<T, CAP>::BinarySearch(const T item) const noexcept {

   if ( item <= arr[0] ) { return 0; }
   if ( item >= arr[total-1] ) { return total; }
   unsigned int loweri = 0;
   unsigned int higheri = total - 1;
   unsigned int i = total/2;
   while( higheri-loweri > 1 ) {
      if ( arr[i] == item ) { return i; }
      else if ( arr[i] > item ) { higheri = i; }
      else if ( arr[i] < item ) { loweri = i; }
      i = (loweri + higheri) / 2;
   }
   return i;
}

template <typename T, unsigned int CAP>
template <typename ...Args>
void SortedArray<T, CAP>::Add(Args... args) {
  T newelm (args...);
  unsigned int i = total;
  for (; (i > 0) && (arr[i-1] > newelm) ; i--) {
     arr[i] = arr[i-1];
  }
  arr[i] = newelm;
  total++;
}

template <typename T, unsigned int CAP>
void SortedArray<T, CAP>::RemoveIndex( unsigned int i ) {
  for (; i < total ; i++) {
     arr[i] = arr[i+1];
  }
  total--;
}

