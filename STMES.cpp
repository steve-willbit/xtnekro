/* ------------------------------------------------------------------------ *

	StampaMessaggi.cpp

	5 Aprile 2005				Fabrizio

	$Log: not supported by cvs2svn $
	Revision 1.1  2005/04/20 07:14:22  taretto
	importazione files
	
	
* ------------------------------------------------------------------------ */

/* Required Includes ********************************************************/
#include PROJECT_HEADERS
#if WINOS
#pragma hdrstop		// force Visual C++ precompiled header
#endif

#include "Include.h"

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "Errori.h"
#include "Paletta.h"
#include "Pref.h"
#include "ResStr.h"
#include "Tempo.h"
#include "XTNecro.h"

#include "StMes.h"

// CONSTS
// CONSTS
// CONSTS

// tipo e creatore del file di rapporto
#define kTipoFileRapporto 0x54584554 // tipo testo
#define kCreatoreFileRapporto 0x74787474 // kCreatoreTeachText

// GLOBALS
// GLOBALS
// GLOBALS

// identificatore del file di debug
static int16 gRiferimentoFileRapporto = 0;
// stringa da aggiungere al messaggio
static uchar gStringaMessaggio[MAXPATHNAME] = "";
// nome del file di rapporto
static uchar gNomeFileRapporto[MAXPATHNAME] = "";
// per il file di rapporto
static FSSpec gFSSpecFileRapporto = {0,0,""};

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			ApriFileRapporto
	@abstract 		gestione file report.
	@discussion		Apertura file di rapporto.

					6 Aprile 2005		Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
static void XTAPI ApriFileRapporto() throw();

/*!
	@function			ChiudiFileRapporto
	@abstract 		gestione file report.
	@discussion		Chiusura file di rapporto.

					6 Aprile 2005		Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
static void XTAPI ChiudiFileRapporto() throw();

/*!
	@function			CancellaFileRapporto
	@abstract 		gestione file report.
	@discussion		Cancella il file di report.

					6 Aprile 2005		Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
static void XTAPI CancellaFileRapporto() throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	ApriFileRapporto

* ------------------------------------------------------------------------ */
static void XTAPI ApriFileRapporto() throw()
{	
	// creo l'FSSpec del file di rapporto
	gFSSpecFileRapporto.vRefNum = gFSSpecXtension.vRefNum;
	gFSSpecFileRapporto.parID = gFSSpecXtension.parID;
	STRCPY(gFSSpecFileRapporto.name,  gFSSpecXtension.name);	
	STRCAT(gFSSpecFileRapporto.name, "\\");
	STRCAT(gFSSpecFileRapporto.name, gStrNomeFileRapporto);
	
	// apertura del file	
	while (1)
	{
		gRiferimentoFileRapporto = _lopen((char*) gFSSpecFileRapporto.name, OF_SHARE_EXCLUSIVE);  // <-- was FSOpen
		if ( gRiferimentoFileRapporto == HFILE_ERROR ) 
			break;
		HCreate(gFSSpecXtension.vRefNum, gFSSpecXtension.parID, gFSSpecFileRapporto.name, kCreatoreFileRapporto, kTipoFileRapporto);
	}
} // ApriFileRapporto

/* ------------------------------------------------------------------------ *

	ChiudiFileRapporto

* ------------------------------------------------------------------------ */
void XTAPI ChiudiFileRapporto() throw()
{
	FSClose(gRiferimentoFileRapporto);
} // ChiudiFileRapporto

/* ------------------------------------------------------------------------ *

	CancellaFileRapporto

* ------------------------------------------------------------------------ */
static void XTAPI CancellaFileRapporto() throw()
{
	// cancello il file
	sprintf((char*)gNomeFileRapporto, "%s\\%s", (char*)gNomeCartellaXpress, (char*)gStrNomeFileRapporto );
	FSDelete(gNomeFileRapporto, gNumeroVolumeXpress);
} // CancellaFileRapporto

/* ------------------------------------------------------------------------ *

	StampaSuRapporto

* ------------------------------------------------------------------------ */
void XTAPI StampaSuRapporto(char *stringa) throw()
{
	StampaSuRapporto((uchar*)stringa);
} // StampaSuRapporto

