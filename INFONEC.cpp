/* ------------------------------------------------------------------------ *

	InfoNecrologi.cpp

	11 Aprile 2005 				Fabrizio

	Copyright © Sinedita S.r.l 2005. All Rights Reserved.

	Descrizione:
	Contiene le procedure per la costruzione delle informazioni sui necrologi

	$Log: not supported by cvs2svn $
	Revision 1.1  2005/04/20 07:14:19  taretto
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
#include "CalcPos.h"
#include "DocInfo.h"
#include "Errori.h"
#include "Paletta.h"
#include "Pref.h"
#include "ReadFile.h"
#include "ResStr.h"
#include "SpaceCol.h"
#include "StMes.h"
#include "XTNecro.h"

#include "InfoNec.h"

// CONSTS
// CONSTS
// CONSTS

#define kCodiceA 'A'
#define kCodiceC 'C'

// GLOBALS
// GLOBALS
// GLOBALS

// contatore degli annunci da posizionare
int16 gNumeroAnnunci = 0;
// contatore degli anniversari da posizionare
int16 gNumeroAnniversari = 0;
// contatore dei ringraziamenti da posizionare
int16 gNumeroRingraziamenti = 0;
// array che contine tutti gli annunci da posizionare
defunto *gPtrAnnunci = NULL;
// array che contine tutti gli anniversari da posizionare
defunto *gPtrAnniversari = NULL;
// array che contine tutti i ringraziamenti da posizionare
defunto *gPtrRingraziamenti = NULL;

// STATICS
// STATICS
// STATICS

// indice per accedere alla struttura che contiene gli annunci
static int16 gIndiceAnnunci = -1;
// indice per accedere alla struttura che contiene gli anniversari
static int16 gIndiceAnniversari = -1;
// indice per accedere alla struttura che contiene gli ringraziamenti
static int16 gIndiceRingraziamenti = -1;

// per sapere se e' arrivato un nuovo defunto
static Boolean gNuovoDefunto = TRUE;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			CalcolaNumeroNecrologi
	@abstract 		gestione annunci necrologi.
	@discussion		Calcola il numero di necrologi da impaginare. Non dà i totali completi
					ma li conta in base ai defunti presenti. Un defunto con tanti annunci vale
					comunque uno. Controlla anche che un defunto abbia solo necrologi dello stesso
					tipo.

					12 Aprile 2005 - Fabrizio.

	@param 			ptrdocumento puntatore al documento da impaginare
	@result  			ritorna l'eventuale errore dell'xtension
*/
static errorixtension XTAPI CalcolaNumeroNecrologi(uchar* ptrdocumento) throw();

/*!
	@function			CostruisciInfoNecrologi
	@abstract 		gestione annunci necrologi.
	@discussion		Costruisce le prime strutture per ordinare i necrologi da posizionare

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			ritorna l'eventuale errore dell'xtension
*/
static errorixtension XTAPI CostruisciInfoNecrologi() throw();

/*!
	@function			InserisciNecrologioA
	@abstract 		gestione annunci necrologi.
	@discussion		Inserisce nelle strutture dati gi… create un necrologio di codice A

					12 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto puntatore alla struttura da utilizzare
	@param			indice indice da usare per fare l'inserimento
	@param			ptrappware puntatore alle info da inserire
	@result  			nessuno
*/
static void XTAPI InserisciNecrologioA(defunto* ptrdefunto, int16 indice, appware* ptrappware) throw();

/*!
	@function			InserisciNecrologioB
	@abstract 		gestione annunci necrologi.
	@discussion		Inserisce nelle strutture dati gia' create un necrologio di codice B,
					cioe' quei necrologi divisi in sottogruppi

					12 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto puntatore alla struttura da utilizzare
	@param			indice indice da usare per fare l'inserimento
	@param			ptrappware puntatore alle info da inserire
	@result  			nessuno
*/
static void XTAPI InserisciNecrologioB(defunto *ptrdefunto, int16 indice, appware *ptrappware) throw();

/*!
	@function			InserisciNecrologioC
	@abstract 		gestione annunci necrologi.
	@discussion		Inserisce nelle strutture dati gia' create un necrologio di codice C

					12 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto puntatore alla struttura da utilizzare
	@param			indice indice da usare per fare l'inserimento
	@param			ptrappware puntatore alle info da inserire
	@result  			nessuno
*/
static void XTAPI InserisciNecrologioC(defunto *ptrdefunto, int16 indice, appware *ptrappware) throw();

/*!
	@function			InserisciAnnuncio
	@abstract 		gestione annunci necrologi.
	@discussion		Inserisce nelle strutture dati gia' create un annuncio

					12 Aprile 2005 - Fabrizio.

	@param			ptrappware puntatore alle info da inserire
	@result  			nessuno
*/
static void XTAPI InserisciAnnuncio(appware *ptrappware) throw();

/*!
	@function			InserisciAnniversario
	@abstract 		gestione annunci necrologi.
	@discussion		Inserisce nelle strutture dati gia' create un anniversario

					12 Aprile 2005 - Fabrizio.

	@param			ptrappware puntatore alle info da inserire
	@result  			nessuno
*/
static void XTAPI InserisciAnniversario(appware *ptrappware) throw();

/*!
	@function			InserisciRingraziamento
	@abstract 		gestione annunci necrologi.
	@discussion		Inserisce nelle strutture dati gia' create un ringraziamento

					12 Aprile 2005 - Fabrizio.

	@param			ptrappware puntatore alle info da inserire
	@result  			nessuno
*/
static void XTAPI InserisciRingraziamento(appware *ptrappware) throw();

/*!
	@function			OrdinaDecrescente
	@abstract 		gestione annunci necrologi.
	@discussion		Ordina in modo decrescente sull'altezza degli anniversari e ringraziamenti di un defunto

					12 Aprile 2005 - Fabrizio.

	@param			a  puntatore alle info del defunto
	@param			b  puntatore alle info del defunto
	@result  			0 se le altezzze sono uguali, 1 se a e' piu' piccolo, -1 se a e' piu' grande
*/
static int16 XTAPI OrdinaDecrescente(const void *aa, const void *bb) throw();

/*!
	@function			OrdinaDecrescente
	@abstract 		gestione annunci necrologi.
	@discussion		Ordina in modo crescente sull'altezza degli anniversari e ringraziamenti di un defunto

					12 Aprile 2005 - Fabrizio.

	@param			a  puntatore alle info del defunto
	@param			b  puntatore alle info del defunto
	@result  			0 se le altezzze sono uguali, 1 se a e' piu' piccolo, -1 se a e' piu' grande
*/
static int16 XTAPI OrdinaCrescente(const void *aa, const void *bb) throw(); 

/*!
	@function			OrdinaDecrescente
	@abstract 		gestione annunci necrologi.
	@discussion		Ordina in modo decrescente sull'altezza degli annunci di un defunto
	
					12 Aprile 2005 - Fabrizio.

	@param			a  puntatore alle info del defunto
	@param			b  puntatore alle info del defunto
	@result  			0 se le altezzze sono uguali, 1 se a e' piu' piccolo, -1 se a e' piu' grande
*/
static int16 XTAPI OrdinaDecrescenteAnnunci(const void *aa, const void *bb) throw();

/*!
	@function			CreaListaNecrologi
	@abstract 		gestione annunci necrologi.
	@discussion		Trasforma gli array creati in precedenza in liste da usare all'interno dell'
					algoritmo di posizionamento
	
					12 Aprile 2005 - Fabrizio.

	@param			ptrdefunto puntatore all'array da trasformare in lista
	@param			numerodefunti numero defunti presenti nella lista
	@result  			nessuno
*/
static void XTAPI CreaListaNecrologi(defunto *ptrdefunto, int16 numerodefunti) throw();

/*!
	@function			InserisciInfoLunghezzaPagina
	@abstract 		gestione annunci necrologi.
	@discussion		nserisce l'occupazione in colonne di ogni defunto

					13 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto  puntatore alla lista dei defunti
	@param			numerodefunti  numero dei defunti contenuti nella lista
	@param			altezzacolonna altezza della colonna ottimale
	@result  			nessuno
*/
static void XTAPI  InserisciInfoLunghezzaPagina(defunto *ptrdefunto, int16 numerodefunti, Fixed altezzacolonna) throw();

/*!
	@function			ContaRigori
	@abstract 		gestione annunci necrologi.
	@discussion		Conta quanti rigori hanno l'array dei defunti passato

					13 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto array dei defunti da aggiornare
	@param			numerodefunti numero dei defunti contenuti nella lista
	@param			rigore 
	@result  			nessuno
*/
static int16 XTAPI ContaRigori(defunto *ptrdefunto, int16 numerodefunti, tiporigore rigore) throw();

/*!
	@function			ContaColonneDefuntiLunghi
	@abstract 		gestione annunci necrologi.
	@discussion		Conta quanti defunti sono piu' lunghi di una colonna senza rigore di inizio colonna
					o documento

					13 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto array dei defunti da contare
	@param  			numerodefunti numero dei defunti contenuti nella lista
	@result			ritorna il numero di rigori
*/
static int16 ContaColonneDefuntiLunghi(defunto *ptrdefunto, int16 numerodefunti) throw();

/*!
	@function			PrendiNumeroNecrologi
	@abstract 		gestione annunci necrologi.
	@discussion		Restituisce il numero dei necrologi nella lista

					13 Aprile 2005 - Fabrizio.

	@param 			ptrnecrologi puntatore alla lista dei necrologi
	@result  			ritorna il numero dei necrologi altrimenti ritorna 0
*/
static int16 PrendiNumeroNecrologi(necrologio *ptrnecrologi) throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	CalcolaNumeroNecrologi

