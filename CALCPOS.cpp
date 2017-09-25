/* ------------------------------------------------------------------------ *

	CalcoloPosizionamento.cpp

	13 Aprile 2005						Fabrizio

	Descrizione:
	Contiene le procedure per il calcolo del posizionamento dei necrologi

	$Log: not supported by cvs2svn $
	Revision 1.2  2005/05/02 10:03:13  taretto
	avanzamento lavori porting
	
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

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "AppWare.h"
#include "DocInfo.h"
#include "Document.h"
#include "Errori.h"
#include "GesSlug.h"
#include "Impag.h"
#include "InfoNec.h"
#include "LastImp.h"
#include "Memoria.h"
#include "Menu.h"
#include "Paletta.h"
#include "Pref.h"
#include "PresFile.h"
#include "ReadFile.h"
#include "ResStr.h"
#include "RiordDef.h"
#include "RiordNec.h"
#include "Ripristi.h"
#include "SpaceAva.h"
#include "SpaceCol.h"
#include "StMes.h"
#include "Tempo.h"

#include "CalcPos.h"

// GLOBALS
// GLOBALS
// GLOBALS

// spazio rimanete per fare il riordino dei necrologi
Fixed gSpazioRimanente = 0;

// STATICS GLOBALS
// STATICS GLOBALS
// STATICS GLOBALS

// per scorrere i defunti di una colonna 
static defunto *gPtrDefuntoSpazioColonna = NULL;
// per sapere se sono gia' entrato nei defunti della colonna
static Boolean gTestaDefunti = FALSE;
// per sapere se sono gia' entrato nei defunti della colonna
static Boolean gTestaFondoDefunti = FALSE;
// per sapere se sono gia' entrato nell'elenco dei necrologi A
static Boolean gElencoNecrologiA = FALSE;
// per sapere se sono gia' entrato nell'elenco dei necrologi B
static Boolean gElencoNecrologiB = FALSE;
// per sapere se sono gia' entrato nell'elenco dei necrologi C
static Boolean gElencoNecrologiC = FALSE;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			StampaSpazioDisponibile
	@abstract 		posizionamento annunci
	@discussion		Stampa dei messaggi dello spazio disponibile appena calcolato

					28 Aprile 2005 - Fabrizio.

	@param 			nessuno		
	@return			nessuno				
*/
static void XTAPI StampaSpazioDisponibile() throw();

/*!
	@function			ScegliColonnaOttimale
	@abstract 		posizionamento annunci
	@discussion		Sceglie la colonna migliore per il defunto passato

					28 Aprile 2005 - Fabrizio.

	@param 			ptrpagina - ritorna la pagina trovata
	@param			ptrcolonna - ritorna la colonna trovata		
	@return			ritorna l'eventuale errore dell'xtension				
*/
static errorixtension XTAPI ScegliColonnaOttimale(int16 *ptrpagina, int16 *ptrcolonna) throw();

/*!
	@function			PrendiPrimaColonnaLibera
	@abstract 		posizionamento annunci
	@discussion		Prende la prima colonna completamente libera a partire dalla colonna
					passata, tenendo conto che il defunto può essere più lungo di una
					pagina

					28 Aprile 2005 - Fabrizio.

	@param 			ptrpagina - ritorna la pagina trovata
	@param			ptrcolonna - ritorna la colonna trovata	
	@param			altezzadefunto - altezza del defunto da inserire nella pagina	
	@return			ritorna TRUE se la colonna e' stata trovata			
*/
static Boolean XTAPI PrendiPrimaColonnaLibera(int16 *ptrpagina, int16 *ptrcolonna, DoubleFixed altezzadefunto) throw();

/*!
	@function			PrendiColonnaLibera
	@abstract 		posizionamento annunci
	@discussion		Prende la prima colonna completamente libera a partire dalla colonna
					passata

					28 Aprile 2005 - Fabrizio.

	@param 			ptrpagina - ritorna la pagina trovata
	@param			ptrcolonna - ritorna la colonna trovata	
	@param			altezzadefunto - altezza del defunto da inserire nella pagina	
	@return			ritorna TRUE se la colonna e' stata trovata			
*/
static Boolean XTAPI PrendiColonnaLibera(int16 *ptrpagina, int16 *ptrcolonna, DoubleFixed altezzadefunto) throw();

/*!
	@function			PrendiPrimoNecrologioDefunto
	@abstract 		posizionamento annunci
	@discussion		Prende il primo necrologio del defunto

					28 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto - puntatore al defunto di cui prendere il necrologio	
	@return			ritorna il puntatore al necrologio		
*/
static necrologio* XTAPI PrendiPrimoNecrologioDefunto(defunto *ptrdefunto) throw();

/*!
	@function			PrendiNecrologioSuccessivoDefunto
	@abstract 		posizionamento annunci
	@discussion		Prende il necrologio successivo del defunto

					28 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto - puntatore al defunto di cui prendere il necrologio	
	@return			ritorna il puntatore al necrologio		
*/
static necrologio* XTAPI PrendiNecrologioSuccessivoDefunto(defunto *ptrdefunto) throw();

