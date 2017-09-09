/* ------------------------------------------------------------------------ *

	RisorseStringhe.cpp

	5 Aprile 2005						Fabrizio

	Descrizione:
	Procedure per la gestione delle stringhe salvate sul file di risorsa.
	In fase di sviluppo, per comodit…, tutte le stringhe sono trattate come Pascal.
	Le stringhe sono salvate su una risorsa STR# con questi identificatori:
	- 23000 stringhe dell'Xtension;
	- 21000 errori bloccanti e errori non bloccanti visualizzati sul file di report
	e sulla palette;
	- 22000 messaggi visualizzati sul file report e sulla palette dell'informazioni.
	Ognuno di questi gruppi viene caricato alla partenza dell'Xtension
	in gruppi di stringhe.
	Il nome dell'Xtension viene usato solamente nel scroll iniziale dell'icona
	dell'Xtension.
	I messaggi di errore bloccanti sono utilizzati all'interno del file Errore.c
	per caricati essere caricati tra gli errori di Xpress.
	I messaggi non bloccanti e i messaggi da stampare sul file di report sono
	disponibili per tutta la durata del lavoro con Xpress.
	
	$Log: not supported by cvs2svn $

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

#include <string.h>
#include <stdio.h>

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"
#include "DbpResource.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "Errori.h"

#include "ResStr.h"

// DEFINES
// DEFINES
// DEFINES

//  risorsa che contiene le stringhe dell'Xtension
#define rStringheXtension 					23000
#define kNomeXtension 					1
#define kNomeFileRapporto 					2
#define kTestoNotifica 						3
#define kStileCodiceDefunto 				4
#define kStileCodiceNecrologio 				5
#define kNomeDocumentoIngombro 			6
#define kSelezionaCartellaImmagini 			7
#define kSelezionaDocumentoBozza 			8
#define kStileImmagine 					9
#define kSelezionaDocumentoIngombro 		10
#define kStileFiletto 						11
#define kNomeDocumentoUltimaImpaginazione 	12
#define kNomeDatiUltimaImpaginazione 		13
#define kNomeColonneUltimaImpaginazione 	14

// risorsa che contiene le stringhe degli errori bloccanti e non bloccanti
#define rStringheErrori 						21000

// risorsa che contiene i messaggi dell'Xtension 
#define rStringheMessaggi 					22000

// GLOBALS
// GLOBALS
// GLOBALS

// Stringhe 23000

// variabile d'ultilita' per la lettura delle stringhe
uchar tmpStr[MAXPATHNAME];

// nome dell'Xtension
uchar gStrNomeXtension[64];
// nome del file di rapporto 
uchar gStrNomeFileRapporto[64];
// testo del messaggio di notifica 
uchar gStrTestoNotifica[64];
// stile da usare per stampare il codice del defunto nella stampa bozze 
uchar gStrDefunto[64];
// stile da usare per stampare il codice del necrologio nella stampa bozze
uchar gStrNecrologio[64];
// nome del documento di ingombro
uchar gStrIngombro[64];
// messaggio per la selezione della cartella immagini
uchar gCartellaImmagini[64];
// messaggio per la selezione del documento bozza
uchar gDocumentoBozza[64];
// stile da usare per stampare il codice dell'immagine
uchar gStrStileImmagine[64];
// messaggio da presentare per la selezione del documento di ingombro
uchar gDocumentoIngombro[64];
// contiene il nome dello stile che ha il filetto superiore
uchar gStrStileFiletto[64];
// contiene il nome del documento Xpress dell'ultima impaginazione
uchar gUltimaImpaginazione[64];
// contiene il nome del documento con i dati dell'ultima impaginazione
uchar gNomeDati[64];
// contiene il nome del documento con i dati sulle colonne dell'ultima impaginazione
uchar gNomeColonne[64];

// Stringhe 21000
uchar gStrErrori[kNumeroErrori][64];

//  Stringhe 22000
uchar gStrMessaggi[kNumeroMessaggi][64];

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			CreaELeggiStringaC
	@abstract 		gestione stringhe.
	@discussion		Legge le stringa salvate sulla risorsa dell'Xtension e crea lo spazio
					di memoria per scriverla nel puntare passato.

					5 Aprile 2005 - Fabrizio.

	@param 			ptrstringa  puntatore da settare, in uscita contiene la stringa letta dal file di risorsa
	@param			idrisorsa  identificatore della risorsa delle stringhe da leggere
	@param 			posizione  contiene la posizione della stringa da caricare
	@result  			TRUE - se e' stato possibile alloccare la memoria
					FALSE - se si e' verificato un errore di memoria
*/
static XTAPI void CreaELeggiStringaC(uchar *ptrstringa, int16 idrisorsa, int16 posizione) throw();

