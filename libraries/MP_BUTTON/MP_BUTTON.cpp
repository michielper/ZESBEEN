#include "Arduino.h"
#include <MP_DEBUG.h>
#include <MP_BUTTON.h>

#define DEBUG
//---------------------------------------
BUTTON::BUTTON( int pin)
{
	mypin = pin;
	
	pinMode( mypin, INPUT_PULLUP);
	
	DBZEGLN3("BUTTON geinitialiseerd op pin ", mypin);
}
//-----------------------------------------
bool BUTTON::Kijk()  // true zolang knop ingedrukt (LOW)
{
	bool result;
	result = (digitalRead(mypin) == LOW);
	return result;
} // end Kijk
//---------------------------------------
bool BUTTON::Losgelaten()	// true als net losgelaten
{
  bool result = false;
  bool huidig = Kijk();

  DBZEGLN4("BUTTON pin huidig vorig: ", mypin, huidig, vorige);
  
  if ( vorige && !huidig ) // knop was ingedrukt maar nu los
  {
    result = true;
  }
  vorige = huidig;
  DBZEGLN4( "Button ", mypin, " Losgelaten: ", result);
  return result;
} // end Losgelaten
//---------------------------------------
bool BUTTON::Ingedrukt()		// true als net ingedrukt
{
  bool result = false;
  bool huidig = Kijk();

  DBZEGLN4("BUTTON pin huidig vorig: ", mypin, huidig, vorige);
  
  if ( !vorige && huidig ) // knop was los maar nu ingedrukt
  {
    result = true;
  }
  vorige = huidig;
  DBZEGLN4( "Button ", mypin, " Ingedrukt: ", result);
  return result;
} // end Losgelaten
