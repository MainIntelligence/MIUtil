#pragma once
//#include <math.h>
// probably want to use glm for sin and cos anyways \_(-_-)_/
#include "Quaternion.h"
#include <math.h>

struct Rotator3 {
   Quaternion<double> q;
   Quaternion<double> qinv;
   
   
   void Update(double angleup, double angleleft, double xrnorm, double yrnorm);
   
   void Rotate(double * pv);
};

void Rotator3::Update(double angleup, double angleleft, double xrnorm, double yrnorm) {
   double cosaz = cos(angleleft/2.0f);
   double costh = cos(angleup/2.0f);
   double sinaz = sin(angleleft/2.0f);
   double sinth = sin(angleup/2.0f);
   
   q.data[0] = cosaz * costh;
   q.data[1] = (xrnorm * cosaz - yrnorm * sinaz) * sinth;
   q.data[2] = (xrnorm * sinaz + yrnorm * cosaz) * sinth;
   q.data[3] = sinaz * costh;
   
   qinv = q.Inverse();
};

void Rotator3::Rotate(double * pv) {
   Quaternion<double> p ( 0, *pv, *(pv + 1), *(pv + 2) );
   p = q * p * qinv;
   for (unsigned int i = 0; i < 3; i++) {
      *(pv + i) = p[1 + i];
   }
}
