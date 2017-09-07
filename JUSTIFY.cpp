/* ------------------------------------------------------------------------ *

	Giustificazione.cpp

	18 Aprile 2005 				Fabrizio

	Copyright © Sinedita S.r.l 2005. All Rights Reserved.

	Descrizione:
	Per giustificare verticalmente il box delle necro

	$Log: not supported by cvs2svn $
	Revision 1.1  2005/04/20 07:14:19  taretto
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"
#include "DbpAssert.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES


#include "Errori.h"
#include "ReadFile.h"
#include "AppWare.h"
#include "InfoNec.h"
#include "SpaceCol.h"
#include "CalcPos.h"
#include "Box.h"
#include "Pref.h"

#include "Justify.h"

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			ImpostaVariazioneAltezza
	@abstract		gestione giustificazione
	@discussion		Visualizza la dialog per testare l'algoritmo di giustificazione.
					Permette di indicare la variazione di altezza del box corrente

					3 Maggio 2005 - Fabrizio.

	@param			nessuno.
	@result			ritorna TRUE se e' stato pigiato OK, FALSE altrimenti
*/
static Boolean XTAPI ImpostaVariazioneAltezza() throw();

/*! 
	@function			AumentaSpazioTraParagrafi
	@abstract		gestione giustificazione
	@discussion		Aumenta lo spazio tra i paragrafi in modo da recuperare lo spazio passato.
					Modifica solo i paragrafi con leading diverso da zero.

					3 Maggio 2005 - Fabrizio.

	@param			spaziodarecuperare - contiene lo spazio da recuperare.
	@param			numeroparagrafi - numero dei pargrafi dove distribuire lo spazio.
	@result			nessuno
*/
static void XTAPI AumentaSpazioTraParagrafi(Fixed spaziodarecuperare, int32 numeroparagrafi) throw();

/*! 
	@function			DiminuisciSpazioTraParagrafi
	@abstract		gestione giustificazione
	@discussion		Diminuisce lo spazio tra i paragrafi in modo da recuperare lo spazio passato.
					Modifica solo i paragrafi con leading diverso da zero.

					3 Maggio 2005 - Fabrizio.

	@param			spaziodarecuperare - contiene lo spazio da recuperare.
	@param			numeroparagrafi - numero dei pargrafi dove distribuire lo spazio.
	@result			nessuno
*/
static void XTAPI DiminuisciSpazioTraParagrafi(Fixed spaziodarecuperare, int32 numeroparagrafi) throw();

/*! 
	@function			TogliTuttoLoSpazioTraParagrafi
	@abstract		gestione giustificazione
	@discussion		Toglie tutto lo spazio tra i paragrafi.

					3 Maggio 2005 - Fabrizio.

	@param			nessuno
	@result			nessuno
*/
static void XTAPI TogliTuttoLoSpazioTraParagrafi() throw();

/*! 
	@function			LavoraSullaInterlinea
	@abstract		gestione giustificazione
	@discussion		Modifica l'interlinea di ogni riga del box corrente
					Modifica solo le righe con leading diverso da zero.

					3 Maggio 2005 - Fabrizio.

	@param			spaziodarecuperare - contiene lo spazio da recuperare. Se negativo bisogna 
					ridurre l'interlinea, tra i paragrafi se e' positivo bisogna aumentare
					l'interlinea. Questo valore deve essere applicato a tutte le linee senza
					autoleading.
	@param			numerolinee - numero delle linee dove fare l'applicazione				
	@result			nessuno
*/
static void XTAPI LavoraSullaInterlinea(Fixed spaziodarecuperare, int32 numerolinee) throw();

/*! 
	@function			VariazioneInterlinea
	@abstract		gestione giustificazione
	@discussion		Modifica l'interlinea di un solo fixed.

					3 Maggio 2005 - Fabrizio.

	@param			spaziodarecuperare - contiene lo spazio da recuperare. Se negativo bisogna 
					ridurre l'interlinea, tra i paragrafi se Š positivo bisogna aumentare
					l'interlinea.
	@result			nessuno
*/
static void XTAPI VariazioneInterlinea(Fixed spaziodarecuperare) throw();

/*! 
	@function			ControllaTesto
	@abstract		gestione giustificazione
	@discussion		Controllo che non cia overflow nel box di testo dobve appena finito di
					lavorare.

					3 Maggio 2005 - Fabrizio.

	@param			nessuno.
	@result			nessuno
*/
static void XTAPI ControllaTesto() throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	PrendiNumeroLinee

* ------------------------------------------------------------------------ */
int32 XTAPI PrendiNumeroLinee() throw()
{
	// handle al flusso di testo corrente
	xehandle lHndlXE = NULL;
	// inizio del testo attualmente selezionato */
	int32 lInizioSelezione = 0;
	// fine del testo attualmente selezionato 
	int32 lFineSelezione = 0;
	// inizio paragrafo 
	int32 lInizio = 0;
	// fine paragrafo 
	int32 lFine = 0;
	// lunghezza del flusso di testo corrente 
	int32 lLunghezzaTesto = 0;
	// handle alle info dei paragrafi 
	prechandle	lHndlParagrafi = NULL;
	// contiene il numero di info sui paragrafi 
	uint32 lNumeroParagrafi = 0;
	// contatore delle linee del box 
	int32 lNumeroLinee = 0;
	// indice per il for sui paragrafi 
	int16 i = 0;
	// per leggere l'interlinea del paragrafo 
	Fixed lInterlinea = 0;
	// handle al LOS del box di testo corrente 
	loshandle lHndlLos = NULL;
	// lunghezza del LOS corrente 
	int32 lLunghezzaLos = 0;
	// identificatore del box di testo corrente 
	boxid lIdBoxCorrente = 0;
	// posizione del primo carattere del flusso di testo del box 
	int32 lPosPrimoCarattere = 0;
	// coordinate del box alta a sinistra 
	fixedpoint lCoordinateBox = {0, 0};
	// indice per il for sul LOS 
	int32 j = 0;
	// per evitare di partire sempre da zero nell'analizzare il LOS 
	int32 lBaseLos = 0;
	// per sapere quando e' possibile interrompere l'analisi del Los 
	Boolean lInterrompere = FALSE;
	// id del box corrente
	boxid curbox;
	
	// prendo la selezione corrente
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	if (lLunghezzaTesto == 0) 
		return(0);
	
	// prendo le info dei paragrafi del box corrente
	lHndlParagrafi = (prechandle) NewHandle(0);
	xtget_curbox(&curbox);
	xtgetxepstuff(curbox, &lNumeroParagrafi, &lHndlParagrafi);
	
	// prendo il LOS del box corrente
	xtget_curbox(&curbox);
	lIdBoxCorrente = curbox;
	lPosPrimoCarattere = 0;
	lHndlLos = (loshandle) NewHandle(0);
	xtgetlos(&lIdBoxCorrente, &lPosPrimoCarattere, FALSE, &lCoordinateBox, &lHndlLos);
	lLunghezzaLos = GetHandleSize((Handle) lHndlLos) / sizeof(losrec);

	// analizzo i paragrafi presenti nel box
	lInizio = lFine = 0;
	lBaseLos = 0;
	for (i = 0; i < lNumeroParagrafi - 1; i++) 
	{
	
		lFine += (**lHndlParagrafi)[i].len;
		xesetsel (lInizio, lFine, FALSE);
		lInterlinea = getparafmt(lHndlXE, P_LEADING, NULL);
		
		lInterrompere = FALSE;
		
		// se l'interlinea e' diversa da 0 conto le line di questo paragrafo
		if (lInterlinea != 0) 
		{
			for (j = lBaseLos; j < lLunghezzaLos; j++) 
			{
			
				if ((**lHndlLos)[j].leading < SPECLOS &&
					lInizio <= (**lHndlLos)[j].chpos &&
					(**lHndlLos)[j].chpos < lFine) 
				{
					lNumeroLinee++;
					lInterrompere = TRUE;
				} 
				else 
				{
					lBaseLos = j;
					if (lInterrompere == TRUE) 
						break;
				}
			}
		}
   		lInizio = lFine;
	}

	// analizzo l'untimo paragrafo
	lFine += (**lHndlParagrafi)[i].len;
	xesetsel (lInizio, lFine, FALSE);
	lInterlinea = getparafmt(lHndlXE, P_LEADING, NULL);
	
	// se l'interlinea e' diversa da 0 conto le linee di questo paragrafo
	if (lInterlinea != 0) 
	{
		for (j = lBaseLos; j < lLunghezzaLos; j++) 
		{
			if ((**lHndlLos)[j].leading < SPECLOS &&
				lInizio <= (**lHndlLos)[j].chpos &&
				(**lHndlLos)[j].chpos <= lFine) 
			{
				lNumeroLinee++;
			}
		}
	}
	
	DisposeHandle((Handle) lHndlParagrafi);
	DisposeHandle((Handle) lHndlLos);
	
	// ripristino la selezione iniziale
	xesetsel(0, 0, TRUE);
	xecalc(XEDOALL);
	xesetsel(lInizioSelezione, lFineSelezione, FALSE);

	return(lNumeroLinee);
} // PrendiNumeroLinee

