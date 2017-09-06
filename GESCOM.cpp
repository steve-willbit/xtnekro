/* ------------------------------------------------------------------------ *

	GestioneComandi.cpp

	14 Aprile 2005 				Fabrizio

	Copyright © Sinedita S.r.l 2005. All Rights Reserved.

	Descrizione:
	Contiene le procedure usate per la gestione dei comandi di impaginazione

	$Log: not supported by cvs2svn $
	Revision 1.2  2005/05/04 12:34:51  taretto
	comandi posiziona e ripristino ok
	
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
#include "Box.h"
#include "Coord.h"
#include "Errori.h"
#include "GesSlug.h"
#include "Immagini.h"
#include "InfoNec.h"
#include "Justify.h"
#include "Pref.h"
#include "ReadFile.h"
#include "ResStr.h"
#include "SpaceCol.h"
#include "Stili.h"
#include "Tempo.h"
#include "Testo.h"

#include "GesCom.h"

// STATIC GLOBALS
// STATIC GLOBALS
// STATIC GLOBALS

// per ricordare il codice defunto
static uchar gStrCodiceDefunto[kDimensioneStringhe] = "";
// per ricordare il codice necrologio
static uchar gStrCodiceNecrologio[kDimensioneStringhe] = "";
// per ricordare il tipo di rigore
static tiporigore gTipoRigore = kNessunRigore;
// lunghezza codice defunto
static Size gLunghezzaCodiceDefunto = 0;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			InserisciCodiceDefunto
	@abstract 		gestione comandi impaginazione
	@discussion		Crea il box di testo con il codice defunto

					18 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
static void XTAPI InserisciCodiceDefunto() throw();

/*!
	@function			ControllaAltezzaBozzaNuovaPagina
	@abstract 		gestione comandi impaginazione
	@discussion		Controllo che l'altezza del box precedentemente generato sia giusta

					14 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
static void XTAPI ControllaAltezzaBozzaNuovaPagina() throw();

/*!
	@function			ControllaAltezzaBozza
	@abstract 		gestione comandi impaginazione
	@discussion		Controllo che l'altezza del box precedentemente generato sia giusta

					14 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
static void XTAPI ControllaAltezzaBozza() throw();

/*!
	@function			GestioneNecrologioBozza
	@abstract 		gestione comandi impaginazione
	@discussion		Esegue tutte le operazioni comuni nella gestione di un necrologio.

					14 Aprile 2005 - Fabrizio.

	@param 			ptrquattrod puntatore alle info del box da creare
	@result  			nessuno
*/
static void XTAPI GestioneNecrologioBozza(quattrod *ptrquattrod) throw();

/*!
	@function			GestioneImmagineBozza
	@abstract 		gestione comandi impaginazione
	@discussion		Genera un box di testo per rappresentare l'ingombro dell'immagine
					da importare nell'impaginazione definitiva

					14 Aprile 2005 - Fabrizio.

	@param 			ptrquattrod puntatore alle info del box da creare
	@result  			nessuno
*/
static void XTAPI GestioneImmagineBozza(quattrod *ptrquattrod) throw();

/*!
	@function			ControllaAltezzaIngombro
	@abstract 		gestione comandi impaginazione
	@discussion		Controllo che l'altezza del box precedentemente generato sia giusta

					14 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
static void XTAPI ControllaAltezzaIngombro() throw();

/*!
	@function			ControllaAltezzaIngombro
	@abstract 		gestione comandi impaginazione
	@discussion		Esegue tutte le operazioni comuni nella gestione di un necrologio.

					14 Aprile 2005 - Fabrizio.

	@param 			ptrquattrod puntatore alle info del box da creare
	@result  			nessuno
*/
static void XTAPI GestioneNecrologioIngombro(quattrod *ptrquattrod) throw();

/*!
	@function			ImpostaSlugDiCodice
	@abstract 		gestione comandi impaginazione
	@discussion		Imposta lo slug del codice sul box corrente e gestisce anche
					l'eventuale errore dell'xtension

					14 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
static void XTAPI ImpostaSlugDiCodice() throw();

/*!
	@function			PreparaSlugDiAppWare
	@abstract 		gestione comandi impaginazione
	@discussion		Prepara lo slug di appware sul box corrente e gestisce anche
					l'eventuale errore dell'xtension

					14 Aprile 2005 - Fabrizio.

	@param 			ptrquattrod puntatore alle info da inserire nello slug del box corrente
	@param			ptrappware puntatore alla struttura da associare al box
	@result  			nessuno
*/
static void XTAPI PreparaSlugDiAppWare(quattrod *ptrquattrod, appware *ptrappware) throw();