/*!
	@function			CreaStringaErrore
	@abstract 		gestione stringhe.
	@discussion		Carica e crea una stringa di errore dell'Xtension.

					5 Aprile 2005 - Fabrizio.

	@param 			errore  puntatore da settare, in uscita contiene la stringa letta dal file di risorsa
	@result  			nessuno
*/
static XTAPI void CreaStringaErrore(errorixtension errore) throw();

/*!
	@function			CreaStringaMessaggio
	@abstract 		gestione stringhe.
	@discussion		Carica e crea una stringa di messaggio dell'Xtension

					5 Aprile 2005 - Fabrizio.

	@param 			messaggio codice della stringa da creare
	@result  			nessuno
*/
static void XTAPI CreaStringaMessaggio(messaggixtension messaggio) throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	CreaELeggiStringaC

* ------------------------------------------------------------------------ */
static XTAPI void CreaELeggiStringaC(uchar *ptrstringa, int16 idrisorsa, int16 posizione) throw()
{
	int16 lLung = 0;
	
	// leggo risorsa
	DBP::GetResourceString(tmpStr, idrisorsa, posizione, 256);
	
	// creo la memoria per la stringa appena letta
	if (STRLEN(tmpStr) != 0) 
	{
		// copio la stringa appena letta dal file delle risorse
		STRCPY(ptrstringa, tmpStr);
	} 
	else
	{
		// si e' verificato un'errore
		sprintf((char*)gStringaC, "Manca la stringa %d di risorsa %d", posizione, idrisorsa);
		lLung = STRLEN(gStringaC);
		STRCPY(ptrstringa, gStringaC);
	}
} // CreaELeggiStringaC

/* ------------------------------------------------------------------------ *

	CreaStringaErrore

* ------------------------------------------------------------------------ */
static void XTAPI CreaStringaErrore(errorixtension errore) throw()
{
	CreaELeggiStringaC(gStrErrori[errore - 1], rStringheErrori, errore);
} // CreaStringaErrore

/* ------------------------------------------------------------------------ *

	CreaStringaMessaggio

* ------------------------------------------------------------------------ */
void XTAPI CreaStringaMessaggio(messaggixtension messaggio) throw()
{
	CreaELeggiStringaC(gStrMessaggi[messaggio - 1], rStringheMessaggi, messaggio);
} // CreaStringaMessaggio

