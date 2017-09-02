/*!
	@header			Box.h
	@abstract		gestione box
	@discussion 		Contiene le descrizioni delle procedure usate per eseguire la creazione
					dei box di testo
	
					15 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.1  2005/04/20 07:14:18  taretto
					importazione files
					
*/
#ifndef Box_h
#define Box_h

#ifndef AppWare_h
#include "AppWare.h"
#endif

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			AumentaAltezzaBox
	@abstract 		gestione box
	@discussion		Aumenta l'altezza del box corrente fino a quando il tresto non e'
					completamente visibile 
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
void XTAPI AumentaAltezzaBox() throw();

/*!
	@function			CreaBoxCodiceDefunto
	@abstract 		gestione box
	@discussion		Crea un box di testo in alto a destra per inserire il codice del defunto.
					Serve solo nella fase di impaginazione di BOZZA.
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
void XTAPI CreaBoxCodiceDefunto() throw();

/*!
	@function			VerificaAltezzaBoxCodiceDefunto
	@abstract 		gestione box
	@discussion		Aggiorna l'altezza del box del codice defunto in modo che il testo sia tutto
					visibile. Nel caso in cui il testo non ci sta lasca il box con la coordinata
					alta uguale a zero
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
void XTAPI VerificaAltezzaBoxCodiceDefunto() throw();

/*!
	@function			CreaBoxTesto
	@abstract 		gestione box
	@discussion		Crea un box di testo sulle coordinate correnti e lo imposta come box corrente
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
void XTAPI CreaBoxTesto() throw();

/*!
	@function			CreaBoxImmagine
	@abstract 		gestione box
	@discussion		Crea un box immagine sulle coordinate correnti e lo imposta come box corrente
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
void XTAPI CreaBoxImmagine() throw();

/*!
	@function			CreaBoxImmagineAncorato
	@abstract 		gestione box
	@discussion		Crea un box di immagine ancorato alla fine del flusso di testo corrente
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
void XTAPI CreaBoxImmagineAncorato() throw();

/*!
	@function			CreaBoxTestoAncorato
	@abstract 		gestione box
	@discussion		Crea un box di testo ancorato alla fine del flusso di testo corrente
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
void XTAPI CreaBoxTestoAncorato() throw();

/*!
	@function			VerificaAltezzaBoxTestoBozza
	@abstract 		gestione box
	@discussion		Aggiorna l'altezza del box corrente di testo dopo che e' stata effettuata
					l'importazione del testo e dell'eventuale immagine al suo interno. Viene
					usata nell'impaginazione di BOZZA perch‚ quando il box non ci sta
					genera un box sulla colonna adiacente con un collegamento di testo
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
void XTAPI VerificaAltezzaBoxTestoBozza() throw();

/*!
	@function			VerificaAltezzaBoxTestoIngombro
	@abstract 		gestione box
	@discussion		Aggiorna l'altezza del box corrente di testo dopo che e' stata effettuata
					l'importazione del testo e dell'eventuale immagine al suo interno. Questa
					procedura viene usata per il CALCOLO INGOMBRO e non si preoccupa di fare
					l'aggiornamento delle coordinate
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
void XTAPI VerificaAltezzaBoxTestoIngombro() throw();

/*!
	@function			VerificaAltezzaBoxTestoDefinitivoManuale
	@abstract 		gestione box
	@discussion		Aggiorna l'altezza del box corrente di testo dopo che e' stata effettuata
					l'importazione del testo e dell'eventuale immagine al suo interno. Questa
					procedura viene usata per la prima impaginazione DEFINITIVA MANUALE e non
					si preoccupa di fare l'aggiornamento delle coordinate
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
void XTAPI VerificaAltezzaBoxTestoDefinitivoManuale() throw();

/*!
	@function			VerificaAltezzaBoxTestoDefinitivoManuale
	@abstract 		gestione box
	@discussion		Aggiorna l'altezza del box corrente di testo dopo che e' stata effettuata
					l'importazione del testo e dell'eventuale immagine al suo interno. Viene
					usata nell'impaginazione di DEFINITIVO SEQUENZA perch‚ quando il box non ci sta
					genera un box sulla colonna adiacente con un collegamento di testo
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
void XTAPI VerificaAltezzaBoxTestoDefinitivoSequenza() throw();

/*!
	@function			VerificaAltezzaBoxTestoDefinitivoAutomatico
	@abstract 		gestione box
	@discussion		Aggiorna l'altezza del box corrente di testo dopo che e' stata effettuata
					l'importazione del testo e dell'eventuale immagine al suo interno. Questa
					procedura viene usata per la prima impaginazione DEFINITIVA AUTOMATICO e non
					si preoccupa di fare l'aggiornamento delle coordinate
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware per salvare le informazioni sul box corrente
	@result			nessuno
*/
void XTAPI VerificaAltezzaBoxTestoDefinitivoAutomatico(appware *ptrappware) throw();

