/*!
	@header			Tempo.h
	@abstract		rapporto
	@discussion 		Contiene la descrizione delle procedure per prelevare data e ora di esecuzione di
					una operazione
	
					6 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
*/
#ifndef Tempo_h
#define Tempo_h

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			StampaSuRapporto
	@abstract 		gestione file data
	@discussion		Prende la data corrente

					6 Aprile 2005		Fabrizio.

	@param 			nessuno
	@result  			una stringa che contiene la data corrente
*/
uchar* XTAPI PrendiData() throw();

/*!
	@function			PrendiDataXNomeFile
	@abstract 		gestione data.
	@discussion		Prende la data corrente

					6 Aprile 2005		Fabrizio.

	@param 			nessuno
	@result  			una stringa Pascal che contiene la data corrente
*/
uchar* XTAPI PrendiDataXNomeFile() throw();

#endif // Tempo_h 