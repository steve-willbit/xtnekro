/*!
	@header			Giustificazione.h
	@abstract		giustificazione documento impaginato
	@discussion 		Procedure per la giustificazione verticale del box di testo
	
					25 Marzo 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.1  2005/04/20 07:14:20  taretto
					importazione files
					
*/

#ifndef InfoNecrologi_h
#include "InfoNec.h"
#endif // InfoNecrologi_h

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			PrendiNumeroLinee
	@abstract		gestione giustificazione
	@discussion		Conta le linee di testo contenute in un box di testo senza overflow. Nel conteggio
					vengona saltate le linee con autoleading.

					18 Aprile 2005 - Fabrizio.

	@param			nessuno.
	@result			ritorna il numero di linee presenti nel box corrente.
*/
int32 XTAPI PrendiNumeroLinee() throw();

/*! 
	@function			PrendiSpazioTraParagrafi
	@abstract		gestione giustificazione
	@discussion		Sul box di testo corrente analizza i paragrafi presenti e ritorna lo spazio
					tra i paragrafi con leading diverso da automatioco

					18 Aprile 2005 - Fabrizio.

	@param			nessuno.
	@result			ritorna lo spazio tra i paragrafi in Fixed
*/
Fixed XTAPI PrendiSpazioTraParagrafi() throw();

/*! 
	@function			PrendiNumeroParagrafiDiversi
	@abstract		gestione giustificazione
	@discussion		Prende il numero di paragrafi del box corrente con spazio diverso da zero
					e con interlinea non automatica

					18 Aprile 2005 - Fabrizio.

	@param			nessuno.
	@result			ritorna il numero di paragrafi
*/
int32 XTAPI PrendiNumeroParagrafiDiversi() throw();

/*! 
	@function			PrendiNumeroParagrafiDiversi
	@abstract		gestione giustificazione
	@discussion		Prende il numero di paragrafi del box corrente con interlinea non
					automatica

					18 Aprile 2005 - Fabrizio.

	@param			nessuno.
	@result			ritorna il numero di paragrafi
*/
int32 XTAPI PrendiNumeroParagrafi() throw();

/*! 
	@function			PrendiNumeroParagrafiDiversi
	@abstract		gestione giustificazione
	@discussion		Richiama tutte le procedure per giustificare il testo lavorando
					sull'interlinea e lo spazio tra i paragrafi

					3 Maggio 2005 - Fabrizio.

	@param			ptrnecrologio - contiene le info per giustificare del necrologio da giustificare
	@param			spaziodarecuperare - contiene lo spazio da recuperare. Se negativo bisogna 
					ridurre l'interlinea, tra i paragrafi se e' positivo bisogna aumentare
					l'interlinea. Questo valore deve essere applicato a tutte le linee senza
					autoleading
	@result			ritorna il numero di paragrafi
*/
void XTAPI GiustificaTesto(necrologio *ptrnecrologio, Fixed spaziodarecuperare) throw();
