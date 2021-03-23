// HV_MAIN02.cpp
//
// Wijzigingen:
// 2020/12/07	initial
// 2021/01/28	wijzigingen voor zes poten

//---------------------------------------------

// #define 	DEBUG

#include "Arduino.h"
#include <Servo.h>
#include <MP_DEBUG.h>
// #include <MP_DEBUG.h>
#include <MP_BUTTON.h>
#include <MP_LED.h>
#include <HV_GEWRICHT.h>
#include <HV_BEEN.h>
#include <ZB_ACT_CONFIG.h>
#include <ZB_STATEMACHINE.h>	// gemeenschappelijk
#include <HV_BEEST.h>
#include <MP_INPUTKANAAL.h>

#define	LOOPDELAY 1

// Actuatoren (dij, knie )
GEWRICHT        rvd("RVD", RVD_PIN, RVD_COR); 
GEWRICHT        rvk("RVK", RVK_PIN, RVK_COR);
          
GEWRICHT        rmd("RMD", RMD_PIN, RMD_COR);  
GEWRICHT        rmk("RMK", RMK_PIN, RMK_COR); 
  
GEWRICHT        rad("RAD", RAD_PIN, RAD_COR);  
GEWRICHT        rak("RAK", RAK_PIN, RAK_COR);   
          
GEWRICHT        lvd("LVD", LVD_PIN, LVD_COR);  
GEWRICHT        lvk("LVK", LVK_PIN, LVK_COR);

GEWRICHT        lmd("LMD", LMD_PIN, LMD_COR);  
GEWRICHT        lmk("LMK", LMK_PIN, LMK_COR);
   
GEWRICHT        lad("LAD", LAD_PIN, LAD_COR);  
GEWRICHT        lak("LAK", LAK_PIN, LAK_COR);
   

GEWRICHT*       LoopGewrichten[AANTAL_LOOPGEWRICHTEN] = 
                  {&rvd,&rvk,&rad,&rak,&lvd,&lvk,&lad,&lak};
                  
BEEN    been_rv("RV", 0, &rvd, &rvk);                
BEEN	been_rm("RM", 1, &rmd, &rmk);        
BEEN    been_ra("RA", 2, &rad, &rak);  

BEEN    been_lv("LV", 3, &lvd, &lvk);
BEEN	been_lm("LM", 4, &lmd, &lmk);   
BEEN    been_la("LA", 5, &lad, &lak);                
             
BEEN*           benen[BST_AANT_BENEN] =
				{
					&been_rv,
					&been_rm,
					&been_ra, 
					&been_lv, 
					&been_lm,
					&been_la
				};
//----------------------------------------------------------------				
BEEST		Houtvoetje( &been_rv, &been_rm, &been_ra, &been_lv, &been_lm, &been_la );
//----------------------------------------------------------------	

LED		LedAchteruit(	LED_ACHT_PIN), 
		LedZit(			LED_ZIT_PIN), 
		LedSta(			LED_STA_PIN), 
		LedLoop(		LED_LOOP_PIN);
		
BUTTON		ButtonAchteruit(	BUTTON_ACHT_PIN), 
			ButtonZit(			BUTTON_ZIT_PIN), 
			ButtonSta(			BUTTON_STA_PIN), 
			ButtonVooruit(			BUTTON_LOOP_PIN);
			
INPUTKANAAL	kanaalA(C1_KANAAL_A_PIN),
			kanaalB(C1_KANAAL_B_PIN);

void AllesUIt()
{
	LedAchteruit.Uit();
	LedZit.Uit();
	LedSta.Uit();
	LedLoop.Uit();
};					

// Variabelen
int         	i = 0;
int         	tiknr         	= 0;    // cyclische tikkenteller
int         	golfval       	= 0;    // golf op IN_MIN tot IN_MAX
int				loopzijde		= 0;	// 0..1

GEM_STATUS		loopstatus;
GEM_RICHTINGSTATUS	richtingstatus;

// toestandsvariabelen
int   index = 0;

int 		stappenteller		= 0;
bool		achteruitlopend 	= false;
int			achteruitstappen 	= 0;
int			prev_rv_index		= 0;

int			afstLinks; 	
int			afstRechts;
int			afstand_midden;	

// Callback functies
// Trigger functie pointer
bool 		(*fpTrigger)();
// Klaar-functie pointer, aangeroepen als fpTrigger true retourneert
void		(*fpKlaar)();

