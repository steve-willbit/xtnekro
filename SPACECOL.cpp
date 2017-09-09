/* ------------------------------------------------------------------------ *

	SpazioColonna.cpp

	12 Aprile 2005					Fabrizio

	Descrizione:
	Contiene le procedure usate per calcolare lo spazio disponibile su un
	documento con box gia' posizionati
	
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

/*
#include "SpaceAva.h"
*/
#include "AppWare.h"
#include "CalcPos.h"
#include "Coord.h"
#include "DocInfo.h"
#include "Errori.h"
#include "InfoNec.h"
#include "Paletta.h"
#include "Pref.h"
#include "ReadFile.h"
#include "XTNecro.h"

#include "SpaceCol.h"

// GLOBALS
// GLOBALS
// GLOBALS

// memorizza per ogni colonna lo spazio disponibile, lo spazio ottimale e lo spazio occupato
// per permettere la disposizione dei necrologi
spaziocolonnaPtr gPtrSpazioColonna = NULL;

// per memorizzare i box fuori pagina
spaziocolonnaPtr gPtrSpazioColonnaFuori = NULL;

// STATICS
// STATICS
// STATICS

// spazio disponibile totale
static uint32 gSpazioDisponibileTotale = 0;

// per calcolare l'allargamento totale possibile
static uint32 gAllargamentoTotale = 0;

// per calcolare il restringimento totale possibile
static uint32 gRestringimentoTotale = 0;

// per salvare l'altezza della colonna ottimale
static Fixed gAltezzaColonnaOttimale = 0;

// per salvare il numero delle pagine del documento al momento del
// calcolo del posizionamento 
static int16 gNumeroPagine = 0;

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	PrendiSpazioColonna

* ------------------------------------------------------------------------ */
spaziocolonna* XTAPI PrendiSpazioColonna(int16 pagina, int16 colonna) throw()
{
	// per settare lo spazio colonna
	spaziocolonna *lPtrSpazioColonna = NULL;
	// per settare lo spazio colonna
	int32 lIndice = 0;
	
	if (gPtrSpazioColonna != NULL) 
	{
		// sono sicuro di aver correttamente creato lo spazio colonna
		lIndice = (pagina - 1) * PrendiNumeroColonne() + (colonna - 1);
		lPtrSpazioColonna = gPtrSpazioColonna + lIndice;
		return(lPtrSpazioColonna);
	} 
	else 
	{
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
		return(NULL);		
	}
} // PrendiSpazioColonna

/* ------------------------------------------------------------------------ *

	PrendiSpazioColonnaFuori

* ------------------------------------------------------------------------ */
spaziocolonna* XTAPI PrendiSpazioColonnaFuori() throw()
{
	if (gPtrSpazioColonnaFuori != NULL) 
	{
		// sono sicuro di aver correttamente creato lo spazio colonna
		return(gPtrSpazioColonnaFuori);
	} 
	else 
	{
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
		return(NULL);		
	}
} // PrendiSpazioColonnaFuori

/* ------------------------------------------------------------------------ *

	PrendiSpazioOttimaleDiPagina

* ------------------------------------------------------------------------ */
Fixed XTAPI PrendiSpazioOttimaleDiPagina(int16 pagina, int16 colonna) throw()
{
	// indice per il for delle colonne
	int16 lColonna = 0;
	// per impostare il descrittore dello spazio delle colonne
	spaziocolonna *lPtrSpazioColonna = NULL;
	// spazio ottimale sulla pagina
	Fixed lSpazioOttimaleDiPagina = 0;
	
	// imposto la variabile per contare lo spazio ottimale di pagina
	lSpazioOttimaleDiPagina = 0;
	
	// calcolo lo spazio che rimane sulla pagina a partire dalla colonna passata
	for (lColonna = PrendiNumeroColonne(); lColonna >= colonna; lColonna--) 
	{
		// prendo le info della pagina corrente
		lPtrSpazioColonna = PrendiSpazioColonna(pagina, lColonna);
		
		// aggiungo lo spazio disponibile di pagina
		lSpazioOttimaleDiPagina += (*lPtrSpazioColonna).spazioOttimale;
		lSpazioOttimaleDiPagina -= (*lPtrSpazioColonna).spazioOccupato;
		lSpazioOttimaleDiPagina -= (*lPtrSpazioColonna).spazioTraNecrologi;
	}
	return(lSpazioOttimaleDiPagina);
} // PrendiSpazioOttimaleDiPagina	