* ------------------------------------------------------------------------ */
static errorixtension XTAPI CalcolaNumeroNecrologi(uchar* ptrdocumento) throw()
{
	// per dare una posizione dell'errore
	int32 lPosizioneErrore = 0;
	// per sapere quando incrementare i contatori 
	Boolean lDevoIncrementare = FALSE;
	// per controllare se c'e' un'incogruenza nei dati di input
	tipocomando lTipoComando = kNessunComando;
	
	while (1) 
	{
		if (*ptrdocumento == kMinore) 
		{
			ptrdocumento++;
			lPosizioneErrore++;
			switch (*ptrdocumento) 
			{
				case kDefunto:
					lDevoIncrementare = TRUE;
					lTipoComando = kNessunComando;
					break;
				case kAnnuncio:
					if (lDevoIncrementare == TRUE) 
					{
						// devo incrementare il contatore degli annunci
						gNumeroAnnunci++;
						lDevoIncrementare = FALSE;
						lTipoComando = kAnnuncio;
					} 
					else if (lTipoComando != kAnnuncio)
					{
						// errore c'e' un defunto con necrologi di tipo diverso
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreNecrologiNonOmogenei);
					}
					break;
				case kAnniversario:
					if (lDevoIncrementare == TRUE) 
					{
						// devo incrementare il contatore degli anniversari
						gNumeroAnniversari++;
						lDevoIncrementare = FALSE;
						lTipoComando = kAnniversario;
					} 
					else if (lTipoComando != kAnniversario) 
					{						
						// errore c'e' un defunto con necrologi di tipo diverso
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreNecrologiNonOmogenei);
					}
					break;
				case kRingraziamento:
					if (lDevoIncrementare == TRUE) 
					{
						// devo incrementare il contatore dei ringraziamenti
						gNumeroRingraziamenti++;
						lDevoIncrementare = FALSE;
						lTipoComando = kRingraziamento;
					} 
					else if (lTipoComando != kRingraziamento) 
					{
						// errore c'e' un defunto con necrologi di tipo diverso
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreNecrologiNonOmogenei);
					}
					break;
			}
		} 
		else
		if (*ptrdocumento == kFineFile)
			break;
		ptrdocumento++;
		lPosizioneErrore++;
	}

	// fine lettura inizio impaginazione
	sprintf((char*)gStringaC,
		    "annunci %d, ringraziamenti %d, anniversari %d",
		    (char*)gNumeroAnnunci,
		    (char*)gNumeroRingraziamenti,
		    (char*)gNumeroAnniversari);
	ConcatenaAllaStringaMessaggio(gStringaC);
	DaiMessaggio(kNumeroNecrologi);

	return(kNessunErrore);
} // CalcolaNumeroNecrologi

/* ------------------------------------------------------------------------ *

	LiberaListaNecrologi

* ------------------------------------------------------------------------ */
void XTAPI LiberaListaNecrologi(necrologio **ptrnecrologio) throw()
{
	// puntatore per scorrere i necrologi nella colonna
	necrologio *lPtrNecrologio = NULL;
	// per liberare la memoria di un necrologio
	necrologio *lPtrNecrologioDaLiberare = NULL;
	
	// imposto il puntatore
	lPtrNecrologio = (*ptrnecrologio);

	while (1) 
	{
		if (lPtrNecrologio == NULL) 
			break;
		lPtrNecrologioDaLiberare = lPtrNecrologio;
		lPtrNecrologio = (*lPtrNecrologio).successivo;
		DisposePtr((Ptr) lPtrNecrologioDaLiberare);
	}

	(*ptrnecrologio) = NULL;
} // LiberaListaNecrologi


/* ------------------------------------------------------------------------ *
	
	LiberaInfoDefuntiNecrologi

* ------------------------------------------------------------------------ */
void XTAPI LiberaInfoDefuntiNecrologi(defunto *ptrdefunto, int16 numerodefunti) throw()
{
	// per prendere un defunto
	defunto *lPtrDefunto = NULL;
	// per scorrere l'array dei defunti 	
	int16 lIndiceDefunti =0;	
	for (lIndiceDefunti = 0; lIndiceDefunti < numerodefunti; lIndiceDefunti++)
	{	
		// imposto il puntatore
		lPtrDefunto = ptrdefunto +lIndiceDefunti;
		LiberaListaNecrologi(&((*lPtrDefunto).elencoNecrologiA));
		(*lPtrDefunto).ultimoNecrologioA = NULL;
		LiberaListaNecrologi(&((*lPtrDefunto).elencoNecrologiB));
		(*lPtrDefunto).ultimoNecrologioB = NULL;
		LiberaListaNecrologi(&((*lPtrDefunto).elencoNecrologiC));
		(*lPtrDefunto).ultimoNecrologioC = NULL;
	 }
} // LiberaInfoDefuntiNecrologi

/* ------------------------------------------------------------------------ *

	LiberaListaDefunti

* ------------------------------------------------------------------------ */
void XTAPI LiberaListaDefunti(defunto **ptrdefunto) throw()
{
	// per scorrere la lista dei defunti
	defunto *lPtrDefunto = NULL;
	// per liberare la memoria di un defunto
	defunto *lPtrDefuntoDaLiberare = NULL;

	// imposto il puntatore 
	lPtrDefunto = (*ptrdefunto);
	
	while (1) 
	{
		if (lPtrDefunto == NULL) 
			break;
		lPtrDefuntoDaLiberare = lPtrDefunto;
		lPtrDefunto = (*lPtrDefunto).successivo;
		LiberaListaNecrologi(&((*lPtrDefuntoDaLiberare).elencoNecrologiA));
		LiberaListaNecrologi(&((*lPtrDefuntoDaLiberare).elencoNecrologiB));
		LiberaListaNecrologi(&((*lPtrDefuntoDaLiberare).elencoNecrologiC));
		DisposePtr((Ptr) lPtrDefuntoDaLiberare);
	}
	(*ptrdefunto) = NULL;
} // LiberaListaDefunti

/* ------------------------------------------------------------------------ *

	LiberaInfoDefunti

* ------------------------------------------------------------------------ */
void XTAPI LiberaInfoDefunti() throw()
{
	
	// libero le strutture costruite precedentemente
	if (gPtrAnnunci != NULL) 
	{
		// cancella la memoria del singoli necrologi
		LiberaInfoDefuntiNecrologi(gPtrAnnunci, gNumeroAnnunci);
		DisposePtr((Ptr) gPtrAnnunci);
		gPtrAnnunci = NULL;
		gNumeroAnnunci = 0;
		gIndiceAnnunci = -1;
	}
	
	if (gPtrAnniversari != NULL) 
	{
		// cancella la memoria del singoli necrologi
		LiberaInfoDefuntiNecrologi(gPtrAnniversari, gNumeroAnniversari);
		DisposePtr((Ptr) gPtrAnniversari);
		gPtrAnniversari = NULL;
		gNumeroAnniversari = 0;
		gIndiceAnniversari = -1;
	}
	
	if (gPtrRingraziamenti != NULL) 
	{
		// cancella la memoria del singoli necrologi
		LiberaInfoDefuntiNecrologi(gPtrRingraziamenti, gNumeroRingraziamenti);
		DisposePtr((Ptr) gPtrRingraziamenti);
		gPtrRingraziamenti = NULL;
		gNumeroRingraziamenti = 0;
		gIndiceRingraziamenti = -1;
	}
} // LiberaInfoDefunti

/* ------------------------------------------------------------------------ *

	CostruisciNecrologi

* ------------------------------------------------------------------------ */
errorixtension XTAPI CostruisciNecrologi(defunto **ptrdefunto, int16 numerodefunti) throw()
{	
	if (numerodefunti != 0) 
	{
		(*ptrdefunto) = (defunto *) NewPtrClear(numerodefunti * sizeof(defunto));
		if ((*ptrdefunto) == NULL) 
		{
			// errore nell'allocazione della memoria
			return(kErroreMemoria);
		}
	}
	return(kNessunErrore);
} // CostruisciNecrologi

/* ------------------------------------------------------------------------ *

	CostruisciInfoNecrologi

* ------------------------------------------------------------------------ */
static errorixtension XTAPI CostruisciInfoNecrologi() throw()
{
	// costruisco la struttura per gli annunci
	gErroreXtension = CostruisciNecrologi(&gPtrAnnunci, gNumeroAnnunci);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nell'allocazione della memoria;
		return(gErroreXtension);
	}

	// costruisco la struttura per gli anniversari 
	gErroreXtension = CostruisciNecrologi(&gPtrAnniversari, gNumeroAnniversari);
	if (gErroreXtension != kNessunErrore) 
	{
		return(gErroreXtension);
	}

	// costruisco la struttura per i ringraziamenti
	gErroreXtension = CostruisciNecrologi(&gPtrRingraziamenti, gNumeroRingraziamenti);
	if (gErroreXtension != kNessunErrore) 
	{
		return(gErroreXtension);
	}

	return(kNessunErrore);
} // CostruisciInfoNecrologi

/* ------------------------------------------------------------------------ *

	CreaInfoNecrologi

* ------------------------------------------------------------------------ */
errorixtension XTAPI CreaInfoNecrologi(uchar*ptrdocumento) throw()
{
	// libero le strutture costruite precedentemente 
	LiberaInfoDefunti();
	
	// conta i necrologi 
	gErroreXtension = CalcolaNumeroNecrologi(ptrdocumento);
	if (gErroreXtension != kNessunErrore) 
	{
		return(gErroreXtension);
	}
	
	// creo la memoria per i necrologi appena contati 
	gErroreXtension = CostruisciInfoNecrologi();
	if (gErroreXtension != kNessunErrore) 
	{
		return(gErroreXtension);
	}
	
	return(kNessunErrore);
} // CreaInfoNecrologi