/* ------------------------------------------------------------------------ *

	PrendiSpazioTraParagrafi

* ------------------------------------------------------------------------ */
Fixed XTAPI PrendiSpazioTraParagrafi() throw()
{
	// handle al flusso di testo corrente
	xehandle lHndlXE = NULL;
	// inizio del testo attualmente selezionato 
	int32 lInizioSelezione = 0;
	// fine del testo attualmente selezionato 
	int32 lFineSelezione = 0;
	// inizio paragrafo 
	int32 lInizio = 0;
	// fine paragrafo 
	int32 lFine = 0;
	// lunghezza del flusso di testo corrente 
	int32 lLunghezzaTesto = 0;
	// handle alle info dei paragrafi 
	prechandle	lHndlParagrafi = NULL;
	// contiene il numero di info sui paragrafi 
	uint32 lNumeroParagrafi = 0;
	// contatore dello spazio tra i paragrafi del box 
	Fixed lSpazioTraParagrafi = 0;
	// indice per il for sui paragrafi 
	int16 i = 0;
	// interlinea del paragrafo correte 
	Fixed lInterlinea = 0;
	// spazio prima del paragrafo corrente 
	Fixed lSpazioPrima = 0;
	// spazio dopo del paragrafo corrente 
	Fixed lSpazioDopo = 0;
	// id del box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	// prendo la selezione corrente 
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	
	// prendo le info dei paragrafi del box corrente
	lHndlParagrafi = (prechandle) NewHandle(0);
	xtgetxepstuff(curbox, &lNumeroParagrafi, &lHndlParagrafi);
	
	// analizzo i paragrafi presenti nel box
	lInizio = lFine = 0;
	for (i = 0; i < lNumeroParagrafi; i++) 
	{
	
		lFine += (**lHndlParagrafi)[i].len;
		xesetsel(lInizio, lFine, FALSE);
		lInterlinea = getparafmt(lHndlXE, P_LEADING, NULL);
				
		// se l'interlinea e' diversa da 0 prendo lo spazio prima e dopo
		if (lInterlinea != 0) 
		{
			lSpazioPrima = getparafmt(lHndlXE, P_SPCBEFORE, NULL);
			lSpazioDopo = getparafmt(lHndlXE, P_SPCAFTER, NULL);
			lSpazioTraParagrafi += lSpazioDopo + lSpazioPrima;
		}
   		lInizio = lFine;
	}
	
	DisposeHandle((Handle)lHndlParagrafi);
	
	// ripristino la selezione iniziale
	xesetsel(0, 0, TRUE);
	xecalc(XEDOALL);
	xesetsel(lInizioSelezione, lFineSelezione, FALSE);

	return(lSpazioTraParagrafi);
} // PrendiSpazioTraParagrafi

/* ------------------------------------------------------------------------ *

	PrendiNumeroParagrafiDiversi

* ------------------------------------------------------------------------ */
int32 XTAPI PrendiNumeroParagrafiDiversi() throw()
{
	// handle al flusso di testo corrente 
	xehandle lHndlXE = NULL;
	// inizio del testo attualmente selezionato 
	int32 lInizioSelezione = 0;
	// fine del testo attualmente selezionato 
	int32 lFineSelezione = 0;
	// inizio paragrafo 
	int32 lInizio = 0;
	// fine paragrafo 
	int32 lFine = 0;
	// lunghezza del flusso di testo corrente 
	int32 lLunghezzaTesto = 0;
	// handle alle info dei paragrafi 
	prechandle	lHndlParagrafi = NULL;
	// contiene il numero di info sui paragrafi 
	uint32 lNumeroParagrafi = 0;
	// contatore dei paragrafi del box 
	int32 lContatoreNumeroParagrafi = 0;
	// indice per il for sui paragrafi 
	int16 i = 0;
	// interlinea del paragrafo corrente 
	Fixed lInterlinea = 0;
	// spazio prima del paragrafo 
	Fixed lSpazioPrima = 0;
	// spazio dopo del paragrafo 
	Fixed lSpazioDopo = 0;
	// id del box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	// prendo la selezione corrente
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	
	// prendo le info dei paragrafi del box corrente
	lHndlParagrafi = (prechandle) NewHandle(0);
	xtgetxepstuff(curbox, &lNumeroParagrafi, &lHndlParagrafi);
	
	// analizzo i paragrafi presenti nel box
	lInizio = lFine = 0;
	for (i = 0; i < lNumeroParagrafi; i++) 
	{
	
		lFine += (**lHndlParagrafi)[i].len;
		xesetsel(lInizio, lFine, FALSE);
		lInterlinea = getparafmt(lHndlXE, P_LEADING, NULL);
		lSpazioPrima = getparafmt(lHndlXE, P_SPCBEFORE, NULL);
		lSpazioDopo = getparafmt(lHndlXE, P_SPCAFTER, NULL);
				
		// se l'interlinea e' diversa da 0 conto il paragrafo
		if (lInterlinea != 0) 
		{
			if (lSpazioPrima != 0 || lSpazioDopo != 0) 
				lContatoreNumeroParagrafi++;
		}
   		lInizio = lFine;
	}
	
	DisposeHandle((Handle)lHndlParagrafi);
	
	// ripristino la selezione iniziale
	xesetsel(0, 0, TRUE);
	xecalc(XEDOALL);
	xesetsel(lInizioSelezione, lFineSelezione, FALSE);

	return(lContatoreNumeroParagrafi);
} // PrendiNumeroParagrafiDiversi

/* ------------------------------------------------------------------------ *

	PrendiNumeroParagrafi

* ------------------------------------------------------------------------ */
int32 XTAPI PrendiNumeroParagrafi() throw()
{
	// handle al flusso di testo corrente 
	xehandle lHndlXE = NULL;
	// inizio del testo attualmente selezionato 
	int32 lInizioSelezione = 0;
	// fine del testo attualmente selezionato 
	int32 lFineSelezione = 0;
	// inizio paragrafo 
	int32 lInizio = 0;
	// fine paragrafo 
	int32 lFine = 0;
	// lunghezza del flusso di testo corrente 
	int32 lLunghezzaTesto = 0;
	// handle alle info dei paragrafi 
	prechandle	lHndlParagrafi = NULL;
	// contiene il numero di info sui paragrafi 
	uint32 lNumeroParagrafi = 0;
	// contatore dei paragrafi del box 
	int32 lContatoreNumeroParagrafi = 0;
	// indice per il for sui paragrafi 
	int16 i = 0;
	// interlinea del paragrafo corrente 
	Fixed lInterlinea = 0;
	// id del box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	// prendo la selezione corrente 
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	
	// prendo le info dei paragrafi del box corrente 
	lHndlParagrafi = (prechandle) NewHandle(0);
	xtgetxepstuff(curbox, &lNumeroParagrafi, &lHndlParagrafi);
	
	// analizzo i paragrafi presenti nel box 
	lInizio = lFine = 0;
	for (i = 0; i < lNumeroParagrafi; i++) 
	{
	
		lFine += (**lHndlParagrafi)[i].len;
		xesetsel(lInizio, lFine, FALSE);
		lInterlinea = getparafmt(lHndlXE, P_LEADING, NULL);
				
		// se l'interlinea e' diversa da 0 conto il paragrafo 
		if (lInterlinea != 0) 
			lContatoreNumeroParagrafi++;
   		lInizio = lFine;
	}
	
	DisposeHandle((Handle)lHndlParagrafi);
	
	// ripristino la selezione iniziale 
	xesetsel(0, 0, TRUE);
	xecalc(XEDOALL);
	xesetsel(lInizioSelezione, lFineSelezione, FALSE);

	return(lContatoreNumeroParagrafi);
} // PrendiNumeroParagrafi

