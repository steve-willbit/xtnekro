/* ------------------------------------------------------------------------ *

	EseguiPosizionamento.cpp

	3 Maggio 2005					Fabrizio

	Descrizione:
	Contiene le procedure per eseguire posizionamento dei necrologi

	$Log: not supported by cvs2svn $

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
#include "ReadFile.h"
#include "AppWare.h"
#include "DocInfo.h"
#include "GesSlug.h"
#include "GesCom.h"
#include "Pref.h"
#include "InfoNec.h"
#include "Justify.h"
#include "Box.h"
#include "Document.h"
#include "Paletta.h"
#include "ResStr.h"
#include "StMes.h"
#include "Menu.h"
#include "SpaceCol.h"
#include "Memoria.h"
#include "Tempo.h"
#include "Impag.h"
#include "PresFile.h"
#include "Ripristi.h"
#include "CalcPos.h"
#include "XTNecro.h"

#include "ExPos.h"

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			PosizionaNecrologio
	@abstract		esegue posizionamento annunci
	@discussion		Sposta un necrologio nelle coordinate specificate in entrata

					3 Maggio 2005 - Fabrizio.

	@param			codicebox - codice del box da impostare
	@param			boxprecedente - box precedente su cui fare il link
	@param			coordinataalta - coordina alta del box da usare
	@param			altezza - altezza da usare per il box corrente
	@result			eventuale errore dell'Xt.
*/
static errorixtension XTAPI PosizionaNecrologio(uint32 codicebox, boxid *boxprecedente, Fixed coordinatasinistra, 
									Fixed coordinataalta,Fixed altezzabox, int16 pagina) throw();

/*! 
	@function			EseguiPosizionamentoNecrologi
	@abstract		esegue posizionamento annunci
	@discussion		Sposta in pogina i necrologi in base alle info presenti nella struttura
					sugli spazi delle colonne

					3 Maggio 2005 - Fabrizio.

	@param			nessuno
	@result			eventuale errore dell'Xt.
*/
static errorixtension XTAPI EseguiPosizionamentoNecrologi() throw();

/*! 
	@function			EliminaCrocini
	@abstract		esegue posizionamento annunci
	@discussion		Cicla su tutti i box del documento ed elimina un eventuale crocino di sbordo
					allungando il box di un decimo di punto alla volta

					3 Maggio 2005 - Fabrizio.

	@param			nessuno
	@result			ritorna l'eventuale errore dell'xtension
*/
static void XTAPI EliminaCrocini() throw();


// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	PosizionaNecrologio

* ------------------------------------------------------------------------ */
static errorixtension XTAPI PosizionaNecrologio(uint32 codicebox, boxid *boxprecedente, Fixed coordinatasinistra, 
									Fixed coordinataalta,Fixed altezzabox, int16 pagina) throw()
{
	// puntatore al box di testo
	xtboxptr lPtrBoxTesto = NULL;
	// per posizionare il box nella pagina corrente
	pagenumdata lDatiPagina ; //= {0, 0, 0, 0, 0}; 
	
	// cerco il box con il codice specificato 
	gErroreXtension = TrovaSlugDiCodice(codicebox);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sulla ricerca del codice
		return(gErroreXtension);
	}
	
	// prendo il box di testo corrente
	lPtrBoxTesto = getxtbox(NULL, NULL, FALSE);
	sprdfrect2page(&lPtrBoxTesto->t.box.s.r.fr);
	
	lPtrBoxTesto->t.box.s.r.fr.top = coordinataalta;
	lPtrBoxTesto->t.box.s.r.fr.bottom = lPtrBoxTesto->t.box.s.r.fr.top + altezzabox;
	lPtrBoxTesto->t.box.s.r.fr.left = coordinatasinistra;
	lPtrBoxTesto->t.box.s.r.fr.right = lPtrBoxTesto->t.box.s.r.fr.left +
									   PrendiLarghezzaColonna();
	
	xtseq2page(pagina, FALSE, &lDatiPagina);
	pagefrect2sprd(&lDatiPagina, &lPtrBoxTesto->t.box.s.r.fr);

	setxtbox(lPtrBoxTesto, NULL, FALSE, TRUE);
	disposextbox(lPtrBoxTesto, TRUE);
	
	// relinka il box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	xtrelinkbox(curbox, (*boxprecedente));
	
	(*boxprecedente) = curbox;
	
	return(kNessunErrore);
} // PosizionaNecrologio

