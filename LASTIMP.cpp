/* ------------------------------------------------------------------------ *

	UltimaImpaginazione.cpp

	18 Aprile 2005						Fabrizio

	Descrizione:
	Contiene le procedure per il il salvattaggio e la successiva lettura dei
	dati per gestire il ripristino dell'ultima impaginazione

	$Log: not supported by cvs2svn $
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

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "Errori.h"
#include "ReadFile.h"
#include "AppWare.h"
#include "Pref.h"
#include "ResStr.h"
#include "InfoNec.h"
#include "DocInfo.h"
#include "SpaceCol.h"
#include "Coord.h"
#include "Memoria.h"
#include "PresFile.h"
#include "XTNecro.h"

#include "LastImp.h"

// CONSTS
// CONSTS
// CONSTS

#define kCreatoreDatiUltimaImpaginazione TEXTEXT
#define kTipoDatiUltimaImpaginazione TEXTEXT

#define kFineDefunto 'd'
#define kFineCodiceA 'a'
#define kFineCodiceB 'b'


// STATICS GLOBALS
// STATICS GLOBALS
// STATICS GLOBALS

// contine i dati dell'ultima impaginazione
static Handle gHndlUltimaImpaginazione = NULL;

// identificatore file dell'ultima impaginazione 
static short gIdFileUltimaImpaginazione = 0;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			ApriDatiUltimaImpaginazione
	@abstract		gestione ultima impaginazione
	@discussion		Apre il file e la wd della cartella dell'ultima impaginazione.

					18 Aprile 2005 - Fabrizio.

	@param			cancellare  per sapere se devo cancellare un eventuale file gia' esistente.
	@result			eventuale errore dell'Xt.
*/
static errorixtension XTAPI ApriDatiUltimaImpaginazione(uchar* nomefiledati, Boolean cancellare) throw();

/*! 
	@function			ChiudiDatiUltimaImpaginazione
	@abstract		gestione ultima impaginazione
	@discussion		Chiude il file e la wd della cartella dell'ultima impaginazione

					18 Aprile 2005 - Fabrizio.

	@param			nessuno
	@result			ritorna l'eventuale errore dell'xtension
*/
static errorixtension XTAPI ChiudiDatiUltimaImpaginazione() throw();

/*! 
	@function			ScriviNecrologi
	@abstract		gestione ultima impaginazione
	@discussion		Scrivi sul file dell'ultima impaginazione le info sui necrologi

					18 Aprile 2005 - Fabrizio.

	@param			ptrdefunto puntatore all'array che contiene i necrologi da stampare
	@parem			numerodefunti numero dei defunti da stampare
	@result			nessuno
*/
static void XTAPI ScriviNecrologi(defunto *ptrdefunto, int16 numerodefunti) throw();

/*! 
	@function			ScriviNecrologi
	@abstract		gestione ultima impaginazione
	@discussion		Scrive sul file dell'ultima impaginazione le info del documento


					18 Aprile 2005 - Fabrizio.

	@param			nessuno
	@result			nessuno
*/
static void XTAPI ScriviInfoDocumento() throw();

/*! 
	@function			LeggiNecrologi
	@abstract		gestione ultima impaginazione
	@discussion		Legge sul file dell'ultima impaginazione le info sui necrologi

					3 Maggio 2005 - Fabrizio.

	@param			ptrdati - puntatore al puntatore del carattere da leggere del file dell'ultima
					impaginazione
	@param 			ptrdefunto - puntatore al puntatore all'array che conterr… i necrologi letti
	@param			ptrnumerodefunti - ritorna il numero dei defunti letti
	@result			ritorna l'eventuale errore dell'xtension
*/
static errorixtension XTAPI LeggiNecrologi(uchar **ptrdati, defunto **ptrdefunto, int16 *ptrnumerodefunti) throw();

/*! 
	@function			LeggiInfoDocumento
	@abstract		gestione ultima impaginazione
	@discussion		Legge sul file dell'ultima impaginazione le info del documento

					3 Maggio 2005 - Fabrizio.

	@param			ptrdati - puntatore al puntatore del carattere da leggere del file dell'ultima
					impaginazione
	@result			nessuno
*/
static void XTAPI LeggiInfoDocumento(uchar **ptrdati) throw();

/*! 
	@function			ScriviDatiSpazioColonna
	@abstract		gestione ultima impaginazione
	@discussion		Scrivi dati delle colonne

					3 Maggio 2005 - Fabrizio.

	@param			nessuno
	@result			nessuno
*/
static void XTAPI ScriviDatiSpazioColonna() throw();

/*! 
	@function			ScriviColonneDocumento
	@abstract		gestione ultima impaginazione
	@discussion		Scrive sul file dell'ultima impaginazione le info sulle colonne

					3 Maggio 2005 - Fabrizio.

	@param			nessuno
	@result			nessuno
*/
static void XTAPI ScriviColonneDocumento() throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	ApriDatiUltimaImpaginazione

