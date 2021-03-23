//------------------------------------
// File:	MP_LCD.h
//
// 2021-03-22	Initieel
//------------------------------------
		
#define 	LCD_LENGTE	16		

class	LCDISPLAY
{
	public:
		LCDISPLAY( LiquidCrystal_I2C* device);
		void		toon();
		char		reg0[LCD_LENGTE + 1]; 
		char		reg1[LCD_LENGTE + 1];
	private:
		LiquidCrystal_I2C* 	lcd;  		// set the LCD address for a 16 chars and 2 line display
		char		reg0_prev[LCD_LENGTE + 1];
		char		reg1_prev[LCD_LENGTE + 1];
};
