// 2021-02-13	Initieel

#define STUUR_RECHT 	90
#define STUUR_LINKS 	60
#define STUUR_RECHTS	120

#define SB_VERSN		1
#define SB_VERTR		1
#define SB_TIKK			10

//-------------------------------
class STUURBEEST
{
  public:
    STUURBEEST( GEWRICHT* rvs, GEWRICHT* ras, GEWRICHT* lvs, GEWRICHT* las);
    void Init();
    void Rechtdoor();
    void ZetDoelRechtdoor();
    void Linksaf();
    void ZetDoelLinksaf();
    void Rechtsaf();
    void ZetDoelRechtsaf();
    
    bool	Trigger();
    
    GEWRICHT*	rvsp;
    GEWRICHT*	rasp;
    GEWRICHT*	lvsp;
    GEWRICHT*	lasp;
    GEWRICHT*	schouders[AANTAL_SCHOUDERS];
};
