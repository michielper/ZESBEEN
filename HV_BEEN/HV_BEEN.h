//	HV_BEEN.h

//	2020/12/08	Initieel, vanuit GV_BEEN.h
//  2021-01-04  Opstafasen
//	2021-01-14	Herziening
//--------------------------------------------------------------------

#define BN_AANT_LOOPFASEN	6	// totaal aantal loopfasen
#define BN_LOOP_OFFSET		3	// faseverschil tussen de benen

#define BN_AANT_OPSTAFASEN	3	// aantal fasen bij het opstaan

#define BN_NAAMLENGTE		8

#define BN_DIJ_RUST		150
#define BN_KNIE_RUST	0

#define BN_DIJ_STAAN	90
#define BN_KNIE_STAAN	155

enum	BEEN_TOESTAND
	{
		RUSTEND,
		STAAND,
		STAPPEND,
		BEWEGEND
	};
// Structs
struct GWDOEL
	{
		int doel;
		int versn;
		int vertr;
		int tikken;
	};
	
struct STAPFASE
	{
		GWDOEL	dij;
		GWDOEL	knie;
	} ;         // positie in een stapcyclus

class BEEN {
	public:
		BEEN(	char* naam, int beennummer, GEWRICHT* dij, GEWRICHT* knie);
		void		Init();	
		
		void		DirectRust();					// zet onmiddellijk in ruststand
		void		ZetDoelRust();					// ga naar ruststand

		void		ZetDoelStaan();					// been recht
		void		DirectStaan();

		void		StartLopen(int start_index);	// begin bij start_index
		void		StartOpstaan();
				
		bool		Trigger();						// ga naar ruststand
		bool		TriggerVooruit();				// geeft true als cyclus voltooid is
		bool		TriggerAchteruit();
		
		bool		TriggerKnieEerst();
		bool		TriggerDijEerst();
		bool		TriggerOpstaan();
				
		char		mijnnaam[BN_NAAMLENGTE];

		int			BN_nummer;				// been nummer
		int			BN_beweeg_index;			// huidige fase in loop

		void		ZetDoelStap( int n);	// zet doel op stap n in de loopcyclus
		void		GaNaarStap( int n );	// ga direct naar stap n in de loopcyclus

		GEWRICHT* 		mijnKnie;
		GEWRICHT*		mijnDij;
		
	private:

		STAPFASE		loop_array[BN_AANT_LOOPFASEN];
		STAPFASE		opstaan_array[BN_AANT_OPSTAFASEN];
		BEEN_TOESTAND	toestand;
};
