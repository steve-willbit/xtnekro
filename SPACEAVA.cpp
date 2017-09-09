/* ------------------------------------------------------------------------ *

	SpazioDisponibile.cpp

	2 Maggio 2005					Fabrizio

	Descrizione:
	Contiene le procedure usate per calcolare lo spazio disponibile su un
	documento con box gia' posizionati
	
	$Log: not supported by cvs2svn $
	Revision 1.3  2005/05/04 12:34:52  taretto
	comandi posiziona e ripristino ok
	
	Revision 1.2  2005/05/02 10:03:14  taretto
	avanzamento lavori porting
	

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
#include "DocInfo.h"
#include "ReadFile.h"
#include "AppWare.h"
#include "GesSlug.h"
#include "InfoNec.h"
#include "SpaceCol.h"
#include "SpaceAva.h"
#include "XtNecro.h"

// CONSTS
// CONSTS
// CONSTS

#define kTolleranza mFixed(5L)

// TYPEDEFS
// TYPEDEFS
// TYPEDEFS

typedef struct 
{
	Fixed margineAlto;
	Fixed margineBasso;
} infocolonna;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			CreaHandle
	@abstract 		gestione spazio avanzato
	@discussion		Controlla se la coordinata passata e' piu' vicino al margine alto o al
					margine basso del documento corrente

					2 Maggio 2005 - Fabrizio.

	@param 			coordinata - coordinata da controllare
	@result  			Ritorna vicinoaquale
*/
static vicinoaquale ControllaDistanza(Fixed coordinata) throw();

/*!
	@function			SonoMaggioriUguali
	@abstract 		gestione spazio avanzato
	@discussion		Confronta due fixed e' restituisce TRUE se valorepiccolo e piu' piccolo
					di valoregrande, resituisce TRUE anche quando valorepiccolo e' piu' grande
					di valoregrande di un valore uguale alla costante kTolleranza

					2 Maggio 2005 - Fabrizio.

	@param 			valorepiccolo - contiene il valore da confrontare
	@param			valoregrande - contiene il valore da confrontare
	@result  			TRUE se i due valori sono quasi maggiori uguali
*/
static Boolean XTAPI SonoMaggioriUguali(Fixed valorepiccolo, Fixed valoregrande) throw();

/*!
	@function			AnalizzaCoordinate
	@abstract 		gestione spazio avanzato
	@discussion		Per capire quali colonne coinvolge il box con le coordinate passate in input

					2 Maggio 2005 - Fabrizio.

	@param 			coordinate - puntatore alle coordinate del box corrente
	@param			ptrcolonneoccupate - puntatore per restituire quali sono le colonne occupate
	@result  			TRUE se i due valori sono quasi maggiori uguali
*/
static void XTAPI AnalizzaCoordinate(fixedrect coordinate, situazionecolonna *ptrsituazionecolonne) throw();

/*!
	@function			ScorriBoxSullaPagina
	@abstract 		gestione spazio avanzato
	@discussion		Scorre i box presenti sulla pagina corrente

					2 Maggio 2005 - Fabrizio.

	@param 			pagina - indica la pagina su cui effettuare lo scorrimento
	@param			ptrinfocolonna - puntatore alle info delle colonne della pagina corrente
					da aggiornare
	@result  			ritorna l'eventuale errore dell'xtension
*/

static errorixtension XTAPI ScorriBoxSullaPagina(int16 pagina, infocolonna *ptrinfocolonna) throw();


// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	ControllaDistanza

* ------------------------------------------------------------------------ */
static vicinoaquale XTAPI ControllaDistanza(Fixed coordinata) throw()
{
	// distanza con il margine alto
	Fixed lDistanzaMargineAlto = 0;
	// distanza con il margine alto
	Fixed lDistanzaMargineBasso = 0;
	
	lDistanzaMargineAlto = coordinata - PrendiMargineAlto();
	lDistanzaMargineBasso = PrendiMargineBasso() - coordinata;
	
	if (lDistanzaMargineAlto < lDistanzaMargineBasso) 
		return(kAlto);
	if (lDistanzaMargineAlto > lDistanzaMargineBasso) 
		return(kBasso);
	return(kUguale);
} // ControllaDistanza

/* ------------------------------------------------------------------------ *

	SonoUguali

* ------------------------------------------------------------------------ */
static Boolean XTAPI SonoMaggioriUguali(Fixed valorepiccolo, Fixed valoregrande) throw()
{
	Fixed lDifferenza = 0;
	
	lDifferenza = valoregrande - valorepiccolo;
	if (lDifferenza >= 0) 
		return(TRUE);
	if (lDifferenza < 0) 
		lDifferenza *= -1L;
	if (lDifferenza <= kTolleranza) 
		return(TRUE);
	else 
		return(FALSE);
} // SonoMaggioriUguali

