/* ------------------------------------------------------------------------ *

	Impaginazione.cpp

	11 Aprile 2005 				Fabrizio

	Copyright © Sinedita S.r.l 2005. All Rights Reserved.

	Descrizione:
	Contiene le procedure usate per eseguire il ciclo dell'impaginazione
	dei necrologi

	$Log: not supported by cvs2svn $
	Revision 1.4  2005/05/09 07:40:56  taretto
	stampa bozze ok
	
	Revision 1.3  2005/05/04 12:34:51  taretto
	comandi posiziona e ripristino ok
	
	Revision 1.2  2005/04/21 09:48:19  taretto
	tolte mLong per gestire numero moduli e colonne, modificata estensione per salvataggi in QXP
	
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
/*
#include "ExPos.h"
*/
#include "AppWare.h"
#include "Box.h"
#include "Coord.h"
#include "DocInfo.h"
#include "Document.h"
#include "GesCom.h"
#include "GesSlug.h"
#include "Errori.h"
#include "Immagini.h"
#include "InfoNec.h"
#include "LastImp.h"
#include "Menu.h"
#include "Memoria.h"
#include "OpXt.h"
#include "Paletta.h"
#include "Pref.h"
#include "PresFile.h"
#include "ReadFile.h"
#include "ResStr.h"
#include "Ripristi.h"
#include "SpaceCol.h"
#include "Stili.h"
#include "StMes.h"
#include "Tempo.h"
#include "XTNecro.h"

#include "Impag.h"

// CONSTS
// CONSTS
// CONSTS

#define kLarghezzaFinestraIngombro 150
#define kAltezzaFinestraIngombro 100

// GLOBALS
// GLOBALS
// GLOBALS

uchar*gPtrDocumento = NULL;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			ImpaginaBozza
	@abstract 		gestione impaginazione.
	@discussion		Esegue l'impaginazione di bozza

					11 Aprile 2005 - Fabrizio.

	@param 			ptrdocumento puntatore al documento da impaginare
	@result  			rritorna l'eventuale errore dell'xtension
*/
static errorixtension XTAPI ImpaginaBozza(uchar** ptrdocumento) throw();

/*!
	@function			ImpaginazioneIngombro
	@abstract 		gestione impaginazione.
	@discussion		Esegue l'impaginazione per il calcolo dell'ingombro

					11 Aprile 2005 - Fabrizio.

	@param 			ptrdocumento puntatore al documento da impaginare
	@result  			ritorna l'eventuale errore dell'xtension
*/
static errorixtension XTAPI ImpaginaIngombro(uchar** ptrdocumento) throw();

/*!
	@function			ImpaginaDefinitivoManuale
	@abstract 		gestione impaginazione.
	@discussion		Esegue l'impaginazione definitiva con posizionamento in pagina
					dei necrologi manuale

					11 Aprile 2005 - Fabrizio.

	@param 			ptrdocumento puntatore al documento da impaginare
	@result  			ritorna l'eventuale errore dell'xtension
*/
static errorixtension XTAPI ImpaginaDefinitivoManuale(uchar** ptrdocumento) throw();

/*!
	@function			ImpaginaDefinitivoSequenza
	@abstract 		gestione impaginazione.
	@discussion		Esegue l'impaginazione definitiva con posizionamento in pagina
					dei necrologi manuale

					11 Aprile 2005 - Fabrizio.

	@param 			ptrdocumento puntatore al documento da impaginare
	@result  			ritorna l'eventuale errore dell'xtension
*/
static errorixtension XTAPI ImpaginaDefinitivoSequenza(uchar**ptrdocumento) throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	StampaIngombro

