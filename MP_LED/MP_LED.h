#include "Arduino.h"
#include <inttypes.h>
#include <time.h>

class LED
{
	private:
		int     mypin;
		unsigned long previousMillis;        // will store last time LED was updated    
	public:
		boolean isAan;
		LED( int pin);
		void Aan();
		void Uit();
		void Set(int state);
		void Toggle();
		void Blink(unsigned long interval);
}; // class LED

//---- voorbeeld van gebruik:
//	LED geel1(51), geel2(47), geel3(43), geel4(39);
//	LED*  gelen[] = {&geel1, &geel2, &geel3, &geel4};
//	LEDARRAY  geel(4, gelen);
//  geel.Aan(3);

class LEDARRAY
{
	public:
		LEDARRAY(int size, LED* ledlist[]);
		int MAX;
		void Uit();			// huidig uit
		void Uit(int x);
		void Add(int x);	// x aan, andere ongemoeid
		void Aan(int x);	// huidig uit en x aan
		void Aan();			// alle leds aan
		void Toggle(int x);
	private:
		LED **p;
		int	huidig;			// brandende led
};