/* ------------------------------------------------------------------------ *

	InserisciNecrologioA

* ------------------------------------------------------------------------ */
static void XTAPI InserisciNecrologioA(defunto *ptrdefunto, int16 indice, appware *ptrappware) throw()
{
	// puntatore per accedere alla struttura dove fare l'inserimento
	defunto *lPtrDefunto = NULL;
	// puntatore per costruire il nuovo record dove fare l'inserimento
	necrologio *lPtrNecrologio = NULL;
	
	// prendo la posizione dove inserire
	lPtrDefunto = ptrdefunto + indice;

	// controllo se ho gi… un necrologio
	if ((*lPtrDefunto).numeroNecrologi == 0)
	{
		// e' il primo necrologio da inserire
		STRCPY((*lPtrDefunto).codiceDefunto, (*ptrappware).codiceDefunto);
		(*lPtrDefunto).tipoRigore = (*ptrappware).tipoRigore;
		(*lPtrDefunto).altezzaTotaleDoubleFixed = (DoubleFixed) (*ptrappware).altezzaBox;
		assert(0 < (*lPtrDefunto).altezzaTotaleDoubleFixed);
		
		(*lPtrDefunto).numeroNecrologi = 1;


		// creo il record per contenere le nuove info
		lPtrNecrologio = (necrologio *) NewPtrClear(sizeof(necrologio));
		if (lPtrNecrologio == NULL) 
		{
			// errore nella creazione della memoria
			MostraErrore(kErroreMemoria);
			return;
		}

		// inserisco le nuove informazioni
		(*lPtrNecrologio).codiceBox = (*ptrappware).codiceBox;
		(*lPtrNecrologio).tipoGruppo = (*ptrappware).tipoGruppo;
		(*lPtrNecrologio).altezzaBox = (*ptrappware).altezzaBox;
		(*lPtrNecrologio).spazioTraParagrafi = (*ptrappware).spazioTraParagrafi;
		(*lPtrNecrologio).numeroParagrafiDiversi = (*ptrappware).numeroParagrafiDiversi;
		(*lPtrNecrologio).numeroParagrafi = (*ptrappware).numeroParagrafi;
		(*lPtrNecrologio).numeroLinee = (*ptrappware).numeroLinee;
		(*lPtrNecrologio).collegato = (*ptrappware).collegato;
		(*lPtrNecrologio).cambioColonna = FALSE;
		(*lPtrNecrologio).successivo = NULL;

		// aggiorno la lista dei necrologi
		(*lPtrDefunto).elencoNecrologiA = lPtrNecrologio;
		(*lPtrDefunto).ultimoNecrologioA = lPtrNecrologio;
	}
	else if ((*lPtrDefunto).elencoNecrologiA == NULL)
	{
		// e' il primo necrologio di codice A

		(*lPtrDefunto).altezzaTotaleDoubleFixed += (DoubleFixed) (*ptrappware).altezzaBox;
		assert(0 < (*lPtrDefunto).altezzaTotaleDoubleFixed);
		
		(*lPtrDefunto).numeroNecrologi++;

		// creo il record per contenere le nuove info
		lPtrNecrologio = (necrologio *) NewPtrClear(sizeof(necrologio));
		if (lPtrNecrologio == NULL) 
		{
			// errore nella creazione della memoria
			MostraErrore(kErroreMemoria);
			return;
		}

		// inserisco le nuove informazioni
		(*lPtrNecrologio).codiceBox = (*ptrappware).codiceBox;
		(*lPtrNecrologio).tipoGruppo = (*ptrappware).tipoGruppo;
		(*lPtrNecrologio).altezzaBox = (*ptrappware).altezzaBox;
		(*lPtrNecrologio).spazioTraParagrafi = (*ptrappware).spazioTraParagrafi;
		(*lPtrNecrologio).numeroParagrafiDiversi = (*ptrappware).numeroParagrafiDiversi;
		(*lPtrNecrologio).numeroParagrafi = (*ptrappware).numeroParagrafi;
		(*lPtrNecrologio).numeroLinee = (*ptrappware).numeroLinee;
		(*lPtrNecrologio).collegato = (*ptrappware).collegato;
		(*lPtrNecrologio).cambioColonna = FALSE;
		(*lPtrNecrologio).successivo = NULL;

		// aggiorno la lista dei necrologi
		(*lPtrDefunto).elencoNecrologiA = lPtrNecrologio;
		(*lPtrDefunto).ultimoNecrologioA = lPtrNecrologio;

	}
	else
	{
		// ho gia' inserito almeno un necrologio
		(*lPtrDefunto).altezzaTotaleDoubleFixed += (DoubleFixed) (*ptrappware).altezzaBox;
		assert(0 < (*lPtrDefunto).altezzaTotaleDoubleFixed);
		(*lPtrDefunto).numeroNecrologi++;
		
		// creo il record per contenere le nuove info
		lPtrNecrologio = (necrologio *) NewPtrClear(sizeof(necrologio));
		if (lPtrNecrologio == NULL) 
		{
			// errore nella creazione della memoria
			MostraErrore(kErroreMemoria);
			return;
		}
		
		// inserisco le nuove informazioni
		(*lPtrNecrologio).codiceBox = (*ptrappware).codiceBox;
		(*lPtrNecrologio).tipoGruppo = (*ptrappware).tipoGruppo;
		(*lPtrNecrologio).altezzaBox = (*ptrappware).altezzaBox;
		(*lPtrNecrologio).spazioTraParagrafi = (*ptrappware).spazioTraParagrafi;
		(*lPtrNecrologio).numeroParagrafiDiversi = (*ptrappware).numeroParagrafiDiversi;
		(*lPtrNecrologio).numeroParagrafi = (*ptrappware).numeroParagrafi;
		(*lPtrNecrologio).numeroLinee = (*ptrappware).numeroLinee;
		(*lPtrNecrologio).collegato = (*ptrappware).collegato;
		(*lPtrNecrologio).cambioColonna = FALSE;
		(*lPtrNecrologio).successivo = NULL;
		
		// aggiorno la lista dei necrologi
		(*(*lPtrDefunto).ultimoNecrologioA).successivo = lPtrNecrologio;
		(*lPtrDefunto).ultimoNecrologioA = lPtrNecrologio;
		
	}
} // InserisciNecrologioA

/* ------------------------------------------------------------------------ *

	InserisciNecrologioB

* ------------------------------------------------------------------------ */
static void XTAPI InserisciNecrologioB(defunto *ptrdefunto, int16 indice, appware *ptrappware) throw()
{
	// puntatore per accedere alla struttura dove fare l'inserimento
	defunto *lPtrDefunto = NULL;
	// puntatore per costruire il nuovo record dove fare l'inserimento
	necrologio *lPtrNecrologio = NULL;
	
	// prendo la posizione dove inserire 
	lPtrDefunto = ptrdefunto + indice;
	
	// controllo se ho gia' un necrologio
	if ((*lPtrDefunto).numeroNecrologi == 0)
	{
		// e' il primo necrologio da inserire
		STRCPY((*lPtrDefunto).codiceDefunto, (*ptrappware).codiceDefunto);
		(*lPtrDefunto).tipoRigore = (*ptrappware).tipoRigore;
		(*lPtrDefunto).altezzaTotaleDoubleFixed = (DoubleFixed) (*ptrappware).altezzaBox;
		assert(0 < (*lPtrDefunto).altezzaTotaleDoubleFixed);
		
		(*lPtrDefunto).numeroNecrologi = 1;
		
		// creo il record per contenere le nuove info 
		lPtrNecrologio = (necrologio *) NewPtrClear(sizeof(necrologio));
		if (lPtrNecrologio == NULL) 
		{
			// errore nella creazione della memoria
			MostraErrore(kErroreMemoria);
			return;
		}
		
		// inserisco le nuove informazioni
		(*lPtrNecrologio).codiceBox = (*ptrappware).codiceBox;
		(*lPtrNecrologio).tipoGruppo = (*ptrappware).tipoGruppo;
		(*lPtrNecrologio).altezzaBox = (*ptrappware).altezzaBox;
		(*lPtrNecrologio).spazioTraParagrafi = (*ptrappware).spazioTraParagrafi;
		(*lPtrNecrologio).numeroParagrafiDiversi = (*ptrappware).numeroParagrafiDiversi;
		(*lPtrNecrologio).numeroParagrafi = (*ptrappware).numeroParagrafi;
		(*lPtrNecrologio).numeroLinee = (*ptrappware).numeroLinee;
		(*lPtrNecrologio).collegato = (*ptrappware).collegato;
		(*lPtrNecrologio).cambioColonna = FALSE;
		(*lPtrNecrologio).successivo = NULL;
		
		// aggiorno la lista dei necrologi
		(*lPtrDefunto).elencoNecrologiB = lPtrNecrologio;
		(*lPtrDefunto).ultimoNecrologioB = lPtrNecrologio;
		
	}
	else if ((*lPtrDefunto).elencoNecrologiB == NULL)
	{
		// e' il primo necrologio di codice B
		(*lPtrDefunto).altezzaTotaleDoubleFixed += (DoubleFixed) (*ptrappware).altezzaBox;
		assert(0 < (*lPtrDefunto).altezzaTotaleDoubleFixed);
		
		(*lPtrDefunto).numeroNecrologi++;

		// creo il record per contenere le nuove info
		lPtrNecrologio = (necrologio *) NewPtrClear(sizeof(necrologio));
		if (lPtrNecrologio == NULL) 
		{
			// errore nella creazione della memoria
			MostraErrore(kErroreMemoria);
			return;
		}

		// inserisco le nuove informazioni
		(*lPtrNecrologio).codiceBox = (*ptrappware).codiceBox;
		(*lPtrNecrologio).tipoGruppo = (*ptrappware).tipoGruppo;
		(*lPtrNecrologio).altezzaBox = (*ptrappware).altezzaBox;
		(*lPtrNecrologio).spazioTraParagrafi = (*ptrappware).spazioTraParagrafi;
		(*lPtrNecrologio).numeroParagrafiDiversi = (*ptrappware).numeroParagrafiDiversi;
		(*lPtrNecrologio).numeroParagrafi = (*ptrappware).numeroParagrafi;
		(*lPtrNecrologio).numeroLinee = (*ptrappware).numeroLinee;
		(*lPtrNecrologio).collegato = (*ptrappware).collegato;
		(*lPtrNecrologio).cambioColonna = FALSE;
		(*lPtrNecrologio).successivo = NULL;

		// aggiorno la lista dei necrologi
		(*lPtrDefunto).elencoNecrologiB = lPtrNecrologio;
		(*lPtrDefunto).ultimoNecrologioB = lPtrNecrologio;

	}
	else
	{
		// ho gia' inserito almeno un necrologi
		(*lPtrDefunto).altezzaTotaleDoubleFixed += (DoubleFixed) (*ptrappware).altezzaBox;
		assert(0 < (*lPtrDefunto).altezzaTotaleDoubleFixed);
		
		(*lPtrDefunto).numeroNecrologi++;
		
		// creo il record per contenere le nuove info
		lPtrNecrologio = (necrologio *) NewPtrClear(sizeof(necrologio));
		if (lPtrNecrologio == NULL) 
		{
			// errore nella creazione della memoria
			MostraErrore(kErroreMemoria);
			return;
		}
		
		// inserisco le nuove informazioni
		(*lPtrNecrologio).codiceBox = (*ptrappware).codiceBox;
		(*lPtrNecrologio).tipoGruppo = (*ptrappware).tipoGruppo;
		(*lPtrNecrologio).altezzaBox = (*ptrappware).altezzaBox;
		(*lPtrNecrologio).spazioTraParagrafi = (*ptrappware).spazioTraParagrafi;
		(*lPtrNecrologio).numeroParagrafiDiversi = (*ptrappware).numeroParagrafiDiversi;
		(*lPtrNecrologio).numeroParagrafi = (*ptrappware).numeroParagrafi;
		(*lPtrNecrologio).numeroLinee = (*ptrappware).numeroLinee;
		(*lPtrNecrologio).collegato = (*ptrappware).collegato;
		(*lPtrNecrologio).cambioColonna = FALSE;
		(*lPtrNecrologio).successivo = NULL;
		
		// aggiorno la lista dei necrologi
		(*(*lPtrDefunto).ultimoNecrologioB).successivo = lPtrNecrologio;
		(*lPtrDefunto).ultimoNecrologioB = lPtrNecrologio;
		
	}
} // InserisciNecrologioB

