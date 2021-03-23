//------------------------------------
// File:	MP_LCD.cpp
//
// 2021-03-22	Initieel
//------------------------------------

#include <LiquidCrystal_I2C.h>
#include <MP_LCD.h>

//------------------------------------------------
LCDISPLAY::LCDISPLAY(  LiquidCrystal_I2C* device )
{
	lcd = device;
	memset( reg0, 0, sizeof(reg0));
	memset( reg1, 0, sizeof(reg1));
	memset( reg0_prev, 0, sizeof(reg0_prev));
	memset( reg1_prev, 0, sizeof(reg1_prev));
}
//------------------------------------------------
LCDISPLAY::toon()
{
	// vernieuw display alleen als inhoud is gewijzigd
	// om knipperen te voorkomen
	if (	strncmp( reg0, reg0_prev, LCD_LENGTE) ||
			strncmp( reg1, reg1_prev, LCD_LENGTE) )
	{
		lcd->clear();
		lcd->setCursor(0,0); lcd->print(reg0);  
		lcd->setCursor(0,1); lcd->print(reg1);  
		strncpy( reg0_prev, reg0, LCD_LENGTE);
		strncpy( reg1_prev, reg1, LCD_LENGTE);
	};
	
}
