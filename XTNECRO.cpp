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

// DEFINES
// DEFINES
// DEFINES

// identificatore dell’Xtension: MOHz
#define kIdXtension NORMALIZE32('MHOn')

// a unique name for saving the palette position
// on Win they appear in the windows registry
// on Mac they are the name of the resources
#define INFOPALETTEPOS "InfoPalette" 

// the windows registry key where to save the xtension preferences
#define XTPREFNAME "XTNekro.prf" // "Software\\Sinedita\\QuarkXPress\\XTNecro.xnt\\2.0.0"

// these strings are used to save the prefereces:
// on WINOS are the name of the windows registry key
// on MACOS are the name saved in the resource
#define XTPREFERENCES "Preferences"

// the resource type where to save the xtension preferences
#define XTPREF 'PREF'
#define XTPREFERENCESID 20000

// STATICS
// STATICS
// STATICS

// set to true when openpalettes_callback is called
static bool8 gOpenPalettesCalled = FALSE;

// GLOBALS
// GLOBALS
// GLOBALS

#if WINOS
HINSTANCE gXTensionHinst = NULL;
#endif //WINOS

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

#if 0

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function 		ImpostaNomeCartellaXtension
	@abstract		cartella di xtension.
	@discussion		Imposta la cartella XTension corrente.

					28 Novembre 1996 - Stefano

	@result			nessuno.
*/
static void XTAPI ImpostaNomeCartellaXtension() throw();

/*!
	@function 		ImpostaNomeCartellaWindows
	@abstract		cartella di windows.
	@discussion		Imposta la cartella di Windows.

					17 Dicembre 1996 - Stefano

	@result			nessuno.
*/
static void XTAPI ImpostaNomeCartellaWindows() throw();

/*!
	@function 		deactivate_callback
	@abstract		callback for _XT_DEACTIVATE call.
	@discussion		XPress calls this callback function when the XTension has to deactivate itself.

					22 marzo 2005 - Fabrizio.

	@param			cbparam pointer to the xtvoidrec record (see XDK documentation).
	@result			returns error code if there is a problem.
*/
int32 XTAPI deactivate_callback(xtvoidrec* cbparam);

/*!
	@function		idle_callback
	@abstract 		callback per la chiamata di _XT_IDLE.
	@discussion		XPress calls this callback every cycle of the main event. It
					is only used on Mac. Xpress strongly discourage registration
					for _XT_IDLE on Windows.

					22 marzo 2005 - Fabrizio.
	
	@param 			cbparam xtidlerec (see XDK documentation).
	@result  		returns noErr.
*/
int32 XTAPI idle_callback(xtidlerec* cbparam);

/*!
	@function 		setlanguage_callback
	@abstract		callback for _XT_SETLANGUAGE call.
	@discussion		XPress calls this callback function each time the user changes the XPress language .

					22 marzo 2005 - Fabrizio.

	@param			cbparam pointer to the xtsetlanguagerec record (see XDK documentation).
	@result			returns error code if there is a problem.
*/
int32 XTAPI setlanguage_callback(xtsetlanguagerec* cbparam);

/*!
	@function 		openpalettes_callback
	@abstract		callback for _XT_OPENPALETTES call.
	@discussion		XPress calls this callback functionat XTension startup.  This is XTension's chance
					to open any palettes that were open when the user quit QuarkXPress last time.

					22 marzo 2005 - Fabrizio.

	@param			cbparam pointer to the xtvoidrec record (see XDK documentation).
	@result			returns error code if there is a problem.
*/
int32 XTAPI openpalettes_callback(xtvoidrec *cbparam);

/*!
	@function 		xtaddxthooks_callback
	@abstract		callback for _XT_ADDXTHOOKS call.
	@discussion		Once your XTensions module is initialized, QuarkXPress calls this function.
					You can use this function for registering XTension cbcodes.

					22 marzo 2005 - Fabrizio.

	@param			cbparam pointer to the xtaddxthooksrec record (see XDK documentation).
	@result			returns error code if there is a problem.
*/
int32 XTAPI xtaddxthooks_callback(xtaddxthooksrec* cbparam) throw();

/*!
	@function 		setlanguage_callback
	@abstract		callback for _XT_SETLANGUAGE call.
	@discussion		XPress calls this callback function each time the user changes the XPress language .

					22 marzo 2005 - Fabrizio.

	@param			cbparam pointer to the xtsetlanguagerec record (see XDK documentation).
	@result			returns error code if there is a problem.
*/
int32 XTAPI setup_callback(xtsetuprec* cbparam) throw();

