/* ------------------------------------------------------------------------ *

	RiordinoNecrologi.c

	13 Marzo 1996						Stefano

	Descrizione:
	Contiene le procedure per il riordino dei necrologi

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
#include "Pref.h"
#include "Paletta.h"
#include "ResStr.h"
#include "StMes.h"
#include "InfoNec.h"
#include "SpaceCol.h"
#include "CalcPos.h"
#include "XTNecro.h"

#include "RiordNec.h"

// STATISCS GLOBALS
// STATISCS GLOBALS
// STATISCS GLOBALS

// e' una lista che contiene i defunti che sono stati messi nelle colonne ma
// che possono essere rimossi per essere usati come tappabuchi
static listanec *gPtrListaNecTappaBuchi = NULL;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			LiberaListaNec
	@abstract		gestione riordinamento dei necrologi
	@discussion		Libera la memoria alloccata per una lista necrologi

					2 Maggio 2005 - Fabrizio.

	@param			ptrlistanec - puntatore al puntatore alla lista dei necrologi da liberare
	@result			nessuno
*/
static void XTAPI LiberaListaNec(listanec **ptrlistanec) throw();

/*! 
	@function			CostruisciListaNec
	@abstract		gestione riordinamento dei necrologi
	@discussion		Allocca la memoria per l'elenco dei necrologi ancora da inserire
					
					2 Maggio 2005 - Fabrizio.

	@param			ptrlistadacostruire - puntatore al puntatore alla lista mosse dei necrologi
					ancora da inserire
	@param			ptrnecrologi - puntatore all'elenco dei necrologi ancora da inserire				
	@result			nessuno
*/
static void XTAPI CostruisciListaNec(listanec **ptrlistadacostruire, necrologio *ptrnecrologi) throw();

/*! 
	@function			CostruisciListaNec
	@abstract		gestione riordinamento dei necrologi
	@discussion		Prende il primo elemento della lista e aggiorna il puntatore alla lista
					all'elemento successivo
					
					2 Maggio 2005 - Fabrizio.

	@param			ptrlista - contiene il puntatore all'elenco dei necrologi ancora da trattare
					dalla procedura ricorsiva
	@result			ritorna il primo elemento della lista passata
*/
static listanec* XTAPI EstraiDaListaNec(listanec **ptrlista) throw();

/*! 
	@function			InserisciElementoInListaNec
	@abstract		gestione riordinamento dei necrologi
	@discussion		nserisce in coda l'elemento ptrelemento nella lista ptrlistanecnecrologi
					
					2 Maggio 2005 - Fabrizio.

	@param			ptrmossa - puntatore all'elemento da inserire
	@param			ptrlistanecnecrologi - lista in cui inserire l'elemento
	@result			nessuno
*/
static void InserisciElementoInListaNec(listanec **ptrelemento, listanec **ptrlistanecnecrologi) throw();

/*! 
	@function			ContaElementiListaNec
	@abstract		gestione riordinamento dei necrologi
	@discussion		Conta il numero di elementi della lista
					
					2 Maggio 2005 - Fabrizio.

	@param			ptrlistanecnecrologi - puntatore alla lista da contare
	@result			numero necrologi
*/
static int32 XTAPI ContaElementiListaNec(listanec *ptrlistanecnecrologi) throw();

/*! 
	@function			OrdinaAltezzeNecrologi
	@abstract		gestione riordinamento dei necrologi
	@discussion		Ordina in modo crescente sul valore assoluto della distanza tra lo spazio
					rimanente e l'altezza del necrologio
					
					2 Maggio 2005 - Fabrizio.

	@param			a - puntatore alle info del necrologio
	@param			b - puntatore alle info del necrologio
	@result			0 se le altezze sono uguali, -1 se a e' piu' piccolo, 1 se a e' piu' grande
*/
static int32 XTAPI OrdinaAltezzeNecrologi(const void *aa, const void *bb) throw();

/*! 
	@function			RiordinaListaNec
	@abstract		gestione riordinamento dei necrologi
	@discussion		Per riodinare l'elenco dei necrologi in base allo spazio che rimane da
					occupare
					
					2 Maggio 2005 - Fabrizio.

	@param			ptrspaziocolonna - puntatore alla colonna in cui inserire
	@param			scarto - scarto della colonna precedente
	@param			ptrlistanecnecrologi - puntatore alla lista dei necrologi da inserire
	@result			nessuno
*/
static void XTAPI RiordinaListaNec(spaziocolonna *ptrspaziocolonna, Fixed scarto, listanec **ptrlistanecnecrologi) throw();

/*! 
	@function			EseguiNormaleNecrologi
	@abstract		gestione riordinamento dei necrologi
	@discussion		Procedura iterativa per cercare di riempire la colonna
					
					2 Maggio 2005 - Fabrizio.

	@param			ptrspaziocolonna - puntatore alla colonna in cui inserire
	@param			ptrlistanecnecrologi - puntatore alla lista dei necrologi da inserire
	@param			scarto - scarto della colonna precedente
	@result			tipo di chiusura della colonna
*/

static tipochiusura XTAPI EseguiNormaleNecrologi(spaziocolonna *ptrspaziocolonna, listanec **ptrlistanecnecrologi, Fixed scarto) throw();