* ------------------------------------------------------------------------ */
void XTAPI StampaIngombro() throw()
{
	uint32 lIngombro = 0;
	uint32 lPagine = 0;
	uint32 lColonne = 0;
	uint32 lFrazioneModuli = 0;
	uint32 lModuli = 0;
	double lRapporto = 0.0;
	double lSpazioTraNecrologi = 0.0;

	// stringa d'utilitaì
	uchar tmpstr[kDimensioneStringhe] = "";
	
	// prendo lo spazio tra i paragrafi 
	lSpazioTraNecrologi = (double) PrendiSpazioTraNecrologiTotale();
	
	if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi)
	{
		// wsprintf((char*)tmpstr, "Errore di divisione per zero.\nSorgente: %s, Linea: %d.",__FILE__, __LINE__);
		// XtAssert(tmpstr, (0 != kFixedMinimoSpazioTraNecrologi));
		assert(0 != kFixedMinimoSpazioTraNecrologi);

		lRapporto = ((double) gXtPreferences.giustificazionePref.valoreIniziale / (double) kFixedMinimoSpazioTraNecrologi);
		lSpazioTraNecrologi =  lRapporto * lSpazioTraNecrologi;
	}

	// prendo l'ingombro
	lIngombro = PrendiIngombroTotale() + (long) lSpazioTraNecrologi;
	
	// salvo l'ingombro nelle preferenze
	 gLongIngombroTotale= lIngombro;
	
	// calcolo le pagine, colonne e moduli
	CalcolaPagineColonneEModuli(lIngombro,
								&lPagine,
								&lColonne,
								&lFrazioneModuli,
								&lModuli);

	wsprintf((char*)gStringaC, "%ld punti", lIngombro);
	ConcatenaAllaStringaMessaggio(gStringaC);
	DaiMessaggio(kIngombroInPunti);
	
	wsprintf((char*)gStringaC, "%ld moduli", lModuli);
	ConcatenaAllaStringaMessaggio(gStringaC);
	DaiMessaggio(kIngombroInModuli);

	if (lFrazioneModuli != 0) 
		wsprintf((char*)gStringaC, "pagine %ld colonne %ld moduli %ld", lPagine, lColonne, lFrazioneModuli);
	else 
		wsprintf((char*)gStringaC, "pagine %ld colonne %ld", lPagine, lColonne);	
	ConcatenaAllaStringaMessaggio(gStringaC);
	DaiMessaggio(kIngombroInPagineColonne);
	
} // StampaIngombro

