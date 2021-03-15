// #define DEBUG

#include "Arduino.h"
#include <Servo.h>
#include <HOUTVOETJE_CONFIG.h>
#include <MP_DEBUG.h>
#include <HV_GEWRICHT.h>
#include <HV_BEEN.h>
#include <HV_BEEST.h>

// 2021-01-28	Aanpassingen voor zes benen

//-----------------------------------------------------------------------
BEEST::BEEST( BEEN* been_rv, BEEN* been_rm, BEEN* been_ra, BEEN* been_lv, BEEN* been_lm, BEEN* been_la)
{
	rv = been_rv; ra = been_ra; rm = been_rm; la = been_la; lv = been_lv; lm = been_lm;
	
	benen[0] = rv;
	benen[1] = rm;
	benen[2] = ra;
	
	benen[3] = lv;
	benen[4] = lm;
	benen[5] = la;
};
//-----------------------------------------------------------------------
void	BEEST::Init()
{
	DBENTER1( "BEEST::Init");
	
	pinMode(POWER_RELAIS_PIN,OUTPUT);
	PowerOn();
	
		for (int i = 0; i < BST_AANT_BENEN; i++)
		{
			benen[i]->Init();
		}
		
	DBLEAVE1( "BEEST::Init");
};
//---------------------------------------------------------
void	BEEST::PowerOn()
{
	digitalWrite(POWER_RELAIS_PIN,HIGH);
}
//--------------------------------------------------------
void	BEEST::PowerOff()
{
	digitalWrite(POWER_RELAIS_PIN,LOW);	
}
//-----------------------------------------------------------------------
void	BEEST::DirectRust()
{
	DBENTER1("BEEST::DirectRust");
		for (int i = 0; i < BST_AANT_BENEN; i++)
		{
			benen[i]->DirectRust();
		}
	DBLEAVE1("BEEST::DirectRust");
};
//-----------------------------------------------------------------------
void	BEEST::ZetDoelRust( int GWvaart)		// alle benen gevouwen
{
	DBENTER1("BEEST::ZetDoelRust");
		for (int i = 0; i < BST_AANT_BENEN; i++)
		{
			benen[i]->ZetDoelRust();
		}
	DBLEAVE1("BEEST::ZetDoelRust");
};
//-----------------------------------------------------------------------
void	BEEST::DirectStaan()
{
	DBENTER1("BEEST::DirectStaan");
		for (int i = 0; i < BST_AANT_BENEN; i++)
		{
			benen[i]->DirectStaan();
		}
	DBLEAVE1("BEEST::DirectStaan");
};
//-----------------------------------------------------------------------
void	BEEST::ZetDoelStaan( int GWvaart)		// alle benen gevouwen
{
	DBENTER1("BEEST::ZetDoelStaan");
		for (int i = 0; i < BST_AANT_BENEN; i++) 	
		{
			DBZEGLN2( "ZetDoelStaan, been# ", i);
			benen[i]->ZetDoelStaan();
		}
	DBLEAVE1("BEEST::ZetDoelStaan");
};
//-----------------------------------------------------------------------

// bool	TriggerStaan();		// trigger naar Staan
// bool	TriggerRust();		// trigger naar Rust


void	BEEST::StartLopen()
{
	rv->StartLopen(BN_LOOP_OFFSET);	
	lm->StartLopen(BN_LOOP_OFFSET);
	ra->StartLopen(BN_LOOP_OFFSET);
		
	lv->StartLopen(0);
	rm->StartLopen(0);
	la->StartLopen(0);
};

void	BEEST::StartOpstaan()
{
	rv->StartOpstaan();	
	rm->StartOpstaan();
	ra->StartOpstaan();	
	la->StartOpstaan();
	lm->StartOpstaan();
	lv->StartOpstaan();
};
//-------------------------------------------------------
bool	BEEST::Trigger()
{
	bool result = true;
	for (int i = 0; i < BST_AANT_BENEN; i++)
	{
		result 	= benen[i]->Trigger() && result;
	}
	return result;
};

//--------------------------------------------------------
bool	BEEST::TriggerVooruit()
{
	bool rc = true;
	for (int i = 0; i < BST_AANT_BENEN; i++)
	{
		rc 	= benen[i]->Trigger() && rc;
	}
	
	if (rc)		//  alle benen klaar met hun beweegfase
	{
		// naar volgende fase
		if ( ++BST_beweeg_index >= BN_AANT_LOOPFASEN )
		{
			BST_beweeg_index = 0;
		}
		
		rv->ZetDoelStap( (BST_beweeg_index + BN_LOOP_OFFSET) % BN_AANT_LOOPFASEN );
		lm->ZetDoelStap( (BST_beweeg_index + BN_LOOP_OFFSET) % BN_AANT_LOOPFASEN );
		ra->ZetDoelStap( (BST_beweeg_index + BN_LOOP_OFFSET) % BN_AANT_LOOPFASEN );
		
		lv->ZetDoelStap( BST_beweeg_index  );
		rm->ZetDoelStap( BST_beweeg_index  );		
		la->ZetDoelStap( BST_beweeg_index  );
	}
	return rc;
};
//--------------------------------------------------------
bool	BEEST::TriggerAchteruit()
{
	bool rc = true;
	for (int i = 0; i < BST_AANT_BENEN; i++)
	{
		rc 	= benen[i]->Trigger() && rc;
	}
	
	if (rc)		//  alle benen klaar met hun beweegfase
	{
		// naar volgende fase
		if ( --BST_beweeg_index < 0 )
		{
			BST_beweeg_index = BN_AANT_LOOPFASEN -1;
		}
		
		rv->ZetDoelStap( (BST_beweeg_index + BN_LOOP_OFFSET) % BN_AANT_LOOPFASEN );
		lm->ZetDoelStap( (BST_beweeg_index + BN_LOOP_OFFSET) % BN_AANT_LOOPFASEN );
		ra->ZetDoelStap( (BST_beweeg_index + BN_LOOP_OFFSET) % BN_AANT_LOOPFASEN );
		
		lv->ZetDoelStap( BST_beweeg_index  );
		rm->ZetDoelStap( BST_beweeg_index  );		
		la->ZetDoelStap( BST_beweeg_index  );
	}
	return rc;
};
//-----------------------------------------------------------------
void	BEEST::GaNaarStap( int n)
{
		rv->GaNaarStap( (n + BN_LOOP_OFFSET) % BN_AANT_LOOPFASEN );
		lm->GaNaarStap( (n + BN_LOOP_OFFSET) % BN_AANT_LOOPFASEN );
		ra->GaNaarStap( (n + BN_LOOP_OFFSET) % BN_AANT_LOOPFASEN );
		
		lv->GaNaarStap( n  );
		rm->GaNaarStap( n  );
		la->GaNaarStap( n  );
}

bool	BEEST::TriggerOpstaan()
{
	bool result = true;
	for (int i = 0; i < BST_AANT_BENEN; i++)
	{
		result 	= benen[i]->TriggerOpstaan() && result;
	}
	return result;
};



