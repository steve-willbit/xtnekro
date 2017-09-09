/* ------------------------------------------------------------------------ *
	LeggiFile.cpp

	11 Aprile 2005 - Fabrizio

	Copyright © Sinedita S.r.l 2005. All Rights Reserved.

	Descrizione:
	Contiene le procedure per la lettura e i controllo del file da impaginare.

	$Log: not supported by cvs2svn $
	Revision 1.1  2005/04/20 07:14:21  taretto
	importazione files
	

* ------------------------------------------------------------------------ */

/* Required Includes ********************************************************/
#include PROJECT_HEADERS
#if WINOS
#pragma hdrstop		// force Visual C++ precompiled header
#endif

#include "Include.h"

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "Errori.h"
#include "Memoria.h"
#include "OpXt.h"
#include "Paletta.h"
#include "XTNecro.h"

#include "ReadFile.h"

// CONSTS
// CONSTS
// CONSTS

#define kLunghezzaStringaConErrore 32

// GLOBALS
// GLOBALS
// GLOBALS

// globali d'utilità
Handle gHndl=NULL;
uchar *gPtrFile;

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	PosizioneEStringaConErrore
	
* ------------------------------------------------------------------------ */
void XTAPI PosizioneEStringaConErrore(int32 posizione, uchar* ptrdocumento) throw()
{
	ConcatenaLongAllaStringaErrore(posizione);
	STRCPY(gStringaC, "\".");
	STRNCAT((char*)gStringaC, (char*)ptrdocumento, kLunghezzaStringaConErrore);
	STRCAT(gStringaC, ".\"");
	ConcatenaAllaStringaErrore(gStringaC);
	Visualizza(gStringaC);
} // PosizioneEStringaConErrore

/* ------------------------------------------------------------------------ *

	ControllaComandi
	
* ------------------------------------------------------------------------ */
errorixtension ControllaComandi(uchar* ptrdocumento) throw()
{
	// imposto il contatore
	int32  lPosizioneErrore = 0;
	
	while (1) 
	{
		if (*ptrdocumento == kMaggiore) 
		{
			// errore carattere sbagliato			
			PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
			return(kErroreComandi);
		} 
		else if (*ptrdocumento == kMinore) 
		{
			while (1) 
			{
				ptrdocumento++;
				lPosizioneErrore++;
				if (*ptrdocumento == kMaggiore) break;
				else if (*ptrdocumento == kMinore) 
				{
					// errore carattere sbagliato
					PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
					return(kErroreComandi);
				}
				 else if (*ptrdocumento == kFineFile) 
				{
					PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
					return(kErroreComandi);
				}
			}
		} 
		else if (*ptrdocumento == kFineFile) 
			return(kNessunErrore);
		ptrdocumento++;
		lPosizioneErrore++;
	}	
} // ControllaComandi

/* ------------------------------------------------------------------------ *

	ControllaPosizioneComandi
	
* ------------------------------------------------------------------------ */
errorixtension XTAPI ControllaPosizioneComandi(uchar* ptrdocumento) throw()
{
	// imposto il contatore posizione errore
	int32 lPosizioneErrore = 0;
	
	// TRUE appena dopo che sia stato un comendo di defunto
	bool8 lComandoDefunto = FALSE;

	while (1) 
	{
		if (*ptrdocumento == kFineFile) 
			return(kNessunErrore);
		if (*ptrdocumento == kMinore) 
		{
			// controllo il tipo di comando
			ptrdocumento++;
			lPosizioneErrore++;
			switch (*ptrdocumento) 
			{
				case kDefunto:
					if (lComandoDefunto == TRUE) 
					{
						// comando di impaginazione nella posizione sbagliata
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErrorePosizioneComandi); /* */
					}
					lComandoDefunto = TRUE;
					break;
				case kAnnuncio:
				case kAnniversario:
				case kRingraziamento:
					lComandoDefunto = FALSE;
					break;
				case kPax:
					break;	
				default:
					if (lComandoDefunto == TRUE) 
					{
						// comando di impaginazione nella posizione sbagliata
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErrorePosizioneComandi); /* */
					}
					break;
			} // switch (*ptrdocumento)
			
			// vado fino alla fine del comando di impaginazione
			while (1) 
			{
				ptrdocumento++;
				lPosizioneErrore++;
				if (*ptrdocumento == kMaggiore) 
					break;
			}
			ptrdocumento++;
			lPosizioneErrore++;
			
			// salto l'eventuale ritorno a capo dopo il comando di impaginazione
			if (*ptrdocumento == kRitornoACapo)
			{
				ptrdocumento++;
				lPosizioneErrore++;
				if (*ptrdocumento == kNuovaLinea)
				{
					ptrdocumento++;
					lPosizioneErrore++;
				}
			}
		} 
		else 
		{
			/* testo */	
			if (lComandoDefunto == TRUE) 
			{
				// comando di impaginazione nella posizione sbagliata
				PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
				return(kErrorePosizioneComandi);
			}
		
			ptrdocumento++;
			lPosizioneErrore++;
		}
	}	
} // ControllaPosizioneComandi

