//	ZB_STATEMACHINE.h
//	
//	2021-03-21	Initieel
//

#define	TO_TIJD_LS		2000	// achteruitlooptijd
#define TO_TIJD_SS		2000	// bocht-tijd

#define ZB_AFST_STUUR	20		// afstand waarbij gestuurd wordt
#define ZB_AFST_LANG	40		// grotere afstand

enum	ZB_STUURSTATUS	{ 	SS_NUL,	SS_M, SS_L, SS_R, SS_AANT };
enum	ZB_LOOPSTATUS	{	LS_NUL, LS_V, LS_A, LS_S, LS_Z, LS_AANT };
enum	ZB_EVENT		{	EV_NUL, EV_OM, EV_OL, EV_OR, EV_K, EV_TO, EV_AANT };

class	ZB_STATEMACHINE
{
	public:
		ZB_STATEMACHINE::ZB_STATEMACHINE();
		
		ZB_EVENT		BepaalEvent( 	int  afstLinks, int afstRechts, 
										bool KL1, bool KL2, bool KL3, bool KL4);
										
		void			Transitie( 		ZB_EVENT		inEv,
										ZB_STUURSTATUS 	inSs,
										ZB_LOOPSTATUS	inLs,
										ZB_STUURSTATUS*	uitSs,
										ZB_LOOPSTATUS*	uitLs);
									
		ZB_STUURSTATUS	stuurStatus;
		ZB_LOOPSTATUS	loopStatus;
	private:
		unsigned long	toTime;		// timeout time
		bool			timerSet;
		void			SetTimer(unsigned long ms);
};