/*! 
	@function			NormaleNecrologi
	@abstract		gestione riordinamento dei necrologi
	@discussion		Per cercare di riodinare i necrologi in base allo spazio disponibile sulla
					colonna
					
					2 Maggio 2005 - Fabrizio.

	@param			ptrdefunto - puntatore al defunto con i necrologi da inserire
	@param			numerocolonne -  contiene il numero delle colonne in cui fare l'inserimento
	@param			ptrspaziocolonne - puntatore alle info sulle colonne in cui inserire
	@param			ptrscarto - per ritornare lo scarto corrente
	@result			nessuno
*/
static void XTAPI NormaleNecrologi(defunto *ptrdefunto, int16 numerocolonne, spaziocolonna *ptrspaziocolonne, Fixed *ptrscarto) throw();

/*! 
	@function			NormaleNecrologiA
	@abstract		gestione riordinamento dei necrologi
	@discussion		Inserisce nelle colonne passate i necrologi con codice A
					
					2 Maggio 2005 - Fabrizio.

	@param			ptrdefunto - puntatore al defunto con i necrologi da inserire
	@param			numerocolonne -  contiene il numero delle colonne in cui fare l'inserimento
	@param			ptrspaziocolonne - puntatore alle info sulle colonne in cui inserire
	@result			nessuno
*/
static void XTAPI NormaleNecrologiA(defunto *ptrdefunto, int numerocolonne, spaziocolonna *ptrspaziocolonne) throw();

/*! 
	@function			NormaleNecrologiB
	@abstract		gestione riordinamento dei necrologi
	@discussion		Inserisce nelle colonne passate i necrologi con codice B
					Marcello: aggingo il controllo sullo spazio dispnibile in colonna
					copiandolo dalla NormaleNecrologiA
					
					2 Maggio 2005 - Fabrizio.

	@param			ptrdefunto - puntatore al defunto con i necrologi da inserire
	@param			numerocolonne -  contiene il numero delle colonne in cui fare l'inserimento
	@param			ptrspaziocolonne - puntatore alle info sulle colonne in cui inserire
	@result			nessuno
*/
static void XTAPI NormaleNecrologiB(defunto *ptrdefunto, int numerocolonne, spaziocolonna *ptrspaziocolonne) throw();



// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	LiberaListaNec

* ------------------------------------------------------------------------ */
static void XTAPI LiberaListaNec(listanec **ptrlistanec) throw()
{
	// per scorrere la lista
	listanec *lPtrListaNec = NULL;
	// per liberare la lista
	listanec *lPtrListaNecDaLiberare = NULL;
	
	lPtrListaNec = (*ptrlistanec);

	while (1) 
	{
		if (lPtrListaNec == NULL) 
			break;
		
		lPtrListaNecDaLiberare = lPtrListaNec;
		
		lPtrListaNec = (*lPtrListaNec).successivo;
		DisposePtr((Ptr) lPtrListaNecDaLiberare);
		
	}
	
	(*ptrlistanec) = NULL;
} // LiberaListaNec
	
/* ------------------------------------------------------------------------ *

	CostruisciListaNec

* ------------------------------------------------------------------------ */
static void XTAPI CostruisciListaNec(listanec **ptrlistadacostruire, necrologio *ptrnecrologi) throw()
{
	// per costruire un elemento della lista delle mosse
	listanec *lPtrListaNec = NULL;
	// per scorrere la lista dei necrologi
	necrologio *lPtrNecrologio = NULL;
	// puntatore al successivo
	listanec **lPtrListaNecSuccessivo = NULL;
	// puntatore al precedente
	listanec *lPtrListaNecPrecedente = NULL;
	
	// imposto i puntatori
	lPtrNecrologio = ptrnecrologi;
	lPtrListaNecSuccessivo = ptrlistadacostruire;
	lPtrListaNecPrecedente = NULL;
	
	while (1) 
	{
		// controllo se devo fermarmi
		if (lPtrNecrologio == NULL) break;
		
		// creo un nuovo elemento
		lPtrListaNec = (listanec *) NewPtrClear(sizeof(listanec));
		if (lPtrListaNec == NULL) 
		{
			MostraErrore(kErroreMemoria);
			return;
		}
		
		// imposto il puntatore precedente
		(*lPtrListaNecSuccessivo) = lPtrListaNec;
		
		// inserisco i valori nel nuovo elemento
		(*lPtrListaNec).ptrNecrologio = lPtrNecrologio;
		(*lPtrListaNec).ptrSpazioColonna = NULL;
		(*lPtrListaNec).precedente = lPtrListaNecPrecedente;
		(*lPtrListaNec).successivo = NULL;
		
		// imposto i puntatori per il prossimo iserimento
		lPtrListaNecSuccessivo = &((*lPtrListaNec).successivo);
		lPtrListaNecPrecedente = lPtrListaNec;

		// per scorrere la lista
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}
} // CostruisciListaNec

