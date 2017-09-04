/*!
	@header			RiordinoNecrologi.h
	@abstract		priordinamento necrologi
	@discussion 		Contiene le descrizioni delle procedure per il riordino dei necrologi
	
					25 Marzo 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
					
*/
#ifndef RiordNec_h
#define RiordNec_h


// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			CalcolaMigliorOrdineNecrologi
	@abstract		gestione riordinamento dei necrologi
	@discussion		Per cercare di riodinare i necrologi del defunto in base allo spazio
					disponibile sulla colonna

					2 Maggio 2005 - Fabrizio.

	@param			ptrdefunto - puntatore al defunto con i necrologi da riordinare
	@param			pagina - pagina corrente
	@param			colonna - colonna corrente
	@param			ptrscarto - puntatore scarto del precedente inserimento
	@param			ptrnecrologi - puntatore per ritornare la lista dei necrologi riordinati
	@result			nessuno
*/
void XTAPI CalcolaMigliorOrdineNecrologi(defunto *ptrdefunto, int16 pagina, int16 colonna, Fixed *ptrscarto, necrologio **ptrnecrologi) throw();

#endif // RiordNec_h