/*!
	@header			Memoria.h
	@abstract		gestione memoria
	@discussion 		Contiene le definizioni per la gestione degli handle.
	
					8 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
*/
#ifndef Memoria_h
#define Memoria_h

#ifndef Errori_h
#include "Errori.h"
#endif // Errori_h

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			CreaHandle
	@abstract 		gestione memoria.
	@discussion		allocca l'handle in base alla dimensione passata come parametro.

					8 Aprile 2005 - Fabrizio.

	@param 			ptrhandle puntatore all'handle da alloccare. Se la creazione non e' andata
					a buon fine ritorna NULL nell'handle.
	@param			dimensione  dimensione in byte da alloccare
	@result  			Ritorna l'eventuale errore dell'xtension 
*/
errorixtension XTAPI CreaHandle(Handle *ptrhandle, int32 dimensione) throw();

/*!
	@function			LiberaTrueHandle
	@abstract 		gestione memoria.
	@discussion		Deallocca l'handle.

					8 Aprile 2005 - Fabrizio.

	@param 			ptrhandle handle da dealloccare
	@result  			nessuno
*/
void XTAPI LiberaTrueHandle(Handle myhandle) throw();

/*!
	@function			LiberaHandle
	@abstract 		gestione memoria.
	@discussion		Deallocca l'handle.

					8 Aprile 2005 - Fabrizio.

	@param 			ptrhandle handle da dealloccare
	@result  			nessuno
*/
void XTAPI LiberaHandle(Handle *ptrhandle) throw();

#endif // Memoria_h