/* ------------------------------------------------------------------------ *

	ImpaginaBozza

* ------------------------------------------------------------------------ */
static errorixtension XTAPI ImpaginaBozza(uchar** ptrdocumento) throw()
{
	// singola struttura da impaginare
	quattrod lQuattroD = {kNessunComando, NULL, 0, kNessunRigore, NULL, NULL};
	// percetuale di scala del documento
	Fixed lPercentuale = 0;
	// per costruire il nome da usare per salvare la bozza appena impaginata
	uchar lNomeDocumento[80] = "";
	// per sapere se l'impaginazione e' stata interotta
	Boolean lInterrotta = FALSE;
	
	// chiudo tutti i documenti aperti correntemente
	gErroreXtension = ChiudiTuttiIDocumenti(FALSE);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sulla chiusura di tutti i documenti
		return(gErroreXtension);
	}
		
	// apertura documento bozza
	gErroreXtension = ApriDocumento(gXtPreferences.bozzaPref.docPathBozza);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sull'apertura del documento bozza
		return(gErroreXtension);
	}
	
	if (IsPagineAffiancate() == TRUE) 
	{
		// se a pagine affiancate non impagino
		ConcatenaAllaStringaErrore(gXtPreferences.bozzaPref.docPathBozza);
		return(kErroreDocumentoPagineAffiancate);
	}

	// imposto la visualizzazione di impaginazione
	lPercentuale = kVistaImpaginazioneBozze;
	ImpostaScalaDocumento(&lPercentuale);

	// imposto le coordinate iniziali
	ImpostaCoordinateIniziali();
		
	// imposto il box immagine campione per il simbolo della Pax
	gErroreXtension = ImpostaImmagineCampionePax();
	if (gErroreXtension != kNessunErrore)
	{
		return(gErroreXtension);
	}

	
	if (gXtPreferences.bozzaPref.tipoStampaBozza == STAMPASINGOLARADIOBTNID) 
	{
		// impaginazione dei necrologi uno per pagina
		while (1) 
		{
			LeggiQuattroD(ptrdocumento, &lQuattroD);

			// esecuzione del comando di impaginazione
			 GestioneComandiBozzaSingoli(&lQuattroD);
			
			if (lQuattroD.tipoComando == kComandoDiFine) 
			{
				break;
			}
			
			// inizializzo lQuattroD
			InizializzaQuattroD(&lQuattroD);
		}
	} 
	else 
	{
		// impaginazione dei necrologi aggregati
		while (1) 
		{
			LeggiQuattroD(ptrdocumento, &lQuattroD);	
			// esecuzione del comando di impaginazione
			GestioneComandiBozzaAggregati(&lQuattroD);
			
			if (lQuattroD.tipoComando == kComandoDiFine) 
			{
				break;
			}
			
			// inizializzo lQuattroD
			InizializzaQuattroD(&lQuattroD);
		}
	}

	// torno sulla prima pagina
	gotopage(kPrimaPagina);

	// imposto la scala precedente
	ImpostaScalaDocumento(&lPercentuale);

	// costruisco il nome del documento
	sprintf((char*)lNomeDocumento,"%s%s", PrendiDataXNomeFile(), ".QXP");

	// salvataggio documento bozza appena impaginato */
	gErroreXtension = SalvaDocumento(/*PercorsoCompleto(*/gXtPreferences.bozzaPref.folderPathBozze/*)*/, lNomeDocumento);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sulla chiusura del documento bozza
		return(gErroreXtension);
	}
	
	if (lInterrotta == TRUE) 
	{
		// l'impaginazione e' stata interotta dall'utente
		ImpostaCoseDellUtente();
	} 
	else if (gXtPreferences.bozzaPref.stampaAutomatica == TRUE) 
	{
		// stampo il documento corrente
		StampaDocumento();
		// aspetto la fine della stampa
		ImpostaCoseDiStampa();
	}
	else 
	{
		// ritorno ad aspettare i file da 4D o fare le cose dell'utente
		if (kCheckAbilitaImpaginazione == TRUE) 
			ImpostaCoseDiQuattroD();
		else 
			ImpostaCoseDellUtente();
	}
		
	return(kNessunErrore);
} // ImpaginaBozza

