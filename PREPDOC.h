/*!
	@header			PreparazioneDocumento.h
	@abstract		preparazione documento
	@discussion 		Contiene le descrizione delle procedure per la gestione della preparazione
					documento.
	
					25 Marzo 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.1  2005/04/20 07:14:21  taretto
					importazione files
					
*/

#ifndef PrepDoc_h
#define PrepDoc_h

#ifndef dbpresource_h
#include "DbpResource.h"
#endif // dbpinclude_h

#ifndef Errori_h
#include "Errori.h"
#endif // Errori_h

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function		InserisciColonneDiIngombro
	@abstract		preparazione documento
	@discussion		Inserisco tutti i box di ingombro

					26 Aprile 2005 - Fabrizio Taretto.

	@param			nessuno.
	@result			nessuno.
*/
void XTAPI InserisciColonneDiIngombro() throw();

/*! 
	@function		CancelloBoxIngombro
	@abstract		preparazione documento
	@discussion		Cancello tutti i box di ingombro

					21 Aprile 2005 - Fabrizio Taretto.

	@param			nessuno.
	@result			nessuno.
*/
void XTAPI CancelloBoxIngombro() throw();

/*! 
	@function			InserisciPagineColonneEModuli
	@abstract		preparazione documento
	@discussion		Inserisce le pagine al documento corrente. Il documento finale sara' sempre
					formato dai parametri passati. Se il documento ha gia' piu' di una pagina le pagine
					in piu' vengono cancellate

					27 Aprile 2005 - Fabrizio Taretto.

	@param			pagine - pagine da inserire
	@param			colonne - colonne da inserire
	@param			moduli - moduli da inserire
	@param			cancella - per sapere se cancellare l'associazione alle pagine mastro.
	@result			nessuno.
*/
void XTAPI InserisciPagineColonneEModuli(int32 pagine, int32 colonne, int32 moduli, Boolean cancella) throw();

/*! 
	@function			ModificoIngombroPrimaPagina
	@abstract		preparazione documento
	@discussion		Togli dall'ingombro della prima pagina i moduli passati.

					27 Aprile 2005 - Fabrizio Taretto.

	@param			moduli - moduli da togliere
	@result			nessuno.
*/
void XTAPI ModificoIngombroPrimaPagina(int16 moduli) throw();

/*! 
	@function			AssociaPaginaMastro
	@abstract		preparazione documento
	@discussion		Associa una pagina mastro ad una pagina del documento

					27 Aprile 2005 - Fabrizio Taretto.

	@param			pagina - pagina a cui applicare la mastro
	@param			mastro - mastro da applicare
	@param			compensa - TRUE se si deve compensare sulla prima pagina
	@result			nessuno.
*/
void XTAPI AssociaPaginaMastro(int16 pagina, int16 mastro, Boolean compensa) throw();

/*! 
	@function			ModificaPrimaPagina
	@abstract		preparazione documento
	@discussion		Modifica la prima pagina inserendo gli ingombri necessari

					27 Aprile 2005 - Fabrizio Taretto.

	@param			colonna - colonna in cui inserire l'ingombro
	@param			moduli - numero dei moduli da inserire
	@result			nessuno.
*/
void XTAPI ModificaPrimaPagina(int16 colonna, int16 moduli) throw();

/*! 
	@function			CercaBoxIngombro
	@abstract		preparazione documento
	@discussion		Cerca sul documento corrente i box di ingombro.

					21 Aprile 2005 - Fabrizio Taretto.

	@param			nessuno.
	@result			nessuno.
*/
void XTAPI CercaBoxIngombro() throw();

/*! 
	@function			PreparaDocumento
	@abstract		finestra di prepara documento.
	@discussion		Apre la finestra per preparare il documento

					21 Aprile 2005 - Fabrizio.

	@param			nessuno.
	@result			nessuno.
*/
void XTAPI PreparaDocumento() throw();

#endif // PrepDoc_h

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

/* Procedure */
/* Procedure */
/* Procedure */

void AlloccaPopUpPreparaDocumento(void);
void PreparaDocumento(void);
#endif // 0 NECRO v1

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/