/*!
	@function			ImpostaSlugDiAppWare
	@abstract 		gestione comandi impaginazione
	@discussion		Imposta lo slug di appware sul box corrente e gestisce anche
					l'eventuale errore dell'xtension

					14 Aprile 2005 - Fabrizio.

	@param 			ptrappware puntatore puntatore alle info che costituisco lo slug di AppWare
	@result  			nessuno
*/
static void XTAPI ImpostaSlugDiAppWare(appware *ptrappware) throw();

/*!
	@function			ControllaAltezzaDefinitivoSequenza
	@abstract 		gestione comandi impaginazione
	@discussion		Controllo che l'altezza del box precedentemente generato sia giusta

					14 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
static void XTAPI ControllaAltezzaDefinitivoSequenza() throw();

/*!
	@function			GestioneNecrologioDefinitivoSequenza
	@abstract 		gestione comandi impaginazione
	@discussion		Esegue tutte le operazioni comuni nella gestione di un necrologio.

					14 Aprile 2005 - Fabrizio.

	@param 			ptrquattrod  puntatore alle info del box da creare
	@result  			nessuno
*/
static void XTAPI GestioneNecrologioDefinitivoSequenza(quattrod *ptrquattrod) throw();

/*!
	@function			GestioneNecrologioDefinitivoManuale
	@abstract 		gestione comandi impaginazione
	@discussion		Esegue tutte le operazioni comuni nella gestione di un necrologio. Genera il
					box, gli slug associati e fa i controlli necessari sull'altezza del box
					precedentemente generato

					14 Aprile 2005 - Fabrizio.

	@param 			ptrquattrod puntatore alle info del box da creare
	@param			ptrappware per generare le info sia per il posizionamento
	@result  			nessuno
*/
static void XTAPI GestioneNecrologioDefinitivoManuale(quattrod *ptrquattrod) throw();

/*!
	@function			GestioneNecrologioDefinitivoAutomatico
	@abstract 		gestione comandi impaginazione
	@discussion		Esegue tutte le operazioni comuni nella gestione di un necrologio. Genera il
					box, gli slug associati e fa i controlli necessari sull'altezza del box
					precedentemente generato

					14 Aprile 2005 - Fabrizio.

	@param 			ptrquattrod puntatore alle info del box da creare
	@param			ptrappware per generare le info sia per il posizionamento
	@result  			nessuno
*/
static void XTAPI GestioneNecrologioDefinitivoAutomatico(quattrod *ptrquattrod, appware *ptrappware) throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	InserisciCodiceDefunto

* ------------------------------------------------------------------------ */
static void XTAPI InserisciCodiceDefunto() throw()
{
	// crea il box di testo per il codice del defunto 
	CreaBoxCodiceDefunto();

	// applico lo stile per il codice defunto 
	ApplicaStileForte(gStrDefunto);
	
	// inserisco la data e l'ora di stampa
	lstrcpy((char*)gStringaC, (char*)PrendiData());
	InserisciTesto(gStringaC, lstrlen((char*)gStringaC));

	// inserisco il codice defunto e il codice necrologio
	InserisciTesto(gStrCodiceDefunto, gLunghezzaCodiceDefunto);
	
	// modifico l'altezza del box del codice del defunto */
	VerificaAltezzaBoxCodiceDefunto();
	
} // InserisciCodiceDefunto

/* ------------------------------------------------------------------------ *

	ControllaAltezzaBozzaNuovaPagina

* ------------------------------------------------------------------------ */
static void XTAPI ControllaAltezzaBozzaNuovaPagina() throw()
{
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);

	// controllo che ci sia un box di testo corrente
	if (istextbox(curbox) == TRUE) 
	{
		// controlla le coordinate per eseguire un eventuale incremento
		// del bottom del box
		VerificaAltezzaBoxTestoBozza();
		
		// aggiorna le coordinate per il nuovo box da creare
		AggiornaCoordinate();

		// continua l'impaginazione su una nuova pagina
		InserisciNuovaPagina();
		
		// inserisco il codice del defunto nella pagina
		InserisciCodiceDefunto();
			
		// per evitare di ricontrollare le coordinate del box appena creato
		xtset_curbox(NULL);
	} 
	else 
	{
		// se sono sulla prima pagina non ho ancora creato il box con il codice
		// defunto
		if (PrendiPaginaCorrente() == kPrimaPagina) 
		{
			InserisciCodiceDefunto();
			xtset_curbox(NULL);	
		}
	}
} // ControllaAltezzaBozzaNuovaPagina

