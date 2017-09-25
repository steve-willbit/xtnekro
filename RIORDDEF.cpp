/* ------------------------------------------------------------------------ *

	RiordinoDefunti.cpp

	28 Aprile 2005					Fabrizio

	Descrizione:
	Contiene le procedure per il riordino dei defunti

	$Log: not supported by cvs2svn $
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
#include "Pref.h"
#include "Paletta.h"
#include "ResStr.h"
#include "StMes.h"
#include "InfoNec.h"
#include "SpaceCol.h"
#include "CalcPos.h"
#include "XTNecro.h"

#include "DisposizioneWap.h"

#include "RiordDef.h"

// STATICS GLOBALS
// STATICS GLOBALS
// STATICS GLOBALS

// e' una lista che contiene i defunti che sono stati messi nelle colonne ma
// che possono essere rimossi per essere usati come tappabuchi
static listadef *gPtrListaDefTappaBuchi = NULL;

// e' una lista che contiene i defunti che sono stati messi nelle colonne ma
// che possono essere scambiati con quelli che devono essere ancora inseriti
static listadef *gPtrListaDefScambiabili = NULL;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			LiberaListaDef
	@abstract 		gestione riordinamento necrologi
	@discussion		Libera la memoria alloccata per una lista defunti

					28 Aprile 2005 - Fabrizio.

	@param 			ptrlistadef - puntatore al puntatore alla lista dei defunti da liberare		
	@return			nessuno				
*/
static void XTAPI LiberaListaDef(listadef **ptrlistadef) throw();

/*!
	@function			CostruisciListaDef
	@abstract 		gestione riordinamento necrologi
	@discussion		Allocca la memoria per l'elenco dei necrologi ancora da inserire

					28 Aprile 2005 - Fabrizio.

	@param 			ptrlistadacostruire - puntatore al puntatore alla lista mosse dei necrologi
					ancora da inserire		
	@return			nessuno				
*/
static void XTAPI CostruisciListaDef(listadef **ptrlistadacostruire, defunto *ptrdefunti) throw();

/*!
	@function			RimuoviElementoListaDef
	@abstract 		gestione riordinamento necrologi
	@discussion		Cerca nella lista l'elemento e lo rimuove

					28 Aprile 2005 - Fabrizio.

	@param 			ptrelemento - puntatore all'elemento da rimuovere
	@param			ptrlista - contiene il puntatore all'elenco dei defunti in cui fare la rimozione
	@return			nessuno				
*/
static void XTAPI RimuoviElementoListaDef(listadef *ptrelemento, listadef **ptrlista) throw();

/*!
	@function			EstraiDaListaDefConRigore
	@abstract 		gestione riordinamento necrologi
	@discussion		Prende il primo elemento della lista con rigore

					29 Aprile 2005 - Fabrizio.

	@param 			ptrlista - contiene il puntatore all'elenco dei defunti ancora da trattare
	@return			ritorna il primo elemento della lista passata con rigore				
*/
static listadef* XTAPI EstraiDaListaDefConRigore(listadef **ptrlista) throw();

/*!
	@function			EstraiDaListaDef
	@abstract 		gestione riordinamento necrologi
	@discussion		Prende il primo elemento della lista e aggiorna il puntatore alla lista
					all'elemento successivo

					29 Aprile 2005 - Fabrizio.

	@param 			ptrlista - contiene il puntatore all'elenco dei defunti ancora da trattare
					dalla procedura ricorsiva
	@return			ritorna il primo elemento della lista passata				
*/
static listadef* XTAPI EstraiDaListaDef(listadef **ptrlista) throw();

/*!
	@function			DuplicaElementoListaDef
	@abstract 		gestione riordinamento necrologi
	@discussion		Duplica l'elemento corrente

					29 Aprile 2005 - Fabrizio.

	@param 			ptrelementodaduplicare - puntatore all'elemento da duplicare
	@param			ptrelementoduplicato - puntatore all'elemento duplicato
	@return			ritorna l'eventuale errore dell'xtension			
*/
static errorixtension XTAPI DuplicaElementoListaDef(listadef *ptrelementodaduplicare, listadef **ptrelementoduplicato) throw();

/*!
	@function			InserisciElementoInListaDef
	@abstract 		gestione riordinamento necrologi
	@discussion		Inserisce in coda l'elemento ptrelemento nella lista ptrlistadefdefunti

					29 Aprile 2005 - Fabrizio.

	@param 			ptrelemento - puntatore all'elemento da inserire
	@param			ptrlistadefdefunti - lista in cui inserire l'elemento
	@return			nessuno	
*/
static void InserisciElementoInListaDef(listadef **ptrelemento,	listadef **ptrlistadefdefunti) throw();

/*!
	@function			ContaElementiListaDef
	@abstract 		gestione riordinamento necrologi
	@discussion		Conta il numero di elementi della lista

					29 Aprile 2005 - Fabrizio.

	@param			ptrlistadefdefunti - lista da contare
	@return			nessuno	
*/
static int32 XTAPI ContaElementiListaDef(listadef *ptrlistadefdefunti) throw();

/*!
	@function			OrdinaAltezzeDefunti
	@abstract 		gestione riordinamento necrologi
	@discussion		Ordina in modo decrescente sul valore assoluto della distanza tra lo spazio rimanente
					e l'altezza del defunto

					29 Aprile 2005 - Fabrizio.

	@param			a - puntatore alle info del defunto
	@param			b - puntatore alle info del defunto
	@return			0 se le altezze sono uguali, -1 se a e' piu' piccolo, 1 se a e' piu' grande	
*/
static int16 XTAPI OrdinaAltezzeDefunti(const void  *aa, const void  *bb) throw();
/*!
	@function			RiordinaListaDef
	@abstract 		gestione riordinamento necrologi
	@discussion		Per riodinare l'elenco dei necrologi in base allo spazio che rimane da occupare

					29 Aprile 2005 - Fabrizio.

	@param			ptrspaziocolonna - puntatore alla colonna in cui inserire
	@param			scarto - scarto della colonna precedente
	@param			ptrlistadefdefunti - puntatore alla lista dei necrologi da inserire
	@return			nessuno	
*/
static void XTAPI RiordinaListaDef(spaziocolonna *ptrspaziocolonna, Fixed scarto, listadef **ptrlistadefdefunti) throw();

/*!
	@function			EseguiNormaleDefunti
	@abstract 		gestione riordinamento necrologi
	@discussion		Procedura iterativa per cercare di riempire la colonna

					29 Aprile 2005 - Fabrizio.

	@param			ptrspaziocolonna - puntatore alla colonna in cui inserire
	@param			ptrlistadefdefunti - puntatore alla lista dei necrologi da inserire
	@param			scarto - scarto della colonna precedente
	@return			tipo di chiusura	
*/
static tipochiusura XTAPI EseguiNormaleDefunti(spaziocolonna *ptrspaziocolonna, listadef **ptrlistadefdefunti, Fixed scarto) throw();

/*!
	@function			DefuntoFondoRigoreColonna
	@abstract 		gestione riordinamento necrologi
	@discussion		Per inserire l'eventuale defunto con rigore di colonna in fondo

					29 Aprile 2005 - Fabrizio.

	@param			ptrspaziocolonna - puntatore alla colonna in cui inserire
	@param			ptrlistadefdefunti - puntatore alla lista dei necrologi da inserire
	@return			nessuno	
*/
static void XTAPI DefuntoFondoRigoreColonna(spaziocolonna *ptrspaziocolonna, listadef **ptrlistadefdefunti) throw();

