/* ------------------------------------------------------------------------ *

	Coordinate.cpp

	11 Aprile 2005						Fabrizio

	Descrizione:
	Contiene la descrizione delle procedure per la gestione delle coordinate
	per eseguire il posizionamento in pagina dei box dei necrologi

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

#include <cassert>

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "Errori.h"
#include "DocInfo.h"
#include "Pref.h"
#include "XTNecro.h"

#include "Coord.h"

// STATICS GLOBALS
// STATICS GLOBALS
// STATICS GLOBALS

static Fixed gCoordinataAlta = 0;
// angolo alto a sinistra corrente dove inserire il prossimo box 
static Fixed gCoordinataSinistra = 0;
// contatore della colonna corrente di impaginazione
static int16 gColonnaCorrente = 0;
// contatore della pagina corrente di impaginazione
static int16 gPaginaCorrente = 0;
// per calcolare l'ingombro totale
static uint32 gIngombroTotale = 0;
// per calcolare lo spazio tra i necrologi totali
static uint32 gSpazioTraNecrologiTotale = 0;
// contatore dei box nella colonna corrente
static int16 gContatoreBoxInColonna = 0;

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	ImpostaCoordinateIniziali

* ------------------------------------------------------------------------ */
void XTAPI ImpostaCoordinateIniziali() throw()
{
	gPaginaCorrente = 1;
	gColonnaCorrente = 1;
	gContatoreBoxInColonna = 0;
	gotopage(gPaginaCorrente);
	
	// crea la struttura che contiene le coordinate alte e basse di ogni colonna
	// senza preoccuparsi di eventuali ingombri
	gCoordinataSinistra = PrendiMargineSinistro();
	gCoordinataAlta = PrendiMargineAlto();
	xtset_curbox(NULL);
} // ImpostaCoordinateIniziali

/* ------------------------------------------------------------------------ *

	ImpostaCoordinateInizialiDefinitivo

* ------------------------------------------------------------------------ */
void XTAPI ImpostaCoordinateInizialiDefinitivo() throw()
{
	gPaginaCorrente = 1;
	gColonnaCorrente = 1;
	gContatoreBoxInColonna = 0;
	gotopage(gPaginaCorrente);
	
	// crea la struttura che contiene le coordinate alte e basse di ogni colonna
	// senza preoccuparsi di eventuali ingombri
	gCoordinataSinistra = PrendiMargineSinistro() - PrendiLarghezzaColonna() * 2;
	gCoordinataAlta = PrendiMargineAlto();
	xtset_curbox(NULL);
} // ImpostaCoordinateInizialiDefinitivo

/* ------------------------------------------------------------------------ *

	ImpostaCoordinateBoxImmagineCampione

* ------------------------------------------------------------------------ */
void XTAPI ImpostaCoordinateBoxImmagineCampione() throw()
{
	gPaginaCorrente = 1;
	gColonnaCorrente = 1;
	gContatoreBoxInColonna = 0;
	gotopage(gPaginaCorrente);

	// crea la struttura che contiene le coordinate alte e basse di ogni colonna
	// senza preoccuparsi di eventuali ingombri
	gCoordinataSinistra = PrendiMargineDestro() + PrendiLarghezzaColonna() * 2;
	gCoordinataAlta = PrendiMargineAlto();
	xtset_curbox(NULL);
} // ImpostaCoordinateBoxImmagineCampione

/* ------------------------------------------------------------------------ *

	InizializzaIngombroTotale

* ------------------------------------------------------------------------ */
void XTAPI InizializzaIngombroTotale() throw()
{
	gIngombroTotale = 0;
} // InizializzaIngombroTotale

/* ------------------------------------------------------------------------ *

	IncrementaIngombroTotale

* ------------------------------------------------------------------------ */
void XTAPI IncrementaIngombroTotale(Fixed ingombro) throw()
{
	gIngombroTotale += mLong(ingombro);
} // IncrementaIngombroTotale

/* ------------------------------------------------------------------------ *

	ImpostaIngombroTotale

* ------------------------------------------------------------------------ */
void XTAPI ImpostaIngombroTotale(uint32 ingombro) throw()
{
	gIngombroTotale = ingombro;
} // ImpostaIngombroTotale

