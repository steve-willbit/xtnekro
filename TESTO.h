/*!
	@header			Testo
	@abstract		gestione testo
	@discussion 		Contiene la descrizione delle procedure usate per eseguire l'importazione
					del testo nel box corrente
	
					15 Aprile 2005	 Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.
					
					$Log: not supported by cvs2svn $

*/

#ifndef Testo_h
#define Testo_h

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			InserisciTesto
	@abstract 		gestione testo.
	@discussion		Inserisce, alla fine del flusso di testo e all'interno del box corrente,
					la stringa di caratteri passata 

					15 Aprile 2005 - Fabrizio.

	@param 			testo puntatore al tetso da inserire
	@param			lunghezza numero di caratteri da inserire
	@result  			nessuno
*/
void XTAPI InserisciTesto(uchar* testo, int32 lunghezza) throw();

#endif // Testo_h