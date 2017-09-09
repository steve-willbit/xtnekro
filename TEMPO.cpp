/* ------------------------------------------------------------------------ *

	Tempo.cpp

	6 Aprile 2005				Fabrizio

	$Log: not supported by cvs2svn $
	
* ------------------------------------------------------------------------ */

// CONFIG INCLUDES
// CONFIG INCLUDES
// CONFIG INCLUDES

// always the first
#include "XTConfig.h"
#include "QXPConfig.h"

// STANDARD INCLUDES
// STANDARD INCLUDES
// STANDARD INCLUDES

#if QXP60
#if defined(__MWERKS__) && defined(__MACH__)
	#define TARGET_API_MAC_OSX 1
	#include <MSL MacHeadersMach-O.h>
#endif // defined(__MWERKS__) && defined(__MACH__)
#endif // QXP60

#include <stdio.h>
#include <time.h>

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "Errori.h"
#include "Tempo.h"

// STATICS
// STATICS
// STATICS

// per restituire la stringa dell'ora corrente
static uchar gStrData[MAXPATHNAME] = "";

// per prendere il tempo della macchina 
static struct tm gTempoMacchina = {0,0,0,0,0,0,0,0,0};

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	PrendiData

* ------------------------------------------------------------------------ */
uchar* XTAPI PrendiData() throw()
{
	// per prelevare i secondi della data corrente
	int32 lSecs = 0;
	
	// prendo i secondi da 01/01/1904
	lSecs = time(NULL);

	// converto i secondi in data
	gTempoMacchina = (*localtime(&lSecs));
	sprintf((char*)gStrData, "%s", asctime(&gTempoMacchina));

	gStrData[STRLEN(gStrData) - 1] = '\0';

	return(gStrData);
} // PrendiData

/* ------------------------------------------------------------------------ *

	PrendiDataXNomeFile

* ------------------------------------------------------------------------ */
uchar* XTAPI PrendiDataXNomeFile() throw()
{
	// per prelevare i secondi della data corrente
	int32 lSecs = 0;

	// prendo i secondi da 01/01/1904
	lSecs = time(NULL);

	// converto i secondi in data
	gTempoMacchina = (*localtime(&lSecs));
	sprintf((char*)gStrData, "%0.2d%0.2d%0.2d%0.2d",
				gTempoMacchina.tm_mon + 1,
				gTempoMacchina.tm_mday,
				gTempoMacchina.tm_hour,
				gTempoMacchina.tm_min );

	return(gStrData);
} // PrendiDataXNomeFile