/* ------------------------------------------------------------------------ *

	PrendiIngombroTotale

* ------------------------------------------------------------------------ */
uint32 PrendiIngombroTotale() throw()
{
	return(gIngombroTotale);
} // PrendiIngombroTotale

/* ------------------------------------------------------------------------ *

	InizializzaSpazioTraNecrologiTotale

* ------------------------------------------------------------------------ */
void XTAPI InizializzaSpazioTraNecrologiTotale() throw()
{
	gSpazioTraNecrologiTotale = 0;
} // InizializzaSpazioTraNecrologiTotale

/* ------------------------------------------------------------------------ *

	IncrementaSpazioTraNecrologiTotale
	
* ------------------------------------------------------------------------ */
void XTAPI IncrementaSpazioTraNecrologiTotale(Fixed spaziotranecrologi) throw()
{
	gSpazioTraNecrologiTotale += mLong(spaziotranecrologi);
} // IncrementaSpazioTraNecrologiTotale


/* ------------------------------------------------------------------------ *

	ImpostaSpazioTraNecrologiTotale
	
* ------------------------------------------------------------------------ */
void XTAPI ImpostaSpazioTraNecrologiTotale(uint32 spaziotranecrologi) throw()
{
	gSpazioTraNecrologiTotale = spaziotranecrologi;
} // ImpostaSpazioTraNecrologiTotale

/* ------------------------------------------------------------------------ *

	PrendiSpazioTraNecrologiTotale

* ------------------------------------------------------------------------ */
uint32 XTAPI PrendiSpazioTraNecrologiTotale() throw()
{
	return(gSpazioTraNecrologiTotale);
} // PrendiSpazioTraNecrologiTotale

/* ------------------------------------------------------------------------ *

	PrendiCoordinataAlta

* ------------------------------------------------------------------------ */
Fixed XTAPI PrendiCoordinataAlta() throw()
{
	return(gCoordinataAlta);
} // PrendiCoordinataAlta

/* ------------------------------------------------------------------------ *

	PrendiCoordinataSinistra

* ------------------------------------------------------------------------ */
Fixed XTAPI PrendiCoordinataSinistra() throw()
{
	return(gCoordinataSinistra);
} // PrendiCoordinataSinistra

/* ------------------------------------------------------------------------ *

	PrendiPaginaCorrente
	
* ------------------------------------------------------------------------ */
int16 PrendiPaginaCorrente() throw()
{
	return(gPaginaCorrente);
} // PrendiPaginaCorrente

/* ------------------------------------------------------------------------ *

	PrendiColonnaCorrente

* ------------------------------------------------------------------------ */
int16 XTAPI PrendiColonnaCorrente() throw()
{
	return(gColonnaCorrente);
} // PrendiColonnaCorrente

/* ------------------------------------------------------------------------ *

	PrendiContatoreBoxInColonna

* ------------------------------------------------------------------------ */
int16 XTAPI PrendiContatoreBoxInColonna() throw()
{
	return(gContatoreBoxInColonna);
} // PrendiContatoreBoxInColonna

/* ------------------------------------------------------------------------ *

	IncrementaContatoreBoxInColonna

* ------------------------------------------------------------------------ */
void XTAPI IncrementaContatoreBoxInColonna() throw()
{
	gContatoreBoxInColonna++;
} // IncrementaContatoreBoxInColonna

/* ------------------------------------------------------------------------ *

	AggiornaCoordinate

* ------------------------------------------------------------------------ */
void XTAPI AggiornaCoordinate() throw()
{
	// per prendere il box corrente
	xtboxptr lPtrBoxTesto = NULL;
	// per prendere le coordinate del box corrente
	pagenumdata lDatiPagina = {0, 0, 0, 0, 0};
	
	// e' stato impaginato un box
	// incremento la coordinata alta dell'altezza del box
	lPtrBoxTesto = getxtbox(NULL, NULL, FALSE);
	sprdfrect2page(&lPtrBoxTesto->box.s.r.fr);
	
	gCoordinataAlta = lPtrBoxTesto->box.s.r.fr.bottom;
	
	disposextbox((xtboxptr ) lPtrBoxTesto, TRUE);
} // AggiornaCoordinate