/*!
	@function		XTActivateCallback
	@abstract		main entry point for QuarkXTensions.
	@discussion		Main entry point for QuarkXTensions.

					22 marzo 2005 - Fabrizio.

	@param			cbparam xtactivaterec (see XDK documentation)
	@result			returns error code if there is a problem.
*/
#if QXP60
extern "C"
#if WINOS
__declspec(dllexport)
#endif
int32 XTActivateCallback(xtactivaterec *cbparam); 
#endif // QXP60

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	ImpostaNomeCartellaXtension

* ------------------------------------------------------------------------ */
static void XTAPI ImpostaNomeCartellaXtension() throw()
{
	Str255 moduleName = "";
	// costruisco il nome completa della cartella XTension
	GetModuleFileName(gXTensionHinst, (char*) moduleName, sizeof(moduleName));
	
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
static void XTAPI ImpostaNomeCartellaWindows() throw()
{
	GetWindowsDirectory((LPSTR) gNomeCartellaWindows, MAXPATHNAME);
} // ImpostaNomeCartellaWindows

/* ------------------------------------------------------------------------ *

	DllMain
	
* ------------------------------------------------------------------------ */
extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  switch (fdwReason) 
  {
	  case DLL_PROCESS_ATTACH:
	  {
			gXTensionHinst = hinstDLL;
			break;
	  }				
	  case DLL_PROCESS_DETACH:
	  {
	     	break;
	  }
  } 
  return (TRUE);
}
	
/* ------------------------------------------------------------------------ *

	deactivate_callback
	
* ------------------------------------------------------------------------ */
int32 XTAPI deactivate_callback(xtvoidrec* cbparam)
{
	assert(NULL != cbparam);
	
	// salvataggio delle preferenze
	DBP::SavePreferences
	(
		(Ptr) &gXtPreferences,
		sizeof(gXtPreferences),
		STRCONST(XTPREFNAME),
		XTPREF,
		XTPREFERENCESID
	);
	
	// save the status af the pagination palette
	DBP::SavePalettePos(/*STRCONST(XTPREFNAME), XTPREFERENCESID,*/ gInfoPaletteWLocHandle, gInfoPaletteShowingFlag);
	
	/// stampa il messaggio di uscita sul file di rapporto e sulla paletta
	StampaMessaggio(kUscita);
	
	return (noErr);

} // deactivate_callback

/* ------------------------------------------------------------------------ *

	idle_callback
	
* ------------------------------------------------------------------------ */
int32 XTAPI idle_callback(xtidlerec* cbparam)
{
	// per ora uso l'idle callback poi sposto tutto su usertimer

	// L'Xtension verifica ciclicamente se nella cartella dei file da
	// impaginare c'e' un file. Se il file e' presente passa all'esecuzione
	// del comando scritto nella prima riga del file.

	// DBP::FreezeUndo();
	
	gErroreXtension = kNessunErrore;

	switch (PrendiOperazioneCorrente()) 
	{
		case kCoseDiQuattroD:
			gErroreXtension = CoseDiQuattroD();
			break;
#if 0 // OLDESTVERSION
		case kCoseDiAppWare:
			gErroreXtension = CoseDiAppWare();
			break;
		case kCoseDiNotifica:
			gErroreXtension = CoseDiNotifica();
			break;
#endif // OLDESTVERSION
		case kCoseDiStampa:
			CoseDiStampa();
			break;
		case kCoseDellUtente:
			break;
		case kIdle:
			break;
	}
	

	if (gErroreXtension != kNessunErrore) 
	{
		// errore nel gestione delle operazioni dell'xtension
		MostraErrore(gErroreXtension);

		// torno dall'utente
		ImpostaCoseDellUtente();
	}

	// in base all'operazione corrente di Xpress scelgo se abilitare o
	// disabilitare l'impaginazione 
	if (PrendiOperazioneCorrente() == kCoseDiQuattroD)
	{
		// devo permettere di disabilitare l'impaginazione
		Str255 tempStr = "";
		DBP::GetResourceString(tempStr, MENUSTRID,  DISABILITAMENUID);
		DBP::SetMenuCommandText(gNecroMenu, gAbilitaDisabilitaMenuId, tempStr);
	}
	else if (PrendiOperazioneCorrente() == kCoseDellUtente)
	{
		// devo permettere di abilitare l'impaginazione
		Str255 tempStr = "";
		DBP::GetResourceString(tempStr, MENUSTRID,  ABILITAMENUID);
		DBP::SetMenuCommandText(gNecroMenu, gAbilitaDisabilitaMenuId, tempStr);
	}

	AggiornaMenu();
	
	// DBP::UnFreezeUndo();

	return(noErr);	 
			
} // idle_callback