/* ------------------------------------------------------------------------ *

	ControllaAltezzaBozza

* ------------------------------------------------------------------------ */
static void XTAPI ControllaAltezzaBozza() throw()
{
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	// controllo che ci sia un box di testo corrente
	if (istextbox(curbox) == TRUE) 
	{
		// controlla le coordinate per eseguire un eventuale incremento
		// del bottom del box
		VerificaAltezzaBoxTestoBozza();
		
		// aggiorna le coordinate per il nuovo box da creare
		AggiornaCoordinate();
	}
} // ControllaAltezzaBozza

/* ------------------------------------------------------------------------ *

	GestioneNecrologioBozza

* ------------------------------------------------------------------------ */
static void XTAPI GestioneNecrologioBozza(quattrod *ptrquattrod) throw()
{
	// crea il box di testo per necrologio
	CreaBoxTesto();
	
	// incremento il contatore dei box nella colonna
	IncrementaContatoreBoxInColonna();
	
	// applico lo stile per il codice necologio
	ApplicaStileForte(gStrNecrologio);
	
	// inserisco il codice defunto
	InserisciTesto(ptrquattrod->testo, ptrquattrod->lunghezzaTesto);
	InserisciTesto((uchar*)"\r", 1L);
} // GestioneNecrologioBozza

/* ------------------------------------------------------------------------ *

	GestioneImmagineBozza

* ------------------------------------------------------------------------ */
static void XTAPI GestioneImmagineBozza(quattrod *ptrquattrod) throw()
{
	// prendo box corrente
	// serve per ricordare il box di testo corrente durante l'importazione dell'immagine
	boxid lIdBoxTestoCorrente = 0;
	xtget_curbox(&lIdBoxTestoCorrente);

	// creo un box di testo delle dimensioni dell'immagine con il nome
	// dell'immagine al centro
	CreaBoxTestoAncorato();
	
	// applico lo stile per l'immagine
	ApplicaStileForte(gStrStileImmagine);
	
	// inserisco il nome dell'immagine
	InserisciTesto(ptrquattrod->testo, ptrquattrod->lunghezzaTesto);
	InserisciTesto((uchar*)"\r", 1L);
	
	xtset_curbox(lIdBoxTestoCorrente);
} // GestioneImmagineBozza

/* ------------------------------------------------------------------------ *

	ControllaAltezzaIngombro
	
* ------------------------------------------------------------------------ */
static void XTAPI ControllaAltezzaIngombro() throw()
{
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);

	// controllo che ci sia un box di testo corrente
	if (istextbox(curbox) == TRUE) 
	{
		// controlla le coordinate per eseguire un eventuale incremento
		// del bottom del box
		VerificaAltezzaBoxTestoIngombro();
	}
} // ControllaAltezzaIngombro

/* ------------------------------------------------------------------------ *

	GestioneNecrologioIngombro

* ------------------------------------------------------------------------ */
static void XTAPI GestioneNecrologioIngombro(quattrod *ptrquattrod) throw()
{
	// salvo il codice del necrologio
	STRCPY(gStrCodiceNecrologio, ptrquattrod->testo);
	
	// controllo l'altezza del box generato precedentemente
	ControllaAltezzaIngombro();
	
	// crea il box di testo per necrologio
	CreaBoxTesto();
} // GestioneNecrologioIngombro

/* ------------------------------------------------------------------------ *

	ImpostaSlugDiCodice

* ------------------------------------------------------------------------ */
static void XTAPI ImpostaSlugDiCodice() throw()
{
	// imposto il codice al box appena creato
	gErroreXtension = SlugDiCodice();
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nell'assegnare lo slug al box corrente
		MostraErrore(gErroreXtension);
	}
} // ImpostaSlugDiCodice