/* ------------------------------------------------------------------------ *

	InserisciNecrologioC

* ------------------------------------------------------------------------ */
static void XTAPI InserisciNecrologioC(defunto *ptrdefunto, int16 indice, appware *ptrappware) throw()
{
	// puntatore per accedere alla struttura dove fare l'inserimento 
	defunto *lPtrDefunto = NULL;
	// puntatore per costruire il nuovo record dove fare l'inserimento
	necrologio *lPtrNecrologio = NULL;
	
	// prendo la posizione dove inserire
	lPtrDefunto = ptrdefunto + indice;
	
	// controllo se ho gi… un necrologio
	if ((*lPtrDefunto).numeroNecrologi == 0)
	{
		// e' il primo necrologio da inserire
		STRCPY((*lPtrDefunto).codiceDefunto, (*ptrappware).codiceDefunto);
		(*lPtrDefunto).tipoRigore = (*ptrappware).tipoRigore;
		(*lPtrDefunto).altezzaTotaleDoubleFixed = (DoubleFixed) (*ptrappware).altezzaBox;
		assert(0 < (*lPtrDefunto).altezzaTotaleDoubleFixed);
		
		(*lPtrDefunto).numeroNecrologi = 1;
		
		// creo il record per contenere le nuove info
		lPtrNecrologio = (necrologio *) NewPtrClear(sizeof(necrologio));
		if (lPtrNecrologio == NULL) 
		{
			// errore nella creazione della memoria
			MostraErrore(kErroreMemoria);
			return;
		}
		
		// inserisco le nuove informazioni
		(*lPtrNecrologio).codiceBox = (*ptrappware).codiceBox;
		(*lPtrNecrologio).tipoGruppo = (*ptrappware).tipoGruppo;
		(*lPtrNecrologio).altezzaBox = (*ptrappware).altezzaBox;
		(*lPtrNecrologio).spazioTraParagrafi = (*ptrappware).spazioTraParagrafi;
		(*lPtrNecrologio).numeroParagrafiDiversi = (*ptrappware).numeroParagrafiDiversi;
		(*lPtrNecrologio).numeroParagrafi = (*ptrappware).numeroParagrafi;
		(*lPtrNecrologio).numeroLinee = (*ptrappware).numeroLinee;
		(*lPtrNecrologio).collegato = (*ptrappware).collegato;
		(*lPtrNecrologio).cambioColonna = FALSE;
		(*lPtrNecrologio).successivo = NULL;
		
		// aggiorno la lista dei necrologi
		(*lPtrDefunto).elencoNecrologiC = lPtrNecrologio;
		(*lPtrDefunto).ultimoNecrologioC = lPtrNecrologio;
		
	}
	else if ((*lPtrDefunto).elencoNecrologiC == NULL)
	{
		// e' il primo necrologio di codice C
		(*lPtrDefunto).altezzaTotaleDoubleFixed += (DoubleFixed) (*ptrappware).altezzaBox;
		assert(0 < (*lPtrDefunto).altezzaTotaleDoubleFixed);
		
		(*lPtrDefunto).numeroNecrologi++;
		
		// creo il record per contenere le nuove info
		lPtrNecrologio = (necrologio *) NewPtrClear(sizeof(necrologio));
		if (lPtrNecrologio == NULL) 
		{
			// errore nella creazione della memoria
			MostraErrore(kErroreMemoria);
			return;
		}
		
		// inserisco le nuove informazioni
		(*lPtrNecrologio).codiceBox = (*ptrappware).codiceBox;
		(*lPtrNecrologio).tipoGruppo = (*ptrappware).tipoGruppo;
		(*lPtrNecrologio).altezzaBox = (*ptrappware).altezzaBox;
		(*lPtrNecrologio).spazioTraParagrafi = (*ptrappware).spazioTraParagrafi;
		(*lPtrNecrologio).numeroParagrafiDiversi = (*ptrappware).numeroParagrafiDiversi;
		(*lPtrNecrologio).numeroParagrafi = (*ptrappware).numeroParagrafi;
		(*lPtrNecrologio).numeroLinee = (*ptrappware).numeroLinee;
		(*lPtrNecrologio).collegato = (*ptrappware).collegato;
		(*lPtrNecrologio).cambioColonna = FALSE;
		(*lPtrNecrologio).successivo = NULL;
		
		// aggiorno la lista dei necrologi
		(*lPtrDefunto).elencoNecrologiC = lPtrNecrologio;
		(*lPtrDefunto).ultimoNecrologioC = lPtrNecrologio;
		
	}
	else
	{
		// ho gia' inserito almeno un necrologio
		(*lPtrDefunto).altezzaTotaleDoubleFixed += (DoubleFixed) (*ptrappware).altezzaBox;
		assert(0 < (*lPtrDefunto).altezzaTotaleDoubleFixed);
		
		(*lPtrDefunto).numeroNecrologi++;
		
		// creo il record per contenere le nuove info
		lPtrNecrologio = (necrologio *) NewPtrClear(sizeof(necrologio));
		if (lPtrNecrologio == NULL) 
		{
			// errore nella creazione della memoria
			MostraErrore(kErroreMemoria);
			return;
		}
		
		// inserisco le nuove informazioni
		(*lPtrNecrologio).codiceBox = (*ptrappware).codiceBox;
		(*lPtrNecrologio).tipoGruppo = (*ptrappware).tipoGruppo;
		(*lPtrNecrologio).altezzaBox = (*ptrappware).altezzaBox;
		(*lPtrNecrologio).spazioTraParagrafi = (*ptrappware).spazioTraParagrafi;
		(*lPtrNecrologio).numeroParagrafiDiversi = (*ptrappware).numeroParagrafiDiversi;
		(*lPtrNecrologio).numeroParagrafi = (*ptrappware).numeroParagrafi;
		(*lPtrNecrologio).numeroLinee = (*ptrappware).numeroLinee;
		(*lPtrNecrologio).collegato = (*ptrappware).collegato;
		(*lPtrNecrologio).cambioColonna = FALSE;
		(*lPtrNecrologio).successivo = NULL;
		
		// aggiorno la lista dei necrologi
		(*(*lPtrDefunto).ultimoNecrologioC).successivo = lPtrNecrologio;
		(*lPtrDefunto).ultimoNecrologioC = lPtrNecrologio;
		
	}
} // InserisciNecrologioC

/* ------------------------------------------------------------------------ *

	InserisciAnnuncio

* ------------------------------------------------------------------------ */
static void XTAPI InserisciAnnuncio(appware *ptrappware) throw()
{
	// aggiorno contatore
	if (gNuovoDefunto == TRUE) 
	{
		gIndiceAnnunci++;
		gNuovoDefunto = FALSE;
	}
	
	if (gIndiceAnnunci < gNumeroAnnunci) 
	{
		// inserisco l'annuncio
		if ((*ptrappware).tipoGruppo == kCodiceA)
			InserisciNecrologioA(gPtrAnnunci, gIndiceAnnunci, ptrappware);
		else if ((*ptrappware).tipoGruppo == kCodiceC)
			InserisciNecrologioC(gPtrAnnunci, gIndiceAnnunci, ptrappware);
		else InserisciNecrologioB(gPtrAnnunci, gIndiceAnnunci, ptrappware);

	} else 
	{
		// errore di chiamata di questa procedura
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
	}
} // InserisciAnnuncio