/* ------------------------------------------------------------------------ *

	ImpaginazioneIngombro

* ------------------------------------------------------------------------ */
static errorixtension XTAPI ImpaginaIngombro(uchar** ptrdocumento) throw()
{
	// singola struttura da impaginare 
	quattrod lQuattroD = {kNessunComando, NULL, 0, kNessunRigore, NULL, NULL};
	// percetuale di scala del documento
	Fixed lPercentuale = 0;
	// per l'altezza della finestra del documento 
	int16 lAltezza = 0;
	// per la larghezza della finestra del documento
	int16 lLarghezza = 0;
	// per sapere se l'impaginazione e' stata interotta
	Boolean lInterrotta = FALSE;
		
	// chiudo tutti i documenti aperti correntemente
	gErroreXtension = ChiudiTuttiIDocumenti(FALSE);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sulla chiusura di tutti i documenti
		return(gErroreXtension);
	}
		
	// apertura documento ingombro
	gErroreXtension = ApriDocumento(gXtPreferences.ingombroPref.docPathIngombro);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sull'apertura del documento di ingombro
		return(gErroreXtension);
	}

	if (IsPagineAffiancate() == TRUE) 
	{
		// se a pagine affiancate non impagino
		ConcatenaAllaStringaErrore(gXtPreferences.bozzaPref.docPathBozza);
		return(kErroreDocumentoPagineAffiancate);
	}
	
	// imposto la visualizzazione di impaginazione
	lPercentuale = kVistaImpaginazioneIngombro;
	ImpostaScalaDocumento(&lPercentuale);

	// imposto la dimensione della finestra del documento
	lLarghezza = kLarghezzaFinestraIngombro;
	lAltezza = kAltezzaFinestraIngombro;
	
	// imposto le coordinate iniziali
	ImpostaCoordinateIniziali();
	
	// imposto a zero l'ingombro totale
	InizializzaIngombroTotale();
	
	// imposto a zero lo spazio tra i necrologi totale
	InizializzaSpazioTraNecrologiTotale();
	
	// imposto il box immagine campione
	ImpostaImmagineCampione();
	
	// imposto il box immagine campione per il simbolo della Pax
	gErroreXtension = ImpostaImmagineCampionePax();
	if (gErroreXtension != kNessunErrore)
	{
		return(gErroreXtension);
	}

	while (1) 
	{
		LeggiQuattroD(ptrdocumento, &lQuattroD);
		
		// esecuzione del comando di impaginazione
		GestioneComandiIngombro(&lQuattroD);
		
		if (lQuattroD.tipoComando == kComandoDiFine) 
		{
			break;
		}
		
		// inizializzo lQuattroD
		InizializzaQuattroD(&lQuattroD);
	}
	
	// visualizza i messaggi sull'ingombro appena calcolato
	StampaIngombro();
		
	// salvataggio documento ingombro appena impaginato 
	gErroreXtension = SalvaDocumento(/*PercorsoCompleto(*/gXtPreferences.ingombroPref.folderPathIngombri/*)*/,
									 gStrIngombro);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sul salvataggio del documento di ingombro
		return(gErroreXtension);
	}
	
	// chiusura del documento di ingombro
	ChiudiDocumento();
	
	// ritorno ad aspettare i file da 4D o fare le cose dell'utente
	if (lInterrotta == TRUE) 
	{
		// l'impaginazione e' stata interotta dall'utente
		ImpostaCoseDellUtente();
	} 
	else 
	{
		if (kCheckAbilitaImpaginazione == TRUE) 
			ImpostaCoseDiQuattroD();
		else ImpostaCoseDellUtente();
	}

	return(kNessunErrore);
} // ImpaginaIngombro	

static errorixtension XTAPI ImpaginaDefinitivoManuale(uchar** ptrdocumento) throw();
/* ------------------------------------------------------------------------ *

	ImpaginaDefinitivoManuale

	25 Novembre 1995						Stefano

	Descrizione:
	Esegue l'impaginazione definitiva con posizionamento in pagina
	dei necrologi manuale

	Parametri:
	ptrdocumento - puntatore al documento da impaginare

	Ritorno:
	ritorna l'eventuale errore dell'Xtension

* ------------------------------------------------------------------------ */
static errorixtension XTAPI ImpaginaDefinitivoManuale(uchar** ptrdocumento) throw()
{
	// singola struttura da impaginare 
	quattrod lQuattroD = {kNessunComando, NULL, 0, kNessunRigore, NULL, NULL};
	// percetuale di scala del documento
	Fixed lPercentuale = 0;
	// per costruire il nome da usare per salvare il documento appena impaginata
	uchar lNomeDocumento[80] = "";
	
	// apertura
	gErroreXtension = ApriDocumento(gXtPreferences.finalePref.docPathFinale);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sull'apertura del documento definitivo
		return(gErroreXtension);
	}	

	if (IsPagineAffiancate() == TRUE) 
	{
		// se a pagine affiancate non impagino 
		ConcatenaAllaStringaErrore((uchar*)curdocname());
		return(kErroreDocumentoPagineAffiancate);
	}

	// imposto la visualizzazione di impaginazione
	lPercentuale = kVistaImpaginazioneDefinitivo;
	ImpostaScalaDocumento(&lPercentuale);
	
	// imposto le coordinate fuori dalla pagina
	ImpostaCoordinateInizialiDefinitivo();
		
	// imposto a zero l'ingombro totale
	InizializzaIngombroTotale();
	
	// imposto a zero lo spazio tra i necrologi totale
	InizializzaSpazioTraNecrologiTotale();
	
	// imposto il box immagine campione
	ImpostaImmagineCampione();
	
	// imposto il box immagine campione per il simbolo della Pax
	gErroreXtension = ImpostaImmagineCampionePax();
	if (gErroreXtension != kNessunErrore)
	{
	 	return(gErroreXtension);
	}

	while (1) 
	{
		if (Button() == TRUE) 
		{
			break;
		}
		LeggiQuattroD(ptrdocumento, &lQuattroD);
				
		// esecuzione del comando di impaginazione 
		GestioneComandiDefinitivoManuale(&lQuattroD);
		
 		if (lQuattroD.tipoComando == kComandoDiFine) 
 		{
			break;
		}
		
		// inizializzo lQuattroD
		InizializzaQuattroD(&lQuattroD);
				
	}

	// visualizza i messaggi sull'ingombro appena calcolato
	StampaIngombro();
		
	// imposto la visualizzazione precedente
	ImpostaScalaDocumento(&lPercentuale);

	// costruisco il nome del documento
	sprintf((char*)lNomeDocumento,"%s%s", PrendiDataXNomeFile(), ".QXP");

	// salvataggio documento appena impaginato
	gErroreXtension = SalvaDocumento(/*PercorsoCompleto(*/gXtPreferences.finalePref.folderPathFinale/*)*/, lNomeDocumento);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sul savataggio del documento definitivo
		return(gErroreXtension);
	}
	

	// permetto all'utente di lavorare con l'impaginazione appena terminata
	ImpostaCoseDellUtente();

	return(kNessunErrore);

} // ImpaginaDefinitivoManuale