/*!
	@function			AggiungiSpazioColonna
	@abstract 		posizionamento annunci
	@discussion		Aggiorna la info della colonna nell'aggiugere un necrologio

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alla colonna in cui si vuole aggiungere
	@param			ptrnecrologio - puntatore al necrologio
	@return			nessuno		
*/
static void XTAPI AggiungiSpazioColonna(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw();

/*!
	@function			TogliSpazioColonna
	@abstract 		posizionamento annunci
	@discussion		Aggiorna la info della colonna nel togliere un necrologio dalla colonna

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alla colonna in cui si vuole rimuovere
	@param			ptrnecrologio - puntatore al necrologio 
	@return			nessuno		
*/
static void XTAPI TogliSpazioColonna(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw();

/*!
	@function			AggiungiDefuntiSpazioColonna
	@abstract 		posizionamento annunci
	@discussion		Aggiorna la info della colonna nell'aggiugere un necrologio dalla colonna che
					appartiene alla lista dei defunti della colonna

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alla colonna in cui si vuole rimuovere
	@param			ptrnecrologio - puntatore al necrologio da rimuovere
	@return			nessuno		
*/
static void XTAPI AggiungiDefuntiSpazioColonna(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw();

/*!
	@function			AggiungiFondoDefuntiSpazioColonna
	@abstract 		posizionamento annunci
	@discussion		Aggiorna la info della colonna nell'aggiugere un necrologio dalla colonna che
					appartiene alla lista dei defunti in fondo alla colonna

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alla colonna in cui si vuole rimuovere
	@param			ptrnecrologio - puntatore al necrologio da rimuovere
	@return			nessuno		
*/
static void XTAPI AggiungiFondoDefuntiSpazioColonna(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw();

/*!
	@function			TogliDefuntiSpazioColonna
	@abstract 		posizionamento annunci
	@discussion		Aggiorna la info della colonna nle togliere un necrologio dalla colonna che
					appartiene alla lista dei defunti nella colonna

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alla colonna in cui si vuole rimuovere
	@param			ptrnecrologio - puntatore al necrologio da rimuovere
	@return			nessuno		
*/
static void XTAPI TogliDefuntiSpazioColonna(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw();

/*!
	@function			TogliFondoDefuntiSpazioColonna
	@abstract 		posizionamento annunci
	@discussion		Aggiorna la info della colonna nle togliere un necrologio dalla colonna che
					appartiene alla lista dei defunti nella colonna

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alla colonna in cui si vuole rimuovere
	@param			ptrnecrologio - puntatore al necrologio da rimuovere
	@return			nessuno		
*/
static void XTAPI TogliFondoDefuntiSpazioColonna(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw();

/*!
	@function			InserisciListaNecrologi
	@abstract 		posizionamento annunci
	@discussion		Inserisce nelle colonne del documento la lista di necrologi passata

					28 Aprile 2005 - Fabrizio.

	@param 			ptrpagina - puntatore alla pagina corrente
	@param			ptrnecrologi - puntatore alla lista di necrologi da inserire
	@param			ptrcolonna - puntatore alla colonna corrente
	@return			ritorna l'eventuale errore dell'xtension		
*/
static errorixtension XTAPI InserisciListaNecrologi(int16 *ptrpagina, int16 *ptrcolonna, necrologio *ptrnecrologi) throw();

/*!
	@function			InserisciDefuntiFineDocumento
	@abstract 		posizionamento annunci
	@discussion		Scorre le colonne che devono essere ancora chiuse partendo dal fonsdo
					e vi inserisce i necrologi della lista dei defunti passata

					28 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto - puntatore alla lista dei defunti da inserire
	@param			ptrscarto - puntatore allo scarto della colonna corrente
	@return			ritorna l'eventuale errore dell'xtension		
*/
static errorixtension XTAPI InserisciDefuntiFineDocumento(defunto *ptrdefunto, Fixed *ptrscarto) throw();

/*!
	@function			InserisciDefuntiFineDocumento
	@abstract 		posizionamento annunci
	@discussion		Scorre le colonne che devono essere ancora chiuse e vi inserisce i necrologi della
					lista dei defunbti passata

					28 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto - puntatore alla lista dei defunti da inserire
	@param			ptrscarto - puntatore allo scarto della colonna corrente
	@return			ritorna l'eventuale errore dell'xtension		
*/
static errorixtension XTAPI InserisciDefuntiNelDocumento(defunto *ptrdefunto, Fixed *ptrscarto) throw();

/*!
	@function			EseguiCalcoloPosizionamento
	@abstract 		posizionamento annunci
	@discussion		Inserisce i necrologi ottimizzando lo spazio occupato

					28 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@return			ritorna l'eventuale errore dell'xtension		
*/
static errorixtension XTAPI EseguiCalcoloPosizionamento() throw();

/*!
	@function			ControllaChiusuraColonne
	@abstract 		posizionamento annunci
	@discussion		Controlla la chiusura delle colonne

					28 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@return			nessuno		
*/
static void XTAPI ControllaChiusuraColonne() throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	PrendiUltimaColonna

* ------------------------------------------------------------------------ */
errorixtension XTAPI PrendiUltimaColonna(int16 *ptrpagina, int16 *ptrcolonna) throw()
{
	// indice per scorrere le pagine
	int16 lPagina = 0;
	// indice per scorrere le colonne
	int16 lColonna = 0;
	// per prendere le info dello spazio della colonna corrente
	spaziocolonna *lPtrSpazioColonna = NULL;
	// per sapere se e' stata trovata una colonna
	Boolean lColonnaTrovata = FALSE;
	
	for (lPagina = PrendiNumeroPagine(); lPagina >= 1; lPagina--) 
	{
		for (lColonna = PrendiNumeroColonne(); lColonna >= 1; lColonna--) 
		{
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);
			if ((*lPtrSpazioColonna).spazioOttimale > 0) 
			{
				lColonnaTrovata = TRUE;
				break;
			}
		}
		if (lColonnaTrovata == TRUE) break;
	}
	
	if (lColonnaTrovata == FALSE) 
	{
		// non c'e' una colonna con lo spazio libero 
		// non dovrebbe mai passare qua
		return(kErroreSceltaColonnaOttimale);
	} 
	else 
	{
		(*ptrpagina) = lPagina;
		(*ptrcolonna) = lColonna;
		return(kNessunErrore);
	}
} // PrendiUltimaColonna

/* ------------------------------------------------------------------------ *

	CosaFare

* ------------------------------------------------------------------------ */
cosafare XTAPI CosaFare(Fixed ingombro, Fixed spaziodisponibile, Fixed *differenza) throw()
{
	if (ingombro > spaziodisponibile) 
	{
		(*differenza) = ingombro - spaziodisponibile;
		return(kRestringere);
	} 
	else if (ingombro < spaziodisponibile) 
	{
		(*differenza) = spaziodisponibile - ingombro;
		return(kAllargare);
	} 
	else 
	{
		(*differenza) = 0;
		return(kNulla);
	}
} // CosaFare

/* ------------------------------------------------------------------------ *

	StampaSpazioDisponibile

* ------------------------------------------------------------------------ */
static void XTAPI StampaSpazioDisponibile() throw()
{
	uint32 lSpazioDisponibile = 0;
	uint32 lPagine = 0;
	uint32 lColonne = 0;
	uint32 lFrazioneModuli = 0;
	uint32 lModuli = 0;
	
	// prendo lo spazio disponibile 
	lSpazioDisponibile = PrendiSpazioDisponibileTotale();
	
	// calcolo pagine colonne e moduli 
	CalcolaPagineColonneEModuli(lSpazioDisponibile,
								&lPagine,
								&lColonne,
								&lFrazioneModuli,
								&lModuli);

	sprintf((char*)gStringaC, "%ld punti", lSpazioDisponibile);
	ConcatenaAllaStringaMessaggio(gStringaC);
	DaiMessaggio(kSpazioDisponibileInPunti);
	
	sprintf((char*)gStringaC, "%ld moduli", lModuli);
	ConcatenaAllaStringaMessaggio(gStringaC);
	DaiMessaggio(kSpazioDisponibileInModuli);

	if (lFrazioneModuli != 0) 
		sprintf((char*)gStringaC, "pagine %ld colonne %ld moduli %ld", lPagine, lColonne, lFrazioneModuli);
	else 
		sprintf((char*)gStringaC, "pagine %ld colonne %ld", lPagine, lColonne);	
	ConcatenaAllaStringaMessaggio(gStringaC);
	DaiMessaggio(kSpazioDisponibileInPagineColonne);
} // StampaSpazioDisponibile

/* ------------------------------------------------------------------------ *

	ScegliColonnaOttimale

* ------------------------------------------------------------------------ */
static errorixtension XTAPI ScegliColonnaOttimale(int16 *ptrpagina, int16 *ptrcolonna) throw()
{
	// indice per scorrere le pagine
	int16 lPagina = 0;
	// indice per scorrere le colonne 
	int16 lColonna = 0;
	spaziocolonna *lPtrSpazioColonna = NULL;
	// per ricordare la pagina trovata 
	int16 lPaginaTrovata = 0;
	// per ricordare la colonna trovata 
	int16 lColonnaTrovata = 0;
	// per memorizzare il massimo spazio lasciato libero 
	Fixed lMassimoSpazio = 0;
	// per calcolare la differenza tra spazio occupato considerando l'altezza del defunto 
	// e lo spazio disponibile 
	Fixed lDifferenza = 0;
	// per sapere se ho trovato la colonna 
	Boolean lTrovata = FALSE;

	for (lPagina = 1; lPagina <= PrendiNumeroPagine(); lPagina++) 
	{
		for (lColonna = 1; lColonna <= PrendiNumeroColonne(); lColonna++) 
		{
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);
			
			// controllo che la colonna sia utilizzabile 
			if ((*lPtrSpazioColonna).spazioDisponibile == 0) continue;
			if ((*lPtrSpazioColonna).chiusuraColonna == kChiusa) continue;
			
			lDifferenza = (*lPtrSpazioColonna).spazioDisponibile -
						   ((*lPtrSpazioColonna).spazioOccupato +
						  	(*lPtrSpazioColonna).spazioTraNecrologi);

			// significa che c'Š ancora spazio disponibile 
			if (lDifferenza > lMassimoSpazio || lTrovata == FALSE) 
			{
				lMassimoSpazio = lDifferenza;
				lPaginaTrovata = lPagina;
				lColonnaTrovata = lColonna;
				lTrovata = TRUE;
			}
		}
	}
	
	if (lTrovata == TRUE) 
	{
		// ho trovato la colonna 
		(*ptrpagina) = lPaginaTrovata;
		(*ptrcolonna) = lColonnaTrovata;
		
		return(kNessunErrore);
	} 
	else 
	{
		// non sono riuscito ha trovare una colonna disponibile 
		// arriva qui solo se sul documento tutte le colonne son non disponibili 
		// quindi mai 
		return(kErroreSceltaColonnaOttimale);
	}
} // ScegliColonnaOttimale

/* ------------------------------------------------------------------------ *

	PrendiPrimaColonnaLibera

* ------------------------------------------------------------------------ */
static Boolean XTAPI PrendiPrimaColonnaLibera(int16 *ptrpagina, int16 *ptrcolonna, DoubleFixed altezzadefunto) throw()
{
	// indice per scorrere le pagine
	int16 lPagina = 0;
	// indice per scorrere le colonne
	int16 lColonna = 0;
	// per prendere le info dello spazio della colonna corrente
	spaziocolonna *lPtrSpazioColonna = NULL;
	// per sapere se e' stata trovata una colonna
	Boolean lColonnaTrovata = FALSE;
	// per prendere lo spazio disponibile di pagina 
	Fixed lSpazioOttimaleDiPagina = 0;
	
	for (lPagina = 1; lPagina <= PrendiNumeroPagine(); lPagina++) 
	{
		for (lColonna = 1; lColonna <= PrendiNumeroColonne(); lColonna++) 
		{
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);
			
			// se sono stati inseriti gia' dei necrologi non e' colonna che cerco
			if (((*lPtrSpazioColonna).numeroNecrologi +
				(*lPtrSpazioColonna).numeroNecrologiDefunti +
				(*lPtrSpazioColonna).numeroNecrologiFondoDefunti) != 0) continue;

			// controllo lo disponibile
			if ((*lPtrSpazioColonna).spazioDisponibile == 0) continue;

			// controllo il margine alto della colonna
			if ((*lPtrSpazioColonna).margineAlto != PrendiMargineAlto()) continue;

			lSpazioOttimaleDiPagina = PrendiSpazioOttimaleDiPagina(lPagina, lColonna);

			if (lSpazioOttimaleDiPagina < altezzadefunto && lColonna != 1) continue;

			// la colonna corrente va bene
			lColonnaTrovata = TRUE;
			break;
		}
		if (lColonnaTrovata == TRUE) break;
	}

	if (lColonnaTrovata == FALSE) 
	{
		// non c'e' una colonna completamente libera
		return(FALSE);
	} 
	else 
	{
		(*ptrpagina) = lPagina;
		(*ptrcolonna) = lColonna;
		return(TRUE);
	}
} // PrendiPrimaColonnaLibera

/* ------------------------------------------------------------------------ *

	PrendiColonnaLibera

* ------------------------------------------------------------------------ */
static Boolean XTAPI PrendiColonnaLibera(int16 *ptrpagina, int16 *ptrcolonna, DoubleFixed altezzadefunto) throw()
{
	// indice per scorrere le pagine
	int16 lPagina = 0;
	// indice per scorrere le colonne
	int16 lColonna = 0;
	// per prendere le info dello spazio della colonna corrente
	spaziocolonna *lPtrSpazioColonna = NULL;
	// per sapere se e' stata trovata una colonna
	Boolean lColonnaTrovata = FALSE;
	// per prendere lo spazio disponibile di pagina
	Fixed lSpazioOttimaleDiPagina = 0;
	
	for (lPagina = 1; lPagina <= PrendiNumeroPagine(); lPagina++) 
	{
		for (lColonna = 1; lColonna <= PrendiNumeroColonne(); lColonna++) 
		{
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);

			// se sono stati inseriti gia' dei necrologi non e' colonna che cerco
			if (((*lPtrSpazioColonna).numeroNecrologi +
				(*lPtrSpazioColonna).numeroNecrologiDefunti +
				(*lPtrSpazioColonna).numeroNecrologiFondoDefunti) != 0) continue;

			// controllo lo disponibile
			if ((*lPtrSpazioColonna).spazioDisponibile == 0) continue;

			// controllo il margine alto della colonna
			if ((*lPtrSpazioColonna).margineAlto != PrendiMargineAlto()) continue;
			
			lSpazioOttimaleDiPagina = PrendiSpazioOttimaleDiPagina(lPagina, lColonna);
			
			if (lSpazioOttimaleDiPagina < altezzadefunto) continue;

			// la colonna corrente va bene
			lColonnaTrovata = TRUE;
			break;
		}
		if (lColonnaTrovata == TRUE) 
			break;
	}

	if (lColonnaTrovata == FALSE) 
	{
		// non c'e' una colonna completamente libera
		return(FALSE);
	} 
	else 
	{
		(*ptrpagina) = lPagina;
		(*ptrcolonna) = lColonna;
		return(TRUE);
	}
} // PrendiColonnaLibera

/* ------------------------------------------------------------------------ *

	PrendiPrimoNecrologioDefunto
	
* ------------------------------------------------------------------------ */
static necrologio* XTAPI PrendiPrimoNecrologioDefunto(defunto *ptrdefunto) throw()
{
	// per ritornare il puntatore al necrologio
	necrologio *lPtrNecrologio = NULL;
	
	// inizializzo i flag di scorrimento
	gElencoNecrologiA = FALSE;
	gElencoNecrologiB = FALSE;
	gElencoNecrologiC = FALSE;
	
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiA;
	gElencoNecrologiA = TRUE;
	if (lPtrNecrologio != NULL) 
	{
		return(lPtrNecrologio);
	}
	
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiB;
	gElencoNecrologiB = TRUE;
	if (lPtrNecrologio != NULL) 
	{
		return(lPtrNecrologio);
	}
	
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiC;
	gElencoNecrologiC = TRUE;
	if (lPtrNecrologio != NULL) 
	{
		return(lPtrNecrologio);
	}
	return(NULL);
} // PrendiPrimoNecrologioDefunto

/* ------------------------------------------------------------------------ *

	PrendiNecrologioSuccessivoDefunto

* ------------------------------------------------------------------------ */
static necrologio* XTAPI PrendiNecrologioSuccessivoDefunto(defunto *ptrdefunto) throw()
{
	// per ritornare il puntatore al necrologio
	necrologio *lPtrNecrologio = NULL;
	
	if (gElencoNecrologiA == FALSE) 
	{
		gElencoNecrologiA = TRUE;
		lPtrNecrologio = (*ptrdefunto).elencoNecrologiA;
		if (lPtrNecrologio != NULL) 
		{
			return(lPtrNecrologio);
		}
	}
	
	if (gElencoNecrologiB == FALSE) 
	{
		gElencoNecrologiB = TRUE;
		lPtrNecrologio = (*ptrdefunto).elencoNecrologiB;
		if (lPtrNecrologio != NULL) 
		{
			return(lPtrNecrologio);
		}
	}
	
	if (gElencoNecrologiC == FALSE) 
	{
		gElencoNecrologiC = TRUE;
		lPtrNecrologio = (*ptrdefunto).elencoNecrologiC;
		if (lPtrNecrologio != NULL) 
		{
			return(lPtrNecrologio);
		}
	}
	
	return(NULL);
} // PrendiNecrologioSuccessivoDefunto

/* ------------------------------------------------------------------------ *

	PrendiPrimoNecrologioSpazioColonna

* ------------------------------------------------------------------------ */
necrologio* XTAPI PrendiPrimoNecrologioSpazioColonna(spaziocolonna *ptrspaziocolonna) throw()
{
	// puntatore al necrologio da restituire
	necrologio *lPtrNecrologio = NULL;
	
	gPtrDefuntoSpazioColonna = NULL;
	gTestaDefunti = FALSE;
	gTestaFondoDefunti = FALSE;

	lPtrNecrologio = (*ptrspaziocolonna).testaNecrologi;
	if (lPtrNecrologio != NULL) 
	{
		return(lPtrNecrologio);
	}
	
	gPtrDefuntoSpazioColonna = (*ptrspaziocolonna).testaDefunti;
	gTestaDefunti = TRUE;
	if (gPtrDefuntoSpazioColonna != NULL) 
	{
		lPtrNecrologio = PrendiPrimoNecrologioDefunto(gPtrDefuntoSpazioColonna);
		if (lPtrNecrologio != NULL) 
		{
			return(lPtrNecrologio);
		}
	}
	
	gPtrDefuntoSpazioColonna = (*ptrspaziocolonna).testaFondoDefunti;
	gTestaFondoDefunti = TRUE;
	if (gPtrDefuntoSpazioColonna != NULL) 
	{
		lPtrNecrologio = PrendiPrimoNecrologioDefunto(gPtrDefuntoSpazioColonna);
		if (lPtrNecrologio != NULL) 
		{
			return(lPtrNecrologio);
		}
	}

	return(NULL);
} // PrendiPrimoNecrologioSpazioColonna

/* ------------------------------------------------------------------------ *

	PrendiNecrologioSuccessivoSpazioColonna

* ------------------------------------------------------------------------ */
necrologio* XTAPI PrendiNecrologioSuccessivoSpazioColonna(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw()
{
	// puntatore al necrologio da restituire
	necrologio *lPtrNecrologio = NULL;
	
	lPtrNecrologio = (*ptrnecrologio).successivo;
	if (lPtrNecrologio != NULL) 
	{
		return(lPtrNecrologio);
	} 
	else 
	{
		if (gTestaDefunti == TRUE || gTestaFondoDefunti == TRUE) 
		{
			lPtrNecrologio = PrendiNecrologioSuccessivoDefunto(gPtrDefuntoSpazioColonna);
			if (lPtrNecrologio != NULL) 
			{
				return(lPtrNecrologio);
			}
		}
	}
	
	if (gTestaDefunti == FALSE) 
	{
		gPtrDefuntoSpazioColonna = (*ptrspaziocolonna).testaDefunti;
		if (gPtrDefuntoSpazioColonna != NULL) 
		{
			gTestaDefunti = TRUE;
			lPtrNecrologio = PrendiPrimoNecrologioDefunto(gPtrDefuntoSpazioColonna);
			if (lPtrNecrologio != NULL) 
			{
				return(lPtrNecrologio);
			}
		}
	} 
	else 
	{
		if (gPtrDefuntoSpazioColonna != NULL) 
		{
			gPtrDefuntoSpazioColonna = (*gPtrDefuntoSpazioColonna).successivo;
			if (gPtrDefuntoSpazioColonna != NULL) 
			{
				lPtrNecrologio = PrendiPrimoNecrologioDefunto(gPtrDefuntoSpazioColonna);
				if (lPtrNecrologio != NULL) 
				{
					return(lPtrNecrologio);
				}
			}
		}
	}
	
	if (gTestaFondoDefunti == FALSE) 
	{
		gPtrDefuntoSpazioColonna = (*ptrspaziocolonna).testaFondoDefunti;
		if (gPtrDefuntoSpazioColonna != NULL) 
		{
			gTestaFondoDefunti = TRUE;
			lPtrNecrologio = PrendiPrimoNecrologioDefunto(gPtrDefuntoSpazioColonna);
			if (lPtrNecrologio != NULL) 
			{
				return(lPtrNecrologio);
			}
		}
	} 
	else 
	{
		if (gPtrDefuntoSpazioColonna != NULL) 
		{
			gPtrDefuntoSpazioColonna = (*gPtrDefuntoSpazioColonna).successivo;
			if (gPtrDefuntoSpazioColonna != NULL) 
			{
				lPtrNecrologio = PrendiPrimoNecrologioDefunto(gPtrDefuntoSpazioColonna);
				if (lPtrNecrologio != NULL) 
				{
					return(lPtrNecrologio);
				}
			}
		}
	}
	
	return(NULL);
} // PrendiNecrologioSuccessivoSpazioColonna

/* ------------------------------------------------------------------------ *

	ImpostaColonnaPrecedente

* ------------------------------------------------------------------------ */
errorixtension XTAPI ImpostaColonnaPrecedente(int16 *ptrpagina, int16 *ptrcolonna) throw()
{
	// devo passare alla colonna precedente
	if ((*ptrcolonna) > 1) 
	{
		// non sono sulla prima colonna
		(*ptrcolonna)--;
	} 
	else 
	{
		// sono sulla prima colonna
		(*ptrcolonna) = PrendiNumeroColonne();
		if ((*ptrpagina) > 1) 
		{
			// non sono sulla prima pagina pagina
			(*ptrpagina)--;
		} 
		else 
		{
			// errore nel decremenatre le colonne
			return(kImpossibileDecrementareColonne);
		}
	} 
	
	return(kNessunErrore);
} // ImpostaColonnaPrecedente

/* ------------------------------------------------------------------------ *

	ImpostaColonnaSuccessiva

* ------------------------------------------------------------------------ */
errorixtension XTAPI ImpostaColonnaSuccessiva(int16 *ptrpagina, int16 *ptrcolonna) throw()
{
	// devo passare alla colonna successiva
	if ((*ptrcolonna) < PrendiNumeroColonne()) 
	{
		// non sono sull'ultima colonna
		(*ptrcolonna)++;
	} 
	else 
	{
		/* sono sull'ultima colonna */
		(*ptrcolonna) = 1;
		if ((*ptrpagina) < PrendiNumeroPagine()) 
		{
			// non sono sull'ultima pagina
			(*ptrpagina)++;
		} 
		else 
		{
			// errore nell'incrementare le colonne
			return(kImpossibileIncrementareColonne);
		}
	} 
	
	return(kNessunErrore);
} // ImpostaColonnaSuccessiva

/* ------------------------------------------------------------------------ *

	CercaPrimaColonnaDaChiudere

* ------------------------------------------------------------------------ */
errorixtension XTAPI CercaPrimaColonnaDaChiudere(int16 *ptrpagina, int16 *ptrcolonna) throw()
{
	// indice per scorrere le pagine
	int16 lPagina = 0;
	// indice per scorrere le colonne
	int16 lColonna = 0;
	// per prendere lo spazio colonna
	spaziocolonna *lPtrSpazioColonna = NULL;
	
	lColonna = (*ptrcolonna);

	for (lPagina = (*ptrpagina) ; lPagina <= PrendiNumeroPagine(); lPagina++) 
	{
		for (lColonna = lColonna; lColonna <= PrendiNumeroColonne(); lColonna++) 
		{
			// prendo le info della colonna corrente 
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);
			
			if ((*lPtrSpazioColonna).spazioOttimale > 0 &&
				(*lPtrSpazioColonna).chiusuraColonna == kAperta) break;
		}
		if ((*lPtrSpazioColonna).spazioOttimale > 0 &&
			(*lPtrSpazioColonna).chiusuraColonna == kAperta ) break;
		lColonna = 1;
	}
	
	// ho trovato la colonna
	if ((*lPtrSpazioColonna).spazioOttimale > 0 &&
		(*lPtrSpazioColonna).chiusuraColonna == kAperta) 
	{
		// ritorno  la colonna trovata
		(*ptrpagina) = lPagina;
		(*ptrcolonna) = lColonna;
			
		return(kNessunErrore);
	} 
	else 
	{
		// errore nell'incrementare le colonne
		return(kImpossibileIncrementareColonne);
	}
} // CercaPrimaColonnaDaChiudere

/* ------------------------------------------------------------------------ *

	CercaUltimaColonnaDaChiudere

* ------------------------------------------------------------------------ */
errorixtension XTAPI CercaUltimaColonnaDaChiudere(int16 *ptrpagina, int16 *ptrcolonna) throw()
{
	// indice per scorrere le pagine
	int16 lPagina = 0;
	// indice per scorrere le colonne
	int16 lColonna = 0;
	// per prendere lo spazio colonna
	spaziocolonna *lPtrSpazioColonna = NULL;
	
	lColonna = (*ptrcolonna);

	for (lPagina = (*ptrpagina) ; lPagina >= 1; lPagina--) 
	{
		for (lColonna = lColonna; lColonna >= 1; lColonna--) 
		{
			// prendo le info della colonna corrente
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);

			if ((*lPtrSpazioColonna).spazioOttimale > 0 &&
				(*lPtrSpazioColonna).chiusuraColonna == kAperta) break;
		}
		if ((*lPtrSpazioColonna).spazioOttimale > 0 &&
			(*lPtrSpazioColonna).chiusuraColonna == kAperta) break;
		lColonna = PrendiNumeroColonne();
	}

	// ho trovato la colonna
	if ((*lPtrSpazioColonna).spazioOttimale > 0 &&
		(*lPtrSpazioColonna).chiusuraColonna == kAperta) 
	{

		// ritorno  la colonna trovata
		(*ptrpagina) = lPagina;
		(*ptrcolonna) = lColonna;

		return(kNessunErrore);
	} 
	else 
	{
		// errore nell'incrementare le colonne
		return(kImpossibileDecrementareColonne);
	}
} // CercaUltimaColonnaDaChiudere

/* ------------------------------------------------------------------------ *

	AggiungiSpazioColonna

* ------------------------------------------------------------------------ */
static void XTAPI AggiungiSpazioColonna(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw()
{
	(*ptrspaziocolonna).spazioOccupato += (*ptrnecrologio).altezzaBox;
	if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE)
		(*ptrspaziocolonna).spazioTraNecrologi += kFixedMinimoSpazioTraNecrologi;
	else 
		(*ptrspaziocolonna).spazioTraNecrologi += gXtPreferences.giustificazionePref.valoreIniziale;
	(*ptrspaziocolonna).spazioTraParagrafi += (*ptrnecrologio).spazioTraParagrafi;
	(*ptrspaziocolonna).numeroParagrafiDiversi += (*ptrnecrologio).numeroParagrafiDiversi;
	(*ptrspaziocolonna).numeroParagrafi += (*ptrnecrologio).numeroParagrafi;
	(*ptrspaziocolonna).numeroLinee += (*ptrnecrologio).numeroLinee;
} // AggiungiSpazioColonna

/* ------------------------------------------------------------------------ *

	TogliSpazioColonna

* ------------------------------------------------------------------------ */
static void XTAPI TogliSpazioColonna(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw()
{
	(*ptrspaziocolonna).spazioOccupato -= (*ptrnecrologio).altezzaBox;
	if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE)
		(*ptrspaziocolonna).spazioTraNecrologi -= kFixedMinimoSpazioTraNecrologi;
	else (*ptrspaziocolonna).spazioTraNecrologi -= gXtPreferences.giustificazionePref.valoreIniziale;
	(*ptrspaziocolonna).spazioTraParagrafi -= (*ptrnecrologio).spazioTraParagrafi;
	(*ptrspaziocolonna).numeroParagrafiDiversi -= (*ptrnecrologio).numeroParagrafiDiversi;
	(*ptrspaziocolonna).numeroParagrafi -= (*ptrnecrologio).numeroParagrafi;
	(*ptrspaziocolonna).numeroLinee -= (*ptrnecrologio).numeroLinee;
}

/* ------------------------------------------------------------------------ *

	InserisciNecrologioInCoda

* ------------------------------------------------------------------------ */
void XTAPI InserisciNecrologioInCoda(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw()
{
	// controllo se non e' stato ancora inserito nessun necrologio
	if ((*ptrspaziocolonna).numeroNecrologi == 0 &&
		(*ptrspaziocolonna).numeroNecrologiDefunti == 0 &&
		(*ptrspaziocolonna).numeroNecrologiFondoDefunti == 0) 
	{
		// e' il primo necrologio
		(*ptrspaziocolonna).numeroNecrologi = 1;
		(*ptrspaziocolonna).testaNecrologi = ptrnecrologio;
		(*ptrspaziocolonna).codaNecrologi = ptrnecrologio;
		(*ptrnecrologio).successivo = NULL;
		
	} 
	else 
	{	
		// ci sono gia' dei necrologi in pagina
		if ((*ptrspaziocolonna).numeroNecrologi == 0) 
		{
			(*ptrspaziocolonna).numeroNecrologi = 1;
			(*ptrspaziocolonna).testaNecrologi = ptrnecrologio;
			(*ptrspaziocolonna).codaNecrologi = ptrnecrologio;
		} 
		else 
		{
			(*ptrspaziocolonna).numeroNecrologi++;
			(*(*ptrspaziocolonna).codaNecrologi).successivo = ptrnecrologio;
			(*ptrspaziocolonna).codaNecrologi = ptrnecrologio;
		}

		(*ptrnecrologio).successivo = NULL;

	}
	AggiungiSpazioColonna(ptrspaziocolonna, ptrnecrologio);
} // InserisciNecrologioInCoda

/* ------------------------------------------------------------------------ *

	RimuoviNecrologio

* ------------------------------------------------------------------------ */
void XTAPI RimuoviNecrologio(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw()
{
	// per scorrere la lista
	necrologio *lPtrNecrologio = NULL;
	// per tenere il puntatore al necrologio precedente
	necrologio *lPtrNecrologioPrecedente = NULL;
	// per tenere il puntatore del campo successivo del necrologio precedente
	necrologio **lPtrNecrologioSuccessivo = NULL;
	
	// imposto il puntatore alla lista dei necrologi nella colonna
	lPtrNecrologio = (*ptrspaziocolonna).testaNecrologi;
	lPtrNecrologioSuccessivo = &((*ptrspaziocolonna).testaNecrologi);
	lPtrNecrologioPrecedente = NULL;
	
	// cerco il necrologio da rimuovere 
	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;
		if (lPtrNecrologio == ptrnecrologio) 
			break;

		// avanzo i puntatori
		lPtrNecrologioPrecedente = lPtrNecrologio;
		lPtrNecrologioSuccessivo = &((*lPtrNecrologio).successivo);
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}

	if (lPtrNecrologio == NULL) 
		return;
	
	// rimuovo il necrologio dall'elenco della colonna
	// sono sicuro che e' l'ultimo della lista
	(*lPtrNecrologioSuccessivo) = (*lPtrNecrologio).successivo;
	if ((*ptrspaziocolonna).codaNecrologi == lPtrNecrologio) 
	{
		(*ptrspaziocolonna).codaNecrologi = lPtrNecrologioPrecedente;
	}
	
	// aggiorno le info sulla colonna
	(*ptrspaziocolonna).numeroNecrologi--;
	TogliSpazioColonna(ptrspaziocolonna, lPtrNecrologio);
} // RimuoviNecrologio

/* ------------------------------------------------------------------------ *

	AggiungiDefuntiSpazioColonna

* ------------------------------------------------------------------------ */
static void XTAPI AggiungiDefuntiSpazioColonna(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw()
{
	(*ptrspaziocolonna).numeroNecrologiDefunti++;
	AggiungiSpazioColonna(ptrspaziocolonna, ptrnecrologio);
} // AggiungiDefuntiSpazioColonna

/* ------------------------------------------------------------------------ *

	AggiungiFondoDefuntiSpazioColonna

* ------------------------------------------------------------------------ */
static void XTAPI AggiungiFondoDefuntiSpazioColonna(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw()
{
	(*ptrspaziocolonna).numeroNecrologiFondoDefunti++;
	AggiungiSpazioColonna(ptrspaziocolonna, ptrnecrologio);
} // AggiungiFondoDefuntiSpazioColonna

/* ------------------------------------------------------------------------ *

	InserisciDefuntoInTestaFondo

* ------------------------------------------------------------------------ */
void XTAPI InserisciDefuntoInTestaFondo(spaziocolonna *ptrspaziocolonna, defunto *ptrdefunto) throw()
{
	// per scorrere la lista dei necrologi del defunto
	necrologio *lPtrNecrologio = NULL;
	
	// controllo se non e' stato ancora inserito nessun defunto
	if ((*ptrspaziocolonna).numeroNecrologiFondoDefunti == 0) 
	{
		(*ptrspaziocolonna).testaFondoDefunti = ptrdefunto;
		(*ptrspaziocolonna).codaFondoDefunti = ptrdefunto;
		(*ptrdefunto).successivo = NULL;
	} 
	else 
	{
		(*ptrdefunto).successivo = (*ptrspaziocolonna).testaFondoDefunti;
		(*ptrspaziocolonna).testaFondoDefunti = ptrdefunto;
	}
	
	// imposto il puntatore ai necrologi codice A
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiA;
	
	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		AggiungiFondoDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);
		
		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}
	
	// imposto il puntatore ai necrologi codice B
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiB;
	
	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		AggiungiFondoDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);
		
		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}
	
	// imposto il puntatore ai necrologi codice C
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiC;
	
	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		AggiungiFondoDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);
		
		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}
} // InserisciDefuntoInTestaFondo