/* ------------------------------------------------------------------------ *

	AnalizzaCoordinate
	
* ------------------------------------------------------------------------ */
static void XTAPI AnalizzaCoordinate(fixedrect coordinate, situazionecolonna *ptrsituazionecolonne) throw()
{
	// per il for
	int16 i = 0;
	// margine sinistro del documento corrente */
	Fixed lMargineSinistro = 0;
	// margine destro del documento corrente */
	Fixed lMargineDestro = 0;
	// distanza tra le colonne del documento corrente */
	Fixed lDistanzaColonne = 0;
	// larghezza colonne del documento corrente */
	Fixed lLarghezzaColonna = 0;
	// margini sinistro della colonna */
	Fixed lMargineSinistroColonna = 0;
	// margini destro della colonna */
	Fixed lMargineDestroColonna = 0;
	// numero colonne documento corrente */
	int lNumeroColonne = 0;
	// il margine e' nella colonna */
	Boolean lIsInColonna = TRUE;
	// limite della colonna sinistra occupata */
	int16 lColonnaSinistra = 0;
	// limite della colonna destra occupata */
	int16 lColonnaDestra = 0;
	
	// prendo le info della pagina corrente
	lMargineSinistro = PrendiMargineSinistro();
	lDistanzaColonne = PrendiDistanzaColonne();
	lLarghezzaColonna = PrendiLarghezzaColonna();
	lNumeroColonne = PrendiNumeroColonne();
	lColonnaSinistra = 0;
	lColonnaDestra = PrendiNumeroColonne();

	for (i = 0; i < lNumeroColonne; i++) 
	{
		lMargineSinistroColonna = lMargineSinistro +
								  lLarghezzaColonna * i +
								  lDistanzaColonne * i;
		lMargineDestroColonna = lMargineSinistroColonna + lLarghezzaColonna;
							
		lIsInColonna = SonoMaggioriUguali(lMargineSinistroColonna, coordinate.left) &&
					   SonoMaggioriUguali(coordinate.left, lMargineDestroColonna);
					   
		if (lIsInColonna == TRUE) 
			lColonnaSinistra = i;
					   
		lIsInColonna = SonoMaggioriUguali(lMargineSinistroColonna, coordinate.right) &&
					   SonoMaggioriUguali(coordinate.right, lMargineDestroColonna);
					   
		if (lIsInColonna == TRUE) 
			lColonnaDestra = i;
	}
	
	for (i = 0; i < lNumeroColonne; i++) 
	{
		if (lColonnaSinistra <= i && i <= lColonnaDestra) 
		{
			ptrsituazionecolonne[i] = kOccupata;
		} 
		else 
		{
			ptrsituazionecolonne[i] = kLibera;
		}
	}
} // AnalizzaCoordinate