/* ------------------------------------------------------------------------ *

	ImpostaVariazioneAltezza

* ------------------------------------------------------------------------ */
static Boolean XTAPI ImpostaVariazioneAltezza() throw()
{
	// per salvare il port corrente
	GrafPtr	lVecchioPorco = NULL;
	// per sapere se e' stato pigiato l'ok 
	Boolean lBool = FALSE;

#ifdef OPERATION

	/* salvataggio del port corrente */
	GetPort(&lVecchioPorco);


	/* apertura della dialog per la variazione dell'altezza */
	frontmost = (WindowPtr) GetNewDialog(rFinestraVariazioneAltezza, NULL, (WindowPtr) -1L);

	/* imposto il campo della variazione della finestra */
	fsetfield((DialogPtr) frontmost, iFixedVariazioneAltezza,  DEFUNITS, 1,
		  kFixedVariazioneAltezza);

	displaywindow(frontmost, TRUE);

	BeginUpdate((WindowPtr)frontmost);
	outlines((DialogPtr) frontmost);
	DrawDialog((DialogPtr) frontmost);
	EndUpdate((WindowPtr) frontmost);

	/* gestione dei click sulla finestra */
	lBool = GestioneFinestraVariazioneAltezza((DialogPtr) frontmost);

	/* chiusura della finestra */
	restorewnds((WindowPeek) frontmost, TRUE);

	/* ripristino del porco vecchio */
	SetPort(lVecchioPorco);
#endif
	return(lBool);
} // ImpostaVariazioneAltezza

/* ------------------------------------------------------------------------ *

	AumentaSpazioTraParagrafi

* ------------------------------------------------------------------------ */
static void XTAPI AumentaSpazioTraParagrafi(Fixed spaziodarecuperare, int32 numeroparagrafi) throw()
{
	// handle al flusso di testo corrente
	xehandle lHndlXE = NULL;
	// inizio del testo attualmente selezionato
	int32 lInizioSelezione = 0;
	// fine del testo attualmente selezionato
	int32 lFineSelezione = 0;
	// inizio paragrafo
	int32 lInizio = 0;
	// fine paragrafo
	int32 lFine = 0;
	// lunghezza del flusso di testo corrente
	int32 lLunghezzaTesto = 0;
	// handle alle info dei paragrafi
	prechandle	lHndlParagrafi = NULL;
	// contiene il numero di info sui paragrafi
	uint32 lNumeroParagrafi = 0;
	// indice per il for sui paragrafi
	int16 i = 0;
	// interlinea del paragrafo corrente
	Fixed lInterlinea = 0;
	// per impostare lo spazio prima
	Fixed lSpazioPrima = 0;
	// per impostare lo spazio prima
	Fixed lSpazioDopo = 0;
	// spazio da recuperare per paragrafo
	Fixed lSpazioPerParagrafo = 0;
	// per non perdere spazio a causa delle divisioni
	Fixed lScartoTotale = 0;
	// per non perdere spazio a causa delle divisioni
	Fixed lScarto = 0;
	

	// controllo se c'e' dello spazio da recupera
	if (spaziodarecuperare == 0) 
		return;
	
	// calcolo lo spazio da recuperare per paragrafo
	assert(0 != numeroparagrafi);

	lSpazioPerParagrafo = spaziodarecuperare / numeroparagrafi;
	lScartoTotale = spaziodarecuperare % numeroparagrafi;
	
	// prendo la selezione corrente
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	
	// prendo le info dei paragrafi del box corrente
	lHndlParagrafi = (prechandle) NewHandle(0);
	boxid curbox;
	xtget_curbox(&curbox);
	xtgetxepstuff(curbox, &lNumeroParagrafi, &lHndlParagrafi);
	
	// modifico lo spazio tra i paragrafi
	lInizio = lFine = 0;
	for (i = 0; i < lNumeroParagrafi; i++) 
	{
		lFine += (**lHndlParagrafi)[i].len;
		xesetsel(lInizio, lFine, FALSE);
		lInterlinea = getparafmt(lHndlXE, P_LEADING, NULL);
				
		// se l'interlinea e' diversa da 0 modifico lo spazio tra i paragrafi
		if (lInterlinea != 0) 
		{
 			lSpazioPrima = getparafmt(lHndlXE, P_SPCBEFORE, NULL);
			lSpazioDopo = getparafmt(lHndlXE, P_SPCAFTER, NULL);
			
			if ((lSpazioPrima != 0 && lSpazioDopo != 0) ||
				(lSpazioPrima == 0 && lSpazioDopo == 0)) 
			{	
				// lo spazio da recuperare lo distribuisco sia sopra che sotto
				lSpazioPrima += lSpazioPerParagrafo / 2L;
				lSpazioDopo += lSpazioPerParagrafo / 2L;
				lScarto = lSpazioPerParagrafo % 2L;
				lSpazioDopo += lScarto;
				
				if (lScartoTotale != 0) 
				{
					lSpazioDopo += lScartoTotale;
					lScartoTotale = 0;
				}
				
				// controllo che non siano diventati negativi
				if (lSpazioPrima < 0) 
				{
					lScartoTotale += lSpazioPrima;
					lSpazioPrima = 0;
				}
				if (lSpazioDopo < 0) 
				{
					lScartoTotale += lSpazioDopo;
					lSpazioDopo = 0;
				}
				
				// imposto il nuovo spazio tra i paragrafi
				setparafmt(lHndlXE, P_SPCBEFORE, lSpazioPrima, NULL);
				setparafmt(lHndlXE, P_SPCAFTER, lSpazioDopo, NULL);	
			} 
			else 
			{
				// lo spazio da recuperare solo sull'unico spazio divesro da zero
				if (lSpazioPrima != 0) 
				{
					lSpazioPrima += lSpazioPerParagrafo;
					
					if (lScartoTotale != 0) 
					{
						lSpazioPrima += lScartoTotale;
						lScartoTotale = 0;
					}
					
					// controllo che non siano diventati negativi
					if (lSpazioPrima < 0) 
					{
						lScartoTotale += lSpazioPrima;
						lSpazioPrima = 0;
					}

					setparafmt(lHndlXE, P_SPCBEFORE, lSpazioPrima, NULL);
					
				} 
				else if (lSpazioDopo != 0) 
				{
					lSpazioDopo += lSpazioPerParagrafo;
					
					if (lScartoTotale != 0) 
					{
						lSpazioDopo += lScartoTotale;
						lScartoTotale = 0;
					}

					// controllo che non siano diventati negativi
					if (lSpazioDopo < 0) 
					{
						lScartoTotale += lSpazioDopo;
						lSpazioDopo = 0;
					}

					setparafmt(lHndlXE, P_SPCAFTER, lSpazioDopo, NULL);
				}
			}
		}
  		lInizio = lFine;
	}
	
	DisposeHandle((Handle)lHndlParagrafi);
	
	// ripristino la selezione iniziale
	xesetsel(0, 0, TRUE);
	xecalc(XEDOALL);
	xesetsel(lInizioSelezione, lFineSelezione, FALSE);
} // AumentaSpazioTraParagrafi

