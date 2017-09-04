/*!
	@header			Ripristina.h
	@abstract		preferenze dell'XT
	@discussion 		Contiene le descrizioni delle procedure per dare la possibilita' di riniziare
					l'impaginazione senza dover riimportare i blocchetti di testo
	
					18 Aprile 2005		Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.1  2005/04/20 07:14:21  taretto
					importazione files
					
*/

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			PrendiCodiceImpaginazione
	@abstract		operazioni ripristino documento.
	@discussion		Ritorna il codice di impaginazione corrente

					25 Marzo 2005 - Fabrizio.

	@param			nessuno.
	@result			ritorna il codice di impaginazione.
*/
uint32 PrendiCodiceImpaginazione() throw(); 

/*! 
	@function			InizializzaCodiceImpaginazione
	@abstract		operazioni ripristino documento.
	@discussion		Imposta il codice di impaginazione corrente

					25 Marzo 2005 - Fabrizio.

	@param			nessuno.
	@result			nessuno
*/
void XTAPI InizializzaCodiceImpaginazione() throw();

/*! 
	@function			ImpostaCodiceImpaginazione
	@abstract		operazioni ripristino documento.
	@discussion		Imposta il codice di impaginazione passato

					25 Marzo 2005 - Fabrizio.

	@param			codice - codice da impostare.
	@result			nessuno
*/
void XTAPI ImpostaCodiceImpaginazione(uint32 codice) throw();

/*! 
	@function			RipristinaDocumento
	@abstract		operazioni ripristino documento.
	@discussion		Permettere di ritornare al documento da cui ripartire per eseguire il
					calcolo del posizionamento

					3 Maggio 2005 - Fabrizio.

	@param			nessuno
	@result			ritorna l'eventuale errore dell'xtension
*/
errorixtension XTAPI RipristinaDocumento() throw();

/*! 
	@function			RipristinaDocumento
	@abstract		operazioni ripristino documento.
	@discussion		Permettere di rileggere i dati dell'ultima impaginazione per eseguire il
					calcolo del posizionamento

					3 Maggio 2005 - Fabrizio.

	@param			nessuno
	@result			ritorna l'eventuale errore dell'xtension
*/
errorixtension XTAPI  RipristinaDati() throw();

