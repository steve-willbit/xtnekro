/* ------------------------------------------------------------------------ *

	Errori.cpp

	5 Aprile 2005						Fabrizio

	Descrizione:
	Contiene per la visualizzazione degli errori bloccanti. Dopo la loro
	visualizzazione si esce dall'applicazione.

	$Log: not supported by cvs2svn $
	Revision 1.1  2005/04/20 07:14:18  taretto
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
#include "DbpResource.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

/*
#include "Include.h"
#include "Define.h"
#include "Appl.h"
#include "Debug.h"
*/
#include "OpXt.h"
#include "Paletta.h"
#include "ResStr.h"
#include "StMes.h"
#include "Tempo.h"
#include "XTNecro.h"

#include "Errori.h"

// GLOBALS
// GLOBALS
// GLOBALS

// sono gli identificatori interni di Xpress per gli errori dell'Xtension
int16 gIdErroriBloccanti[kNumeroErroriBloccanti];

// stringa da aggiungere all'errore
uchar gStringaErrore[MAXPATHNAME] = "";

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			InizializzaErrore
	@abstract 		gestione errori.
	@discussion		Allocca, usando le procedure interne di Xpress, un errore dell'Xtension.
					Deve essere chiamata dopo la procedura per la lettura delle stringhe delle
					risorse. Non uso la PPP perche' importante che ci sia una conversione
					della stringa di errore.

					5 Aprile 2005 - Fabrizio.

	@param 			errore - codice errore da alloccare
	@result  			nessuno
*/
static void XTAPI InizializzaErrore(errorixtension errore) throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	InizializzaErrore

* ------------------------------------------------------------------------ */
static void XTAPI InizializzaErrore(errorixtension errore) throw()
{
	//gIdErroriBloccanti[errore - 1] = allocerror((StringPtr) gStrErrori[errore - 1]);

	QXStringRef resStrRef = NULL;
	int16 userErrID = 0;
	QXStringCreateFromCString((char*) gStrErrori[errore-1], 0 , (int32) CSTRLEN(gStrErrori[errore-1]),&resStrRef);
	XTAllocateErrorNumberForString(resStrRef, &userErrID);	// allocate custom error
	QXStringDestroy(resStrRef);


} // InizializzaErrore

/* ------------------------------------------------------------------------ *

	InizializzaErrori

* ------------------------------------------------------------------------ */
void XTAPI InizializzaErrori() throw()
{
	// alloco gli errori
	for (int16 lIndiceErrori = kPrimoErrore; lIndiceErrori < kErroreBloccante; lIndiceErrori++) 
	{
		InizializzaErrore((errorixtension)lIndiceErrori);
	}
} // InizializzaErrori

/* ------------------------------------------------------------------------ *

	ConcatenaAllaStringaErrore

* ------------------------------------------------------------------------ */
void XTAPI ConcatenaAllaStringaErrore(uchar *stringaerrore) throw()
{
	if (gStringaErrore[0] == '\0') 
	{
		// la stringa non contiene nulla
		STRCPY(gStringaErrore, stringaerrore);
	} 
	else 
	{
		// la stringa contiene gia' qualcosa
		STRCAT(gStringaErrore, " ");
		STRCAT(gStringaErrore, stringaerrore);
	}
} // ConcatenaAllaStringaErrore

/* ------------------------------------------------------------------------ *

	ConcatenaLongAllaStringaErrore

* ------------------------------------------------------------------------ */
void XTAPI ConcatenaLongAllaStringaErrore(int32 valore) throw()
{
	sprintf((char*)gStringaC, "%ld", valore);
	if (gStringaErrore[0] == '\0') 
	{
		// la stringa non contiene nulla
		STRCPY(gStringaErrore, gStringaC);
	} 
	else 
	{
		// la stringa contiene gia' qualcosa 
		STRCAT(gStringaErrore, " ");
		STRCAT(gStringaErrore, gStringaC);
	}
} // ConcatenaLongAllaStringaErrore

/* ------------------------------------------------------------------------ *

	VisualizzaErrore

* ------------------------------------------------------------------------ */
void XTAPI VisualizzaErrore(errorixtension errore) throw()
{
	Visualizza(gStrErrori[errore - 1]);
	if (gStringaErrore[0] != '\0') 
	{
		Visualizza(gStringaErrore);
	}
} // VisualizzaErrore