/* ------------------------------------------------------------------------ *

	PreparaSlugDiAppWare

* ------------------------------------------------------------------------ */
static void XTAPI PreparaSlugDiAppWare(quattrod *ptrquattrod, appware *ptrappware) throw()
{
	// preparo le info per lo slug di appware
	InizializzaAppWare(ptrappware);
	ImpostaTipoAppWare(ptrappware, (tipoappware) ptrquattrod->tipoComando);
	ImpostaCodiceDefunto(ptrappware, gStrCodiceDefunto);
	ImpostaCodiceNecrologio(ptrappware, gStrCodiceNecrologio);
	ImpostaTipoRigore(ptrappware, gTipoRigore);
	ImpostaTipoGruppo(ptrappware, ptrquattrod->tipoGruppo);
	ImpostaPosizioneNelGruppo(ptrappware, ptrquattrod->posizioneNelGruppo);
	ImpostaCodiceBox(ptrappware, PrendiCodiceBox());	
} // PreparaSlugDiAppWare

/* ------------------------------------------------------------------------ *

	ImpostaSlugDiAppWare

* ------------------------------------------------------------------------ */
static void XTAPI ImpostaSlugDiAppWare(appware *ptrappware) throw()
{
	// altezza del box corrente
	Fixed lAltezzaBox = 0;
	// spazio tra i paragrafi
	Fixed lSpazioTraParagrafi = 0;
	// numero paragrafi con spazio diverso da zero */
	int32 lNumeroParagrafiDiversi = 0;
	// numero paragrafi
	int32 lNumeroParagrafi = 0;
	// numero linee
	int32 lNumeroLinee = 0;
	// allargamento
	Fixed lAllargamento = 0;
	// restrigimento
	Fixed lRestringimento = 0;
	
	// imposto l'altezza box 
	lAltezzaBox = PrendiAltezzaBox();
	ImpostaAltezzaBox(ptrappware, lAltezzaBox);
	
	// imposto lo spazio tra i paragrafi
	lSpazioTraParagrafi = PrendiSpazioTraParagrafi();
	ImpostaSpazioTraParagrafi(ptrappware, lSpazioTraParagrafi);
	
	// imposto il numero di paragrafi diversi da zero
	lNumeroParagrafiDiversi = PrendiNumeroParagrafiDiversi();
	ImpostaNumeroParagrafiDiversi(ptrappware, lNumeroParagrafiDiversi);
	
	// imposto il numero di paragrafi
	lNumeroParagrafi = PrendiNumeroParagrafi();
	ImpostaNumeroParagrafi(ptrappware, lNumeroParagrafi);
	
	// imposto il numero di linee
	lNumeroLinee = PrendiNumeroLinee();
	ImpostaNumeroLinee(ptrappware, lNumeroLinee);
	
	// imposto l'allargamento
	lAllargamento = gXtPreferences.giustificazionePref.incrementoMaxSpazioParagrafi * lNumeroParagrafi;
	lAllargamento += gXtPreferences.giustificazionePref.incrementoMaxInterlinea * lNumeroLinee;
	if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE)
		lAllargamento += kFixedDifferenzaSpazioTraNecrologi;
	IncrementaAllargamentoTotale(lAllargamento);
	ImpostaAllargamento(ptrappware, lAllargamento);
		
	// imposto il restringimento
	lRestringimento = gXtPreferences.giustificazionePref.decrementoMaxSpazioParagrafi * lNumeroParagrafiDiversi;
	if (lRestringimento > lSpazioTraParagrafi) 
		lRestringimento = lSpazioTraParagrafi;
	lRestringimento += gXtPreferences.giustificazionePref.decrementoMaxInterlinea * lNumeroLinee;
	IncrementaRestringimentoTotale(lRestringimento);
	ImpostaRestringimento(ptrappware, lRestringimento);
	
	// imposto il collegamento
	ImpostaCollegato(ptrappware, FALSE);

	// imposto lo slug di appware al box appena creato
	gErroreXtension = SlugDiAppWare(ptrappware);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nell'assegnare lo slug al box corrente
		MostraErrore(gErroreXtension);
	}	
} // ImpostaSlugDiAppWare

