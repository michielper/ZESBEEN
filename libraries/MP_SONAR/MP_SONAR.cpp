// 	MP_SONAR.CPP
//	2019/12/07

#include "Arduino.h"
#include <limits.h>
#include <MP_DEBUG.h>
#include <MP_SONAR.h>

SONAR::SONAR( int trig_pin, int echo_pin)
{
	my_trig_pin = trig_pin;
	my_echo_pin = echo_pin;
	pinMode(my_trig_pin, OUTPUT);
	pinMode(my_echo_pin, INPUT);

};

// fysica:
// geluidssnelheid bij kamertemp = 343 m/s
// dat is ongeveer 30 microsec per cm
// omdat de echo het dubbele pad aflegt
// wordt dat 60 microsec / cm

//----------------------------------
// Meet geeft afstand in centimeters
//----------------------------------
int SONAR::Meet()
{
	int  result;
	long duration;
	
	digitalWrite(my_trig_pin, LOW);  
	delayMicroseconds(2); 
	digitalWrite(my_trig_pin, HIGH);
	delayMicroseconds(10); 
	digitalWrite(my_trig_pin, LOW);
	duration = pulseIn(my_echo_pin, HIGH);
	duration = constrain(duration, 0, INT_MAX);
	result 	 = duration / 60;
	return result;
};
