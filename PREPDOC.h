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

// DEFINES
// DEFINES
// DEFINES

/*!
	@defined			PREPARADOCDIALOGID
	@abstract		resource id.
	@discussion		risorsa della dialog di preferenze di prepara documento.
*/
#define PREPARADOCDIALOGID				(21010 + LANGUAGE)

#define PAGINEPREPDOCPOPID				4
#define COLONNEPREPDOCPOPID			5
#define MODULIPREPDOCPOPID				6
#define CANCELLAMASTROCHECKID			7
#define PREPARABTNID						8

#define PAGINEMASTROPOPID				9
#define MASTROPOPID						10
#define COMPENSAZIONECHECKID			11
#define ASSOCIABTNID					12

#define COLONNEINGOMBRIPOPID			13
#define MODULIINGOMBRIPOPID				14
#define APPLICABTNID						15

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			PreparaDocWap
	@abstract		finestra di prepara documento.
	@discussion		Finestra per la gestione delle opzioni di prepara documento.

					25 Marzo 2005 - Fabrizio.

	@param			cbparam puntatore a xdwapparamrec record (see XDK Documentation).
	@result			void.
*/
int32 XTAPI PreparaDocWap(xdwapparamptr params) throw();

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

#pragma mark -
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