/* ------------------------------------------------------------------------ *

	ImpaginaDefinitivoSequenza

/* ------------------------------------------------------------------------ */
static errorixtension XTAPI ImpaginaDefinitivoSequenza(uchar**ptrdocumento) throw()
{
	// singola struttura da impaginare 
	quattrod lQuattroD = {kNessunComando, NULL, 0, kNessunRigore, NULL, NULL};
	// per impostare le informazioni per AppWare
	// appware lAppWare;
	// percetuale di scala del documento
	Fixed lPercentuale = 0;
	// per costruire il nome da usare per salvare il documento appena impaginata
	uchar lNomeDocumento[80] = "";
	
	// apertura
	gErroreXtension = ApriDocumento(gXtPreferences.finalePref.docPathFinale);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sull'apertura del documento definitivo
		return(gErroreXtension);
	}
	
	if (IsPagineAffiancate() == TRUE) 
	{
		// se a pagine affiancate non impagino
		ConcatenaAllaStringaErrore((uchar*)curdocname());
		return(kErroreDocumentoPagineAffiancate);
	}

	// imposto la visualizzazione di impaginazione
	lPercentuale = kVistaImpaginazioneDefinitivo;
	ImpostaScalaDocumento(&lPercentuale);

	// imposto le coordinate fuori dalla pagina
	ImpostaCoordinateIniziali();

	// imposto a zero l'ingombro totale
	InizializzaIngombroTotale();
	
	// imposto a zero lo spazio tra i necrologi totale
	InizializzaSpazioTraNecrologiTotale();
	
	// imposto a zero l'allargamento totale
	InizializzaAllargamentoTotale();
	
	// imposto a zero il restringimenyo totale 
	InizializzaRestringimentoTotale();
	
	// leggo le impostazioni per il filetto di fine colonna
	ImpostaCaratteristicheFiletto(gStrStileFiletto);
	
	// imposto il box immagine campione per il simbolo della Pax
	gErroreXtension = ImpostaImmagineCampionePax();
	if (gErroreXtension != kNessunErrore)
	{
		return(gErroreXtension);
	}
		
	while (1) 
	{
		if (Button() == TRUE) 
		{
			break;
		}
		
		LeggiQuattroD(ptrdocumento, &lQuattroD);
		
		// esecuzione del comando di impaginazione
		GestioneComandiDefinitivoSequenza(&lQuattroD);
		
		if (lQuattroD.tipoComando == kComandoDiFine) 
		{
			break;
		}
		
		// inizializzo lQuattroD
		InizializzaQuattroD(&lQuattroD);
	}
	
	// torno alla prima pagina
	gotopage(kPrimaPagina);
	
	// imposto la scala precedente
	ImpostaScalaDocumento(&lPercentuale);
		
	// costruisco il nome del documento
	wsprintf((char*)lNomeDocumento,"%s%s", PrendiDataXNomeFile(), ".QXP");

	// salvataggio documento appena impaginato
	gErroreXtension = SalvaDocumento(/*PercorsoCompleto(*/gXtPreferences.finalePref.folderPathFinale/*)*/,
									 lNomeDocumento);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sul savataggio del documento definitivo
		return(gErroreXtension);
	}
	
	// permetto all'utente di lavorare con l'impaginazione appena terminata
	ImpostaCoseDellUtente();

	return(kNessunErrore);
} // ImpaginaDefinitivoSequenza