/* ------------------------------------------------------------------------ *

	ControllaAltezzaDefinitivoSequenza

* ------------------------------------------------------------------------ */
static void XTAPI ControllaAltezzaDefinitivoSequenza() throw()
{
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	// controllo che ci sia un box di testo corrente
	if (istextbox(curbox) == TRUE) 
	{
		// controlla le coordinate per eseguire un eventuale incremento
		// del bottom del box
		VerificaAltezzaBoxTestoDefinitivoSequenza();
		
		// aggiorna le coordinate per il nuovo box da creare
		AggiornaCoordinate();
	}
} // ControllaAltezzaDefinitivoSequenza

/* ------------------------------------------------------------------------ *

	GestioneNecrologioDefinitivoSequenza

* ------------------------------------------------------------------------ */
static void XTAPI GestioneNecrologioDefinitivoSequenza(quattrod *ptrquattrod) throw()
{
	// crea il box di testo per necrologio
	CreaBoxTesto();
	
	// incremento il contatore dei box nella colonna
	IncrementaContatoreBoxInColonna();
} // GestioneNecrologioDefinitivoSequenza

/* ------------------------------------------------------------------------ *

	GestioneNecrologioDefinitivoManuale

* ------------------------------------------------------------------------ */
static void XTAPI  GestioneNecrologioDefinitivoManuale(quattrod *ptrquattrod) throw()
{
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	// salvo il codice del necrologio
	STRCPY(gStrCodiceNecrologio, ptrquattrod->testo);
	
	if (istextbox(curbox) == TRUE) 
	{
		// controlla l'altezza del box generato precedentemente
		VerificaAltezzaBoxTestoDefinitivoManuale();
	}

	// crea il box di testo per necrologio
	CreaBoxTesto();
} // GestioneNecrologioDefinitivoManuale

/* ------------------------------------------------------------------------ *

	GestioneNecrologioDefinitivoAutomatico

* ------------------------------------------------------------------------ */
static void XTAPI GestioneNecrologioDefinitivoAutomatico(quattrod *ptrquattrod, appware *ptrappware) throw()
{
		// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	// salvo il codice del necrologio
	STRCPY(gStrCodiceNecrologio, ptrquattrod->testo);
	
	if (istextbox(curbox) == TRUE) 
	{
		// controlla l'altezza del box generato precedentemente
		VerificaAltezzaBoxTestoDefinitivoAutomatico(ptrappware);
		
		// imposto lo slug di AppWare
		ImpostaSlugDiAppWare(ptrappware);
				
		// inserisce le info nella struttura per fare il posizionamento
		InserisciInfoNecrologio(ptrappware);
	}

	// crea il box di testo per necrologio
	CreaBoxTesto();
	
	// imposto lo slug del codice
	ImpostaSlugDiCodice();
	
	// inizio a preparare lo slug di AppWare	
	PreparaSlugDiAppWare(ptrquattrod, ptrappware);

} // GestioneNecrologioDefinitivoAutomatico

/* ------------------------------------------------------------------------ *

	PrendiCodiceDefunto

* ------------------------------------------------------------------------ */
uchar* XTAPI PrendiCodiceDefunto() throw()
{
	return(gStrCodiceDefunto);
} // PrendiCodiceDefunto()

/* ------------------------------------------------------------------------ *

	PrendiCodiceNecrologio

* ------------------------------------------------------------------------ */
uchar* XTAPI PrendiCodiceNecrologio() throw()
{
	return(gStrCodiceNecrologio);
} // PrendiCodiceNecrologio

/* ------------------------------------------------------------------------ *
8
	GestioneComandiBozzaSingoli

* ------------------------------------------------------------------------ */
void XTAPI GestioneComandiBozzaSingoli(quattrod *ptrquattrod) throw()
{
	boxid lIdBoxTestoCorrente = 0;

	switch (ptrquattrod->tipoComando) 
	{
		case kTesto:
			InserisciTesto(ptrquattrod->testo, ptrquattrod->lunghezzaTesto);
			
			break;
		case kStileForte:
			ApplicaStileForte(ptrquattrod->testo);
			
			break;
		case kStileDebole:
			ApplicaStileDebole(ptrquattrod->testo);
			
			break;
		case kImmagine:
			GestioneImmagineBozza(ptrquattrod);
						
			break;
		
		case kPax:
			// salvo il box su cui sto lavorando
			boxid curbox;
			xtget_curbox(&curbox);
			lIdBoxTestoCorrente = curbox;

			//creo il box della Pax
			CopiaIncollaBoxPaxAncorato();

			// ripristina il box salvato
			xtset_curbox(lIdBoxTestoCorrente);
			
			break;	

		case kDefunto:
			// salvo il codice del defunto
			STRCPY(gStrCodiceDefunto, ptrquattrod->testo);
			gLunghezzaCodiceDefunto = ptrquattrod->lunghezzaTesto;
			// salvo il rigore del defunto
			gTipoRigore = ptrquattrod->tipoRigore;
			
			break;
		case kAnnuncio:
		case kAnniversario:
		case kRingraziamento:
			ControllaAltezzaBozzaNuovaPagina();
			GestioneNecrologioBozza(ptrquattrod);

			break;
		case kComandoDiFine:
			ControllaAltezzaBozza();

			break;
		case kNessunComando:
			break;
	}
} // GestioneComandiBozzaSingoli