/* ------------------------------------------------------------------------ *

	InserisciAnniversario

* ------------------------------------------------------------------------ */
static void XTAPI InserisciAnniversario(appware *ptrappware) throw()
{
	// aggiorno contatore
	if (gNuovoDefunto == TRUE) 
	{
		gIndiceAnniversari++;
		gNuovoDefunto = FALSE;
	}
	
	if (gIndiceAnniversari < gNumeroAnniversari) 
	{
		// inserisco l'anniversario
		InserisciNecrologioC(gPtrAnniversari, gIndiceAnniversari, ptrappware);
	} else 
	{
		// errore di chiamata di questa procedura
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
	}
} // InserisciAnniversario

/* ------------------------------------------------------------------------ *

	InserisciRingraziamento

* ------------------------------------------------------------------------ */
static void XTAPI InserisciRingraziamento(appware *ptrappware) throw()
{
	// aggiorno contatore
	if (gNuovoDefunto == TRUE) 
	{
		gIndiceRingraziamenti++;
		gNuovoDefunto = FALSE;
	}
	
	if (gIndiceRingraziamenti < gNumeroRingraziamenti) 
	{
		// inserisco il ringraziamento
		InserisciNecrologioC(gPtrRingraziamenti, gIndiceRingraziamenti, ptrappware);
	} 
	else 
	{
		// errore di chiamata di questa procedura
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
	}
} // InserisciRingraziamento

/* ------------------------------------------------------------------------ *

	ImpostaNuovoDefunto

* ------------------------------------------------------------------------ */
void XTAPI ImpostaNuovoDefunto() throw()
{
	gNuovoDefunto = TRUE;
} // ImpostaNuovoDefunto

/* ------------------------------------------------------------------------ *

	InserisciInfoNecrologio

* ------------------------------------------------------------------------ */
void XTAPI InserisciInfoNecrologio(appware *ptrappware) throw()
{
	switch ((*ptrappware).tipoAppWare) 
	{
		case kAnnuncioAppWare:
			InserisciAnnuncio(ptrappware);
			break;
		case kAnniversarioAppWare:
			InserisciAnniversario(ptrappware);
			break;
		case kRingraziamentoAppWare:
			InserisciRingraziamento(ptrappware);
			break;
	}
} // InserisciInfoNecrologio

/* ------------------------------------------------------------------------ *

	OrdinaDecrescente

* ------------------------------------------------------------------------ */
static int16 XTAPI OrdinaDecrescente(const void *aa, const void *bb) throw()
{
	defunto *a, *b;
	a = (defunto *)aa;
	b = (defunto *)bb;

	if( (a->tipoRigore == kNessunRigore && b->tipoRigore == kNessunRigore) ||
		(a->tipoRigore == kRigoreDiColonna && b->tipoRigore == kRigoreDiColonna)) 
	{
		if (a->altezzaTotaleDoubleFixed < b->altezzaTotaleDoubleFixed) 
			return(1);
		if (a->altezzaTotaleDoubleFixed > b->altezzaTotaleDoubleFixed) 
			return(-1);
		return(0);
	} 
	else if (a->tipoRigore == kRigoreDiColonna && b->tipoRigore == kNessunRigore) 
	{
		return(-1);
	}
	 else if (a->tipoRigore == kNessunRigore && b->tipoRigore == kRigoreDiColonna) 
	{
		return(1);
	} 
	else 
	{
		if (a->altezzaTotaleDoubleFixed < b->altezzaTotaleDoubleFixed) 
			return(1);
		if (a->altezzaTotaleDoubleFixed > b->altezzaTotaleDoubleFixed) 
			return(-1);
		return(0);
	}
} // OrdinaDecrescente

/* ------------------------------------------------------------------------ *

	OrdinaCrescente

* ------------------------------------------------------------------------ */
static int16 XTAPI OrdinaCrescente(const void *aa, const void *bb) throw()
{
	defunto *a, *b;
	a = (defunto *)aa;
	b = (defunto *)bb;

	if( (a->tipoRigore == kNessunRigore && b->tipoRigore == kNessunRigore) ||
		(a->tipoRigore == kRigoreDiColonna && b->tipoRigore == kRigoreDiColonna)) 
	{
		if (a->altezzaTotaleDoubleFixed < b->altezzaTotaleDoubleFixed) 
			return(-1);
		if (a->altezzaTotaleDoubleFixed > b->altezzaTotaleDoubleFixed) 
			return(1);
		return(0);
	} 
	else if (a->tipoRigore == kRigoreDiColonna && b->tipoRigore == kNessunRigore) 
	{
		return(1);
	} 
	else if (a->tipoRigore == kNessunRigore && b->tipoRigore == kRigoreDiColonna) 
	{
		return(-1);
	} 
	else 
	{
		if (a->altezzaTotaleDoubleFixed < b->altezzaTotaleDoubleFixed) 
			return(-1);
		if (a->altezzaTotaleDoubleFixed > b->altezzaTotaleDoubleFixed) 
			return(1);
		return(0);
	}
} // OrdinaCrescente

/* ------------------------------------------------------------------------ *

	OrdinaDecrescente

* ------------------------------------------------------------------------ */
static int16 XTAPI OrdinaDecrescenteAnnunci(const void *aa, const void *bb) throw()
{
	defunto *a, *b;
	a = (defunto *)aa;
	b = (defunto *)bb;

	if ((a->tipoRigore == kNessunRigore && b->tipoRigore == kNessunRigore) ||
		(a->tipoRigore == kRigoreDiColonna && b->tipoRigore == kRigoreDiColonna))
	{
		if (a->altezzaTotaleDoubleFixed < b->altezzaTotaleDoubleFixed) 
			return(1);
		if (a->altezzaTotaleDoubleFixed > b->altezzaTotaleDoubleFixed) 
			return(-1);
		return(0);
	} 
	else if (a->tipoRigore == kRigoreDiDocumento && b->tipoRigore == kNessunRigore) 
	{
		return(-1);
	} 
	else if (a->tipoRigore == kRigoreDiColonna && b->tipoRigore == kNessunRigore) 
	{
		if (b->numeroColonne == 1) 
		{
			return(-1);
		} 
		else 
		{
			if (a->altezzaTotaleDoubleFixed < b->altezzaTotaleDoubleFixed) 
				return(1);
			if (a->altezzaTotaleDoubleFixed > b->altezzaTotaleDoubleFixed) 
				return(-1);
			return(0);
		}
	} 
	else if (a->tipoRigore == kNessunRigore && b->tipoRigore == kRigoreDiDocumento) 
	{
		return(1);
	} 
	else if (a->tipoRigore == kRigoreDiColonna && b->tipoRigore == kRigoreDiDocumento) 
	{
		return(1);
	} 
	else if (a->tipoRigore == kNessunRigore && b->tipoRigore == kRigoreDiColonna) 
	{
		if (a->numeroColonne == 1) 
		{
			return(1);
		} 
		else 
		{
			if (a->altezzaTotaleDoubleFixed < b->altezzaTotaleDoubleFixed) 
				return(1);
			if (a->altezzaTotaleDoubleFixed > b->altezzaTotaleDoubleFixed) 
				return(-1);
			return(0);
		}
	} 
	else if (a->tipoRigore == kRigoreDiDocumento && b->tipoRigore == kRigoreDiColonna) 
	{
		return(-1);
	} else 
	{
		if (a->altezzaTotaleDoubleFixed < b->altezzaTotaleDoubleFixed) 
			return(1);
		if (a->altezzaTotaleDoubleFixed > b->altezzaTotaleDoubleFixed) 
			return(-1);
		return(0);
	}
} // OrdinaDecrescenteAnnunci

/* ------------------------------------------------------------------------ *

	OrdinaInfoNecrologi

* ------------------------------------------------------------------------ */
void XTAPI OrdinaInfoNecrologi() throw()
{
	// ordino in modo decrescente gli annunci
	qsort(gPtrAnnunci,
			(size_t) gNumeroAnnunci,
			(size_t) sizeof(defunto),
			(int (*)(const void*, const void*)) OrdinaDecrescenteAnnunci);

	// novita' versione 2
	if (gXtPreferences.disposizionePref.tipoDisposizione == VERTICALERADIOBTNID)
	{
		// impaginazione tipo Bg
		// ordino in modo crescente i ringraziamenti
		qsort(gPtrRingraziamenti,
				(size_t) gNumeroRingraziamenti,
				(size_t) sizeof(defunto),
				(int (*)(const void*, const void*)) OrdinaDecrescente);

		// ordino in modo crescente gli anniversari
		qsort(gPtrAnniversari,
				(size_t) gNumeroAnniversari,
				(size_t) sizeof(defunto),
				(int (*)(const void*, const void*)) OrdinaDecrescente);
	}
	else
	{
		// impaginazione tipo  Co
		// ordino in modo crescente i ringraziamenti
		qsort(gPtrRingraziamenti,
				(size_t) gNumeroRingraziamenti,
				(size_t) sizeof(defunto),
				(int (*)(const void*, const void*))OrdinaCrescente);

		// ordino in modo crescente gli anniversari
		qsort(gPtrAnniversari,
				(size_t) gNumeroAnniversari,
				(size_t) sizeof(defunto),
				(int (*)(const void*, const void*)) OrdinaCrescente);
	}
} // OrdinaInfoNecrologi