* ------------------------------------------------------------------------ */
static errorixtension XTAPI ApriDatiUltimaImpaginazione(uchar* nomefiledati, Boolean cancellare) throw()
{
	// nome del file dati dell'ultima impaginazione
	uchar lNomeFileUltimaImpaginazione[kDimensioneStringhe] ="";
	/* per prendere l'identificatore della cartella dell'ultima impaginazione */
	int32 lIdCartellaUltimaImpaginazione = 0;
	
#if 0 // OPERATION
	gErroreXtension = PrendiIdentificatoreCartella(lNomeCartellaUltimaImpaginazione,
													&lIdCartellaUltimaImpaginazione);
	if (gErroreXtension != kNessunErrore) {
		/* errore nella creazione del FSSpec */
		StampaDebug("PrendiIdentificatoreCartella", (long) gErroreXtension);
		return(gErroreXtension);
	}

	/* apro la working directory della cartella dell'ultima impaginazione */
	lFSSpecCartellaUltimaImpaginazione.parID = lIdCartellaUltimaImpaginazione;
	gErrore = OpenWD(lFSSpecCartellaUltimaImpaginazione.vRefNum,
					 lFSSpecCartellaUltimaImpaginazione.parID,
					 0,
					 &gWDCartellaUltimaImpaginazione);
	if (gErrore != noErr) {
		/* errore sulla apertura della cartella del documento */
		StampaDebug("OpenWD", (long) gErrore);
		return(kErroreDiSistema);
	}


	/* costruisco il nome del file dati dell'ultima impaginazione */
	strcpy(lNomeFileUltimaImpaginazione,
			PercorsoCompleto(XtPreferences.cartellePref.cartellaLastImp));


	/* costruisco il nome del file dei dati dell'ultima impaginazione */
	strcat(lNomeFileUltimaImpaginazione, "\\" );
	strcat(lNomeFileUltimaImpaginazione, nomefiledati);
#endif // OPERATION

	wsprintf((char*) lNomeFileUltimaImpaginazione, "%s\\%s", /*PercorsoCompleto(*/gXtPreferences.cartellePref.cartellaLastImp/*)*/, nomefiledati);
	if (cancellare == TRUE) 
	{
		// creo il file dei dati dell'ultima impaginazione
		while (1) 
		{
			//gErrore = Create(lNomeFileUltimaImpaginazione,
			//				 0,
			//				 kCreatoreDatiUltimaImpaginazione,
			//				 kTipoDatiUltimaImpaginazione);
			XFileInfoRef fileInfoRef = INVALID_XFILEINFOREF;
			FileExist((char*) lNomeFileUltimaImpaginazione, &fileInfoRef);
			gErrore = XTCreateFile(fileInfoRef, kCreatoreDatiUltimaImpaginazione);
			if (gErrore == noErr) {
				if ( NULL != fileInfoRef ) {
					XTDisposeXFileInfoRef(fileInfoRef);
				}
				break;
			}
			if (gErrore == dupFNErr) 
			{
				// cancello il file ancora esistente
				gErrore = FSDelete(lNomeFileUltimaImpaginazione, 0);
				if (gErrore != noErr) 
				{
					if ( NULL != fileInfoRef ) {
						XTDisposeXFileInfoRef(fileInfoRef);
					}
					// errore di cancellazione file ultima impaginazione
					return(kErroreDiSistema);
				}	
			} 
			else 
			{
				if ( NULL != fileInfoRef ) {
					XTDisposeXFileInfoRef(fileInfoRef);
				}
				// errore nella creazione del FSSpec
				return(kErroreDiSistema);
			}
		}
	}
	
	// apertura del file dei dati dell'ultima impaginazione
	gErrore = HOpenDF(0, 0, lNomeFileUltimaImpaginazione, fsRdWrPerm, &gIdFileUltimaImpaginazione); // <-- was FSOpen
	if (gErrore != noErr) 
	{
		// errore nell'apertura del file dell'ultima impaginazione
		return(kErroreDiSistema);
	}
	
	return(kNessunErrore);

} // ApriDatiUltimaImpaginazione

/* ------------------------------------------------------------------------ *

	ChiudiDatiUltimaImpaginazione

* ------------------------------------------------------------------------ */
static errorixtension XTAPI ChiudiDatiUltimaImpaginazione() throw()
{
	gErrore = FSClose(gIdFileUltimaImpaginazione);
	if (gErrore != noErr) 
	{
		// errore sulla chiusura del file dell'ultima impaginazione
		return(kErroreDiSistema);
	}	

#if 0 // OPERATION
	gErrore = CloseWD(gWDCartellaUltimaImpaginazione);
	if (gErrore != noErr) 
	{
		/* errore sulla chiusura del wd della cartella dell'ultima impaginazione */
		StampaDebug("CloseWD", (long) gErrore);
		return(kErroreDiSistema);
	}
#endif // 0 OPERATION
	return(kNessunErrore);
} // ChiudiDatiUltimaImpaginazione

