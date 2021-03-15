//----------------------
// Zesbenig beest
// 2021/01/28	aanpassing voor zes benen
//----------------------
#define BST_AANT_BENEN				6

class BEEST
{
	public:
		BEEST( BEEN* been_rv, BEEN* been_rm, BEEN* been_ra, BEEN* been_lv, BEEN* been_lm, BEEN* been_la);
		void 	Init();
		void	PowerOn();
		void	PowerOff();
		void	StartLopen();
		void	StartOpstaan();
		
		void	ZetDoelRust(int GWvaart = 1);		// alle benen gevouwen
		void	ZetDoelStaan(int GWvaart = 1);		// alle benen recht
		
		void	DirectRust();
		void	DirectStaan();
		void	GaNaarStap( int n);	// ga direct naar beweegindex n
		
		bool	TriggerVooruit();		// trigger dij en knie en neem volgende loopstand wanneer klaar
		bool	TriggerAchteruit();
		bool	Trigger();			// trigger dij en knie geef true indien beide klaar
		bool	TriggerOpstaan();
		int		BST_beweeg_index;	// index in beweegcyclus
		
	private:
		BEEN* rv;
		BEEN* rm;
		BEEN* ra;
		BEEN* lv;
		BEEN* lm;
		BEEN* la;
		BEEN* benen[BST_AANT_BENEN];
};