/* ------------------------------------------------------------------------ *

	GestioneComandiBozzaAggregati

* ------------------------------------------------------------------------ */
void XTAPI GestioneComandiBozzaAggregati(quattrod *ptrquattrod) throw()
{
	boxid lIdBoxTestoCorrente = 0;

	switch (ptrquattrod->tipoComando) 
	{
		case kTesto:
			InserisciTesto(ptrquattrod->testo, ptrquattrod->lunghezzaTesto);
			
			break;
		case kStileForte:
			ApplicaStileForte(ptrquattrod->testo);
			
			break;
		case kStileDebole:
			ApplicaStileDebole(ptrquattrod->testo);
			
			break;
		case kImmagine:
			GestioneImmagineBozza(ptrquattrod);
			
			break;
			
		case kPax:		
			// salvo il box su cui sto lavorando
			boxid curbox;
			xtget_curbox(&curbox);
			lIdBoxTestoCorrente = curbox;

			//creo il box della Pax
			CopiaIncollaBoxPaxAncorato();

			// ripristina il box salvato
			xtset_curbox(lIdBoxTestoCorrente);
			
			break;	


		case kDefunto:
			// salvo il codice del defunto
			STRCPY(gStrCodiceDefunto, ptrquattrod->testo);
			gLunghezzaCodiceDefunto = ptrquattrod->lunghezzaTesto;
			// salvo il rigore del defunto
			gTipoRigore = ptrquattrod->tipoRigore;
			
			ControllaAltezzaBozzaNuovaPagina();

			break;
		case kAnnuncio:
		case kAnniversario:
		case kRingraziamento:
			ControllaAltezzaBozza();
			GestioneNecrologioBozza(ptrquattrod);

			break;
		case kComandoDiFine:
			ControllaAltezzaBozza();

			break;
		case kNessunComando:
			break;
	}
} // GestioneComandiBozzaAggregati

/* ------------------------------------------------------------------------ *

	GestioneComandiIngombro

* ------------------------------------------------------------------------ */
void XTAPI GestioneComandiIngombro(quattrod *ptrquattrod) throw()
{
	// serve per ricordare il box di testo corrente durante l'importazione dell'immagine
	boxid lIdBoxTestoCorrente = 0;
	
	switch (ptrquattrod->tipoComando) 
	{
		case kTesto:
			InserisciTesto(ptrquattrod->testo, ptrquattrod->lunghezzaTesto);
			
			break;
		case kStileForte:
			ApplicaStileForte(ptrquattrod->testo);
			
			break;
		case kStileDebole:
			ApplicaStileDebole(ptrquattrod->testo);
			
			break;
		case kImmagine:
			
			xtget_curbox(&lIdBoxTestoCorrente);
			CreaBoxImmagineAncorato();			
			xtset_curbox(lIdBoxTestoCorrente);
			
			break;
	
		case kPax:
			// salvo il box su cui sto lavorando
			boxid curbox;
			xtget_curbox(&curbox);
			lIdBoxTestoCorrente = curbox;

			//creo il box della Pax
			CopiaIncollaBoxPaxAncorato();

			// ripristina il box salvato
			xtset_curbox(lIdBoxTestoCorrente);
			
			break;	

		case kDefunto:
			// salvo il codice del defunto 
			STRCPY(gStrCodiceDefunto, ptrquattrod->testo);
			// salvo il rigore del defunto 
			gTipoRigore = ptrquattrod->tipoRigore;
			
			break;
		case kAnnuncio:
		case kAnniversario:
		case kRingraziamento:
			GestioneNecrologioIngombro(ptrquattrod);

			break;
		case kComandoDiFine:
			ControllaAltezzaIngombro();

			break;
		case kNessunComando:
			break;
	}
} // GestioneComandiIngombro

