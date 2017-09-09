
/* ------------------------------------------------------------------------ *

	Ripristina.cpp

	18 Aprile 2005		Fabrizio

	Descrizione:
	Contiene le procedure per dare la possibilita' di riniziare l'impaginazione
	senza dover riimportare i blocchetti di testo
	
	$Log: not supported by cvs2svn $
	Revision 1.2  2005/04/21 09:48:19  taretto
	tolte mLong per gestire numero moduli e colonne, modificata estensione per salvataggi in QXP
	
	Revision 1.1  2005/04/20 07:14:21  taretto
	importazione files
	

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

#include <cassert>
#include <stdio.h>
#include <string.h>
#include <time.h>

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "Errori.h"
#include "Menu.h"
#include "Document.h"
#include "ResStr.h"
#include "Pref.h"
#include "ReadFile.h"
#include "AppWare.h"
#include "DocInfo.h"
#include "InfoNec.h"
#include "SpaceCol.h"
#include "Stili.h"
#include "GesSlug.h"
#include "Coord.h"
#include "PresFile.h"
#include "LastImp.h"
#include "Impag.h"
#include "Tempo.h"
#include "Ripristi.h"

#include "Ripristi.h"

// STATICS GLOBALS
// STATICS GLOBALS
// STATICS GLOBALS

// serve per sapere se le informazioni in memoria si riferiscono
// al documento corrente
static uint32 gCodiceImpaginazione = 0;

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	PrendiCodiceImpaginazione

* ------------------------------------------------------------------------ */
uint32 PrendiCodiceImpaginazione() throw()
{
	return(gCodiceImpaginazione);
} // PrendiCodiceImpaginazione

/* ------------------------------------------------------------------------ *

	InizializzaCodiceImpaginazione

* ------------------------------------------------------------------------ */
void XTAPI InizializzaCodiceImpaginazione() throw()
{
	// per prelevare i secondi della data corrente
	uint32 lSecs = 0;
	
	// prendo i secondi 
	lSecs = time(NULL);
	// imposto il codice di impaginazione
	gCodiceImpaginazione = lSecs;
} // InizializzaCodiceImpaginazione

/* ------------------------------------------------------------------------ *

	ImpostaCodiceImpaginazione

* ------------------------------------------------------------------------ */
void XTAPI ImpostaCodiceImpaginazione(uint32 codice) throw()
{
	// imposto il codice di impaginazione
	gCodiceImpaginazione = codice;
} // ImpostaCodiceImpaginazione

/* ------------------------------------------------------------------------ *

	RipristinaImpaginazione

* ------------------------------------------------------------------------ */
errorixtension XTAPI RipristinaDocumento() throw()
{
	// contiene il nome completo del documento dell'ultima impaginazione
	uchar lNomeDocUlmp[kDimensioneStringhe] = "";
	// per salvare il documento dell'ultima impaginazione
	uchar lNomeDocumento[80] = "";

	// leggo i dati dell'ultima impaginazione
	RipristinaDati();

	// chiudo tutti i documenti aperti in questo momento
	gErroreXtension = ChiudiTuttiIDocumenti(FALSE);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nella chiusura dei documenti aperti
		return(gErroreXtension);
	}
	
	// costruisco il nome completo del documento dell'ultima impaginazione
	STRCPY(lNomeDocUlmp, gXtPreferences.cartellePref.cartellaLastImp);
	STRCAT(lNomeDocUlmp, "\\");
	STRCAT(lNomeDocUlmp, gUltimaImpaginazione);
	
	// apro il documento dell'ultima impaginazione
	gErroreXtension = ApriDocumento(lNomeDocUlmp);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nell'apertura del documento dell'ultima impaginazione
		return(gErroreXtension);
	}
	
	// prendo il codice di impaginazione del documento aperto
	ImpostaCodiceImpaginazione(PrendiCodiceDocumento());

	// costruisco il nome del documento
	sprintf((char*)lNomeDocumento,"%s%s", PrendiDataXNomeFile(), ".QXP");

	// salvataggio documento appena aperto
	gErroreXtension = SalvaDocumento(gXtPreferences.finalePref.folderPathFinale, lNomeDocumento);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sul savataggio del documento definitivo
		return(gErroreXtension);
	}
	
	// imposto le coordinate prima colonna prima pagina
	ImpostaCoordinateIniziali();
	
	// leggo le impostazioni per il filetto di fine colonna
	ImpostaCaratteristicheFiletto(gStrStileFiletto);

	// stampo l'ingombro dell'ultima impaginazione
	StampaIngombro();

	return(kNessunErrore);
} // RipristinaDocumento

/* ------------------------------------------------------------------------ *

	RipristinaDati
	
* ------------------------------------------------------------------------ */
errorixtension XTAPI  RipristinaDati() throw()
{
	// imposto a zero l'ingombro totale
	InizializzaIngombroTotale();
	
	// imposto a zero lo spazio tra i necrologi totale
	InizializzaSpazioTraNecrologiTotale();
	
	// imposto a zero l'allargamento totale
	InizializzaAllargamentoTotale();
	
	// imposto a zero il restringimenyo totale
	InizializzaRestringimentoTotale();
	
	// leggo le info sui necrologi dell'ultima impaginazione
	gErroreXtension = LeggiInfoNecrologi();
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nella lettura dei dati dell'ultima impaginazione
		return(gErroreXtension);
	}

	// disabilito il comando di rispristina documento
	DisabilitaMenu(RIPRISTINADOCMENUID);

	// disabilito il comando di rispristina dati
	DisabilitaMenu(RIPRISTINADATIULTIMAMENUID);

	// permetto di calcolare un nuovo posizionamento
	AbilitaMenu(CALCPOSMENUID);

	// disabilito il comando di posizionare in base al vecchio posizionamento
	DisabilitaMenu(POSIZIONAMENUID);
	
	return(kNessunErrore);

} // RipristinaDati