/* ------------------------------------------------------------------------ *

	ScriviNecrologi

* ------------------------------------------------------------------------ */
static void XTAPI ScriviNecrologi(defunto *ptrdefunto, int16 numerodefunti) throw()
{
	// per sapere dove scrivere */
	int32 lLunghezza = 0;
	// per stampare i necrologi 
	int16 lIndice = 0;
	// per stampare i necrologi 
	defunto *lPtrDefunto = NULL;
	// per scorrere la lista dei necrologi associati ad uno stesso defunto 
	necrologio *lPtrNecrologio = NULL;
	
	// posizionamento in fondo per la scrittura 
	GetEOF(gIdFileUltimaImpaginazione, &lLunghezza);
	SetFPos(gIdFileUltimaImpaginazione, fsFromStart, lLunghezza);
	
	// numero dei defunti 
	sprintf((char*)gStringaC, "%d\r", numerodefunti);
	lLunghezza = STRLEN(gStringaC);
	FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);

	// stampo tutti i necrologi 
	for (lIndice = 0; lIndice < numerodefunti; lIndice++) 
	{	
		// imposto il puntatore ai defunti 	
		lPtrDefunto = ptrdefunto + lIndice;
		
		// codice defunto 
		sprintf((char*)gStringaC, "%s\r", (*lPtrDefunto).codiceDefunto);
		lLunghezza = STRLEN(gStringaC);
		FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);

		// rigore 
		sprintf((char*)gStringaC, "%c\r", (*lPtrDefunto).tipoRigore);
		lLunghezza = STRLEN(gStringaC);
		FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);

		// altezza totale 
		sprintf((char*)gStringaC, "%lf\r", (*lPtrDefunto).altezzaTotaleDoubleFixed);
		lLunghezza = STRLEN(gStringaC);
		FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);

		// numero necrologi 
		sprintf((char*)gStringaC, "%d\r", (*lPtrDefunto).numeroNecrologi);
		lLunghezza = STRLEN(gStringaC);
		FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);

		// imposto il puntatore ai singoli necrologi del defunto codice A 
		lPtrNecrologio = (*lPtrDefunto).elencoNecrologiA;

		while (1) 
		{
			if (lPtrNecrologio == NULL) 
				break;
			
			// codice box 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).codiceBox);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// tipo gruppo 
			sprintf((char*)gStringaC, "%c\r", (*lPtrNecrologio).tipoGruppo);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// altezza box 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).altezzaBox);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// spazio tra i paragrafi 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).spazioTraParagrafi);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// numero paragrafi con spazio diverso da zero 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).numeroParagrafiDiversi);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// numero paragrafi 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).numeroParagrafi);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// numero linee 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).numeroLinee);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// collegato 
			sprintf((char*)gStringaC, "%d\r", (*lPtrNecrologio).collegato);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// avanzo il puntatore 
			lPtrNecrologio = (*lPtrNecrologio).successivo;
		}
		
		// chiudo i necrologi di codice A 
		sprintf((char*)gStringaC, "%c\r", kFineCodiceA);
		lLunghezza = STRLEN(gStringaC);
		FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
		
		// imposto il puntatore ai singoli necrologi del defunto codice B 
		lPtrNecrologio = (*lPtrDefunto).elencoNecrologiB;

		while (1) 
		{	
			if (lPtrNecrologio == NULL) 
				break;
			
			// codice box 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).codiceBox);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// tipo gruppo 
			sprintf((char*)gStringaC, "%c\r", (*lPtrNecrologio).tipoGruppo);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// altezza box 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).altezzaBox);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// spazio tra i paragrafi 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).spazioTraParagrafi);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// numero paragrafi con spazio diverso da zero 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).numeroParagrafiDiversi);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// numero paragrafi 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).numeroParagrafi);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// numero linee 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).numeroLinee);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// collegato 
			sprintf((char*)gStringaC, "%d\r", (*lPtrNecrologio).collegato);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// avanzo il puntatore 
			lPtrNecrologio = (*lPtrNecrologio).successivo;
		}
		
		// chiudo i necrologi di codice B 
		sprintf((char*)gStringaC, "%c\r", kFineCodiceB);
		lLunghezza = STRLEN(gStringaC);
		FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
		
		// imposto il puntatore ai singoli necrologi del defunto codice C 
		lPtrNecrologio = (*lPtrDefunto).elencoNecrologiC;

		while (1) 
		{
			if (lPtrNecrologio == NULL) 
				break;
			
			// codice box 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).codiceBox);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// tipo gruppo 
			sprintf((char*)gStringaC, "%c\r", (*lPtrNecrologio).tipoGruppo);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// altezza box 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).altezzaBox);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// spazio tra i paragrafi 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).spazioTraParagrafi);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// numero paragrafi con spazio diverso da zero 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).numeroParagrafiDiversi);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// numero paragrafi 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).numeroParagrafi);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// numero linee 
			sprintf((char*)gStringaC, "%ld\r", (*lPtrNecrologio).numeroLinee);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// collegato 
			sprintf((char*)gStringaC, "%d\r", (*lPtrNecrologio).collegato);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
			
			// avanzo il puntatore 
			lPtrNecrologio = (*lPtrNecrologio).successivo;
		}
		
		// chiudo il defunto 
		sprintf((char*)gStringaC, "%c\r", kFineDefunto);
		lLunghezza = STRLEN(gStringaC);
		FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
	}
} // ScriviNecrologi

/* ------------------------------------------------------------------------ *

	ScriviInfoNecrologi

* ------------------------------------------------------------------------ */
errorixtension XTAPI ScriviInfoNecrologi() throw()
{
	// apre il file per salvare i dati dell'ultima impaginazione 
	gErroreXtension = ApriDatiUltimaImpaginazione(gNomeDati, TRUE);
	if (gErroreXtension != kNessunErrore) 
	{
		// impossibile prendere i dati dell'ultima impaginazione
		return(gErroreXtension);
	}

	// scrittura delle info sui documenti
	ScriviInfoDocumento();
	
	// scrittura delle info sui necrologi
	ScriviNecrologi(gPtrAnnunci, gNumeroAnnunci);
	ScriviNecrologi(gPtrAnniversari, gNumeroAnniversari);
	ScriviNecrologi(gPtrRingraziamenti, gNumeroRingraziamenti);
	
	gErroreXtension = ChiudiDatiUltimaImpaginazione();
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nella chiusura dei dati dell'ultima impaginazione
		return(gErroreXtension);
	}
	
	return(kNessunErrore);
} // ScriviInfoNecrologi


/* ------------------------------------------------------------------------ *

	ScriviInfoDocumento

* ------------------------------------------------------------------------ */
static void XTAPI ScriviInfoDocumento() throw()
{
	// per sapere dove scrivere
	int32 lLunghezza = 0;
	
	// posizionamento in fondo per la scrittura
	GetEOF(gIdFileUltimaImpaginazione, &lLunghezza);
	SetFPos(gIdFileUltimaImpaginazione, fsFromStart, lLunghezza);
	
	// ingombro totale
	sprintf((char*)gStringaC, "%ld\r", PrendiIngombroTotale());
	lLunghezza = STRLEN(gStringaC);
	FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);

	// spazio tra i necrologi totale
	sprintf((char*)gStringaC, "%ld\r", PrendiSpazioTraNecrologiTotale());
	lLunghezza = STRLEN(gStringaC);
	FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);

	// allargamento totale
	sprintf((char*)gStringaC, "%ld\r", PrendiAllargamentoTotale());
	lLunghezza = STRLEN(gStringaC);
	FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);

	// restringimento totale
	sprintf((char*)gStringaC, "%ld\r", PrendiRestringimentoTotale());
	lLunghezza = STRLEN(gStringaC);
	FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
} // ScriviInfoDocumento

