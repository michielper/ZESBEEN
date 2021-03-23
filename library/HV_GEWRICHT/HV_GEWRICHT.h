//----------------------------------------------------------
//	HV_GEWRICHT.h
//
//	Aansturing van servo met Doel en Trigger
//
//	2019/12/23	GaAbs aangepast met long cast om integer overflow te voorkomen
//	2019/12/23	class members stapteken en stapgrootte consequent gemaakt
// 	2019/12/24 	V17	verbeterde stapgrootte en teken, class naam in hoofdletters
//-----------------------------------------------------------------

//~ #define		GW_NORM_VAART	10			// GWvaart waarbij 1 tik = 1 graad
#define		GRD_MIN			0
#define		GRD_MAX			180
#define		GRD_MID			90

#define     NAAMLENGTE 8

#define 	NSNL		1			//	default versnelling (bedrag waarmee accu per tik wordt opgehoogd)
#define 	NTRG		4			//	default vertraging (factor waarmee doel wordt vermenigvuldigd)
#define 	NTIK		200			//  aantal tiks naar doel - bepalend voor snelheid van lopen

//==================================================
class GEWRICHT
{
  public:						
	GEWRICHT( 							// Zet variabelen
				char* naam,				// Naam van het gewricht
				int pin, 				// Hardware positie
				int grdCorCorr); 		// Correctie: opgeteld bij aanstuurwaarde
				
	void	Init();						// Doe start-acties
			
    int 	GaAbs( 	 int grdIn);   		// Ga direct naar grdIn; update positie
										// geeft servo-waarde terug 
    int 	GaRel(   int grdIn);   		// Ga naar GWgrdLaatst + grdIn
										// geeft servo-waarde terug 
					
//--------------------------------------------------
// 		doel:	doel in graden (0-180)
//		s	:	factor waarmee elke tik wordt vemenigvuldigd voor optelling in GW_accu
//		t	:	factor waar GWtikAcccu door wordt gedeeld om de nieuwe positie te bepalen
//		n	:	gewenst aantal tikken om doel te bereiken
//					als n == 0 is de snelheid constant
//					als n != 0 is het aantal tikken constant
//--------------------------------------------------
    void    ZetDoel( 					// Zet een nieuwe doelstand
			int 	grdNieuwDoel, 		// Nieuwe stand in graden
			int		versnelling = 1,	// Accu increment per tik
			int		vertraging  = 10,	// Factor waarmee accu doel wordt vemenigvuldigd	
			int		n = 100);			// gewenst aantal tikken om doel te bereiken
					
	void	ZetDoelRust(); 				// Zet doel op Rust
    int 	DirectRust();               // ga direct naar rustpositie
	
	void	ZetDoelStaan();				// been recht	
	int		DirectStaan();
	
    bool	Trigger();					// Activeer beweging naar doel
    
    int		Write( int grd);			// schrijf waarde direct in servo
    int		Read();						// lees laatste waarde uit servo
    
    // public variabelen
	char	GWnaam[NAAMLENGTE];			// naam van gewricht
    int 	GWgrdLaatst;  				// laatste stand in graden
    int   	GWgrdDoel;					// huidige doelwaarde
private:
    Servo 	GWspier;			//	servo object
    int   	GWpin;   			// 	hardware configuratie
    int	  	GWgrdCor;			// 	correctie, opgeteld bij input van GaAbs
    int		GWgrdRst;			//	rustwaarde voor GaAbs
    int		GWgrdSta;			//	standwaarde	voor GaAbs
					        
    int		GW_accu;			// waarde verzameld, opgehoogd met GW_versnelling per tik
    int		GW_versnelling;		// increment van accu per tik
    int		GW_vertraging;		// factor waarmee afstand wordt vermenigvuldigd
    int		GW_tikken;			// gewenst aantal tikken naar doel
    
    int		GW_vooruit;			// vooruit: +1 of achteruit: -1
    int		GWtikTegaan;		// aantal tikken nog te gaan naar doel, wordt afgeteld tot 0
    int		GWgrdBegin;			// beginstand bij ZetDoel in graden (0-180)
    
    int		GWgrdMin;			// minimum waarde (default 0)
    int		GWgrdMax;			// maximum waarde (default 180)
    
}; // end class GEWRICHT

