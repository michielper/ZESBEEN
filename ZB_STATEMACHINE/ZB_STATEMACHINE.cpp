//	ZB_STATEMACHINE.cpp
//	
//	2021-03-21	Initieel
//
#include <ZB_STATEMACHINE.h>

//------------------------------------------------------------------
ZB_STATEMACHINE::ZB_STATEMACHINE()
{
	ClearTimer();
}
//------------------------------------------------------------------
void		ZB_STATEMACHINE::SetTimer( unsigned long ms)
{
	timerSet = true;
	toTime   = now() + ms;
}
//------------------------------------------------------------------
void		ZB_STATEMACHINE::ClearTimer()
{
	timerSet = false;
}
//------------------------------------------------------------------
ZB_EVENT	ZB_STATEMACHINE::BepaalEvent( 	int 	afstLinks, int afstRechts, 
											bool 	KL1, bool KL2, bool KL3, bool KL4)
{
	if ( KL1 || KL2 || KL3 || KL4 )										return EV_K;
	if ( (afstLinks < ZB_AFST_STUUR) && (afstRechts < ZB_AFST_STUUR) )	return EV_OM;
	if ( (afstLinks < ZB_AFST_STUUR) && (afstRechts > ZB_AFST_STUUR) )	return EV_OL;
	if ( (afstLinks > ZB_AFST_STUUR) && (afstRechts < ZB_AFST_STUUR) )	return EV_OR;
	if ( timerSet && (now() > toTime)) 									return EV_TO;
}
//------------------------------------------------------------------
void		ZB_STATEMACHINE::Transitie( 	ZB_EVENT		inEv,
											ZB_STUURSTATUS 	inSs,
											ZB_LOOPSTATUS	inLs,
											ZB_STUURSTATUS*	uitSs,
											ZB_LOOPSTATUS*	uitLs)
{
	if ( inLs == LS_S ) return;		// Geen actie bij stilstand
	
	switch (inEv)
	{		
		case	EV_OM:
		case	EV_K:
			*uitLs = LS_A;
			*uitSs = SS_M;	 		// Loop recht naar achteren
			SetTimer(TO_TIJD_LS);	// gedurende TO_TIJD_LS
			break;
		case	EV_OL:
			*uitLs = LS_V;
			*uitSs = SS_R;	 		// Stuur naar rechts
			SetTimer(TO_TIJD_SS);	// gedurende TO_TIJD_SS
			break;
		case	EV_OR:
			*uitLs = LS_V;
			*uitSs = SS_L;	 		// Stuur naar links
			SetTimer(TO_TIJD_SS);	// gedurende TO_TIJD_SS
			break;
		case	EV_TO: 				// Bij timeout altijd recht vooruit lopen
			*uitLs = LS_V;
			*uitSs = SS_M;	 		// Loop recht naar voren
			ClearTimer();
			break;
	}
}