/* ------------------------------------------------------------------------ *

	InserisciDefuntoInCodaFondo

* ------------------------------------------------------------------------ */
void XTAPI  InserisciDefuntoInCodaFondo(spaziocolonna *ptrspaziocolonna, defunto *ptrdefunto) throw()
{
	// per scorrere la lista dei necrologi del defunto
	necrologio *lPtrNecrologio = NULL;
	
	// controllo se non e' stato ancora inserito nessun defunto
	if ((*ptrspaziocolonna).numeroNecrologiFondoDefunti == 0) 
	{
		(*ptrspaziocolonna).testaFondoDefunti = ptrdefunto;
		(*ptrspaziocolonna).codaFondoDefunti = ptrdefunto;
		(*ptrdefunto).successivo = NULL;
	} 
	else 
	{
		(*(*ptrspaziocolonna).codaFondoDefunti).successivo = ptrdefunto;
		(*ptrdefunto).successivo = NULL;
		(*ptrspaziocolonna).codaFondoDefunti = ptrdefunto;
	}
	
	// imposto il puntatore ai necrologi codice A
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiA;
	
	while (1) 
	{

		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		AggiungiFondoDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);
		
		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}
	
	// imposto il puntatore ai necrologi codice B
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiB;
	
	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		AggiungiFondoDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);
		
		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}
	
	// imposto il puntatore ai necrologi codice C
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiC;
	
	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		AggiungiFondoDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);
		
		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}
} // InserisciDefuntoInCodaFondo

