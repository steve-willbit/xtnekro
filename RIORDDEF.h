/*!
	@header			RiordinoDefunti.h
	@abstract		riordinamento annunci funebri
	@discussion 		Contiene le descrizioni delle procedure per il riordino dei defunti
	
					28 Aprile 2005		Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $					
*/
#ifndef RiordDef_h
#define RiordDef_h

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			NormaleDefuntiFondo
	@abstract 		gestione riordinamento necrologi
	@discussion		Cerca di inserire i defunti della lista nelle colonne ancora da chiudere

					2 Maggio 2005 - Fabrizio.

	@param			ptrdefunto - puntatore alla lista dei defunti
	@param			pagina - pagina di partenza
	@param			colonna - colonna di partenza
	@param			ptrscarto - per ritornare lo scarto corrente
	@return			ritorna l'eventuale errore dell'xtension	
*/
errorixtension XTAPI CalcolaMigliorOrdineDefunti(defunto *ptrdefunto, int16 pagina, int16 colonna, Fixed *ptrscarto) throw();

/*!
	@function			NormaleDefuntiFondo
	@abstract 		gestione riordinamento necrologi
	@discussion		Cerca di inserire i defunti della lista nelle colonne ancora da chiudere
					partendo dalla fine del documento

					2 Maggio 2005 - Fabrizio.

	@param			ptrdefunto - puntatore alla lista dei defunti
	@param			pagina - pagina di partenza
	@param			colonna - colonna di partenza
	@param			ptrscarto - per ritornare lo scarto corrente
	@return			ritorna l'eventuale errore dell'xtension	
*/
errorixtension XTAPI CalcolaMigliorOrdineDefuntiFondo(defunto *ptrdefunto, int16 pagina,  int16 colonna, Fixed *ptrscarto) throw();
#endif // RiordDef
