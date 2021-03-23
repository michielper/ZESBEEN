class BUTTON
{
  private:
    int 	mypin;
    bool 	vorige;			// vorige resultaat van Kijk()
  public:
	BUTTON( int pin);			// Constructor
	bool 	Kijk();			// true = ingedrukt = LOW
    bool 	Losgelaten();	// true als nu HIGH en vorige keer LOW
    bool	Ingedrukt();	// true als nu LOW en vorige keer HIGH
}; // end class BUTTON