/* ------------------------------------------------------------------------ *

	CreaListaNecrologi

* ------------------------------------------------------------------------ */
static void XTAPI CreaListaNecrologi(defunto *ptrdefunto, int16 numerodefunti) throw()
{
	// per scorrere i necrologi 
	int16 lIndice = 0;
	// per scorrere l'array
	defunto *lPtrDefunto = NULL;
	// per ricordare l'elemento precedente della lista
	defunto *lPtrDefuntoPrecedente = NULL;
	
	// salvo il puntatore al necrologio
	lPtrDefuntoPrecedente = ptrdefunto;
	
	// scorro tutti i defunti
	for (lIndice = 1; lIndice < numerodefunti; lIndice++) 
	{
		lPtrDefunto = ptrdefunto + lIndice;
		
		// imposto il puntatore alla lista
		(*lPtrDefuntoPrecedente).successivo = lPtrDefunto;
		lPtrDefuntoPrecedente = lPtrDefunto;
	}
} // CreaListaNecrologi

/* ------------------------------------------------------------------------ *

	CreaListeInfoNecrologi

* ------------------------------------------------------------------------ */
void XTAPI CreaListeInfoNecrologi() throw()
{ 
	CreaListaNecrologi(gPtrAnnunci, gNumeroAnnunci);
	CreaListaNecrologi(gPtrAnniversari, gNumeroAnniversari);
	CreaListaNecrologi(gPtrRingraziamenti, gNumeroRingraziamenti);
} // CreaListeInfoNecrologi

/* ------------------------------------------------------------------------ *

	EstraiNecrologio

* ------------------------------------------------------------------------ */
necrologio* XTAPI EstraiNecrologio(defunto **ptrdefunto) throw()
{
	// per scorrere la lista dei defunti
	defunto *lPtrDefunto = NULL;
	// per scorrere la lista dei necrologi
	necrologio *lPtrNecrologio = NULL;
	
	// imposto il puntatore alla lista dei defunti
	lPtrDefunto = (*ptrdefunto);
	
	while (1) 
	{
		// controllo se ci sono altri elementi nella lista
		if (lPtrDefunto == NULL) 
		{
			return(NULL);
		}
		
		// controllo se il defunto corrente ha ancora dei necrologi
		if ((*lPtrDefunto).elencoNecrologiA != NULL) 
		{
			// estraggo il primo elemento della lista codice A
			lPtrNecrologio = (*lPtrDefunto).elencoNecrologiA;
			
			// avanzo il puntatore all'elemento successivo della lista
			(*lPtrDefunto).elencoNecrologiA = (*lPtrNecrologio).successivo;
			
			(*ptrdefunto) = lPtrDefunto;
			(*lPtrNecrologio).successivo = NULL;
			return(lPtrNecrologio);
		} 
		else if ((*lPtrDefunto).elencoNecrologiB != NULL) 
		{
			// estraggo il primo elemento della lista codice B
			lPtrNecrologio = (*lPtrDefunto).elencoNecrologiB;
			
			// avanzo il puntatore all'elemento successivo della lista
			(*lPtrDefunto).elencoNecrologiB = (*lPtrNecrologio).successivo;
			
			(*ptrdefunto) = lPtrDefunto;
			(*lPtrNecrologio).successivo = NULL;
			return(lPtrNecrologio);
		}
		else if ((*lPtrDefunto).elencoNecrologiC != NULL) 
		{
			// estraggo il primo elemento della lista codice C
			lPtrNecrologio = (*lPtrDefunto).elencoNecrologiC;
			
			// avanzo il puntatore all'elemento successivo della lista
			(*lPtrDefunto).elencoNecrologiC = (*lPtrNecrologio).successivo;
			
			(*ptrdefunto) = lPtrDefunto;
			(*lPtrNecrologio).successivo = NULL;
			return(lPtrNecrologio);
		} 
		else 
		{
			(*lPtrDefunto).ultimoNecrologioA = NULL;
			(*lPtrDefunto).ultimoNecrologioB = NULL;
			(*lPtrDefunto).ultimoNecrologioC = NULL;
		}
		
		// questo defunto non ha piu' necrologi
		// vado sul successivo
		lPtrDefunto = (*lPtrDefunto).successivo;
	}
} // EstraiNecrologio

/* ------------------------------------------------------------------------ *

	EstraiSoloNecrologio

* ------------------------------------------------------------------------ */
necrologio* XTAPI EstraiSoloNecrologio(defunto *ptrdefunto) throw()
{
	// per scorrere la lista dei defunti
	defunto *lPtrDefunto = NULL;
	// per scorrere la lista dei necrologi
	necrologio *lPtrNecrologio = NULL;

	// imposto il puntatore alla lista dei defunti
	lPtrDefunto = ptrdefunto;
	
	while (1) 
	{
		// controllo se ci sono altri elementi nella lista
		if (lPtrDefunto == NULL) 
		{
			return(NULL);
		}
		
		// controllo se il defunto corrente ha ancora dei necrologi
		if ((*lPtrDefunto).elencoNecrologiA != NULL) 
		{
			// estraggo il primo elemento della lista codice A
			lPtrNecrologio = (*lPtrDefunto).elencoNecrologiA;
			
			// avanzo il puntatore all'elemento successivo della lista
			(*lPtrDefunto).elencoNecrologiA = (*lPtrNecrologio).successivo;
			
			(*lPtrNecrologio).successivo = NULL;
			return(lPtrNecrologio);
		} 
		else if ((*lPtrDefunto).elencoNecrologiB != NULL) 
		{
			// estraggo il primo elemento della lista codice B
			lPtrNecrologio = (*lPtrDefunto).elencoNecrologiB;
			
			// avanzo il puntatore all'elemento successivo della lista
			(*lPtrDefunto).elencoNecrologiB = (*lPtrNecrologio).successivo;
			
			(*lPtrNecrologio).successivo = NULL;
			return(lPtrNecrologio);
		} 
		else if ((*lPtrDefunto).elencoNecrologiC != NULL) 
		{
			// estraggo il primo elemento della lista codice C
			lPtrNecrologio = (*lPtrDefunto).elencoNecrologiC;
			
			// avanzo il puntatore all'elemento successivo della lista
			(*lPtrDefunto).elencoNecrologiC = (*lPtrNecrologio).successivo;
			
			(*lPtrNecrologio).successivo = NULL;
			return(lPtrNecrologio);
		} 
		else 
		{
			(*lPtrDefunto).ultimoNecrologioA = NULL;
			(*lPtrDefunto).ultimoNecrologioB = NULL;
			(*lPtrDefunto).ultimoNecrologioC = NULL;
			return(NULL);
		}
		
	}
} // EstraiSoloNecrologio

/* ------------------------------------------------------------------------ *

	EstraiSoloNecrologioA

* ------------------------------------------------------------------------ */
necrologio* XTAPI EstraiSoloNecrologioA(defunto *ptrdefunto) throw()
{
	// per scorrere la lista dei defunti 
	defunto *lPtrDefunto = NULL;
	// per scorrere la lista dei necrologi
	necrologio *lPtrNecrologio = NULL;
		
	// imposto il puntatore alla lista dei defunti
	lPtrDefunto = ptrdefunto;
	
	while (1) 
	{
		// controllo se ci sono altri elementi nella lista
		if (lPtrDefunto == NULL) 
		{
			return(NULL);
		}
		
		// controllo se il defunto corrente ha ancora dei necrologi
		if ((*lPtrDefunto).elencoNecrologiA != NULL) 
		{
			// estraggo il primo elemento della lista codice A
			lPtrNecrologio = (*lPtrDefunto).elencoNecrologiA;
			
			// avanzo il puntatore all'elemento successivo della lista
			(*lPtrDefunto).elencoNecrologiA = (*lPtrNecrologio).successivo;
			
			(*lPtrNecrologio).successivo = NULL;
			return(lPtrNecrologio);
		} 
		else 
		{
			(*lPtrDefunto).ultimoNecrologioA = NULL;
			return(NULL);
		}
		
	}
} // EstraiSoloNecrologioA

/* ------------------------------------------------------------------------ *

	EstraiSoloNecrologioBC

* ------------------------------------------------------------------------ */
necrologio* XTAPI EstraiSoloNecrologioBC(defunto *ptrdefunto) throw()
{
	// per scorrere la lista dei defunti
	defunto *lPtrDefunto = NULL;
	// per scorrere la lista dei necrologi
	necrologio *lPtrNecrologio = NULL;
	
	// imposto il puntatore alla lista dei defunti
	lPtrDefunto = ptrdefunto;
	
	while (1) 
	{
		// controllo se ci sono altri elementi nella lista
		if (lPtrDefunto == NULL) 
		{
			return(NULL);
		}
		
		// controllo se il defunto corrente ha ancora dei necrologi
		if ((*lPtrDefunto).elencoNecrologiB != NULL) 
		{
			// estraggo il primo elemento della lista codice B
			lPtrNecrologio = (*lPtrDefunto).elencoNecrologiB;
			
			// avanzo il puntatore all'elemento successivo della lista
			(*lPtrDefunto).elencoNecrologiB = (*lPtrNecrologio).successivo;
			
			(*lPtrNecrologio).successivo = NULL;
			return(lPtrNecrologio);
		} 
		else if ((*lPtrDefunto).elencoNecrologiC != NULL) 
		{
			// estraggo il primo elemento della lista codice C
			lPtrNecrologio = (*lPtrDefunto).elencoNecrologiC;
			
			// avanzo il puntatore all'elemento successivo della lista
			(*lPtrDefunto).elencoNecrologiC = (*lPtrNecrologio).successivo;
			
			(*lPtrNecrologio).successivo = NULL;
			return(lPtrNecrologio);
		} 
		else
		{
			(*lPtrDefunto).ultimoNecrologioB = NULL;
			(*lPtrDefunto).ultimoNecrologioC = NULL;
			return(NULL);
		}
	}
} // EstraiSoloNecrologioBC