/* ------------------------------------------------------------------------ *

	GestioneComandiDefinitivoManuale

* ------------------------------------------------------------------------ */
void XTAPI GestioneComandiDefinitivoManuale(quattrod *ptrquattrod) throw()
{
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);

	// serve per ricordare il box di testo corrente durante l'importazione dell'immagine
	boxid lIdBoxTestoCorrente = 0;
	
	switch (ptrquattrod->tipoComando) 
	{
		case kTesto:
			InserisciTesto(ptrquattrod->testo, ptrquattrod->lunghezzaTesto);
			
			break;
		case kStileForte:
			ApplicaStileForte(ptrquattrod->testo);
			
			break;
		case kStileDebole:
			ApplicaStileDebole(ptrquattrod->testo);
			
			break;
		case kImmagine:
			xtget_curbox(&lIdBoxTestoCorrente);
			lIdBoxTestoCorrente = curbox;
			CreaBoxImmagineAncorato();
			// importo l'immagine
			if (gXtPreferences.immaginiPref.abilitaImportImag == TRUE) 
				ImportaImmagine(ptrquattrod->testo);
			else 
				SlugDiImmagine(ptrquattrod->testo);
			xtset_curbox(lIdBoxTestoCorrente);
			
			break;
			
		case kPax:
			// salvo il box su cui sto lavorando
			xtget_curbox(&curbox);
			lIdBoxTestoCorrente = curbox;

			//creo il box della Pax
			CopiaIncollaBoxPaxAncorato();

			// ripristina il box salvato
			xtset_curbox(lIdBoxTestoCorrente);
			break;	

		
		case kDefunto:
			if (istextbox(curbox) == TRUE) 
			{
				// controlla le coordinate per eseguire un eventuale incremento
				// del bottom del box
				VerificaAltezzaBoxTestoDefinitivoManuale();
				
				// imposto il box corrente a NULL per evitare di ripetere
				// queste operazioni 
				xtset_curbox(NULL); 
			}
			
			// salvo il codice del defunto
			STRCPY(gStrCodiceDefunto, ptrquattrod->testo);
			// salvo il rigore del defunto
			gTipoRigore = ptrquattrod->tipoRigore;
			
			break;
		case kAnnuncio:
		case kAnniversario:
		case kRingraziamento:
			GestioneNecrologioDefinitivoManuale(ptrquattrod);
			
			break;
		case kComandoDiFine:
			if (istextbox(curbox) == TRUE) 
			{
				// controlla le coordinate per eseguire un eventuale incremento
				// del bottom del box
				VerificaAltezzaBoxTestoDefinitivoManuale();

			}

			break;
		case kNessunComando:
			break;
	}
} // GestioneComandiDefinitivoManuale

/* ------------------------------------------------------------------------ *

	GestioneComandiDefinitivoSequenza

* ------------------------------------------------------------------------ */
void XTAPI GestioneComandiDefinitivoSequenza(quattrod *ptrquattrod) throw()
{
	// serve per ricordare il box di testo corrente durante l'importazione dell'immagine
	boxid lIdBoxTestoCorrente = 0;
	
	switch (ptrquattrod->tipoComando) 
	{
		case kTesto:
			InserisciTesto(ptrquattrod->testo, ptrquattrod->lunghezzaTesto);
			
			break;
		case kStileForte:
			ApplicaStileForte(ptrquattrod->testo);
			
			break;
		case kStileDebole:
			ApplicaStileDebole(ptrquattrod->testo);
			
			break;
		case kImmagine:
			xtget_curbox(&lIdBoxTestoCorrente);
			CreaBoxImmagineAncorato();

			// importo l'immagine
			if (gXtPreferences.immaginiPref.abilitaImportImag == TRUE) 
			{
			
				ImportaImmagine(ptrquattrod->testo);
			} 
			else 
			{
				gErroreXtension = SlugDiImmagine(ptrquattrod->testo);
				if (gErroreXtension != kNessunErrore) 
				{
					// errore nell'allocazione di uno slug 
					MostraErrore(gErroreXtension);
					return;
				}
			}

			xtset_curbox(lIdBoxTestoCorrente);
			
			break;
		
		case kPax:
			// salvo il box su cui sto lavorando
			boxid curbox;
			xtget_curbox(&curbox);
			lIdBoxTestoCorrente = curbox;

			//creo il box della Pax
			CopiaIncollaBoxPaxAncorato();

			// ripristina il box salvato
			xtset_curbox(lIdBoxTestoCorrente);
			
			break;	

		case kDefunto:
		
			// salvo il codice del defunto
			STRCPY(gStrCodiceDefunto, ptrquattrod->testo);
			gLunghezzaCodiceDefunto = ptrquattrod->lunghezzaTesto;
			// salvo il rigore del defunto
			gTipoRigore = ptrquattrod->tipoRigore;
			
			break;
		case kAnnuncio:
		case kAnniversario:
		case kRingraziamento:
			ControllaAltezzaDefinitivoSequenza();
			GestioneNecrologioDefinitivoSequenza(ptrquattrod);

			break;
		case kComandoDiFine:
			ControllaAltezzaDefinitivoSequenza();

			break;
		case kNessunComando:
			break;
	}
} // GestioneComandiDefinitivoSequenza