/* ------------------------------------------------------------------------ *

	InserisciDefuntoInOrdineFondo

* ------------------------------------------------------------------------ */
void XTAPI InserisciDefuntoInOrdineFondo(spaziocolonna *ptrspaziocolonna, defunto *ptrdefunto) throw()
{
	// per scorrere la lista dei necrologi del defunto
	necrologio *lPtrNecrologio = NULL;
	// per scorrere l'elenco dei defunti
	defunto *lPtrDefunto = NULL;
	// puntatore al defunto precedente
	defunto *lPtrDefuntoPrecedente = NULL;
	// puntatore al campo successivo del defunto precedente
	defunto **lPtrDefuntoSuccessivo = NULL;
	
	// imposto i puntatori
	lPtrDefunto = (*ptrspaziocolonna).testaFondoDefunti;
	lPtrDefuntoPrecedente = NULL;
	lPtrDefuntoSuccessivo = &(*ptrspaziocolonna).testaFondoDefunti;

	// scorro la lista per trovare la posizione giusta
	while (1) 
	{
		// controllo se ci sono ancora defunti nella lista
		if (lPtrDefunto == NULL) 
			break;

		// controllo se per la dimensione si pu• inserire
		if ((*lPtrDefunto).altezzaTotaleDoubleFixed < (*ptrdefunto).altezzaTotaleDoubleFixed &&
			(*lPtrDefunto).tipoRigore != kRigoreDiColonna) 
				break;

		// incremento i puntatori
		lPtrDefuntoSuccessivo = &(*lPtrDefunto).successivo;
		lPtrDefuntoPrecedente = lPtrDefunto;
		lPtrDefunto = (*lPtrDefunto).successivo;
	}

	// inserisco il defunto nella posizione trovata
	(*lPtrDefuntoSuccessivo) = ptrdefunto;
	(*ptrdefunto).successivo = lPtrDefunto;

	// controllo se devo modificare anche il puntatore alla coda
	if ((*ptrspaziocolonna).codaFondoDefunti == lPtrDefuntoPrecedente) 
	{
		(*ptrspaziocolonna).codaFondoDefunti = ptrdefunto;
		(*ptrdefunto).successivo = NULL;
	}

	// imposto il puntatore ai necrologi codice A
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiA;

	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		AggiungiDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);

		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}

	// imposto il puntatore ai necrologi codice B
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiB;

	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		AggiungiDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);

		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}

	// imposto il puntatore ai necrologi codice C
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiC;

	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		AggiungiDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);

		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}
} // InserisciDefuntoInOrdineFondo

