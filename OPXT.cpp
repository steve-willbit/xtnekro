/* ------------------------------------------------------------------------ *
	OperazioniXtension.cpp

	4 Aprile 2005 - Fabrizio

	Copyright © Sinedita S.r.l 2005. All Rights Reserved.

	Descrizione:
	Contiene le procedure usate per eseguire le operazioni dell'Xtension
	nella fase di IDLE

	$Log: not supported by cvs2svn $
	Revision 1.2  2005/04/20 13:48:52  taretto
	impaginazione ok
	
	Revision 1.1  2005/04/20 07:14:20  taretto
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
#include "DbpResource.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

//#include "Include.h"
//#include "Define.h"
//#include "Notify.h"
//#include "Appl.h"
//#include "Debug.h"
//#include "Appl.h"
//#include "OpString.h"
//#include "Suono.h"

#include "Document.h"
#include "Errori.h"
#include "Impag.h"
#include "Menu.h"
#include "Pref.h"
#include "PresFile.h"
#include "ReadFile.h"
#include "ResStr.h"
#include "StMes.h"
#include "Tempo.h"

#include "OpXt.h"

// GLOBALS
// GLOBALS
// GLOBALS

// per memorizzare l'operazione corrente dell'Xtension
tipooperazione gOperazioneXtension = kCoseDiQuattroD;

// per sapere ogni quanto tempo testare se e' presente un fiel da impaginare
static uint32 gSecondiProssimoLancio = 0;
static struct	tm *tblock;

// variabile d'utilita'
Handle gHndlFile=NULL;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			DevoTestare
	@abstract 		test
	@discussion		Serve a temporizzare l'operazione di verifica della presenza dei file
					da impaginare
					
					4 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result  			ritorna TRUE quando e' ora di andare a vedere se c'e' un file da impaginare
*/
static bool8 XTAPI DevoTestare() throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	DevoTestare

* ------------------------------------------------------------------------ */
static bool8 XTAPI DevoTestare(void) throw()
{
	time_t lSecondi = 0;
	unsigned long lManca = 0;
	
	/* prendo i secondi da 01/01/1904 */
	lSecondi = time(NULL);

	/* calcolo quanto manca */
	if (lSecondi < gSecondiProssimoLancio)
		lManca = gSecondiProssimoLancio - lSecondi;
	else lManca = 0;

	if (lManca == 0) 
	{
		gSecondiProssimoLancio = lSecondi + kLongIntervalloControllo;
		return(TRUE);
	} else return(FALSE);
} // DevoTestare

/* ------------------------------------------------------------------------ *
	
	PrendiOperazioneCorrente

* ------------------------------------------------------------------------ */
tipooperazione XTAPI PrendiOperazioneCorrente() throw()
{
	return(gOperazioneXtension);
} // PrendiOperazioneCorrente