static errorixtension XTAPI ImpaginaDefinitivoAutomatico(uchar** ptrdocumento) throw();
/* ------------------------------------------------------------------------ *

	ImpaginaDefinitivoAutomatico

	25 Novembre 1995						Stefano

	Descrizione:
	Esegue l'impaginazione definitiva con posizionamento in pagina
	dei necrologi in automatico

	Parametri:
	ptrdocumento - puntatore al documento da impaginare

	Ritorno:
	ritorna l'eventuale errore dell'Xtension

* ------------------------------------------------------------------------ */
static errorixtension XTAPI ImpaginaDefinitivoAutomatico(uchar** ptrdocumento) throw()
{
	// singola struttura da impaginare 
	quattrod lQuattroD = {kNessunComando, NULL, 0, kNessunRigore, NULL, NULL};
	// per impostare le informazioni per AppWare
	appware lAppWare;
	// percetuale di scala del documento
	Fixed lPercentuale = 0;
	// per costruire il nome da usare per salvare il documento appena impaginata
	uchar lNomeDocumento[80] = "";
	
	// chiudo tutti i documenti aperti correntemente
	gErroreXtension = ChiudiTuttiIDocumenti(FALSE);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sulla chiusura di tutti i documenti
		return(gErroreXtension);
	}

	// apertura
	gErroreXtension = ApriDocumento(gXtPreferences.finalePref.docPathFinale);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sull'apertura del documento definitivo
		return(gErroreXtension);
	}

	if (IsPagineAffiancate() == TRUE) 
	{
		// se a pagine affiancate non impagino
		ConcatenaAllaStringaErrore((uchar*)curdocname());
		return(kErroreDocumentoPagineAffiancate);
	}

	if (PrendiCodiceDocumento() != 0) 
	{
		// e' un documento su cui ho gia' importato dati
		ConcatenaAllaStringaErrore((uchar*)curdocname());
		return(kErroreDocumentoGiaUsato);
	}

	// imposto la visualizzazione di impaginazione
	lPercentuale = kVistaImpaginazioneDefinitivo;
	ImpostaScalaDocumento(&lPercentuale);

	// imposto le coordinate fuori dalla pagina
	ImpostaCoordinateInizialiDefinitivo();

	// imposto a zero l'ingombro totale
	InizializzaIngombroTotale();

	// imposto a zero lo spazio tra i necrologi totale
	InizializzaSpazioTraNecrologiTotale();

	// imposto a zero l'allargamento totale
	InizializzaAllargamentoTotale();

	// imposto a zero il restringimenyo totale
	InizializzaRestringimentoTotale();

	// leggo le impostazioni per il filetto di fine colonna
	ImpostaCaratteristicheFiletto(gStrStileFiletto);

	// imposto il box immagine campione
	ImpostaImmagineCampione();

	// imposto il box immagine campione per il simbolo della Pax
	gErroreXtension = ImpostaImmagineCampionePax(); 
	if (gErroreXtension != kNessunErrore)
	{
		return(gErroreXtension);
	}

	
	// crea la struttura per poi eseguire il posizionamento
	gErroreXtension = CreaInfoNecrologi(*ptrdocumento);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sul calcolo sul numero dei necrologi
		return(gErroreXtension);
	}

	while (1) 
	{	
		if (Button() == TRUE) 
		{
			break;
		}
		
		LeggiQuattroD(ptrdocumento, &lQuattroD);
				
		// esecuzione del comando di impaginazione
		GestioneComandiDefinitivoAutomatico(&lQuattroD, &lAppWare);
		
 		if (lQuattroD.tipoComando == kComandoDiFine) 
 		{
			break;
		}
		
		// inizializzo lQuattroD
		InizializzaQuattroD(&lQuattroD);
	}
	
	// visualizza i messaggi sull'ingombro appena calcolato
	StampaIngombro();
	
	// imposto la visualizzazione precedente
	ImpostaScalaDocumento(&lPercentuale);

	// imposto il codice di impaginazione
	InizializzaCodiceImpaginazione();
	
	// imposto il codice di impaginazione al documento corrente
	SlugDiDocumento(PrendiCodiceImpaginazione());
	
	// salvo il documento come ultima impaginazione
	STRCPY(lNomeDocumento,gUltimaImpaginazione);

	// salvataggio documento ultima impaginazione
	gErroreXtension = SalvaDocumento(/*PercorsoCompleto(*/gXtPreferences.cartellePref.cartellaLastImp/*)*/,
									 lNomeDocumento);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sul savataggio del documento definitivo
		return(gErroreXtension);
	}

	// costruisco il nome del documento
	sprintf((char*)lNomeDocumento,"%s%s", PrendiDataXNomeFile(), ".QXP");

	// salvataggio documento appena impaginato
	gErroreXtension = SalvaDocumento(/*PercorsoCompleto(*/gXtPreferences.finalePref.folderPathFinale/*)*/,
									 lNomeDocumento);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sul savataggio del documento definitivo
		return(gErroreXtension);
	}
	
	// salvo i dati dell'ultima impaginazione
	gErroreXtension = ScriviInfoNecrologi();
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sulla scrittura dei dati dell'ultima impaginazione
		return(gErroreXtension);
	}
	
	// abilito la possibilita' di calcolare il posizionamento
	// dei necrologi appena importati
	AbilitaMenu(CALCPOSMENUID);

	// disabilito il ripristino dell'ultima impaginazione
	// perche' ormai coicide con questa appena terminata
	DisabilitaMenu(RIPRISTINADOCMENUID);
	DisabilitaMenu(RIPRISTINADATIULTIMAMENUID);
	
	// permetto all'utente de scegliere se fare il calcolo del posizionamento
	ImpostaCoseDellUtente();

	return(kNessunErrore);
} // ImpaginaDefinitivoAutomatico