/*!
	@function			EseguiNormaleDefuntiFondo
	@abstract 		gestione riordinamento necrologi
	@discussion		Procedura iterativa per cercare di riempire la colonna partendo dal fondo

					29 Aprile 2005 - Fabrizio.

	@param			ptrspaziocolonna - puntatore alla colonna in cui inserire
	@param			ptrlistadefdefunti - puntatore alla lista dei necrologi da inserire
	@param			scarto - scarto della colonna precedente
	@return			tipo di chiusura	
*/
static tipochiusura XTAPI EseguiNormaleDefuntiFondo(spaziocolonna *ptrspaziocolonna, listadef **ptrlistadefdefunti, Fixed scarto) throw();

/*!
	@function			NormaleDefunti
	@abstract 		gestione riordinamento necrologi
	@discussion		Cerca di riordinare i defunti in modo da riepire le colonne da usare al meglio

					2 Maggio 2005 - Fabrizio.

	@param			ptrdefunto - puntatore alla lista dei defunti
	@param			pagina - pagina di partenza
	@param			colonna - colonna di partenza
	@param			ptrscarto - per ritornare lo scarto corrente
	@return			tipo di chiusura	
*/
static errorixtension XTAPI NormaleDefunti(defunto *ptrdefunto, int16 pagina, int16 colonna, Fixed *ptrscarto) throw();

/*!
	@function			NormaleDefuntiFondo
	@abstract 		gestione riordinamento necrologi
	@discussion		Cerca di riordinare i defunti in modo da riepire le colonne da usare al meglio
					partendo dal fondo del documento

					2 Maggio 2005 - Fabrizio.

	@param			ptrdefunto - puntatore alla lista dei defunti
	@param			pagina - pagina di partenza
	@param			colonna - colonna di partenza
	@param			ptrscarto - per ritornare lo scarto corrente
	@return			ritorna l'eventuale errore dell'xtension	
*/
static errorixtension XTAPI NormaleDefuntiFondo(defunto *ptrdefunto, int16 pagina, int16 colonna, Fixed *ptrscarto) throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	LiberaListaDef

* ------------------------------------------------------------------------ */
static void XTAPI LiberaListaDef(listadef **ptrlistadef) throw()
{
	// per scorrere la lista
	listadef *lPtrListaDef = NULL;
	// per liberare la lista
	listadef *lPtrListaDefDaLiberare = NULL;
	
	lPtrListaDef = (*ptrlistadef);

	while (1) 
	{
		if (lPtrListaDef == NULL) 
			break;
		
		lPtrListaDefDaLiberare = lPtrListaDef;
		
		lPtrListaDef = (*lPtrListaDef).successivo;
		DisposePtr((Ptr) lPtrListaDefDaLiberare);	
	}
	
	(*ptrlistadef) = NULL;
} // LiberaListaDef

/* ------------------------------------------------------------------------ *

	CostruisciListaDef

* ------------------------------------------------------------------------ */
static void XTAPI CostruisciListaDef(listadef **ptrlistadacostruire, defunto *ptrdefunti) throw()
{
	// per costruire un elemento della lista delle mosse
	listadef *lPtrListaDef = NULL;
	// per scorrere la lista dei defunti
	defunto *lPtrDefunto = NULL;
	// puntatore al successivo
	listadef **lPtrListaDefSuccessivo = NULL;
	// puntatore al precedente
	listadef *lPtrListaDefPrecedente = NULL;
	
	// imposto i puntatori
	lPtrDefunto = ptrdefunti;
	lPtrListaDefSuccessivo = ptrlistadacostruire;
	lPtrListaDefPrecedente = NULL;
	
	while (1) 
	{
		// controllo se devo fermarmi
		if (lPtrDefunto == NULL) 
			break;
		
		// creo un nuovo elemento
		lPtrListaDef = (listadef *) NewPtrClear(sizeof(listadef));
		if (lPtrListaDef == NULL) 
		{
			MostraErrore(kErroreMemoria);
			return;
		}
		
		// imposto il puntatore precedente
		(*lPtrListaDefSuccessivo) = lPtrListaDef;
		
		// inserisco i valori nel nuovo elemento
		(*lPtrListaDef).ptrDefunto = lPtrDefunto;
		(*lPtrListaDef).ptrSpazioColonna = NULL;
		(*lPtrListaDef).precedente = lPtrListaDefPrecedente;
		(*lPtrListaDef).successivo = NULL;
		
		// imposto i puntatori per il prossimo iserimento
		lPtrListaDefSuccessivo = &((*lPtrListaDef).successivo);
		lPtrListaDefPrecedente = lPtrListaDef;
		
		// per scorrere la lista
		lPtrDefunto = (*lPtrDefunto).successivo;
	}
} // CostruisciListaDef

/* ------------------------------------------------------------------------ *

	RimuoviElementoListaDef

* ------------------------------------------------------------------------ */
static void XTAPI RimuoviElementoListaDef(listadef *ptrelemento, listadef **ptrlista) throw()
{
	// puntatore del defunto da rimuovere
	defunto *lPtrDefuntoDaRimuovere = NULL;
	// per scorrere la lista
	listadef *lPtrListaDef = NULL;
	// per tenere il puntatore all'elemento precedente
	listadef *lPtrListaDefPrecedente = NULL;
	// per tenere il puntatore del campo successivo dell'elemento precedente
	listadef **lPtrListaDefSuccessivo = NULL;
	
	// imposto i puntatori
	lPtrDefuntoDaRimuovere = (*ptrelemento).ptrDefunto;
	lPtrListaDef = (*ptrlista);
	lPtrListaDefPrecedente = NULL;
	lPtrListaDefSuccessivo = ptrlista;
	
	while (1) 
	{
		if (lPtrListaDef == NULL) 
			break;
		if ((*lPtrListaDef).ptrDefunto == lPtrDefuntoDaRimuovere) 
			break;
		
		// aggiorno i puntatori
		lPtrListaDefPrecedente = lPtrListaDef;
		lPtrListaDefSuccessivo = &((*lPtrListaDef).successivo);
		lPtrListaDef = (*lPtrListaDef).successivo;
	}
	
	// non ho trovato l'elemento da rimuovere
	if (lPtrListaDef == NULL) 
		return;

	// rimuovo l'elemento
	if ((*lPtrListaDef).successivo != NULL)
	{
		(*(*lPtrListaDef).successivo).precedente = lPtrListaDefPrecedente;
	}
	(*lPtrListaDefSuccessivo) = (*lPtrListaDef).successivo;
	(*lPtrListaDef).precedente = NULL;
	(*lPtrListaDef).successivo = NULL;
	
	// libero la memoria dell'elemento trovato
	DisposePtr((Ptr) lPtrListaDef);
	lPtrListaDef = NULL;
} // RimuoviElementoListaDef