/* ------------------------------------------------------------------------ *

	CoseDiQuattroD
	
* ------------------------------------------------------------------------ */
errorixtension XTAPI CoseDiQuattroD() throw()
{
	// per prendere gli errori dopo l'errore di impaginazione dell'Xtension
	errorixtension lErroreXtension1 = kNessunErrore;
	// per conoscere se le operazioni sono andate a buon fine
	Boolean lBool = FALSE;
	// nome nuovo per il file di Bertellino
	uchar lNuovoNome[MAXPATHNAME] = "";
	// per salvare il vecchio nome del file di Bertellino
	uchar lVecchioNome[MAXPATHNAME] = "";
	// per salvare il vecchio nome del file di Bertellino
	uchar name[MAXPATHNAME] = "";
	uchar name1[MAXPATHNAME] = "";
	
	//per verificare se esiste il box di Pax
	boxid ilBoxDiPax = 0L;

	// controllo se l'XT e' abilitata x l'impaginazione

	// controllo se e' passato il tempo per andare a verificare
	// la presenza del file
	if (DevoTestare() == FALSE) 
		return(kNessunErrore);
	if (gFlagAbilita == TRUE)
	{
		 ImpostaCoseDellUtente();
		 return(kNessunErrore);
	}
		
	gErroreXtension = PresenzaFileDaQuattroD(name, &lBool);
	if (gErroreXtension != kNessunErrore) 
	{
		// si e' verificato un errore nel testare la presenza del file
		return(gErroreXtension);
	}
	if (lBool == FALSE) 
		return(kNessunErrore);
	
	// imposta idle
	gOperazioneXtension = kIdle;
	
	// copio il vecchio nome
	STRCPY(lVecchioNome, name);
	STRCPY(lNuovoNome, name);

	gErroreXtension = GenNomeFilenewExt( lNuovoNome, (uchar*)".QUD");
	if (gErroreXtension != kNessunErrore) 
	{
		// si e verificato un errore nel rinominare il file di 4D
		return(kErroreDiSistema);
	}


	// delete a file with the same renamed data file
	CancellaFile(lNuovoNome);

	gErrore  = rename((char*)name, (char*)lNuovoNome);
	if (gErrore != noErr) {
		//  si e' verificato un errore nel rinominare il file di 4D
		return(kErroreDiSistema);
	}
	// copio il nuovo nome nella struttura FSSpec
	STRCPY((char *)name, (char *)lNuovoNome);

	// bisogna eseguire una impaginazione 
	// allocca anche la memoria in gHndlFile
	gErroreXtension = LeggiContenutoFSSpecFile(name, NULL);
	if (gErroreXtension != kNessunErrore) 
	{
		// si e' verificato un errore nel leggere il file
		return(gErroreXtension);
	}

	//  il file da impaginare e' in memoria
	ConcatenaAllaStringaMessaggio(lVecchioNome);
	DaiMessaggio(kImpaginazioneDocumento);
	
	//  esecuzione dell'impaginazione e 
	//  libera anche la memoria alloccata in gHndlFile
	gErroreXtension = Impagina(gHndlFile);
	if (gErroreXtension != kNessunErrore) 
	{
		if (gErroreXtension == kErroreTroppaDifferenza) 
		{
			// c'e' un errore di differenza tra lo spozio disponibile e
			//  lo spazio calcolato tramite l'estimo
			return(gErroreXtension);
			
		} 
		else 
		{
			//  cerco di spostare il file sulla scrivania
			lErroreXtension1 = SpostaFileNellaCartella(name, gXtPreferences.cartellePref.cartellaTemporanei, &lBool);
			if (lErroreXtension1 != kNessunErrore) 
			{
				// si e' verificato un errore nel spostare il file da impaginare
				return(lErroreXtension1);
			}

			if (lBool == FALSE) 
			{
				// non sono riuscito a spostare il file, allora lo cancello
				lErroreXtension1 = CancellaFile(name);
				if (lErroreXtension1 != kNessunErrore) 
				{
					// si e verificato un errore nel cancellare il file da impaginare
					return(lErroreXtension1);
				}
			}

#if 0	 // OLDESTVERSIOV		
			// cerco di spostare il file sulla scrivania
			lErroreXtension1 = SpostaFileSuScrivania(name, &lBool);
			if (lErroreXtension1 != kNessunErrore) {
				// si e- verificato un errore nel spostare il file da impaginare
				StampaDebug("SpostaFileSuScrivania", (long) lErroreXtension1);
				StampaDebug(name , (long) lErroreXtension1);
				return(lErroreXtension1);
			}

			if (lBool == FALSE) {
				// non sono riuscito a spostare il file, allora lo cancello
				lErroreXtension1 = CancellaFile(name);
				if (lErroreXtension1 != kNessunErrore) {
					// si e- verificato un errore nel cancellare il file da impaginare
					StampaDebug("CancellaFile", (long) lErroreXtension1);
					return(lErroreXtension1);
				}
			}
#endif // 0 OLDESTVERSIOV
			
			return(gErroreXtension);
		}
	}
	
	// cerco di spostare il file nella cartella dei temporanei
	gErroreXtension = SpostaFileNellaCartella(name, gXtPreferences.cartellePref.cartellaTemporanei, &lBool);
	if (gErroreXtension != kNessunErrore) 
	{
		// si e' verificato un errore nel spostare il file impaginato 
		return(gErroreXtension);
	}
	if (lBool == FALSE) 
	{
		// non sono riuscito a spostare il file, allora lo cancello
		gErroreXtension = CancellaFile(name);
		if (gErroreXtension != kNessunErrore) 
		{
			// si e' verificato un errore nel cancellare il file da impaginare
			return(gErroreXtension);
		}
	}

#if 0 // OLDESTVERSION

	/* l'impaginazione è andata a buon fine cancello il file da impaginare */
	gErroreXtension = CancellaFile(name);
	if (gErroreXtension != kNessunErrore) {
		/* si è verificato un errore nel cancellare il file da impaginare */
		StampaDebug("CancellaFile", (long) gErroreXtension);
		return(gErroreXtension);
	}

	/* l'impaginazione e andata a buon fine cancello il file da impaginare */
	GetNomeFile( name, name1);
	strcpy( lVecchioNome, kStrCartellaTmp);
	strcat( lVecchioNome, "\\" );
	strcat( lVecchioNome, name1 );
	CancellaFile(lVecchioNome);
	gErrore = rename(name, lVecchioNome );
	if (gErrore != noErr) {
		/* un errore nel rinominare il file di 4D */
		StampaDebug("Rename", (long) gErrore);
		StampaDebug(name, (long) gErrore);
		StampaDebug(lVecchioNome, (long) gErrore);
		return(kErroreDiSistema);
	}
#endif // OLDESTVERSION

	// fine impaginazione 
	DaiMessaggio(kFineImpaginazione);
	return(kNessunErrore);
}