/* ------------------------------------------------------------------------ *

	InserisciDefuntoInCoda

* ------------------------------------------------------------------------ */
void XTAPI InserisciDefuntoInCoda(spaziocolonna *ptrspaziocolonna, defunto *ptrdefunto) throw()
{
	// per scorrere la lista dei necrologi del defunto
	necrologio *lPtrNecrologio = NULL;
	
	// controllo se non e' stato ancora inserito nessun defunto
	if ((*ptrspaziocolonna).numeroNecrologiDefunti == 0) 
	{
		(*ptrspaziocolonna).testaDefunti = ptrdefunto;
		(*ptrspaziocolonna).codaDefunti = ptrdefunto;
	} 
	else 
	{
		(*(*ptrspaziocolonna).codaDefunti).successivo = ptrdefunto;
		(*ptrspaziocolonna).codaDefunti = ptrdefunto;
	}

	(*ptrdefunto).successivo = NULL;

	// imposto il puntatore ai necrologi codice A 
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiA;

	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		AggiungiDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);

		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}

	// imposto il puntatore ai necrologi codice B 
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiB;

	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;
		
		// aggiorno le info sulla colonna
		AggiungiDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);

		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}

	// imposto il puntatore ai necrologi codice C
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiC;

	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		AggiungiDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);

		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}

} // InserisciDefuntoInCoda

/* ------------------------------------------------------------------------ *

	InserisciDefuntoInOrdine

* ------------------------------------------------------------------------ */
void XTAPI InserisciDefuntoInOrdine(spaziocolonna *ptrspaziocolonna, defunto *ptrdefunto) throw()
{
	// per scorrere la lista dei necrologi del defunto
	necrologio *lPtrNecrologio = NULL;
	// per scorrere l'elenco dei defunti
	defunto *lPtrDefunto = NULL;
	// puntatore al defunto precedente
	defunto *lPtrDefuntoPrecedente = NULL;
	// puntatore al campo successivo del defunto precedente
	defunto **lPtrDefuntoSuccessivo = NULL;
	
	// imposto i puntatori
	lPtrDefunto = (*ptrspaziocolonna).testaDefunti;
	lPtrDefuntoPrecedente = NULL;
	lPtrDefuntoSuccessivo = &(*ptrspaziocolonna).testaDefunti;

	// scorro la lista per trovare la posizione giusta
	while (1) 
	{
		// controllo se ci sono ancora defunti nella lista
		if (lPtrDefunto == NULL) 
			break;

		// controllo se per la dimensione si puo' inserire
		if ((*lPtrDefunto).altezzaTotaleDoubleFixed < (*ptrdefunto).altezzaTotaleDoubleFixed &&
			(*lPtrDefunto).tipoRigore != kRigoreDiColonna) break;

		// incremento i puntatori
		lPtrDefuntoSuccessivo = &(*lPtrDefunto).successivo;
		lPtrDefuntoPrecedente = lPtrDefunto;
		lPtrDefunto = (*lPtrDefunto).successivo;
	}

	// inserisco il defunto nella posizione trovata
	(*lPtrDefuntoSuccessivo) = ptrdefunto;
	(*ptrdefunto).successivo = lPtrDefunto;

	// controllo se devo modificare anche il puntatore alla coda
	if ((*ptrspaziocolonna).codaDefunti == lPtrDefuntoPrecedente) 
	{
		(*ptrspaziocolonna).codaDefunti = ptrdefunto;
		(*ptrdefunto).successivo = NULL;
	}

	// imposto il puntatore ai necrologi codice A
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiA;

	while (1)
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		AggiungiDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);

		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}

	// imposto il puntatore ai necrologi codice B
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiB;

	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		AggiungiDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);

		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}

	// imposto il puntatore ai necrologi codice C
	lPtrNecrologio = (*ptrdefunto).elencoNecrologiC;

	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		AggiungiDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);

		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}
} // InserisciDefuntoInOrdine

/* ------------------------------------------------------------------------ *

	TogliDefuntiSpazioColonna

* ------------------------------------------------------------------------ */
static void XTAPI TogliDefuntiSpazioColonna(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw()
{
	(*ptrspaziocolonna).numeroNecrologiDefunti--;
	TogliSpazioColonna(ptrspaziocolonna, ptrnecrologio);
} // TogliDefuntiSpazioColonna

/* ------------------------------------------------------------------------ *

	TogliFondoDefuntiSpazioColonna

* ------------------------------------------------------------------------ */
static void XTAPI TogliFondoDefuntiSpazioColonna(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw()
{
	(*ptrspaziocolonna).numeroNecrologiFondoDefunti--;
	TogliSpazioColonna(ptrspaziocolonna, ptrnecrologio);
} // TogliFondoDefuntiSpazioColonna

/* ------------------------------------------------------------------------ *

	RimuoviDefunto

* ------------------------------------------------------------------------ */
void XTAPI RimuoviDefunto(spaziocolonna *ptrspaziocolonna, defunto *ptrdefunto) throw()
{
	// per scorrere la lista
	defunto *lPtrDefunto = NULL;
	// per tenere il puntatore al defunto precedente
	defunto *lPtrDefuntoPrecedente = NULL;
	// per tenere il puntatore del campo successivo del defunto precedente
	defunto **lPtrDefuntoSuccessivo = NULL;
	// per aggiornare la lista dei necrologi del defunto
	defunto *lPtrDefuntoUltimo = NULL;
	// per sapere se sto esaminado la lista dei defunti a fondo pagina
	Boolean lFondo = FALSE;
	// per scorrere la lista dei necrologi del defunto
	necrologio *lPtrNecrologio = NULL;
	
	// imposto il puntatore alla lista dei defunti nella colonna
	if ((*ptrspaziocolonna).testaDefunti != NULL) 
	{
		lPtrDefunto = (*ptrspaziocolonna).testaDefunti;
		lPtrDefuntoSuccessivo = &((*ptrspaziocolonna).testaDefunti);
	} 
	else 
	{
		lPtrDefunto = (*ptrspaziocolonna).testaFondoDefunti;
		lPtrDefuntoSuccessivo = &((*ptrspaziocolonna).testaFondoDefunti);
		lFondo = TRUE;
	}
	lPtrDefuntoPrecedente = NULL;

	// cerco il defunto da rimuovere
	while (1) 
	{
		if (lPtrDefunto == NULL) 
			break;
		if (lPtrDefunto == ptrdefunto) 
			break;

		// avanzo i puntatori
		lPtrDefuntoPrecedente = lPtrDefunto;
		lPtrDefuntoSuccessivo = &((*lPtrDefunto).successivo);
		lPtrDefunto = (*lPtrDefunto).successivo;
		if (lPtrDefunto == NULL && lFondo == FALSE) 
		{
			lPtrDefunto = (*ptrspaziocolonna).testaFondoDefunti;
			lPtrDefuntoSuccessivo = &((*ptrspaziocolonna).testaFondoDefunti);
			lFondo = TRUE;
		}
	}

	if (lPtrDefunto == NULL) return;

	// rimuovo il defunto dall'elenco della colonna
	(*lPtrDefuntoSuccessivo) = (*lPtrDefunto).successivo;
	if (lFondo == FALSE) 
	{
		// ho tolto dai defunti della colonna
		if ((*ptrspaziocolonna).codaDefunti == lPtrDefunto) 
		{
			(*ptrspaziocolonna).codaDefunti = lPtrDefuntoPrecedente;
		}
	} 
	else 
	{
		// ho tolto dai defunti del fondo della colonna
		if ((*ptrspaziocolonna).codaFondoDefunti == lPtrDefunto) 
		{
			(*ptrspaziocolonna).codaFondoDefunti = lPtrDefuntoPrecedente;
		}
	}

	// imposto il puntatore ai necrologi codice A 
	lPtrNecrologio = (*lPtrDefunto).elencoNecrologiA;

	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		if (lFondo == FALSE)
			TogliDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);
		else 
			TogliFondoDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);

		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}

	// imposto il puntatore ai necrologi codice B 
	lPtrNecrologio = (*lPtrDefunto).elencoNecrologiB;

	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		if (lFondo == FALSE)
			TogliDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);
		else 
			TogliFondoDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);

		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}

	// imposto il puntatore ai necrologi codice C
	lPtrNecrologio = (*lPtrDefunto).elencoNecrologiC;

	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// aggiorno le info sulla colonna
		if (lFondo == FALSE)
			TogliDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);
		else 
			TogliFondoDefuntiSpazioColonna(ptrspaziocolonna, lPtrNecrologio);

		// avanzo nella lista dei necrologi che tolgo
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}
} // RimuoviDefunto