/* ------------------------------------------------------------------------ *

	EstraiDaListaDefConRigore

* ------------------------------------------------------------------------ */
static listadef* XTAPI EstraiDaListaDefConRigore(listadef **ptrlista) throw()
{
	// per scorrere la lista
	listadef *lPtrListaDef = NULL;
	// puntatore al campo successivo dell'elemento precedente
	listadef **lPtrListaDefSuccessivo = NULL;
	// puntatore all'elemento precedente
	listadef *lPtrListaDefPrecedente = NULL;
	
	// imposto il puntatore
	lPtrListaDef = (*ptrlista);
	lPtrListaDefSuccessivo = ptrlista;
	lPtrListaDefPrecedente = NULL;
	
	while (1) 
	{	
		if (lPtrListaDef == NULL) 
			break;
		
		if ((*(*lPtrListaDef).ptrDefunto).tipoRigore != kNessunRigore) 
			break;
		
		// aggiorno i puntatori
		lPtrListaDefSuccessivo = &(*lPtrListaDef).successivo;
		lPtrListaDefPrecedente = lPtrListaDef;
		lPtrListaDef = (*lPtrListaDef).successivo;
	}
	
	if (lPtrListaDef == NULL) 
		return(NULL);
	
	// estraggo l'elemento
	(*lPtrListaDefSuccessivo) = (*lPtrListaDef).successivo;
	if ((*lPtrListaDef).successivo != NULL)
		(*(*lPtrListaDef).successivo).precedente = lPtrListaDefPrecedente;
	
	(*lPtrListaDef).successivo = NULL;
	(*lPtrListaDef).precedente = NULL;
	
	return(lPtrListaDef);
} // EstraiDaListaDefConRigore

/* ------------------------------------------------------------------------ *

	EstraiDaListaDef

* ------------------------------------------------------------------------ */
static listadef* XTAPI EstraiDaListaDef(listadef **ptrlista) throw()
{
	// elemento da restituire
	listadef *lPtrListaDef = NULL;
	
	lPtrListaDef = (*ptrlista);
	
	if (lPtrListaDef == NULL) return(NULL);
	
	(*ptrlista) = (*lPtrListaDef).successivo;
	if ((*ptrlista) != NULL) 
		(**ptrlista).precedente = NULL;
	
	(*lPtrListaDef).successivo = NULL;
	(*lPtrListaDef).precedente = NULL;
	
	return(lPtrListaDef);
} // EstraiDaListaDef

/* ------------------------------------------------------------------------ *

	DuplicaElementoListaDef

* ------------------------------------------------------------------------ */
static errorixtension XTAPI DuplicaElementoListaDef(listadef *ptrelementodaduplicare, listadef **ptrelementoduplicato) throw()
{
	// per duplicare l'elemento
	listadef *lPtrElemento = NULL;
	
	if (ptrelementodaduplicare == NULL) 
	{
		(*ptrelementoduplicato) = NULL;
		return(kNessunErrore);
	}
	
	lPtrElemento = (listadef *) NewPtrClear(sizeof(listadef));
	if (lPtrElemento == NULL) 
	{
		// errore di memoria
		return(kErroreMemoria);
	}
	
	// copio il contenuto dell'elemento
	(*lPtrElemento) = (*ptrelementodaduplicare);
	
	// ritorno il puntatore dell'elemento creato
	(*ptrelementoduplicato) = lPtrElemento;
	
	return(kNessunErrore);
} // DuplicaElementoListaDef

/* ------------------------------------------------------------------------ *

	InserisciElementoInListaDef

* ------------------------------------------------------------------------ */
static void InserisciElementoInListaDef(listadef **ptrelemento,	listadef **ptrlistadefdefunti) throw()
{
	// per scorrere la lista
	listadef *lPtrListaDef = NULL;
	
	// imposto il puntatore
	lPtrListaDef = (*ptrlistadefdefunti);
	
	// controllo che la lista non sia vuota
	if (lPtrListaDef == NULL) 
	{
		(*ptrlistadefdefunti) = (*ptrelemento);
		(*ptrelemento) = NULL;
		return;
	}
	
	// scorro la lista
	while (1) 
	{
		// controllo se sono arrivato all'ultimo elemento della lista 
		if ((*lPtrListaDef).successivo == NULL) 
			break;
		
		lPtrListaDef = (*lPtrListaDef).successivo;
	}
	
	// inserisco l'elemento
	(*lPtrListaDef).successivo = (*ptrelemento);
	(**ptrelemento).precedente = lPtrListaDef;
	(**ptrelemento).successivo = NULL;
	
	(*ptrelemento) = NULL;
} // InserisciElementoInListaDef

/* ------------------------------------------------------------------------ *

	ContaElementiListaDef

* ------------------------------------------------------------------------ */
static int32 XTAPI ContaElementiListaDef(listadef *ptrlistadefdefunti) throw()
{
	listadef *lPtrListaDef = NULL;
	/* per scorrere la lista */
	long lNumeroElementi = 0;
	/* contatore degli elementi */
	
	/* imposto il puntatore */
	lPtrListaDef = ptrlistadefdefunti;
	
	while (1) {
	
		if (lPtrListaDef == NULL) break;
		
		/* avanzo il puntatore */
		lPtrListaDef = (*lPtrListaDef).successivo;
		
		/*incremento il contatore */
		lNumeroElementi++;
	}
	
	return(lNumeroElementi);
} // ContaElementiListaDef

/* ------------------------------------------------------------------------ *

	OrdinaAltezzeDefunti

* ------------------------------------------------------------------------ */
static int16 XTAPI OrdinaAltezzeDefunti(const void  *aa, const void  *bb) throw()
{
	DoubleFixed lAltezzaADoubleFixed = (DoubleFixed) gSpazioRimanente;
	DoubleFixed lAltezzaBDoubleFixed = (DoubleFixed) gSpazioRimanente;
	altezza *a, *b;

	a = (altezza *)aa;
	b = (altezza *)bb;

	lAltezzaADoubleFixed -= a->altezzaDoubleFixed;

	if (gXtPreferences.disposizionePref.tipoDisposizione == DisposizioneWap::VERTICALERADIOBTNID)
	{
		// impaginazione tipo Bg
		if (lAltezzaADoubleFixed < 0) 
			lAltezzaADoubleFixed *= -1;

		lAltezzaBDoubleFixed -= b->altezzaDoubleFixed;
		if (lAltezzaBDoubleFixed < 0) 
			lAltezzaBDoubleFixed *= -1;

		if (lAltezzaADoubleFixed < lAltezzaBDoubleFixed) 
			return(-1);
		else if (lAltezzaADoubleFixed > lAltezzaBDoubleFixed) 
			return(1);		
	}
	else
	{
		// impaginazione tipo Co
		
		if (lAltezzaADoubleFixed < 0) 
			lAltezzaADoubleFixed *= 1;
		
		if (lAltezzaBDoubleFixed < 0) 
			lAltezzaBDoubleFixed *= 1;
		
		if (lAltezzaADoubleFixed < lAltezzaBDoubleFixed) 
			return(1);
		else if (lAltezzaADoubleFixed > lAltezzaBDoubleFixed) 
			return(-1);
	}
	
	return(0);
} // OrdinaAltezzeDefunti