/* ------------------------------------------------------------------------ *

	LeggiNecrologi

* ------------------------------------------------------------------------ */
static errorixtension XTAPI LeggiNecrologi(uchar **ptrdati, defunto **ptrdefunto, int16 *ptrnumerodefunti) throw()
{
	// per determinare la lunghezza del testo da leggere
	Size lContatore = 0;
	// punta all'inizio del testo
	uchar *lPtrInizio = NULL;
	// per scorrere i necrologi
	int16 lIndice = 0;
	// per scorrere i defunti
	defunto *lPtrDefunto = NULL;
	// per scorrere la lista dei necrologi associati ad uno stesso defunto
	necrologio *lPtrNecrologio = NULL;
	// per agganciare i puntatori dell'elenco dei necrologi del defunto
	necrologio **lPtrPrecedente = NULL;
	// puntatore al puntatore dell'ultimo necrologio della lista
	necrologio **lPtrUltimo = NULL;
	
	// leggo il numero di defunti da creare
	lPtrInizio = (*ptrdati);
	lContatore = 0;
	while (1) 
	{
		if (**ptrdati == kRitornoACapo) 
			break;
		(*ptrdati)++;
		lContatore++;
	}
	if (lContatore == 0) 
		(*ptrnumerodefunti) = 0;
	else 
	{
		// devo prelevare il valore della stringa puntato da lPtrInizio
		BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
		gStringaC[lContatore] = kFineFile;
		sscanf((char*)gStringaC,"%d", ptrnumerodefunti);
	}
	
	if ((*ptrnumerodefunti) == 0) 
	{
		// non ci sono defunti da leggere
		(*ptrdati)++;
		return(kNessunErrore);
	}
	
	// creazione dello spazio di memoria necessario
	gErroreXtension = CostruisciNecrologi(ptrdefunto, (*ptrnumerodefunti));
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nell'allocazione della memoria
		return(gErroreXtension);
	}

	// leggo tutti i necrologi
	for (lIndice = 0; lIndice < (*ptrnumerodefunti); lIndice++) 
	{
		// imposto il puntatore ai defunti 
		lPtrDefunto = (*ptrdefunto) + lIndice;

		// leggo il codice del defunto
		(*ptrdati)++;
		lPtrInizio = (*ptrdati);
		lContatore = 0;
		while (1) 
		{
			if (**ptrdati == kRitornoACapo) 
				break;
			(*ptrdati)++;
			lContatore++;
		}
		// copio il codice del defunto
		BlockMove((Ptr) lPtrInizio,
				  (Ptr) (*lPtrDefunto).codiceDefunto,
				  lContatore);
		// inserisco il carattere '\0' alla fine del testo
		(*lPtrDefunto).codiceDefunto[lContatore] = kFineFile;
		
		// proseguo la lettura
		(*ptrdati)++;
		
		// leggo il rigore
		lPtrInizio = (*ptrdati);
		lContatore = 0;
		while (1) 
		{
			if (**ptrdati == kRitornoACapo) 
				break;
			(*ptrdati)++;
			lContatore++;
		}
		if (lContatore == 1) 
			(*lPtrDefunto).tipoRigore = (tiporigore)lPtrInizio[0];
		else 
			(*lPtrDefunto).tipoRigore = kNessunRigore;
		
		// proseguo la lettura
		(*ptrdati)++;
		
		// leggo l'altezza del defunto
		lPtrInizio = (*ptrdati);
		lContatore = 0;
		while (1) 
		{
			if (**ptrdati == kRitornoACapo) 
				break;
			(*ptrdati)++;
			lContatore++;
		}
		if (lContatore == 0) 
			(*lPtrDefunto).altezzaTotaleDoubleFixed = 0;
		else 
		{
			// devo prelevare il valore della stringa puntato da lPtrInizio
			BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
			gStringaC[lContatore] = kFineFile;
			sscanf((char*)gStringaC,"%lf", &((*lPtrDefunto).altezzaTotaleDoubleFixed));
		}
		
		// proseguo la lettura
		(*ptrdati)++;
		
		// leggo il numero dei necrologi
		lPtrInizio = (*ptrdati);
		lContatore = 0;
		while (1) 
		{
			if (**ptrdati == kRitornoACapo) 
				break;
			(*ptrdati)++;
			lContatore++;
		}
		if (lContatore == 0) 
			(*lPtrDefunto).numeroNecrologi = 0;
		else 
		{
			// devo prelevare il valore della stringa puntato da lPtrInizio
			BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
			gStringaC[lContatore] = kFineFile;
			sscanf((char*)gStringaC,"%d", &((*lPtrDefunto).numeroNecrologi));
		}
		
		// proseguo la lettura
		(*ptrdati)++;
		
		// inserisco il valore per il numero delle colonne
		(*lPtrDefunto).numeroColonne = 0;
		
		// imposto gli indirizzi ai puntatori codice A
		lPtrPrecedente = &((*lPtrDefunto).elencoNecrologiA);
		lPtrUltimo = &((*lPtrDefunto).ultimoNecrologioA);
		
		while (1) 
		{
			// controllo se sono arrivato alla fine del defunto
			//o alla fine del file
			if (**ptrdati == kFineCodiceA) 
				break;
			
			// creo la memoria per il primo necrologio
			lPtrNecrologio = (necrologio *) NewPtrClear(sizeof(necrologio));
			if (lPtrNecrologio == NULL) 
			{
				// errore nella creazione della memoria
				return(kErroreMemoria);
			}
			
			// aggancio il puntatore al defunto
			(*lPtrPrecedente) = lPtrNecrologio;
			(*lPtrUltimo) = lPtrNecrologio;
			
			// leggo il codice del box
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).codiceBox = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).codiceBox));
			}
			
			// leggo il tipo gruppo
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 1) 
				(*lPtrNecrologio).tipoGruppo = lPtrInizio[0];
			else 
				(*lPtrNecrologio).tipoGruppo = kNessunGruppo;

			// leggo l'altezza del box
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).altezzaBox = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).altezzaBox));
			}

			// leggo lo spazio tra i paragrafi
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).spazioTraParagrafi = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).spazioTraParagrafi));
			}

			// leggo il numero dei paragrafi diversi
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).numeroParagrafiDiversi = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).numeroParagrafiDiversi));
			}

			// leggo il numero dei paragrafi
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).numeroParagrafi = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).numeroParagrafi));
			}

			// leggo il numero delle linee
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).numeroLinee = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).numeroLinee));
			}
			
			// leggo il collegato
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).collegato = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%d", &((*lPtrNecrologio).collegato));
			}
			
			// imposto il cambio pagina
			(*lPtrNecrologio).cambioColonna = FALSE;
			
			// imposto i puntatori per proseguire l'inserimento nella lista
			(*lPtrNecrologio).successivo = NULL;
			lPtrPrecedente = &((*lPtrNecrologio).successivo);
						
			// proseguo la lettura
			(*ptrdati)++;
		}

		// proseguo la lettura
		(*ptrdati)++;
		(*ptrdati)++;
		
		// imposto gli indirizzi ai puntatori codice B
		lPtrPrecedente = &((*lPtrDefunto).elencoNecrologiB);
		lPtrUltimo = &((*lPtrDefunto).ultimoNecrologioB);
		
		while (1) 
		{
			// controllo se sono arrivato alla fine del defunto
			// o alla fine del file
			if (**ptrdati == kFineCodiceB) 
				break;
			
			// creo la memoria per il primo necrologio
			lPtrNecrologio = (necrologio *) NewPtrClear(sizeof(necrologio));
			if (lPtrNecrologio == NULL) 
			{
				// errore nella creazione della memoria
				return(kErroreMemoria);
			}
			
			// aggancio il puntatore al defunto
			(*lPtrPrecedente) = lPtrNecrologio;
			(*lPtrUltimo) = lPtrNecrologio;
			
			// leggo il codice del box
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).codiceBox = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).codiceBox));
			}
			
			// leggo il tipo gruppo
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 1) 
				(*lPtrNecrologio).tipoGruppo = lPtrInizio[0];
			else 
				(*lPtrNecrologio).tipoGruppo = kNessunGruppo;

			// leggo l'altezza del box
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).altezzaBox = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).altezzaBox));
			}

			// leggo lo spazio tra i paragrafi
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).spazioTraParagrafi = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).spazioTraParagrafi));
			}

			// leggo il numero dei paragrafi diversi
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).numeroParagrafiDiversi = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).numeroParagrafiDiversi));
			}

			// leggo il numero dei paragrafi
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).numeroParagrafi = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).numeroParagrafi));
			}

			// leggo il numero delle linee
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).numeroLinee = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).numeroLinee));
			}
			
			// leggo il collegato
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).collegato = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%d", &((*lPtrNecrologio).collegato));
			}
			
			// imposto il cambio pagina
			(*lPtrNecrologio).cambioColonna = FALSE;

			// imposto i puntatori per proseguire l'inserimento nella lista
			(*lPtrNecrologio).successivo = NULL;
			lPtrPrecedente = &((*lPtrNecrologio).successivo);
						
			// proseguo la lettura
			(*ptrdati)++;
		}

		// proseguo la lettura 
		(*ptrdati)++;
		(*ptrdati)++;
		
		// imposto gli indirizzi ai puntatori
		lPtrPrecedente = &((*lPtrDefunto).elencoNecrologiC);
		lPtrUltimo = &((*lPtrDefunto).ultimoNecrologioC);
		
		while (1) 
		{
			// controllo se sono arrivato alla fine del defunto
			// o alla fine del file
			if (**ptrdati == kFineDefunto) 
				break;
			
			// creo la memoria per il primo necrologio 
			lPtrNecrologio = (necrologio *) NewPtrClear(sizeof(necrologio));
			if (lPtrNecrologio == NULL) 
			{
				// errore nella creazione della memoria
				return(kErroreMemoria);
			}
			
			// aggancio il puntatore al defunto
			(*lPtrPrecedente) = lPtrNecrologio;
			(*lPtrUltimo) = lPtrNecrologio;
			
			// leggo il codice del box
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).codiceBox = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).codiceBox));
			}
			
			// leggo il tipo gruppo
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 1) 
				(*lPtrNecrologio).tipoGruppo = lPtrInizio[0];
			else 
				(*lPtrNecrologio).tipoGruppo = kNessunGruppo;

			// leggo l'altezza del box
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).altezzaBox = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).altezzaBox));
			}

			// leggo lo spazio tra i paragrafi
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).spazioTraParagrafi = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).spazioTraParagrafi));
			}

			// leggo il numero dei paragrafi diversi
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).numeroParagrafiDiversi = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).numeroParagrafiDiversi));
			}

			// leggo il numero dei paragrafi
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).numeroParagrafi = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).numeroParagrafi));
			}

			// leggo il numero delle linee
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0)
				(*lPtrNecrologio).numeroLinee = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%ld", &((*lPtrNecrologio).numeroLinee));
			}
			
			// leggo il collegato
			(*ptrdati)++;
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) 
			{
				if (**ptrdati == kRitornoACapo) 
					break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) 
				(*lPtrNecrologio).collegato = 0;
			else 
			{
				// devo prelevare il valore della stringa puntato da lPtrInizio
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf((char*)gStringaC,"%d", &((*lPtrNecrologio).collegato));
			}
			
			// imposto il cambio pagina
			(*lPtrNecrologio).cambioColonna = FALSE;

			// imposto i puntatori per proseguire l'inserimento nella lista
			(*lPtrNecrologio).successivo = NULL;
			lPtrPrecedente = &((*lPtrNecrologio).successivo);
						
			// proseguo la lettura
			(*ptrdati)++;
		}

		// proseguo la lettura
		(*ptrdati)++;
	}
	
	// proseguo la lettura
	(*ptrdati)++;
	
	return(kNessunErrore);
} // LeggiNecrologi