/* ------------------------------------------------------------------------ *

	ControllaChiusuraColonna

* ------------------------------------------------------------------------ */
errorixtension XTAPI ControllaChiusuraColonna(spaziocolonna *ptrspaziocolonna, tipochiusura *ptrchiusura, Fixed *ptrdifferenza) throw()
{
	// spazio disponibile sulla colonna 
	Fixed lSpazioDisponibile = 0;
	// spazio occupato sulla colonna 
	Fixed lSpazioOccupato = 0;
	// differenza tra lo spazio occupato e lo spazio disponibile: Š sempre positivo 
	Fixed lDifferenza = 0;
	cosafare lCosaFare = kNulla;
	// contiene il numero di necrologi da inserire nella colonna 
	int16 lNumeroNecrologi = 0;
	// indice per scorrere i necrologi da inserire nella colonna 
	// mi dice se devo restringere o allargare 
	int16 lIndiceNecrologio = 0;
	// massimo spazio recuperabile allargando al massimo lo spazio tra i necrologi 
	Fixed lSpazioRecuperabile  = 0;
	// puntatore per scorrere i necrologi da inserire nella colonna 
	necrologio *lPtrNecrologio = NULL;
	// per calcolare il numero dei paragrafi 
	int32 lNumeroParagrafi = 0;
	// per calcolare il numero dei paragrafi con spazio diverso da zero 
	int32 lNumeroParagrafiDiversi = 0;
	// per calcolare il numero di linee 
	int32 lNumeroLinee = 0;
	
	// prendo il numero dei necrologi 
	lNumeroNecrologi = (*ptrspaziocolonna).numeroNecrologi +
						(*ptrspaziocolonna).numeroNecrologiDefunti +
						(*ptrspaziocolonna).numeroNecrologiFondoDefunti;

	if (lNumeroNecrologi == 0) 
	{
		(*ptrchiusura) = kInserireAncora;
		(*ptrdifferenza) = (*ptrspaziocolonna).spazioDisponibile;
		return(kNessunErrore);
	}

	// calcolo il numero dei paragrafi 
	lNumeroParagrafi = (*ptrspaziocolonna).numeroParagrafi;

	// calcolo il numero dei paragrafi diversi 
	lNumeroParagrafiDiversi = (*ptrspaziocolonna).numeroParagrafiDiversi;

	// calcolo il numero delle linee 
	lNumeroLinee = (*ptrspaziocolonna).numeroLinee;

	// calcolo lo spazio disponibile sulla colonna 
	lSpazioDisponibile = (*ptrspaziocolonna).spazioDisponibile;

	// calcolo lo spazio occupato 
	lSpazioOccupato = (*ptrspaziocolonna).spazioOccupato +
					  (*ptrspaziocolonna).spazioTraNecrologi;

	// calcolo se devo allargare o restringere 
	lCosaFare = CosaFare(lSpazioOccupato, lSpazioDisponibile, &lDifferenza);

	switch (lCosaFare) 
	{
		case kNulla:
			// chiusura perfetta 
			(*ptrchiusura) = kPreferenzeRispettate;
			break;
		case kAllargare:
			// devo allargare 
			// verifico se rispetto le preferenze 
			if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE) 
			{
				// posso modificare lo spazio tra i necrologi 
				// questo spazio cerco di recuperarlo tutto 
				// calcolo lo spazio che posso ancora recuperare 
				lSpazioRecuperabile = kFixedDifferenzaSpazioTraNecrologi *
									  (lNumeroNecrologi - 1);

				// calcolo lo spazio che ho recuperato modificando 
				// lo spazio tra i necrologi 
				lDifferenza -= lSpazioRecuperabile;
			}

			if (lDifferenza > 0) 
			{
				// devo modificare anche lo spazio tra i paragrafi 
				lSpazioRecuperabile = lNumeroParagrafi *
									  gXtPreferences.giustificazionePref.incrementoMaxSpazioParagrafi;

				lDifferenza -= lSpazioRecuperabile;
			}

			if (lDifferenza > 0) 
			{
				// devo modificare anche l'interlinea 
				lSpazioRecuperabile = lNumeroLinee *
									  gXtPreferences.giustificazionePref.incrementoMaxInterlinea;

				lDifferenza -= lSpazioRecuperabile;
			}

			// controllo la variazione dell'interlinea per sapere se 
			// rispetto le impostazioni delle preferenze 
			if (lDifferenza > 0) 
				(*ptrchiusura) = kInserireAncora;
			else 
				(*ptrchiusura) = kPreferenzeRispettate;

			break;

		case kRestringere:
			// devo restringere 
			if (lDifferenza > 0) 
			{
				// devo modificare lo spazio tra i paragrafi 

				// imposto il puntatore ai necrologi della colonna corrente 
				lPtrNecrologio = PrendiPrimoNecrologioSpazioColonna(ptrspaziocolonna);
				if (lPtrNecrologio == NULL) 
				{
					// errore sul numero dei necrologi 
					return(kErroreNumeroNecrologiSullaColonna);
				}

				// imposto lo spazio recuperabile 
				lSpazioRecuperabile = 0;

				// scorro i necrologi per calcolare lo spazio tra i paragrafi 
				for (lIndiceNecrologio = 0; lIndiceNecrologio < lNumeroNecrologi;
					 lIndiceNecrologio++) 
				{
					// calcolo lo spazio tra i paragrafi 
					lSpazioRecuperabile = (*lPtrNecrologio).numeroParagrafiDiversi *
										  gXtPreferences.giustificazionePref.decrementoMaxSpazioParagrafi;

					// controllo che il decremento non sia maggiore 
					// dello spazio realmente presente 
					if (lSpazioRecuperabile > (*lPtrNecrologio).spazioTraParagrafi) 
					{
						// non posso togliere tutto queso spazio 
						lSpazioRecuperabile = (*lPtrNecrologio).spazioTraParagrafi;
					 }

					// controllo che la differenza che rimane non sia negativa 
					if (lDifferenza < lSpazioRecuperabile) 
					{
						lDifferenza = 0;
						break;
					}

					// calcolo la nuova differenza 
					lDifferenza -= lSpazioRecuperabile;

					// passo al necrologio successivo 
					lPtrNecrologio = PrendiNecrologioSuccessivoSpazioColonna(ptrspaziocolonna,
																			 lPtrNecrologio);
					if (lPtrNecrologio == NULL &&
						(lIndiceNecrologio + 1) != lNumeroNecrologi) 
					{
						return(kErroreNumeroNecrologiSullaColonna);
					}
				}
			}

			if (lDifferenza > 0) 
			{
				// devo modificare anche l'interlinea 
				lSpazioRecuperabile = lNumeroLinee *
									  gXtPreferences.giustificazionePref.decrementoMaxInterlinea;

				lDifferenza -= lSpazioRecuperabile;
			}

			// controllo la variazione dell'interlinea per sapere se 
			// rispetto le impostazioni delle preferenze 
			if (lDifferenza > 0) 
				(*ptrchiusura) = kPreferenzeNonRispettate;
			else 
				(*ptrchiusura) = kPreferenzeRispettate;

			break;
	}

	(*ptrdifferenza) = lDifferenza;

	return(kNessunErrore);
} // ControllaChiusuraColonna

/* ------------------------------------------------------------------------ *

	PrendiAllargamento

* ------------------------------------------------------------------------ */
Fixed XTAPI PrendiAllargamento(spaziocolonna *ptrspaziocolonna) throw()
{
	// contiene il numero di necrologi da inserire nella colonna
	int16 lNumeroNecrologi = 0;
	// contiene lo spazio recuperabile allargando i necrologi
	Fixed lSpazioRecuperabile  = 0;
	// per calcolare il numero dei paragrafi
	int32 lNumeroParagrafi = 0;
	// per calcolare il numero di linee
	int32 lNumeroLinee = 0;
	

	// prendo il numero dei necrologi
	lNumeroNecrologi = (*ptrspaziocolonna).numeroNecrologi +
						(*ptrspaziocolonna).numeroNecrologiDefunti +
						(*ptrspaziocolonna).numeroNecrologiFondoDefunti;

	if (lNumeroNecrologi == 0) 
		return(0);

	// calcolo il numero dei paragrafi
	lNumeroParagrafi = (*ptrspaziocolonna).numeroParagrafi;

	// calcolo il numero delle linee
	lNumeroLinee = (*ptrspaziocolonna).numeroLinee;

	// devo allargare
	// verifico se rispetto le preferenze 
	if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE) 
	{
		// posso modificare lo spazio tra i necrologi
		// questo spazio cerco di recuperarlo tutto
		// calcolo lo spazio che posso ancora recuperare
		lSpazioRecuperabile = kFixedDifferenzaSpazioTraNecrologi *
							  (lNumeroNecrologi - 1);

	} 
	else 
		lSpazioRecuperabile = 0;

	// devo modificare anche lo spazio tra i paragrafi
	lSpazioRecuperabile += lNumeroParagrafi * gXtPreferences.giustificazionePref.incrementoMaxSpazioParagrafi;

	// devo modificare anche l'interlinea
	lSpazioRecuperabile += lNumeroLinee * gXtPreferences.giustificazionePref.incrementoMaxInterlinea;

	// ritorno l'allargamento 
	return(lSpazioRecuperabile);
} // PrendiAllargamento

/* ------------------------------------------------------------------------ *

	PrendiRestringimento

* ------------------------------------------------------------------------ */
Fixed XTAPI PrendiRestringimento(spaziocolonna *ptrspaziocolonna) throw()
{
	// indice per scorrere i necrologi da inserire nella colonna
	int16 lNumeroNecrologi = 0;
	// mi dice se devo restringere o allargare
	int16 lIndiceNecrologio = 0;
	// massimo spazio recuperabile allargando al massimo lo spazio tra i necrologi 
	Fixed lSpazioRecuperabile  = 0;
	// per calcolare lo spazio tra i paragrafi 
	Fixed lSpazioTraParagrafi = 0;
	// puntatore per scorrere i necrologi da inserire nella colonna 
	necrologio *lPtrNecrologio = NULL;
	// per calcolare il numero dei paragrafi 
	int32 lNumeroParagrafi = 0;
	// per calcolare il numero dei paragrafi con spazio diverso da zero 
	int32 lNumeroParagrafiDiversi = 0;
	// per calcolare il numero di linee 
	int32 lNumeroLinee = 0;
	

	// prendo il numero dei necrologi
	lNumeroNecrologi = (*ptrspaziocolonna).numeroNecrologi +
						(*ptrspaziocolonna).numeroNecrologiDefunti +
						(*ptrspaziocolonna).numeroNecrologiFondoDefunti;

	if (lNumeroNecrologi == 0) 
		return(0);

	// calcolo il numero dei paragrafi
	lNumeroParagrafi = (*ptrspaziocolonna).numeroParagrafi;

	// calcolo il numero dei paragrafi diversi
	lNumeroParagrafiDiversi = (*ptrspaziocolonna).numeroParagrafiDiversi;

	// calcolo il numero delle linee
	lNumeroLinee = (*ptrspaziocolonna).numeroLinee;

	// imposto il puntatore ai necrologi della colonna corrente 
	lPtrNecrologio = PrendiPrimoNecrologioSpazioColonna(ptrspaziocolonna);
	if (lPtrNecrologio == NULL) 
	{
		MostraErrore(kErroreNumeroNecrologiSullaColonna);
		return(0);
	}

	// imposto lo recuperabile
	lSpazioRecuperabile = 0;
	lSpazioTraParagrafi = 0;

	// scorro i necrologi per calcolare lo spazio tra i paragrafi
	for (lIndiceNecrologio = 0; lIndiceNecrologio < lNumeroNecrologi;
		 lIndiceNecrologio++) 
	{
		// calcolo lo spazio tra i paragrafi
		lSpazioTraParagrafi = (*lPtrNecrologio).numeroParagrafiDiversi *
							  gXtPreferences.giustificazionePref.decrementoMaxSpazioParagrafi;

		// controllo che il decremento non sia maggiore
		// dello spazio realmente presente
		if (lSpazioTraParagrafi > (*lPtrNecrologio).spazioTraParagrafi) 
		{
			// non posso togliere tutto queso spazio
			lSpazioTraParagrafi = (*lPtrNecrologio).spazioTraParagrafi;
		}

		// aimento lo spazio recuperabile
		lSpazioRecuperabile += lSpazioTraParagrafi;

		// passo al necrologio successivo
		lPtrNecrologio = PrendiNecrologioSuccessivoSpazioColonna(ptrspaziocolonna,
																 lPtrNecrologio);
		if (lPtrNecrologio == NULL &&
			(lIndiceNecrologio + 1) != lNumeroNecrologi) 
		{
			MostraErrore(kErroreNumeroNecrologiSullaColonna);
			return(0);
		}
	}

	// calcolo lo spazio recuperabile
	lSpazioRecuperabile += lNumeroLinee *  gXtPreferences.giustificazionePref.decrementoMaxInterlinea;

	// ritrono il restringimento
	return(lSpazioRecuperabile);
} // PrendiRestringimento

