// HV_BEEN.cpp
//
//	2020-12-08	Initieel
//	2020-12-15	Nieuwe waarden in loop array, eerste test
//  2020-12-18	Nieuwe waarden in loop array. Benen moeten meer rechtop (en korter!)
//  2021-01-03  loop array met factor, debug regels in verschillende functies
//  2021-03-23	GITHUB TEST
//-------------------------------------------------------

// #define DEBUG

#include "Arduino.h"
#include <Servo.h>
#include <MP_DEBUG.h>
#include <HV_GEWRICHT.h>
#include <HV_BEEN.h>


//-------------------------------------------------------
BEEN::BEEN(	char* naam, int num, GEWRICHT* dij, GEWRICHT* knie)
{
	strcpy( mijnnaam, naam);
	BN_nummer 		= 	num;
	mijnKnie 		= 	knie;
	mijnDij			= 	dij;
	BN_beweeg_index		= 0;
	BN_beweeg_index		= 0;
	
	// dij beweegt in vier fasen naar achteren
	// knie staat eerst omhoog dan naar beneden
	//												DIJ						KNIE
	loop_array[5] 	=	(STAPFASE)	{ (GWDOEL) {125, NSNL,NTRG, NTIK} 	, (GWDOEL) {80,NSNL,NTRG, NTIK} };	// neer
	loop_array[0] 	=	(STAPFASE)	{ (GWDOEL) {132, NSNL,NTRG, NTIK} 	, (GWDOEL) {80,NSNL,NTRG, NTIK} };	// neer
	loop_array[1] 	=	(STAPFASE)	{ (GWDOEL) {140, NSNL,NTRG, NTIK} 	, (GWDOEL) {87,NSNL,NTRG, NTIK} };	// neer
	loop_array[2] 	=	(STAPFASE)	{ (GWDOEL) {180, NSNL,NTRG, NTIK} 	, (GWDOEL) {30,NSNL,NTRG, NTIK} };	// op
	loop_array[3] 	=	(STAPFASE)	{ (GWDOEL) {120, NSNL,NTRG, NTIK} 	, (GWDOEL) {30, NSNL,NTRG, NTIK} };	// op
	loop_array[4] 	=	(STAPFASE)	{ (GWDOEL) {110, NSNL,NTRG, NTIK} 	, (GWDOEL) {85, NSNL,NTRG, NTIK} };	// neer
	
	//--------------------------------------------------------------
	opstaan_array[0] =	(STAPFASE)	{ (GWDOEL) {90,NSNL,NTRG, NTIK} 	, (GWDOEL) {90,NSNL,NTRG, NTIK} };
	opstaan_array[1] =	(STAPFASE)	{ (GWDOEL) {90,NSNL,NTRG, NTIK} 	, (GWDOEL) {90,NSNL,NTRG, NTIK} };
	opstaan_array[2] =	(STAPFASE)	{ (GWDOEL) {90,NSNL,NTRG, NTIK} 	, (GWDOEL) {90,NSNL,NTRG, NTIK} };

};
//-------------------------------------------------------
void	BEEN::Init()
{
	DBENTER2("BEEN::Init", mijnnaam);
		mijnDij->Init();
		mijnKnie->Init();
		
		DirectStaan();
		
	DBLEAVE2("BEEN::Init", mijnnaam);	
};
//-------------------------------------------------------
// Begin met lopen bij start_index
//--------------------------------------------------------
void	BEEN::StartLopen(int start_index)
{
	BN_beweeg_index = start_index;
	
	mijnDij->ZetDoel( 	loop_array[BN_beweeg_index].dij.doel, 
						loop_array[BN_beweeg_index].dij.versn, 
						loop_array[BN_beweeg_index].dij.vertr);
	
	mijnKnie->ZetDoel( 	loop_array[BN_beweeg_index].knie.doel, 
	                    loop_array[BN_beweeg_index].knie.versn, 
	                    loop_array[BN_beweeg_index].knie.vertr);
};
//---------------------------------------------------------
void	BEEN::StartOpstaan()
{
	BN_beweeg_index = 0;
	
	mijnDij->ZetDoel( 	opstaan_array[BN_beweeg_index].dij.doel, 
						opstaan_array[BN_beweeg_index].dij.versn, 
						opstaan_array[BN_beweeg_index].dij.vertr);
	
	mijnKnie->ZetDoel( 	opstaan_array[BN_beweeg_index].knie.doel, 
	                    opstaan_array[BN_beweeg_index].knie.versn, 
	                    opstaan_array[BN_beweeg_index].knie.vertr);
};
//---------------------------------------------------------
void    BEEN::GaNaarStap( int n )	// ga direct naar stap n in de loopcyclus
{
	mijnDij->GaAbs(  loop_array[n].dij.doel );
	mijnKnie->GaAbs( loop_array[n].knie.doel );
}
//-------------------------------------------------------
void	BEEN::ZetDoelRust()
{
	DBENTER2( "BEEN::ZetDoelRust" , mijnnaam);
		// toestand = RUSTEND;
		mijnDij->ZetDoel(BN_DIJ_RUST, 1, 10, 1000);
		mijnKnie->ZetDoel(BN_KNIE_RUST, 1, 10, 1000);
	DBLEAVE2( "BEEN::ZetDoelRust" , mijnnaam);
};
//-------------------------------------------------------
void	BEEN::DirectRust()
{
	DBENTER2( "BEEN::DirectRust" , mijnnaam);
		toestand = RUSTEND;
		mijnDij->GaAbs(BN_DIJ_RUST);
		mijnKnie->GaAbs(BN_KNIE_RUST);
	DBLEAVE2( "BEEN::DirectRust" , mijnnaam);
};
//-------------------------------------------------------
void	BEEN::ZetDoelStaan()
{
	DBENTER2( "BEEN::ZetDoelStaan" , mijnnaam);
		mijnDij->ZetDoel(BN_DIJ_STAAN, 1, 10, 1000);
		mijnKnie->ZetDoel(BN_KNIE_STAAN, 1, 10, 1000);
	DBLEAVE2( "BEEN::ZetDoelStaan" , mijnnaam);
};
//-------------------------------------------------------
void	BEEN::DirectStaan()
{
	DBENTER2( "BEEN::DirectStaan" , mijnnaam);
		toestand = STAAND;
		mijnDij->GaAbs(BN_DIJ_STAAN);
		mijnKnie->GaAbs(BN_KNIE_STAAN);
	DBLEAVE2( "BEEN::DirectStaan" , mijnnaam);
};
//-------------------------------------------------------
bool	BEEN::TriggerVooruit()
{
	bool	res_d	=	mijnDij->Trigger();
	bool	res_k	=	mijnKnie->Trigger();
	
	if (res_d && res_k)
	{	// fase voltooid, neem volgende fase
		BN_beweeg_index = (++BN_beweeg_index % BN_AANT_LOOPFASEN);
		
		mijnDij->ZetDoel( 	loop_array[BN_beweeg_index].dij.doel, 
							loop_array[BN_beweeg_index].dij.versn, 
							loop_array[BN_beweeg_index].dij.vertr,
							loop_array[BN_beweeg_index].dij.tikken);
		
		mijnKnie->ZetDoel( 	loop_array[BN_beweeg_index].knie.doel, 
							loop_array[BN_beweeg_index].knie.versn, 
							loop_array[BN_beweeg_index].knie.vertr,
							loop_array[BN_beweeg_index].knie.tikken);
	                    
//		PR6( "index: ", index, " doel Dij: ", mijnDij->GWgrdDoel, "Knie: ", mijnKnie->GWgrdDoel);
		
		return (0 == BN_beweeg_index); // signaal als cyclus is afgelopen
	}				
};
//-------------------------------------------------------
bool	BEEN::TriggerAchteruit()
{
	bool	res_d	=	mijnDij->Trigger();
	bool	res_k	=	mijnKnie->Trigger();
	
	if (res_d && res_k)
	{	// fase voltooid, neem volgende fase
		if (--BN_beweeg_index < 0 )
		{
			BN_beweeg_index = BN_AANT_LOOPFASEN - 1;
		}
		
		mijnDij->ZetDoel( 	loop_array[BN_beweeg_index].dij.doel, 
							loop_array[BN_beweeg_index].dij.versn, 
							loop_array[BN_beweeg_index].dij.vertr,
							loop_array[BN_beweeg_index].dij.tikken);
		
		mijnKnie->ZetDoel( 	loop_array[BN_beweeg_index].knie.doel, 
							loop_array[BN_beweeg_index].knie.versn, 
							loop_array[BN_beweeg_index].knie.vertr,
							loop_array[BN_beweeg_index].knie.tikken);
	                    
//		PR6( "index: ", index, " doel Dij: ", mijnDij->GWgrdDoel, "Knie: ", mijnKnie->GWgrdDoel);
		
		return (0 == BN_beweeg_index); // signaal als cyclus is afgelopen
	}				
};
//-------------------------------------------------------------
void	BEEN::ZetDoelStap( int n)	// zet doel op stap n in de loopcyclus
{
	if (n >= BN_AANT_LOOPFASEN) 
	{ 
		n = BN_AANT_LOOPFASEN - 1 ;
	}
	else if ( n < 0 ) 
	{ 
		n = 0 ;
	}
	
	mijnDij->ZetDoel( 	loop_array[n].dij.doel, 
						loop_array[n].dij.versn, 
						loop_array[n].dij.vertr,
						loop_array[n].dij.tikken);
	
	mijnKnie->ZetDoel( 	loop_array[n].knie.doel, 
						loop_array[n].knie.versn, 
						loop_array[n].knie.vertr,
						loop_array[n].knie.tikken);
};
//------------------------------------------------------------------
bool	BEEN::TriggerOpstaan()
{
	bool	res_d	=	mijnDij->Trigger();
	bool	res_k	=	mijnKnie->Trigger();
	
	if (res_d && res_k)
	{	// fase voltooid, neem volgende fase
		BN_beweeg_index++;
		
		mijnDij->ZetDoel( 	opstaan_array[BN_beweeg_index].dij.doel, 
							opstaan_array[BN_beweeg_index].dij.versn, 
							opstaan_array[BN_beweeg_index].dij.vertr,
							opstaan_array[BN_beweeg_index].dij.tikken);
		
		mijnKnie->ZetDoel( 	opstaan_array[BN_beweeg_index].knie.doel, 
							opstaan_array[BN_beweeg_index].knie.versn, 
							opstaan_array[BN_beweeg_index].knie.vertr,
							opstaan_array[BN_beweeg_index].knie.tikken);
	}	
	return (BN_AANT_OPSTAFASEN == BN_beweeg_index); 		
};

//-------------------------------------------------------
bool	BEEN::Trigger()  // resultaat true als doel voor dij en knie bereikt
{
	bool 	b1, b2;
	
	b1	=	mijnDij->Trigger();
	b2	=	mijnKnie->Trigger();

	return (b1 && b2);
};
//-------------------------------------------------------
bool	BEEN::TriggerKnieEerst()
{
	if (mijnKnie->Trigger())
	{
		return mijnDij->Trigger();
	}
}
//-------------------------------------------------------
bool	BEEN::TriggerDijEerst()
{
	if (mijnDij->Trigger())
	{
		return mijnKnie->Trigger();
	}
}


