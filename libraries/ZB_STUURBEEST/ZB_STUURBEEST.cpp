// 2021-02-13	Initieel

#define DEBUG

#include "Arduino.h"
#include <Servo.h>
#include <MP_DEBUG.h>
#include <MP_LED.h>
#include <HOUTVOETJE_CONFIG.h>
#include <MP_SONAR.h>
#include <MP_LICHTMETER.h>
#include <HV_GEWRICHT.h>
#include <MP_KIJKER.h>
#include <ZB_STUURBEEST.h>

STUURBEEST::STUURBEEST( GEWRICHT* rvs, GEWRICHT* ras, GEWRICHT* lvs, GEWRICHT* las)
{
	rvsp = rvs; rasp = ras; lvsp = lvs; lasp = las;
	schouders[0] = rvsp;
	schouders[1] = rasp;
	schouders[2] = lvsp;
	schouders[3] = lasp;
	status = SB_STAT_MID;
};
//---------------------------
void STUURBEEST::Init()
{
	rvsp->Init();
	lvsp->Init();
	rasp->Init();
	lasp->Init();
};
//---------------------------
bool STUURBEEST::Trigger()
{
	rvsp->Trigger();
	lvsp->Trigger();
	rasp->Trigger();
	lasp->Trigger();
};
//---------------------------
void STUURBEEST::Rechtdoor()
{
	rvsp->GaAbs( STUUR_RECHT );
	lvsp->GaAbs( STUUR_RECHT );
	rasp->GaAbs( STUUR_RECHT );
	lasp->GaAbs( STUUR_RECHT );
	status = SB_STAT_MID;
};
//---------------------------
void STUURBEEST::ZetDoelRechtdoor()
{
	rvsp->ZetDoel( STUUR_RECHT , SB_VERSN, SB_VERTR, SB_TIKK);
	lvsp->ZetDoel( STUUR_RECHT , SB_VERSN, SB_VERTR, SB_TIKK);
	rasp->ZetDoel( STUUR_RECHT , SB_VERSN, SB_VERTR, SB_TIKK);
	lasp->ZetDoel( STUUR_RECHT , SB_VERSN, SB_VERTR, SB_TIKK);
	status = SB_STAT_MID;
};
//---------------------------
void STUURBEEST::Linksaf()
{
	rvsp->GaAbs( STUUR_LINKS );
	lvsp->GaAbs( STUUR_LINKS );
	rasp->GaAbs( STUUR_RECHTS);
	lasp->GaAbs( STUUR_RECHTS);
	status = SB_STAT_LINKS;
};
//---------------------------
void STUURBEEST::ZetDoelLinksaf()
{
	rvsp->ZetDoel( STUUR_LINKS , SB_VERSN, SB_VERTR, SB_TIKK);
	lvsp->ZetDoel( STUUR_LINKS , SB_VERSN, SB_VERTR, SB_TIKK);
	rasp->ZetDoel( STUUR_RECHTS, SB_VERSN, SB_VERTR, SB_TIKK);
	lasp->ZetDoel( STUUR_RECHTS, SB_VERSN, SB_VERTR, SB_TIKK);
	status = SB_STAT_LINKS;
};
//---------------------------
void STUURBEEST::Rechtsaf()
{
	rvsp->GaAbs( STUUR_RECHTS  );
	lvsp->GaAbs( STUUR_RECHTS );
	rasp->GaAbs( STUUR_LINKS );
	lasp->GaAbs( STUUR_LINKS  );
	status = SB_STAT_RECHTS;
};
//---------------------------
void STUURBEEST::ZetDoelRechtsaf()
{
	rvsp->ZetDoel( STUUR_RECHTS  , SB_VERSN, SB_VERTR, SB_TIKK);
	lvsp->ZetDoel( STUUR_RECHTS , SB_VERSN, SB_VERTR, SB_TIKK);
	rasp->ZetDoel( STUUR_LINKS , SB_VERSN, SB_VERTR, SB_TIKK);
	lasp->ZetDoel( STUUR_LINKS  , SB_VERSN, SB_VERTR, SB_TIKK);
	status = SB_STAT_RECHTS;
};
//---------------------------
void STUURBEEST::ZetDoelStatus( ZB_STUURSTATUS doelstatus)
{
	switch (doelstatus)
	{
		case SS_M:
			ZetDoelRechtdoor();
			break;
		case SS_L:
			ZetDoelLinksaf();
			break;
		case SS_R:
			ZetDoelRechtsaf();
			break;
	}
}



