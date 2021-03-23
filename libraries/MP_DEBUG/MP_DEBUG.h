
//	2021-02-04	MP


#ifdef DEBUG

	#define DB_MAX_LEVEL	20
	extern int DB_level;		// globaal indent-niveau
	char* DB_levstr();
	char* DB_enter();
	char* DB_leave();
	
	#define DBINIT					Serial.begin(9600);DBZEGLN1("--- Serial Init ---")		

	#define DBZEG1(a) 				Serial.print(DB_levstr());Serial.print(a)
	#define DBZEGLN1(a) 			DBZEG1(a); Serial.println()
	#define DBZEG2(a,b) 			DBZEG1(a); Serial.print(" "); Serial.print(b)
	#define DBZEGLN2(a,b) 			DBZEG2(a,b); Serial.println()
	#define DBZEG3(a,b,c) 			DBZEG2(a,b); Serial.print(" "); Serial.print(c)
	#define DBZEGLN3(a,b,c) 		DBZEG3(a,b,c); Serial.println()
	#define DBZEG4(a,b,c,d) 		DBZEG3(a,b,c);Serial.print(" ");Serial.print(d)
	#define DBZEGLN4(a,b,c,d) 		DBZEG4(a,b,c,d);Serial.println()
	#define DBZEG5(a,b,c,d,e) 		DBZEG4(a,b,c,d);Serial.print(" ");Serial.print(e)
	#define DBZEGLN5(a,b,c,d,e) 	DBZEG5(a,b,c,d,e);Serial.println()
	#define DBZEG6(a,b,c,d,e,f) 	DBZEG5(a,b,c,d,e);Serial.print(" ");Serial.print(f)
	#define DBZEGLN6(a,b,c,d,e,f)	DBZEG6(a,b,c,d,e,f);Serial.println()


	#define DBENTER1(NAME) 				DBZEGLN2("=>", NAME)	   			;	DB_level++
	#define DBENTER2(NAME,x) 			DBZEGLN3("=>", NAME, x)   			;   DB_level++
	#define DBENTER3(NAME,x, y) 		DBZEGLN4("=>", NAME, x, y)			;   DB_level++
	#define DBENTER4(NAME,x, y, z) 		DBZEGLN5("=>", NAME, x, y, z)		;   DB_level++
	#define DBENTER5(NAME,x, y, z, p) 	DBZEGLN6("=>", NAME, x, y, z, p)	;   DB_level++

	#define DBLEAVE1(NAME) 			DB_level--; DBZEGLN2("<=", NAME); 			
	#define DBLEAVE2(NAME, x)		DB_level--; DBZEGLN3("<=", NAME, x); 		
	#define DBLEAVE3(NAME, x, y)	DB_level--; DBZEGLN4("<=", NAME, x, y); 	

#else

	#define DBINIT
	#define DBENTER1
	#define DBENTER2
	#define DBENTER3
	#define DBENTER4
	#define DBENTER5
	#define DBENTER1
	#define DBLEAVE1
	#define DBLEAVE2
	#define DBLEAVE3
	#define DBZEG1
	#define DBZEG2
	#define DBZEG3
	#define DBZEG4
	#define DBZEG5
	#define DBZEG6
	#define DBZEGLN1
	#define DBZEGLN2
	#define DBZEGLN3
	#define DBZEGLN4
	#define DBZEGLN5
	#define DBZEGLN6
	
#endif


