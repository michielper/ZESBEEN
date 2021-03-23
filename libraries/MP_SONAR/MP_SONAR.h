// 	MP_SONAR.h
//	2019/12/07


#define	SONARDURATIONMAX	10000


class SONAR
{
	public:
		SONAR( int trig_pin, int echo_pin);
		int		Meet();		// geeft afstand in centimeters
	private:
		int 	my_trig_pin;
		int 	my_echo_pin;
};