/* ------------------------------------------------------------------------ *

	EstraiSoloNecrologioB

* ------------------------------------------------------------------------ */
necrologio* XTAPI EstraiSoloNecrologioB(defunto *ptrdefunto) throw()
{
	// per scorrere la lista dei defunti
	defunto *lPtrDefunto = NULL;
	// per scorrere la lista dei necrologi
	necrologio *lPtrNecrologio = NULL;
	
	// imposto il puntatore alla lista dei defunti
	lPtrDefunto = ptrdefunto;
	
	while (1) 
	{
		// controllo se ci sono altri elementi nella lista
		if (lPtrDefunto == NULL) 
		{
			return(NULL);
		}
		
		// controllo se il defunto corrente ha ancora dei necrologi
		if ((*lPtrDefunto).elencoNecrologiB != NULL) 
		{
			// estraggo il primo elemento della lista codice B
			lPtrNecrologio = (*lPtrDefunto).elencoNecrologiB;
			
			// avanzo il puntatore all'elemento successivo della lista
			(*lPtrDefunto).elencoNecrologiB = (*lPtrNecrologio).successivo;
			
			(*lPtrNecrologio).successivo = NULL;
			return(lPtrNecrologio);
		} 
		else 
		{
			(*lPtrDefunto).ultimoNecrologioB = NULL;
			return(NULL);
		}
	}
} // EstraiSoloNecrologioB

/* ------------------------------------------------------------------------ *

	EstraiSoloNecrologioC

* ------------------------------------------------------------------------ */
necrologio* XTAPI EstraiSoloNecrologioC(defunto *ptrdefunto) throw()
{
	// per scorrere la lista dei defunti
	defunto *lPtrDefunto = NULL;
	// per scorrere la lista dei necrologi
	necrologio *lPtrNecrologio = NULL;
		
	// imposto il puntatore alla lista dei defunti
	lPtrDefunto = ptrdefunto;
	
	while (1) 
	{
	
		// controllo se ci sono altri elementi nella lista
		if (lPtrDefunto == NULL) 
		{
			return(NULL);
		}
		
		// controllo se il defunto corrente ha ancora dei necrologi
		if ((*lPtrDefunto).elencoNecrologiC != NULL) 
		{
			// estraggo il primo elemento della lista codice C
			lPtrNecrologio = (*lPtrDefunto).elencoNecrologiC;
			
			// avanzo il puntatore all'elemento successivo della lista
			(*lPtrDefunto).elencoNecrologiC = (*lPtrNecrologio).successivo;
			
			(*lPtrNecrologio).successivo = NULL;
			return(lPtrNecrologio);
		} 
		else 
		{
			(*lPtrDefunto).ultimoNecrologioC = NULL;
			return(NULL);
		}
	}
} // EstraiSoloNecrologioC

/* ------------------------------------------------------------------------ *

	EstraiDefunto

* ------------------------------------------------------------------------ */
defunto* XTAPI EstraiDefunto(defunto **ptrdefunto) throw()
{
	// per scorrere la lista dei defunti
	defunto *lPtrDefunto = NULL;
	
	// imposto il puntatore alla lista dei defunti
	lPtrDefunto = (*ptrdefunto);
	
	while (1) 
	{
		// controllo se ci sono altri elementi nella lista
		if (lPtrDefunto == NULL) 
		{
			(*ptrdefunto) = NULL;
			return(NULL);
		}
		
		(*ptrdefunto) = (*lPtrDefunto).successivo;
		(*lPtrDefunto).successivo = NULL;
		return(lPtrDefunto);
	}
} // EstraiDefunto

/* ------------------------------------------------------------------------ *

	PrendiAltezzaDefunto
	
* ------------------------------------------------------------------------ */
DoubleFixed XTAPI PrendiAltezzaDefunto(defunto *ptrdefunto) throw()
{
	// controllo se ci sono elementi nella lista
	if (ptrdefunto == NULL) 
		return(0);

	// ritorno l'altezza del defunto
	assert(0 < (*ptrdefunto).altezzaTotaleDoubleFixed);
		return((*ptrdefunto).altezzaTotaleDoubleFixed);
} // PrendiAltezzaDefunto

/* ------------------------------------------------------------------------ *

	PrendiAltezzaTotaleDefunti
	
* ------------------------------------------------------------------------ */
uint32 PrendiAltezzaTotaleDefunti(defunto *ptrdefunto, int16 numerodefunti) throw()
{
	// per scorrere i necrologi
	int16 lIndice = 0;
	// per scorrere l'array
	defunto *lPtrDefunto = NULL;
	// per calcolare l'altezza totale
	DoubleFixed lAltezzaTotaleDoubleFixed = 0.0;
	// per calcolare l'altezza totale
	uint32 lAltezzaTotale = 0;
	uchar lStr[kDimensioneStringhe] = "";

	// scorro tutti i defunti
	for (lIndice = 0; lIndice < numerodefunti; lIndice++) 
	{
		lPtrDefunto = ptrdefunto + lIndice;

		lAltezzaTotaleDoubleFixed += (*lPtrDefunto).altezzaTotaleDoubleFixed;
		assert(0 < (*lPtrDefunto).altezzaTotaleDoubleFixed);
	}

	lAltezzaTotale =  mDoubleFixed2Long(lAltezzaTotaleDoubleFixed);
	assert(0 <= lAltezzaTotale);

	return(lAltezzaTotale);
} // PrendiAltezzaTotaleDefunti

/* ------------------------------------------------------------------------ *

	PrendiAltezzaTotaleListaDefunti

* ------------------------------------------------------------------------ */
uint32 XTAPI PrendiAltezzaTotaleListaDefunti(defunto *ptrdefunto) throw()
{
	// per scorrere la lista dei defunti
	defunto *lPtrDefunto = NULL;
	// per calcolare l'altezza totale
	DoubleFixed lAltezzaTotaleDoubleFixed = 0.0;
	// per calcolare l'altezza totale
	uint32 lAltezzaTotale = 0;
	
	lPtrDefunto = ptrdefunto;

	while (1) 
	{
		if (lPtrDefunto == NULL) 
			break;
		lAltezzaTotaleDoubleFixed += (*lPtrDefunto).altezzaTotaleDoubleFixed;
		lPtrDefunto = (*lPtrDefunto).successivo;
	}

	lAltezzaTotale += mDoubleFixed2Long(lAltezzaTotaleDoubleFixed);
	assert(0 < lAltezzaTotale);
	return(lAltezzaTotale);
} // PrendiAltezzaTotaleListaDefunti

/* ------------------------------------------------------------------------ *

	InserisciInfoLunghezzaPagina

* ------------------------------------------------------------------------ */
static void XTAPI  InserisciInfoLunghezzaPagina(defunto *ptrdefunto, int16 numerodefunti, Fixed altezzacolonna) throw()
{
	// per scorrere i necrologi
	int16 lIndice = 0;
	// per scorrere l'array
	defunto *lPtrDefunto = NULL;
	// per calcolare l'altezza
	DoubleFixed lAltezzaDefuntoDoubleFixed = 0;
	// per prenbdere il numero di necrologi
	int lNumeroNecrologi = 0;
	
	// scorro tutti i defunti
	for (lIndice = 0; lIndice < numerodefunti; lIndice++) 
	{
		lPtrDefunto = ptrdefunto + lIndice;
		lAltezzaDefuntoDoubleFixed = (*lPtrDefunto).altezzaTotaleDoubleFixed;
		lNumeroNecrologi = PrendiNumeroNecrologiDefunto(lPtrDefunto);
		
		if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE)
			lAltezzaDefuntoDoubleFixed += (DoubleFixed) (lNumeroNecrologi * kFixedMinimoSpazioTraNecrologi);
		else
			lAltezzaDefuntoDoubleFixed += (DoubleFixed) (lNumeroNecrologi * gXtPreferences.giustificazionePref.valoreIniziale);
		
		// uchar tmpstr[kDimensioneStringhe] = "";
		// wsprintf((char*)tmpstr, "Errore di divisione per zero.\nSorgente: %s, Linea: %d.",__FILE__, __LINE__);
		assert(altezzacolonna + 1);

		(*lPtrDefunto).numeroColonne = (int) (lAltezzaDefuntoDoubleFixed / (DoubleFixed) (altezzacolonna + 1));
	}
} // InserisciInfoLunghezzaPagina

/* ------------------------------------------------------------------------ *

	InserisciInfoLunghezzaPaginaNecrologi

* ------------------------------------------------------------------------ */
void XTAPI InserisciInfoLunghezzaPaginaNecrologi() throw()
{
	// per prendere l'altezza della pagina ottimale
	Fixed lAltezzaColonnaOttimale = 0;
	
	lAltezzaColonnaOttimale = PrendiAltezzaColonnaOttimale();
	
	// lavoro sugli annunci
	InserisciInfoLunghezzaPagina(gPtrAnnunci, gNumeroAnnunci, lAltezzaColonnaOttimale);
	
	// lavoro sugli anniversari
	InserisciInfoLunghezzaPagina(gPtrAnniversari, gNumeroAnniversari, lAltezzaColonnaOttimale);

	// lavoro sui ringraziamenti
	InserisciInfoLunghezzaPagina(gPtrRingraziamenti, gNumeroRingraziamenti, lAltezzaColonnaOttimale);
	
} // InserisciInfoLunghezzaPaginaNecrologi

/* ------------------------------------------------------------------------ *

	ContaRigori

* ------------------------------------------------------------------------ */
static int16 XTAPI ContaRigori(defunto *ptrdefunto, int16 numerodefunti, tiporigore rigore) throw()
{
	// per scorrere i necrologi
	int16 lIndice = 0;
	// per scorrere l'array
	defunto *lPtrDefunto = NULL;
	// contatore del rigore scelto
	int16 lContatoreRigori = 0;
	
	// scorro tutti i defunti
	for (lIndice = 0; lIndice < numerodefunti; lIndice++) 
	{
		lPtrDefunto = ptrdefunto + lIndice;	
		if ((*lPtrDefunto).tipoRigore == rigore) 
			lContatoreRigori++;
	}	
	return(lContatoreRigori);
} // ContaRigori

