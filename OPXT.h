/*!
	@header			OperazioniXtension.h
	@abstract		operazione eseguite dall'XT
	@discussion 		Contiene le procedure usate per eseguire le operazioni dell'Xtension.
	
					4 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.1  2005/04/20 07:14:20  taretto
					importazione files
					
*/
#ifndef OperazioniXtension_h
#define OperazioniXtension_h

// INCLUDES
// INCLUDES
// INCLUDES

#ifndef Errori_h
#include "Errori.h"
#endif // Errori_h

// ENUMS
// ENUMS
// ENUMS

typedef enum {
	kCoseDiNotifica,
	kCoseDiQuattroD,
	kCoseDiAppWare,
	kCoseDellUtente,
	kCoseDiStampa,
	kCoseDiPreparaDocumento,
	kCoseDiCalcolaPosizionamento,
	kCoseDiPosiziona,
	kCoseDiPaletteInfo,
	kCoseDiPreferenze,
	kIdle
} tipooperazione;

// EXTERN GLOBALS
// EXTERN GLOBALS
// EXTERN GLOBALS

extern tipooperazione gOperazioneXtension;

// globale d'utilita'
extern Handle gHndlFile;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function		PrendiOperazioneCorrente
	@abstract 		operazione
	@discussion		Ritorna l'operazione di Xpress attuale. Permette di sapere se Xpress
					deve aspettarsi i file di impaginazione da 4D o da AppWare, oppure
					e' una situazione in cui non deve eseguire alcuna operazione di impaginazione
					
					5 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result  			rritorna l'operazione corrente
*/
tipooperazione XTAPI PrendiOperazioneCorrente() throw();

/*!
	@function			CoseDiQuattroD (tengo nome per compatibilita')
	@abstract 		attesa file
	@discussion		Dobbiamo aspettare i file di 4D (oggi PresNet) da impaginare
					
					5 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result  			ritorna l'eventuale errore dell'xtension
*/
errorixtension XTAPI  CoseDiQuattroD() throw();

/*!
	@function			ImpostaCoseDellUtente 
	@abstract 		attesa file
	@discussion		Imposta come operazione dell'Xtension le cose dell'Utente
					
					14 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result  			nessuno
*/
void XTAPI ImpostaCoseDellUtente() throw();

/*!
	@function			ImpostaCoseDiQuattroD 
	@abstract 		attesa file
	@discussion		Imposta come operazione dell'Xtension le cose di 4D
					
					14 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result  			nessuno
*/
void XTAPI ImpostaCoseDiQuattroD() throw();

/*!
	@function			CoseDiStampa 
	@abstract 		attesa file
	@discussion		Aspetto che Xpress finisca la stampa per poi chiudere il documento corrente
					e ritornare all'impaginazione dei file da 4D  
					
					14 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result  			nessuno
*/
void XTAPI CoseDiStampa() throw();

/*!
	@function			CambiaOperazioneCorrente 
	@abstract 		operazioni XT
	@discussion		Permette all'utente di disabilitare l'impaginazione automatica quando
					viene memorizzata nella cartella opportuna il file da impaginare
					
					19 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result  			nessuno
*/
void XTAPI CambiaOperazioneCorrente() throw();

/*!
	@function			ImpostaCoseDiStampa 
	@abstract 		operazioni XT
	@discussion		Imposta come operazione dell'Xtension le cose di Stampa
					
					6 Maggio 2005 - Taretto Fabrizio.

	@param			nessuno
	@result  			nessuno
*/
void XTAPI ImpostaCoseDiStampa() throw();

void XTAPI ImposteCoseDiPreparaDocumento() throw();
void XTAPI ImposteCoseDiCalcolaPosizionameto() throw();
void XTAPI ImposteCoseDiPosiziona() throw();
void XTAPI ImposteCoseDiPaletteInfo() throw();
void XTAPI ImposteCoseDiPreferenze() throw();

#endif /* OperazioniXtension_h */

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

/* Procedure */
/* Procedure */
/* Procedure */

errorixtension CoseDiAppWare(void);
errorixtension CoseDiNotifica(void);
errorixtension EsecuzioneAppWare(void);
void ImpostaCoseDiAppWare(void);
void ImpostaCoseDiNotifica(void);
#endif // NECRO v1
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