/* ------------------------------------------------------------------------ *

	DiminuisciSpazioTraParagrafi

* ------------------------------------------------------------------------ */
static void XTAPI DiminuisciSpazioTraParagrafi(Fixed spaziodarecuperare, int32 numeroparagrafi) throw()
{
	// handle al flusso di testo corrente
	xehandle lHndlXE = NULL;
	// inizio del testo attualmente selezionato
	int32 lInizioSelezione = 0;
	// fine del testo attualmente selezionato
	int32 lFineSelezione = 0;
	// inizio paragrafo
	int32 lInizio = 0;
	// fine paragrafo
	int32 lFine = 0;
	// lunghezza del flusso di testo corrente
	int32 lLunghezzaTesto = 0;
	// handle alle info dei paragrafi
	prechandle	lHndlParagrafi = NULL;
	// contiene il numero di info sui paragrafi
	uint32 lNumeroParagrafi = 0;
	// indice per il for sui paragrafi
	int16 i = 0;
	// interlinea del paragrafo corrente
	Fixed lInterlinea = 0;
	// per impostare lo spazio prima
	Fixed lSpazioPrima = 0;
	// per impostare lo spazio prima
	Fixed lSpazioDopo = 0;
	// spazio da recuperare per paragrafo
	Fixed lSpazioPerParagrafo = 0;
	// per non perdere spazio a causa delle divisioni
	Fixed lScartoTotale = 0;
	// per non perdere spazio a causa delle divisioni
	Fixed lScarto = 0;
	// cantatore dei paragrafi con spazio divesro da zero
	int32 lNumeroParagrafiDiversi = 0;

	// controllo se c'e' dello spazio da recupera
	if (spaziodarecuperare == 0) 
		return;
	
	// calcolo lo spazio da recuperare per paragrafo
	assert(0 != numeroparagrafi);

	lSpazioPerParagrafo = spaziodarecuperare / numeroparagrafi;
	lScartoTotale = spaziodarecuperare % numeroparagrafi;
	
	// prendo la selezione corrente
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	
	// prendo le info dei paragrafi del box corrente
	lHndlParagrafi = (prechandle) NewHandle(0);
	boxid curbox;
	xtget_curbox(&curbox);
	xtgetxepstuff(curbox, &lNumeroParagrafi, &lHndlParagrafi);
	
	// modifico lo spazio tra i paragrafi
	lInizio = lFine = 0;
	for (i = 0; i < lNumeroParagrafi; i++) 
	{
		lFine += (**lHndlParagrafi)[i].len;
		xesetsel(lInizio, lFine, FALSE);
		lInterlinea = getparafmt(lHndlXE, P_LEADING, NULL);
				
		// se l'interlinea e' diversa da 0 modifico lo spazio tra i paragrafi
		if (lInterlinea != 0) 
		{
 			lSpazioPrima = getparafmt(lHndlXE, P_SPCBEFORE, NULL);
			lSpazioDopo = getparafmt(lHndlXE, P_SPCAFTER, NULL);
			
			if (lSpazioPrima != 0 && lSpazioDopo != 0) 
			{
				// lo spazio da recuperare lo distribuisco sia sopra che sotto
				lSpazioPrima -= lSpazioPerParagrafo / 2L;
				lSpazioDopo -= lSpazioPerParagrafo / 2L;
				lScarto = lSpazioPerParagrafo % 2L;
				lSpazioDopo -= lScarto;
				
				if (lScartoTotale != 0) 
				{
					lSpazioDopo -= lScartoTotale;
					lScartoTotale = 0;
				}
				
				// controllo che non siano diventati negativi
				if (lSpazioPrima < 0) 
				{
					lScartoTotale -= lSpazioPrima;
					lSpazioPrima = 0;
				}
				if (lSpazioDopo < 0) 
				{
					lScartoTotale -= lSpazioDopo;
					lSpazioDopo = 0;
				}
				
				// imposto il nuovo spazio tra i paragrafi
				setparafmt(lHndlXE, P_SPCBEFORE, lSpazioPrima, NULL);
				setparafmt(lHndlXE, P_SPCAFTER, lSpazioDopo, NULL);
				
				// incremento il contatore dei paragrafi diversi da zero
				if (lSpazioPrima != 0 || lSpazioDopo != 0) 
					lNumeroParagrafiDiversi++;
				
			} 
			else 
			{
				// lo spazio da recuperare solo sull'unico spazio divesro da zero
				if (lSpazioPrima != 0) 
				{
					lSpazioPrima -= lSpazioPerParagrafo;
					
					if (lScartoTotale != 0) 
					{
						lSpazioPrima -= lScartoTotale;
						lScartoTotale = 0;
					}
					
					// controllo che non siano diventati negativi
					if (lSpazioPrima < 0) 
					{
						lScartoTotale -= lSpazioPrima;
						lSpazioPrima = 0;
					}

					setparafmt(lHndlXE, P_SPCBEFORE, lSpazioPrima, NULL);
					
					// incremento il contatore dei paragrafi diversi da zero
					if (lSpazioPrima != 0) 
						lNumeroParagrafiDiversi++;

				} 
				else if (lSpazioDopo != 0) 
				{
					lSpazioDopo -= lSpazioPerParagrafo;
					
					if (lScartoTotale != 0) 
					{
						lSpazioDopo -= lScartoTotale;
						lScartoTotale = 0;
					}

					// controllo che non siano diventati negativi
					if (lSpazioDopo < 0) 
					{
						lScartoTotale -= lSpazioDopo;
						lSpazioDopo = 0;
					}

					setparafmt(lHndlXE, P_SPCAFTER, lSpazioDopo, NULL);
					
					// incremento il contatore dei paragrafi diversi da zero
					if (lSpazioDopo != 0) 
						lNumeroParagrafiDiversi++;

				}
			}
		}
  		lInizio = lFine;
	}
	
	while (1) 
	{
		if (lScartoTotale == 0) 
			break;
		if (lNumeroParagrafiDiversi == 0) 
			break;
		
		// e' necessario ridistruibuire ancora lo spazio tra i paragrafi
		assert(0 != lNumeroParagrafiDiversi);
		lSpazioPerParagrafo = lScartoTotale / lNumeroParagrafiDiversi;
		lScartoTotale =  lScartoTotale % lNumeroParagrafiDiversi;;

		// inizializzo il contatore dei paragrafi diversi da zero
		lNumeroParagrafiDiversi = 0;

		lInizio = lFine = 0;
		for (i = 0; i < lNumeroParagrafi; i++) 
		{
			lFine += (**lHndlParagrafi)[i].len;
			xesetsel(lInizio, lFine, FALSE);
			lInterlinea = getparafmt(lHndlXE, P_LEADING, NULL);
					
			// se l'interlinea e' diversa da 0 modifico lo spazio tra i paragrafi
			if (lInterlinea != 0) 
			{
	 			lSpazioPrima = getparafmt(lHndlXE, P_SPCBEFORE, NULL);
				lSpazioDopo = getparafmt(lHndlXE, P_SPCAFTER, NULL);
				
				if (lSpazioPrima != 0 && lSpazioDopo != 0) 
				{	
					// lo spazio da recuperare lo distribuisco sia sopra che sotto
					lSpazioPrima -= lSpazioPerParagrafo / 2L;
					lSpazioDopo -= lSpazioPerParagrafo / 2L;
					lScarto = lSpazioPerParagrafo % 2L;
					lSpazioDopo -= lScarto;
					
					if (lScartoTotale != 0) 
					{
						lSpazioDopo -= lScartoTotale;
						lScartoTotale = 0;
					}
					
					// controllo che non siano diventati negativi
					if (lSpazioPrima < 0) 
					{
						lScartoTotale -= lSpazioPrima;
						lSpazioPrima = 0;
					}
					if (lSpazioDopo < 0) 
					{
						lScartoTotale -= lSpazioDopo;
						lSpazioDopo = 0;
					}
					
					// imposto il nuovo spazio tra i paragrafi
					setparafmt(lHndlXE, P_SPCBEFORE, lSpazioPrima, NULL);
					setparafmt(lHndlXE, P_SPCAFTER, lSpazioDopo, NULL);
					
					// incremento il contatore dei paragrafi diversi da zero
					if (lSpazioPrima != 0 || lSpazioDopo != 0) lNumeroParagrafiDiversi++;

				} 
				else 
				{
					// lo spazio da recuperare solo sull'unico spazio divesro da zero
					if (lSpazioPrima != 0) 
					{
						lSpazioPrima -= lSpazioPerParagrafo;
						
						if (lScartoTotale != 0) 
						{
							lSpazioPrima -= lScartoTotale;
							lScartoTotale = 0;
						}
						
						// controllo che non siano diventati negativi
						if (lSpazioPrima < 0) 
						{
							lScartoTotale -= lSpazioPrima;
							lSpazioPrima = 0;
						}

						setparafmt(lHndlXE, P_SPCBEFORE, lSpazioPrima, NULL);
						
						// incremento il contatore dei paragrafi diversi da zero
						if (lSpazioPrima != 0) lNumeroParagrafiDiversi++;

					} 
					else if (lSpazioDopo != 0) 
					{
						lSpazioDopo -= lSpazioPerParagrafo;
						
						if (lScartoTotale != 0) 
						{
							lSpazioDopo -= lScartoTotale;
							lScartoTotale = 0;
						}
	
						// controllo che non siano diventati negativi
						if (lSpazioDopo < 0) 
						{
							lScartoTotale -= lSpazioDopo;
							lSpazioDopo = 0;
						}
	
						setparafmt(lHndlXE, P_SPCAFTER, lSpazioDopo, NULL);

						// incremento il contatore dei paragrafi diversi da zero
						if (lSpazioDopo != 0) lNumeroParagrafiDiversi++;

					}
				}
			}
	  		lInizio = lFine;
		}
	}
	
	DisposeHandle((Handle)lHndlParagrafi);
	
	// ripristino la selezione iniziale
	xesetsel(0, 0, TRUE);
	xecalc(XEDOALL);
	xesetsel(lInizioSelezione, lFineSelezione, FALSE);
} // DiminuisciSpazioTraParagrafi