/*!
	@function			CreaFiletto
	@abstract 		gestione box
	@discussion		Crea un filetto orizzontale con le caretteristiche caricate in fase della
					lettura delle preferenze sulle coordinate passate
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			coordinatasinistra coordinata sinistra
	@param			coordinataalta coordinata alta
	@param			pagina pagina corrente
	@result			nessuno
*/
void XTAPI CreaFiletto(Fixed coordinatasinistra, Fixed coordinataalta, int16 pagina) throw();

/*!
	@function			TogliRitornoACapo
	@abstract 		gestione box
	@discussion		Se l'utimo carattere del box corrente e' un return lo tolgo
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
void XTAPI TogliRitornoACapo() throw();

/*!
	@function			CancellaBox
	@abstract 		gestione box
	@discussion		Cancello tutti i box di ingombro presenti sulla prima pagina
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			colonna identificatore del box da cancellare
	@result			nessuno
*/
void XTAPI CancellaBox(int16 colonna) throw();

/*!
	@function			CreaColonnaIngombro
	@abstract 		gestione box
	@discussion		Crea un box di ingombro di colonna
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			coordinatasinistra coordina sinistra del box di ingombro
	@param			altezza altezza del box di ingombro
	@result			nessuno
*/
void XTAPI CreaColonnaIngombro(Fixed coordinatasinistra, Fixed altezza) throw();

/*!
	@function			InserisciColonnaDiIngombro
	@abstract 		gestione box
	@discussion		Inserisce una colonna di ingombro
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			colonna colonna in cui fare l'inserimento
	@param			moduli altezza in moduli del box di ingombro
	@result			boxid del box di ingombro
*/
boxid XTAPI InserisciColonnaDiIngombro(int16 colonna, int16 moduli) throw();

/*!
	@function			InserisciColonnaDiIngombro
	@abstract 		gestione box
	@discussion		Prende i moduli di ingombro della pagina passata
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			pagina pagina in cui leggere il numero di moduli di ingombro
	@param			ptrnumeromoduli per ritornare il numero moduli presenti
	@result			boxid del box di ingombro
*/
void XTAPI PrendiModuliPagina(int16 pagina, int16 *ptrnumeromoduli) throw();

/*!
	@function			PrendiModuliPaginaMastro
	@abstract 		gestione box
	@discussion		Prende i moduli di ingombro della pagina mastro passata
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			mastro mastro in cui leggere il numero di moduli di ingombro
	@param			ptrnumeromoduli  arrivano i moduli presenti sulla pagina a cui applicare
					questo mastro e ritorna la differenza con i moduli di ingombro della pagina
					mastro. Mastro va da 3 in poi.
	@result			nessuno
*/
void XTAPI PrendiModuliPaginaMastro(int16 mastro, int16 *ptrnumeromoduli) throw();

/*!
	@function			PrendiAltezzaBox
	@abstract 		gestione box
	@discussion		Prendi l'altezza del box correntemente selezionato
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			ritorna l'altezza del box
*/
Fixed XTAPI PrendiAltezzaBox() throw();

/*!
	@function			CopiaIncollaBoxPaxAncorato
	@abstract 		gestione box
	@discussion		Cerca il box campione del simbolo di Pax, lo copia e lo incolla al fondo
					del box di testo corrente.
					
					11 Maggio 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
errorixtension XTAPI CopiaIncollaBoxPaxAncorato() throw();


#endif // Box_h