/* ------------------------------------------------------------------------ *

	EstraiDaListaNec

* ------------------------------------------------------------------------ */
static listanec* XTAPI EstraiDaListaNec(listanec **ptrlista) throw()
{
	// elemento da restituire
	listanec *lPtrListaNec = NULL;
	
	lPtrListaNec = (*ptrlista);
	
	if (lPtrListaNec == NULL) 
		return(NULL);
	
	(*ptrlista) = (*lPtrListaNec).successivo;
	if ((*ptrlista) != NULL) 
		(**ptrlista).precedente = NULL;
	
	(*lPtrListaNec).successivo = NULL;
	(*lPtrListaNec).precedente = NULL;
	
	return(lPtrListaNec);
} // EstraiDaListaNec

/* ------------------------------------------------------------------------ *

	InserisciElementoInListaNec

* ------------------------------------------------------------------------ */
static void InserisciElementoInListaNec(listanec **ptrelemento, listanec **ptrlistanecnecrologi) throw()
{
	// per scorrere la lista
	listanec *lPtrListaNec = NULL;
	
	// imposto il puntatore
	lPtrListaNec = (*ptrlistanecnecrologi);
	
	// controllo che la lista non sia vuota
	if (lPtrListaNec == NULL) 
	{
		(*ptrlistanecnecrologi) = (*ptrelemento);
		(*ptrelemento) = NULL;
		return;
	}
	
	// scorro la lista
	while (1) 
	{
		// controllo se sono arrivato all'ultimo elemento della lista
		if ((*lPtrListaNec).successivo == NULL) 
			break;
		
		lPtrListaNec = (*lPtrListaNec).successivo;
	}
	
	// inserisco l'elemento
	(*lPtrListaNec).successivo = (*ptrelemento);
	(**ptrelemento).precedente = lPtrListaNec;
	(**ptrelemento).successivo = NULL;
	
	(*ptrelemento) = NULL;
} // InserisciElementoInListaNec

/* ------------------------------------------------------------------------ *

	ContaElementiListaNec

* ------------------------------------------------------------------------ */
static int32 XTAPI ContaElementiListaNec(listanec *ptrlistanecnecrologi) throw()
{
	// per scorrere la lista
	listanec *lPtrListaNec = NULL;
	// contatore degli elementi
	int32 lNumeroElementi = 0;
	
	// imposto il puntatore
	lPtrListaNec = ptrlistanecnecrologi;
	
	while (1) 
	{
		if (lPtrListaNec == NULL) 
			break;
		
		// avanzo il puntatore
		lPtrListaNec = (*lPtrListaNec).successivo;
		
		//incremento il contatore
		lNumeroElementi++;
	}
	
	return(lNumeroElementi);
} // ContaElementiListaNec

/* ------------------------------------------------------------------------ *

	OrdinaAltezzeNecrologi

* ------------------------------------------------------------------------ */
static int32 OrdinaAltezzeNecrologi(const void *aa, const void *bb) throw()
{
	DoubleFixed lAltezzaADoubleFixed = (DoubleFixed) gSpazioRimanente;
	DoubleFixed lAltezzaBDoubleFixed = (DoubleFixed) gSpazioRimanente;
	altezza *a, *b;

	a = (altezza *)aa;
	b = (altezza *)bb;

	lAltezzaADoubleFixed -= a->altezzaDoubleFixed;
	if (lAltezzaADoubleFixed < 0) 
		lAltezzaADoubleFixed *= -1;
	lAltezzaBDoubleFixed -= b->altezzaDoubleFixed;
	if (lAltezzaBDoubleFixed < 0) 
		lAltezzaBDoubleFixed *= -1;
	
	if (lAltezzaADoubleFixed < lAltezzaBDoubleFixed) 
		return(-1);
	else if (lAltezzaADoubleFixed > lAltezzaBDoubleFixed) 
		return(1);
	else 
		return(0);

} // OrdinaAltezzeNecrologi

