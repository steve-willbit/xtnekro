/*!
	@header			Documenti.h
	@abstract		gestione document XPress
	@discussion 		Contiene la descrizione delle procedure usate per aprire, salvare e chiudere
					i documenti Xpress
	
					13 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.1  2005/04/20 07:14:18  taretto
					importazione files
					
*/

#ifndef  Documenti_h
#define Documenti_h

#ifndef  Errori_h
#include "Errori.h"
#endif //  Errori_h

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			ApriDocumento
	@abstract		gestione documento XPress
	@discussion		Apre il documento passato come parametro

					13 Aprile 2005 - Fabrizio.

	@param			nomedocumento contiene il nome completo del documento Xpress da aprire
	@result			ritorna l'eventuale errore dell'Xtensione
*/
errorixtension XTAPI ApriDocumento(uchar *nomedocumento) throw();

/*! 
	@function			ChiudiDocumento
	@abstract		gestione documento XPress
	@discussion		Permette di chiudere il documento corrente

					13 Aprile 2005 - Fabrizio.

	@param			nessuno
	@result			nessuno.
*/
void XTAPI ChiudiDocumento() throw();

/*! 
	@function			SalvaDocumento
	@abstract		gestione documento XPress
	@discussion		Salva il documento con il nome passato come parametro nella cartella passata
					come parametro

					13 Aprile 2005 - Fabrizio.

	@param			nomecartella contiene il nome della cartella dove salvare il documento
	@param			nomedocumento contiene il nome da usare per salvare il documento corrente
	@result			ritorna l'eventuale errore dell'Xtensione
*/
errorixtension XTAPI SalvaDocumento(uchar* nomecartella, uchar* nomedocumento) throw();

/*! 
	@function			SalvaDocumento
	@abstract		gestione documento XPress
	@discussion		Permette di chiudere tutti i documenti aperti correntemente

					13 Aprile 2005 - Fabrizio.

	@param			devosalvare TRUE se devo salvare i documenti
	@result			ritorna l'eventuale errore dell'Xtensione
*/
errorixtension XTAPI ChiudiTuttiIDocumenti(bool8 devosalvare) throw();

/*! 
	@function			StampaDocumento
	@abstract		gestione documento XPress
	@discussion		Stampa il documento corrente

					13 Aprile 2005 - Fabrizio.

	@param			nessuno
	@result			nessuno
*/
void XTAPI StampaDocumento() throw();

#endif // Documenti_h