/* ------------------------------------------------------------------------ *

	TogliTuttoLoSpazioTraParagrafi

* ------------------------------------------------------------------------ */
static void XTAPI TogliTuttoLoSpazioTraParagrafi() throw()
{	
	// handle al flusso di testo corrente
	xehandle lHndlXE = NULL;
	// inizio del testo attualmente selezionato
	int32 lInizioSelezione = 0;
	// fine del testo attualmente selezionato
	int32 lFineSelezione = 0;
	// inizio paragrafo
	int32 lInizio = 0;
	// fine paragrafo
	int32 lFine = 0;
	// lunghezza del flusso di testo corrente
	int32 lLunghezzaTesto = 0;
	// handle alle info dei paragrafi
	prechandle	lHndlParagrafi = NULL;
	// contiene il numero di info sui paragrafi
	uint32 lNumeroParagrafi = 0;
	// indice per il for sui paragrafi 
	int16 i = 0;
	Fixed lInterlinea = 0;

	// prendo la selezione corrente
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	
	// prendo le info dei paragrafi del box corrente
	lHndlParagrafi = (prechandle) NewHandle(0);
	boxid curbox;
	xtget_curbox(&curbox);
	xtgetxepstuff(curbox, &lNumeroParagrafi, &lHndlParagrafi);
	
	// modifico lo spazio tra i paragrafi
	lInizio = lFine = 0;
	for (i = 0; i < lNumeroParagrafi; i++) 
	{
		lFine += (**lHndlParagrafi)[i].len;
		xesetsel(lInizio, lFine, FALSE);
		lInterlinea = getparafmt(lHndlXE, P_LEADING, NULL);
				
		// se l'interlinea e' diversa da 0 modifico lo spazio tra i paragrafi
		if (lInterlinea != 0) 
		{	
			// imposto il nuovo spazio tra i paragrafi
			setparafmt(lHndlXE, P_SPCBEFORE, 0, NULL);
			setparafmt(lHndlXE, P_SPCAFTER, 0, NULL);
		}
  		lInizio = lFine;
	}
	
	DisposeHandle((Handle)lHndlParagrafi);
	
	// ripristino la selezione iniziale
	xesetsel(0, 0, TRUE);
	xecalc(XEDOALL);
	xesetsel(lInizioSelezione, lFineSelezione, FALSE);
} // TogliTuttoLoSpazioTraParagrafi

/* ------------------------------------------------------------------------ *

	LavoraSullaInterlinea

* ------------------------------------------------------------------------ */
static void XTAPI LavoraSullaInterlinea(Fixed spaziodarecuperare, int32 numerolinee) throw()
{
	// handle al flusso di testo corrente
	xehandle lHndlXE = NULL;
	// inizio del testo attualmente selezionato
	int32 lInizioSelezione = 0;
	// fine del testo attualmente selezionato
	int32 lFineSelezione = 0;
	// inizio paragrafo
	int32 lInizio = 0;
	// fine paragrafo
	int32 lFine = 0;
	// lunghezza del flusso di testo corrente
	int32 lLunghezzaTesto = 0;
	// handle alle info dei paragrafi
	prechandle	lHndlParagrafi = NULL;
	// contiene il numero di info sui paragrafi
	uint32 lNumeroParagrafi = 0;
	// indice per il for sui paragrafi
	int16 i = 0;
	// interlinea del paragrafo corrente
	Fixed lInterlinea = 0;
	// spazio da recuperare per linea
	Fixed lSpazioPerLinea = 0;
	// per prendere lo scarto finale
	Fixed lScartoTotale = 0;
	
	// controllo se c'Š dello spazio da recupera
	if (spaziodarecuperare == 0) 
		return;

	// calcolo lo spazio da recuperare per paragrafo
	assert(0 != numerolinee);

	lSpazioPerLinea = spaziodarecuperare / numerolinee;
	lScartoTotale = spaziodarecuperare % numerolinee;
	
	// prendo la selezione corrente
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	
	// prendo le info dei paragrafi del box corrente
	lHndlParagrafi = (prechandle) NewHandle(0);
	
	boxid curbox;
	xtget_curbox(&curbox);
	xtgetxepstuff(curbox, &lNumeroParagrafi, &lHndlParagrafi);
	
	// controllo l'ultimo paragrafo
	if ((**lHndlParagrafi)[lNumeroParagrafi - 1].len == 0) 
		lNumeroParagrafi--;
	
	// modifico l'interlinea
	lInizio = lFine = 0;
	for (i = 0; i < lNumeroParagrafi; i++) 
	{
		lFine += (**lHndlParagrafi)[i].len;
		xesetsel(lInizio, lFine, FALSE);
		lInterlinea = getparafmt(lHndlXE, P_LEADING, NULL);
				
		// se l'interlinea e' diversa da 0 la modifico
		if (lInterlinea != 0) 
		{
 			lInterlinea += lSpazioPerLinea;
 			if (lScartoTotale != 0) 
 			{
 				lInterlinea += lScartoTotale;
 				lScartoTotale = 0;
 			}
 			
			// imposto la nuova interlinea
			setparafmt(lHndlXE, P_LEADING, lInterlinea, NULL);
				
		}
  		lInizio = lFine;
	}
	
	DisposeHandle((Handle)lHndlParagrafi);
	
	// ripristino la selezione iniziale
	xesetsel(0, 0, TRUE);
	xecalc(XEDOALL);
	xesetsel(lInizioSelezione, lFineSelezione, FALSE);
} // LavoraSullaInterlinea