/* ------------------------------------------------------------------------ *

	LeggiInfoDocumento

* ------------------------------------------------------------------------ */
static void XTAPI LeggiInfoDocumento(uchar **ptrdati) throw()
{
	// per determinare la lunghezza del testo da leggere
	Size lContatore = 0;
	// punta all'inizio del testo
	uchar *lPtrInizio = NULL;
	// per impostare il valore letto
	uint32 lValore = 0;
	
	// leggo l'ingombro totale
	lPtrInizio = (*ptrdati);
	lContatore = 0;
	while (1) 
	{
		if (**ptrdati == kRitornoACapo) 
			break;
		(*ptrdati)++;
		lContatore++;
	}
	if (lContatore == 0) 
		lValore = 0;
	else 
	{
		// devo prelevare il valore della stringa puntato da lPtrInizio
		BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
		gStringaC[lContatore] = kFineFile;
		sscanf((char*)gStringaC,"%ld", &lValore);
	}
	ImpostaIngombroTotale(lValore);
	
	// leggo lo spazio tra i necrologi
	(*ptrdati)++;
	lPtrInizio = (*ptrdati);
	lContatore = 0;
	while (1) 
	{
		if (**ptrdati == kRitornoACapo) 
			break;
		(*ptrdati)++;
		lContatore++;
	}
	if (lContatore == 0) 
		lValore = 0;
	else 
	{
		// devo prelevare il valore della stringa puntato da lPtrInizio
		BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
		gStringaC[lContatore] = kFineFile;
		sscanf((char*)gStringaC,"%ld", &lValore);
	}
	ImpostaSpazioTraNecrologiTotale(lValore);
	
	// leggo l'allargamento
	(*ptrdati)++;
	lPtrInizio = (*ptrdati);
	lContatore = 0;
	while (1) 
	{
		if (**ptrdati == kRitornoACapo) 
			break;
		(*ptrdati)++;
		lContatore++;
	}
	if (lContatore == 0) 
		lValore = 0;
	else 
	{
		// devo prelevare il valore della stringa puntato da lPtrInizio
		BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
		gStringaC[lContatore] = kFineFile;
		sscanf((char*)gStringaC,"%ld", &lValore);
	}
	ImpostaAllargamentoTotale(lValore);
	
	// leggo il restringimento
	(*ptrdati)++;
	lPtrInizio = (*ptrdati);
	lContatore = 0;
	while (1) 
	{
		if (**ptrdati == kRitornoACapo) 
			break;
		(*ptrdati)++;
		lContatore++;
	}
	if (lContatore == 0) 
		lValore = 0;
	else 
	{
		// devo prelevare il valore della stringa puntato da lPtrInizio
		BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
		gStringaC[lContatore] = kFineFile;
		sscanf((char*)gStringaC,"%ld", &lValore);
	}
	ImpostaRestringimentoTotale(lValore);
	
	// proseguo la lettura
	(*ptrdati)++;
} // LeggiInfoDocumento

