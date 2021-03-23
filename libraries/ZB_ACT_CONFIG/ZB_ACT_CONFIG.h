//---------------------------------------------------------------------------
// Hardware configuratie voor HOUTVOETJE met Arduino Mega
// 2020-12-05 Init 
// 2020-12-11 met Ruststand
// 2021-01-09 nieuwe hw config
// 2021-01-10 vereenvoudigde initialisatie, alleen correctie voor middenstand
// 2021/02/28 aanpassing voor zes benen
// 2021-03-02 Gesplitst en hernoemd naar ZB_ACT_CONFIG.h
//---------------------------------------------------------------------------

// Hardware configuratie

#define	AANTAL_LOOPGEWRICHTEN	12

// dijen en knieen - aangestuurd door Controller-1
#define RVD_PIN			2	
#define	RVK_PIN			3	
#define	RMD_PIN			4
#define	RMK_PIN			5
#define	RAD_PIN 		6
#define	RAK_PIN 		7

#define	LVD_PIN			8
#define	LVK_PIN			9
#define	LMD_PIN			10
#define	LMK_PIN			11
#define	LAD_PIN			12
#define	LAK_PIN			13


// Buttons en Leds op Controller-1
#define	LED_ACHT_PIN	35
#define	BUTTON_ACHT_PIN	37
#define LED_ZIT_PIN		39
#define BUTTON_ZIT_PIN	41
#define LED_STA_PIN		43
#define BUTTON_STA_PIN	45
#define LED_LOOP_PIN	46
#define BUTTON_LOOP_PIN	49

#define POWER_RELAIS_PIN		22

// Correctiewaarden, opgeteld bij stuurwaarde
#define RVK_COR		0
#define RVD_COR		0
#define RMK_COR		0
#define RMD_COR		0
#define RAK_COR		0
#define RAD_COR		0

#define LVK_COR		0
#define LVD_COR		0
#define LMK_COR		0
#define LMD_COR		0
#define LAK_COR		0
#define LAD_COR		0

// Communicatiekanalen
#define C1_KANAAL_A_PIN			47		// 	SEN naar ACT
#define C1_KANAAL_B_PIN			49		// 	SEN naar ACT

#define C1_KANAAL_C_PIN			51		//	ACT naar SEN, loopstatus lsb
#define C1_KANAAL_D_PIN			53		//	ACT naar SEN, loopstatus msb






