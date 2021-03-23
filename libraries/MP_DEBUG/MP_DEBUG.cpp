#define DEBUG
#include <string.h>
#include <MP_DEBUG.h>

int DB_level;

char DB_string[2 * DB_MAX_LEVEL];								// hergebruikte ruimte voor indent string
char DB_spaties[] = "                                     ";	// voorraad spaties....

char* DB_levstr()
{
	memcpy( DB_string, DB_spaties, 2 * DB_level);
	DB_string[2 * DB_level] = '\0';
	return DB_string;
}

char* DB_enter()
{
	DB_level++;
	if (DB_level > DB_MAX_LEVEL) DB_level = DB_MAX_LEVEL;
	return DB_levstr();
}

char* DB_leave()
{
	char *result = DB_levstr();
	DB_level--;
	if (DB_level < 0 ) DB_level = 0;
	return result;
}
