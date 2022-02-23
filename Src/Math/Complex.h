#pragma once
#include "FieldType.h"
//#include <math.h> //include this yourself if you need Magnitude(), or GetAngle(double)
template <typename T>
T cos(T angle);
template <typename T>
T sin(T angle);

template <typename T> 
struct Complex : public FieldType<Complex<T>> {
	T data [2] {0};
	
	Complex() { data[0] = 0; data[1] = 0; }
	Complex(T re) { data[0] = re; data[1] = 0; }
	Complex(T re, T im) { data[0] = re; data[1] = im; }
	
	void operator=(const Complex& other) { Re() = other.Re(); Im() = other.Im(); }
	void GetAngle(double angle) { Re() = cos(angle); Im() = sin(angle); }
	
	T SquareMagnitude() const { return Re() * Re() + Im() * Im(); }
	T Magnitude() const { return sqrt(SquareMagnitude()); }
	
	T Re() const { return data[0]; }
	T Im() const { return data[1]; }
	T& Re() { return data[0]; }
	T& Im() { return data[1]; }
	
	//static Complex<T> operator*(const T item, Complex<T> obj);
	
	void operator+=(const Complex& other);
	void operator*=(const Complex& other);
	void Invert();
	//Get a complex number that rotates other complex numbers by the given angle
	static Complex<T> Rotator(const double angle) { return Complex<T>(cos(angle), sin(angle)); }
};

template <typename T>
void Complex<T>::operator+=(const Complex& other) { 
   Re() += other.Re(); 
   Im() += other.Im();
}

template <typename T>
void Complex<T>::operator*=(const Complex& other) { 
   T re = Re();
   Re() = (Re() * other.Re()) - (Im() * other.Im());
   Im() = (re * other.Im()) + (Im() * other.Re());
}

template <typename T>
void Complex<T>::Invert() {
   T mag = SquareMagnitude();
   Re() /= mag;
   Im() /= -mag;
}

template <typename T, typename T2>
static Complex<T> operator*(const T2 item, Complex<T> obj) {
   return Complex<T>{item * obj.Re(), item * obj.Im()};
}