/* ------------------------------------------------------------------------ *

	LeggiInfoNecrologi

* ------------------------------------------------------------------------ */
errorixtension XTAPI LeggiInfoNecrologi() throw()
{
	// per puntata al singolo carattere dell'handle 
	uchar *lPtrDati = NULL;
	
	// libero le strutture costruite precedentemente
	LiberaInfoDefunti();

	// apre il file per salvare i dati dell'ultima impaginazione
	gErroreXtension = ApriDatiUltimaImpaginazione(gNomeDati, FALSE);
	if (gErroreXtension != kNessunErrore) 
	{
		// impossibile prendere i dati dell'ultima impaginazione
		return(gErroreXtension);
	}
	
	// leggo il contenuto dell'ultima impaginazione
	gErroreXtension = LeggiContenutoFile(gIdFileUltimaImpaginazione,
										 &gHndlUltimaImpaginazione);
	if (gErroreXtension != kNessunErrore) 
	{
		// impossibile leggere i dati dell'ultima impaginazione
		return(gErroreXtension);
	}
	
	gErroreXtension = ChiudiDatiUltimaImpaginazione();
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nella chiusura dei dati dell'ultima impaginazione
		return(gErroreXtension);
	}
	
	// imposto il puntatore al file dei dati
	lPtrDati = (uchar *) (*gHndlUltimaImpaginazione);

	// lettura delle info sul documento
	LeggiInfoDocumento(&lPtrDati);
	
	gErroreXtension = LeggiNecrologi(&lPtrDati, &gPtrAnnunci, &gNumeroAnnunci);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sulla lettura degli annunci
		return(gErroreXtension);
	}
	
	gErroreXtension = LeggiNecrologi(&lPtrDati, &gPtrAnniversari, &gNumeroAnniversari);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sulla lettura degli anniversari
		return(gErroreXtension);
	}
	
	gErroreXtension = LeggiNecrologi(&lPtrDati, &gPtrRingraziamenti, &gNumeroRingraziamenti);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sulla lettura dei ringraziamenti
		return(gErroreXtension);
	}
	
	LiberaHandle(&gHndlUltimaImpaginazione);

	return(kNessunErrore);
} // LeggiInfoNecrologi

/* ------------------------------------------------------------------------ *

	ScriviDatiSpazioColonna

* ------------------------------------------------------------------------ */
static void XTAPI ScriviDatiSpazioColonna() throw()
{
	// per sapere dove scrivere
	int32 lLunghezza = 0;
	// per scorrere le colonne
	int16 lColonna = 0;
	// per scorrere le pagine
	int16 lPagina = 0;
	// per stampare i necrologi
	spaziocolonna *lPtrSpazioColonna = NULL;
	
	// posizionamento in fondo per la scrittura
	GetEOF(gIdFileUltimaImpaginazione, &lLunghezza);
	SetFPos(gIdFileUltimaImpaginazione, fsFromStart, lLunghezza);
	
	// stampo i daiti delle colonne
	for (lPagina = 1; lPagina <= PrendiNumeroPagine(); lPagina++) 
	{
		for (lColonna = 1; lColonna <= PrendiNumeroColonne(); lColonna++) 
		{	
			// prendo le info della colonna corrente
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);
			
			// margine alto
			sprintf((char*)gStringaC, "%ld\r", (*lPtrSpazioColonna).margineAlto);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);

			// margine basso
			sprintf((char*)gStringaC, "%ld\r", (*lPtrSpazioColonna).margineBasso);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);

			// spazio disponibile
			sprintf((char*)gStringaC, "%ld\r", (*lPtrSpazioColonna).spazioDisponibile);
			lLunghezza = STRLEN(gStringaC);
			FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
		}
	}	
} // ScriviDatiSpazioColonna