/* ------------------------------------------------------------------------ *

	ControllaArgomenti

* ------------------------------------------------------------------------ */
errorixtension XTAPI ControllaArgomenti(uchar* ptrdocumento) throw()
{
	// imposto il contatore posizione errore
	int32 lPosizioneErrore = 0;
	
	// verifico il primo comando che indica il tipo di impaginazione
	if (*ptrdocumento != kMinore) 
	{
		// errore carattere sbagliato
		PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
		return(kErroreArgomenti);
	}

	ptrdocumento++;
	lPosizioneErrore++;
	if (*ptrdocumento == kPuntoEVirgola) 
	{
		// errore carattere sbagliato
		PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
		return(kErroreArgomenti);
	}
	if (*ptrdocumento == kMaggiore) 
	{
		// errore carattere sbagliato
		PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
		return(kErroreArgomenti);
	}

	// salto fino al ; il comando di impaginazione
	while (1) 
	{
		ptrdocumento++;
		lPosizioneErrore++;
		if (*ptrdocumento == kPuntoEVirgola) 
			break;
		if (*ptrdocumento == kMaggiore) 
		{
			// errore carattere sbagliato
			PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
			return(kErroreArgomenti);
		}
		if (*ptrdocumento == kMinore) 
		{
			// errore carattere sbagliato
			PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
			return(kErroreArgomenti);
		}
	}
	// salto fino al > ogni eventuale commento
	while (1) 
	{
		ptrdocumento++;
		lPosizioneErrore++;
		if (*ptrdocumento == kMaggiore) 
			break;
		if (*ptrdocumento == kPuntoEVirgola) 
		{
			// errore carattere sbagliato
			PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
			return(kErroreArgomenti);
		}
		
		if (*ptrdocumento == kMinore) 
		{
			PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
			return(kErroreArgomenti);
		}
	}
	ptrdocumento++;
	lPosizioneErrore++;

	while (1) 
	{
		if (*ptrdocumento == kMaggiore) 
		{
			// errore carattere sbagliato
			PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
			return(kErroreArgomenti);
		}
		if (*ptrdocumento == kFineFile) return(kNessunErrore);
		if (*ptrdocumento == kMinore) {
			// si tratta di un comando di impaginazione
			 ptrdocumento++;
			 lPosizioneErrore++;
			switch (*ptrdocumento) 
			{
				case kMeno:
					// devo vedere se il carattere successivo e' una chiocciola
					ptrdocumento++;
					lPosizioneErrore++;
					if (*ptrdocumento == kChiocciola) 
					{
						// devo verificare che il carattere successivo sia diverso da  ; > 
						ptrdocumento++;
						lPosizioneErrore++;
						if (*ptrdocumento == kPuntoEVirgola) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
						if (*ptrdocumento == kMaggiore) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
						
						// devo avanzare fino al ; per saltare l'immagine
						while (1) 
						{
							ptrdocumento++;
							lPosizioneErrore++;
							if (*ptrdocumento == kPuntoEVirgola) break;
							if (*ptrdocumento == kMaggiore) 
							{
								// errore carattere sbagliato
								PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
								return(kErroreArgomenti);
							}
						}

						// devo avanzare fino al > per saltare l'eventuale commento
						while (1) 
						{
							ptrdocumento++;
							lPosizioneErrore++;
							if (*ptrdocumento == kMaggiore) 
								break;
						}
						ptrdocumento++;
						lPosizioneErrore++;
					} 
					else 
					{
						// devo verificare che il carattere successivo sia diverso da ; > 
						if (*ptrdocumento == kPuntoEVirgola) 
						{
							// errore carattere sbagliato 
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
						if (*ptrdocumento == kMaggiore) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
				
						// devo avanzare fino al  > per saltare lo stile
						while (1) 
						{
							ptrdocumento++;
							lPosizioneErrore++;
							if (*ptrdocumento == kMaggiore) 
								break;
						}
						ptrdocumento++;
						lPosizioneErrore++;
					}
					break;
				
				case kPiu:
					// devo verificare che il carattere successivo sia diverso da ; >
					if (*ptrdocumento == kPuntoEVirgola) 
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
					if (*ptrdocumento == kMaggiore) 
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
					
					// devo avanzare fino al  > per saltare lo stile
					while (1) 
					{
						ptrdocumento++;
						lPosizioneErrore++;
						if (*ptrdocumento == kMaggiore) 
							break;
					}
					ptrdocumento++;
					lPosizioneErrore++;
					break;
				
				case kDefunto:
					// devo vedere se il carattere successivo e' una chiocciola
					ptrdocumento++;
					lPosizioneErrore++;
					if (*ptrdocumento != kChiocciola) 
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
					
					// devo verificare che il carattere successivo sia diverso da ; >
					ptrdocumento++;
					lPosizioneErrore++;
					if (*ptrdocumento == kPuntoEVirgola) 
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
					if (*ptrdocumento == kMaggiore) 
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
						
					// devo avanzare fino al ; per saltare il codice defunto
					while (1) 
					{
						ptrdocumento++;
						lPosizioneErrore++;
						if (*ptrdocumento == kPuntoEVirgola) 
							break;
						if (*ptrdocumento == kMaggiore) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
					}
					
					// devo avanzare fino al > per saltare l'eventuale posizione di rigore
					while (1) 
					{
						ptrdocumento++;
						lPosizioneErrore++;
						if (*ptrdocumento == kMaggiore) 
							break;
						
						if (*ptrdocumento == kPuntoEVirgola) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
						if (*ptrdocumento == kVirgola) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
					}
					ptrdocumento++;
					lPosizioneErrore++;
					break;
					
				case kAnnuncio:
					// devo vedere se il carattere successivo E' una chiocciola
					ptrdocumento++;
					lPosizioneErrore++;
					if (*ptrdocumento != kChiocciola) 
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
					
					// devo verificare che il carattere successivo sia diverso da ; >
					ptrdocumento++;
					lPosizioneErrore++;
					if (*ptrdocumento == kPuntoEVirgola) 
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
					if (*ptrdocumento == kMaggiore) 
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}

					// devo avanzare fino al ; per saltare il codice necrologio
					while (1) 
					{
						ptrdocumento++;
						lPosizioneErrore++;
						if (*ptrdocumento == kPuntoEVirgola) 
							break;
						if (*ptrdocumento == kMaggiore) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
					}
		
					// devo avanzare fino alla , per saltare il gruppo 
					while (1) 
					{
						ptrdocumento++;
						lPosizioneErrore++;
						if (*ptrdocumento == kVirgola) 
							break;
						if (*ptrdocumento == kMaggiore) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
					}
					
					// devo avanzare fino al > per saltare la posizione gruppo
					while (1) 
					{
						ptrdocumento++;
						lPosizioneErrore++;
						if (*ptrdocumento == kMaggiore) 
							break;
						if (*ptrdocumento == kPuntoEVirgola) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
						if (*ptrdocumento == kVirgola) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
					}
					
					ptrdocumento++;
					lPosizioneErrore++;
					break;
					
				case kAnniversario:
					// devo vedere se il carattere successivo E' una chiocciola
					ptrdocumento++;
					lPosizioneErrore++;
					if (*ptrdocumento != kChiocciola) 
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
					
					// devo verificare che il carattere successivo sia diverso da ; >
					ptrdocumento++;
					lPosizioneErrore++;
					if (*ptrdocumento == kPuntoEVirgola) 
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
					if (*ptrdocumento == kMaggiore) 
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
						
					// devo avanzare fino al ; per saltare il codice anniversario 
					while (1) 
					{
						ptrdocumento++;
						lPosizioneErrore++;
						if (*ptrdocumento == kPuntoEVirgola) 
							break;
						if (*ptrdocumento == kMaggiore) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
					}

					// devo avanzare fino alla , per saltare il gruppo
					while (1) 
					{
						ptrdocumento++;
						lPosizioneErrore++;
						if (*ptrdocumento == kVirgola) 
							break;
						if (*ptrdocumento == kMaggiore) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
					}
					
					// devo avanzare fino al > per saltare la posizione gruppo
					while (1) 
					{
						ptrdocumento++;
						lPosizioneErrore++;
						if (*ptrdocumento == kMaggiore) 
							break;
						if (*ptrdocumento == kPuntoEVirgola) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
						if (*ptrdocumento == kVirgola) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
					}
					
					ptrdocumento++;
					lPosizioneErrore++;
					break;
					
				case kRingraziamento:
					// devo vedere se il carattere successivo e' una chiocciola
					ptrdocumento++;
					lPosizioneErrore++;
					if (*ptrdocumento != kChiocciola) 
					{
						// errore carattere sbagliato 
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
					
					// devo verificare che il carattere successivo sia diverso da ; >
					ptrdocumento++;
					lPosizioneErrore++;
					if (*ptrdocumento == kPuntoEVirgola) 
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
					if (*ptrdocumento == kMaggiore) 
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}

					// devo avanzare fino al ; per saltare il codice ringraziamento
					while (1) 
					{
						ptrdocumento++;
						lPosizioneErrore++;
						if (*ptrdocumento == kPuntoEVirgola) 
							break;
						if (*ptrdocumento == kMaggiore) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
					}			

					// devo avanzare fino alla , per saltare il gruppo
					while (1) 
					{
						ptrdocumento++;
						lPosizioneErrore++;
						if (*ptrdocumento == kVirgola) 
							break;
						if (*ptrdocumento == kMaggiore) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
					}
					
					// devo avanzare fino al > per saltare la posizione gruppo
					while (1) 
					{
						ptrdocumento++;
						lPosizioneErrore++;
						if (*ptrdocumento == kMaggiore) 
							break;
						if (*ptrdocumento == kPuntoEVirgola) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
						if (*ptrdocumento == kVirgola) 
						{
							// errore carattere sbagliato
							PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
							return(kErroreArgomenti);
						}
					}
					
					ptrdocumento++;
					lPosizioneErrore++;
					
					break;
					
				case kPax:
					// devo vedere se il carattere successivo e' una chiocciola
					ptrdocumento++;
					lPosizioneErrore++;
					if (*ptrdocumento != kChiocciola) 
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
					
					// devo verificare se i tre caratteri successivi siano diversi da Pax, quindi 
					// devo verificare se il carattere successivo sia diverso da P 
					ptrdocumento++;
					lPosizioneErrore++;
					if (*ptrdocumento != 'P')
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
					
					// devo verificare se il carattere successivo sia diverso da a 
					ptrdocumento++;
					lPosizioneErrore++;
					if (*ptrdocumento != 'a')
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
					
					// devo verificare se il carattere successivo sia diverso da x
					ptrdocumento++;
					lPosizioneErrore++;
					if (*ptrdocumento != 'x')
					{
						// errore carattere sbagliato 
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}

					// devo verificare se il carattere successivo sia diverso da >
					ptrdocumento++;
					lPosizioneErrore++;
					if (*ptrdocumento != kMaggiore)
					{
						// errore carattere sbagliato
						PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
						return(kErroreArgomenti);
					}
					ptrdocumento++;
					lPosizioneErrore++;
					
					break;	
					
				default:
					// errore manca un indicatore di comando di impaginazione
					PosizioneEStringaConErrore(lPosizioneErrore, ptrdocumento);
					return(kErroreArgomenti);
					break;
			} // switch (*ptrdocumento)
		} 
		else 
		{
			// testo
			ptrdocumento++;
			lPosizioneErrore++;
		}
	}	
} // ControllaArgomento