/* ------------------------------------------------------------------------ *

	VariazioneInterlinea

* ------------------------------------------------------------------------ */
static void XTAPI VariazioneInterlinea(Fixed spaziodarecuperare) throw()
{
	// handle al flusso di testo corrente
	xehandle lHndlXE = NULL;
	// inizio del testo attualmente selezionato
	int32 lInizioSelezione = 0;
	// fine del testo attualmente selezionato
	int32 lFineSelezione = 0;
	// inizio paragrafo
	int32 lInizio = 0;
	// fine paragrafo
	int32 lFine = 0;
	// lunghezza del flusso di testo corrente
	int32 lLunghezzaTesto = 0;
	// handle alle info dei paragrafi
	prechandle	lHndlParagrafi = NULL;
	// contiene il numero di info sui paragrafi
	uint32 lNumeroParagrafi = 0;
	// indice per il for sui paragrafi 
	int16 i = 0;
	// interlinea del paragrafo corrente
	Fixed lInterlinea = 0;
	// spazio da recuperare per linea
	Fixed lSpazioPerLinea = 0;
	// come variare l'interlinea
	Fixed lVariazioneInterlinea = 0;

	// controllo se c'e' dello spazio da recupera
	if (spaziodarecuperare == 0) 
		return;

	// calcolo lo spazio da recuperare per la prima linea
	lSpazioPerLinea = spaziodarecuperare;
	
	// prendo la selezione corrente
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	
	// prendo le info dei paragrafi del box corrente
	lHndlParagrafi = (prechandle) NewHandle(0);
	boxid curbox;
	xtget_curbox(&curbox);
	xtgetxepstuff(curbox, &lNumeroParagrafi, &lHndlParagrafi);
	
	if (lSpazioPerLinea == -1) 
		lSpazioPerLinea *= lNumeroParagrafi;
	
	while (1) 
	{
#ifdef OPERATION
		if (Button() == TRUE) break;
#endif
		if (lSpazioPerLinea > 0) 
		{
			lVariazioneInterlinea = 1;
		} 
		else if (lSpazioPerLinea < 0) 
		{
			lVariazioneInterlinea = -1;
		} 
		else 
			break;

		// modifico l'interlinea
		lInizio = lFine = 0;
		for (i = 0; i < lNumeroParagrafi; i++) 
		{
			lFine += (**lHndlParagrafi)[i].len;
			xesetsel(lInizio, lFine, FALSE);
			lInterlinea = getparafmt(lHndlXE, P_LEADING, NULL);
					
			// se l'interlinea e' diversa da 0 la modifico
			if (lInterlinea != 0) 
			{
		 		lInterlinea += lVariazioneInterlinea;
		 		lSpazioPerLinea -= lVariazioneInterlinea;
		 			
				// imposto la nuova interlinea
				setparafmt(lHndlXE, P_LEADING, lInterlinea, NULL);
				
				if (lSpazioPerLinea == 0) 
					break;
					
			}
		  	lInizio = lFine;
		}
	}
	
	DisposeHandle((Handle) lHndlParagrafi);
	
	// ripristino la selezione iniziale
	xesetsel(0, 0, TRUE);
	xecalc(XEDOALL);
	xesetsel(lInizioSelezione, lFineSelezione, FALSE);
} // VariazioneInterlinea

/* ------------------------------------------------------------------------ *

	ControllaTesto

* ------------------------------------------------------------------------ */
static void XTAPI ControllaTesto() throw()
{
	// handle al LOS del box di testo corrente
	loshandle lHndlLos = NULL;
	// lunghezza del LOS corrente
	int32 lLunghezzaLos = 0;
	// puntatore al box di testo corrente
	xtboxptr lPtrBoxTesto = NULL;
	// identificatore del box di testo corrente
	boxid lIdBoxCorrente = 0;
	// posizione del primo carattere del flusso di testo del box
	int32 lPosPrimoCarattere = 0;
	// coordinate del box alta a sinistra
	fixedpoint lCoordinateBox = {0, 0};
	// per sapere se c'e' dell'overflow del testo del box
	Fixed lSituazioneTesto = 0;
	// altezza dell'ultima riga per aggioornare il bottom del box
	Fixed lAltezzaUltimaLinea = 0;
	// altezza del box di testo
	Fixed lAltezzaBox = 0;
	// differenza tra l'altezza giusta e' quella ottenuta cambiando l'interlinea
	Fixed lDifferenza = 0;
	// per sapere quando interrompere il cicl
	Boolean lBool = FALSE;
	
	while (1) 
	{
		if (lBool == TRUE)
			break;

		// prende il LOS del box di testo corrente
		boxid curbox;
		xtget_curbox(&curbox);
		lIdBoxCorrente = curbox;
		lPosPrimoCarattere = 0;
		lHndlLos = (loshandle) NewHandle(0);
		xtgetlos(&lIdBoxCorrente, &lPosPrimoCarattere, FALSE, &lCoordinateBox, &lHndlLos);
		lLunghezzaLos = GetHandleSize((Handle) lHndlLos) / sizeof(losrec);
	
		lSituazioneTesto = (**lHndlLos)[lLunghezzaLos - 1L].leading;
	   
		if (lSituazioneTesto == ENDOTXT) 
		{
			// non c'e' overflow
			// controllo che l'altezza del box

			lAltezzaUltimaLinea = (**lHndlLos)[lLunghezzaLos - 2L].linev +
								  (**lHndlLos)[lLunghezzaLos - 2L].descent;
			
			lPtrBoxTesto = getxtbox(NULL,NULL,FALSE);
			lAltezzaBox = lAltezzaUltimaLinea + 
						  lPtrBoxTesto->t.textinset[BOTTOM_INSET] +
						  lPtrBoxTesto->t.textinset[TOP_INSET] +
						  + (2L * lPtrBoxTesto->t.box.frame.width);
						  
			lDifferenza = lPtrBoxTesto->t.box.s.r.fr.bottom -
						  (lPtrBoxTesto->t.box.s.r.fr.top + lAltezzaBox);
						  
			if (lDifferenza != 0) 
			{
				lBool = TRUE;
			} 
			else 
			{
				// posso uscire perch‚ le operazioni sono andate a buon fine
				lBool = TRUE;
			}
			
			disposextbox(lPtrBoxTesto,TRUE);

		} 
		else 
		{
			// c'e' overflow
			lBool = TRUE;
		}
		
		DisposeHandle((Handle) lHndlLos);
	}
} // ControllaTesto