/* ------------------------------------------------------------------------ *

	setlanguage_callback
	
* ------------------------------------------------------------------------ */
int32 XTAPI setlanguage_callback(xtsetlanguagerec* cbparam)
{
	assert(NULL != cbparam);
	
	gXTensionLanguage = cbparam->currentlanguage;

	// fino a quando mancano le risorse commento la parte relativa alla lingua inglese
	if (verItaly != gXTensionLanguage /*&& verUS != gXTensionLanguage*/ )
	{
		// default if a language we can't deal with
		// gXTensionLanguage = verUS;
		gXTensionLanguage = verItaly;	
	}
	
	if (gOpenPalettesCalled)
	{
		// pagination palette
		if (gInfoPaletteShowingFlag)
		{
			assert(NULL != gInfoPaletteId);
		
			if (noErr == xd_pushdlg(gInfoPaletteId)) 
			{
				xd_close(CLOSE_CANCEL, 0, NULL);
				xd_popdlg();
			}
			xd_createpalette(_XT_INFOPALETTEWAP, (uint32) gInfoPaletteWLocHandle, &gInfoPaletteId);
		}
	}	

	return (noErr);
	
} // setlanguage_callback

/* ------------------------------------------------------------------------ *

	openpalettes_callback
	
* ------------------------------------------------------------------------ */
int32 XTAPI openpalettes_callback(xtvoidrec *cbparam)
{
	XTError result = noErr;

	// info box palette
	if (gInfoPaletteShowingFlag)
	{
		result = xd_createpalette(_XT_INFOPALETTEWAP, (uint32) gInfoPaletteWLocHandle, &gInfoPaletteId);
	}

	// this is a simple to remberer if the openpalettes callback is already called
	gOpenPalettesCalled = TRUE;

	return(result);	
}

/* ------------------------------------------------------------------------ *

	xtaddxthooks_callback
	
* ------------------------------------------------------------------------ */
int32 XTAPI xtaddxthooks_callback(xtaddxthooksrec* cbparam) throw()
{
	// request one custom cbcode for each of the xtension's waps
	XTError err = xtallocatecbcodes(NUMBEROFWAPS, &gXtWapCbCode);
	
	return(noErr);
} // xtaddxthooks_callback