/* ------------------------------------------------------------------------ *

	ContaColonneDefuntiLunghi

* ------------------------------------------------------------------------ */
static int16 ContaColonneDefuntiLunghi(defunto *ptrdefunto, int16 numerodefunti) throw()
{
	// per scorrere i necrologi
	int16 lIndice = 0;
	// per scorrere l'array
	defunto *lPtrDefunto = NULL;
	// contatore delle colonne dei defunti lunghi
	int lContatoreColonneDefuntiLunghi = 0;
	
	// scorro tutti i defunti
	for (lIndice = 0; lIndice < numerodefunti; lIndice++) 
	{
		lPtrDefunto = ptrdefunto + lIndice;
		
		if ((*lPtrDefunto).tipoRigore == kNessunRigore &&
			(*lPtrDefunto).numeroColonne > 1) 
		{
			lContatoreColonneDefuntiLunghi += (*lPtrDefunto).numeroColonne;
		}
	}
	
	return(lContatoreColonneDefuntiLunghi);
} // ContaColonneDefuntiLunghi

/* ------------------------------------------------------------------------ *

	ControlloRigoreNecrologi

* ------------------------------------------------------------------------ */
errorixtension XTAPI ControllaRigoreNecrologi() throw()
{
	// per contare i defunti con rigore
	int16 lNumeroRigori = 0;
	// per contare i defunti con rigore di documento
	int16 lNumeroRigoreDocumento = 0;
	// per prendere l'altezza totale di tutti i defunti di uno stesso tipo
	uint32 lAltezzaTotale = 0;
	// per contare il numero delle colonne libere
	int16 lNumeroColonne = 0;
	// per prendere lo spazio ottimale
	uint32 lSpazioOttimale = 0;
	// per scorrere le colonne
	int16 lColonna = 0;
	// per scorrere le pagine
	int16 lPagina = 0;
	// per prendere lo spazio colonna
	spaziocolonna *lPtrSpazioColonna = NULL;
	// contatore dei defunti pi— lunghi di una colonna
	int16 lNumeroColonneDefuntiLunghi = 0;

	// conto i rigori di documento per gli anniversari
	lNumeroRigori = ContaRigori(gPtrAnniversari, gNumeroAnniversari, kRigoreDiDocumento);
	if (lNumeroRigori > 0) 
	{
		// non e' possibile che gli anniversari abbiano un rigore di documento
		return(kErroreRigoreAnniversari);
	}

	// conto i rigori di documento per i rigraziamenti
	lNumeroRigori = ContaRigori(gPtrRingraziamenti, gNumeroRingraziamenti, kRigoreDiDocumento);
	if (lNumeroRigori > 0) 
	{
		// non e' possibile che i ringraziamenti abbiano un rigore di documento
		return(kErroreRigoreRingraziamenti);
	}

	// conto i rigori di documento per gli annunci
	lNumeroRigoreDocumento = ContaRigori(gPtrAnnunci, gNumeroAnnunci, kRigoreDiDocumento);
	if (lNumeroRigoreDocumento > 1) 
	{
		// non e' possibile che gli annunci abbiano piu' di un rigore di documento
		return(kErroreRigoreAnnunci);
	}

	// controllo sul rigore degli anniversari
	lAltezzaTotale = PrendiAltezzaTotaleDefunti(gPtrAnniversari, gNumeroAnniversari);
	
	// cerco la prima colonna libera partendo dal fondo
	gErroreXtension = PrendiUltimaColonna(&lPagina, &lColonna);
	if (gErroreXtension != kNessunErrore) 
	{
		// non e' possibile trovare una colonna libera
		return(gErroreXtension);
	}
		
	// conto le colonne per gli anniversari
	for (lPagina = lPagina; lPagina > 0; lPagina--)
	{
		for (lColonna = lColonna; lColonna > 0; lColonna--)
		{
			// prendo lo spazio della colonna corrente
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);
			lSpazioOttimale = mLong((*lPtrSpazioColonna).spazioOttimale);
			if (lAltezzaTotale < lSpazioOttimale) 
				break;

			// incremento il contatore delle colonne
			lNumeroColonne++;

			lAltezzaTotale -= lSpazioOttimale;
		}

		if (lAltezzaTotale < lSpazioOttimale) break;
		lColonna = PrendiNumeroColonne();
	}

	// conto i rigore di colonna per gli anniversari
	lNumeroRigori = ContaRigori(gPtrAnniversari, gNumeroAnniversari, kRigoreDiColonna);
	if (lNumeroRigori > lNumeroColonne) 
	{
		// anniversari con troppi rigore di documento
		sprintf((char*)gStringaC, "numero rigori %d, numero colonne %d",
				lNumeroRigori, lNumeroColonne);
		ConcatenaAllaStringaErrore(gStringaC);
		return(kErroreTroppiRigoriAnniversari);
	}

	// inizializzo il conatore delle colonne
	lNumeroColonne = 0;

	// controllo sul rigore dei ringraziamenti
	lAltezzaTotale += PrendiAltezzaTotaleDefunti(gPtrRingraziamenti, gNumeroRingraziamenti);

	// conto le colonne per i ringraziamenti
	for (lPagina = lPagina; lPagina > 0; lPagina--)
	{
		for (lColonna = lColonna; lColonna > 0; lColonna--)
		{
			// prendo lo spazio della colonna corrente
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);
			lSpazioOttimale = mLong((*lPtrSpazioColonna).spazioOttimale);

			if (lAltezzaTotale < lSpazioOttimale) 
				break;

			// incremento il contatore delle colonne
			lNumeroColonne++;

			lAltezzaTotale -= lSpazioOttimale;
		}

		if (lAltezzaTotale < lSpazioOttimale) 
			break;
		lColonna = PrendiNumeroColonne();
	}

	// conto i rigore di colonna per i ringraziamenti
	lNumeroRigori = ContaRigori(gPtrRingraziamenti, gNumeroRingraziamenti, kRigoreDiColonna);
	if (lNumeroRigori > lNumeroColonne) 
	{
		// ringraziamenti con troppi rigore di documento
		sprintf((char*)gStringaC, "numero rigori %d, numero colonne %d",
				lNumeroRigori, lNumeroColonne);
		ConcatenaAllaStringaErrore(gStringaC);
		return(kErroreTroppiRigoriRingraziamenti);
	}

	// controllo sul rigore degli annunci:
	// inizializzo il contatore delle colonne
	lNumeroColonne = 0;

	// conto le colonne per gli annunci
	for (lPagina = lPagina; lPagina > 0; lPagina--) 
	{
		for (lColonna = lColonna; lColonna > 0; lColonna--) 
		{
			// prendo lo spazio della colonna corrente
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);

			if ((*lPtrSpazioColonna).spazioOttimale == 0) 
				continue;

			// incremento il contatore delle colonne
			lNumeroColonne++;
		}
		lColonna = PrendiNumeroColonne();
	}

	// conto i rigore di colonna per gli annunci
	lNumeroRigori = ContaRigori(gPtrAnnunci, gNumeroAnnunci, kRigoreDiColonna);

	// aggiungo l'eventuale rigore di documento
	lNumeroRigori += lNumeroRigoreDocumento;

	// conto gli annunci senza rigore pi— lunghi di una colonna
	lNumeroColonneDefuntiLunghi = ContaColonneDefuntiLunghi(gPtrAnnunci, gNumeroAnnunci);

	if ((lNumeroRigori + lNumeroColonneDefuntiLunghi) > lNumeroColonne)
	{
		//annunci con troppi rigore di documento

		sprintf((char*)gStringaC, "col. necessarie tra rigori e necr. lunghi %d, col. disp. %d",
				lNumeroRigori + lNumeroColonneDefuntiLunghi, lNumeroColonne);
		ConcatenaAllaStringaErrore(gStringaC);
		return(kErroreTroppiRigoriAnnunci);
	}

	if (lNumeroColonneDefuntiLunghi > lNumeroColonne)
	{
		sprintf((char*)gStringaC, "numero colonne necessarie %d, numero colonne %d",
				lNumeroColonneDefuntiLunghi, lNumeroColonne);
		ConcatenaAllaStringaErrore(gStringaC);
		return(kErroreTroppiAnnunciLunghi);
	}

	return(kNessunErrore);
} // ControlloRigoreNecrologi

/* ------------------------------------------------------------------------ *

	PrendiNumeroNecrologi

* ------------------------------------------------------------------------ */
static int16 PrendiNumeroNecrologi(necrologio *ptrnecrologi) throw()
{
	// per scorrere le liste dei necrologi
	necrologio *lPtrNecrologio = NULL;
	// contatore dei necrologi 
	int16 lNumeroNecrologi = 0;
	
	// imposto il puntatore alla lista dei necrologi
	lPtrNecrologio = ptrnecrologi;
	
	// inizializzo il contatore
	lNumeroNecrologi = 0;
		
	// conto i necrologi
	while (1) 
	{
		// controllo se ci sono elementi nella lista
		if (lPtrNecrologio == NULL) 
			break;
		
		// avanzo nella lista
		lPtrNecrologio = (*lPtrNecrologio).successivo;
		
		// incremento il contatore
		lNumeroNecrologi++;
	}

	return(lNumeroNecrologi);
} // PrendiNumeroNecrologi

/* ------------------------------------------------------------------------ *

	PrendiNumeroNecrologiDefunto

* ------------------------------------------------------------------------ */
int16 XTAPI  PrendiNumeroNecrologiDefunto(defunto *ptrdefunto) throw()
{
	// controllo se ci sono elementi nella lista
	if (ptrdefunto == NULL) 
		return(0);
	
	return((*ptrdefunto).numeroNecrologi);
} // PrendiNumeroNecrologiDefunto