/* ------------------------------------------------------------------------ *

	RiordinaListaNec

* ------------------------------------------------------------------------ */
static void XTAPI RiordinaListaNec(spaziocolonna *ptrspaziocolonna, Fixed scarto, listanec **ptrlistanecnecrologi) throw()
{
	// numero degli elemnti da riordinare
	int32 lNumeroElementi = 0;
	// array da riordinare
	altezza *lPtrAltezze = NULL;
	// per prendere un elemento dell'array dell'altezze
	altezza *lPtrAltezza = NULL;
	// indice dell'altezze
	int32 lIndiceAltezze = 0;
	// per scorrere la lista
	listanec *lPtrListaNec = NULL;
	// per ricostruire la lista
	listanec **lPtrListaNecSuccessivo = NULL;
	// per ricostruire la lista
	listanec *lPtrListaNecPrecedente = NULL;
	// per prendere lo spazio recuperabile sulla colonna
	Fixed lSpazioRecuperabile = 0;
	
	if ((*ptrlistanecnecrologi) == NULL) 
		return;
	
	// conto gli elementi della lista
	lNumeroElementi = ContaElementiListaNec(*ptrlistanecnecrologi);
	
	// creo l'array da riordinare
	lPtrAltezze = (altezza *) NewPtrClear(lNumeroElementi * sizeof(altezza));
	if (lPtrAltezze == NULL) 
	{
		MostraErrore(kErroreMemoria);
		return;
	}
	
	// imposto il puntatore
	lPtrListaNec = *ptrlistanecnecrologi;
	lIndiceAltezze = 0;
	
	// inserisco le altezze nell'array
	while (1) 
	{
		if (lPtrListaNec == NULL) 
			break;
		
		// imposto il puntatore all'altezza
		lPtrAltezza = lPtrAltezze + lIndiceAltezze;
		
		// copio i valori
		(*lPtrAltezza).puntatore = (Ptr) lPtrListaNec;
		(*lPtrAltezza).altezzaDoubleFixed = (DoubleFixed) (*(*lPtrListaNec).ptrNecrologio).altezzaBox;
		
		// avanzo il puntatore
		lPtrListaNec = (*lPtrListaNec).successivo;
		
		// incremento l'indice
		lIndiceAltezze++;
	}
	
	// imposto lo spazio rimanete globale
	gSpazioRimanente = (*ptrspaziocolonna).spazioOttimale -
					   (*ptrspaziocolonna).spazioOccupato -
					   (*ptrspaziocolonna).spazioTraNecrologi;

	if (scarto > 0) 
	{
		// nelle colonne precedenti ho inserito meno di quello che dovevo inserire
		lSpazioRecuperabile = PrendiRestringimento(ptrspaziocolonna);
		if (scarto > lSpazioRecuperabile)
			gSpazioRimanente += PrendiRestringimento(ptrspaziocolonna);
		else 
			gSpazioRimanente += scarto;
	} else if (scarto < 0) 
	{
		// nelle colonne precedenti ho inserito piu' di quello che dovevo inserire
		lSpazioRecuperabile = -PrendiAllargamento(ptrspaziocolonna);
		if (scarto > lSpazioRecuperabile)
			gSpazioRimanente += scarto;
		else 
			gSpazioRimanente += lSpazioRecuperabile;
	}
	
	// riordino l'array dei necrologi
	qsort(lPtrAltezze, (size_t) lNumeroElementi,
			(size_t) sizeof(altezza), (int (*)(const void*, const void*))OrdinaAltezzeNecrologi);

	// imposto i puntatori
	lPtrListaNecSuccessivo = ptrlistanecnecrologi;
	lPtrListaNecPrecedente = NULL;

	// ricostruisco la lista dei necrologi
	for (lIndiceAltezze = 0; lIndiceAltezze < lNumeroElementi; lIndiceAltezze++)
	{
		// imposto il puntatore all'altezza
		lPtrAltezza = lPtrAltezze + lIndiceAltezze;

		// imposto il puntatore del giro precedente
		lPtrListaNec = (listanec *) (*lPtrAltezza).puntatore;
		(*lPtrListaNecSuccessivo) = lPtrListaNec;
		
		// imposto l'elemento corrente
		(*lPtrListaNec).precedente = lPtrListaNecPrecedente;
		(*lPtrListaNec).successivo = NULL;
		
		// imposto i puntatori per il giro successivo
		lPtrListaNecSuccessivo = &((*lPtrListaNec).successivo);
		lPtrListaNecPrecedente = lPtrListaNec;
	}
	
	// libero la memoria dell'array delle altezze
	if (lPtrAltezze != NULL) 
	{
		DisposePtr((Ptr) lPtrAltezze);
		lPtrAltezze = NULL;
	}
} // RiordinaListaNec

