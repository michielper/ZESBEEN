// 2021-02-16	initieel

#include "Arduino.h"
#include <MP_INPUTKANAAL.h>

INPUTKANAAL::INPUTKANAAL( int pin )
{
	mypin = pin;
	pinMode( mypin, INPUT_PULLUP);
	laatste_waarde	= false;
};
//-----------------------------------
bool	INPUTKANAAL::Opgaand()
{
  bool result = false;
  bool huidig = Kijk();
  
  result 			= ( !laatste_waarde && huidig );
  laatste_waarde 	= huidig;

  return result;
};
//-----------------------------------
bool	INPUTKANAAL::Neergaand()
{
  bool result = false;
  bool huidig = Kijk();
  
  result 			= ( laatste_waarde && !huidig );
  laatste_waarde 	= huidig;

  return result;
};
//-----------------------------------
bool	INPUTKANAAL::Kijk()			// actuele waarde, true indien HIGH
{
	return ( digitalRead( mypin ) == HIGH );
};