/* ------------------------------------------------------------------------ *

	EseguiPosizionamentoNecrologi

* ------------------------------------------------------------------------ */
static errorixtension XTAPI EseguiPosizionamentoNecrologi() throw()
{
	// per ricordare il box precendete
	boxid lBoxPrecedente = 0;
	// indice per scorrere le pagine
	int16 lPagina = 0;
	// indice per scorrere le colonne
	int16 lColonna = 0;
	// per prendere le info dello spazio della colonna corrente
	spaziocolonna *lPtrSpazioColonna = NULL;
	// contiene il numero di necrologi da inserire nella colonna
	int16 lNumeroNecrologi = 0;
	// indice per scorrere i necrologi da inserire nella colonna
	int16 lIndiceNecrologio = 0;
	// coordinata alta della colonna corrente
	Fixed lCoordinataAlta = 0;	
   	Fixed lNewCoordinataAlta = 0;
	// coordinata bassa della colonna corrente
	Fixed lCoordinataBassa = 0;
	// coordinata sinistra della colonna corrente 
	Fixed lCoordinataSinistra = 0;
	// spazio disponibile sulla colonna 
	Fixed lSpazioDisponibile = 0;
	// spazio occupato sulla colonna 
	Fixed lSpazioOccupato = 0;
	// differenza tra lo spazio occupato e lo spazio disponibile: e' sempre positivo 
	Fixed lDifferenza = 0;
	// mi dice se devo restringere o allargare 
	cosafare lCosaFare = kNulla;
	// per calcolare lo spazio tra i necrologi da usare 
	Fixed lSpazioTraNecrologi = 0;
	// per calcolare lo spazio tra i paragrafi 
	Fixed lSpazioTraParagrafi = 0;
	// per calcolare la variazione dezll'interlinea 
	Fixed lInterlinea = 0;
	// puntatore per scorrere i necrologi da inserire nella colonna 
	necrologio *lPtrNecrologio = NULL;
	// altezza del box da inserire 
	Fixed lAltezzaBox = 0;
	// valore da aggiungere o levare all'altezza del box 
	Fixed lDifferenzaAltezzaBox = 0.0;
	// nuova altezza del box da inserire 
	Fixed lNuovaAltezzaBox = 0;
	// massimo spazio recuperabile allargando al massimo lo spazio tra i necrologi 
	Fixed lSpazioRecuperabile  = 0;
	// puntatore all'array degli spazio tra i paragrafi per restringere 
	Fixed *lPtrSpaziTraParagrafi = NULL;
	// puntatore allo spazio tra i paragrafi per restringere 
	Fixed *lPtrSpazioTraParagrafi = NULL;
	// per sgranare i box fuori pagina 
	Fixed lIncrementoCoordinataAlta = 0;
	// chiusura della colonna corrente 
	tipochiusura lTipoChiusura = kNessunaChiusura;
	

	// ciclo di posizionamento 
	for (lPagina = 1; lPagina <= PrendiNumeroPagine(); lPagina++) 
	{
		// visualizzo la pagina dove devo posizionare
		gotopage(lPagina);
	
		// inizializzo il box precedente 
		lBoxPrecedente = getsprdbox(lPagina);
		
		for (lColonna = 1; lColonna <= PrendiNumeroColonne(); lColonna++) 
		{
			// prendo le info della colonna corrente 
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);
			
			// controllo se devo inserire dei necrologi
			lNumeroNecrologi = (*lPtrSpazioColonna).numeroNecrologi +
							   (*lPtrSpazioColonna).numeroNecrologiDefunti +
							   (*lPtrSpazioColonna).numeroNecrologiFondoDefunti;
							   
			if (lNumeroNecrologi == 0) 
			{
				continue;
			}

			// prendo la chiusura della colonna
			lTipoChiusura = (*lPtrSpazioColonna).tipoChiusura;
			
			// imposto la coordinata sinistra
			lCoordinataSinistra = PrendiMargineSinistro() +
					  			  (lColonna - 1L) * PrendiLarghezzaColonna() +
					 			  (lColonna - 1L) * PrendiDistanzaColonne();
			
			// imposto la coordinata alta e bassa della colonna
			lCoordinataAlta = (*lPtrSpazioColonna).margineAlto;
			lCoordinataBassa = (*lPtrSpazioColonna).margineBasso;
			
			// calcolo lo spazio disponibile sulla colonna
			lSpazioDisponibile = (*lPtrSpazioColonna).spazioDisponibile;
			
			// calcolo lo spazio occupato
			lSpazioOccupato = (*lPtrSpazioColonna).spazioOccupato +
							  (*lPtrSpazioColonna).spazioTraNecrologi;
							  			

			// calcolo se devo allargare o restringere
			lCosaFare = CosaFare(lSpazioOccupato, lSpazioDisponibile, &lDifferenza);


			switch (lCosaFare) 
			{
				case kNulla:

					// calcolo lo spazio tra i necrologi
					if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE) 
					{
						lSpazioTraNecrologi = kFixedMinimoSpazioTraNecrologi;
					} 
					else 
					{
						lSpazioTraNecrologi = gXtPreferences.giustificazionePref.valoreIniziale;
					}
					

					// imposto il puntatore ai necrologi della colonna corrente
					lPtrNecrologio = PrendiPrimoNecrologioSpazioColonna(lPtrSpazioColonna);
					if (lPtrNecrologio == NULL) 
					{
						gErroreXtension = kErroreNumeroNecrologiSullaColonna;
						goto ErroreEseguiPosizionamento;
					}
			
					// scorro i necrologi da inserire nella colonna corrente
					for (lIndiceNecrologio = 1; lIndiceNecrologio < lNumeroNecrologi;lIndiceNecrologio++) 
					{	 
#ifdef OPERATION
						SystemTask();

						// per interrompere il posizionamento dei necrologi
						if (Button() == TRUE) 
						{
							// conferma l'interruzione forzata del posizionamento dei necrologi
							if (ConfermaInterruzione() == TRUE) 
							{

								// basta finire questa procedura perch‚ il salvataggio del documento
								// corrente e' fatto comunque alla fine del posizionamento
								gErroreXtension = kNessunErrore;
								goto ErroreEseguiPosizionamento;
							}
						}
#endif
						// prendo l'altezza del box
						lAltezzaBox = (*lPtrNecrologio).altezzaBox;
						// calcolo l'altezza del box
						lNuovaAltezzaBox = lAltezzaBox;

						// mette il box della colonna
						gErroreXtension = PosizionaNecrologio((*lPtrNecrologio).codiceBox,
															  &lBoxPrecedente,
															  lCoordinataSinistra,
															  lCoordinataAlta,
															  lNuovaAltezzaBox,
															  lPagina);

						if (gErroreXtension != kNessunErrore) 
						{
							// errore nella riimpaginazione dei necrologi
							goto ErroreEseguiPosizionamento;
						}
				
						// aggiorno la coordinata alta
						lCoordinataAlta += (lNuovaAltezzaBox + lSpazioTraNecrologi);
					
						// passo al necrologio successivo
						lPtrNecrologio = PrendiNecrologioSuccessivoSpazioColonna(lPtrSpazioColonna,
																				 lPtrNecrologio);
						if (lPtrNecrologio == NULL) 
						{
							gErroreXtension = kErroreNumeroNecrologiSullaColonna;
							goto ErroreEseguiPosizionamento;
						}
					}

					break;
				case kAllargare:

					if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE) 
					{
						// posso modificare lo spazio tra i necrologi 
						// questo spazio cerco di recuperarlo tutto 
						// calcolo lo spazio che posso ancora recuperare
						lSpazioRecuperabile = kFixedDifferenzaSpazioTraNecrologi *
											  (lNumeroNecrologi - 1);

						// controllo che lo spazio appena calcolato non sia piu' grande
						// di quello che si deve recuperare
						if (lSpazioRecuperabile >= lDifferenza) 
						{
							// recupero tutto allargando tra i box
							lSpazioTraNecrologi = kFixedMinimoSpazioTraNecrologi;

							wsprintf((char*)gStringaC, "Errore di divisione per zero.\nSorgente: %s, Linea: %d.",__FILE__, __LINE__);
							assert(0 != (lNumeroNecrologi - 1));

							lSpazioTraNecrologi += lDifferenza / (lNumeroNecrologi - 1);
							lDifferenza = 0;
						} 
						else 
						{
							// posso dare il massimo spazio tra i necrologi
							lSpazioTraNecrologi = kFixedMassimoSpazioTraNecrologi;
						
							lDifferenza -= kFixedDifferenzaSpazioTraNecrologi *
								            (lNumeroNecrologi - 1);
						}

					} 
					else 
						lSpazioTraNecrologi = gXtPreferences.giustificazionePref.valoreIniziale;
					
					if (lDifferenza > 0) 
					{
						// devo modificare anche lo spazio tra i paragrafi
						lSpazioRecuperabile = (*lPtrSpazioColonna).numeroParagrafi * 
											  gXtPreferences.giustificazionePref.incrementoMaxSpazioParagrafi;
			
						if (lSpazioRecuperabile >= lDifferenza) 
						{
							// basta per recuperare la differenza
							// allargare lo spazio tra i paragrafi

							wsprintf((char*)gStringaC, "Errore di divisione per zero.\nSorgente: %s, Linea: %d.",__FILE__, __LINE__);
							assert(0 != (*lPtrSpazioColonna).numeroParagrafi);

							lSpazioTraParagrafi = lDifferenza /
												  (*lPtrSpazioColonna).numeroParagrafi;
							lDifferenza = 0;
						} 
						else 
						{
							lSpazioTraParagrafi = gXtPreferences.giustificazionePref.incrementoMaxSpazioParagrafi;
							lDifferenza -= lSpazioRecuperabile;
						}
					} 
					else 
						lSpazioTraParagrafi = 0;

					if (lDifferenza > 0) 
					{
						// devo modificare anche l'interlinea
						lSpazioRecuperabile = (*lPtrSpazioColonna).numeroLinee * 
											  gXtPreferences.giustificazionePref.incrementoMaxInterlinea;
			

						if (lSpazioRecuperabile >= lDifferenza) 
						{
							// basta per recuperare la differenza
							// allargare l'interlinea

							wsprintf((char*)gStringaC, "Errore di divisione per zero.\nSorgente: %s, Linea: %d.",__FILE__, __LINE__);
							assert((*lPtrSpazioColonna).numeroLinee);


							lInterlinea = lDifferenza /
										  (*lPtrSpazioColonna).numeroLinee;
							lDifferenza = 0;
						} 
						else 
						{
							lInterlinea = gXtPreferences.giustificazionePref.incrementoMaxInterlinea;
							lDifferenza -= lSpazioRecuperabile;
						}
					} 
					else 
						lInterlinea = 0;

					// imposto il puntatore ai necrologi della colonna corrente
					lPtrNecrologio = PrendiPrimoNecrologioSpazioColonna(lPtrSpazioColonna);
					if (lPtrNecrologio == NULL) 
					{
						gErroreXtension = kErroreNumeroNecrologiSullaColonna;
						goto ErroreEseguiPosizionamento;
					}
			
					// scorro i necrologi da inserire nella colonna corrente
					for (lIndiceNecrologio = 1; lIndiceNecrologio < lNumeroNecrologi; lIndiceNecrologio++) 
					{
#ifdef OPERATION
						SystemTask();

						// per interrompere il posizionamento dei necrologi
						if (Button() == TRUE) 
						{
							// conferma l'interruzione forzata del posizionamento dei necrologi
							if (ConfermaInterruzione() == TRUE) 
							{
								// basta finire questa procedura perch‚ il salvataggio del documento
								// corrente E' fatto comunque alla fine del posizionamento 
								gErroreXtension = kNessunErrore;
								goto ErroreEseguiPosizionamento;
							}
						}
#endif
						// prendo l'altezza del box
						lAltezzaBox = (*lPtrNecrologio).altezzaBox;
						// calcolo la variazione dell'altezza
						lDifferenzaAltezzaBox = (*lPtrNecrologio).numeroParagrafi *
													lSpazioTraParagrafi +
									       		(*lPtrNecrologio).numeroLinee *
									       		lInterlinea;
						lNuovaAltezzaBox = lAltezzaBox + lDifferenzaAltezzaBox;

						// mette il box della colonna
						gErroreXtension = PosizionaNecrologio((*lPtrNecrologio).codiceBox,
															  &lBoxPrecedente,
															  lCoordinataSinistra,
															  lCoordinataAlta,
															  lNuovaAltezzaBox,
															  lPagina);
						if (gErroreXtension != kNessunErrore) 
						{
							// errore nella riimpaginazione dei necrologi
							goto ErroreEseguiPosizionamento;
						}
						
						if (lDifferenzaAltezzaBox != 0) 
						{
							// giustificazione del testo all'interno della nuova dimensione 
							// del box
							GiustificaTesto(lPtrNecrologio, lDifferenzaAltezzaBox);
						}
						
						// aggiorno la coordinata alta
						lCoordinataAlta += (lNuovaAltezzaBox + lSpazioTraNecrologi);
					
						// passo al necrologio successivo
						lPtrNecrologio = PrendiNecrologioSuccessivoSpazioColonna(lPtrSpazioColonna,
																				 lPtrNecrologio);
						if (lPtrNecrologio == NULL) 
						{
							gErroreXtension = kErroreNumeroNecrologiSullaColonna;
							goto ErroreEseguiPosizionamento;
						}
					}

					break;
				case kRestringere:

					if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE) 
					{
						// posso modificare lo spazio tra i necrologi
						// questo spazio cerco di recuperarlo tutto
						lSpazioTraNecrologi = kFixedMinimoSpazioTraNecrologi;
					} 
					else 
						lSpazioTraNecrologi = gXtPreferences.giustificazionePref.valoreIniziale;
					

					// costruisco il puntatore allo spazio tra i paragrafi 
					if (lPtrSpaziTraParagrafi != NULL) 
					{
						DisposePtr((Ptr) lPtrSpaziTraParagrafi);
						lPtrSpaziTraParagrafi = NULL;
					}

					lPtrSpaziTraParagrafi = (Fixed *) NewPtrClear(sizeof(Fixed) *
														          lNumeroNecrologi);
					if (lPtrSpaziTraParagrafi == NULL) 
					{
						// errore di memoria
						gErroreXtension = kErroreMemoria;
						goto ErroreEseguiPosizionamento;
					}
															 
					if (lDifferenza > 0) 
					{
						// devo modificare lo spazio tra i paragrafi
						
						// imposto il puntatore ai necrologi della colonna corrente 
						lPtrNecrologio = PrendiPrimoNecrologioSpazioColonna(lPtrSpazioColonna);
						if (lPtrNecrologio == NULL) 
						{
							gErroreXtension = kErroreNumeroNecrologiSullaColonna;
							goto ErroreEseguiPosizionamento;
						}

						// imposto il totale dello spazio tra paragrafi
						lSpazioTraParagrafi = 0;
	
						// scorro i necrologi per calcolare lo spazio tra i paragrafi */
						for (lIndiceNecrologio = 0; lIndiceNecrologio < lNumeroNecrologi;lIndiceNecrologio++) 
						{
							// imposto il puntatore allo spazio tra i paragrafi
							lPtrSpazioTraParagrafi = lPtrSpaziTraParagrafi +
							 						 lIndiceNecrologio;
							 
							// calcolo lo spazio tra i paragrafi
							(*lPtrSpazioTraParagrafi) =
								(*lPtrNecrologio).numeroParagrafiDiversi *
								gXtPreferences.giustificazionePref.decrementoMaxSpazioParagrafi;
							 							 
							// controllo che il decremento non sia maggiore
							// dello spazio realmente presente
							if ((*lPtrSpazioTraParagrafi) > (*lPtrNecrologio).spazioTraParagrafi) 
							{
								// non posso togliere tutto queso spazio
								(*lPtrSpazioTraParagrafi) = (*lPtrNecrologio).spazioTraParagrafi;
							 }
							 
							// controllo che la differenza che rimane non sia negativa
							if (lDifferenza < (*lPtrSpazioTraParagrafi)) 
							{
								(*lPtrSpazioTraParagrafi) = lDifferenza;
								lSpazioTraParagrafi += lDifferenza;
								lDifferenza = 0;
								break;
							}
							 
							// calcolo lo spazio tra i paragrafi totale
							lSpazioTraParagrafi += (*lPtrSpazioTraParagrafi);
							 
							// calcolo la nuova differenza
							lDifferenza -= (*lPtrSpazioTraParagrafi);

							// passo al necrologio successivo 
							lPtrNecrologio = PrendiNecrologioSuccessivoSpazioColonna(lPtrSpazioColonna,
																				 	 lPtrNecrologio);
							if (lPtrNecrologio == NULL &&
								lIndiceNecrologio != (lNumeroNecrologi - 1)) 
							{
								gErroreXtension = kErroreNumeroNecrologiSullaColonna;
								goto ErroreEseguiPosizionamento;
							}
						}
					} 
					else 
						lSpazioTraParagrafi = 0;

					if (lDifferenza > 0) 
					{
						// devo modificare anche l'interlinea
						lSpazioRecuperabile = (*lPtrSpazioColonna).numeroLinee * 
											  gXtPreferences.giustificazionePref.decrementoMaxInterlinea;
			

						if (lSpazioRecuperabile >= lDifferenza) 
						{
							// basta per recuperare la differenza
							// allargare l'interlinea 
							
							wsprintf((char*)gStringaC, "Errore di divisione per zero.\nSorgente: %s, Linea: %d.",__FILE__, __LINE__);
							assert(0 != (*lPtrSpazioColonna).numeroLinee);


							lInterlinea = lDifferenza /
										  (*lPtrSpazioColonna).numeroLinee;
							lDifferenza = 0;
						} 
						else 
						{
							lInterlinea = gXtPreferences.giustificazionePref.decrementoMaxInterlinea;
							lDifferenza -= lSpazioRecuperabile;
						}
					} 
					else 
						lInterlinea = 0;

					// imposto il puntatore ai necrologi della colonna corrente
					lPtrNecrologio = PrendiPrimoNecrologioSpazioColonna(lPtrSpazioColonna);
					if (lPtrNecrologio == NULL) 
					{
						gErroreXtension = kErroreNumeroNecrologiSullaColonna;
						goto ErroreEseguiPosizionamento;
					}
			
					// scorro i necrologi da inserire nella colonna corrente
					for (lIndiceNecrologio = 1; lIndiceNecrologio < lNumeroNecrologi; lIndiceNecrologio++) 
					{
#ifdef OPERATION
						SystemTask();

						// per interrompere il posizionamento dei necrologi
						if (Button() == TRUE) {
							// conferma l'interruzione forzata del posizionamento dei necrologi
							if (ConfermaInterruzione() == TRUE) {

								// basta finire questa procedura perch‚ il salvataggio del documento
								// corrente e' fatto comunque alla fine del posizionamento
								gErroreXtension = kNessunErrore;
								goto ErroreEseguiPosizionamento;
							}
						}
#endif
						// imposto il puntatore allo spazio tra i paragrafi
						lPtrSpazioTraParagrafi = lPtrSpaziTraParagrafi +
												 (lIndiceNecrologio - 1);

						// prendo l'altezza del box
						lAltezzaBox = (*lPtrNecrologio).altezzaBox;
						// calcolo la variazione dell'altezza
						lDifferenzaAltezzaBox = (*lPtrSpazioTraParagrafi) +
													(*lPtrNecrologio).numeroLinee *
													lInterlinea;
						lNuovaAltezzaBox = lAltezzaBox - lDifferenzaAltezzaBox;


						// mette il box della colonna
						gErroreXtension = PosizionaNecrologio((*lPtrNecrologio).codiceBox,
															  &lBoxPrecedente,
															  lCoordinataSinistra,
															  lCoordinataAlta,
															  lNuovaAltezzaBox,
															  lPagina);
						if (gErroreXtension != kNessunErrore) 
						{
							// errore nella riimpaginazione dei necrologi
							goto ErroreEseguiPosizionamento;
						}
						
						if (lDifferenzaAltezzaBox != 0) 
						{
							// giustificazione del testo all'interno della nuova dimensione
							// del box
							GiustificaTesto(lPtrNecrologio, -lDifferenzaAltezzaBox);
						}

						// aggiorno la coordinata alta
						lCoordinataAlta += (lNuovaAltezzaBox + lSpazioTraNecrologi);
					
						/* passo al necrologio successivo */
						lPtrNecrologio = PrendiNecrologioSuccessivoSpazioColonna(lPtrSpazioColonna,
																				 lPtrNecrologio);

						if (lPtrNecrologio == NULL) 
						{
							gErroreXtension = kErroreNumeroNecrologiSullaColonna;
							goto ErroreEseguiPosizionamento;
						}
					}
					
					break;
			}
			
			
			// prendo l'altezza dell'ultimo box
			lAltezzaBox = (*lPtrNecrologio).altezzaBox;

			// modifica chiesta da Bossetti per avere il fine colonna sempre uguale
			// calcolo la nuova altezza
			lNuovaAltezzaBox = lCoordinataBassa - lCoordinataAlta -
							   kFixedMinimoSpazioTraNecrologi;

			// per evitare di allargare un box oltre le preferenze 
			if (lTipoChiusura != kPreferenzeRispettate && lNuovaAltezzaBox > lAltezzaBox) 
			{
				lNuovaAltezzaBox = lAltezzaBox;
			}

			// calcolo la differenza di altezza per l'ultimo box
			lDifferenzaAltezzaBox = lNuovaAltezzaBox - lAltezzaBox;

			// mette l'ultimo box della colonna
			gErroreXtension = PosizionaNecrologio((*lPtrNecrologio).codiceBox,
												  &lBoxPrecedente,
												  lCoordinataSinistra,
												  lCoordinataAlta,
												  lNuovaAltezzaBox,
												  lPagina);
												  
			if (gErroreXtension != kNessunErrore) 
			{
				// errore nella riimpaginazione dei necrologi
				goto ErroreEseguiPosizionamento;
			}
				
			// controllo che sia necessario giustificare
			if (lDifferenzaAltezzaBox != 0) 
			{
				// giustificazione del testo all'interno della nuova
				// dimensione del box

				GiustificaTesto(lPtrNecrologio, lDifferenzaAltezzaBox);

			}
			

			// creo il filetto
			CreaFiletto(lCoordinataSinistra, lCoordinataBassa, lPagina);
			
		}
		
	}

	// posiziono i necrologi fuori pagina
	
	// prendo le info dei necrologi fuori pagina
	lPtrSpazioColonna = PrendiSpazioColonnaFuori();
			
	// controllo se devo inserire dei necrologi
	lNumeroNecrologi = (*lPtrSpazioColonna).numeroNecrologi +
					   (*lPtrSpazioColonna).numeroNecrologiDefunti +
					   (*lPtrSpazioColonna).numeroNecrologiFondoDefunti;
					   
	if (lNumeroNecrologi != 0) 
	{
		// imposto la pagina
		lPagina = PrendiNumeroPagine();
	
		// imposto la coordinata sinistra
		lCoordinataSinistra = PrendiMargineDestro() + 2 * PrendiLarghezzaColonna();
		
		// imposto la coordinata alta e bassa della colonna
		lCoordinataAlta = PrendiMargineAlto();
		lCoordinataBassa = PrendiMargineBasso();

		// calcolo lo spazio disponibile sulla colonna
		lSpazioDisponibile = lCoordinataBassa - lCoordinataAlta;


		/* calcolo l'incremento dela coordinata alta */
		lSpazioDisponibile /= 2;

		lIncrementoCoordinataAlta = lSpazioDisponibile / lNumeroNecrologi;

		// imposto il puntatore ai necrologi della colonna
		lPtrNecrologio = PrendiPrimoNecrologioSpazioColonna(lPtrSpazioColonna);
		if (lPtrNecrologio == NULL) 
		{
			gErroreXtension = kErroreNumeroNecrologiSullaColonna;
			goto ErroreEseguiPosizionamento;
		}

		// scorro i necrologi da inserire nella colonna
		for (lIndiceNecrologio = 0; lIndiceNecrologio < lNumeroNecrologi; lIndiceNecrologio++) 
		{
#ifdef OPERATION
			SystemTask();

			// per interrompere il posizionamento dei necrologi
			if (Button() == TRUE) {
				// conferma l'interruzione forzata del posizionamento dei necrologi
				if (ConfermaInterruzione() == TRUE) {

					// basta finire questa procedura perch‚ il salvataggio del documento
					// corrente e' fatto comunque alla fine del posizionamento
					gErroreXtension = kNessunErrore;
					goto ErroreEseguiPosizionamento;
				}
			}
#endif
			// prendo l'altezza del box
			lAltezzaBox = (*lPtrNecrologio).altezzaBox;
			lNuovaAltezzaBox = lAltezzaBox;

			// aggiorno la coordinata alta posizionando i necrologi uno dietro all'altro
			lNewCoordinataAlta += lAltezzaBox + gXtPreferences.giustificazionePref.valoreIniziale;
			
			// controllo se ho superato il margine inferiore
			if (lNewCoordinataAlta >= PrendiMargineBasso())
			{
				lCoordinataAlta = PrendiMargineAlto();
				lNewCoordinataAlta = lCoordinataAlta + lAltezzaBox;
				lCoordinataSinistra += PrendiLarghezzaColonna() + PrendiDistanzaColonne();				
			}

			// mette il box della colonna
			gErroreXtension = PosizionaNecrologio((*lPtrNecrologio).codiceBox,
												  &lBoxPrecedente,
												  lCoordinataSinistra,
												  lCoordinataAlta,
												  lNuovaAltezzaBox,
												  lPagina);
												  
			if (gErroreXtension != kNessunErrore) 
			{
				// errore nella riimpaginazione dei necrologi
				goto ErroreEseguiPosizionamento;
			}
	
			// aggiorno la coordinata alta posizionando i necrologi uno dietro all'altro
			lCoordinataAlta = lNewCoordinataAlta;

			// passo al necrologio successivo
			lPtrNecrologio = PrendiNecrologioSuccessivoSpazioColonna(lPtrSpazioColonna,
																	 lPtrNecrologio);

			if (lPtrNecrologio == NULL &&
				lIndiceNecrologio != (lNumeroNecrologi - 1)) 
			{
				gErroreXtension = kErroreNumeroNecrologiSullaColonna;
				goto ErroreEseguiPosizionamento;
			}
		}
		
	} 

	// per evitare di segnalare degli errori inesistenti
	gErroreXtension = kNessunErrore;