/* ------------------------------------------------------------------------ *

	EseguiNormaleNecrologi

* ------------------------------------------------------------------------ */
static tipochiusura XTAPI EseguiNormaleNecrologi(spaziocolonna *ptrspaziocolonna, listanec **ptrlistanecnecrologi, Fixed scarto) throw()
{
	// per prendere il tipo di chiusura della colonna
	tipochiusura lTipoChiusura = kNessunaChiusura;
	// per prendere lo spazio irrecuperabile
	Fixed lDifferenza = 0;
	// per calcolare lo spazio rimanente
	Fixed lSpazioRimanente = 0;
	// puntatore al necrologio corrente
	listanec *lPtrElementoCorrente = NULL;
	// chiusura derlla colonna finale
	tipochiusura lChiusuraFinale = kNessunaChiusura;
	// puntatore alla lista degli scartati
	listanec *lPtrListaNecScartati = NULL;
	
	while (1) 
	{
		// ordina la lista dei necrologi da considerare
		RiordinaListaNec(ptrspaziocolonna, scarto, ptrlistanecnecrologi);
	
		// estraggo la prima mossa
		lPtrElementoCorrente = EstraiDaListaNec(ptrlistanecnecrologi);
			
		if (lPtrElementoCorrente == NULL) 
			break;
			
		// inserisco l'elemento nella colonna
		InserisciNecrologioInCoda(ptrspaziocolonna, (*lPtrElementoCorrente).ptrNecrologio);
	
		// controllo il rispetto delle preferenze
		gErroreXtension = ControllaChiusuraColonna(ptrspaziocolonna,
												   &lTipoChiusura,
												   &lDifferenza);
		if (gErroreXtension != kNessunErrore) 
		{
			MostraErrore(gErroreXtension);
			return(kNessunaChiusura);
		}
		
		if (lTipoChiusura == kPreferenzeNonRispettate) 
		{
			// tolgo il necrologio dalla colonna
			RimuoviNecrologio(ptrspaziocolonna, (*lPtrElementoCorrente).ptrNecrologio); 

			// controllo se posso finire il ciclo
			if (lChiusuraFinale == kPreferenzeRispettate) 
			{
				// preferenze rispettate
			
				// rinserisco l'elemento nella lista dei necrologi
				InserisciElementoInListaNec(&lPtrElementoCorrente, ptrlistanecnecrologi);
				
				break;
				
			}
			
			// inserisco l'elemento nella lista dei necrologi scartati
			InserisciElementoInListaNec(&lPtrElementoCorrente, &lPtrListaNecScartati);

			if ((*ptrlistanecnecrologi) != NULL) 
			{
				// ci sono dei necrologi ancora da provare
			
			} 
			else if (gPtrListaNecTappaBuchi != NULL) 
			{
				// uso i tappabuchi
			
				// ordina la lista dei defunti da tapabuchi
				RiordinaListaNec(ptrspaziocolonna, scarto, &gPtrListaNecTappaBuchi);
	
				// estraggo il primo tappabuchi
				lPtrElementoCorrente = EstraiDaListaNec(&gPtrListaNecTappaBuchi);
				
				// tolgo il defunto trovato dalla colonna in cui era inserito
				RimuoviNecrologio((*lPtrElementoCorrente).ptrSpazioColonna,
								  (*lPtrElementoCorrente).ptrNecrologio); 
				
				/* rinserisco l'elemento nella lista dei necrologi */
				(*lPtrElementoCorrente).ptrSpazioColonna = NULL;
				InserisciElementoInListaNec(&lPtrElementoCorrente, ptrlistanecnecrologi);
			
			} 
			else 
			{
			
				// non riesco a chiudere questa colonna
				lChiusuraFinale = kPreferenzeNonRispettate;
			
				break;
			}
		
		} 
		else if (lTipoChiusura == kPreferenzeRispettate) 
		{
			// preferenze rispettate

			if (lChiusuraFinale == kPreferenzeRispettate) 
			{
				// l'elemento corrente puo' essere usato come tappabuchi
				(*lPtrElementoCorrente).ptrSpazioColonna = ptrspaziocolonna;
				InserisciElementoInListaNec(&lPtrElementoCorrente, &gPtrListaNecTappaBuchi);	
			} 
			else 
			{
				// salvo la chiusura
				lChiusuraFinale = kPreferenzeRispettate;
			}
			

			// calcolo lo spazio rimanente nella colonna corrente
			lSpazioRimanente = (*ptrspaziocolonna).spazioOttimale - 
								(*ptrspaziocolonna).spazioOccupato -
							   (*ptrspaziocolonna).spazioTraNecrologi;
			
			// tengo conto dello scarto
			lSpazioRimanente += scarto;
			
			if (lSpazioRimanente < 0) 
				break;			
		} 
		else 
		{
			// devo inserire ancora
			lChiusuraFinale = kInserireAncora;
		}
		
		// libero la memoria del'elemento corrente
		if (lPtrElementoCorrente != NULL) 
		{
			DisposePtr((Ptr) lPtrElementoCorrente);
			lPtrElementoCorrente = NULL;
		}				
	}
	
	// libero la memoria del'elemento corrente
	if (lPtrElementoCorrente != NULL) 
	{
		DisposePtr((Ptr) lPtrElementoCorrente);
		lPtrElementoCorrente = NULL;
	}
				
	// prendo gli elementi della lista degli scartati e li metto nella lista 
	// dei defunti
	if (lPtrListaNecScartati != NULL) 
	{
		lPtrElementoCorrente = EstraiDaListaNec(&lPtrListaNecScartati);
	
		while (1) 
		{	
			if (lPtrElementoCorrente == NULL) 
				break;
			
			InserisciElementoInListaNec(&lPtrElementoCorrente, ptrlistanecnecrologi);
			
			lPtrElementoCorrente = EstraiDaListaNec(&lPtrListaNecScartati);
		}
	}
	
	if (lChiusuraFinale == kPreferenzeNonRispettate) 
	{
		// controllo la chiusura della colonna
		gErroreXtension = ControllaChiusuraColonna(ptrspaziocolonna,
												   &lTipoChiusura,
												   &lDifferenza);
		if (gErroreXtension != kNessunErrore) 
		{
			MostraErrore(gErroreXtension);
			return(kNessunaChiusura);
		}
		
		return(lTipoChiusura);
		
	} 
	else 
	{
		return(lChiusuraFinale);
	}
} // EseguiNormaleNecrologi

