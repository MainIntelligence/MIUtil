#pragma once

//CRTP Utility to define a bunch of operations using Inverse, *=, +=
template <typename T>
struct FieldType {
   T& This() { return static_cast<T&>(*this); }
   const T& This() const { return static_cast<const T&>(*this); }
   
   T operator+(const T& o2) const;
   T operator-(const T& o2) const;
   T operator*(const T& o2) const;
   T operator/(const T& o2) const;
   
   void operator-=(const T& o2);
   void operator/=(const T& o2);
   //Unary negation == -1*This()
   T operator-() const;
   T Inverse() const;
};

template <typename T>
T FieldType<T>::operator+(const T& o2) const {
   T rv = This();
   rv += o2;
   return rv;
}

template <typename T>
T FieldType<T>::operator-(const T& o2) const {
   T rv = This();
   rv -= o2;
   return rv;
}

template <typename T>
T FieldType<T>::operator*(const T& o2) const {
   T rv = This();
   rv *= o2;
   return rv;
}

template <typename T>
T FieldType<T>::operator/(const T& o2) const {
   T rv = This();
   rv /= o2;
   return rv;
}

template <typename T>
void FieldType<T>::operator-=(const T& o2) {
   This() += -o2;
}

template <typename T>
void FieldType<T>::operator/=(const T& o2) {
   This() *= o2.Inverse();
}

template <typename T>
T FieldType<T>::operator-() const {
   return -1 * This();
}

template <typename T>
T FieldType<T>::Inverse() const {
   T rv = This();
   rv.Invert();
   return rv;
}