/* ------------------------------------------------------------------------ *

	LeggiTipoImpaginazione

* ------------------------------------------------------------------------ */
errorixtension XTAPI LeggiTipoImpaginazione(uchar **ptrdocumento, tipoimpaginazione *ptrtipoimpaginazione) throw()
{
	// per leggere il tipo di impaginazione
	uchar lComando = kFineFile;
	
	if (**ptrdocumento != kMinore) 
	{
		// errore carattere sbagliato
		return(kErroreTipoImpaginazione);
	}
	(*ptrdocumento)++;
	lComando = **ptrdocumento; 
	(*ptrdocumento)++;
	if (**ptrdocumento != kPuntoEVirgola) 
	{
		// errore carattere sbagliato
		return(kErroreTipoImpaginazione);
	}
	// salto fino al > ogni eventuale commento
	while (1) 
	{
		if (**ptrdocumento == kMaggiore) 
			break;
		(*ptrdocumento)++;
	}
	(*ptrdocumento)++;
	if (**ptrdocumento == kRitornoACapo)
	{
		(*ptrdocumento)++;
		if (**ptrdocumento == kNuovaLinea)
		{
			(*ptrdocumento)++;
		}
	}
	
	(*ptrtipoimpaginazione) = (tipoimpaginazione) lComando;
	return(kNessunErrore);
} // LeggiTipoImpaginazione