/* ------------------------------------------------------------------------ *

	ScorriBoxSullaPagina

* ------------------------------------------------------------------------ */
static errorixtension XTAPI ScorriBoxSullaPagina(int16 pagina, infocolonna *ptrinfocolonna) throw()
{
	// puntatore al box di testo
	xtboxptr lPtrBoxTesto = NULL;
	// per posizionare il box nella pagina corrente
	pagenumdata lDatiPagina;
	// identificatore del box da scorrere
	boxid lIdentificatoreBox = 0;
	// array per sapere se la colonna e' occupata o libera
	situazionecolonna *lSituazioneColonne = NULL;
	// indice per il for
	int16 i = 0;
	// per sapere a quale margine e' piu' vicino la coordinata bassa del box
	vicinoaquale lVicinoAQualeAlta = kUguale;
	// per sapere a quale margine e' piu' vicino la coordinata bassa del box
	vicinoaquale lVicinoAQualeBassa = kUguale;
	// per calcolare lo spazio sopra al box
	Fixed lSpazioSopra = 0;
	// per calcolare lo spazio sotto al box
	Fixed lSpazioSotto = 0;
	
	// creo l'array di booleni
	lSituazioneColonne = (situazionecolonna *) NewPtrClear(PrendiNumeroColonne() *
														   sizeof(situazionecolonna));
	if (lSituazioneColonne == NULL) 
	{
		// errore nell'allocazione della memoria
		return(kErroreMemoria);
	}
	
	lIdentificatoreBox = firstbox(pagina, TRUE);
	
	while (1) 
	{
		if (lIdentificatoreBox == NULL) 
			break;

		xtset_curbox(lIdentificatoreBox);
		
		// prendo il box di testo corrente
		lPtrBoxTesto = getxtbox(NULL, NULL, FALSE);
		sprdfrect2page(&lPtrBoxTesto->t.box.s.r.fr);
		
		boxid curbox;
		xtget_curbox(&curbox);
		xtgetboxpage(curbox, FALSE, &lDatiPagina);

		// verifico che il box non sia all'esterno della pagina
		// e che il box non sia un box creato dall'impaginazione
		if (lDatiPagina.pagenum == LEFTPASTE ||
			lDatiPagina.pagenum == RIGHTPASTE ||
			IsBoxCorrenteMio() == TRUE ||
			isapicture(curbox) == FALSE) 
		{
			// non e' un box di ingombro

		} 
		else 
		{
			// e' un box di ingombro

			// analizzo le coordinate per dire su quali colonne e' il box corrente
			AnalizzaCoordinate(lPtrBoxTesto->t.box.s.r.fr, lSituazioneColonne);
		
			for (i = 0; i < PrendiNumeroColonne(); i++) 
			{
				if (lSituazioneColonne[i] == kOccupata) 
				{

					// controllo a quale margine si avvicina il box
					lVicinoAQualeAlta = ControllaDistanza(lPtrBoxTesto->t.box.s.r.fr.top);
					lVicinoAQualeBassa = ControllaDistanza(lPtrBoxTesto->t.box.s.r.fr.bottom);
	
					if (lVicinoAQualeAlta == kBasso && lVicinoAQualeBassa == kBasso) 
					{

						// si avvicina alla coordinata bassa
						// controllo se devo modificare il margine basso
						// se il top e' piu' in alto del margine basso alzo il margine basso
						if (ptrinfocolonna[i].margineBasso > lPtrBoxTesto->t.box.s.r.fr.top) {
							// cambio  il margine basso di impaginazione
							ptrinfocolonna[i].margineBasso = lPtrBoxTesto->t.box.s.r.fr.top;
						}
					} 
					else if (lVicinoAQualeAlta == kAlto && lVicinoAQualeBassa == kAlto) 
					{
	
						// si avvicina alla coordinata alta
						// controllo se devo modificare il margine alto
						// se il bottom e' pi— in basso del margine alto abbasso il margine alto
						if (ptrinfocolonna[i].margineAlto < lPtrBoxTesto->t.box.s.r.fr.bottom) 
						{
							// cambio  il margine alto di impaginazione
							ptrinfocolonna[i].margineAlto = lPtrBoxTesto->t.box.s.r.fr.bottom;
						}
					} 
					else 
					{
	
						// e' posto a meta'
						// per decidere quale margine modificare devo vedere
						// se c'e' piu' spazio libero sopra al box oppure sotto
						lSpazioSopra = lPtrBoxTesto->t.box.s.r.fr.top - PrendiMargineAlto();
						lSpazioSotto = PrendiMargineBasso() - lPtrBoxTesto->t.box.s.r.fr.bottom;
						
						if (lSpazioSopra < lSpazioSotto) 
						{
							// c'e' piu' spazio sotto
							// controllo se devo modificare il margine alto
							if (ptrinfocolonna[i].margineAlto <
								lPtrBoxTesto->t.box.s.r.fr.bottom) 
							{
								// cambio  il margine basso di impaginazione
								ptrinfocolonna[i].margineAlto = lPtrBoxTesto->t.box.s.r.fr.bottom;
							}
						} else  if (lSpazioSopra > lSpazioSotto) 
						{
							// c'e' piu' spazio sopra
							// controllo se devo modificare il margine basso
	
							if (ptrinfocolonna[i].margineBasso >
								lPtrBoxTesto->t.box.s.r.fr.top) 
							{
								// cambio  il margine alto di impaginazione								
								ptrinfocolonna[i].margineBasso = lPtrBoxTesto->t.box.s.r.fr.top;
							}
						} 
						else 
						{
							// lo spazio e' uguale sia sopra che sotto
							// controllo se devo modificare il margine basso
							if (lSpazioSopra <= 0) 
							{
								// non c'e' spazio
								ptrinfocolonna[i].margineAlto = 0;
								ptrinfocolonna[i].margineBasso = 0;
							} 
							else 
							{
								// scelgo di impaginare sulla parte alta
								if (ptrinfocolonna[i].margineBasso >
									lPtrBoxTesto->t.box.s.r.fr.top) 
								{
									// cambio  il margine alto di impaginazione
									ptrinfocolonna[i].margineBasso =
										lPtrBoxTesto->t.box.s.r.fr.top;
								}
							}
						}
					}
				}
			}
		}
		
		disposextbox(lPtrBoxTesto, TRUE);
	
		lIdentificatoreBox = nextbox(lIdentificatoreBox, TRUE);
	}
	
	// libero il puntatore
	DisposePtr((Ptr) lSituazioneColonne);
	
	return(kNessunErrore);
} // ScorriBoxSullaPagina