/* ------------------------------------------------------------------------ *

	GetOptimunAvailableSpaceFromDocument
	
* ------------------------------------------------------------------------ */
DoubleFixed XTAPI GetOptimunAvailableSpaceFromDocument(int16 iPage, int16 iColumn) throw()
{
	DoubleFixed space = 0.0;
	int page = 0;
	int column = 0;
	
	// get the space of the current page
	for (column = PrendiNumeroColonne(); column >= iColumn; column--) 
	{
		// get the info of the column
		spaziocolonna* columnSpacePtr = PrendiSpazioColonna(iPage, column);
		
		// add the available space
		space += (DoubleFixed) (*columnSpacePtr).spazioOttimale;
		space -= (DoubleFixed) (*columnSpacePtr).spazioOccupato;
		space -= (DoubleFixed) (*columnSpacePtr).spazioTraNecrologi;
	}

	// get the space of the other pages
	for (page = iPage + 1; page <= PrendiNumeroPagine(); page++)
	{
		for (column = 1; column <= PrendiNumeroColonne(); column++)
		{
			// get the info of the column
			spaziocolonna* columnSpacePtr = PrendiSpazioColonna(page, column);
			
			// add the available space
			space += (DoubleFixed) (*columnSpacePtr).spazioOttimale;
			space -= (DoubleFixed) (*columnSpacePtr).spazioOccupato;
			space -= (DoubleFixed) (*columnSpacePtr).spazioTraNecrologi;
		}
	}
	
	return(space);
	
} // GetOptimunAvailableSpaceFromDocument

/* ------------------------------------------------------------------------ *

	CalcolaSpazioOttimale

* ------------------------------------------------------------------------ */
errorixtension XTAPI CalcolaSpazioOttimale() throw()
{
	// per prendere l'ingombro totale dei necrologi da impaginare
	uint32 lIngombroTotale = 0;
	// per prendere lo spazio recuperabile in base ai parametri delle preferenze 
	uint32 lSpazioRecuperabile = 0;
	// per calcolare la differenza tra l'ingombro totale e lo spazio disponibile
	uint32 lDifferenza = 0;
	// indice per il for sulle colonne 
	int16 lColonna = 0;
	// indice sul for delle pagine
	int16 lPagina = 0;
	// puntatore alle info dello spazio colonna 
	spaziocolonna *lPtrSpazioColonna = NULL;
	// rapporto tra spazio disponibile e ingombro
	double lRapporto = 0.0;
	// per calcolare lo spazio ottimale 
	double lSpazioOttimale = 0.0;

	// carico l'ingombro totale
	lIngombroTotale = PrendiIngombroTotale() + PrendiSpazioTraNecrologiTotale();

	// impostare cosa fare
	cosafare lCosaFare = CosaFare((Fixed) lIngombroTotale,
						 (Fixed) gSpazioDisponibileTotale,
						 (Fixed *) &lDifferenza);

	// calcolo lo spazio recuperabile
	if (lCosaFare == kAllargare) 
	{
		lSpazioRecuperabile = gAllargamentoTotale;
	} 
	else if (lCosaFare == kRestringere)
	{
		lSpazioRecuperabile = gRestringimentoTotale;
	} 
	else 
	{
		lSpazioRecuperabile = 0;
	}

	// controllo che sia possibile recuperare lo spazio che manca
	if (lDifferenza > lSpazioRecuperabile) 
	{
		// i parametri delle preferenze non mi permettono di recuperare
		sprintf((char*)gStringaC, "differenza %ld punti, spazio recuperabile %ld punti",
				lDifferenza, lSpazioRecuperabile);
		ConcatenaAllaStringaErrore(gStringaC);
		if (lCosaFare == kAllargare)
		{
			// si doveva allargare ma lo spazio dato è troppo: devo stringere
			return(kErroreDeviDareMenoSpazio);
		}
		else
		{
			// si doveva restringere ma lo spazio dato è poco: devo allagare
			return(kErroreDeviDarePiuSpazio);
		}
	}

	// calcolo il rapporto tra spazio disponibile e ingombro
	// uchar tmpstr[kDimensionesStringhe] = "";
	// wsprintf(tmpstr, "Errore di divisione per zero.\nSorgente: %s, Linea: %d.",__FILE__, __LINE__);
	// XtAssert(tmpstr, (0 != gSpazioDisponibileTotale));
	assert(0 != gSpazioDisponibileTotale);

	lRapporto = ((double) lIngombroTotale / (double) gSpazioDisponibileTotale);

	// inzializzo l'altezza della colonna ottimale
	gAltezzaColonnaOttimale = 0;
	
	for (lPagina = 1; lPagina <= PrendiNumeroPagine(); lPagina++) 
	{
		for (lColonna = 1; lColonna <= PrendiNumeroColonne(); lColonna++) 
		{
			// prendo le info sullo spazio colonna
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);
			
			// calcolo lo spazio ottimale
			lSpazioOttimale = lRapporto * ((double) (*lPtrSpazioColonna).spazioDisponibile);
			
			// imposto lo spazio ottimale
			(*lPtrSpazioColonna).spazioOttimale = lSpazioOttimale;
			
			if (gAltezzaColonnaOttimale < lSpazioOttimale) 
			{
				// inserisco il valore piu' grande che coincide con l'altezza della colonna
				gAltezzaColonnaOttimale = lSpazioOttimale;
			}
		}
	}
	
	return(kNessunErrore);
} // CalcolaSpazioOttimale

