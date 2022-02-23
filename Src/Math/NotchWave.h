#pragma once
#include "Complex.h"
const double __TAO = 2 * 3.14159265;

//A wave represented by a winding frequency (a proportionality constant, relative to a notch)

template <typename AMPT>
struct NotchWave {
	AMPT mag; //defines amplitude
	Complex<double> rotator; //characteristic of frequency (cycles/notch)
	Complex<double> direction = 1; //current direction
	
	Complex<double> GetValue() { return mag * direction; }
	double GetReal() { return (mag * direction).Re(); }
	double GetIm() { return (mag * direction).Im(); }
	
	//windfreq describes the proportion of a rotation encapsulated by a notch. a 2hz wave (1 cycle per half second), notched every second would give 2. (2 cycles per notch)
	//	Tn * f_w = windwfreq (notch time (delta time) * wave frequency (inverse dimension))
	NotchWave() {};
	//cycles per notch (probably much smaller than 1 in most real cases) and amplitude
	NotchWave(const double windfreq, const AMPT magnitude = 1);
	void Progress() { direction *= rotator; }
	AMPT& Magnitude() { return mag; }
	
	//static NotchWave NotchedWave(const double cycpnotch, const AMPT amp );
};

//cycles/notch == (micros/notch) * (cycles/second ~ Hz) / 1000000
template <typename AMPT>
static NotchWave<AMPT> NotchedWave(const double cycpnotch, const AMPT amp) { 
   return NotchWave<AMPT>(cycpnotch, amp);
}

template <typename AMPT>
NotchWave<AMPT>::NotchWave(const double windfreq, const AMPT magnitude) : 
	mag{magnitude}, rotator(Complex<double>::Rotator(__TAO * windfreq)) {}


//A wave made by composing the heights of other waves at points in time (additively)
//  ex. for forming more complex sounds from simple sine frequencies.
template <typename AMPT>
struct ComposedNotchWave {
        //up to 32 simple waves being composed into a final wave
	NotchWave<AMPT> waves [32] {};
	unsigned int totalWaves = 0;
	double amp = 0;
	const double mspernotch; //must be constant over process
	
	ComposedNotchWave( const double mspernotch );
	//Add a wave with the given Hz frequency and amplitude
	void AddWave(const unsigned int hzfreq, const unsigned int amp);
	/*update amplitude (sum of real parts over waves), go a notch forward */
	void Progress();
	
	double Magnitude() const { return amp; }
};

template <typename AMPT>
ComposedNotchWave<AMPT>::ComposedNotchWave( const double msnotch ) : mspernotch{msnotch} {}

template <typename AMPT>
void ComposedNotchWave<AMPT>::AddWave(const unsigned int hzfreq, const unsigned int amp) {
   waves[totalWaves++] = 
   	NotchedWave<AMPT>(mspernotch * hzfreq / 1000000, amp);
}

template <typename AMPT>
void ComposedNotchWave<AMPT>::Progress() {
	amp = 0;
	for (int i = 0; i < totalWaves ; i++) {
		amp += (waves[i]).GetReal();
		waves[i].Progress();
	};
}