/* ------------------------------------------------------------------------ *

	StampaErrore
	
* ------------------------------------------------------------------------ */
void XTAPI StampaErrore(errorixtension errore) throw()
{
	wsprintf((char*)gStringaC, "%s:", PrendiData());
	StampaSuRapporto(gStringaC);
	StampaSuRapporto("\r\n");
	StampaSuRapporto(gStrErrori[kErroreNonBloccante - 1]);
	StampaSuRapporto("\r\n");
	StampaSuRapporto(gStrErrori[errore - 1]);
	StampaSuRapporto("\r\n");
	if (gStringaErrore[0] != '\0') 
	{
		StampaSuRapporto(gStringaErrore);
		StampaSuRapporto("\r\n");
		STRCPY(gStringaErrore, "");
	}
} // StampaErrore

/* ------------------------------------------------------------------------ *

	DaiErrore

* ------------------------------------------------------------------------ */
void XTAPI DaiErrore(errorixtension errore) throw()
{
	// preparo la stringa con data e ora
	sprintf((char*)gStringaC, "%s:", PrendiData());

	// stampa sul rapporto
	StampaSuRapporto(gStringaC);
	StampaSuRapporto("\r\n");
	StampaSuRapporto(gStrErrori[kErroreNonBloccante - 1]);
	StampaSuRapporto("\r\n");
	StampaSuRapporto(gStrErrori[errore - 1]);
	StampaSuRapporto("\r\n");
	if (gStringaErrore[0] != '\0') 
	{
		StampaSuRapporto(gStringaErrore);
		StampaSuRapporto("\r\n");
	}

	// visualizza sulla paletta 
	Visualizza(gStrErrori[errore - 1]);
	if (gStringaErrore[0] != '\0') 
	{
		Visualizza(gStringaErrore);
	}

	// cancella l'eventuale impostazione della stringa di errore
	STRCPY(gStringaErrore, "");
} // DaiErrore

/* ------------------------------------------------------------------------ *

	MostraErrore

* ------------------------------------------------------------------------ */
void XTAPI MostraErrore(errorixtension errore) throw()
{	
	// per testare se Xpress e' attivo
	Boolean lBool = TRUE;
	
	if (errore == kErroreDelCodice || errore < 1) 
	{
		quitxpress();
	}

	if (errore == kNessunErrore) return;
	
	if (errore < kErroreBloccante) 
	{
		// visualizza l'errore bloccante e poi esce da Xpress
		giveerror(gIdErroriBloccanti[errore - 1]);
	} 
	else 
	{
		// stampa l'errore sulla paletta e sul file di rapporto
		DaiErrore(errore);
	}

#if 0 // OLDESTVERSIONE
	if (errore < kErroreBloccante) {
	
		gErroreXtension = IsXpressAttivo(&lBool);
		if (gErroreXtension != kNessunErrore) {
			/* si e' verificato un errore nel testare se Xpress Š attivo */
			StampaDebug("IsXpressAttivo", (long) gErroreXtension);
			DaiErrore(gErroreXtension);
			quitxpress();
		}
		
		/* visualizza l'errore bloccante e poi esce da Xpress */
		giveerror(gIdErroriBloccanti[errore - 1]);
		
		while (1) {
			gErroreXtension = IsXpressAttivo(&lBool);
			if (gErroreXtension != kNessunErrore) {
				/* si e' verificato un errore nel testare se Xpress e' attivo */
				StampaDebug("IsXpressAttivo", (long) gErroreXtension);
				DaiErrore(gErroreXtension);
				quitxpress();
			}

			if (lBool == TRUE) break;
		}

		if (errore <= kErroreDiSistema) quitxpress();

	} 
	else
	{

		/* stampa l'errore sulla paletta e sul file di rapporto */
		DaiErrore(errore);

		/* per dare modo all'utente di cercare di risolvere l'errore */
		ImpostaCoseDellUtente();

		/* se Xpress non e' attivo invio una notifica */
		gErroreXtension = IsXpressAttivo(&lBool);
		if (gErroreXtension != kNessunErrore) {
			/* si e' verifica un errore bloccante nel testare se Xpress e' attivo */
			StampaDebug("IsXpressAttivo", (long) gErroreXtension);
			DaiErrore(gErroreXtension);
			quitxpress();

		}
		if (lBool == FALSE) {
			/* devo inviare la notifica */
			gErroreXtension = InviaNotifica();
			if (gErroreXtension != kNessunErrore) {
				/* si e' verifica un errore bloccante nell'invio della notifica */
				StampaDebug("InviaNotifica", (long) gErroreXtension);
				DaiErrore(gErroreXtension);
				quitxpress();
			}
			/* devo aspettare che xpress diventi per poter rimuovere la notifica */
			ImpostaCoseDiNotifica();
		}
	}
#endif  // 0 OLDESTVERSION
} // MostraErrore




