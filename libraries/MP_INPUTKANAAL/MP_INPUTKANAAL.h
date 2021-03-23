// 2021-02-16	initieel

class INPUTKANAAL
{
	public:
		INPUTKANAAL( int pin );
		bool	Opgaand();
		bool	Neergaand();
		bool	Kijk();			// actuele waarde, true indien HIGH
	private:
		int		mypin;
		bool	laatste_waarde;
};