/* ------------------------------------------------------------------------ *

	RiordinaListaDef

* ------------------------------------------------------------------------ */
static void XTAPI RiordinaListaDef(spaziocolonna *ptrspaziocolonna, Fixed scarto, listadef **ptrlistadefdefunti) throw()
{
	// numero degli elemnti da riordinare
	int32 lNumeroElementi = 0;
	// array da riordinare 
	altezza *lPtrAltezze = NULL;
	// per prendere un elemento dell'array dell'altezze
	altezza *lPtrAltezza = NULL;
	// indice dell'altezze
	int16 lIndiceAltezze = 0;
	// per scorrere la lista
	listadef *lPtrListaDef = NULL;
	// per ricostruire la lista
	listadef **lPtrListaDefSuccessivo = NULL;
	// per ricostruire la lista
	listadef *lPtrListaDefPrecedente = NULL;
	// per prendere lo spazio recuperabile sulla colonna
	Fixed lSpazioRecuperabile = 0;
	// per prendere il numero dei necrologi del defunto 
	int16 lNumeroNecrologi = 0;
	
	if ((*ptrlistadefdefunti) == NULL) 
		return;
	
	// conto gli elementi della lista
	lNumeroElementi = ContaElementiListaDef(*ptrlistadefdefunti);
	
	// creo l'array da riordinare
	lPtrAltezze = (altezza *) NewPtrClear(lNumeroElementi * sizeof(altezza));
	if (lPtrAltezze == NULL) 
	{
		MostraErrore(kErroreMemoria);
		return;
	}
	
	// imposto il puntatore
	lPtrListaDef = *ptrlistadefdefunti;
	lIndiceAltezze = 0;
	
	// inserisco le altezze nell'array
	while (1) 
	{
		if (lPtrListaDef == NULL) 
			break;
		
		// imposto il puntatore all'altezza
		lPtrAltezza = lPtrAltezze + lIndiceAltezze;
		
		// copio i valori
		(*lPtrAltezza).puntatore = (Ptr) lPtrListaDef;
		if ((*(*lPtrListaDef).ptrDefunto).tipoRigore == kNessunRigore) 
		{
			(*lPtrAltezza).altezzaDoubleFixed = (*(*lPtrListaDef).ptrDefunto).altezzaTotaleDoubleFixed;
		} 
		else 
		{
			(*lPtrAltezza).altezzaDoubleFixed = kMaxDoubleFixed;
		}
		
		// aggiungo lo spazio tra i necrologi
		lNumeroNecrologi = PrendiNumeroNecrologiDefunto((*lPtrListaDef).ptrDefunto);
		if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE)
			(*lPtrAltezza).altezzaDoubleFixed += (DoubleFixed) (lNumeroNecrologi * kFixedMinimoSpazioTraNecrologi);
		else 
			(*lPtrAltezza).altezzaDoubleFixed += (DoubleFixed) (lNumeroNecrologi * gXtPreferences.giustificazionePref.valoreIniziale);

		// avanzo il puntatore
		lPtrListaDef = (*lPtrListaDef).successivo;
		
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
		else gSpazioRimanente += scarto;
	} 
	else if (scarto < 0) 
	{
		// nelle colonne precedenti ho inserito piu di quello che dovevo inserire
		lSpazioRecuperabile = -PrendiAllargamento(ptrspaziocolonna);
		if (scarto > lSpazioRecuperabile)
			gSpazioRimanente += scarto;
		else 
			gSpazioRimanente += lSpazioRecuperabile;
	}
	
	// riordino l'array dei necrologi
	qsort(lPtrAltezze, (size_t) lNumeroElementi, (size_t) sizeof(altezza), (int (*)(const void*, const void*)) OrdinaAltezzeDefunti);
	      
	// imposto i puntatori
	lPtrListaDefSuccessivo = ptrlistadefdefunti;
	lPtrListaDefPrecedente = NULL;

	// ricostruisco la lista dei necrologi
	for (lIndiceAltezze = 0; lIndiceAltezze < lNumeroElementi; lIndiceAltezze++) 
	{
		// imposto il puntatore all'altezza
		lPtrAltezza = lPtrAltezze + lIndiceAltezze;

		// imposto il puntatore del giro precedente
		lPtrListaDef = (listadef *) (*lPtrAltezza).puntatore;
		(*lPtrListaDefSuccessivo) = lPtrListaDef;
		
		// imposto l'elemento corrente
		(*lPtrListaDef).precedente = lPtrListaDefPrecedente;
		(*lPtrListaDef).successivo = NULL;
		
		// imposto i puntatori per il giro successivo
		lPtrListaDefSuccessivo = &((*lPtrListaDef).successivo);
		lPtrListaDefPrecedente = lPtrListaDef;
	}
	
	// libero la memoria dell'array delle altezze
	if (lPtrAltezze != NULL) 
	{
		DisposePtr((Ptr) lPtrAltezze);
		lPtrAltezze = NULL;
	}
} // RiordinaListaDef

