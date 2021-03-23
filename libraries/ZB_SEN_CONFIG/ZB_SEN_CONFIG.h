//---------------------------------------------------------------------------
// Hardware configuratie voor ZESBEEN - Sensoren Arduino Mega
// 2021-03-02 	Gesplitst en hernoemd naar ZB_ACT_CONFIG.h
// 2021-03-12	Toevoeging van aanraaksensoren en sonar voor beide voorbenen
//---------------------------------------------------------------------------

// Hardware configuratie

//--------------------------------------------------------------------
//-------------- Configuratie voor Controller-2 (sensoren en schouders)
//--------------------------------------------------------------------
#define AANTAL_SCHOUDERS	4

// schouders - aangestuurd door Controller-2
#define RVS_PIN		11	
#define RAS_PIN		8
#define LVS_PIN		10
#define LAS_PIN		9
// correctiewaarden voor de schouders, opgeteld bij stuurwaarde
#define RVS_COR		0	
#define RAS_COR		-10
#define LVS_COR		5
#define LAS_COR		0

//--------------------------------------------
// Sonar-afstand waarbij event optreeedt
//--------------------------------------------
#define SONAR_AFST_KORT		10
#define SONAR_AFST_MID		20

//--------------------------------------------
// Sonars en indicatoren
//--------------------------------------------
#define SONAR_LINKS_TRIG_PIN			25
#define SONAR_LINKS_ECHO_PIN			24
#define LED1_SONAR_LINKS_PIN			26
#define LED2_SONAR_LINKS_PIN			27

#define SONAR_RECHTS_TRIG_PIN			35
#define SONAR_RECHTS_ECHO_PIN			34
#define LED1_SONAR_RECHTS_PIN			36
#define LED2_SONAR_RECHTS_PIN			37

//---------------------------------------------
// Aanraakschakelaars
//---------------------------------------------
#define SCHAK_LINKS_BUITEN_PIN			23
#define SCHAK_LINKS_BINNEN_PIN			22

#define SCHAK_RECHTS_BUITEN_PIN			33
#define SCHAK_RECHTS_BINNEN_PIN			32

// Communicatiekanalen
#define C2_KANAAL_A_PIN			3		// 	output
#define C2_KANAAL_B_PIN			2		// 	output
#define C2_KANAAL_C_PIN			A1		//	input
#define C2_KANAAL_D_PIN			A0		//	input