/* ------------------------------------------------------------------------ *

	ScriviColonneDocumento

* ------------------------------------------------------------------------ */
static void XTAPI ScriviColonneDocumento() throw()
{
	// per sapere dove scrivere
	int32 lLunghezza = 0;
	
	// posizionamento in fondo per la scrittura
	GetEOF(gIdFileUltimaImpaginazione, &lLunghezza);
	SetFPos(gIdFileUltimaImpaginazione, fsFromStart, lLunghezza);
	
	// numero pagine
	sprintf((char*)gStringaC, "%d\r", PrendiNumeroPagine());
	lLunghezza = STRLEN(gStringaC);
	FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);

	// numero colonne
	sprintf((char*)gStringaC, "%d\r", PrendiNumeroColonne());
	lLunghezza = STRLEN(gStringaC);
	FSWrite(gIdFileUltimaImpaginazione, &lLunghezza, (Ptr) gStringaC);
} // ScriviColonneDocumento

/* ------------------------------------------------------------------------ *

	ScriviSpazioColonna

* ------------------------------------------------------------------------ */
errorixtension XTAPI ScriviSpazioColonna() throw()
{
	// apre il file per salvare i dati delle colonne
	gErroreXtension = ApriDatiUltimaImpaginazione(gNomeColonne, TRUE);
	if (gErroreXtension != kNessunErrore) 
	{
		// impossibile prendere i dati dell'ultima impaginazione
		return(gErroreXtension);
	}

	// scrittura delle info su numero di colonne e pagine
	ScriviColonneDocumento();
	
	// scrittura dei dati delle colonne
	ScriviDatiSpazioColonna();
	
	gErroreXtension = ChiudiDatiUltimaImpaginazione();
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nella chiusura dei dati dell'ultima impaginazione
		return(gErroreXtension);
	}
	
	return(kNessunErrore);
} // ScriviSpazioColonna

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

/* Variabili */
/* Variabili */
/* Variabili */

static short gWDCartellaUltimaImpaginazione = 0;
/* working directory della cartella dell'ultima impaginazione */	

#if kOLD

static errorixtension LeggiDatiSpazioColonna(char **ptrdati);
static errorixtension LeggiDatiSpazioColonna(char **ptrdati)
/* ------------------------------------------------------------------------ *

	LeggiDatiSpazioColonna

	11 Febbraio 1996						Stefano

	Descrizione:
	Legge sul file dell'ultima impaginazione i dati delle colonne

	Parametri:
	ptrdati - puntatore al puntatore del carattere da leggere del file dell'ultima
	impaginazione

	Ritorno:
	ritorna l'eventuale errore dell'xtension

* ------------------------------------------------------------------------ */

{
	Size lContatore = 0;
	/* per determinare la lunghezza del testo da leggere */	
	char *lPtrInizio = NULL;
	/* punta all'inizio del testo */
	int lPagina = 0;
	/* indice per il for sulle pagine */
	int lColonna = 0;
	/* indice per il for delle colonne */
	spaziocolonna *lPtrSpazioColonna = NULL;
	/* per impostare il descrittore dello spazio delle colonne */
	
	/* libero il vecchio puntatore allo spazio colonna */
	if (gPtrSpazioColonna != NULL) {
		DisposPtr((Ptr) gPtrSpazioColonna);
		gPtrSpazioColonna = NULL;
	}
	
	/* imposta il numero di pagine */
	ImpostaNumeroPagine(numpages());
	
	/* 	creo la memoria per lo spazio colonna */
	gPtrSpazioColonna = (spaziocolonna *) NewPtrAlignClear(PrendiNumeroPagine() *
													  PrendiNumeroColonne() *
													  sizeof(spaziocolonna));
	if (gPtrSpazioColonna == NULL) {
		StampaDebug("NewPtrClear", 0);
		return(kErroreMemoria);
	}
													  	
	/* libero il vecchio puntatore allo spazio colonna fuori */
	if (gPtrSpazioColonnaFuori != NULL) {
		DisposPtr((Ptr) gPtrSpazioColonnaFuori);
		gPtrSpazioColonnaFuori = NULL;
	}
	
	/* creo lo spazio colonna per i box fuori */
	gPtrSpazioColonnaFuori = (spaziocolonna *) NewPtrClear(sizeof(spaziocolonna));
	if (gPtrSpazioColonnaFuori == NULL) {
		/* errore nell'allocazione della memoria */
		StampaDebug("NewPtrClear", 0);
		return(kErroreMemoria);
	}

	/* inizializzo lo spazio disponibile totale */
	InizializzaSpazioDisponibileTotale();
	
	for (lPagina = 1; lPagina <= PrendiNumeroPagine(); lPagina++) {
		for (lColonna = 1; lColonna <= PrendiNumeroColonne(); lColonna++ ) {
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);
			
			/* leggo il margine alto */
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) {
				if (**ptrdati == kRitornoACapo) break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) (*lPtrSpazioColonna).margineAlto = 0;
			else {
				/* devo prelevare il valore della stringa puntato da lPtrInizio */
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf(gStringaC,"%ld", &((*lPtrSpazioColonna).margineAlto));
			}
			
			/* proseguo la lettura */
			(*ptrdati)++;
		
			/* leggo il margine basso */
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) {
				if (**ptrdati == kRitornoACapo) break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) (*lPtrSpazioColonna).margineBasso = 0;
			else {
				/* devo prelevare il valore della stringa puntato da lPtrInizio */
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf(gStringaC,"%ld", &((*lPtrSpazioColonna).margineBasso));
			}
			
			/* proseguo la lettura */
			(*ptrdati)++;
		
			/* leggo lo spazio disponibile */
			lPtrInizio = (*ptrdati);
			lContatore = 0;
			while (1) {
				if (**ptrdati == kRitornoACapo) break;
				(*ptrdati)++;
				lContatore++;
			}
			if (lContatore == 0) (*lPtrSpazioColonna).spazioDisponibile = 0;
			else {
				/* devo prelevare il valore della stringa puntato da lPtrInizio */
				BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
				gStringaC[lContatore] = kFineFile;
				sscanf(gStringaC,"%ld", &((*lPtrSpazioColonna).spazioDisponibile));
			}
			
			/* proseguo la lettura */
			(*ptrdati)++;		

			(*lPtrSpazioColonna).spazioOttimale = 0;
			(*lPtrSpazioColonna).spazioOccupato = 0;
			(*lPtrSpazioColonna).spazioTraNecrologi = 0;
			(*lPtrSpazioColonna).numeroLinee = 0;
			(*lPtrSpazioColonna).numeroNecrologi = 0;
			(*lPtrSpazioColonna).numeroNecrologiDefunti = 0;
			(*lPtrSpazioColonna).numeroNecrologiFondoDefunti = 0;
			(*lPtrSpazioColonna).chiusa = FALSE;
			(*lPtrSpazioColonna).testaNecrologi = NULL;
			(*lPtrSpazioColonna).codaNecrologi = NULL;
			(*lPtrSpazioColonna).testaDefunti = NULL;
			(*lPtrSpazioColonna).codaDefunti = NULL;
			(*lPtrSpazioColonna).testaFondoDefunti = NULL;
			(*lPtrSpazioColonna).codaFondoDefunti = NULL;
			
			/* incremento lo spazio disponibile totale */
			IncrementaSpazioDisponibileTotale((*lPtrSpazioColonna).spazioDisponibile);
		}
	}
		
	return(kNessunErrore);
}