/* ------------------------------------------------------------------------ *

	NormaleNecrologi

* ------------------------------------------------------------------------ */
static void XTAPI NormaleNecrologi(defunto *ptrdefunto, int16 numerocolonne, spaziocolonna *ptrspaziocolonne, Fixed *ptrscarto) throw()
{
	// per prendere lo spazio colonna
	spaziocolonna *lPtrSpazioColonna = NULL;
	// per scorrere lo spazio colonna da usare per il calcolo del posizionamento
	int16 lIndiceSpaziColonne = 0;
	// per memorizzare il puntatore alla lista dei necrologi da inserire
	listanec *lPtrListaNecNecrologi = NULL;
	// per sapere come e' stata chiusa una colonna
	tipochiusura lTipoChiusura = kNessunaChiusura;
	// per posizionare i necrologi avanzati
	listanec *lPtrElementoCorrente = NULL;
	
	// controllo che ci siano necrologi da inserire
	if ((*ptrdefunto).elencoNecrologiC == NULL) 
		return;
	
	// costruisci la listanec dei necrologi da inserire
	CostruisciListaNec(&lPtrListaNecNecrologi, (*ptrdefunto).elencoNecrologiC);
	
	// rimuovo il link tra i necrologi e il defunto
	(*ptrdefunto).elencoNecrologiC = NULL;
	(*ptrdefunto).ultimoNecrologioC = NULL;
	
	// per tutte le colonne eseguo l'inserimento sui necrologi
	for (lIndiceSpaziColonne = 0; lIndiceSpaziColonne < numerocolonne;
		 lIndiceSpaziColonne++) 
	{	 
		// prendo le info della colonna
		lPtrSpazioColonna = ptrspaziocolonne + lIndiceSpaziColonne;

		// test per verificare se ho già chiuso la colonna
		if ((*lPtrSpazioColonna).chiusuraColonna == kChiusa &&
			 (*lPtrSpazioColonna).tipoChiusura == kPreferenzeRispettate)
		{
			continue;
		}

		// riempo la colonna corrente
		lTipoChiusura = EseguiNormaleNecrologi(lPtrSpazioColonna,
												&lPtrListaNecNecrologi,
												(*ptrscarto));


		(*ptrscarto) += (*lPtrSpazioColonna).spazioOttimale -
						  (*lPtrSpazioColonna).spazioOccupato -
							(*lPtrSpazioColonna).spazioTraNecrologi;
	}

	// libero la lista dei tapabuchi
	LiberaListaNec(&gPtrListaNecTappaBuchi);

	// controllo se sono stati inseriti tutti i necrologi 
	if (lPtrListaNecNecrologi != NULL) 
	{
		// posiziono i necrologi avanzati fuori
		while (1) 
		{
			lPtrElementoCorrente = EstraiDaListaNec(&lPtrListaNecNecrologi);

			if (lPtrElementoCorrente == NULL) 
				break;

			// prendo le info della colonna per i box fuori
			lPtrSpazioColonna = PrendiSpazioColonnaFuori();

			// modifico lo scarto
			(*ptrscarto) -= (*(*lPtrElementoCorrente).ptrNecrologio).altezzaBox;
			if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE)
				(*ptrscarto) -= kFixedMinimoSpazioTraNecrologi;
			else 
				(*ptrscarto) -= gXtPreferences.giustificazionePref.valoreIniziale;

			// inserisco il necrologio in coda alla colonna corrente
			InserisciNecrologioInCoda(lPtrSpazioColonna,
									  (*lPtrElementoCorrente).ptrNecrologio);

			// libero la memoria del'elemento corrente
			if (lPtrElementoCorrente != NULL) 
			{
				DisposePtr((Ptr) lPtrElementoCorrente);
				lPtrElementoCorrente = NULL;
			}

		}
	}
} // NormaleNecrologi

/* ------------------------------------------------------------------------ *

	NormaleNecrologiA

* ------------------------------------------------------------------------ */
static void XTAPI NormaleNecrologiA(defunto *ptrdefunto, int numerocolonne, spaziocolonna *ptrspaziocolonne) throw()
{
	// per prendere lo spazio colonna
	spaziocolonna *lPtrSpazioColonna = NULL;
	// per scorrere lo spazio colonna da usare per il calcolo del posizionamento
	int16 lIndiceSpaziColonne = 0;
	// per prendere i necrologi della lista dei defunti passata
	necrologio *lPtrNecrologio = NULL;
	// per calcolare lo spazio rimanente
	Fixed lSpazioRimanente = 0;
	
	// posiziono i necrologi di codice A
	lPtrNecrologio = EstraiSoloNecrologioA(ptrdefunto);

	// imposto l'indice di scorrimento
	lIndiceSpaziColonne = 0;

	while (1) 
	{
		// ho finito di inserire i necrologi di codice A
		if (lPtrNecrologio == NULL) 
			break;

		// prendo le info dello spazio colonna corrente
		lPtrSpazioColonna = ptrspaziocolonne + lIndiceSpaziColonne;

		// calcolo lo spazio rimanente nella colonna corrente
		lSpazioRimanente = (*lPtrSpazioColonna).spazioDisponibile -
							(*lPtrSpazioColonna).spazioOccupato -
							(*lPtrSpazioColonna).spazioTraNecrologi;

		// controllo se posso inserire il necrologio corrente
		if ((lSpazioRimanente >= (*lPtrNecrologio).altezzaBox) ||
			(lIndiceSpaziColonne == numerocolonne - 1)) 
		{

			// inserisco il necrologio in coda alla colonna corrente
			InserisciNecrologioInCoda(lPtrSpazioColonna, lPtrNecrologio);

			// controllo se e' un box cooleagto a quello successivo
			if ((*lPtrNecrologio).collegato == TRUE) 
			{
				// dico che la colonna corrente non e' usabile
				(*lPtrSpazioColonna).chiusuraColonna = kChiusa;
				(*lPtrSpazioColonna).tipoChiusura = kPreferenzeRispettate;

				// devo passare alla colonna successiva
				lIndiceSpaziColonne++;
			}

			// passo al necrologio successivo
			lPtrNecrologio = EstraiSoloNecrologioA(ptrdefunto);
		} 
		else 
		{
			// dico che la colonna corrente non e' usabile
			(*lPtrSpazioColonna).chiusuraColonna = kChiusa;

			// devo passare alla colonna successiva
			lIndiceSpaziColonne++;
		}

	}
} // NormaleNecrologiA