ErroreEseguiPosizionamento:
	if (lPtrSpaziTraParagrafi != NULL) 
	{
		DisposePtr((Ptr) lPtrSpaziTraParagrafi);
		lPtrSpaziTraParagrafi = NULL;
	}
	return(gErroreXtension);
} // EseguiPosizionamentoNecrologi

/* ------------------------------------------------------------------------ *

	EliminaCrocini

* ------------------------------------------------------------------------ */
static void XTAPI EliminaCrocini() throw()
{
	// indice per il for
	int16 lPagina = 0;
	// per prendere il numero delle pagini del dcoumento corrente
	int16 lNumeroPagine = 0;
	// identificatore del box su cui eventualmente importare l'immagine
	boxid lIdentificatoreBox = 0;
	// percetuale di scala del documento
	Fixed lPercetuale = 0;
	// lunghezza del nome immagine
	Size lLunghezza = 0;
	
	// prendo il numero delle pagine del documento corrente
	lNumeroPagine = numpages();

	// imposto la visualizzazione di impaginazione
	lPercetuale = kVistaControlloImmagini;
	ImpostaScalaDocumento(&lPercetuale);

	// ciclo sulle pagine del documento 
	for (lPagina = 1; lPagina <= lNumeroPagine; lPagina++) 
	{
		gotopage(lPagina);
		lIdentificatoreBox = firstbox(lPagina, FALSE);

		// scorro tutti i box della pagina
		while (1) 
		{
			if (lIdentificatoreBox == NULL) 
				break;
			xtset_curbox(lIdentificatoreBox);

			if (istextbox(lIdentificatoreBox) == TRUE)
			{
				// aumento l'altezza del box corrente
				AumentaAltezzaBox();
			}

			lIdentificatoreBox = nextbox(lIdentificatoreBox, FALSE);
		}

	}
	xtset_curbox(NULL);

	// torno alla prima pagina
	gotopage(kPrimaPagina);

	// imposto la scala precedente
	ImpostaScalaDocumento(&lPercetuale);

	return;
}