/* ------------------------------------------------------------------------ *

	GiustificaTesto

* ------------------------------------------------------------------------ */
void XTAPI GiustificaTesto(necrologio *ptrnecrologio, Fixed spaziodarecuperare) throw()
{
	// per leggere l'altezza del box di testo corrente 
	Fixed lAltezzaBox = 0;
	// nuova altezza del box di testo corrente 
	Fixed lNuovaAltezzaBox = 0;
	// differenza tra la nuova e la vecchia altezza del box di testo corrente, sempre positiva 
	Fixed lDifferenzaAltezzaBox = 0;
	// numero delle linee interne del box 
	int32 lNumeroLinee = 0;
	// spazio tra i paragrafi del box corrente 
	Fixed lSpazioTraParagrafi = 0;
	// numero dei paragrafi del box corrente con spazio diverso da zero 
	int32 lNumeroParagrafiDiversi = 0;
	// numero dei paragrafi del box corrente 
	int32 lNumeroParagrafi = 0;
	// spazio da recuperare tra i paragrafi 
	Fixed lRecuperoTraParagrafi = 0;
	// spazio da recuperare sull'interlinea 
	Fixed lRecuperaInterlinea = 0;
	// per sapere cosa devo fare 
	cosafare lCosaFare = kNulla;
	// per sapere se devo togliere tutto lo spazio tra i paragrafi 
	Boolean lTogliTuttoLoSpazioTraParagrafi = FALSE;
	// per calcolare la differenza tra la giustificazione di preferenze e quella reale 
	Fixed lPunti = 0;
	// per calcolare la differenza tra la giustificazione di preferenze e quella reale 
	int32 lPuntiPerLinea = 0;
	// per calcolare la differenza tra la giustificazione di preferenze e quella reale 
	int32 lFrazionePuntiPerLinea = 0;
	// per stampare un messaggio di errore 
	uchar lStringa[156] = "";
	
	// giustifico solo se almeno uno dei check delle preferenze e' stato abilitato
	if (gXtPreferences.giustificazionePref.abilitaInterlinea == FALSE && gXtPreferences.giustificazionePref.abilitaSpazioTraParagrafi == FALSE) 
		return;

	if (ptrnecrologio == NULL) 
	{
		// quando viene usata questa procedura da menu
		boxid curbox;
		xtget_curbox(&curbox);
		if (istextbox(curbox) == FALSE) 
		{
			SysBeep(1);
			return;
		}
		
		// prendo le impostazioni per variare l'altezza del box
		if (ImpostaVariazioneAltezza() == FALSE) 
			return;
	
		// se l'ultimo carattere e' un return lo tolgo
		TogliRitornoACapo();

		// prendo le linee del box
		lNumeroLinee = PrendiNumeroLinee();

		// prendo lo spazio tra i paragrafi correnti
		lSpazioTraParagrafi = PrendiSpazioTraParagrafi();

		// prendo il numero di paragrafi con spazio diverso da zero
		lNumeroParagrafiDiversi = PrendiNumeroParagrafiDiversi();
		
		// prendo il numero di paragrafi
		lNumeroParagrafi = PrendiNumeroParagrafi();
		
		// prendo l'altezza del box corrente
		lAltezzaBox = PrendiAltezzaBox();
		
		// prendo la nuova altezza del box
		lNuovaAltezzaBox = lAltezzaBox + kFixedVariazioneAltezza;
		
		// modifico subito l'altezza del box
		{
			// puntatore al box di testo
			xtboxptr lPtrBoxTesto = NULL;
			
			// modifico l'altezza del box corrente
			lPtrBoxTesto = getxtbox(NULL, NULL ,FALSE);
			lPtrBoxTesto->t.box.s.r.fr.bottom = lPtrBoxTesto->t.box.s.r.fr.top + lNuovaAltezzaBox;
			setxtbox(lPtrBoxTesto, NULL, FALSE, TRUE);
			disposextbox(lPtrBoxTesto,TRUE);
		}
	} 
	else 
	{
		// sono nell'impaginazione
		// prendo le linee del box
		lNumeroLinee = (*ptrnecrologio).numeroLinee;
		
		// prendo lo spazio tra i paragrafi correnti
		lSpazioTraParagrafi = (*ptrnecrologio).spazioTraParagrafi;
		
		// prendo il numero di paragrafi con spazio divesro da zero
		lNumeroParagrafiDiversi = (*ptrnecrologio).numeroParagrafiDiversi;
		
		// prendo il numero di paragrafi
		lNumeroParagrafi = (*ptrnecrologio).numeroParagrafi;
		
		// prendo l'altezza del box corrente
		lAltezzaBox = (*ptrnecrologio).altezzaBox;
		
		// prendo la nuova altezza del box
		lNuovaAltezzaBox = lAltezzaBox + spaziodarecuperare;
	}
	
	// controllo che ci siano delle line di testo
	// a cui modificare l'interlinea
	if (lNumeroLinee == 0) 
		return;
	
	// determino cosa devo fare
	if (lAltezzaBox > lNuovaAltezzaBox) 
		lCosaFare = kRestringere;
	else if (lNuovaAltezzaBox > lAltezzaBox) 
		lCosaFare = kAllargare;
	else 
		lCosaFare = kNulla;

	if (lCosaFare == kNulla) 
		return;
	
	if (lCosaFare == kAllargare) 
	{
		// devo allargare 
		lDifferenzaAltezzaBox = lNuovaAltezzaBox - lAltezzaBox;
	
		// recupero prima sullo spazio tra i paragrafi
		// calcolo lo spazio che posso recuperare in base ai parametri delle preferenze
		if (gXtPreferences.giustificazionePref.abilitaSpazioTraParagrafi == TRUE) 
		{
			lRecuperoTraParagrafi = lNumeroParagrafi * gXtPreferences.giustificazionePref.incrementoMaxSpazioParagrafi;
		}
		else 
		{
			lRecuperoTraParagrafi = 0;
		}
		if (gXtPreferences.giustificazionePref.abilitaInterlinea == TRUE) 
		{
			lRecuperaInterlinea = lNumeroLinee * gXtPreferences.giustificazionePref.incrementoMaxInterlinea;
		} 
		else 
		{
			lRecuperaInterlinea = 0;
		}

		// controllo che lo spazio che posso recuperare e' sufficiente
		if (lRecuperoTraParagrafi >= lDifferenzaAltezzaBox) 
		{
			// posso recuperare lo spazio solo usando
			// la distanza tra i paragrafi
			lRecuperoTraParagrafi = lDifferenzaAltezzaBox;
			lRecuperaInterlinea = 0;
		} 
		else 
		{
			// devo distribuire il recupero anche sull'interlinea
			// calcolo quanto spazio deve essere distribuito sull'interlinea
			lRecuperaInterlinea = lDifferenzaAltezzaBox - lRecuperoTraParagrafi;
		}
		
		if (gXtPreferences.giustificazionePref.abilitaInterlinea == FALSE) 
		{
			lRecuperoTraParagrafi += lRecuperaInterlinea;
			lRecuperaInterlinea = 0;
		} 
		else if (gXtPreferences.giustificazionePref.abilitaSpazioTraParagrafi == FALSE) 
		{
			lRecuperaInterlinea += lRecuperoTraParagrafi;
			lRecuperoTraParagrafi = 0;
		}

		// controllo che lo spazio recuperato rispetti i vincoli delle preferenze 
		if (lRecuperaInterlinea > (lNumeroLinee * gXtPreferences.giustificazionePref.incrementoMaxInterlinea)) 
		{
			// non c'e' il rispetto delle preferenze
			lPunti = lRecuperaInterlinea -
					 (lNumeroLinee * gXtPreferences.giustificazionePref.incrementoMaxInterlinea);

			assert(0 != lNumeroLinee);


			lPuntiPerLinea = mLong(lPunti / lNumeroLinee);
			lFrazionePuntiPerLinea = (lPunti / lNumeroLinee) %
					          		 65536;
			sprintf((char*)lStringa, "punti %ld,%ld",
					lPuntiPerLinea, lFrazionePuntiPerLinea);
			ConcatenaAllaStringaErrore(lStringa);
			DaiErrore(kAllargamentoForzato);
		}

		// lavoro sullo spazio tra i paragrafi
		AumentaSpazioTraParagrafi(lRecuperoTraParagrafi, lNumeroParagrafi);
		
		// lovoro sull'interlinea di ogni riga
		LavoraSullaInterlinea(lRecuperaInterlinea, lNumeroLinee);
		
	} 
	else 
	{
		// devo restringere
		lDifferenzaAltezzaBox = lAltezzaBox - lNuovaAltezzaBox;
	
		// recupero prima sullo spazio tra i paragrafi
		// calcolo lo spazio che posso recuperare in base ai parametri delle preferenze
		if (gXtPreferences.giustificazionePref.abilitaSpazioTraParagrafi == TRUE) 
		{
			lRecuperoTraParagrafi = lNumeroParagrafiDiversi * gXtPreferences.giustificazionePref.decrementoMaxSpazioParagrafi;
			if (lRecuperoTraParagrafi > lSpazioTraParagrafi) 
			{
				lRecuperoTraParagrafi = lSpazioTraParagrafi;
			}
		} 
		else 
			lRecuperoTraParagrafi = 0;
		
		if (gXtPreferences.giustificazionePref.abilitaInterlinea == TRUE)
			lRecuperaInterlinea = lNumeroLinee * gXtPreferences.giustificazionePref.decrementoMaxInterlinea;
		else 
			lRecuperaInterlinea = 0;

		if (gXtPreferences.giustificazionePref.abilitaSpazioTraParagrafi == FALSE || lRecuperoTraParagrafi == 0) 
		{
			// la differenza la posso solo recuperare modificando l'interlinea
			lTogliTuttoLoSpazioTraParagrafi = FALSE;
			lRecuperoTraParagrafi = 0; 
			lRecuperaInterlinea = lDifferenzaAltezzaBox;
		} 
		else if (gXtPreferences.giustificazionePref.abilitaInterlinea == FALSE) 
		{
			// la differenza la devo recuperare solo sullo spazio tra i paragrafi
			if (lRecuperoTraParagrafi > lDifferenzaAltezzaBox) 
			{
				// lo spazio che posso togliere tra i paragrafi basta per recuperare la
				// differenza
				lTogliTuttoLoSpazioTraParagrafi = FALSE;
				lRecuperoTraParagrafi = lDifferenzaAltezzaBox;
				lRecuperaInterlinea = 0;
			} 
			else if (lRecuperoTraParagrafi < lDifferenzaAltezzaBox) 
			{
				// lo spazio che posso togliere tra i paragrafi non basta per recuperare
				if (lRecuperoTraParagrafi < lSpazioTraParagrafi) 
				{
					// lo spazio tra i pragrafi che posso togliere e'
					// piu' piccolo dello spazio tra i paragrafi 

					lTogliTuttoLoSpazioTraParagrafi = FALSE;
					lRecuperaInterlinea = lDifferenzaAltezzaBox - lRecuperoTraParagrafi;
				} 
				else 
				{
					// lo spazio tra i paragrafi e' uguale allo spazio
					// che posso recuperare in base alle preferenze
					lTogliTuttoLoSpazioTraParagrafi = TRUE;
					lRecuperoTraParagrafi = 0;
					lRecuperaInterlinea = lDifferenzaAltezzaBox - lSpazioTraParagrafi;

				}
			} 
			else 
			{
				// lo spazio tra i paragrafi e' uguale allo spazio da recuperare
				if (lDifferenzaAltezzaBox < lSpazioTraParagrafi) 
				{
					// lo spazio tra i paragrafi e' sufficiente e non devo
					// toglierlo tutto

					lTogliTuttoLoSpazioTraParagrafi = FALSE;
					lRecuperoTraParagrafi = lDifferenzaAltezzaBox; 
					lRecuperaInterlinea = 0;

				} 
				else if (lDifferenzaAltezzaBox > lSpazioTraParagrafi) 
				{
					// lo spazio tra i paragrafi non basta per recuperare
					// lavoro per lo spazio che mlanca sull'interlinea

					lTogliTuttoLoSpazioTraParagrafi = TRUE;
					lRecuperoTraParagrafi = 0; 
					lRecuperaInterlinea = lDifferenzaAltezzaBox - lSpazioTraParagrafi;

				} 
				else 
				{
					// lo spazio da recuperare e' uguale allo spazio tra i paragrafi
					lTogliTuttoLoSpazioTraParagrafi = TRUE;
					lRecuperoTraParagrafi = 0; 
					lRecuperaInterlinea = 0;
				}
			}
		} 
		else 
		{
			// la differenza devo recuperarla sia sullo spazio tra i paragrafi
			// che sull'interlinea
			if (lRecuperoTraParagrafi > lDifferenzaAltezzaBox) 
			{
				lTogliTuttoLoSpazioTraParagrafi = FALSE;
				lRecuperoTraParagrafi = lDifferenzaAltezzaBox;
				lRecuperaInterlinea = 0;
			} 
			else if (lRecuperoTraParagrafi == lDifferenzaAltezzaBox) 
			{
				// posso togliere tutto lo spazio tra i paragrafi
				if (lRecuperoTraParagrafi == lSpazioTraParagrafi) 
				{
					// lo spazio recuperabile in base alle preferenze e' uguale allo 
					// spazio tra i paragrafi

					lTogliTuttoLoSpazioTraParagrafi = TRUE;
					lRecuperoTraParagrafi = 0;
					lRecuperaInterlinea = 0;

				} 
				else 
				{
					// lo spazio recuperabile in base alle preferenze e' piu' piccolo 
					// dello spazio tra i paragrafi
					lTogliTuttoLoSpazioTraParagrafi = FALSE;
					lRecuperoTraParagrafi = lDifferenzaAltezzaBox;
					lRecuperaInterlinea = 0;

				}
			} 
			else 
			{
				// lo spazio tra i paragrafi non basta devo togliere anche l'intelinea
				if (lRecuperoTraParagrafi == lSpazioTraParagrafi) 
				{
					// lo spazio recuperabile in base alle preferenze e' uguale
					// allo spazio tra i paragrafi
					lTogliTuttoLoSpazioTraParagrafi = TRUE;
					lRecuperoTraParagrafi = 0;
					lRecuperaInterlinea = lDifferenzaAltezzaBox - lSpazioTraParagrafi;
				} 
				else 
				{
					// lo spazio recuperabile in base alle preferenze e' piu'
					// piccolo dello spazio tra i paragrafi

					lTogliTuttoLoSpazioTraParagrafi = FALSE;
					lRecuperaInterlinea = lDifferenzaAltezzaBox - lRecuperoTraParagrafi;

				}
			}
		}

		// controllo che lo spazio recuperato rispetti i vincoli delle preferenze
		if (lRecuperaInterlinea > (lNumeroLinee * gXtPreferences.giustificazionePref.decrementoMaxInterlinea)) 
		{
			// non c'e' il rispetto delle preferenze
			lPunti = lRecuperaInterlinea  - 
					 (lNumeroLinee * gXtPreferences.giustificazionePref.decrementoMaxInterlinea);

			assert(0 != lNumeroLinee);

			CalcolaPunti(lPunti / lNumeroLinee, &lPuntiPerLinea, &lFrazionePuntiPerLinea);
			sprintf((char*)lStringa, "punti %ld,%ld", lPuntiPerLinea, lFrazionePuntiPerLinea);
			ConcatenaAllaStringaErrore(lStringa);
			DaiErrore(kRestringimentoForzato);
		}

		if (lTogliTuttoLoSpazioTraParagrafi == TRUE) 
		{
			// tolgo tutto lo spazio tra i parargrafi
			TogliTuttoLoSpazioTraParagrafi();
		} 
		else 
		{
			// lavoro sullo spazio tra i paragrafi
			DiminuisciSpazioTraParagrafi(lRecuperoTraParagrafi, lNumeroParagrafiDiversi);
		}
		
		// lavoro sull'interlinea di ogni riga 
		lRecuperaInterlinea *= -1L;
		LavoraSullaInterlinea(lRecuperaInterlinea, lNumeroLinee);		
	}
} // GiustificaTesto

