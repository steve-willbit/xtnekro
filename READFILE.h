/*!
	@header			LeggiFile.h
	@abstract		file
	@discussion 		Contiene le descrizioni delle procedure per la lettura e il controllo del file
					da impaginare.
	
					8 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
*/
#ifndef LeggiFile_h
#define LeggiFile_h

#ifndef Errori_h
#include "Errori.h"
#endif // Errori_h

// CONSTS
// CONSTS
// CONSTS

#define kMinore '<'
#define kMaggiore '>'
#define kChiocciola '@'
#define kMeno '-'
#define kPiu '+'
#define kPuntoEVirgola ';'
#define kVirgola ','
#define kAsterisco '*'
#define kSpazio ' '
#define kRitornoACapo '\r'
#define kNuovaLinea '\n'
#define kTabulatore '\t'
#define kFineFile '\0'
#define kNessunGruppo '*'
#define kNessunaPosizione '*'

// ENUMS
// ENUMS
// ENUMS

typedef enum 
{
	kNessunaImpaginazione,
	kImpaginazioneBozza = '1',
	kImpaginazioneIngombro = '2',
	kImpaginazioneDefinitiva = '3'
} tipoimpaginazione;

typedef enum 
{
	kNessunComando,
	kComandoDiFine,
	kTesto,
	kStileForte,
	kStileDebole,
	kImmagine,
	kDefunto = 'd',
	kAnnuncio = 'n',
	kAnniversario = 'a',
	kRingraziamento = 'r',
	kPax = '#'
} tipocomando;

typedef enum {
	kNessunRigore = '*',
	kRigoreDiColonna = 'T',
	kRigoreDiDocumento = 'I'
} tiporigore;

typedef struct 
{
	tipocomando tipoComando;
	uchar *testo;
	Size lunghezzaTesto;
	tiporigore tipoRigore;
	uchar tipoGruppo;
	uchar posizioneNelGruppo;
} quattrod;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			PosizioneEStringaConErrore
	@abstract 		gestione lettura file
	@discussion		Concatena alla stringa di errore la posizione e una porzione del documento dove si
					e' verificato un errore

					11 Aprile 2005 - Fabrizio.

	@param 			posizione contiene la posizione dell'errore
	@param			ptrdocumento puntatore al documento che contiene i comandi da impaginare
	@result  			nessuno.
*/
void XTAPI PosizioneEStringaConErrore(int32 posizione, uchar *ptrdocumento) throw();

/*!
	@function			ControllaComandi
	@abstract 		gestione lettura file
	@discussion		Verifica che all'interno delle parentesi angolari non ci sia il carattere
					\0' di terminazione del file e che i comandi siano sempre aperti e chiusi
					da una parentesi angolare.

					11 Aprile 2005 - Fabrizio.

	@param 			ptrdocumento puntatore al documento che contiene i comandi da impaginare
	@result  			ritorna l'eventuale errore se un comando di impaginazione contiene
					tra le parentesi angolari il carattere di terminazione file.
*/
errorixtension XTAPI ControllaComandi(uchar* ptrdocumento) throw();

/*!
	@function			ControllaPosizioneComandi
	@abstract 		gestione lettura file
	@discussion		Verifica la posizione dei comandi di impaginazione. In particolare si preoccupa
					di evitare che sia cia sia un comando di impaginazione quando non e' ancora
					stato costruito un box di testo

					11 Aprile 2005 - Fabrizio.

	@param 			ptrdocumento puntatore al documento che contiene i comandi da impaginare
	@result  			ritorna l'eventuale errore dell'xt
*/
errorixtension XTAPI ControllaPosizioneComandi(uchar* ptrdocumento) throw();

/*!
	@function			ControllaArgomenti
	@abstract 		gestione lettura file
	@discussion		Verifica che all'interno delle parentesi angolari ci siano tutti i separatori
					previsti dalla sintassi

					11 Aprile 2005 - Fabrizio.

	@param 			ptrdocumento puntatore al documento che contiene i comandi da impaginare
	@result  			ritorna l'eventuale errore dell'xt
*/
errorixtension XTAPI ControllaArgomenti(uchar* ptrdocumento) throw();

