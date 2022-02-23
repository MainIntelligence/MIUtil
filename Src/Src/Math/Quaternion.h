#pragma once
#include "FieldType.h"

template <typename T>
struct Quaternion : public FieldType<Quaternion<T>> {
   T data [4] {0};
   
   Quaternion() : data{0} {}
   Quaternion(const double r, const double i, const double j, const double k);
   T& operator[](const unsigned int i) { return data[i]; } 
   
   void operator+=(const Quaternion<T>& q);
   void operator*=(const Quaternion<T>& q); //Hamilton product
   void Invert();

   T SquareMagnitude() const;
   T Norm() { return sqrt( SquareMagnitude() ); }
   void MakeVersor(); //versor == unit quaternion (polar decomp == Norm * Versor)
   
};
template <typename T>
Quaternion<T>::Quaternion(const double r, const double i, const double j, const double k) :
	data{r, i, j, k} {}

template <typename T>
void Quaternion<T>::operator+=(const Quaternion<T>& q) {
   for (unsigned int i = 0; i < 4; i++) {
      data[i] += q.data[i];
   }
}

template <typename T>
void Quaternion<T>::operator*=(const Quaternion<T>& q) {
   Quaternion<T> t;
   t.data[0] = data[0] * q.data[0] - data[1] * q.data[1] - data[2] * q.data[2] - data[3] * q.data[3];
   t.data[1] = data[0] * q.data[1] + data[1] * q.data[0] + data[2] * q.data[3] - data[3] * q.data[2];
   t.data[2] = data[0] * q.data[2] - data[1] * q.data[3] + data[2] * q.data[0] + data[3] * q.data[1];
   t.data[3] = data[0] * q.data[3] + data[1] * q.data[2] - data[2] * q.data[1] + data[3] * q.data[0];
   for (unsigned int i = 0; i < 4; i++) { data[i] = t.data[i]; }
}

template <typename T>
void Quaternion<T>::Invert() {
   T sqmag = SquareMagnitude();
   data[0] /= sqmag;
   for ( unsigned int i = 1; i < 4; i++ ) { data[i] /= -sqmag; }
}

template <typename T>
T Quaternion<T>::SquareMagnitude() const {
   T sum = 0;
   for (unsigned int i = 0; i < 4; i++) { sum += data[i] * data[i]; }
   return sum;
}

template <typename T>
void Quaternion<T>::MakeVersor() {
   *this /= Norm();
}

template <typename T, typename T2>
static Quaternion<T> operator/=(Quaternion<T> obj, const T2 item) {
   for ( unsigned int i = 0; i < 4; i++) {
      obj.data[i] /= item; 
   }
   return obj;
}

//Confuses this with item for quaternion 
template <typename T>
static Quaternion<T> operator*(const T item, Quaternion<T> obj) {
   for ( unsigned int i = 0; i < 4; i++) {
      obj[i] *= item; 
   }
   return obj;
}