/* ------------------------------------------------------------------------ *

	GestioneComandiDefinitivoAutomatico

* ------------------------------------------------------------------------ */
void GestioneComandiDefinitivoAutomatico(quattrod *ptrquattrod, appware *ptrappware) throw()
{
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);

	// serve per ricordare il box di testo corrente durante l'importazione dell'immagine
	boxid lIdBoxTestoCorrente = 0;
	
	switch (ptrquattrod->tipoComando) 
	{
		case kTesto:
			InserisciTesto(ptrquattrod->testo, ptrquattrod->lunghezzaTesto);
			
			break;
		case kStileForte:
			ApplicaStileForte(ptrquattrod->testo);
			
			break;
		case kStileDebole:
			ApplicaStileDebole(ptrquattrod->testo);
			
			break;
		case kImmagine:
			lIdBoxTestoCorrente = curbox;
			CreaBoxImmagineAncorato();


			// importo l'immagine 
			if (gXtPreferences.immaginiPref.abilitaImportImag == TRUE) 
				ImportaImmagine(ptrquattrod->testo);
			else 
				SlugDiImmagine(ptrquattrod->testo);

			xtset_curbox(lIdBoxTestoCorrente);
			
			break;

		case kPax:
			// salvo il box su cui sto lavorando
			xtget_curbox(&curbox);
			lIdBoxTestoCorrente = curbox;

			//creo il box della Pax
			CopiaIncollaBoxPaxAncorato();

			// ripristina il box salvato
			xtset_curbox(lIdBoxTestoCorrente);
			
			break;	

		case kDefunto:
			if (istextbox(curbox) == TRUE) 
			{
				// controlla le coordinate per eseguire un eventuale incremento 
				// del bottom del box/
				VerificaAltezzaBoxTestoDefinitivoAutomatico(ptrappware);
				
				// imposto lo slug di AppWare
				ImpostaSlugDiAppWare(ptrappware);
				// inserisce le info nella struttura per fare il posizionamento
				InserisciInfoNecrologio(ptrappware);
				
				// imposto il box corrente a NULL per evitare di ripetere
				// queste operazioni
				xtset_curbox(NULL); 
			}
			
			// salvo il codice del defunto
			STRCPY(gStrCodiceDefunto, ptrquattrod->testo);
			// salvo il rigore del defunto
			gTipoRigore = ptrquattrod->tipoRigore;
			
			ImpostaNuovoDefunto();
			
			break;
		case kAnnuncio:
		case kAnniversario:
		case kRingraziamento:
			GestioneNecrologioDefinitivoAutomatico(ptrquattrod, ptrappware);
			
			break;
		case kComandoDiFine:
			if (istextbox(curbox) == TRUE) 
			{
				// controlla le coordinate per eseguire un eventuale incremento
				// del bottom del box
				VerificaAltezzaBoxTestoDefinitivoAutomatico(ptrappware);
				
				// imposto lo slug di AppWare
				ImpostaSlugDiAppWare(ptrappware);
				// inserisce le info nella struttura per fare il posizionamento
				InserisciInfoNecrologio(ptrappware);
			}

			break;
		case kNessunComando:
			break;
	}
} // GestioneComandiDefinitivoAutomatico