/* ------------------------------------------------------------------------ *

	NormaleNecrologiB

* ------------------------------------------------------------------------ */
static void XTAPI NormaleNecrologiB(defunto *ptrdefunto, int numerocolonne, spaziocolonna *ptrspaziocolonne) throw()
{
	// per prendere lo spazio colonna
	spaziocolonna *lPtrSpazioColonna = NULL;
	// per scorrere lo spazio colonna da usare per il calcolo del posizionamento
	int16 lIndiceSpaziColonne = 0;
	// per prendere i necrologi della lista dei defunti passata
	necrologio *lPtrNecrologio = NULL;
	// per prendere il tipo gruppo
	char lTipoGruppo = kNessunGruppo;
	// per calcolare lo spazio rimanente
	Fixed lSpazioRimanente = 0;
	int16 lOldIndiceSpaziColonne = -1;

	// prendo il primo necrologio
	lPtrNecrologio = EstraiSoloNecrologioB(ptrdefunto);

	// posiziono i necrologi di codice B
	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;

		// prendo il codice del gruppo
		lTipoGruppo = (*lPtrNecrologio).tipoGruppo;

		// scelgo la colonna migliore per inserire questo defunto
		ScegliColonna(ptrspaziocolonne, numerocolonne, &lIndiceSpaziColonne);

		// prendo le info della colonna corrente
		lPtrSpazioColonna = ptrspaziocolonne + lIndiceSpaziColonne;

		while (1)
		{
			// calcolo lo spazio rimanente nella colonna corrente
			lSpazioRimanente =	(*lPtrSpazioColonna).spazioDisponibile -
										(*lPtrSpazioColonna).spazioOccupato -
										(*lPtrSpazioColonna).spazioTraNecrologi;

			// controllo se posso inserire il necrologio corrente
			if ((lSpazioRimanente >= (*lPtrNecrologio).altezzaBox) ||
				lOldIndiceSpaziColonne == lIndiceSpaziColonne)
			{
				// inserisco il necrologio in coda alla colonna corrente
				InserisciNecrologioInCoda(lPtrSpazioColonna, lPtrNecrologio);

				// prendo il necrologio successivo
				lPtrNecrologio = EstraiSoloNecrologioB(ptrdefunto);
			}
			else
			{
				// dico che la colonna corrente non è usabile
				(*lPtrSpazioColonna).chiusuraColonna = kChiusa;
				
				if (lOldIndiceSpaziColonne != lIndiceSpaziColonne)
				{
					lOldIndiceSpaziColonne = lIndiceSpaziColonne;
					break;
				}
				else
				{
					// devo passare alla colonna successiva
					lIndiceSpaziColonne++;

					// prendo le info della nuova colonna
					lPtrSpazioColonna = ptrspaziocolonne + lIndiceSpaziColonne;
				}
			}

			if (lPtrNecrologio == NULL) 
				break;

			// se e' ancora dello stesso gruppo continuo l'inserimento
			if (lTipoGruppo != (*lPtrNecrologio).tipoGruppo) 
				break;
		}

	}

} // NormaleNecrologiB