/* ------------------------------------------------------------------------ *

	setup_callback
	
* ------------------------------------------------------------------------ */
int32 XTAPI setup_callback(xtsetuprec* cbparam) throw()
{
	assert(NULL != cbparam);

	BEGIN_SETUP_XT;
	
	if (!initsetuprecord(cbparam, XTVER, XTSTRUCTVER, XTLIBVER) && XT_SETUP_FUNC_GROUPS)
	{	

#if defined(NDEBUG)
		// controllo la presenza della chiave
		if (!DBP::TestHaspKey(XTNECROHASPSTR))
		{
			// chiave non trovata
			// mostra la dialog di errore
			NOPROTECTIONMESSAGE;

			goto _XTNotActivated;
		}
#endif // defined(NDEBUG)		
		
		// chiudo lo splash di XPress all'avvio
		DBP::CloseXPressSplash();
			
		// deactivate
		xtregistercbcode(_XT_DEACTIVATE, deactivate_callback, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);	
		
		// menu
		xtregistercbcode(_XT_ADDMENUITEMS, addmenuitems_callback, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);		
		xtregistercbcode(_XT_UPDATEMENUITEMS, updatemenuitems_callback, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
	
		// menu command
		xtregistercbcode(_XT_HANDLECOMMAND, handlecommand_callback, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);

		// language
		xtregistercbcode(_XT_SETLANGUAGE, setlanguage_callback, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);

		// palettes
		xtregistercbcode(_XT_OPENPALETTES, openpalettes_callback, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);

		// idle
		xtregistercbcode(_XT_IDLE, idle_callback, XTCBF_NORMAL | XTCBF_NOTACTIVE , XT_NORMPRIORITY, NULL);
		
		// registrazione delle WAP
		xtregistercbcode(_XT_IMPAGINAZIONEWAP, ImpaginazioneWap, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		xtregistercbcode(_XT_GIUSTIFICAZIONEWAP, GiustificazioneWap, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		xtregistercbcode(_XT_RIEMPITIVIWAP, RiempitiviWap, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		xtregistercbcode(_XT_IMMAGINIWAP, ImmaginiWap, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		xtregistercbcode(_XT_BOZZAWAP, BozzaWap, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		xtregistercbcode(_XT_INGOMBROWAP, IngombroWap, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		xtregistercbcode(_XT_FINALEWAP, FinaleWap, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		xtregistercbcode(_XT_DOCUMENTOWAP, DocumentoWap, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		xtregistercbcode(_XT_DATIWAP, DatiWap, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		xtregistercbcode(_XT_DISPOSIZIONEWAP, DisposizioneWap, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		xtregistercbcode(_XT_PREFERENZEWAP, PreferenzeWap, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		xtregistercbcode(_XT_CARTELLEWAP, CartelleWap, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		xtregistercbcode(_XT_PREPARADOCWAP, PreparaDocWap, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		xtregistercbcode(_XT_ABOUTWAP, AboutNecroWap, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		
		// registrazione paletta informazioni
		xtregistercbcode(_XT_INFOPALETTEWAP,InfoPaletteWap, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		
		// Imposta la cartella di Windows
		ImpostaNomeCartellaWindows();		
		
		// prendo nome cartella Xtension
		ImpostaNomeCartellaXtension();
		
		// get the palettes position
	
		DBP::LoadPalettePos
		(
		//	STRCONST(XTPREFNAME),
			gInfoPaletteWLocHandle,
			gInfoPaletteShowingFlag,
		//	XTPREFERENCESID,							
			STRCONST(INFOPALETTEPOS)
		);
		
		// carico le preferenze dell'XT
		DBP::LoadPreferences
		(
			(Ptr) &gXtPreferences,
			sizeof(gXtPreferences),
			STRCONST(XTPREFNAME),
			XTPREF,
			XTPREFERENCESID,
			STRCONST(XTPREFERENCES)
		);
			
		// inizializzazione delle stringhe
		CreaTutteLeStringhe();

		// inizializzazione degli errori dell'Xtension
		InizializzaErrori();
		
		// stampa il messaggio di inizio sul file di rapporto e
		// sulla paletta dell'Xtension
		DaiMessaggio(kInizializzazione);
	}
	else
	{
		goto _XTNotActivated;
	}
	
	END_SETUP_XT;
	
_XTNotActivated:

	return(XTnotactivated);
	
} // setup_callback

/* ------------------------------------------------------------------------ *

	xtactivate_cb
	
* ------------------------------------------------------------------------ */
extern "C"
#if WINOS
__declspec(dllexport)
#endif
int32 XTActivateCallback(xtactivaterec *cbparam) 
{
	assert(NULL != cbparam);

	BEGIN_ACTIVATE_XT;
	
	if (ADD_XT(kIdXtension))
	{
		xtsetdefpriority(XT_NORMPRIORITY);
		xtregister_xtsetup(setup_callback, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
		xtregister_xtaddxthooks(xtaddxthooks_callback, XTCBF_NORMAL, XT_NORMPRIORITY, NULL);
	}
	else
	{
		END_ACTIVATE_XTUNACTIVATED;
	}
	END_ACTIVATE_XT;

} // xtactivate_cb

#endif // 0

/* ------------------------------------------------------------------------ *

	XTNecro.cpp

	22 marzo 2005 - Fabrizio

	Copyright © Sinedita S.r.l 2005. All Rights Reserved.

	$Log: not supported by cvs2svn $
	Revision 1.6  2005/06/10 13:46:47  taretto
	modificato salvataggio preferenze su QXP60
	
	Revision 1.5  2005/05/11 16:26:04  taretto
	versione 2.0.0.1
	
	Revision 1.4  2005/05/10 09:30:28  taretto
	finestra di about
	
	Revision 1.3  2005/05/09 12:36:45  taretto
	aggiunta protezione HW
	
	Revision 1.2  2005/05/04 12:34:52  taretto
	comandi posiziona e ripristino ok
	
	Revision 1.1  2005/04/20 07:14:22  taretto
	importazione files
	
	
* ------------------------------------------------------------------------ */