/* ------------------------------------------------------------------------ *

	ImpostaCoseDellUtente

* ------------------------------------------------------------------------ */
void XTAPI ImpostaCoseDellUtente() throw()
{
	gOperazioneXtension = kCoseDellUtente;
} // ImpostaCoseDellUtente

/* ------------------------------------------------------------------------ *

	ImpostaCoseDiQuattroD

	10 Novembre 1995						Stefano

	Descrizione:
	Imposta come operazione dell'Xtension le cose di 4D

	Parametri:
	nessuno

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
void XTAPI ImpostaCoseDiQuattroD() throw()
{
	gOperazioneXtension = kCoseDiQuattroD;
} // ImpostaCoseDiQuattroD

/* ------------------------------------------------------------------------ *

	CoseDiStampa
	
* ------------------------------------------------------------------------ */
void XTAPI CoseDiStampa() throw()
{
	// finche' la stampa non e' finita aspetto a chiudere il documentoù
	int16 jobinprogress;
	xtget_jobinprogress(&jobinprogress);

	if (jobinprogress == TRUE) 
	{
		return;
	}

	// chiudo il documento che e' stato appena impaginato
	ChiudiDocumento();
	
	if (kCheckAbilitaImpaginazione == TRUE) 
		gOperazioneXtension = kCoseDiQuattroD;
	else 
		gOperazioneXtension = kCoseDellUtente;
} // CoseDiStampa

/* ------------------------------------------------------------------------ *

	OperazioneCorrente

* ------------------------------------------------------------------------ */
void XTAPI CambiaOperazioneCorrente() throw()
{
	if (gOperazioneXtension == kCoseDiQuattroD ||
		gOperazioneXtension == kCoseDiAppWare) gOperazioneXtension = kCoseDellUtente;
	else gOperazioneXtension = kCoseDiQuattroD;
	gSecondiProssimoLancio = 0;
	gFlagAbilita=FALSE;
} // CambiaOperazioneCorrente

/* ------------------------------------------------------------------------ *

	ImpostaCoseDiStampa

* ------------------------------------------------------------------------ */
void XTAPI ImpostaCoseDiStampa() throw()
{
	gOperazioneXtension = kCoseDiStampa;
} // ImpostaCoseDiStampa

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

errorixtension CoseDiAppWare(void)
/* ------------------------------------------------------------------------ *

	CoseDiAppWare

	10 Novembre 1995						Stefano

	Descrizione:
	Dobbiamo aspettare il file di AppWare da rimpaginare

	Parametri:
	nessuno

	Ritorno:
	ritorna l'eventuale errore dell'xtension

* ------------------------------------------------------------------------ */