/* ------------------------------------------------------------------------ *

	InizializzaAllargamentoTotale

* ------------------------------------------------------------------------ */
void XTAPI InizializzaAllargamentoTotale() throw()
{
	gAllargamentoTotale = 0;
} // InizializzaAllargamentoTotale

/* ------------------------------------------------------------------------ *

	IncrementaAllargamentoTotale

* ------------------------------------------------------------------------ */
void XTAPI IncrementaAllargamentoTotale(Fixed allargamento) throw()
{
	gAllargamentoTotale += mLong(allargamento);
} // IncrementaAllargamentoTotale

/* ------------------------------------------------------------------------ *

	ImpostaAllargamentoTotale

* ------------------------------------------------------------------------ */
void XTAPI ImpostaAllargamentoTotale(uint32 allargamento) throw()
{
	gAllargamentoTotale = allargamento;
} // ImpostaAllargamentoTotale

/* ------------------------------------------------------------------------ *

	PrendiAllargamentoTotale

* ------------------------------------------------------------------------ */
uint32 PrendiAllargamentoTotale() throw()
{
	return(gAllargamentoTotale);
} // PrendiAllargamentoTotale


/* ------------------------------------------------------------------------ *

	InizializzaRestringimentoTotale

* ------------------------------------------------------------------------ */
void XTAPI InizializzaRestringimentoTotale() throw()
{
	gRestringimentoTotale = 0;
} // InizializzaRestringimentoTotale

/* ------------------------------------------------------------------------ *

	IncrementaRestringimentoTotale

* ------------------------------------------------------------------------ */
void XTAPI IncrementaRestringimentoTotale(Fixed restringimento) throw()
{
	gRestringimentoTotale += mLong(restringimento);
} // IncrementaRestringimentoTotale

/* ------------------------------------------------------------------------ *

	ImpostaRestringimentoTotale

* ------------------------------------------------------------------------ */
void XTAPI ImpostaRestringimentoTotale(uint32 restringimento) throw()
{
	gRestringimentoTotale = restringimento;
} // ImpostaRestringimentoTotale

/* ------------------------------------------------------------------------ *

	PrendiRestringimentoTotale

* ------------------------------------------------------------------------ */
uint32 PrendiRestringimentoTotale() throw()
{
	return(gRestringimentoTotale);
} // PrendiRestringimentoTotale

