#include <MP_LED.h>

LED::LED( int pin)
{
  mypin = pin;
  pinMode( mypin, OUTPUT);
  isAan = false;
  previousMillis = 0;
}// Init

void LED::Aan()
{
  digitalWrite( mypin, HIGH);
  isAan = true;
}

void LED::Uit()
{
  digitalWrite( mypin, LOW);
  isAan = false; 
}

void LED::Set( int state)
{
  if (LOW == state) Uit(); else Aan();
}

void LED::Toggle()
{
  if (isAan) Uit();  else Aan();
}

void LED::Blink(unsigned long interval)
{
  unsigned long currentMillis = millis();
   
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    Toggle();
  }
}

LEDARRAY::LEDARRAY(int aantal_leds, LED* ledlist[])
{
	MAX = aantal_leds - 1;
	p 	= ledlist;
};

void LEDARRAY::Uit()	// alle uit
{
	for (int i = 0; i <= MAX; i++)
	{
		p[i]->Uit();
	}
};

void LEDARRAY::Uit(int x)
{
	x = constrain(x, 0, MAX);
	p[x]->Uit();
};

void LEDARRAY::Add(int x)
{
	x = constrain(x, 0, MAX);
	p[x]->Aan();
};

void LEDARRAY::Aan(int x)
{
	x = constrain(x, 0, MAX);
	p[huidig]->Uit();
	p[x]->Aan();
	huidig = x;
};

void LEDARRAY::Aan()	// alle aan
{
	for (int i = 0; i <= MAX; i++)
	{
		p[i]->Aan();
	}
};

void LEDARRAY::Toggle(int x)
{
	x = constrain(x, 0, MAX);
	p[x]->Toggle();
}
//============================================
