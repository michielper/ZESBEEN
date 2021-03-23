// HV_sensoren_en_display
// 2021-01-26  	initieel, 4x 4-digit display voor voedingspanningen
// 2021-02-05  	pins voor displays veranderd, LED arrays toegevoegd
// 2021-02-05  	sonar toegevoegd
// 2021-02-18	LCD display ipv 4-digit displays
// 2021-02-18	gebruik snprintf voor LCD display
// 2021-02-16	voeg debug toe
// 2021-03-17	reorganisatie van loop met extra functies
//-------------------------------------------------

#define DEBUG

#include "Arduino.h"
//~ #include <TM1637Display.h>
#include <LiquidCrystal_I2C.h>
#include <MP_LCD.h>
#include <Servo.h>
#include <MP_DEBUG.h>
#include <MP_LED.h>
#include <MP_BUTTON.h>
#include <ZB_SEN_CONFIG.h>
#include <ZB_STATEMACHINE.h>
#include <MP_SONAR.h>
#include <MP_LICHTMETER.h>
#include <HV_GEWRICHT.h>
#include <ZB_STUURBEEST.h>
#include <MP_INPUTKANAAL.h>
#include <TimerObject.h>
		

#define LOOP_DELAY              30      // ms
#define	SONAR_MEETVERTRAGING	300

// Actuatoren (schouders)
GEWRICHT        	rvs("RVS", RVS_PIN, RVS_COR); 
GEWRICHT        	ras("RAS", RAS_PIN, RAS_COR);  
GEWRICHT        	lvs("LVS", LVS_PIN, LVS_COR);  
GEWRICHT        	las("LAS", LAS_PIN, LAS_COR);

STUURBEEST  		stuurbeest( &rvs, &ras, &lvs, &las );

// LCD display
LiquidCrystal_I2C 	lcd(0x27,16,2);  		// set the LCD address for a 16 chars and 2 line display
LCDISPLAY			display( &lcd );

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
BUTTON			KL0(	SCHAK_LINKS_BINNEN_PIN), 
				KL1(	SCHAK_LINKS_BUITEN_PIN), 
				KR0(	SCHAK_RECHTS_BINNEN_PIN), 
				KR1(	SCHAK_RECHTS_BUITEN_PIN);

// Callback functies voor sonar timers
bool		beurtLinks;
//--------------------------------------------------------------------------
void MeetSonar()
{
	if (beurtLinks) 
	{
		afstLinks = SonarLinks.Meet();		
			if (afstLinks < ZB_AFST_STUUR ) Led1SonarLinks.Aan(); else Led1SonarLinks.Uit();
			if (afstLinks < ZB_AFST_LANG ) Led2SonarLinks.Aan(); else Led2SonarLinks.Uit();
		DBZEGLN2( "afstLinks:", afstLinks);
		beurtLinks = false;
	}
	else
	{
		afstRechts = SonarRechts.Meet();
			if (afstRechts < ZB_AFST_STUUR ) Led1SonarRechts.Aan(); else Led1SonarRechts.Uit();
			if (afstRechts < ZB_AFST_LANG ) Led2SonarRechts.Aan(); else Led2SonarRechts.Uit();
		DBZEGLN2( "afstRechts:", afstRechts);
		beurtLinks = true;
	}
};

// Timer voor de twee sonars, ze meten om de beurt, niet vaker dan SONAR_MEETVERTRAGING
TimerObject		SonarTimer(SONAR_MEETVERTRAGING, &MeetSonar);		// milliseconden

//--------------------------------
//	Verkrijg de huidigeLoopStatus van Controller 1
//--------------------------------
ZB_LOOPSTATUS lees_C1_status( char* teken)
{
	GEM_LOOPSTATUS	result;
	if 		(inputC.Kijk() 	and inputD.Kijk())	{ result = LS_Z; 	*teken = 'Z';}
	else if (!inputC.Kijk() and inputD.Kijk())	{ result = LS_A; 	*teken = 'A';}
	else if (inputC.Kijk() 	and !inputD.Kijk())	{ result = LS_V; 	*teken = 'V';}
	else if (!inputC.Kijk() and !inputD.Kijk())	{ result = LS_S; 	*teken = 'S';}
	else { result = LS_NUL; *teken = 'U';};
		
	return result;
};

//--------------------------------
ZB_EVENT			huidigEvent;
ZB_STATEMACHINE		StateMachine;

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
	char	statusTeken;
	
	// Verkrijg status van Controller 1
	ZB_LOOPSTATUS	huidigeLoopStatus 	= 	lees_C1_status( &statusTeken);
	ZB_STUURSTATUS	huidigeStuurStatus	=	stuurbeest.status;	
	
	ZB_LOOPSTATUS	nieuweLoopStatus 	= 	huidigeLoopStatus;
	ZB_STUURSTATUS	nieuweStuurStatus 	= 	huidigeStuurStatus;
	
	// bepaal eventueel te behandelen event
	huidigEvent = StateMachine.BepaalEvent(	afstLinks, afstRechts,
											KL0.Kijk(),
											KL1.Kijk(),
											KR0.Kijk(),
											KR1.Kijk());
		
	if (huidigEvent != EV_NUL)
	{	
		// voer transitiefunctie uit en update status
		StateMachine.Transitie( huidigEvent, huidigeStuurStatus, huidigeLoopStatus,
											&nieuweStuurStatus, &nieuweLoopStatus);
		stuurbeest.ZetDoelStatus( nieuweStuurStatus);
		// Zend loopstatus naar C1
	}
		
	stuurbeest.Trigger();
	
	// schrijf naar buffers in display object
	snprintf( display.reg0, sizeof(display.reg0), "<<<<<<-%1d->>>>>>", richtingstatus);  
	snprintf( display.reg1, sizeof(display.reg1), "%1d%1d-%3d-%c-%3d-%1d%1d", 
		KL0.Kijk(),	KL1.Kijk(),	afstLinks, statusTeken,	afstRechts,	KR1.Kijk(),	KR0.Kijk()	);
	display.toon();
									
	delay(LOOP_DELAY);
};