/* ------------------------------------------------------------------------ *

	LiberaNecrologiSpazioColonna

* ------------------------------------------------------------------------ */
void XTAPI LiberaNecrologiSpazioColonna() throw()
{
	// per settare lo spazio colonna
	spaziocolonna *lPtrSpazioColonna = NULL;
	// per settare lo spazio colonna
	int32 lIndice = 0;
	// per scorrere le pagine
	int16 lPagina = 0;
	// per scorrere le colonne
	int16 lColonna = 0;

	if (gPtrSpazioColonna != NULL) 
	{
		//sono sicuro di aver correttamente creato lo spazio colonna	
		for (lPagina = 0; lPagina < PrendiNumeroPagine(); lPagina++) 
		{
			for (lColonna = 0; lColonna < PrendiNumeroColonne(); lColonna++) 
			{
				lIndice = lPagina * PrendiNumeroColonne() + lColonna;
				lPtrSpazioColonna = gPtrSpazioColonna + lIndice;
				
				// imposto il puntatore alla testa
				LiberaListaNecrologi(&((*lPtrSpazioColonna).testaNecrologi));

				(*lPtrSpazioColonna).pagina = 0;
				(*lPtrSpazioColonna).colonna = 0;
				(*lPtrSpazioColonna).spazioOccupato = 0;
				(*lPtrSpazioColonna).spazioTraNecrologi = 0;
				(*lPtrSpazioColonna).spazioTraParagrafi = 0;
				(*lPtrSpazioColonna).numeroParagrafiDiversi = 0;
				(*lPtrSpazioColonna).numeroParagrafi = 0;
				(*lPtrSpazioColonna).numeroLinee = 0;
				(*lPtrSpazioColonna).numeroNecrologi = 0;
				(*lPtrSpazioColonna).numeroNecrologiDefunti = 0;
				(*lPtrSpazioColonna).numeroNecrologiFondoDefunti = 0;
				(*lPtrSpazioColonna).chiusuraColonna = kAperta;
				(*lPtrSpazioColonna).tipoChiusura = kNessunaChiusura;
				(*lPtrSpazioColonna).testaNecrologi = NULL;
				(*lPtrSpazioColonna).codaNecrologi = NULL;
				(*lPtrSpazioColonna).testaDefunti = NULL;
				(*lPtrSpazioColonna).codaDefunti = NULL;
				(*lPtrSpazioColonna).testaFondoDefunti = NULL;
				(*lPtrSpazioColonna).codaFondoDefunti = NULL;
			}
		}
		
	} 
	else 
	{
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
	}
} // LiberaNecrologiSpazioColonna

/* ------------------------------------------------------------------------ *

	LiberaNecrologiSpazioColonnaFuori

* ------------------------------------------------------------------------ */
void XTAPI LiberaNecrologiSpazioColonnaFuori() throw()
{
	if (gPtrSpazioColonnaFuori != NULL) 
	{
		// sono sicuro di aver correttamente creato lo spazio colonna fuori
		// libero la lista dei necrologi 
		LiberaListaNecrologi(&((*gPtrSpazioColonnaFuori).testaNecrologi));
		
		(*gPtrSpazioColonnaFuori).pagina = 0;
		(*gPtrSpazioColonnaFuori).colonna = 0;
		(*gPtrSpazioColonnaFuori).spazioOccupato = 0;
		(*gPtrSpazioColonnaFuori).spazioTraNecrologi = 0;
		(*gPtrSpazioColonnaFuori).spazioTraParagrafi = 0;
		(*gPtrSpazioColonnaFuori).numeroParagrafiDiversi = 0;
		(*gPtrSpazioColonnaFuori).numeroParagrafi = 0;
		(*gPtrSpazioColonnaFuori).numeroLinee = 0;
		(*gPtrSpazioColonnaFuori).numeroNecrologi = 0;
		(*gPtrSpazioColonnaFuori).numeroNecrologiDefunti = 0;
		(*gPtrSpazioColonnaFuori).numeroNecrologiFondoDefunti = 0;
		(*gPtrSpazioColonnaFuori).chiusuraColonna = kAperta;
		(*gPtrSpazioColonnaFuori).tipoChiusura = kNessunaChiusura;
		(*gPtrSpazioColonnaFuori).testaNecrologi = NULL;
		(*gPtrSpazioColonnaFuori).codaNecrologi = NULL;
		(*gPtrSpazioColonnaFuori).testaDefunti = NULL;
		(*gPtrSpazioColonnaFuori).codaDefunti = NULL;
		(*gPtrSpazioColonnaFuori).testaFondoDefunti = NULL;
		(*gPtrSpazioColonnaFuori).codaFondoDefunti = NULL;
	} 
	else 
	{
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
	}
} // LiberaNecrologiSpazioColonnaFuori