{
	errorixtension lErroreXtension1 = kNessunErrore;
	/* per prendere gli errori dopo l'errore di impaginazione dell'Xtension */
	Boolean lBool = FALSE;
	/* per conoscere se le operazioni sono andate a buon fine */
//	FSSpec lFSSpecFile = {0, 0, "\p"};
	/* per prendere il riferimento del file da impaginare copiato */
//	Handle gHndlFile = NULL;
	/* per contenere il file da impaginare */
	char lNuovoNome[kDimensioneStringhe] = "";
	/* nome nuovo del file da AppWare */
	char name[kDimensioneStringhe] = "";

	/* testo se Xpress Š l'applicazione attiva */
#ifdef OPERATION
	gErroreXtension = IsXpressAttivo(&lBool);
	if (gErroreXtension != kNessunErrore) {
		/* si Š verificato un errore nel testare se Xpress Š attivo */
		StampaDebug("IsXpressAttivo", (long) gErroreXtension);
		return(gErroreXtension);
	}

	if (lBool == FALSE) return(kNessunErrore);
#endif

	gErroreXtension = PresenzaFileDaAppWare(name, &lBool);
	if (gErroreXtension != kNessunErrore) {
		/* si Š verificato un errore nel testare la presenza del file */
		StampaDebug("PresenzaFileDaAppWare", (long) gErroreXtension);
		return(gErroreXtension);
	}
	if (lBool == FALSE) return(kNessunErrore);

	/* rinomino il file da AppWare */
//	sprintf(lNuovoNome,"%s %s", PrendiData(), PrendiOra());

//	strncpy(lNuovoNome, PrendiData(), 8);
//	strcat(lNuovoNome, ".apw");
	strcpy(lNuovoNome, name);

	gErroreXtension = GenNomeFilenewExt( lNuovoNome, ".APW");
	if (gErroreXtension != kNessunErrore) {
		/* si Š verificato un errore nel rinominare il file di 4D */
		StampaDebug("GenNomeFilenewExt", (long) gErrore);
		return(kErroreDiSistema);
	}

//	gErrore = FSpRename(name, lNuovoNome);
	gErrore  = rename( name, lNuovoNome);
	if (gErrore != noErr) {
		/* si Š verificato un errore nel rinominare il file da AppWare */
		StampaDebug("FSpRename", (long) gErrore);
		return(kErroreDiSistema);
	}
	/* copio il nuovo nome nella struttura FSSpec */
	strcpy((char *) name, (char *) lNuovoNome);

	/* suono di inizio */
//	Suona(rTrovatoFile);

	/* bisogna eseguire una impaginazione */
	/* allocca anche la memoria in gHndlFile */
	gErroreXtension = LeggiNormContenutoFSSpecFile(name, &gHndlFile);
	if (gErroreXtension != kNessunErrore) {
		/* si Š verificato un errore nel leggere il file */
		StampaDebug("LeggiContenutoFSSpecFile", (long) gErroreXtension);
		return(gErroreXtension);
	}

#if kNS
	StampaDebug("---- Inizio", 0);
	StampaHandle(gHndlFile); 
	StampaDebug("---- Fine", 0);
#endif

	/* il file da impaginare Š in memoria */
	DaiMessaggio(kImpaginazioneAppWare);
	
	/* esecuzione dell'impaginazione e */
	/* libera anche la memoria alloccata in gHndlFile */
	gErroreXtension = ImpaginaAppWare(&gHndlFile);
	if (gErroreXtension != kNessunErrore) {
		/* si Š verificato un errore nell'esecuzione dell'impaginazione */

#if kNS
		StampaDebug("ImpaginaAppWare", (long) gErroreXtension);
#endif

#if kNEW

		/* cerco di spostare il file sulla scrivania */
		lErroreXtension1 = SpostaFileNellaCartella(name,
																 kStrCartellaTmp,
																 &lBool);
		if (lErroreXtension1 != kNessunErrore) {
			/* si Š verificato un errore nel spostare il file da impaginare */
			StampaDebug("SpostaFileNellaCartella", (long) lErroreXtension1);
			return(lErroreXtension1);
		}
		if (lBool == FALSE) {
			/* non sono riuscito a spostare il file, allora lo cancello */
			lErroreXtension1 = CancellaFile(name);
			if (lErroreXtension1 != kNessunErrore) {
				/* si Š verificato un errore nel cancellare il file da impaginare */
				StampaDebug("CancellaFile", (long) lErroreXtension1);
				return(lErroreXtension1);
			}
		}

#else

		/* cerco di spostare il file sulla scrivania */
		lErroreXtension1 = SpostaFileSuScrivania(name, &lBool);
		if (lErroreXtension1 != kNessunErrore) {
			/* si Š verificato un errore nel spostare il file da impaginare */
			StampaDebug("SpostaFileSuScrivania", (long) lErroreXtension1);
			return(lErroreXtension1);
		}
		if (lBool == FALSE) {
			/* non sono riuscito a spostare il file, allora lo cancello */
			lErroreXtension1 = CancellaFile(name);
			if (lErroreXtension1 != kNessunErrore) {
				/* si Š verificato un errore nel cancellare il file da impaginare */
				StampaDebug("CancellaFile", (long) lErroreXtension1);
				return(lErroreXtension1);
			}
		}

#endif /* kNEW */

		return(gErroreXtension);
	}
	
	/* l'impaginazione Š andata a buon fine cancello il file da impaginare */
	gErroreXtension = CancellaFile(name);
	if (gErroreXtension != kNessunErrore) {
		/* si Š verificato un errore nel cancellare il file da impaginare */
		StampaDebug("CancellaFile", (long) gErroreXtension);
		return(gErroreXtension);
	}

	/* imposto le cose dell'utente */
	gOperazioneXtension = kCoseDellUtente;

	return(kNessunErrore);
}