/* ------------------------------------------------------------------------ *

	LeggiQuattroD

* ------------------------------------------------------------------------ */
void XTAPI LeggiQuattroD(uchar** ptrdocumento, quattrod *ptrquattrod) throw()
{
	// per determinare la lunghezza del testo da leggere 	
	Size lContatore = 0;
	// punta all'inizio del testo
	uchar* lPtrInizio = NULL;
	
	if (**ptrdocumento == kFineFile) 
	{
		(*ptrquattrod).tipoComando = kComandoDiFine;
		return;
	}
	if (**ptrdocumento == kMinore) 
	{
		// si tratta di un comando di impaginazione
		(*ptrdocumento)++;
		switch (**ptrdocumento) 
		{
			case kMeno:
				// devo vedere se il carattere successivo e' una chiocciola
				(*ptrdocumento)++;
				if (**ptrdocumento == kChiocciola) 
				{
					// devo catturare il nome dell'immagine fino al ;
					(*ptrdocumento)++;
					lPtrInizio = (*ptrdocumento);
					lContatore = 0;
					while (1) 
					{
						if (**ptrdocumento == kPuntoEVirgola) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
					// creo il puntatore alla stringa che deve contenere il testo da importare 
					(*ptrquattrod).testo = (uchar *) NewPtrClear(lContatore + 1);
					// copio il nome dell'immagine
					BlockMove((Ptr) lPtrInizio, (Ptr) (*ptrquattrod).testo, lContatore);
					// inserisco il carattere '\0' alla fine del testo
					(*ptrquattrod).testo[lContatore] = kFineFile;
					// salvo la lunghezza del campo testo
					(*ptrquattrod).lunghezzaTesto = lContatore;
					// dico che si tratta di una immagine
					(*ptrquattrod).tipoComando = kImmagine;
					(*ptrquattrod).tipoRigore = kNessunRigore;
					(*ptrquattrod).tipoGruppo = kNessunGruppo;
					(*ptrquattrod).posizioneNelGruppo = kNessunaPosizione;

					// serve per saltare eventuali commenti inseriti nei  comandi di impaginazione
					while (1) 
					{
						(*ptrdocumento)++;
						if (**ptrdocumento == kMaggiore) 
							break;
					}
					(*ptrdocumento)++;
					// per saltare l'eventuale ritorno a capo
					if (**ptrdocumento == kRitornoACapo)
					{
						(*ptrdocumento)++;
						if (**ptrdocumento == kNuovaLinea)
						{
							(*ptrdocumento)++;
						}
					}
				}
				 else 
				{
					// devo catturare il nome dello stile fino alla parentesi angolare
					lPtrInizio = (*ptrdocumento);
					lContatore = 0;
					while (1) 
					{
						if (**ptrdocumento == kMaggiore) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
					// creo il puntatore alla stringa che deve contenere il testo
					// da importare
					(*ptrquattrod).testo = (uchar *) NewPtrClear(lContatore + 1);
					// copio il nome dello stile
					BlockMove((Ptr) lPtrInizio, (Ptr) (*ptrquattrod).testo, lContatore);
					// inserisco il carattere '\0' alla fine del testo
					(*ptrquattrod).testo[lContatore] = kFineFile;
					// salvo la lunghezza del campo testo
					(*ptrquattrod).lunghezzaTesto = lContatore;
					// dico che si tratta di uno stile debole
					(*ptrquattrod).tipoComando = kStileDebole;
					(*ptrquattrod).tipoRigore = kNessunRigore;
					(*ptrquattrod).tipoGruppo = kNessunGruppo;
					(*ptrquattrod).posizioneNelGruppo = kNessunaPosizione;
										
					// vado su carattere successivo alla parentesi angolare
					(*ptrdocumento)++;

				}
				break;
				
			case kPiu:
				// devo catturare il nome dello stile fino alla parentesi angolare
				(*ptrdocumento)++;
				lPtrInizio = (*ptrdocumento);
				lContatore = 0;
				while (1) 
				{
					if (**ptrdocumento == kMaggiore) 
						break;
					(*ptrdocumento)++;
					lContatore++;
				}
				// creo il puntatore alla stringa che deve contenere il testo da importare
				(*ptrquattrod).testo = (uchar *) NewPtrClear(lContatore + 1);
				// copio il nome dello stile 
				BlockMove((Ptr) lPtrInizio, (Ptr) (*ptrquattrod).testo, lContatore);
				// inserisco il carattere '\0' alla fine del testo
				(*ptrquattrod).testo[lContatore] = kFineFile;
				// salvo la lunghezza del campo testo
				(*ptrquattrod).lunghezzaTesto = lContatore;
				// dico che si tratta di uno stile debole
				(*ptrquattrod).tipoComando = kStileForte;
				(*ptrquattrod).tipoRigore = kNessunRigore;
				(*ptrquattrod).tipoGruppo = kNessunGruppo;
				(*ptrquattrod).posizioneNelGruppo = kNessunaPosizione;
				
				// vado su carattere successivo alla parentesi angolare
				(*ptrdocumento)++;
				break;
				
			case kDefunto:
				// devo vedere se il carattere successivo e' una chiocciola
				(*ptrdocumento)++;
				if (**ptrdocumento == kChiocciola) 
				{
					// devo catturare il codice del defunto fino al ;
					(*ptrdocumento)++;
					lPtrInizio = (*ptrdocumento);
					lContatore = 0;
					while (1) 
					{
						if (**ptrdocumento == kPuntoEVirgola) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
					
					if ( lContatore >= kDimensioneStringhe )
					{	
						// limito la lunghezza del campo a 255 caratteri
						lContatore = kDimensioneStringhe - 1;
					}
					// creo il puntatore alla stringa che deve contenere il testo da importare
					(*ptrquattrod).testo = (uchar *) NewPtrClear(lContatore + 1);
					// copio il codice del defunto
					BlockMove((Ptr) lPtrInizio, (Ptr) (*ptrquattrod).testo, lContatore);
					// inserisco il carattere '\0' alla fine del testo
					(*ptrquattrod).testo[lContatore] = kFineFile;
					// salvo la lunghezza del campo testo
					(*ptrquattrod).lunghezzaTesto = lContatore;
					// dico che si tratta di un defunto
					(*ptrquattrod).tipoComando = kDefunto;
					(*ptrquattrod).tipoGruppo = kNessunGruppo;
					(*ptrquattrod).posizioneNelGruppo = kNessunaPosizione;

					// devo catturare la posizione di rigore del defunto fino al >
					(*ptrdocumento)++;
					lPtrInizio = (*ptrdocumento);
					lContatore = 0;
					while (1) 
					{
						if (**ptrdocumento == kMaggiore) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
					if (lContatore == 1) 
					{
						if (lPtrInizio[0] == kRigoreDiDocumento) 
						{
							(*ptrquattrod).tipoRigore = kRigoreDiDocumento;
						} 
						else 
						{
							(*ptrquattrod).tipoRigore = kRigoreDiColonna;
						}
					} else 
					{
						(*ptrquattrod).tipoRigore = kNessunRigore;
					}

					(*ptrdocumento)++;
					
					// per saltare l'eventuale ritorno a capo
					if (**ptrdocumento == kRitornoACapo)
					{
						(*ptrdocumento)++;
						if (**ptrdocumento == kNuovaLinea)
						{
							(*ptrdocumento)++;
						}
					}
				}
				break;
				
			case kAnnuncio:
				// devo vedere se il carattere successivo e' una chiocciola
				(*ptrdocumento)++;
				if (**ptrdocumento == kChiocciola) 
				{
					// devo catturare il codice del necrologio fino al ;
					(*ptrdocumento)++;
					lPtrInizio = (*ptrdocumento);
					lContatore = 0;
					while (1) 
					{
						if (**ptrdocumento == kPuntoEVirgola) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
					// creo il puntatore alla stringa che deve contenere il testo da importare
					(*ptrquattrod).testo = (uchar *) NewPtrClear(lContatore + 1);
					// copio il codice del necrologio
					BlockMove((Ptr) lPtrInizio,  (Ptr) (*ptrquattrod).testo, lContatore);
					// inserisco il carattere '\0' alla fine del testo
					(*ptrquattrod).testo[lContatore] = kFineFile;
					// salvo la lunghezza del campo testo
					(*ptrquattrod).lunghezzaTesto = lContatore;
					// dico che si tratta di un necrologio
					(*ptrquattrod).tipoComando = kAnnuncio;
					(*ptrquattrod).tipoRigore = kNessunRigore;
#if 	0 // VECCHIA SINTASSI		
					// devo saltare la posizione di pagina fino alla ,
					(*ptrdocumento)++;
					while (1) 
					{
						if (**ptrdocumento == kVirgola) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
									
					// devo saltare la posizione di colonna fino al ;
					(*ptrdocumento)++;
					while (1) 
					{
						if (**ptrdocumento == kPuntoEVirgola) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
#endif // VECCHIA SINTASSI						
					// devo catturare il codice del gruppo fino alla ,
					(*ptrdocumento)++;
					lPtrInizio = (*ptrdocumento);
					lContatore = 0;
					while (1) 
					{
						if (**ptrdocumento == kVirgola) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
					// prendo il carattere del gruppo
					if (lContatore == 1) 
						(*ptrquattrod).tipoGruppo = lPtrInizio[0];
					else 
						(*ptrquattrod).tipoGruppo = kNessunGruppo;
					
					// devo catturare la posizione del gruppo fino al >
					(*ptrdocumento)++;
					lPtrInizio = (*ptrdocumento);
					lContatore = 0;
					while (1) 
					{
						if (**ptrdocumento == kMaggiore) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
					// prendo il carattere del gruppo
					if (lContatore == 1) 
						(*ptrquattrod).posizioneNelGruppo = lPtrInizio[0];
					else 
						(*ptrquattrod).posizioneNelGruppo = kNessunaPosizione;
					
					(*ptrdocumento)++;
					
					// per saltare l'eventuale ritorno a capo
					if (**ptrdocumento == kRitornoACapo)
					{
						(*ptrdocumento)++;
						if (**ptrdocumento == kNuovaLinea)
						{
							(*ptrdocumento)++;
						}
					}
				}
				break;
				
			case kAnniversario:
				// devo vedere se il carattere successivo e' una chiocciola
				(*ptrdocumento)++;
				if (**ptrdocumento == kChiocciola) 
				{
					// devo catturare il codice dell'anniversario fino al ;
					(*ptrdocumento)++;
					lPtrInizio = (*ptrdocumento);
					lContatore = 0;
					while (1) 
					{
						if (**ptrdocumento == kPuntoEVirgola) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
					// creo il puntatore alla stringa che deve contenere il testo da importare 
					(*ptrquattrod).testo = (uchar *) NewPtrClear(lContatore + 1);
					// copio il codice dell'anniversario
					BlockMove((Ptr) lPtrInizio,  (Ptr) (*ptrquattrod).testo, lContatore);
					// inserisco il carattere '\0' alla fine del testo
					(*ptrquattrod).testo[lContatore] = kFineFile;
					// salvo la lunghezza del campo testo
					(*ptrquattrod).lunghezzaTesto = lContatore;
					// dico che si tratta di un anniversario
					(*ptrquattrod).tipoComando = kAnniversario;
					(*ptrquattrod).tipoRigore = kNessunRigore;
#if 	0 // VECCHIA SINTASSI					
					// devo saltare la posizione di pagina fino alla ,
					(*ptrdocumento)++;
					while (1) 
					{
						if (**ptrdocumento == kVirgola) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
					
					// devo saltare la posizione di colonna fino al ;
					(*ptrdocumento)++;
					while (1) 
					{
						if (**ptrdocumento == kPuntoEVirgola) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
#endif // VECCHIA SINTASSI					
					// devo catturare il codice del gruppo fino alla ,
					(*ptrdocumento)++;
					lPtrInizio = (*ptrdocumento);
					lContatore = 0;
					while (1) 
					{
						if (**ptrdocumento == kVirgola) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
					// prendo il carattere del gruppo
					if (lContatore == 1) 
						(*ptrquattrod).tipoGruppo = lPtrInizio[0];
					else 
						(*ptrquattrod).tipoGruppo = kNessunGruppo;
					
					// devo catturare la posizione del gruppo fino al >
					(*ptrdocumento)++;
					lPtrInizio = (*ptrdocumento);
					lContatore = 0;
					while (1) 
					{
						if (**ptrdocumento == kMaggiore) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
					// prendo il carattere del gruppo
					if (lContatore == 1) 
						(*ptrquattrod).posizioneNelGruppo = lPtrInizio[0];
					else 
						(*ptrquattrod).posizioneNelGruppo = kNessunaPosizione;

					(*ptrdocumento)++;
					// per saltare l'eventuale ritorno a capo
					if (**ptrdocumento == kRitornoACapo)
					{
						(*ptrdocumento)++;
						if (**ptrdocumento == kNuovaLinea)
						{
							(*ptrdocumento)++;
						}
					}
					
				}
				break;
				
			case kRingraziamento:
				// devo vedere se il carattere successivo e' una chiocciola 
				(*ptrdocumento)++;
				if (**ptrdocumento == kChiocciola) 
				{
					// devo catturare il codice del ringraziamento fino al ;
					(*ptrdocumento)++;
					lPtrInizio = (*ptrdocumento);
					lContatore = 0;
					while (1) 
					{
						if (**ptrdocumento == kPuntoEVirgola) break;
						(*ptrdocumento)++;
						lContatore++;
					}
					// creo il puntatore alla stringa che deve contenere il testo
					// da importare
					(*ptrquattrod).testo = (uchar *) NewPtrClear(lContatore + 1);
					// copio il codice del defunto
					BlockMove((Ptr) lPtrInizio, (Ptr) (*ptrquattrod).testo, lContatore);
					// inserisco il carattere '\0' alla fine del testo 
					(*ptrquattrod).testo[lContatore] = kFineFile;
					// salvo la lunghezza del campo testo
					(*ptrquattrod).lunghezzaTesto = lContatore;
					// dico che si tratta di un ringraziamento
					(*ptrquattrod).tipoComando = kRingraziamento;
					(*ptrquattrod).tipoRigore = kNessunRigore;
					// devo saltare la posizione di pagina fino alla ,
#if 	0 // VECCHIA SINTASSI					
					(*ptrdocumento)++;
					while (1) 
					{
						if (**ptrdocumento == kVirgola) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
					
					// devo saltare la posizione di colonna fino al ; 
					(*ptrdocumento)++;
					while (1) 
					{
						if (**ptrdocumento == kPuntoEVirgola) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
#endif // VECCHIA SINTASSI 					
					// devo catturare il codice del gruppo fino alla ,
					(*ptrdocumento)++;
					lPtrInizio = (*ptrdocumento);
					lContatore = 0;
					while (1) 
					{
						if (**ptrdocumento == kVirgola) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
					// prendo il carattere del gruppo
					if (lContatore == 1) 
						(*ptrquattrod).tipoGruppo = lPtrInizio[0];
					else 
						(*ptrquattrod).tipoGruppo = kNessunGruppo;
					
					// devo catturare la posizione del gruppo fino al > 
					(*ptrdocumento)++;
					lPtrInizio = (*ptrdocumento);
					lContatore = 0;
					while (1) 
					{
						if (**ptrdocumento == kMaggiore) 
							break;
						(*ptrdocumento)++;
						lContatore++;
					}
					// prendo il carattere del gruppo
					if (lContatore == 1) 
						(*ptrquattrod).posizioneNelGruppo = lPtrInizio[0];
					else 
						(*ptrquattrod).posizioneNelGruppo = kNessunaPosizione;

					(*ptrdocumento)++;
					// per saltare l'eventuale ritorno a capo
					if (**ptrdocumento == kRitornoACapo)
					{
						(*ptrdocumento)++;
						if (**ptrdocumento == kNuovaLinea)
						{
							(*ptrdocumento)++;
						}
					}
					
				} 
				break;
				
			case kPax:
				// devo vedere se il carattere successivo e' una chiocciola seguita dalla parola Pax
				(*ptrdocumento)++;
				if ( (**ptrdocumento == kChiocciola) && ((*(*ptrdocumento + 1)) == 'P') && ((*(*ptrdocumento + 2)) == 'a') && ((*(*ptrdocumento + 3)) == 'x') )
				{
						// salto fino al > finale
						(*ptrdocumento) += 5;

						// per saltare l'eventuale ritorno a capo
						if (**ptrdocumento == kRitornoACapo)
						{
							(*ptrdocumento)++;
							if (**ptrdocumento == kNuovaLinea)
							{
								(*ptrdocumento)++;
							}
						}
						
						// imposto il descrittore del comando
						(*ptrquattrod).tipoComando = kPax;

						//gli altri campi sono inutilizzati
						(*ptrquattrod).testo = NULL;
						(*ptrquattrod).lunghezzaTesto = 0L;
						(*ptrquattrod).tipoRigore = kNessunRigore;
						(*ptrquattrod).tipoGruppo = kNessunGruppo;
						(*ptrquattrod).posizioneNelGruppo = kNessunaPosizione;
				}
				break;	
		} // switch (*ptrdocumento) 
	} 
	else 
	{
		// si tratta di testo da leggere
		lPtrInizio = (*ptrdocumento);
		(*ptrdocumento)++;
		lContatore = 1;

		// determino la lunghezza del testo da leggere
		while (1) 
		{
			if (**ptrdocumento == kMinore) 
				break;
			if (**ptrdocumento == kFineFile) 
				break;
			(*ptrdocumento)++;
			lContatore++;
		}
		// creo il puntatore alla stringa che deve contenere il testo da importare
		(*ptrquattrod).testo = (uchar *) NewPtrClear(lContatore + 1);
		// copio il testo
		BlockMove((Ptr) lPtrInizio, (Ptr) (*ptrquattrod).testo, lContatore);
		// inserisco il carattere '\0' alla fine del testo
		(*ptrquattrod).testo[lContatore] = kFineFile;
		// salvo la lunghezza del testo 
		(*ptrquattrod).lunghezzaTesto = lContatore;
		// dico che si tratta di testo
		(*ptrquattrod).tipoComando = kTesto;
		(*ptrquattrod).tipoRigore = kNessunRigore;
		(*ptrquattrod).tipoGruppo = kNessunGruppo;
		(*ptrquattrod).posizioneNelGruppo = kNessunaPosizione;
	}
} // LeggiQuattroD

/* ------------------------------------------------------------------------ *

	InizializzaQuattroD

* ------------------------------------------------------------------------ */
void XTAPI InizializzaQuattroD(quattrod *ptrquattrod) throw()
{
	(*ptrquattrod).tipoComando = kNessunComando;
	if ((*ptrquattrod).testo != NULL) 
	{
		DisposePtr((Ptr) (*ptrquattrod).testo);
		(*ptrquattrod).testo = NULL;
	}
	(*ptrquattrod).lunghezzaTesto = 0;
	(*ptrquattrod).tipoRigore = kNessunRigore;
	(*ptrquattrod).tipoGruppo = kNessunGruppo;
	(*ptrquattrod).posizioneNelGruppo = kNessunaPosizione;
}  // InizializzaQuattroD

/* ------------------------------------------------------------------------ *

	LeggiContenutoFSSpecFile

* ------------------------------------------------------------------------ */
errorixtension XTAPI LeggiContenutoFSSpecFile(uchar *name, Handle *ptrhandle) throw()
{
	// identificatore del file da leggere
	int16 lIdentificatoreFile = 0;
	
	// lunghezza del file da leggere
	int32 lLunghezzaFile = 0;

	// apertura del file
	gErrore = FSOpen(name, 0, &lIdentificatoreFile);
	if (gErrore != noErr) 
	{
		return(kErroreLetturaFile);
	}
	
	// imposto la lettura all'inizio del file
	gErrore = SetFPos(lIdentificatoreFile, fsFromStart, 0);
	if (gErrore != noErr) 
	{
		return(kErroreLetturaFile);
	}

	// lettura della dimensione del file
	gErrore = GetEOF(lIdentificatoreFile, &lLunghezzaFile);
	if (gErrore != noErr) 
	{
		return(kErroreLetturaFile);
	}
	
	// gHndlFile = (Handle)NewHandleClear(lLunghezzaFile+1);
	CreaHandle(&gHndlFile, lLunghezzaFile+1);
  	if (gHndlFile == NULL) 
  	{
		return(kErroreMemoria);
	}

	// lock dell'handle appena creato
	HLock (gHndlFile);

	// lettura del file all'interno del handle
	gErrore = FSRead(lIdentificatoreFile, &lLunghezzaFile, (*gHndlFile));
	if (gErrore != noErr) 
	{
		LiberaTrueHandle((Handle)gHndlFile);
		return(kErroreLetturaFile);
	}

	// chiusura del file
	gErrore = FSClose(lIdentificatoreFile);
	if (gErrore != noErr) 
	{
		LiberaTrueHandle(gHndlFile);
		return(kErroreLetturaFile);
	}

	return(kNessunErrore);
} // LeggiContenutoFSSpecFile


/* ------------------------------------------------------------------------ *

	LeggiNormContenutoFSSpecFile

* ------------------------------------------------------------------------ */
errorixtension XTAPI LeggiNormContenutoFSSpecFile(uchar *name, Handle *ptrhandle) throw()
{
	// identificatore del file da leggere
	int16 lIdentificatoreFile = 0;
	// lunghezza del file da leggere
	int32 lLunghezzaFile = 0;
	
	// apertura del file
	gErrore = FSOpen(name, 0, &lIdentificatoreFile);
	if (gErrore != noErr) 
	{
		return(kErroreLetturaFile);
	}
	// imposto la lettura all'inizio del file
	gErrore = SetFPos(lIdentificatoreFile, fsFromStart, 0);
	if (gErrore != noErr) 
	{
		return(kErroreLetturaFile);
	}

	// lettura della dimensione del file
	gErrore = GetEOF(lIdentificatoreFile, &lLunghezzaFile);
	if (gErrore != noErr) 
	{
		return(kErroreLetturaFile);
	}

	// creazione del handle
	gErroreXtension = CreaHandle(&gHndl, lLunghezzaFile + 1);
	if (gErroreXtension != kNessunErrore) 
	{
		// si e' verificato un errore nell'allocare la memoria
		return(gErroreXtension);
	}

	// lettura del file all'interno del handle
	gErrore = FSRead(lIdentificatoreFile, &lLunghezzaFile, (*gHndlFile));
	if (gErrore != noErr) 
	{
		LiberaHandle((Handle *) gHndlFile);
		return(kErroreLetturaFile);
	}

	// chiusura del file
	gErrore = FSClose(lIdentificatoreFile);
	if (gErrore != noErr) 
	{
		LiberaHandle((Handle *) gHndlFile);
		return(kErroreLetturaFile);
	}

	// la lettura e' andata a buon fine
	(*ptrhandle) = gHndl;
	
	return(kNessunErrore);
} // LeggiNormContenutoFSSpecFile

/* ------------------------------------------------------------------------ *

	LeggiContenutoFile

* ------------------------------------------------------------------------ */
errorixtension XTAPI LeggiContenutoFile(int16 identificatorefile, Handle *ptrhandle) throw()
{
	// lunghezza del file da leggere
	int32  lLunghezzaFile = 0;
	
	// imposto la lettura all'inizio del file
	gErrore = SetFPos(identificatorefile, fsFromStart, 0);
	if (gErrore != noErr) 
	{
		return(kErroreLetturaFile);
	}
		
	// lettura della dimensione del file
	gErrore = GetEOF(identificatorefile, &lLunghezzaFile);
	if (gErrore != noErr) 
	{
		return(kErroreLetturaFile);
	}
	
	// creazione del handle 
	gErroreXtension = CreaHandle(&gHndl, lLunghezzaFile);
	if (gErroreXtension != kNessunErrore) 
	{
		return(gErroreXtension);
	}
	
	// lettura del file all'interno del handle
	gErrore = FSRead(identificatorefile, &lLunghezzaFile, *gHndl);
	if (gErrore != noErr) 
	{
		LiberaHandle((Handle *) &gHndl);
		return(kErroreLetturaFile);
	}
	
	// la lettura e' andata a buon fine
	(*ptrhandle) = gHndl;
	
	return(kNessunErrore);

} // LeggiContenutoFile

/* ------------------------------------------------------------------------ *

	CancellaFile

* ------------------------------------------------------------------------ */
errorixtension XTAPI CancellaFile(uchar *name) throw()
{
	// costruisco FSSpec
	FSSpec spec;
	FSMakeFSSpec(0, 0, name, &spec);

	// per cancellare
	gErrore = FSDelete(name, spec.vRefNum);  /* 0 vuol dire drive corrente */
	if (gErrore != noErr) 
	{
		return(kErroreDiSistema);
	}
	
	return(kNessunErrore);

} // CancellaFile