/* ------------------------------------------------------------------------ *

	Posiziona

* ------------------------------------------------------------------------ */
errorixtension XTAPI Posiziona() throw()
{
	// per costruire il nome da usare per salvare il documento appena impaginata
	uchar lNomeDocumento[32] = "";
	
	if (PrendiCodiceDocumento() != PrendiCodiceImpaginazione()) 
	{
		// e' un documento su cui ho gia' importato dati
		ConcatenaAllaStringaErrore((uchar*)curdocname());
		return(kErrorePosizionamentoSuDocumentoSbagliato);
	}

	// stampa inizio posizionamento
	DaiMessaggio(kInizioPosizionamento);

	// posiziono i necrologi 
	gErroreXtension = EseguiPosizionamentoNecrologi();
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sul calcolo dello spazio ottimale
		return(gErroreXtension);
	}

	// costruisco il nome del documento
	sprintf((char*)lNomeDocumento,"%s%s", PrendiDataXNomeFile(), "_9X.QXP");

	// salvataggio documento appena impaginato
	gErroreXtension = SalvaDocumento(gXtPreferences.finalePref.folderPathFinale ,lNomeDocumento);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sul savataggio del documento definitivo
		return(gErroreXtension);
	}

	// fine posizionamento
	DaiMessaggio(kFinePosizionamento);

	// disabilito il calcolo del posizionamento dei necrologi
	DisabilitaMenu(CALCPOSMENUID);

	// disabilito il comando di posizionamento dei necrologi
	DisabilitaMenu(POSIZIONAMENUID);

	// disabilito il ripristino dei dat
	DisabilitaMenu(RIPRISTINADATIULTIMAMENUID);

	// permetto di riniziare le operazioni di impaginazione
	AbilitaMenu(RIPRISTINADOCMENUID);

	return(kNessunErrore);
} // Posiziona