/* ------------------------------------------------------------------------ *

	InserisciListaNecrologi

* ------------------------------------------------------------------------ */
static errorixtension XTAPI InserisciListaNecrologi(int16 *ptrpagina, int16 *ptrcolonna, necrologio *ptrnecrologi) throw()
{
	// per scorrere la lista dei necrologi da inserire
	necrologio *lPtrNecrologio = NULL;
	// puntatore al successivo elemento da inserire
	necrologio *lPtrNecrologioSuccessivo = NULL;
	// puntatore alle info di colonna dove inserire
	spaziocolonna *lPtrSpazioColonna = NULL;

	// imposto il puntatore
	lPtrNecrologio = ptrnecrologi;

	// prendo le info dello spazio colonna corrente 
	lPtrSpazioColonna  = PrendiSpazioColonna((*ptrpagina), (*ptrcolonna));

	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// prendo il puntatore al necrologio successivo
		lPtrNecrologioSuccessivo = (*lPtrNecrologio).successivo;

		// controllo se devo cambiare la pagina
		if ((*lPtrNecrologio).cambioColonna == TRUE) 
		{
			// controllo se devo mettere la colonna come non usabile
			if ((*lPtrNecrologio).codiceBox != 0 || (lPtrNecrologioSuccessivo != NULL &&
				 (*lPtrNecrologioSuccessivo).codiceBox != 0)) 
			{

				// dico che la colonna corrente non e' usabile
				(*lPtrSpazioColonna).chiusuraColonna = kChiusa;

				// salvo la chiusura della colonna corrente
				(*lPtrSpazioColonna).tipoChiusura = kPreferenzeRispettate;
			}

			// vado alla colonna successiva
			if (lPtrNecrologioSuccessivo != NULL &&
				 (*lPtrNecrologioSuccessivo).codiceBox == 0) 
			{
				// passo alla colonna successiva
				gErroreXtension = ImpostaColonnaSuccessiva(ptrpagina, ptrcolonna);
				if (gErroreXtension != kNessunErrore) 
				{
					// non e' possibile incrementare la colonna
					return(gErroreXtension);
				}
			} 
			else 
			{
				// passo alla colonna successiva
				gErroreXtension = CercaPrimaColonnaDaChiudere(ptrpagina, ptrcolonna);
				if (gErroreXtension != kNessunErrore) 
				{
					// non e' possibile incrementare la colonna
					return(gErroreXtension);
				}

			}

			// prendo le info dello spazio colonna corrente
			lPtrSpazioColonna  = PrendiSpazioColonna((*ptrpagina), (*ptrcolonna));
		}

		if ((*lPtrNecrologio).codiceBox != 0) 
		{
			// inserisco il necrologio in coda della colonna corrente
			InserisciNecrologioInCoda(lPtrSpazioColonna, lPtrNecrologio);
		} 
		else 
		{
			// libero la memoria per il necrologio vuoto
			// usato solo per il cambio di colonna
			DisposePtr((Ptr) lPtrNecrologio);
			lPtrNecrologio = NULL;
		}

		// passo al necrologio successivo
		lPtrNecrologio = lPtrNecrologioSuccessivo;
	}
	return (kNessunErrore);
} // InserisciListaNecrologi

/* ------------------------------------------------------------------------ *

	InserisciDefuntiFineDocumento

* ------------------------------------------------------------------------ */
static errorixtension XTAPI InserisciDefuntiFineDocumento(defunto *ptrdefunto, Fixed *ptrscarto) throw()
{
	// indice per scorrere le pagine
	int16 lPagina = 0;
	// indice per scorrere le colonne
	int16 lColonna = 0;
	// per scorrere la lista dei defunti
	defunto *lPtrDefunto = NULL;
	
	// inizializzo i contatori di pagina
	lPagina = PrendiNumeroPagine();
	lColonna = PrendiNumeroColonne();

	// controllo che la lista passata non sia nulla
	if (ptrdefunto == NULL) 
		return(kNessunErrore);;

	// cerco la prima colonna ancora da chiudere
	gErroreXtension = CercaUltimaColonnaDaChiudere(&lPagina, &lColonna);
	if (gErroreXtension != kNessunErrore) 
	{
		// non riesco a trovare la colonna da chiudere
		return(gErroreXtension);
	}

	// imposto il puntatore alla lista dei defunti
	lPtrDefunto = ptrdefunto;

	// calcolo l'ordinamento per la lista dei defunti
	gErroreXtension = CalcolaMigliorOrdineDefuntiFondo(lPtrDefunto,
														  lPagina,
														  lColonna,
														  ptrscarto);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nell'ordinamento
		return(gErroreXtension);
	}

	return(kNessunErrore);
} // InserisciDefuntiFineDocumento

/* ------------------------------------------------------------------------ *

	InserisciDefuntiNelDocumento

* ------------------------------------------------------------------------ */
static errorixtension XTAPI InserisciDefuntiNelDocumento(defunto *ptrdefunto, Fixed *ptrscarto) throw()
{
	// indice per scorrere le pagine
	int16 lPagina = 0;
	// indice per scorrere le colonne
	int16 lColonna = 0;
	// per scorrere la lista dei defunti
	defunto *lPtrDefunto = NULL;
	
	// inizializzo i contatori di pagina
	lPagina = 1;
	lColonna = 1;

	// controllo che la lista passata non sia nulla
	if (ptrdefunto == NULL) 
		return(kNessunErrore);

	// cerco la prima colonna ancora da chiudere
	gErroreXtension = CercaPrimaColonnaDaChiudere(&lPagina, &lColonna);
	if (gErroreXtension != kNessunErrore) 
	{
		// non riesco a trovare la colonna da chiudere
		return(gErroreXtension);
	}

	// imposto il puntatore alla lista dei defunti
	lPtrDefunto = ptrdefunto;

	// calcolo l'ordinamento per la lista dei defunti
	gErroreXtension = CalcolaMigliorOrdineDefunti(lPtrDefunto,
													lPagina,
													lColonna,
													ptrscarto);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nell'ordinamento
		return(gErroreXtension);
	}

	return(kNessunErrore);
} // InserisciDefuntiNelDocumento