#pragma mark -
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

/* Costanti */
/* Costanti */
/* Costanti */

#define rFinestraVariazioneAltezza 20040
/* dialog per prendere la variazione di altezza */
#define iFixedVariazioneAltezza 5

#define kMinimaVariazioneAltezza mFixed(-40L)
#define kMassimaVariazioneAltezza mFixed(40L)



#ifdef OPERATION
static Boolean GestioneFinestraVariazioneAltezza(DialogPtr ptrfinestra);
static Boolean GestioneFinestraVariazioneAltezza(DialogPtr ptrfinestra)
/* ------------------------------------------------------------------------ *

	GestioneFinestraVariazioneAltezza

	15 Gennaio 1996						Stefano

	Descrizione:
	Procedura di gestione degli item della finestra per variazione dell'altezza

	Parametri:
	nessuno

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */

{
	short lBottoneScelto = 0;
	/* item della finestra selezionato */
	Fixed lVariazioneAltezza = 0;
	/* per prendere la variazione dell'altezza */

	while (1) {
		ARROWCURSOR;
		ModalDialog(NULL, &lBottoneScelto);
		
		switch (lBottoneScelto) {
			case iOk:
			
				clearerror();

				/* prendo la variazione dell'altezza */
				gErrore = fgetfield(ptrfinestra, iFixedVariazioneAltezza,
									DEFUNITS, 1, &lVariazioneAltezza);
				if (gErrore == 0) {
					giveerror(BADNUMBER);
					SelIText(ptrfinestra, iFixedVariazioneAltezza, 0, 32767);
					outlines((DialogPtr) frontmost);
					break;
				} else {
					rangecheck(lVariazioneAltezza, kMinimaVariazioneAltezza,
							   kMassimaVariazioneAltezza,
								DEFUNITS, 1, VALUERANGE, 1);
					if (showerror()) {
						SelIText(ptrfinestra, iFixedVariazioneAltezza, 0, 32767);
						outlines((DialogPtr) frontmost);
						break;
					}
				}
				fsetfield(ptrfinestra, iFixedVariazioneAltezza, DEFUNITS, 1, lVariazioneAltezza);
				kFixedVariazioneAltezza = lVariazioneAltezza;
				return(TRUE);
			case iAnnulla:
				return(FALSE);
		}
	}
}
#endif

#endif // 0 NECRO v1

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