/* ------------------------------------------------------------------------ *

	StampaSuRapporto

* ------------------------------------------------------------------------ */
void XTAPI StampaSuRapporto(uchar *stringa) throw()
{
	int32	lLung = 0;
	
	if (stringa != NULL) 
	{
		ApriFileRapporto();

		// posizionamento in fondo per la scrittura
		GetEOF(gRiferimentoFileRapporto, &lLung);
		SetFPos(gRiferimentoFileRapporto, 1, lLung);
	
		// scrive la stringa
		lLung =  STRLEN(stringa);
		FSWrite(gRiferimentoFileRapporto, &lLung, (Ptr) stringa);
		
		/* -->
		uchar tmpstr[8] = "";
		CSTRCPY(tmpstr, "\r\n");

		lLung = CSTRLEN(tmpstr);
		FSWrite(gRiferimentoFileRapporto, &lLung, (Ptr) tmpstr);
		*/ // <-- da SClass per stampare un solo pessaggio per ogni riga
				
		ChiudiFileRapporto();
	}
} // StampaSuRapporto

/* ------------------------------------------------------------------------ *

	ConcatenaAllaStringaMessaggio

* ------------------------------------------------------------------------ */
void XTAPI ConcatenaAllaStringaMessaggio(uchar *stringamessaggio) throw()
{
	if (gStringaMessaggio[0] == '\0') 
	{
		// la stringa non contiene nulla
		STRCPY(gStringaMessaggio, stringamessaggio);
	}
	 else 
	{
		// la stringa contiene gi… qualcosa
		STRCAT(gStringaMessaggio, " ");
		STRCAT(gStringaMessaggio, stringamessaggio);
	}
} // ConcatenaAllaStringaMessaggio


/* ------------------------------------------------------------------------ *

	StampaMessaggio
	
* ------------------------------------------------------------------------ */
void XTAPI StampaMessaggio(messaggixtension messaggio) throw()
{
	if (messaggio == kInizializzazione) 
		CancellaFileRapporto();
	
	sprintf((char*)gStringaC, "%s:", (char*)PrendiData());
	StampaSuRapporto(gStringaC);
	StampaSuRapporto("\r\n");
	StampaSuRapporto(gStrMessaggi[messaggio - 1]);
	StampaSuRapporto("\r\n");
	
	if (gStringaMessaggio[0] != '\0') 
	{
		StampaSuRapporto(gStringaMessaggio);
		StampaSuRapporto("\r\n");
		STRCPY(gStringaMessaggio, "");
	}
	StampaSuRapporto("\r\n");
} // StampaMessaggio

/* ------------------------------------------------------------------------ *

	VisualizzaMessaggio

* ------------------------------------------------------------------------ */
void XTAPI VisualizzaMessaggio(messaggixtension messaggio) throw()
{
	Visualizza(gStrMessaggi[messaggio - 1]);
	if (gStringaMessaggio[0] != '\0') 
	{
		Visualizza(gStringaMessaggio);
	}

} // VisualizzaMessaggio

/* ------------------------------------------------------------------------ *

	DaiMessaggio

* ------------------------------------------------------------------------ */
void XTAPI DaiMessaggio(messaggixtension messaggio) throw()
{

	if (messaggio == kInizializzazione) 
		CancellaFileRapporto();

	// preparo la stringa con data e ora
	sprintf((char*)gStringaC, "%s:", (char*)PrendiData());

	// stampo sul file di rapporto
	StampaSuRapporto(gStringaC);
	StampaSuRapporto("\r\n");
	StampaSuRapporto(gStrMessaggi[messaggio - 1]);
	StampaSuRapporto("\r\n");
	if (gStringaMessaggio[0] != '\0') 
	{
		StampaSuRapporto(gStringaMessaggio);
		StampaSuRapporto("\r\n");
	}
	StampaSuRapporto("\r\n");

	// visualizzo sulla paletta
	Visualizza(gStrMessaggi[messaggio - 1]);
	if (gStringaMessaggio[0] != '\0')
	 {
		Visualizza(gStringaMessaggio);
	}

	// cancella l'eventuale impostazione della stringa di messaggio
	STRCPY(gStringaMessaggio, "");
}