/* ------------------------------------------------------------------------ *

	Impagina

* ------------------------------------------------------------------------ */
errorixtension XTAPI Impagina(Handle handle) throw()
{
	tipoimpaginazione lTipoImpaginazione = kNessunaImpaginazione;
	
	// inizializzazione del puntatore per leggere l'handle
	gPtrDocumento = (uchar*) (*handle);
	
	// controllo del file 
	gErroreXtension = ControllaComandi(gPtrDocumento);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sui comandi del file di impaginazione
		// tra le <> c'e' qualcosa che non deve esserci
		LiberaTrueHandle((Handle)handle);
		return(gErroreXtension);
	}
	
	// controllo sintattico dei comandi
	gErroreXtension = ControllaArgomenti(gPtrDocumento);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sui separatori degli argomenti dei comandi di impaginazione
		// non ci sono tutti i separatori richiesti
		LiberaTrueHandle((Handle)handle);
		return(gErroreXtension);
	}
	
	// controllo sulla posizione dei comandi di impaginazione
	gErroreXtension = ControllaPosizioneComandi(gPtrDocumento);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sulla posizione dei comandi di impaginazione
		// ci sono dei comandi di impaginazione nel posto sbagliato
		LiberaTrueHandle((Handle)handle);
		return(gErroreXtension);
	}
	
	// leggo il tipo di impaginazione da eseguire
	gErroreXtension = LeggiTipoImpaginazione(&gPtrDocumento, &lTipoImpaginazione);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sul tipo di impaginazione
		LiberaTrueHandle((Handle)handle);
		return(gErroreXtension);
	}
	
	// fine lettura inizio impaginazione
	DaiMessaggio(kFineLettura);

	// azzera il flag sugli errori sullo stile
	InizializzaErroreSulloStile();

	switch (lTipoImpaginazione) 
	{
		case kImpaginazioneBozza:
			DaiMessaggio(kMessaggioBozza);
			gErroreXtension = ImpaginaBozza(&gPtrDocumento);
			break;
		case kImpaginazioneIngombro:
			DaiMessaggio(kMessaggioIngombro);
			gErroreXtension = ImpaginaIngombro(&gPtrDocumento);
			break;
		case kImpaginazioneDefinitiva:
			DaiMessaggio(kMessaggioDefinitiva);
			switch (gXtPreferences.impaginazionePref.tipoImpaginazione) 
			{
				case MANUALERADIOBTNID:
					gErroreXtension = ImpaginaDefinitivoManuale(&gPtrDocumento);
					break;
				case SEQFILEDATIRADIOBTNID:
					gErroreXtension = ImpaginaDefinitivoSequenza(&gPtrDocumento);
					break;
				case POSAUTORADIOBTNID:
					gErroreXtension = ImpaginaDefinitivoAutomatico(&gPtrDocumento);
					break;
			}
			DocID curdoc;
			xtget_curdoc(&curdoc);
			if (curdoc >= 0)
				gotopage(kPrimaPagina);

			break;
	}
	
	if ( handle != NULL) 
	{
		HUnlock((Handle)handle);
		DisposeHandle((Handle)handle);
		handle = NULL;
	}

	return(gErroreXtension);
} // Impagina

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

