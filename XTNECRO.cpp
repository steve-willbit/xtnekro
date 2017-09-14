/* ------------------------------------------------------------------------ *

	XTNecro.cpp

	22 marzo 2005 - Fabrizio

	Convenzioni per la scrittura delle variabili, funzioni, costanti, .
	r. indica che si riferisce ad una costante di risorsa,
	i. indica che un item di una dialog
	k. indica che è una costante,
	m. indica che è una macro,
	g. indica che è una variabile globale,
	l. indica che è una variabile locale,
	le chiamate alle funzioni hanno la prima lettera maiuscola,

	Copyright © Sinedita S.r.l 2005. All Rights Reserved.
	
* ------------------------------------------------------------------------ */

/* Required Includes ********************************************************/
#include PROJECT_HEADERS
#if WINOS
#pragma hdrstop		// force Visual C++ precompiled header
#endif

#define MAINFILE 1

#include "Include.h"

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

//#include "DbpInclude.h"
//#include "DbpMenu.h"
//#include "DbpResource.h"
//#include "DbpPascalString.h"
//#include "DbpPref.h"
//#include "DbpVersion.h"
//#include "DbpOkDialog.h"
//#include "DbpConsts.h"
//#include "DbpSplashScreen.h"
//#include "DbpUndoEngine.h"

// XTNECRO INCLUDES
// XTNECRO INCLUDES
// XTNECRO INCLUDES

//#include "AppWare.h"
//#include "CalcPos.h"
//#include "Coord.h"
//#include "DocInfo.h"
//#include "Errori.h"
//#include "InfoNec.h"
//#include "Impag.h"
//#include "Menu.h"
//#include "OpXt.h"
//#include "Paletta.h"
//#include "Pref.h"
//#include "PrepDoc.h"
//#include "ReadFile.h"
//#include "ResStr.h"
//#include "StMes.h"
//#include "SpaceCol.h"
//#include "Tempo.h"

#include "XTNecro.h"

// GLOBALS
// GLOBALS
// GLOBALS

// set to true when openpalettes_callback is called
bool8 gOpenPalettesCalled = FALSE;

// FSSpec cartella XTension
FSSpec gFSSpecXtension= {0, 0, ""};

// nome cartella windows 
uchar gNomeCartellaWindows[MAXPATHNAME] = "";

// per prendere gli errori dell'xtension in tutta l'xtension
errorixtension gErroreXtension = kNessunErrore;

// per leggere gli errori del sistema operativo in tutta l'xtension
OSErr gErrore = noErr;

// stringa di uso generale
uchar gStringaC[MAXPATHNAME] = "";

// per prendere il nome della cartella di Xpress (XTension)
uchar gNomeCartellaXpress[MAXPATHNAME] = "";

// volume di partenza di QuarkXpress
int16 gNumeroVolumeXpress = 0;

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	ImpostaNomeCartellaXtension

* ------------------------------------------------------------------------ */
void XTAPI ImpostaNomeCartellaXtension() throw()
{
	Str255 moduleName = "";
	// costruisco il nome completa della cartella XTension
	GetModuleFileName(hinst, (char*) moduleName, sizeof(moduleName));
	
	STRCPY(gFSSpecXtension.name, moduleName);
	for (int32 i = STRLEN(gFSSpecXtension.name); i >= 0; --i)
	{
		if (gFSSpecXtension.name[i - 1] == '\\')
		{	
			// il primo back slash e sulla cartella XTension
			gFSSpecXtension.name[i - 1] = 0;
			break;
		}
	}
	
	// assegno nome cartella XT alla relativa globale 
	STRCPY(gNomeCartellaXpress, gFSSpecXtension.name);
		
} // ImpostaNomeCartellaXtension
	
/* ------------------------------------------------------------------------ *

	ImpostaNomeCartellaWindows

* ------------------------------------------------------------------------ */
void XTAPI ImpostaNomeCartellaWindows() throw()
{
	GetWindowsDirectory((LPSTR) gNomeCartellaWindows, MAXPATHNAME);
} // ImpostaNomeCartellaWindows

/* ------------------------------------------------------------------------ *

	XTNecro.cpp

* ------------------------------------------------------------------------ */