/* ------------------------------------------------------------------------ *

	CalcolaMigliorOrdineNecrologi

* ------------------------------------------------------------------------ */
void XTAPI CalcolaMigliorOrdineNecrologi(defunto *ptrdefunto, int16 pagina, int16 colonna, Fixed *ptrscarto, necrologio **ptrnecrologi) throw()
{
	// per prendere l'altezza totale degli elementi da riordinare
	DoubleFixed lAltezzaTotaleDoubleFixed = 0.0;
	// per contare il numero dei necrologi
	int16 lNumeroNecrologi = 0;
	// per scorrere le colonne
	int16 lColonna = 0;
	// per scorrere le pagine 
	int16 lPagina = 0;
	// per contare quante colonne bisogna occupare 
	int16 lNumeroColonne = 0;
	// per prendere lo spazio ottimale 
	uint32 lSpazioOttimale = 0;
	// per prendere lo spazio colonna 
	spaziocolonna *lPtrSpazioColonna = NULL;
	// per creare gli spazio colonna per posizionare questi defunti 
	spaziocolonna *lPtrSpaziColonne = NULL;
	// per scorrere lo spazio colonna da usare per il calcolo del posizionamento 
	int16 lIndiceSpaziColonne = 0;
	// per generare la lista finale dei necrologi 
	necrologio *lPtrListaFinaleNecrologi = NULL;
	// puntatore all'indirizzo del necrologio precedente 
	necrologio **lPtrPrecedente = NULL;
	// per creare il necrologio vuoto di cambio colonna 
	necrologio *lPtrNecrologio = NULL;
	
	// controllo il puntatore al defunto non sia vuoto
	if (ptrdefunto == NULL) 
	{
		(*ptrnecrologi) = NULL;
		return;
	}

	// prende l'altezza totale
	lAltezzaTotaleDoubleFixed = PrendiAltezzaDefunto(ptrdefunto);
	
	// prendo il numero dei necrologi del defunto
	lNumeroNecrologi = PrendiNumeroNecrologiDefunto(ptrdefunto);
	
	// incremento l'altezza totale
	if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE)
		lAltezzaTotaleDoubleFixed += (DoubleFixed) (lNumeroNecrologi * kFixedMinimoSpazioTraNecrologi);
	else lAltezzaTotaleDoubleFixed += (DoubleFixed) (lNumeroNecrologi * gXtPreferences.giustificazionePref.valoreIniziale);
	
	// per non perdere il valore in entrata della colonna di partenza
	lColonna = colonna;
	
	// determina quante colonne bisogna occupare
	for (lPagina = pagina; lPagina <= PrendiNumeroPagine(); lPagina++) 
	{
		for (lColonna = lColonna; lColonna <= PrendiNumeroColonne(); lColonna++) 
		{
			// incremento il contatore delle colonne
			lNumeroColonne++;
		
			// prendo lo spazio della colonna corrente
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);

			lSpazioOttimale = (*lPtrSpazioColonna).spazioOttimale;
			
			if (lAltezzaTotaleDoubleFixed < (DoubleFixed) lSpazioOttimale) 
				break;

			// decremento l'altezza
			lAltezzaTotaleDoubleFixed -= (DoubleFixed) lSpazioOttimale;
		}
		if (lColonna <= PrendiNumeroColonne() && lAltezzaTotaleDoubleFixed < (DoubleFixed) lSpazioOttimale) 
			break;
		lColonna = 1;
	}
	
	// creo la memoria per le colonne da usare per il calcolo della posizione dei necrologi
	lPtrSpaziColonne = (spaziocolonna *) NewPtrClear(sizeof(spaziocolonna) * lNumeroColonne);
	if (lPtrSpaziColonne == NULL) 
	{
		MostraErrore(kErroreMemoria);
		return;
	}
	
	// imposto l'indice di scorrimento
	lIndiceSpaziColonne = 0;
	
	// per non perdere il valore in entrata della colonna di partenza
	lColonna = colonna;
	
	// copio le informazioni per lo spazio colonna appena creato
	for (lPagina = pagina; lPagina <= PrendiNumeroPagine(); lPagina++) 
	{
		for (lColonna = lColonna; lColonna <= PrendiNumeroColonne(); lColonna++) 
		{
			// imposto il puntatore
			lPtrSpazioColonna = lPtrSpaziColonne + lIndiceSpaziColonne;
			
			// copio le informazioni dello spazio colonna
			(*lPtrSpazioColonna) = (*PrendiSpazioColonna(lPagina, lColonna));
			
			// incremento l'indice dello spazio colonna
			lIndiceSpaziColonne++;
			
			if (lIndiceSpaziColonne == lNumeroColonne) 
				break;
		}
		if (lIndiceSpaziColonne == lNumeroColonne) 
			break;
		lColonna = 1;
	}
	
	// copio l'ultima l'altezza
	(*lPtrSpazioColonna).spazioOttimale = (Fixed) lAltezzaTotaleDoubleFixed;
	assert(0 < (*lPtrSpazioColonna).spazioOttimale);

	// inserisco i necrologi con codice A
	NormaleNecrologiA(ptrdefunto, lNumeroColonne, lPtrSpaziColonne);
	
	// inserisco i necrologi codice B
	NormaleNecrologiB(ptrdefunto, lNumeroColonne, lPtrSpaziColonne);

	// inserisco i necrologi di codice C
	NormaleNecrologi(ptrdefunto, lNumeroColonne, lPtrSpaziColonne, ptrscarto);
	
	lPtrPrecedente = &lPtrListaFinaleNecrologi;

	// costruisco la lista dei necrologi riordinata
	for (lIndiceSpaziColonne = 0; lIndiceSpaziColonne < lNumeroColonne; lIndiceSpaziColonne++) 
	{
		// prendo le info della colonna
		lPtrSpazioColonna = lPtrSpaziColonne + lIndiceSpaziColonne;
		
		if ((*lPtrSpazioColonna).numeroNecrologi != 0) 
		{
			(*lPtrPrecedente) = (*lPtrSpazioColonna).testaNecrologi;
			if (lIndiceSpaziColonne != 0) 
			{
				(*(*lPtrPrecedente)).cambioColonna = TRUE;
			}
			lPtrPrecedente = &((*((*lPtrSpazioColonna).codaNecrologi)).successivo);
		} 
		else 
		{
			lPtrNecrologio = (necrologio *) NewPtrClear(sizeof(necrologio));
			if (lPtrNecrologio == NULL) 
			{
				MostraErrore(kErroreMemoria);
				return;
			}
			
			(*lPtrNecrologio).codiceBox = 0;
			(*lPtrNecrologio).cambioColonna = TRUE;
			
			(*lPtrPrecedente) = lPtrNecrologio;
			lPtrPrecedente = &((*lPtrNecrologio).successivo);
		}
	}
	
	(*ptrnecrologi) = lPtrListaFinaleNecrologi;

	// libero la memoria usata per creare lo spazio colonna
	if (lPtrSpaziColonne != NULL) 
	{
		DisposePtr((Ptr) lPtrSpaziColonne);
		lPtrSpaziColonne = NULL;
	}
} // CalcolaMigliorOrdineNecrologi
