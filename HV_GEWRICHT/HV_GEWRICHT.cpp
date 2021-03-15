//----------------------------------------------------------
//	HV_GEWRICHT.cpp
//
//	Aansturing van servo met Doel en Trigger
//
//	2020-12-08	Initieel, vanuit HV_GEWRICHT.cpp
//  2020-12-11	Met ruststand ongelijk aan grdCorIn
//  2021-01-02	Met naam in constructor
//	2021-01-10	Nieuwe aanpak van ZetDoel/Trigger/GaAbs
//-----------------------------------------------------------

// #define DEBUG

#include "Arduino.h"
#include <Servo.h>
#include <MP_DEBUG.h>
#include <HV_GEWRICHT.h>

//----------------------------------------------------------------------------
// GV_Gewricht wordt aangestuurd met waarden tussen GRD_MIN en GRD_MAX (0-180)
//----------------------------------------------------------------------------
GEWRICHT::GEWRICHT( 					// Zet variabelen
				char* naam,				// Naam van het gewricht
				int pinIn, 				// Hardware positie
				int grdCorIn) 		// Correctie: opgeteld bij aanstuurwaarde
{
	strcpy( GWnaam, naam);
	GWpin 		= 	pinIn;
	GWgrdCor 	= 	grdCorIn;	// Correctie, wordt opgeteld bij input waarde
	GWgrdMin	=	0;			// Minimum fysieke input
	GWgrdMax	= 	180;		// Maximum fysieke input
	
	GWgrdRst	=	GRD_MID;
	GWgrdLaatst =   GRD_MID;
	
} // GEWRICHT
//------------------------------------------------------
void GEWRICHT::Init() 
{
	DBENTER2( "GEWRICHT::Init ", GWnaam);
		
	GWspier.attach(GWpin);
	
	DBLEAVE2( "GEWRICHT::Init ", GWnaam);
	
} // Init

//--------------------------------------------------    
//  Ga naar absolute stand in graden
//	corrigeer voor afwijking
//  Retourneer de werkelijk geschreven waarde
//--------------------------------------------------
int 	GEWRICHT::GaAbs( int grdIn)
{
	DBENTER3( "GEWRICHT::GaAbs", GWnaam, grdIn);
			
		grdIn = constrain( grdIn, GWgrdMin, GWgrdMax);	
		GWspier.write( grdIn + GWgrdCor);
		GWgrdLaatst 	= grdIn;
	
	DBLEAVE3( "GEWRICHT::GaAbs", GWnaam, grdIn);
	
	return GWspier.read();
	
} // GaAbs
//--------------------------------------------------
int 	GEWRICHT::GaRel( int grdIn)
{
	return GaAbs( GWgrdLaatst + grdIn );
}
//--------------------------------------------------
// 		doel:	doel in graden (0-180)
//		s	:	factor waarmee elke tik wordt vemenigvuldigd voor optelling in GW_accu
//		t	:	factor waar GWtikAcccu door wordt gedeeld om de nieuwe positie te bepalen
//		n	:	gewenst aantal tikken om doel te bereiken
//					als n == 0 is de snelheid constant
//					als n != 0 is het aantal tikken constant
//--------------------------------------------------
void  GEWRICHT::ZetDoel( int doel, int s, int t, int n)
{
	DBENTER5(GWnaam, "ZetDoel: ", doel, s, t);
	
	doel = constrain( doel, GWgrdMin, GWgrdMax);
	
	GWgrdBegin 		= GWgrdLaatst;		// begin bij laatste positie
	GWgrdDoel		= doel;				// nieuw doel in graden
	
	DBZEGLN3(GWnaam, "GWgrdDoel: ", GWgrdDoel);
	DBZEGLN3(GWnaam, "GWgrdBegin: ", GWgrdBegin);
	
	GW_versnelling	= s;	// snelheid (graden per tik)
	GW_vertraging	= t;	// factor waarmee accu doel wordt vermenigvuldigd, wordt berekend als n > 0
	GW_tikken		= n;
	
	if ( n > 0 )
	{
		GW_vertraging   = GW_tikken * GW_versnelling / abs(GWgrdDoel - GWgrdBegin);
		
		if (GW_vertraging == 0 )
		{
			GW_vertraging = 1;
		}
	}

	GWtikTegaan		= GW_vertraging * abs(GWgrdDoel - GWgrdBegin) / GW_versnelling;	// aantal tikken te gaan
	
 	GW_accu			= 0;				// neemt toe met GW_versnelling per tik
	
 	if (GWgrdDoel >= GWgrdBegin)
 	{
		GW_vooruit	= 1;
	}
	else
	{
		GW_vooruit	= -1;
	}
  	
	DBLEAVE1( "ZetDoel");
}
//--------------------------------------------------
// Trigger geeft true als doel bereikt is
// Twee variabelen bepalen de snelheid:
//		GW_versnelling	:	factor waarmee elke tik wordt vemenigvuldigd voor optelling in GW_accu
//		GW_vertraging	:	factor waar GWtikAcccu door wordt gedeeld om de nieuwe positie te bepalen
//--------------------------------------------------
bool GEWRICHT::Trigger()
{
	DBENTER2(	GWnaam, "Trigger ");

	bool result = false;		// Geeft true als doel bereikt is
	int grdNieuw;
	
	if (GWgrdDoel == GWgrdBegin) 	return true;
	if (GWgrdDoel == GWgrdLaatst)	return true;
		
	if ( --GWtikTegaan <= 0 )
	{
		grdNieuw 	= GWgrdDoel;
		result 		= true;		// doel bereikt
	}
	else
	{	
		GW_accu		+=  GW_versnelling;	// accumuleer tikken, altijd positief
		grdNieuw	= 	GWgrdBegin + GW_vooruit * GW_accu / GW_vertraging;
	}

	GaAbs( grdNieuw);
	
	DBLEAVE3( GWnaam, "Trigger", result);	
		
	return result;
} // end Stap

//----------------------------------------------
int		GEWRICHT::Write( int grd)			// schrijf waarde direct in servo
{
	grd = constrain(grd, 0, 180);
	GWspier.write( grd );
	return grd;
};
//----------------------------------------------
int		GEWRICHT::Read()			// lees laatst geschreven waarde 
{
	return GWspier.read();
};