/* ------------------------------------------------------------------------ *

	EseguiCalcoloPosizionamento

* ------------------------------------------------------------------------ */
static errorixtension XTAPI EseguiCalcoloPosizionamento() throw()
{
	// per calcolare lo scarto ad ogni inserimento
	Fixed lScarto = 0;
	// puntatore defunto
	defunto *lPtrDefunto = NULL;
	// altezza del defunto da inserire in una colonna completamente libera
	DoubleFixed lAltezzaDefuntoDoubleFixed = 0;
	// contatore del numero dei necrologi di un defunto
	int16 lNumeroNecrologi = 0;
	// per sapere il rigore del defunto da inserire
	tiporigore lTipoRigore = kNessunRigore;
	// indice per scorrere le pagine
	int16 lPagina = 0;
	// indice per scorrere le colonne
	int16 lColonna = 0;
	// per sapere se ho trovato una colonna completamente vuota
	Boolean lColonnaVuota = FALSE;
	// puntatore defunto
	defunto *lPtrDefuntoDaInserire = NULL;
	// puntatore necrologio
	necrologio *lPtrNecrologio = NULL;
	

	if (gXtPreferences.riempitiviPref.usaAnniversari == FALSE && gPtrAnniversari != NULL) 
	{
		// gli anniversari non posso essere usati come riempitivi
		// li posizione in ordine crescete partendo dall'ultima colonna
		gErroreXtension = InserisciDefuntiFineDocumento(gPtrAnniversari,&lScarto);
		if (gErroreXtension != kNessunErrore) 
		{
			// non e' possibile posizionare in fondo gli anniversari
			return(gErroreXtension);
		}
	}

	if (gXtPreferences.riempitiviPref.usaRingraziamenti == FALSE && gPtrRingraziamenti != NULL) 
	{
		// i ringraziamenti non posso essere usati come riempitivi
		// li posizione in ordine crescete partendo dall'ultima colonna

		gErroreXtension = InserisciDefuntiFineDocumento(gPtrRingraziamenti,&lScarto);
		if (gErroreXtension != kNessunErrore) 
		{
			// non e' possibile posizionare in fondo gli anniversari
			return(gErroreXtension);
		}
	}

	// inserisco i necrologi di un defunto piu' lungo di una colonna
	// in testa alle colonne completamente libere

	// inzializzo lo scarto per il nuovo calcolo
	lScarto = 0;

	// prendo il puntatore degli annunci
	lPtrDefunto = gPtrAnnunci;

	// prendo l'altezza del prossimo defunto
	lAltezzaDefuntoDoubleFixed = PrendiAltezzaDefunto(lPtrDefunto);

	// prendo il numero dei necrologi del defunto
	lNumeroNecrologi = PrendiNumeroNecrologiDefunto(lPtrDefunto);

	// incremento l'altezza totale
	if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE)
		lAltezzaDefuntoDoubleFixed += (DoubleFixed) (lNumeroNecrologi * kFixedMinimoSpazioTraNecrologi);
	else
		lAltezzaDefuntoDoubleFixed += (DoubleFixed) lNumeroNecrologi * gXtPreferences.giustificazionePref.valoreIniziale;

	// prendo il rigore del defunto
	lTipoRigore = (*lPtrDefunto).tipoRigore;

	// scelgo la colonna adatta
	lColonnaVuota = PrendiPrimaColonnaLibera(&lPagina, &lColonna, lAltezzaDefuntoDoubleFixed);

	while (1) 
	{
		// controllo se ho a disposizione una colonna completamente libera
		if (lColonnaVuota == FALSE) 
			break;

		// controllo se il defunto ha un rigore di colonna
		// o se e' piu' lungo di una colonna
		if (lAltezzaDefuntoDoubleFixed < (DoubleFixed) PrendiAltezzaColonnaOttimale() &&
			lTipoRigore == kNessunRigore) break;

		// prendo un defunto da inserire
		lPtrDefuntoDaInserire = EstraiDefunto(&lPtrDefunto);

		// non ci sono pi— defunti da inserire: esco dal ciclo
		if (lPtrDefuntoDaInserire == NULL) 
			break;

		// calcola l'ordinamento dei necrologi del defunto corrente
		CalcolaMigliorOrdineNecrologi(lPtrDefuntoDaInserire,
									  lPagina,
									  lColonna,
									  &lScarto,
									  &lPtrNecrologio);
									  
		// inserisce i necrologi ordinati
		InserisciListaNecrologi(&lPagina, &lColonna, lPtrNecrologio);

		// controllo il prossimo defunto
		if ( lPtrDefunto == NULL ) 
			break;

		// prendo l'altezza del prossimo defunto
		lAltezzaDefuntoDoubleFixed = PrendiAltezzaDefunto(lPtrDefunto);

		// incremento l'altezza totale 
		if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE)
			lAltezzaDefuntoDoubleFixed += (DoubleFixed) (lNumeroNecrologi * kFixedMinimoSpazioTraNecrologi);
		else 
			lAltezzaDefuntoDoubleFixed += (DoubleFixed) (lNumeroNecrologi * gXtPreferences.giustificazionePref.valoreIniziale);

		// prendo il rigore del defunto
		lTipoRigore = (*lPtrDefunto).tipoRigore;

		// ciclo per scegliere la colonna successiva adatta
		lColonnaVuota = PrendiColonnaLibera(&lPagina, &lColonna, lAltezzaDefuntoDoubleFixed);
	}

	// inserisco i necrologi rimasti
	gErroreXtension = InserisciDefuntiNelDocumento(lPtrDefunto, &lScarto);
	if (gErroreXtension != kNessunErrore) 
	{
		// non e' possibile inserire gli annunci nel documento
		return(gErroreXtension);
	}

	if (gXtPreferences.riempitiviPref.usaRingraziamenti == TRUE && gPtrRingraziamenti != NULL) 
	{
		// significa che posso usare i ringraziamenti come
		// riempitivi per tutte le colonne

		// inserisco i ringraziamenti 
		gErroreXtension = InserisciDefuntiNelDocumento(gPtrRingraziamenti, &lScarto);
		if (gErroreXtension != kNessunErrore) 
		{
			// non e' possibile inserire gli anniversari nel documento
			return(gErroreXtension);
		}
	}

	if (gXtPreferences.riempitiviPref.usaAnniversari == TRUE && gPtrAnniversari != NULL) 
	{
		// significa che posso usare gli anniversari come riempitivi per tutte le colonne

		// inserisco gli anniversari
		gErroreXtension = InserisciDefuntiNelDocumento(gPtrAnniversari, &lScarto);
		if (gErroreXtension != kNessunErrore) 
		{
			// non e' possibile inserire gli anniversari nel documento
			return(gErroreXtension);
		}
	}

	return(kNessunErrore);
} // EseguiCalcoloPosizionamento

/* ------------------------------------------------------------------------ *

	ControllaChiusuraColonne

* ------------------------------------------------------------------------ */
static void XTAPI ControllaChiusuraColonne() throw()
{
	// indice per scorrere le pagine
	int16 lPagina = 0;
	// indice per scorrere le colonne
	int16 lColonna = 0;
	// per prendere le info dello spazio della colonna corrente
	spaziocolonna *lPtrSpazioColonna = NULL;
	// per sapere come chiudono le colonne
	tipochiusura lTipoChiusura = kNessunaChiusura;
	// per prendere lo spazio che non si riesce a recuperare
	Fixed lDifferenza = 0;
	// per trasformare la differenza in punti
	int32 lPuntiDifferenza = 0;
	// per trasformare la differenza in frazione di punti
	int32 lFrazioneDifferenza = 0;
	// spazio occupato dai necrologi fuori pagina
	Fixed lIngombro = 0;
	// numero dei necrologi fuori pagina
	int16 lNumeroNecrologi = 0;
	// moduli necessari 
	uint32 lModuli = 0;
	
	// controllo la chiusura delle colonne
	for (lPagina = 1; lPagina <= PrendiNumeroPagine(); lPagina++) 
	{
		for (lColonna = 1; lColonna <= PrendiNumeroColonne(); lColonna++) 
		{
			// prendo le info dello spazio colonna corrente
			lPtrSpazioColonna  = PrendiSpazioColonna(lPagina, lColonna);

			// controllo che ci sia spazio disponibile
			if ((*lPtrSpazioColonna).spazioDisponibile > 0) 
			{
				// controllo la chiusura della colonna corrente
				gErroreXtension = ControllaChiusuraColonna(lPtrSpazioColonna,
															&lTipoChiusura,
															&lDifferenza);
				if (gErroreXtension != kNessunErrore) 
				{
					MostraErrore(gErroreXtension);
					return;
				}
				(*lPtrSpazioColonna).tipoChiusura = lTipoChiusura;
				CalcolaPunti(lDifferenza, &lPuntiDifferenza, &lFrazioneDifferenza);
				if (lTipoChiusura == kPreferenzeNonRispettate) 
				{
					sprintf((char*)gStringaC,
							"pagina %d, colonna %d, spazio irrecuperabile %ld,%ld",
							lPagina, lColonna,
							lPuntiDifferenza,
							lFrazioneDifferenza);
					ConcatenaAllaStringaErrore(gStringaC);
					DaiErrore(kErroreColonnaStretta);
				} 
				else if (lTipoChiusura == kInserireAncora) 
				{
					sprintf((char*)gStringaC,
							"pagina %d, colonna %d, spazio irrecuperabile %ld,%ld",
							lPagina, lColonna,
							lPuntiDifferenza,
							lFrazioneDifferenza);
					ConcatenaAllaStringaErrore(gStringaC);
					DaiErrore(kErroreColonnaLarga);
				}
			}
		}
	}

	// prendo le info sui necrologi fuori pagina
	lPtrSpazioColonna = PrendiSpazioColonnaFuori();

	// prendo il numero dei necrologi fuori pagina
	lNumeroNecrologi = (*lPtrSpazioColonna).numeroNecrologi +
						(*lPtrSpazioColonna).numeroNecrologiDefunti +
						(*lPtrSpazioColonna).numeroNecrologiFondoDefunti;

	if (lNumeroNecrologi != 0) 
	{
		// calcolo quanto spazio ci vuole in piu'
		lIngombro = (*lPtrSpazioColonna).spazioOccupato +
					lNumeroNecrologi * gXtPreferences.giustificazionePref.valoreIniziale;

		// calcolo le pagine, colonne e moduli 
		CalcolaPagineColonneEModuli((uint32) mLong(lIngombro),
									NULL,
									NULL,
									NULL,
									&lModuli);

		sprintf((char*)gStringaC, "necrologi %d", lNumeroNecrologi);
		ConcatenaAllaStringaErrore(gStringaC);
		DaiErrore(kErroreNecrologiFuoriPagina);

		sprintf((char*)gStringaC, "punti %ld", mLong(lIngombro));
		ConcatenaAllaStringaErrore(gStringaC);
		DaiErrore(kErrorePuntiNecessari);

		sprintf((char*)gStringaC, "moduli %ld", lModuli);
		ConcatenaAllaStringaErrore(gStringaC);
		DaiErrore(kErroreModuliNecessari);

	}
} // ControllaChiusuraColonne

/* ------------------------------------------------------------------------ *

	CalcolaPosizionamento

* ------------------------------------------------------------------------ */
errorixtension XTAPI CalcolaPosizionamento() throw()
{
	// per costruire il nome da usare per salvare il documento appena impaginata
	uchar lNomeDocumento[32] = "";

	if (PrendiCodiceDocumento() != PrendiCodiceImpaginazione()) 
	{
		// e' un documento su cui ho gia' importato dati */
		ConcatenaAllaStringaErrore((uchar*)curdocname());
		return(kErrorePosizionamentoSuDocumentoSbagliato);
	}

	// calcolo lo spazio disponibile
	gErroreXtension = CalcolaSpazioDisponibile();
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sul calcolo dello spazio disponibile
		return(gErroreXtension);
	}

	// stampa lo spazio disponibile appena calcolato
	StampaSpazioDisponibile();

	// stampa inizio calcolo posizionamento
	DaiMessaggio(kInizioCalcoloPosizionamento);

	// calcolo lo spazio ottimale
	gErroreXtension = CalcolaSpazioOttimale();
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sul calcolo dello spazio ottimale
		return(gErroreXtension);
	}

	// inserisco la informazione sulla lunghezza dell'annuncio rispetto la colonna
	InserisciInfoLunghezzaPaginaNecrologi();

	// controllo il numero di colonne libere confrontandolo il numero
	// di annunci da posizionare ad inizio colonna 
	gErroreXtension = ControllaRigoreNecrologi();
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sul calcolo dello spazio ottimale
		return(gErroreXtension);
	}

	// ordino le strutture per il posizionamento
	OrdinaInfoNecrologi();

	// trasformo gli array creati in liste
	CreaListeInfoNecrologi();

	// calcolo il posizionamento dei necrologi
	gErroreXtension = EseguiCalcoloPosizionamento();
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sul calcolo del posizionamento
		return(gErroreXtension);
	}

	// fine calcolo posizionamento
	DaiMessaggio(kFineCalcoloPosizionamento);

	// controllo la chiusura delle colonne 
	ControllaChiusuraColonne();

	// disabilito il comando calcolo del posizionamento
	DisabilitaMenu(CALCPOSMENUID);

	// permetto di posizionare in base al calcolo appena fatto
	AbilitaMenu(POSIZIONAMENUID);

	// permetto di riniziare le operazioni di impaginazione
	AbilitaMenu(RIPRISTINADOCMENUID);
	AbilitaMenu(RIPRISTINADATIULTIMAMENUID);

	return(kNessunErrore);
} // CalcolaPosizionamento



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1


void VisualizzaStoLavorando(void);
void VisualizzaStoLavorando()
{
}


/*
errorixtension  StampaFilePerAppWare(void);
errorixtension  StampaFilePerAppWare(void)
{
	return( kNessunErrore );
}
*/

#endif // 0
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