errorixtension ImpaginaAppWare(Handle *handle)
/* ------------------------------------------------------------------------ *

	ImpaginaAppWare

	11 Marzo 1996						Stefano

	Descrizione:
	Esegue il posizionamento dei necrologi in base alle informazioni che arrivano
	da AppWare.

	Parametri:
	handle - puntatore all'handle che contiene il documento da impaginare

	Ritorno:
	ritorna l'eventuale errore dell'Xtension
	
* ------------------------------------------------------------------------ */
{
	/* per puntata al singolo carattere dell'handle  */
	
	/* inizializzazione del puntatore per leggere l'handle */
	gPtrDocumento = (char far *) (**handle);

	/* libero la memoria dei defunti */
	LiberaInfoDefunti();

	/* resettare le info sulle colonne e fuori */
	LiberaNecrologiSpazioColonna();
	LiberaNecrologiSpazioColonnaFuori();
	
	/* leggo e inserisco i necrologi nelle colonne */
	gErroreXtension = InserisciNecrologiNelleColonne(gPtrDocumento);
	if (gErroreXtension != kNessunErrore) {
		/* errore nel tentare di inserire i necrologi nelle colonne */
		StampaDebug("InserisciNecrologiNelleColonne", (long) gErroreXtension);
		LiberaHandle(*handle);
		return(gErroreXtension);
	}

	/* fine lettura inizio impaginazione */
	DaiMessaggio(kFineLettura);

	/* libero la memoria del file dati */
	LiberaHandle(*handle);

#if kNS
	StampaDebugSpazioColonna();
#endif

	/* posiziono i necrologi */
	gErroreXtension = Posiziona();
	if (gErroreXtension != kNessunErrore) {
		/* errore nel tentare di posizionare i necrologi */
		StampaDebug("Posiziona", (long) gErroreXtension);
	}

	return(gErroreXtension);
}

#endif // // NECRO v1
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/