/* ------------------------------------------------------------------------ *

	PrendiSpazioDisponibileTotale

* ------------------------------------------------------------------------ */
uint32 PrendiSpazioDisponibileTotale() throw()
{
	return(gSpazioDisponibileTotale);
} // PrendiSpazioDisponibileTotale

/* ------------------------------------------------------------------------ *

	InizializzaSpazioDisponibileTotale

* ------------------------------------------------------------------------ */
void XTAPI InizializzaSpazioDisponibileTotale() throw()
{
	gSpazioDisponibileTotale = 0;
} // InizializzaSpazioDisponibileTotale

/* ------------------------------------------------------------------------ *

	IncrementaSpazioDisponibileTotale

* ------------------------------------------------------------------------ */
void XTAPI IncrementaSpazioDisponibileTotale(Fixed spaziodisponibile) throw()
{
	gSpazioDisponibileTotale += mLong(spaziodisponibile);
} // IncrementaSpazioDisponibileTotale

/* ------------------------------------------------------------------------ *

	PrendiAltezzaColonnaOttimale

* ------------------------------------------------------------------------ */
Fixed XTAPI PrendiAltezzaColonnaOttimale() throw()
{
	return(gAltezzaColonnaOttimale);
} // PrendiAltezzaColonnaOttimale

/* ------------------------------------------------------------------------ *

	CalcolaPagineColonneEModuli

* ------------------------------------------------------------------------ */
void XTAPI  CalcolaPagineColonneEModuli(uint32 spazio, uint32 *ptrpagine,uint32 *ptrcolonne, 
							  uint32 *ptrfrazionecolonna,  uint32 *ptrmoduli) throw()
{
	Fixed lPuntiPerColonna = 0;
	Fixed lPuntiPerPagina = 0;
	uint32 lPagine = 0;
	uint32 lPuntiColonne = 0;
	uint32 lColonne = 0;
	uint32 lFrazioneColonna = 0;

	if (ptrmoduli != NULL)
	{
		// wsprintf(tmpstr, "Errore di divisione per zero.\nSorgente: %s, Linea: %d.",__FILE__, __LINE__);
		// XtAssert(tmpstr, (0 != mLong(kFixedAltezzaModulo + kFixedDistanzaModuli)));
		assert(0 != mLong(gXtPreferences.documentoPref.altezzaModulo + gXtPreferences.documentoPref.distanzaModuli));
		(*ptrmoduli) = spazio / mLong(gXtPreferences.documentoPref.altezzaModulo + gXtPreferences.documentoPref.distanzaModuli);
	}

	// calcolo i punti per pagina e per colonna
	lPuntiPerColonna = gXtPreferences.documentoPref.altezzaModulo * (gXtPreferences.documentoPref.moduliColonna) +
					   gXtPreferences.documentoPref.distanzaModuli * ((gXtPreferences.documentoPref.moduliColonna) - 1L);
					   
	lPuntiPerPagina = lPuntiPerColonna * PrendiNumeroColonne();

	// calcolo il numero di pagine
	// wsprintf(tmpstr, "Errore di divisione per zero.\nSorgente: %s, Linea: %d.",__FILE__, __LINE__);
	// XtAssert(tmpstr, (0 != mLong(lPuntiPerPagina)));
	assert(0 != mLong(lPuntiPerPagina));
	lPagine = spazio / mLong(lPuntiPerPagina); 	

	// calcolo le colonne
	lPuntiColonne = spazio - (lPagine * mLong(lPuntiPerPagina));
		
	// wsprintf(tmpstr, "Errore di divisione per zero.\nSorgente: %s, Linea: %d.",__FILE__, __LINE__);
	// XtAssert(tmpstr, (0 != mLong(lPuntiPerPagina)));
	assert(0 != mLong(lPuntiPerPagina));

	lColonne = lPuntiColonne / mLong(lPuntiPerColonna);		 
	lFrazioneColonna = lPuntiColonne % mLong(lPuntiPerColonna);
	lFrazioneColonna *= 100L;

	// wsprintf(tmpstr, "Errore di divisione per zero.\nSorgente: %s, Linea: %d.",__FILE__, __LINE__);
	// XtAssert(tmpstr, (0 != mLong(lPuntiPerColonna)));
	assert(0 != mLong(lPuntiPerColonna));

	lFrazioneColonna /= mLong(lPuntiPerColonna);
	lFrazioneColonna += 5L;
	lFrazioneColonna /=10L;
	if (lFrazioneColonna == 10L)
	{
		lFrazioneColonna = 0;
		lColonne++;
		if (lColonne == PrendiNumeroColonne())
		{
			lColonne = 0;
			lPagine++;
		}
	}
		 
	if (ptrpagine != NULL)
		(*ptrpagine) = lPagine;
	if (ptrcolonne != NULL)
		(*ptrcolonne) = lColonne;
	if (ptrfrazionecolonna != NULL)
		(*ptrfrazionecolonna) = lFrazioneColonna;
} // CalcolaPagineColonneEModuli