/* ------------------------------------------------------------------------ *

	CalcolaSpazioDisponibile

* ------------------------------------------------------------------------ */
errorixtension XTAPI CalcolaSpazioDisponibile() throw()
{
	// struttura che contiene le informazioni sulla pagina corrente
	infocolonna *lPtrInfoColonna = NULL;
	// indice per il for sulle pagine
	int16 lPagina = 0;
	// indice per il for delle colonne
	int16 lColonna = 0;
	// per calcolare lo spazio disponibile sulla colonna
	Fixed lSpazioDisponibile = 0;
	// per impostare il descrittore dello spazio delle colonne
	spaziocolonna *lPtrSpazioColonna = NULL;
	
	// libero il vecchio puntatore allo spazio colonna
	if (gPtrSpazioColonna != NULL) 
	{
		LiberaNecrologiSpazioColonna();
		DisposePtr((Ptr) gPtrSpazioColonna);
		gPtrSpazioColonna = NULL;
	}
	
	// imposto il numero delle pagine 
	ImpostaNumeroPagine(numpages());
	
	// 	creo la memoria per lo spazio colonna
	gPtrSpazioColonna = (spaziocolonna *) NewPtrClear(PrendiNumeroPagine() *
													  PrendiNumeroColonne() *
													  sizeof(spaziocolonna));
	if (gPtrSpazioColonna == NULL) 
	{
		// errore nell'allocazione della memoria
		return(kErroreMemoria);
	}
	
	// libero il vecchio puntatore allo spazio colonna fuori
	if (gPtrSpazioColonnaFuori != NULL) 
	{
		LiberaNecrologiSpazioColonnaFuori();
		DisposePtr((Ptr) gPtrSpazioColonnaFuori);
		gPtrSpazioColonnaFuori = NULL;
	}
	
	// creo lo spazio colonna per i box fuori
	gPtrSpazioColonnaFuori = (spaziocolonna *) NewPtrClear(sizeof(spaziocolonna));
	if (gPtrSpazioColonnaFuori == NULL) 
	{
		// errore nell'allocazione della memoria
		return(kErroreMemoria);
	}
	
	// inizializzo lo spazio disponibile totale
	InizializzaSpazioDisponibileTotale();
	
	// creo l'array che contiene i margine delle colonne della pagina del documento
	lPtrInfoColonna = (infocolonna *) NewPtrClear(PrendiNumeroColonne() * sizeof(infocolonna));
	if (lPtrInfoColonna == NULL) 
	{
		// errore nell'allocazione della memoria
		return(kErroreMemoria);
	}
	
	
	for (lPagina = 1; lPagina <= PrendiNumeroPagine(); lPagina++) 
	{
		// inizializzo le info sulle colonne
		for (lColonna = 1; lColonna <= PrendiNumeroColonne(); lColonna++ ) 
		{
			lPtrInfoColonna[lColonna - 1].margineAlto = PrendiMargineAlto();
			lPtrInfoColonna[lColonna - 1].margineBasso = PrendiMargineBasso();
		}

		// vado sulla pagina e leggo le coordinate dei box di ingombro
		gotopage(lPagina);
		gErroreXtension = ScorriBoxSullaPagina(lPagina, lPtrInfoColonna);
		if (gErroreXtension != kNessunErrore) 
		{
			// errore nello scorrere i box in pagina del documento
			return(gErroreXtension);
		}
		
		for (lColonna = 1; lColonna <= PrendiNumeroColonne(); lColonna++) 
		{
			lSpazioDisponibile = lPtrInfoColonna[lColonna - 1].margineBasso - 
								 lPtrInfoColonna[lColonna - 1].margineAlto;
			if (lSpazioDisponibile < 0) 
				lSpazioDisponibile = 0;
			
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);
			
			(*lPtrSpazioColonna).pagina = lPagina;
			(*lPtrSpazioColonna).colonna = lColonna;
			(*lPtrSpazioColonna).margineAlto = lPtrInfoColonna[lColonna - 1].margineAlto;
			(*lPtrSpazioColonna).margineBasso = lPtrInfoColonna[lColonna - 1].margineBasso;
			(*lPtrSpazioColonna).spazioDisponibile = lSpazioDisponibile;
			(*lPtrSpazioColonna).spazioOttimale = 0;
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
			
			// incremento lo spazio disponibile totale
			IncrementaSpazioDisponibileTotale(lSpazioDisponibile);
		}
	}

	gotopage(kPrimaPagina);
	xtset_curbox(NULL);
	
	// libero il puntatore
	DisposePtr((Ptr) lPtrInfoColonna);
	
	return(kNessunErrore);
} // CalcolaSpazioDisponibile
