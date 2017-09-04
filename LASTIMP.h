/*!
	@header			UltimaImpaginazione.h
	@abstract		preferenze dell'XT
	@discussion 		Contiene la descrizione delle procedure per il il salvattaggio e la successiva
					lettura dei dati per gestire il ripristino dell'ultima impaginazione
	
					25 Marzo 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.1  2005/04/20 07:14:20  taretto
					importazione files
					
*/

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			ScriviInfoNecrologi
	@abstract		gestione ultima impaginazione
	@discussion		Scrive sul file dell'ultima impaginazione le informazione sui necrologi
					impaginati ma non ancora posizionati

					18 Aprile 2005 - Fabrizio.

	@param			nesuuno.
	@result			eventuale errore dell'Xt.
*/
errorixtension XTAPI ScriviInfoNecrologi() throw();

/*! 
	@function			LeggiInfoNecrologi
	@abstract		gestione ultima impaginazione
	@discussion		Legge dal file dell'ultima impaginazione le informazione sui necrologi
					impaginati ma non ancora posizionati

					4 Maggio 2005 - Fabrizio.

	@param			nesuuno.
	@result			eventuale errore dell'Xt.
*/
errorixtension XTAPI LeggiInfoNecrologi() throw();

/*! 
	@function			ScriviSpazioColonna
	@abstract		gestione ultima impaginazione
	@discussion		Scrive sul file dell'ultima impaginazione le informazione sui dati delle colonne

					4 Maggio 2005 - Fabrizio.

	@param			nesuuno.
	@result			eventuale errore dell'Xt.
*/
errorixtension XTAPI ScriviSpazioColonna() throw();