errorixtension CoseDiNotifica(void)
/* ------------------------------------------------------------------------ *

	CoseDiNotifica

	3 Novembre 1995						Stefano

	Descrizione:
	Serve per rimuovere la notifica appena inviata all'utente. Si rimuove la
	notifica solo se Xpress Š diventato attivo.

	Parametri:
	nessuno

	Ritorno:
	ritorna l'eventuale errore dell'xtension

* ------------------------------------------------------------------------ */

{
#ifdef OPERATION

	Boolean lBool = FALSE;
	/* per il test se Xpress Š attivo */

	gErroreXtension = IsXpressAttivo(&lBool);
	if (gErroreXtension != kNessunErrore) {
		/* si Š verificato un errore nel testare se Xpress Š attivo */
		StampaDebug("IsXpressAttivo", (long) gErroreXtension);
		return(gErroreXtension);
	}

	if (lBool == TRUE) {
		gErroreXtension = RimuoviNotifica();
		if (gErroreXtension != kNessunErrore) {
			/* si Š verificato un errore nel rimuovere la notifica */
			StampaDebug("RimuoviNotifica", (long) gErroreXtension);
			return(gErroreXtension);
		}
		gOperazioneXtension = kCoseDellUtente;
	} else gOperazioneXtension = kCoseDiNotifica;
#endif

	return(kNessunErrore);
}

errorixtension EsecuzioneAppWare(void)
/* ------------------------------------------------------------------------ *

	EsecuzioneAppWare

	10 Novembre 1995						Stefano

	Descrizione:
	Manda in esecuzione l'applicazione AppWare e poi manda lXtension
	in attesa sul file di risposta

	Parametri:
	nessuno

	Ritorno:
	ritorna l'eventuale errore dell'xtension

* ------------------------------------------------------------------------ */

{
#if kOLD
	/* prima di mandare in esecuzione AppWare cancello i file presenti */
	/* nella cartella da AppWare */
	gErroreXtension = CancellaTutto(PercorsoCompleto(kStrCartellaDaAppWare));
	if (gErroreXtension != kNessunErrore) {
		/* errore non Š possibile cancellare i file */
		/* nella cartella di AppWare */
		StampaDebug("CancellaTutto", (long) gErroreXtension);
		return(gErroreXtension);
	}

	/* mando in esecuzione AppWare */
	gErroreXtension = EseguiApplicazione(PercorsoCompleto(kStrApplicazioneAppWare));
	if (gErroreXtension != kNessunErrore) {
		/* errore non Š possibile mandare in */
		/* esecuzione l'applicazione AppWare */
		StampaDebug("EseguiApplicazione", (long) gErroreXtension);
		return(gErroreXtension);
	}
	
#endif /* kOLD */

	/* rimango in attesa del file da AppWare */
	gOperazioneXtension = kCoseDiAppWare;

	return(kNessunErrore);
}

void ImpostaCoseDiAppWare(void)
/* ------------------------------------------------------------------------ *

	ImpostaCoseDiAppWare

	10 Novembre 1995						Stefano

	Descrizione:
	Imposta come operazione dell'Xtension le cose di AppWare

	Parametri:
	nessuno

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */

{
	gOperazioneXtension = kCoseDiAppWare;
}

void ImpostaCoseDiNotifica(void)
/* ------------------------------------------------------------------------ *

	ImpostaCoseDiNotifica

	13 Novembre 1995						Stefano

	Descrizione:
	Imposta come operazione dell'Xtension le cose di Notifica

	Parametri:
	nessuno

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */

{
	gOperazioneXtension = kCoseDiNotifica;
}

#endif // // NECRO v1
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/