static void LeggiColonneDocumento(char **ptrdati);
static void LeggiColonneDocumento(char **ptrdati)
/* ------------------------------------------------------------------------ *

	LeggiColonneDocumento

	7 Febbraio 1996						Stefano

	Descrizione:
	Legge sul file dell'ultima impaginazione le colonne del documento

	Parametri:
	ptrdati - puntatore al puntatore del carattere da leggere del file dell'ultima
	impaginazione

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */

{
	Size lContatore = 0;
	/* per determinare la lunghezza del testo da leggere */	
	char *lPtrInizio = NULL;
	/* punta all'inizio del testo */
	int lValore = 0;
	/* per impostare il valore letto */

	/* leggo il numero di pagine */
	lPtrInizio = (*ptrdati);
	lContatore = 0;
	while (1) {
		if (**ptrdati == kRitornoACapo) break;
		(*ptrdati)++;
		lContatore++;
	}
	if (lContatore == 0) lValore = 0;
	else {
		/* devo prelevare il valore della stringa puntato da lPtrInizio */
		BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
		gStringaC[lContatore] = kFineFile;
		sscanf(gStringaC,"%d", &lValore);
	}
	gNumeroPagineInterne = lValore;
	
	/* leggo il numero di colonne */
	(*ptrdati)++;
	lPtrInizio = (*ptrdati);
	lContatore = 0;
	while (1) {
		/* per saltare l'eventuale ritorno a capo */
		if (**ptrdati == kRitornoACapo) 
			break;
		(*ptrdati)++;
		lContatore++;
	}
	
	if (lContatore == 0) lValore = 0;
	else {
		/* devo prelevare il valore della stringa puntato da lPtrInizio */
		BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
		gStringaC[lContatore] = kFineFile;
		sscanf(gStringaC,"%d", &lValore);
	}
	gNumeroColonne = lValore;
		
	/* proseguo la lettura */
	(*ptrdati)++;
}

errorixtension LeggiSpazioColonna(void)
/* ------------------------------------------------------------------------ *

	LeggiSpazioColonna

	11 Febbraio 1996						Stefano

	Descrizione:
	Legge dal file dell'ultima impaginazione le informazione sui dati delle colonne

	Parametri:
	nessuno

	Ritorno:
	ritorna l'eventuale errore dell'xtension

* ------------------------------------------------------------------------ */
{
	char *lPtrDati = NULL;
	/* per puntata al singolo carattere dell'handle  */

	/* apre il file per leggere i dati dell'ultima impaginazione */
	gErroreXtension = ApriDatiUltimaImpaginazione(gNomeColonneUltimaImpaginazione, FALSE);
	if (gErroreXtension != kNessunErrore) {
		/* impossibile prendere i dati dell'ultima impaginazione */
		StampaDebug("ApriDatiUltimaImpaginazione", (long) gErroreXtension);
		return(gErroreXtension);
	}
	
	/* leggo il contenuto dell'ultima impaginazione */
	gErroreXtension = LeggiContenutoFile(gIdFileUltimaImpaginazione,
										 &gHndlUltimaImpaginazione);
	if (gErroreXtension != kNessunErrore) {
		/* impossibile leggere i dati dell'ultima impaginazione */
		StampaDebug("LeggiContenutoFile", (long) gErroreXtension);
		return(gErroreXtension);
	}
	
	gErroreXtension = ChiudiDatiUltimaImpaginazione();
	if (gErroreXtension != kNessunErrore) {
		/* errore nella chiusura dei dati dell'ultima impaginazione */
		StampaDebug("ChiudiDatiUltimaImpaginazione", (long) gErroreXtension);
		return(gErroreXtension);
	}
	
	/* imposto il puntatore al file dei dati */
	lPtrDati = (char *) (*gHndlUltimaImpaginazione);

	/* lettura del numero di colonne sul documento */
	LeggiColonneDocumento(&lPtrDati);
	
	gErroreXtension = LeggiDatiSpazioColonna(&lPtrDati);
	if (gErroreXtension != kNessunErrore) {
		/* errore sulla lettura degli annunci */
		StampaDebug("LeggiNecrologi", (long) gErroreXtension);
		return(gErroreXtension);
	}
	
	DealloccaHandle(&gHndlUltimaImpaginazione);

	return(kNessunErrore);
}
#endif 
#endif // 0 NECRO v1

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/