/* ------------------------------------------------------------------------ *

	ScegliColonna

* ------------------------------------------------------------------------ */
void XTAPI ScegliColonna(spaziocolonna *ptrspazicolonna, int16 numerocolonne, int16 *indicespazicolonne) throw()
{
	// per prendere le info delle colonne 
	spaziocolonna *lPtrSpazioColonna = NULL;
	// per scorrere la colonna trovata
	int16 lIndiceSpaziColonne = 0;
	// per ritornare l'indice della colonna trovata 
	int16 lIndiceTrovato = 0;
	// per memorizzare il massimo spazio lasciato libero
	Fixed lMassimoSpazio = 0;
	// per calcolare la differenza tra spazio occupato considerando l'altezza del defunto e lo spazio disponibile	
	Fixed lDifferenza = 0;
	// per sapere se ho trovato la colonna
	Boolean lTrovata = FALSE;

	for (lIndiceSpaziColonne = 0; lIndiceSpaziColonne < numerocolonne; lIndiceSpaziColonne++) 
	{
		lPtrSpazioColonna = ptrspazicolonna + lIndiceSpaziColonne;
			
		lDifferenza = (*lPtrSpazioColonna).spazioOttimale -
					   ((*lPtrSpazioColonna).spazioOccupato +
					  	(*lPtrSpazioColonna).spazioTraNecrologi);

		// significa che c'e' ancora spazio disponibile
		if (lDifferenza > lMassimoSpazio || lTrovata == FALSE) 
		{
			lMassimoSpazio = lDifferenza;
			lIndiceTrovato = lIndiceSpaziColonne;
			lTrovata = TRUE;
		}
	}
	
	(*indicespazicolonne) = lIndiceTrovato;
} // ScegliColonna

/* ------------------------------------------------------------------------ *

	CalcolaPunti

* ------------------------------------------------------------------------ */
void XTAPI CalcolaPunti(Fixed spazio, int32 *intero, int32 *frazione) throw()
{
	(*intero) = mLong(spazio);
	(*frazione) = ((spazio % 65536) * 10000) / 65536;
} // CalcolaPunti     

/* ------------------------------------------------------------------------ *

	ImpostaNumeroPagine
	
* ------------------------------------------------------------------------ */
void XTAPI ImpostaNumeroPagine(int16 numeropagine) throw()
{
	gNumeroPagine = numeropagine;
} // ImpostaNumeroPagine

/* ------------------------------------------------------------------------ *

	PrendiNumeroPagine

* ------------------------------------------------------------------------ */
int16 XTAPI PrendiNumeroPagine() throw()
{
	return(gNumeroPagine);
} // PrendiNumeroPagine