/* ------------------------------------------------------------------------ *

	CambiaCoordinate
	
* ------------------------------------------------------------------------ */
tipospostamento XTAPI CambiaCoordinate() throw()
{
	tipospostamento lRisultato = kNuovaColonna;
	
	// incremento il contatore delle colonne
	gColonnaCorrente++;
	// inizializzo il contatore del box nella colonna
	gContatoreBoxInColonna = 1;
	
	// controllo se devo andare su una nuova pagina
	if (gColonnaCorrente > PrendiNumeroColonne()) 
	{
		// aggiorno il contatore delle colonne
		gColonnaCorrente = 1;
		// devo andare su una nuova pagina 
		gPaginaCorrente++;

		// controllo se devo inserire questa pagina
		if (gPaginaCorrente > numpages()) 
		{
			// inserisco la nuova pagina al fondo del documento corrente
			// applicando la prima pagina mastro e senza collegare
			// il flusso di testo
			// nsertpages2(numpages(), 1, 8, NULL, 3);
		
			DocID curDocID = INVALDOC;
 			DocRef DRef;
 			xtget_curdoc(&curDocID);
 			DRef = XTGetDocRefFromDocID(curDocID);
			XTInsertPages(DRef, numpages(), 1, NOCALC, NULL, 3);
		}
		gotopage(gPaginaCorrente);
		// imposto il flag che dice che ho appena iniziato una nuova pagina
		// nell'aggiornare le coordinate ho cambiato pagina
		lRisultato = kNuovaPagina;
	}
	
	// aggiorno le coordinate di pagina
	gCoordinataSinistra = PrendiMargineSinistro() +
						  (gColonnaCorrente - 1) * PrendiLarghezzaColonna() +
						  (gColonnaCorrente - 1) * PrendiDistanzaColonne();
	gCoordinataAlta = PrendiMargineAlto();
	
	return(lRisultato);							  
} // CambiaCoordinate

/* ------------------------------------------------------------------------ *

	InserisciNuovaPagina

* ------------------------------------------------------------------------ */
void XTAPI InserisciNuovaPagina() throw()
{
	// aggiorno il contatore delle colonne
	gColonnaCorrente = 1;
	// inizializzo il contatore del box nella colonna
	gContatoreBoxInColonna = 0;
	// devo andare su una nuova pagina
	gPaginaCorrente++;
	// controllo se devo inserire questa pagina
	if (gPaginaCorrente > numpages()) 
	{
		// inserisco la nuova pagina al fondo del documento corrente
		// applicando la prima pagina mastro e senza collegare
		// il flusso di testo
		// insertpages2(numpages(), 1, 8, NULL, 3);
		
		DocID curDocID = INVALDOC;
 		DocRef DRef;
 		xtget_curdoc(&curDocID);
 		DRef = XTGetDocRefFromDocID(curDocID);
		XTInsertPages(DRef, numpages(), 1, NOCALC, NULL, 3);
	}
	gotopage(gPaginaCorrente);
	gCoordinataSinistra = PrendiMargineSinistro();
	gCoordinataAlta = PrendiMargineAlto();
} // InserisciNuovaPagina

/* ------------------------------------------------------------------------ *

	DecrementaColonna

* ------------------------------------------------------------------------ */
void XTAPI DecrementaColonna() throw()
{
	gCoordinataSinistra -= (PrendiLarghezzaColonna() + PrendiDistanzaColonne());
} // DecrementaColonna

/* ------------------------------------------------------------------------ *

	IncrementaColonna

* ------------------------------------------------------------------------ */
void XTAPI IncrementaColonna() throw()
{
	gColonnaCorrente++;
	gCoordinataSinistra = PrendiMargineSinistro() +
						  (gColonnaCorrente - 1) * PrendiLarghezzaColonna() +
						  (gColonnaCorrente - 1) * PrendiDistanzaColonne();
} // IncrementaColonna

/* ------------------------------------------------------------------------ *

	PrimaColonnaIngombro

* ------------------------------------------------------------------------ */
void XTAPI PrimaColonnaIngombro() throw()
{
	gColonnaCorrente = 1;
	gCoordinataSinistra = PrendiMargineSinistro();
} // PrimaColonnaIngombro

/* ------------------------------------------------------------------------ *

	PrimaColonnaDefinitivo

* ------------------------------------------------------------------------ */
void XTAPI PrimaColonnaDefinitivo() throw()
{
	gColonnaCorrente = 1;
	gCoordinataSinistra = PrendiMargineSinistro() - PrendiLarghezzaColonna() * 2;
} // PrimaColonnaDefinitivo