//---- callback functies ---------------------------------------
//	rechtstreeks gebruiken van de methods lijkt niet te werken
bool  	hv_trigger() 			{ 	return Houtvoetje.Trigger();			}
bool	hv_trigger_vooruit()	{	return Houtvoetje.TriggerVooruit();		}
bool	hv_trigger_achteruit()  {   return Houtvoetje.TriggerAchteruit(); 	}
//-------------------------------------------
void	hv_power_off()			{   Houtvoetje.PowerOff(); }
void	hv_power_on()			{   Houtvoetje.PowerOn();  }
void	hv_nop()				{ }
void	hv_ga_vooruit()			{   fpTrigger = &hv_trigger_vooruit; fpKlaar = &hv_nop; }
void	hv_ga_achteruit()		{   fpTrigger = &hv_trigger_achteruit; fpKlaar = &hv_nop; }
	
//-----------------------------------------
void zendStatus( GEM_STATUS loopStatus)
{
	int	c,d;
	switch (loopStatus)
	{
		case VOORUIT:	c = HIGH; 	d = LOW;	break;
		case ACHTERUIT:	c = LOW; 	d = HIGH;	break;
		case STAAN:		c = HIGH; 	d = HIGH;	break;
		case ZITTEN:	c = LOW; 	d = LOW;	break;
		default:
		c = LOW; 	d = LOW;
	}
	digitalWrite(C1_KANAAL_C_PIN, c);
	digitalWrite(C1_KANAAL_D_PIN, d);
}
//------------------------------------------------------
int	behandelinput()
{
	// DBZEGLN1( "Behandelinput");
	
	if (ButtonAchteruit.Ingedrukt() or ( kanaalA.Opgaand() ) )
	{
		Houtvoetje.PowerOn();
		AllesUIt();
		DBZEGLN1( "ButtonAchteruit.Ingedrukt" );
		LedAchteruit.Aan();
		Houtvoetje.ZetDoelStaan();
		fpTrigger 	= &hv_trigger;
		fpKlaar		= &hv_ga_achteruit;
		zendStatus( ACHTERUIT );
	}
	
	if (ButtonZit.Ingedrukt())
	{
		Houtvoetje.PowerOn();
		AllesUIt();
		DBZEGLN1( "ButtonZit.Ingedrukt" );
		LedZit.Aan();
		Houtvoetje.ZetDoelRust(10);
		fpTrigger 	= &hv_trigger;
		fpKlaar 	= &hv_power_off;
		zendStatus( ZITTEN);
	}	
	
	if (ButtonSta.Ingedrukt() )
	{
		Houtvoetje.PowerOn();
		AllesUIt();
		DBZEGLN1( "ButtonSta.Ingedrukt" );
		LedSta.Aan();
		Houtvoetje.ZetDoelStaan();
		fpTrigger 	= &hv_trigger;
		fpKlaar 	= &hv_nop;			// geen power off als ie staat.....
		zendStatus( STAAN);
	}	
	
	if (ButtonVooruit.Ingedrukt())
	{
		Houtvoetje.PowerOn();
		AllesUIt();
		DBZEGLN1( "ButtonVooruit.Ingedrukt" );
		LedLoop.Aan();		
		Houtvoetje.ZetDoelStaan();
		fpTrigger 	= &hv_trigger;
		fpKlaar		= &hv_ga_vooruit;
		zendStatus( VOORUIT);
	}		
}
//-----------------------------------------
void setup() 
{
	DBINIT;
	
	Houtvoetje.Init();
	
	// kanalen om loopStatus door te geven
	pinMode( C1_KANAAL_C_PIN, OUTPUT);
	pinMode( C1_KANAAL_D_PIN, OUTPUT);
	digitalWrite( C1_KANAAL_C_PIN, LOW);
	digitalWrite( C1_KANAAL_D_PIN, LOW);
	
	Houtvoetje.DirectStaan();
	// Houtvoetje.ZetDoelStaan();	

	fpTrigger 	= &hv_trigger;
	fpKlaar 	= &hv_power_off;

} // setup
//----------------------------------------------

void loop()
{  
	bool rc;
	
	if(NULL != fpTrigger)
	{
		rc = fpTrigger();
		DBZEGLN2( "rc: ", rc);
		if (rc == 1 )
		{
			// klaar met proces/cyclus
			// roep klaar-callback aan
			// hv_power_off();
			
			DBZEGLN1("klaar");
			fpKlaar();	
		}		
	}; 
	
	behandelinput();
	
	delay(LOOPDELAY);
}