/* ------------------------------------------------------------------------ *

	CreaTutteLeStringhe

* ------------------------------------------------------------------------ */
void XTAPI CreaTutteLeStringhe() throw()
{
	// per scorrere le stringhe da creare
	int16 lIndice = 0;	
		
	DBP::GetResourceString(tmpStr, rStringheXtension, kNomeXtension, 256);
	STRCPY(gStrNomeXtension, tmpStr);
	DBP::GetResourceString(tmpStr, rStringheXtension, kNomeFileRapporto, 256);
	STRCPY(gStrNomeFileRapporto, tmpStr);
	DBP::GetResourceString(tmpStr, rStringheXtension, kTestoNotifica, 256);
	STRCPY(gStrTestoNotifica, tmpStr);
	DBP::GetResourceString(tmpStr, rStringheXtension, kStileCodiceDefunto, 256);
	STRCPY(gStrDefunto, tmpStr);
	DBP::GetResourceString(tmpStr, rStringheXtension, kStileCodiceNecrologio, 256);
	STRCPY(gStrNecrologio, tmpStr);
	DBP::GetResourceString(tmpStr, rStringheXtension, kNomeDocumentoIngombro, 256);
	STRCPY(gStrIngombro, tmpStr);
	DBP::GetResourceString(tmpStr, rStringheXtension, kSelezionaCartellaImmagini, 256);
	STRCPY(gCartellaImmagini, tmpStr);
	DBP::GetResourceString(tmpStr, rStringheXtension, kSelezionaDocumentoBozza, 256);
	STRCPY(gDocumentoBozza, tmpStr);
	DBP::GetResourceString(tmpStr, rStringheXtension, kStileImmagine, 256);
	STRCPY(gStrStileImmagine, tmpStr);
	DBP::GetResourceString(tmpStr, rStringheXtension, kSelezionaDocumentoIngombro, 256);
	STRCPY(gDocumentoIngombro, tmpStr);
	DBP::GetResourceString(tmpStr, rStringheXtension, kStileFiletto, 256);
	STRCPY(gStrStileFiletto, tmpStr );
	DBP::GetResourceString(tmpStr, rStringheXtension, kNomeDocumentoUltimaImpaginazione, 256);
	STRCPY(gUltimaImpaginazione, tmpStr);
	DBP::GetResourceString(tmpStr, rStringheXtension, kNomeDatiUltimaImpaginazione, 256);
	STRCPY(gNomeDati, tmpStr);
	DBP::GetResourceString(tmpStr, rStringheXtension, kNomeColonneUltimaImpaginazione, 256);
	STRCPY(gNomeColonne, tmpStr );

	for (lIndice = kPrimoErrore; lIndice <= kNumeroErrori; lIndice++) 
	{
		CreaStringaErrore((errorixtension)lIndice);
	}

	for (lIndice = kPrimoMessaggio; lIndice <= kNumeroMessaggi; lIndice++) 
	{
		CreaStringaMessaggio((messaggixtension)lIndice);
	}
} // CreaTutteLeStringhe

#pragma mark -
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

static void StampaDebugTutteLeStringhe(void);
static void StampaDebugTutteLeStringhe(void)
/* ------------------------------------------------------------------------ *

	StampaDebugTutteLeStringhe

	5 Marzo 1995						Stefano

	Descrizione:
	Stampa le stringhe dell'Xtension

	Parametri:
	nessuno

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */

{
#if kCT
	int lIndice = 0;

	StampaDebug("StampaDebugTutteLeStringhe", 0);
	StampaDebug("==========================", 0);
	StampaDebug(gNomeCartellaXpress, 0);
	StampaDebug(gNomeCartellaWindows, 0);
	StampaDebug(gStrNomeXtension, 0);
	StampaDebug(gStrNomeFileRapporto, 0);
	StampaDebug(gStrTestoNotifica, 0);
	StampaDebug(gStrDefunto, 0);
	StampaDebug(gStrNecrologio, 0);
	StampaDebug(gStrIngombro, 0);
	StampaDebug(gCartellaImmagini, 0);
	StampaDebug(gDocumentoBozza, 0);
	StampaDebug(gStrStileImmagine, 0);
	StampaDebug(gDocumentoIngombro, 0);
	StampaDebug(gStrStileFiletto, 0);
	StampaDebug(gUltimaImpaginazione, 0);
	StampaDebug(gNomeDati, 0);
	StampaDebug(gNomeColonne, 0);

	for (lIndice = kPrimoErrore; lIndice <= kNumeroErrori; lIndice++) {
		StampaDebug(gStrErrori[lIndice - 1], 0);
	}
	for (lIndice = kPrimoMessaggio; lIndice <= kNumeroMessaggi; lIndice++) {
		StampaDebug(gStrMessaggi[lIndice - 1], 0);
	}
	StampaDebug("==========================", 0);
#endif
}

#endif // // NECRO v1
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