/* ------------------------------------------------------------------------ *

	EseguiNormaleDefunti

* ------------------------------------------------------------------------ */
static tipochiusura XTAPI EseguiNormaleDefunti(spaziocolonna *ptrspaziocolonna, listadef **ptrlistadefdefunti, Fixed scarto) throw()
{
	// per prendere il tipo di chiusura della colonna
	tipochiusura lTipoChiusura = kNessunaChiusura;
	// per prendere lo spazio irrecuperabile
	Fixed lDifferenza = 0;
	// per calcolare lo spazio rimanente
	Fixed lSpazioRimanente = 0;
	// puntatore al defunto corrente 
	listadef *lPtrElementoCorrente = NULL;
	// puntatore al defunto scambiabile
	listadef *lPtrElementoScambiabile = NULL;
	// chiusura della colonna finale 
	tipochiusura lChiusuraFinale = kNessunaChiusura;
	// puntatore alla lista degli scartati 
	listadef *lPtrListaDefScartati = NULL;
	// puntatore alla lista degli scartati due volte
	listadef *lPtrListaDefDoppiScartati = NULL;
	// puntatore alla lista degli scambiabili scartati 
	listadef *lPtrListaDefScambiabiliScartati = NULL;

	// controllo il rispetto delle preferenze
	gErroreXtension = ControllaChiusuraColonna(ptrspaziocolonna,
											   &lChiusuraFinale,
											   &lDifferenza);
	if (gErroreXtension != kNessunErrore) 
	{
		MostraErrore(gErroreXtension);
		return(lTipoChiusura); // <- da verificare 
	}
											   
	while (1) 
	{
		// ordina la lista dei necrologi da considerare
		RiordinaListaDef(ptrspaziocolonna, scarto, ptrlistadefdefunti);
	
		// estraggo la prima mossa 
		lPtrElementoCorrente = EstraiDaListaDef(ptrlistadefdefunti);
		

		if (lPtrElementoCorrente == NULL)  
		{
			if (gPtrListaDefTappaBuchi == NULL || lTipoChiusura == kPreferenzeRispettate) 
				break;

			// uso la lista dei tappabuchi
			// ordina la lista dei defunti da tapabuchi
			RiordinaListaDef(ptrspaziocolonna, scarto, &gPtrListaDefTappaBuchi);

			// estraggo il primo tappabuchi
			lPtrElementoCorrente = EstraiDaListaDef(&gPtrListaDefTappaBuchi);
			
			// tolgo il defunto trovato dalla colonna in cui era inserito
			RimuoviDefunto((*lPtrElementoCorrente).ptrSpazioColonna,
						   (*lPtrElementoCorrente).ptrDefunto); 
			
			// annullo la colonna di appartenenza
			(*lPtrElementoCorrente).ptrSpazioColonna = NULL;

			// rimuovo l'elemento trovato dalla lista degli scambiabili
			RimuoviElementoListaDef(lPtrElementoCorrente, &gPtrListaDefScambiabili);
		}

		// duplico l'elmento corrente 
		gErroreXtension = DuplicaElementoListaDef(lPtrElementoCorrente, &lPtrElementoScambiabile);
		if (gErroreXtension != kNessunErrore) 
		{
			MostraErrore(gErroreXtension);
			return(lTipoChiusura);
		}
			
		// inserisco l'elemento nella colonna
		if (gXtPreferences.disposizionePref.tipoDisposizione == DisposizioneWap::VERTICALERADIOBTNID)
		{
			// impaginazione tipo Bg
			InserisciDefuntoInCoda(ptrspaziocolonna, (*lPtrElementoCorrente).ptrDefunto);
		}
		else
		{
			// impaginazione tipo Co
			InserisciDefuntoInOrdine(ptrspaziocolonna, (*lPtrElementoCorrente).ptrDefunto);
		}
		// controllo il rispetto delle preferenze
		gErroreXtension = ControllaChiusuraColonna(ptrspaziocolonna,
												   &lTipoChiusura,
												   &lDifferenza);
		if (gErroreXtension != kNessunErrore) 
		{
			MostraErrore(gErroreXtension);
			return(lTipoChiusura);
		}
		
		if (lTipoChiusura == kPreferenzeNonRispettate) 
		{
			// libero la memoria del'elemento scambiabile
			if (lPtrElementoScambiabile != NULL) 
			{
				DisposePtr((Ptr) lPtrElementoScambiabile);
				lPtrElementoScambiabile = NULL;
			}
		
			// tolgo il necrologio dalla colonna
			RimuoviDefunto(ptrspaziocolonna, (*lPtrElementoCorrente).ptrDefunto); 
			
			// controllo se posso finire il ciclo
			if (lChiusuraFinale == kPreferenzeRispettate) 
			{
				// preferenze rispettate
				// rinserisco l'elemento nella lista dei defunti
				InserisciElementoInListaDef(&lPtrElementoCorrente, ptrlistadefdefunti);
				
				break;
			}
			
			// inserisco l'elemento nella lista dei defunti scartati
			InserisciElementoInListaDef(&lPtrElementoCorrente, &lPtrListaDefScartati);
				
			if ((*ptrlistadefdefunti) != NULL) 
			{
				// ci sono ancora dei defunti da provare
							
			} 
			else if (gPtrListaDefTappaBuchi != NULL) 
			{
				// uso la lista dei tappabuchi 
			
				// ordina la lista dei defunti da tappabuchi
				RiordinaListaDef(ptrspaziocolonna, scarto, &gPtrListaDefTappaBuchi);
	
				// estraggo il primo tappabuchi
				lPtrElementoCorrente = EstraiDaListaDef(&gPtrListaDefTappaBuchi);
				
				// tolgo il defunto trovato dalla colonna in cui era inserito
				RimuoviDefunto((*lPtrElementoCorrente).ptrSpazioColonna,
							   (*lPtrElementoCorrente).ptrDefunto); 
				
				// rimuovo l'elemento dalla lista degli scambiabili
				RimuoviElementoListaDef(lPtrElementoCorrente, &gPtrListaDefScambiabili);

				// rinserisco l'elemento nella lista dei necrologi
				(*lPtrElementoCorrente).ptrSpazioColonna = NULL;
				InserisciElementoInListaDef(&lPtrElementoCorrente, ptrlistadefdefunti);
			
			} 
			else if (gPtrListaDefScambiabili != NULL) 
			{
				// uso la lista degli scambiabili
				while (1) 
				{
					// ordina la lista dei defunti scambiabili
					RiordinaListaDef(ptrspaziocolonna, scarto, &gPtrListaDefScambiabili);
					
					// estraggo il primo scambiabile
					lPtrElementoScambiabile = EstraiDaListaDef(&gPtrListaDefScambiabili);
					
					if (lPtrElementoScambiabile == NULL) 
						break;
					
					// controllo che l'elemento scelto non sia gi… nella colonna corrente
					if ((*lPtrElementoScambiabile).ptrSpazioColonna != ptrspaziocolonna) 
					{	
						// tolgo il defunto scambiabile dalla sua colonna
						RimuoviDefunto((*lPtrElementoScambiabile).ptrSpazioColonna,
									   (*lPtrElementoScambiabile).ptrDefunto);
					
						// provo ad inserire il primo scambiabile nella colonna
						InserisciDefuntoInOrdine(ptrspaziocolonna, (*lPtrElementoScambiabile).ptrDefunto);
					
						// controllo il rispetto delle preferenze
						gErroreXtension = ControllaChiusuraColonna(ptrspaziocolonna,
																   &lTipoChiusura,
																   &lDifferenza);
						if (gErroreXtension != kNessunErrore) 
						{
							MostraErrore(gErroreXtension);
							return(lTipoChiusura);
						}
						
						if (lTipoChiusura == kPreferenzeRispettate) 
						{
							// provo ad eseguire la sostituzione
							
							// scorro tutti gli elementi scartati in precedenza
							while (1) 
							{
								// ordina la lista dei defunti scartati
								RiordinaListaDef((*lPtrElementoScambiabile).ptrSpazioColonna,
												 scarto,
												 &lPtrListaDefScartati);

								// estraggo il primo scartato
								lPtrElementoCorrente = EstraiDaListaDef(&lPtrListaDefScartati);
								
								if (lPtrElementoCorrente == NULL) 
									break;
								
								// provo ad inserire lo scartato
								InserisciDefuntoInOrdine((*lPtrElementoScambiabile).ptrSpazioColonna,
													   (*lPtrElementoCorrente).ptrDefunto);
							
								// controllo il rispetto delle preferenze
								gErroreXtension = ControllaChiusuraColonna((*lPtrElementoScambiabile).ptrSpazioColonna,
																		   &lTipoChiusura,
																		   &lDifferenza);
								if (gErroreXtension != kNessunErrore) 
								{
									MostraErrore(gErroreXtension);
									return(lTipoChiusura);
								}
								
								if (lTipoChiusura == kPreferenzeRispettate) 
								{
									// sono riscito a fare lo scambio
									lChiusuraFinale = kPreferenzeRispettate;
									
									// salvo la colonna in cui e' stato inserito l'elemento corrente
									(*lPtrElementoCorrente).ptrSpazioColonna = (*lPtrElementoScambiabile).ptrSpazioColonna;
									
									// salvo la colonna in cui e' stato inserto l'elemento scambiabile
									(*lPtrElementoScambiabile).ptrSpazioColonna = ptrspaziocolonna;
									
									break;
								}
								
								// rimuovo dalla colonna il defunto appena inserito
								RimuoviDefunto((*lPtrElementoScambiabile).ptrSpazioColonna,
											   (*lPtrElementoCorrente).ptrDefunto);
											   
								// inserisco l'elemento corrente nella lista dei doppi scartati
								InserisciElementoInListaDef(&lPtrElementoCorrente,
															&lPtrListaDefDoppiScartati);
							}
							
							// inserisco gli elementi doppi scartati nella lista degli scartati
							while (1) 
							{
								lPtrElementoCorrente = EstraiDaListaDef(&lPtrListaDefDoppiScartati);
								
								if (lPtrElementoCorrente == NULL) 
									break;
								
								InserisciElementoInListaDef(&lPtrElementoCorrente, &lPtrListaDefScartati);
							}
				
						}
						
						if (lChiusuraFinale == kPreferenzeRispettate) 
							break;
						
						// rimuovo dalla colonna il defunto appena inserito
						RimuoviDefunto(ptrspaziocolonna, (*lPtrElementoScambiabile).ptrDefunto);
						
						// rinserisco il defunto nella sua colonna originaria
						InserisciDefuntoInOrdine((*lPtrElementoScambiabile).ptrSpazioColonna,
												(*lPtrElementoScambiabile).ptrDefunto);
						
					}
					
					if (lChiusuraFinale == kPreferenzeRispettate) 
						break;
						
					// inserisco l'elemento corrente nella lista degli scambiabili scartati
					InserisciElementoInListaDef(&lPtrElementoScambiabile,
												&lPtrListaDefScambiabiliScartati);
				}
				
				// inserisco gli elementi scartati nella lista degli scambiabili
				while (1) 
				{
					lPtrElementoCorrente = EstraiDaListaDef(&lPtrListaDefScambiabiliScartati);
					
					if (lPtrElementoCorrente == NULL) 
						break;
					
					InserisciElementoInListaDef(&lPtrElementoCorrente, &gPtrListaDefScambiabili);
				}
				
				// esco dal ciclo sia se lo scambio eì  riuscito oppure no
				
				break;
				
				
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
			
			// inserisco l'elemento nella lista degli scambiabili
			(*lPtrElementoScambiabile).ptrSpazioColonna = ptrspaziocolonna;
			InserisciElementoInListaDef(&lPtrElementoScambiabile, &gPtrListaDefScambiabili);	

			// controllo se posso usare l'elemento corrente come tappabuchi
			if (lChiusuraFinale == kPreferenzeRispettate) 
			{
				// l'elemento corrente pu• essere usato come tappabuchi
				(*lPtrElementoCorrente).ptrSpazioColonna = ptrspaziocolonna;
				InserisciElementoInListaDef(&lPtrElementoCorrente, &gPtrListaDefTappaBuchi);	
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
			
			if (lSpazioRimanente < 0) break;
			
		} 
		else 
		{
			// devo inserire ancora
			lChiusuraFinale = kInserireAncora;

			// inserisco l'elmento nella lista degli scambiabili
			(*lPtrElementoScambiabile).ptrSpazioColonna = ptrspaziocolonna;
			InserisciElementoInListaDef(&lPtrElementoScambiabile, &gPtrListaDefScambiabili);	

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
				
	// prendo gli elementi della lista degli scartati e li metto nella lista dei defunti
	if (lPtrListaDefScartati != NULL) 
	{
		while (1) 
		{	
			lPtrElementoCorrente = EstraiDaListaDef(&lPtrListaDefScartati);
	
			if (lPtrElementoCorrente == NULL) 
				break;
			
			InserisciElementoInListaDef(&lPtrElementoCorrente, ptrlistadefdefunti);
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
			return(lTipoChiusura);
		}
		
		return(lTipoChiusura);
		
	} 
	else 
	{
		return(lChiusuraFinale);
	}
} // EseguiNormaleDefunti

/* ------------------------------------------------------------------------ *

	DefuntoFondoRigoreColonna

* ------------------------------------------------------------------------ */
static void XTAPI DefuntoFondoRigoreColonna(spaziocolonna *ptrspaziocolonna, listadef **ptrlistadefdefunti) throw()
{
	// puntatore al necrologio corrente
	listadef *lPtrElementoCorrente = NULL;
	
	// estraggo l'elemento con rigore di colonna
	lPtrElementoCorrente = EstraiDaListaDefConRigore(ptrlistadefdefunti);
	
	if (lPtrElementoCorrente == NULL) 
		return;
	
	// inserisco l'elemento nella colonna in fondo in testa
	InserisciDefuntoInTestaFondo(ptrspaziocolonna, (*lPtrElementoCorrente).ptrDefunto);
			
	// libero la memoria del'elemento corrente
	if (lPtrElementoCorrente != NULL) 
	{
		DisposePtr((Ptr) lPtrElementoCorrente);
		lPtrElementoCorrente = NULL;
	}
} // DefuntoFondoRigoreColonna

/* ------------------------------------------------------------------------ *

	EseguiNormaleDefuntiFondo

* ------------------------------------------------------------------------ */
static tipochiusura XTAPI EseguiNormaleDefuntiFondo(spaziocolonna *ptrspaziocolonna, listadef **ptrlistadefdefunti, Fixed scarto) throw()
{
	// per prendere il tipo di chiusura della colonna
	tipochiusura lTipoChiusura = kNessunaChiusura;
	// per prendere lo spazio irrecuperabile
	Fixed lDifferenza = 0;
	// per calcolare lo spazio rimanente
	Fixed lSpazioRimanente = 0;
	// puntatore al necrologio corrente
	listadef *lPtrElementoCorrente = NULL;
	// chiusura derlla colonna finale
	tipochiusura lChiusuraFinale = kNessunaChiusura;
	// puntatore alla lista degli scartati
	listadef *lPtrListaDefScartati = NULL;
	
	// controllo il rispetto delle preferenze
	gErroreXtension = ControllaChiusuraColonna(ptrspaziocolonna,
											   &lTipoChiusura,
											   &lDifferenza);
											   
	if (gErroreXtension != kNessunErrore) 
	{
		MostraErrore(gErroreXtension);
		return(lTipoChiusura);
	}
											   
	while (1) 
	{
		// ordina la lista dei necrologi da considerare
		RiordinaListaDef(ptrspaziocolonna, scarto, ptrlistadefdefunti);
	
		// estraggo la prima mossa
		lPtrElementoCorrente = EstraiDaListaDef(ptrlistadefdefunti);
			
		if (lPtrElementoCorrente == NULL)  
		{
		
			if (gPtrListaDefTappaBuchi == NULL || lTipoChiusura == kPreferenzeRispettate) 
				break;
			
			// uso la lista dei tappabuchi
		
			// ordina la lista dei defunti da tapabuchi
			RiordinaListaDef(ptrspaziocolonna, scarto, &gPtrListaDefTappaBuchi);

			// estraggo il primo tappabuchi
			lPtrElementoCorrente = EstraiDaListaDef(&gPtrListaDefTappaBuchi);
			
			// tolgo il defunto trovato dalla colonna in cui era inserito
			RimuoviDefunto((*lPtrElementoCorrente).ptrSpazioColonna,
						   (*lPtrElementoCorrente).ptrDefunto); 
			
			// rinserisco l'elemento nella lista dei necrologi
			(*lPtrElementoCorrente).ptrSpazioColonna = NULL;
			
		}

		// inserisco l'elemento nella colonna
		if (gXtPreferences.disposizionePref.tipoDisposizione == DisposizioneWap::VERTICALERADIOBTNID)
		{
			// impaginazione tipo Bg
			InserisciDefuntoInCodaFondo(ptrspaziocolonna, (*lPtrElementoCorrente).ptrDefunto);
		}
		else
		{
			// impaginazione tipo Co
			InserisciDefuntoInTestaFondo(ptrspaziocolonna, (*lPtrElementoCorrente).ptrDefunto);	
		}
		// controllo il rispetto delle preferenze
		gErroreXtension = ControllaChiusuraColonna(ptrspaziocolonna,
												   &lTipoChiusura,
												   &lDifferenza);
		if (gErroreXtension != kNessunErrore) 
		{
			MostraErrore(gErroreXtension);
			return(lTipoChiusura);
		}
		
		if (lTipoChiusura == kPreferenzeNonRispettate) 
		{
			// tolgo il necrologio dalla colonna
			RimuoviDefunto(ptrspaziocolonna, (*lPtrElementoCorrente).ptrDefunto); 
			
			// controllo se posso finire il ciclo
			if (lChiusuraFinale == kPreferenzeRispettate) 
			{
				// preferenze rispettate
			
				// rinserisco l'elemento nella lista dei defunti
				InserisciElementoInListaDef(&lPtrElementoCorrente, ptrlistadefdefunti);
				
				break;
				
			}
			
			// inserisco l'elemento nella lista dei defunti scartati
			InserisciElementoInListaDef(&lPtrElementoCorrente, &lPtrListaDefScartati);
				
			if ((*ptrlistadefdefunti) != NULL) 
			{
				// ci sono ancora dei defunti da provare
							
			} 
			else if (gPtrListaDefTappaBuchi != NULL) 
			{
				// uso la lista dei tappabuchi
			
				// ordina la lista dei defunti da tapabuchi
				RiordinaListaDef(ptrspaziocolonna, scarto, &gPtrListaDefTappaBuchi);
	
				// estraggo il primo tappabuchi
				lPtrElementoCorrente = EstraiDaListaDef(&gPtrListaDefTappaBuchi);
				
				// tolgo il defunto trovato dalla colonna in cui era inserito
				RimuoviDefunto((*lPtrElementoCorrente).ptrSpazioColonna,
							   (*lPtrElementoCorrente).ptrDefunto); 
				
				// rinserisco l'elemento nella lista dei necrologi
				(*lPtrElementoCorrente).ptrSpazioColonna = NULL;
				InserisciElementoInListaDef(&lPtrElementoCorrente, ptrlistadefdefunti);
			
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
				InserisciElementoInListaDef(&lPtrElementoCorrente, &gPtrListaDefTappaBuchi);	
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
			DisposePtr((Ptr)lPtrElementoCorrente);
				lPtrElementoCorrente = NULL;
		}
	}
	
	// libero la memoria del'elemento corrente
	if (lPtrElementoCorrente != NULL) 
	{
		DisposePtr((Ptr) lPtrElementoCorrente);
		lPtrElementoCorrente = NULL;
	}
				
	// prendo gli elementi della lista degli scartati e li metto nella lista dei defunti
	if (lPtrListaDefScartati != NULL) 
	{
		lPtrElementoCorrente = EstraiDaListaDef(&lPtrListaDefScartati);
	
		while (1) 
		{
			
			if (lPtrElementoCorrente == NULL) 
				break;
			
			InserisciElementoInListaDef(&lPtrElementoCorrente, ptrlistadefdefunti);
			
			lPtrElementoCorrente = EstraiDaListaDef(&lPtrListaDefScartati);
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
			return(lTipoChiusura);
		}
		
		return(lTipoChiusura);

	} 
	else 
	{
		return(lChiusuraFinale);
	}
} // EseguiNormaleDefuntiFondo

/* ------------------------------------------------------------------------ *

	NormaleDefunti

* ------------------------------------------------------------------------ */
static errorixtension XTAPI NormaleDefunti(defunto *ptrdefunto, int16 pagina, int16 colonna, Fixed *ptrscarto) throw()
{
	// lista per trovare l'ordinamento dei defunti migliore
	listadef *lPtrListaDefDefunti = NULL;
	// per scorrere le pagine
	int16 lPagina = 0;
	// per scorrere le colonne
	int16 lColonna = 0;
	// per prendere lo spazio colonna
	spaziocolonna *lPtrSpazioColonna = NULL;
	// per scorrere lo spazio colonna da usare per il calcolo del posizionamento
	int16 lIndiceSpaziColonne = 0;
	// per sapere come e' stata chiusu una colonna
	tipochiusura lTipoChiusura = kNessunaChiusura;
	// per posizionare i defunti avanzati
	listadef *lPtrElementoCorrente = NULL;
	// per prendere il numero dei necrologi da inserire fuori dalla pagina
	int16 lNumeroNecrologi = 0;
	
	// controllo se la lista e' nulla
	if (ptrdefunto == NULL) 
		return(kNessunErrore);

	// costruisci la listadef dei defunti da inserire
	CostruisciListaDef(&lPtrListaDefDefunti, ptrdefunto);

	// imposto i valori di pagina e colonna 
	lPagina = pagina;
	lColonna = colonna;

	// per tutte le colonne eseguo l'inserimento sui defunti
	while (1) 
	{
		// prendo le info della colonna
		lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);

		// riempo la colonna corrente
		lTipoChiusura = EseguiNormaleDefunti(lPtrSpazioColonna,
											 &lPtrListaDefDefunti,
											 (*ptrscarto));

		if (lPtrListaDefDefunti != NULL) 
		{
			// calcolo lo scarto della colonna
			if (lTipoChiusura == kPreferenzeRispettate) 
			{
				(*ptrscarto) += (*lPtrSpazioColonna).spazioOttimale -
								  (*lPtrSpazioColonna).spazioOccupato -
									(*lPtrSpazioColonna).spazioTraNecrologi;
			}

			// chiudo quasi la colonna corrente
			(*lPtrSpazioColonna).chiusuraColonna = kQuasiChiusa;

			// salvo la chiusura della colonna
			(*lPtrSpazioColonna).tipoChiusura = lTipoChiusura;

		} 
		else 
		{

			// non ci son piu' defunti da posizionare
			break;

		}

		// devo incrementare la colonna
		gErroreXtension = CercaPrimaColonnaDaChiudere(&lPagina, &lColonna);
		if (gErroreXtension != kNessunErrore) 
		{
			// ho terminato le colonne da chiudere
			break;
		}
	}

	// libero la lista dei tapabuchi
	LiberaListaDef(&gPtrListaDefTappaBuchi);

	// libero la lista degli scambiabili
	LiberaListaDef(&gPtrListaDefScambiabili);

	// provo a scorrere tutte le colonne per vedere se riesco ad inserire ancora qualcosa
	if (lPtrListaDefDefunti != NULL) 
	{
		// scorro le pagine per vedere se posso ancora inserire
		for (lPagina = 1; lPagina <= PrendiNumeroPagine(); lPagina++) 
		{
			for (lColonna = 1; lColonna <= PrendiNumeroColonne(); lColonna++) 
			{
				// prendo le info della colonna corrente
				lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);

				if ((*lPtrSpazioColonna).spazioDisponibile > 0 &&
					(*lPtrSpazioColonna).chiusuraColonna == kQuasiChiusa) 
				{
					// riempo la colonna corrente
					lTipoChiusura = EseguiNormaleDefunti(lPtrSpazioColonna,
														 &lPtrListaDefDefunti,
														 (*ptrscarto));

					// chiudo quasi la colonna corrente
					(*lPtrSpazioColonna).chiusuraColonna = kChiusa;

					// salvo la chiusura della colonna
					(*lPtrSpazioColonna).tipoChiusura = lTipoChiusura;

				}
			}
		}

		// libero la lista dei tapabuchi
		LiberaListaDef(&gPtrListaDefTappaBuchi);

		// libero la lista degli scambiabili
		LiberaListaDef(&gPtrListaDefScambiabili);
	}

	// controllo se sono stati inseriti tutti i defunti 
	if (lPtrListaDefDefunti != NULL) 
	{
		// posiziono i defunti avanzati fuori pagina
		while (1) 
		{
			lPtrElementoCorrente = EstraiDaListaDef(&lPtrListaDefDefunti);
			if (lPtrElementoCorrente == NULL) 
				break;

			// prendo le info della colonna per i box fuori
			lPtrSpazioColonna = PrendiSpazioColonnaFuori();

			// modifico lo scarto 
			(*ptrscarto) -= (Fixed) (*(*lPtrElementoCorrente).ptrDefunto).altezzaTotaleDoubleFixed;
			assert(0 < (Fixed) (*(*lPtrElementoCorrente).ptrDefunto).altezzaTotaleDoubleFixed);

			lNumeroNecrologi = PrendiNumeroNecrologiDefunto((*lPtrElementoCorrente).ptrDefunto);
			if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE) {
				(*ptrscarto) -= (kFixedMinimoSpazioTraNecrologi * lNumeroNecrologi);
			} 
			else 
			{
				(*ptrscarto) -= (gXtPreferences.giustificazionePref.valoreIniziale * lNumeroNecrologi);
			}
						
			// inserisco il defunto in coda alla colonna corrente
			InserisciDefuntoInCoda(lPtrSpazioColonna, (*lPtrElementoCorrente).ptrDefunto);
									  
			// libero la memoria del'elemento corrente
			if (lPtrElementoCorrente != NULL) 
			{
				DisposePtr((Ptr) lPtrElementoCorrente);
				lPtrElementoCorrente = NULL;
			}
		}
	}

	return(kNessunErrore);
} // NormaleDefunti

/* ------------------------------------------------------------------------ *

	NormaleDefuntiFondo

* ------------------------------------------------------------------------ */
static errorixtension XTAPI NormaleDefuntiFondo(defunto *ptrdefunto, int16 pagina, int16 colonna, Fixed *ptrscarto) throw()
{
	// lista per trovare l'ordinamento dei defunti migliore
	listadef *lPtrListaDefDefunti = NULL;
	// per scorrere le pagine */
	int16 lPagina = 0;
	// per scorrere le colonne */
	int16 lColonna = 0;
	// per prendere lo spazio colonna */
	spaziocolonna *lPtrSpazioColonna = NULL;
	// per sapere come Š stata chiusu una colonna */
	tipochiusura lTipoChiusura = kNessunaChiusura;
	// per posizionare i defunti avanzati */
	listadef *lPtrElementoCorrente = NULL;

	// controllo se la lista e' nulla
	if (ptrdefunto == NULL) 
		return(kNessunErrore);

	// costruisci la listadef dei defunti da inserire
	CostruisciListaDef(&lPtrListaDefDefunti, ptrdefunto);

	// imposto i valori di pagina e colonna
	lPagina = pagina;
	lColonna = colonna;

	// per tutte le colonne eseguo l'inserimento sui defunti
	while (1) 
	{
		// prendo le info della colonna
		lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);

		// estraggo l'eventuale defunto in testa alla colonna
		DefuntoFondoRigoreColonna(lPtrSpazioColonna, &lPtrListaDefDefunti);

		// riempo la colonna corrente
		lTipoChiusura = EseguiNormaleDefuntiFondo(lPtrSpazioColonna,
													&lPtrListaDefDefunti,
													(*ptrscarto));

		if (lPtrListaDefDefunti != NULL && (*lPtrSpazioColonna).spazioOccupato > 0)
		{
			// calcolo il nuovo scarto
			(*ptrscarto) +=	(*lPtrSpazioColonna).spazioOttimale -
									(*lPtrSpazioColonna).spazioOccupato -
									(*lPtrSpazioColonna).spazioTraNecrologi;

			// chiudo la colonna corrente
			(*lPtrSpazioColonna).chiusuraColonna = kChiusa;

			// salvo la chiusura della colonna
			(*lPtrSpazioColonna).tipoChiusura = lTipoChiusura;

		} 
		else 
		{
			// non ci son piu' defunti da posizionare
			break;

		}

		// decrement of the current column
		gErroreXtension = ImpostaColonnaPrecedente(&lPagina, &lColonna);
		if (kNessunErrore != gErroreXtension) break;
	}

	if (NULL != lPtrListaDefDefunti)
	{
		// add the left mortuary add to the
		// list of the out of page elements

		// posiziono i defunti avanzati fuori pagina
		while (1)
		{
			listadef* lPtrElementoCorrente = NULL;
			spaziocolonna* lPtrSpazioColonna = NULL;
			int16 lNumeroNecrologi = 0;

			lPtrElementoCorrente = EstraiDaListaDef(&lPtrListaDefDefunti);

			if (lPtrElementoCorrente == NULL) 
				break;

			// prendo le info della colonna per i box fuori
			lPtrSpazioColonna = PrendiSpazioColonnaFuori();

			// modifico lo scarto
			(*ptrscarto) -= (Fixed) (*(*lPtrElementoCorrente).ptrDefunto).altezzaTotaleDoubleFixed;
			assert(0 < (Fixed) (*(*lPtrElementoCorrente).ptrDefunto).altezzaTotaleDoubleFixed);

			lNumeroNecrologi = PrendiNumeroNecrologiDefunto((*lPtrElementoCorrente).ptrDefunto);
			if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE) {
				(*ptrscarto) -= (kFixedMinimoSpazioTraNecrologi * lNumeroNecrologi);
			} 
			else 
			{
				(*ptrscarto) -= (gXtPreferences.giustificazionePref.valoreIniziale * lNumeroNecrologi);
			}
						
			//  inserisco il defunto in coda alla colonna corrente
			InserisciDefuntoInCoda(lPtrSpazioColonna, (*lPtrElementoCorrente).ptrDefunto);
									  
			// libero la memoria del'elemento corrente
			if (lPtrElementoCorrente != NULL) 
			{
				DisposePtr((Ptr) lPtrElementoCorrente);
				lPtrElementoCorrente = NULL;
			}
		}
	}

	// libero la lista dei tapabuchi
	LiberaListaDef(&gPtrListaDefTappaBuchi);

	return(kNessunErrore);
} // NormaleDefuntiFondo


