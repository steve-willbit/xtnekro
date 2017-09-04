/*!
	@header			SpazioColonna.h
	@abstract		gestione memoria
	@discussion 		Contiene le procedure usate per calcolare i margini su un documento con
					box gia' posizionati
	
					2 Maggio 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $			
*/

// TYPEDEFS
// TYPEDEFS
// TYPEDEFS

typedef enum 
{
	kLibera,
	kOccupata
} situazionecolonna;

typedef enum 
{
	kAlto,
	kBasso,
	kUguale
} vicinoaquale;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			CalcolaSpazioDisponibile
	@abstract 		gestione spazio avanzato
	@discussion		Calcola lo spazio disponibile su tutte le pagine del documento corrente

					2 Maggio 2005 - Fabrizio.

	@param 			nessuno
	@result  			Ritorna eventuale errore Xt
*/
errorixtension XTAPI CalcolaSpazioDisponibile() throw();