/*!
	@function			LeggiTipoImpaginazione
	@abstract 		gestione lettura file
	@discussion		Per leggere il comando presente all'inizio di ogni documento da impaginare
					che permette di sapere se bisogna eseguire una impaginazione di Bozza,
					un Calcolo dell'Ingombro oppure una Impaginazione Definitiva. Avanza il puntatore
					sul documento da impaginare fino al primo carattere dopo il comando sul tipo
					di impaginazione.

					11 Aprile 2005 - Fabrizio.

	@param 			ptrdocumento puntatore al documento che contiene i comandi da impaginare
	@param			mpaginazione per ritornare il tipo di impaginazione da eseguire
	@result  			ritorna l'eventuale errore dell'xtension se il documento non contiene
					alcun comando di impaginazione
*/
errorixtension XTAPI LeggiTipoImpaginazione(uchar **ptrdocumento, tipoimpaginazione *ptrtipoimpaginazione) throw();

/*!
	@function			LeggiQuattroD
	@abstract 		gestione lettura file
	@discussion		Legge una struttura da impaginare. Fa avanzare il punatore al documento da
					impaginare fino al primo carattere del successiva struttura.
					Per oggetti si intendono quei comandi di impaginazione che possono essere
					interpretati singolarmente. Eccone l'elenco:
					<+nomestile>
					<-nomestile>
					<-@nome dell'immagine;>
					<d@codice univoco defunto;posizione di rigore>
					<n@codice univoco annuncio;gruppo, posizione gruppo>
					<a@codice univo anniversario;gruppo, posizione gruppo>
					<r@codice univoco ringraziamento;gruppo, posizione gruppo>
					blocco di testo da importare con eventuali ritorni a capo

					11 Aprile 2005 - Fabrizio.

	@param 			ptrdocumento puntatore al documento che contiene i comandi da impaginare
	@param			ptrquattrod serve per ritornare il comando di impaginazione corrente
	@result  			nessuno
*/
void XTAPI LeggiQuattroD(uchar** ptrdocumento, quattrod *ptrquattrod) throw();

/*!
	@function			InizializzaQuattroD
	@abstract 		gestione lettura file
	@discussion		Inizializza i campi della struttura 4D

					11 Aprile 2005 - Fabrizio.

	@param 			ptrquattrod  puntatore alla struttura da inizializzare
	@result  			ritorna l'eventuale errore dell'xt
*/
void XTAPI InizializzaQuattroD(quattrod *ptrquattrod) throw();

/*!
	@function			LeggiContenutoFSSpecFile
	@abstract 		gestione lettura file
	@discussion		Apre il file specificato, lo legge inserendo il suo contenuto in un handle.
					Handle creato da questa procedura non deve essere deallocato

					11 Aprile 2005 - Fabrizio.

	@param 			name specifica il nome del file da leggere
	@param			ptrhandle parametro di uscita che contiene il contenuto del file
	@result  			ritorna l'eventuale errore dell'xt
*/
errorixtension XTAPI LeggiContenutoFSSpecFile(uchar *name, Handle *ptrhandle) throw();

/*!
	@function			LeggiNormContenutoFSSpecFile
	@abstract 		gestione lettura file
	@discussion		Apre il file specificato, lo legge inserendo il suo contenuto in un handle.
					Handle creato da questa procedura non deve essere deallocato

					11 Aprile 2005 - Fabrizio.

	@param 			name specifica il nome del file da leggere
	@param			ptrhandle parametro di uscita che contiene il contenuto del file
	@result  			ritorna l'eventuale errore dell'xt
*/
errorixtension XTAPI LeggiNormContenutoFSSpecFile(uchar *name, Handle *ptrhandle) throw();

/*!
	@function			LeggiContenutoFile
	@abstract 		gestione lettura file
	@discussion		Apre il file specificato, lo legge inserendo il suo contenuto in un handle.
					Handle creato da questa procedura non deve essere deallocato

					11 Aprile 2005 - Fabrizio.

	@param 			identificatorefile identificatore del file da leggere
	@param			ptrhandle parametro di uscita che contiene il contenuto del file
	@result  			ritorna l'eventuale errore dell'xt
*/
errorixtension XTAPI LeggiContenutoFile(int16 identificatorefile, Handle *ptrhandle) throw();

/*!
	@function			CancellaFile
	@abstract 		gestione lettura file
	@discussion		Cancella il file passato in input

					11 Aprile 2005 - Fabrizio.

	@param 			nome  nome del file da cancellare
	@result  			ritorna l'eventuale errore dell'xt
*/
errorixtension XTAPI CancellaFile(uchar *name) throw();

#endif // LeggiFile_h