/* ------------------------------------------------------------------------ *

	CalcolaMigliorOrdineDefunti

* ------------------------------------------------------------------------ */
errorixtension XTAPI CalcolaMigliorOrdineDefunti(defunto *ptrdefunto, int16 pagina, int16 colonna, Fixed *ptrscarto) throw()
{

	// controllo che la lista in arrivo non sia vuota 
	if (ptrdefunto == NULL) 
	{
		return(kNessunErrore);
	}
	
	gErroreXtension = NormaleDefunti(ptrdefunto, pagina, colonna, ptrscarto);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nel posizionare i defunti nella colonna
		return(gErroreXtension);
	}

	return(kNessunErrore);
} // CalcolaMigliorOrdineDefunti

/* ------------------------------------------------------------------------ *

	CalcolaMigliorOrdineDefuntiFondo

* ------------------------------------------------------------------------ */
errorixtension XTAPI CalcolaMigliorOrdineDefuntiFondo(defunto *ptrdefunto, int16 pagina,  int16 colonna, Fixed *ptrscarto) throw()
{	
	// controllo che la lista in arrivo non sia vuota
	if (ptrdefunto == NULL) 
	{
		return(kNessunErrore);
	}
	
	gErroreXtension = NormaleDefuntiFondo(ptrdefunto, pagina, colonna, ptrscarto);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nel posizionare i defunti in fondo alla colonna
		return(gErroreXtension);
	}
	
	return(kNessunErrore);
} //CalcolaMigliorOrdineDefuntiFondo
