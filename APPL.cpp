
/* ------------------------------------------------------------------------ *

	Appl.c

	3 Novembre 1995						Stefano

	Descrizione:
	Contiene le procedure usate per eseguire il lancio di applicazioni esterne
	da Xpress

* ------------------------------------------------------------------------ */

/* Required Includes ********************************************************/
#include PROJECT_HEADERS
#if WINOS
#pragma hdrstop		// force Visual C++ precompiled header
#endif

#include "Include.h"

#include "Errori.h"
#include "PresFile.h"
#include "Appl.h"

#if kOLD

errorixtension IsXpressAttivo(Boolean *risultato)
/* ------------------------------------------------------------------------ *

	IsXpressAttivo

	3 Novembre 1995						Stefano

	Descrizione:
	Verifica che Xpress sia il programma attivo correntemente

	Parametri:
	risultato - puntatore al risultato, TRUE se Xpress Š attivo e FALSE
	altrimenti

	Ritorno:
	ritorna l'eventuale errore dell'xtension

* ------------------------------------------------------------------------ */

{
	ProcessSerialNumber lNPXpress = {0, 0};
	/* per prendere il numero del processo di Xpress */
	ProcessSerialNumber lNPCorrente = {0, 0};
	/* per prendere il numero del processo corrente */
	
	/* imposto il risultato */
	(*risultato) = FALSE;
	
	/* prendo il numero processo di Xpress */
	gErrore = GetCurrentProcess(&lNPXpress);
	if (gErrore != noErr) {
		/* errore di sistema */
		StampaDebug("GetCurrentProcess", (long) gErrore);
		return(kErroreGestioneProcessi);
	}

	/* prendo il numero processo del processo corrente */
	gErrore = GetFrontProcess(&lNPCorrente);
	if (gErrore != noErr) {
		/* errore di sistema */
		StampaDebug("GetFrontProcess", gErrore);
		return(kErroreGestioneProcessi);
	}
	
	/* verifico che Xpress sia l'applicazione di front */
	(*risultato) = (lNPCorrente.highLongOfPSN == lNPXpress.highLongOfPSN &&
				 	lNPCorrente.lowLongOfPSN == lNPXpress.lowLongOfPSN);

	return(kNessunErrore);
}

#endif /* kOLD */

errorixtension EseguiApplicazione(char *nomeapplicazione)
/* ------------------------------------------------------------------------ *

	EseguiApplicazione

	3 Novembre 1995						Stefano

	Descrizione:
	Permette di lanciare un'applicazione

	Parametri:
	nomeapplicazione - puntatore al percorso completo dell'applicazione da
	lanciare

	Ritorno:
	ritorna l'eventuale errore di sistema operativo

* ------------------------------------------------------------------------ */

{
#if kOLD

	FSSpec lFSSpecApplicazione = {0, 0, "\p"};
	/* FSSpec dell'applicazione da lanciare */
	LaunchParamBlockRec lRecordDiLancio;
	/* record per effettuare l'esecuzione dell'applicazione */
	ProcessSerialNumber lNPApplicazione = {0, 0};
	/* numero processo applicazione */

	/* prendo il FSSpec dell'applicazione da lanciare */
	gErrore = CreaFSSpecFile(nomeapplicazione, &lFSSpecApplicazione);
	if (gErrore != noErr) {
		/* non riesco a trovare l'applicazione AppWare */
		StampaDebug("CreaFSSpecFile", (long) gErrore);
		ConcatenaAllaStringaErrore(nomeapplicazione);
		return(kErrorePresenzaFile);
	}

	/* preparo il record di lancio */
	lRecordDiLancio.launchBlockID = extendedBlock;
	lRecordDiLancio.launchEPBLength = extendedBlockLen;
	lRecordDiLancio.launchFileFlags = 0;
	lRecordDiLancio.launchControlFlags = launchContinue + launchNoFileFlags;
	lRecordDiLancio.launchAppSpec = &lFSSpecApplicazione;
	lRecordDiLancio.launchAppParameters = NULL;

	/* lancio l'applicazione */
	gErrore = LaunchApplication(&lRecordDiLancio);
	if (gErrore != noErr) {
		/* non riesco a lanciare l'applicazione AppWare */
		StampaDebug("LaunchApplication", (long) gErrore);
		return(kErroreEsecuzioneAppWare);
	}

#endif /* kOLD */

	WinExec( "c:\\necro\\appl\\lista.exe", SW_SHOW );
	return(kNessunErrore);
}
