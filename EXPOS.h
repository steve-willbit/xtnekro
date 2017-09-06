/*!
	@header			EsecuzionePosizionamento.h
	@abstract		preferenze dell'XT
	@discussion 		Contiene le descrizioni delle procedure per eseguire posizionamento dei necrologi
	
					3 Maggio 2005	  Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
*/

#ifndef  ExPos_h
#define ExPos_h

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			Posiziona
	@abstract		esegue posizionamento annunci
	@discussion		Posiziona i necrologi sul documento corrente in base al calcolo corrente

					3 Maggio 2005 - Fabrizio.

	@param			nessuno
	@result			ritorna l'eventuale errore dell'xtension
*/
errorixtension XTAPI Posiziona() throw();

#endif // ExPos_h
