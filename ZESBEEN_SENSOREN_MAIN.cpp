// HV_sensoren_en_display
// 2021-01-26  initieel, 4x 4-digit display voor voedingspanningen
// 2021-02-05  pins voor displays veranderd, LED arrays toegevoegd
// 2021-02-05  sonar toegevoegd
// 2021-02-18	LCD display ipv 4-digit displays
// 2021-02-20	gebruik snprintf voor LCD display
// 2021-02-20	voer debug toe
//-------------------------------------------------

#define DEBUG

#include "Arduino.h"
//~ #include <TM1637Display.h>
 #include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <MP_DEBUG.h>
#include <MP_LED.h>
#include <MP_BUTTON.h>
#include <ZB_SEN_CONFIG.h>
#include <ZB_GEMEEN.h>
#include <MP_SONAR.h>
#include <MP_LICHTMETER.h>
#include <HV_GEWRICHT.h>
#include <ZB_STUURBEEST.h>
#include <MP_INPUTKANAAL.h>
#include <TimerObject.h>
		

#define LOOP_DELAY              30      // ms


// Actuatoren (schouders)
GEWRICHT        	rvs("RVS", RVS_PIN, RVS_COR); 
GEWRICHT        	ras("RAS", RAS_PIN, RAS_COR);  
GEWRICHT        	lvs("LVS", LVS_PIN, LVS_COR);  
GEWRICHT        	las("LAS", LAS_PIN, LAS_COR);

STUURBEEST  		stuurbeest( &rvs, &ras, &lvs, &las );

GEM_LOOPSTATUS		loopstatus;
GEM_RICHTINGSTATUS	richtingstatus;

// LCD display
LiquidCrystal_I2C 	lcd(0x27,16,2);  		// set the LCD address for a 16 chars and 2 line display
const int 			LCD_LENGTE = 16;		// 16 + 1 voor de /0
char				regel0[LCD_LENGTE + 1], 
					regel1[LCD_LENGTE + 1],
					regel0_prev[LCD_LENGTE + 1],
					regel1_prev[LCD_LENGTE + 1];

// Leds op uitgaande communicatiekanalen
LED   			ledA(C2_KANAAL_A_PIN), 
				ledB(C2_KANAAL_B_PIN); 
// Inkomende communicatie
INPUTKANAAL		inputC( C2_KANAAL_C_PIN ), inputD( C2_KANAAL_D_PIN );
				
// SONAR initialisatie met trig_pin, echo_pin
SONAR       	SonarLinks(		SONAR_LINKS_TRIG_PIN,	SONAR_LINKS_ECHO_PIN);
SONAR       	SonarRechts(	SONAR_RECHTS_TRIG_PIN,	SONAR_RECHTS_ECHO_PIN);

LED				Led1SonarLinks( LED1_SONAR_LINKS_PIN );
LED				Led2SonarLinks( LED2_SONAR_LINKS_PIN );
LED				Led1SonarRechts( LED1_SONAR_RECHTS_PIN );
LED				Led2SonarRechts( LED2_SONAR_RECHTS_PIN );

// Globals, bijgewerkt door callback functies
int   			afstLinks, afstRechts;

// Aanraakschakelaars als Button
BUTTON			schakLinksBinnen(	SCHAK_LINKS_BINNEN_PIN), 
				schakLinksBuiten(	SCHAK_LINKS_BUITEN_PIN), 
				schakRechtsBinnen(	SCHAK_RECHTS_BINNEN_PIN), 
				schakRechtsBuiten(	SCHAK_RECHTS_BUITEN_PIN);

// Callback functies voor sonar timers
bool	beurtLinks;

void MeetAfstand()
{
	if (beurtLinks) 
	{
		afstLinks = SonarLinks.Meet();
		if (afstLinks < 20 ) Led1SonarLinks.Aan(); else Led1SonarLinks.Uit();
		if (afstLinks < 10 ) Led2SonarLinks.Aan(); else Led2SonarLinks.Uit();
		DBZEGLN2( "afstLinks:", afstLinks);
		beurtLinks = false;
	}
	else
	{
		afstRechts = SonarRechts.Meet();
		if (afstRechts < 20 ) Led1SonarRechts.Aan(); else Led1SonarRechts.Uit();
		if (afstRechts < 10 ) Led2SonarRechts.Aan(); else Led2SonarRechts.Uit();
		DBZEGLN2( "afstRechts:", afstRechts);
		beurtLinks = true;
	}
};

// Timer voor de twee sonars, ze meten om de beurt
TimerObject		SonarTimer(300, &MeetAfstand);		// milliseconden

//--------------------------------
GEM_LOOPSTATUS Statusbepaling( char* teken)
{
	GEM_LOOPSTATUS	loopStatus;
	if 		(inputC.Kijk() 	and inputD.Kijk())	{ loopStatus = ZITTEN; 		*teken = 'Z';}
	else if (!inputC.Kijk() and inputD.Kijk())	{ loopStatus = ACHTERUIT; 	*teken = 'A';}
	else if (inputC.Kijk() 	and !inputD.Kijk())	{ loopStatus = VOORUIT; 	*teken = 'V';}
	else if (!inputC.Kijk() and !inputD.Kijk())	{ loopStatus = STAAN; 		*teken = 'S';}
	else { loopStatus = UNDEFINED; *teken = 'U';};
	
	return loopStatus;
}

//-----------------------------------------------------
void setup() 
{
	DBINIT;
	
	lcd.init();                    // initialize the lcd 
    lcd.backlight();
    lcd.clear();

	stuurbeest.Init();
	stuurbeest.Rechtdoor();
	
	SonarTimer.Start();
};
//------------------------------------------------------------
void loop() 
{
	char			statusTeken;
	
	loopstatus = Statusbepaling( &statusTeken);
		
	SonarTimer.Update();	// meet afstanden
	
	if ((afstLinks < 20) && (afstRechts > 20) && (richtingstatus != RECHTS))
	{
		stuurbeest.ZetDoelRechtsaf();
		richtingstatus = RECHTS;
	}
	else if ((afstLinks > 20) && (afstRechts < 20) && (richtingstatus != LINKS))
	{
		stuurbeest.ZetDoelLinksaf();
		richtingstatus = LINKS;
	}
	else if ((afstLinks > 20) && (afstRechts > 20) && (richtingstatus != MIDDEN))
	{
		stuurbeest.ZetDoelRechtdoor();
		richtingstatus = MIDDEN;
	}
	stuurbeest.Trigger();
	
	snprintf( regel0, sizeof(regel0), "<<<<<<-%1d->>>>>>", richtingstatus);  
	snprintf( regel1, sizeof(regel1), "%1d%1d-%3d-%c-%3d-%1d%1d", 
									schakLinksBinnen.Kijk(),
									schakLinksBuiten.Kijk(),
									afstLinks, 
									statusTeken,
									schakRechtsBinnen.Kijk(),
									schakRechtsBuiten.Kijk(),
									afstRechts);
									
	if (	strncmp( regel0, regel0_prev, LCD_LENGTE) ||
			strncmp( regel1, regel1_prev, LCD_LENGTE) )
	{
		lcd.clear();
		lcd.setCursor(0,0); lcd.print(regel0);  
		lcd.setCursor(0,1); lcd.print(regel1);  
		strncpy( regel0_prev, regel0, LCD_LENGTE);
		strncpy( regel1_prev, regel1, LCD_LENGTE);
	};

	delay(LOOP_DELAY);
};
