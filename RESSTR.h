/*!
	@header			RisorseStringhe.h
	@abstract		operazione eseguite dall'XT
	@discussion 		File incluso dal file Stringhe.cpp.
	
					5 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
*/

#ifndef RisorseStinghe_h
#define RisorseStinghe_h

#ifndef Errori_h
#include "Errori.h"
#endif // Errori_h

// ENUMS
// ENUMS
// ENUMS

/*!
	@enum 			messaggixtension
	@abstract		messaggi dell'XT.
	@discussion		identificatori dei messaggi dell'xtension
*/
typedef enum {
	kInizializzazione = 1,
	kUscita = 2,
	kImpaginazioneDocumento = 3,
	kFineImpaginazione = 4,
	kFineLettura = 5,
	kMessaggioBozza = 6,
	kMessaggioIngombro = 7,
	kMessaggioDefinitiva = 8,
	kIngombroInPunti = 9,
	kIngombroInModuli = 10,
	kImpaginazioneAppWare = 11,
	kSpazioDisponibileInPunti = 12,
	kSpazioDisponibileInModuli = 13,
	kIngombroInPagineColonne = 14,
	kControllaElencoImmagini = 15,
	kFineControllaElencoImmagini = 16,
	kInizioCalcoloPosizionamento = 17,
	kFineCalcoloPosizionamento = 18,
	kSpazioDisponibileInPagineColonne = 19,
	kNumeroNecrologi = 20,
	kInizioPosizionamento = 21,
	kFinePosizionamento = 22,
	kSimboloPaxNonPresente = 23,
	kUltimoMessaggio = 24
} messaggixtension;

// DEFINES
// DEFINES
// DEFINES

#define kPrimoMessaggio 1
#define kNumeroMessaggi kUltimoMessaggio - 1

// GLOBALS
// GLOBALS
// GLOBALS

// stringa C di uso generale
extern uchar gStringaC[MAXPATHNAME];

// Stringhe 23000 

// nome dell'Xtension
extern uchar gStrNomeXtension[];
// nome del file di rapporto
extern uchar gStrNomeFileRapporto[];
// testo del messaggio di notifica 
extern uchar  gStrTestoNotifica[];
// stile da usare per stampare il codice del defunto nella stampa bozze
extern uchar  gStrDefunto[];
// stile da usare per stampare il codice del necrologio nella stampa bozze
extern uchar  gStrNecrologio[];
// nome del documento di ingombro */
extern uchar  gStrIngombro[];
// messaggio da presentare per la selezione della cartella delle immagini
extern uchar  gCartellaImmagini[];
// messaggio per la selezione del documento bozza 
extern uchar  gDocumentoBozza[];
// stile da usare per stampare il codice dell'immagine 
extern uchar  gStrStileImmagine[];
// messaggio da presentare per la slezione del documento di ingombro
extern uchar  gDocumentoIngombro[];
// contiene il nome dello stile che ha il filetto superiore
extern uchar  gStrStileFiletto[];
// contiene il nome del documento Xpress dell'ultima impaginazione
extern uchar gUltimaImpaginazione[];
// contiene il nome del documento con i dati dell'ultima impaginazione
extern uchar gNomeDati[];
// contiene il nome del documento con i dati sulle colonne dell'ultima impaginazione
extern uchar gNomeColonne[];

// Stringhe 21000 
extern uchar gStrErrori[kNumeroErrori][64];

// Stringhe 22000
extern uchar gStrMessaggi[kNumeroMessaggi][64];

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			CreaTutteLeStringhe
	@abstract 		gestione stringhe.
	@discussion		Carica e crea tutte le stringhe dell'Xtension.

					5 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI CreaTutteLeStringhe() throw();

#endif // RisorseStinghe_h
