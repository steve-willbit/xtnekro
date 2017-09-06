
/* ------------------------------------------------------------------------ *

	GestioneSlug.cpp

	14 Aprile 2005 				Fabrizio

	Descrizione:
	Contiene le procedure usate per la gestione dei comandi di impaginazione
	
	$Log: not supported by cvs2svn $
	Revision 1.1  2005/04/20 07:14:19  taretto
	importazione files
	
	
* ------------------------------------------------------------------------ */

// CONFIG INCLUDES
// CONFIG INCLUDES
// CONFIG INCLUDES

// always the first
#include "XTConfig.h"
#include "QXPConfig.h"

// STANDARD INCLUDES
// STANDARD INCLUDES
// STANDARD INCLUDES

#if QXP60
#if defined(__MWERKS__) && defined(__MACH__)
	#define TARGET_API_MAC_OSX 1
	#include <MSL MacHeadersMach-O.h>
#endif // defined(__MWERKS__) && defined(__MACH__)
#endif // QXP60

#include <string.h>

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "Errori.h"
#include "Memoria.h"
#include "ReadFile.h"
#include "AppWare.h"

#include "GesSlug.h"

// STATICS GLOBALS
// STATICS GLOBALS
// STATICS GLOBALS

// codice interno da associare ai box impaginai dal sistema
static uint32 gCodiceInterno = 0;

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	PrendiCodiceBox

* ------------------------------------------------------------------------ */
uint32 XTAPI PrendiCodiceBox() throw()
{
	return(gCodiceInterno);
} // PrendiCodiceBox

/* ------------------------------------------------------------------------ *

	SlugDiCodice

* ------------------------------------------------------------------------ */
errorixtension XTAPI SlugDiCodice() throw()
{
	// handle al codice da associare al box corrente
	handlecodicebox lHndlCodiceBox = NULL;
		
	gErroreXtension = CreaHandle((Handle *) &lHndlCodiceBox, sizeof(codicebox));
	if (gErroreXtension != kNessunErrore) 
	{
		return(gErroreXtension);
	}

	// incremento il codice interno
	gCodiceInterno++;

	(**lHndlCodiceBox).codiceBox = gCodiceInterno;
	setslug(NULL, OBJECTSLUG, kSlugDiCodice, (Handle) lHndlCodiceBox);
	verydirty();
	LiberaHandle((Handle *) &lHndlCodiceBox);

	return(kNessunErrore);
} // SlugDiCodice

/* ------------------------------------------------------------------------ *

	TrovaBoxConSlug

* ------------------------------------------------------------------------ */
errorixtension XTAPI TrovaSlugDiCodice(uint32 codicebox) throw()
{
	// handle al codice da associare al box corrente
	handlecodicebox lHndlCodiceBox = NULL;
	// identificatore del box trovato
	boxid lIdentificatoreBoxTrovato = 0;
	
	gErroreXtension = CreaHandle((Handle *) &lHndlCodiceBox, sizeof(codicebox));
	if (gErroreXtension != kNessunErrore) 
	{
		// impossibile alloccare memoria
		return(gErroreXtension);
	}
	
	(** lHndlCodiceBox).codiceBox = codicebox;
	lIdentificatoreBoxTrovato = findslug(OBJECTSLUG, kSlugDiCodice, (Handle) lHndlCodiceBox);
	LiberaHandle((Handle *) &lHndlCodiceBox);
	if (lIdentificatoreBoxTrovato != 0) 
	{
		xtset_curbox(lIdentificatoreBoxTrovato);
		return(kNessunErrore);
	} 
	else 
	{
		// non ha trovato alcun box
		return(kErroreRiimpaginazioneSuDocumentoSbagliato);
	}
} // TrovaSlugDiCodice

/* ------------------------------------------------------------------------ *

	IsBoxCorrenteMio

* ------------------------------------------------------------------------ */
bool8 XTAPI IsBoxCorrenteMio() throw()
{
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	// handle al codice da associare al box corrente
	handlecodicebox lHndlCodiceBox = NULL;
	
	// prendo lo slug del box corrente
	lHndlCodiceBox = (handlecodicebox) getslug(curbox, OBJECTSLUG, kSlugDiCodice);
	
	if (lHndlCodiceBox == NULL) 
	{
		// non c'e' il mio slug
		return(FALSE);
	} 
	else 
	{
		LiberaHandle((Handle *) &lHndlCodiceBox);
		return(TRUE);
	}
} // IsBoxCorrenteMio

/* ------------------------------------------------------------------------ *

	SlugDiImmagine

* ------------------------------------------------------------------------ */
errorixtension XTAPI  SlugDiImmagine(uchar* nomeimmagine) throw()
{
	// handle all'immagine da associare al box corrente
	handleimmagine lHndlImmagine = NULL;	
	
	gErroreXtension = CreaHandle((Handle *) &lHndlImmagine, sizeof(immagine));
	if (gErroreXtension != kNessunErrore) 
	{
		// impossibile alloccare memoria
		return(gErroreXtension);
	}
	
	STRCPY((**lHndlImmagine).nomeImmagine, nomeimmagine);
	setslug(NULL, OBJECTSLUG, kSlugDiImmagine, (Handle) lHndlImmagine);					
	verydirty();
	LiberaHandle((Handle *) &lHndlImmagine);
	
	return(kNessunErrore);
} // SlugDiImmagine

/* ------------------------------------------------------------------------ *

	PrendiSlugDiImmagine

* ------------------------------------------------------------------------ */
errorixtension XTAPI PrendiSlugDiImmagine(uchar* nomeimmagine) throw()
{
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);

	// handle all'immagine da prendere dal box corrente
	handleimmagine lHndlImmagine = NULL;
	
	// prendo lo slug del box corrente
	lHndlImmagine = (handleimmagine) getslug(curbox, OBJECTSLUG, kSlugDiImmagine);
	
	if (lHndlImmagine == NULL) 
	{
		// non e' stato associato nessuno slug al box corrente
		return(kNessunSlug);
	} 
	else 
	{
		if (nomeimmagine != NULL)
			STRCPY(nomeimmagine, (**lHndlImmagine).nomeImmagine);
		LiberaHandle((Handle *) &lHndlImmagine);
		return(kNessunErrore);
	}
} // PrendiSlugDiImmagine

/* ------------------------------------------------------------------------ *

	SlugDiAppWare

* ------------------------------------------------------------------------ */
errorixtension XTAPI  SlugDiAppWare(appware *ptrappware) throw()
{
	// handle alla struttura AppWare da associare al box corrente
	handleappware lHndlAppWare = NULL;
	
	gErroreXtension = CreaHandle((Handle *) &lHndlAppWare, sizeof(appware));
	if (gErroreXtension != kNessunErrore) 
	{
		// impossibile alloccare memoria
		return(gErroreXtension);
	}

	(**lHndlAppWare) = (*ptrappware);
	setslug(NULL, OBJECTSLUG, kSlugDiAppWare, (Handle) lHndlAppWare);					
	verydirty();
	LiberaHandle((Handle *) &lHndlAppWare);
	return(kNessunErrore);
} // SlugDiAppWare

/* ------------------------------------------------------------------------ *

	PrendiSlugDiAppWare

* ------------------------------------------------------------------------ */
errorixtension XTAPI PrendiSlugDiAppWare(appware *ptrappware) throw()
{
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	// handle alla struttura AppWare da associare al box corrente 
	handleappware lHndlAppWare = NULL;
	
	// prendo lo slug del box corrente
	lHndlAppWare = (handleappware) getslug(curbox, OBJECTSLUG, kSlugDiAppWare);
	
	if (lHndlAppWare == NULL) 
	{
		// non e' stato associato nessuno slug al box corrente
		return(kNessunSlug);
	} 
	else 
	{
		if (ptrappware != NULL) 
			(*ptrappware) = (**lHndlAppWare);
		LiberaHandle((Handle *) &lHndlAppWare);
		return(kNessunErrore);
	}
} // PrendiSlugDiAppWare

/* ------------------------------------------------------------------------ *

	SlugDiDocumento

* ------------------------------------------------------------------------ */
errorixtension XTAPI  SlugDiDocumento(uint32 codicedocumento) throw()
{
	// handle alla struttura del codice documento
	handlecodicedoc lHndlCodiceDoc = NULL;
	
	gErroreXtension = CreaHandle((Handle *) &lHndlCodiceDoc, sizeof(codicedoc));
	if (gErroreXtension != kNessunErrore) 
	{
		// impossibile alloccare memoria
		return(gErroreXtension);
	}

	(**lHndlCodiceDoc).codiceDocumento = codicedocumento;
	setslug(NULL, DOCSLUG, kSlugDiDocumento, (Handle) lHndlCodiceDoc);					
	verydirty();
	LiberaHandle((Handle *) &lHndlCodiceDoc);
	return(kNessunErrore);
} // SlugDiDocumento

/* ------------------------------------------------------------------------ *

	PrendiCodiceDocumento

* ------------------------------------------------------------------------ */
uint32 XTAPI PrendiCodiceDocumento() throw()
{
	// handle alla struttura del codice documento
	handlecodicedoc lHndlCodiceDoc = NULL;
	// per prendere il codice del documento corrente
	uint32 lCodiceDocumento = 0;
	
	// prendo lo slug del documento corrente corrente
	lHndlCodiceDoc = (handlecodicedoc) getslug(NULL, DOCSLUG, kSlugDiDocumento);
	
	if (lHndlCodiceDoc == NULL) 
	{
		// al documento corrente non e' stato associato alcun slug di documento
		return(0);
	} 
	else 
	{
		lCodiceDocumento = (**lHndlCodiceDoc).codiceDocumento;
		LiberaHandle((Handle *) &lHndlCodiceDoc);
		return(lCodiceDocumento);
	}
} // PrendiCodiceDocumento

/* ------------------------------------------------------------------------ *

	SlugDiImmagineCampione

* ------------------------------------------------------------------------ */
errorixtension XTAPI SlugDiImmagineCampione() throw()
{
	// handle all'immagine campione da associare al box corrente
	handleimmaginecampione lHndlImmagineCampione = NULL;
	
	gErroreXtension = CreaHandle((Handle *) &lHndlImmagineCampione, sizeof(immaginecampione));
	if (gErroreXtension != kNessunErrore) 
	{
		// impossibile alloccare memoria
		return(gErroreXtension);
	}
	
	(**lHndlImmagineCampione).immagineCampione = TRUE;
	setslug(NULL, OBJECTSLUG, kSlugDiImmagineCampione, (Handle) lHndlImmagineCampione);					
	verydirty();
	LiberaHandle((Handle *) &lHndlImmagineCampione);
	
	return(kNessunErrore);
} // SlugDiImmagineCampione

/* ------------------------------------------------------------------------ *

	TrovaSlugDiImmagineCampione

* ------------------------------------------------------------------------ */
errorixtension XTAPI TrovaSlugDiImmagineCampione() throw()
{
	// handle allo slug dell'immagine campione
	handleimmaginecampione lHndlImmagineCampione = NULL;
	// identificatore del box trovato 
	boxid lIdentificatoreBoxTrovato = 0;
	
	gErroreXtension = CreaHandle((Handle *) &lHndlImmagineCampione, sizeof(immaginecampione));
	if (gErroreXtension != kNessunErrore) 
	{
		// impossibile alloccare memoria
		return(gErroreXtension);
	}
	
	(** lHndlImmagineCampione).immagineCampione = TRUE;
	lIdentificatoreBoxTrovato = findslug(OBJECTSLUG, kSlugDiImmagineCampione, (Handle) lHndlImmagineCampione);
	LiberaHandle((Handle *) &lHndlImmagineCampione);
	if (lIdentificatoreBoxTrovato != 0) 
	{
		xtset_curbox(lIdentificatoreBoxTrovato);
		return(kNessunErrore);
	} 
	else 
	{
		// non ha trovato alcun box
		return(kNessunSlug);
	}
} // TrovaSlugDiImmagineCampione

/* ------------------------------------------------------------------------ *

	SlugDiIngombro
	
* ------------------------------------------------------------------------ */
errorixtension XTAPI SlugDiIngombro(int16 colonna) throw()
{
	// handle al codice da associare al box di ingombro corrente 
	handlecodiceingombro lHndlCodiceIngombro = NULL;
	
	gErroreXtension = CreaHandle((Handle *) &lHndlCodiceIngombro, sizeof(codiceingombro));
	if (gErroreXtension != kNessunErrore) 
	{
		// impossibile alloccare memoria
		return(gErroreXtension);
	}
	
	(**lHndlCodiceIngombro).colonna = colonna;
	setslug(NULL, OBJECTSLUG, kSlugDiIngombro, (Handle) lHndlCodiceIngombro);					
	verydirty();
	LiberaHandle((Handle *) &lHndlCodiceIngombro);

	return(kNessunErrore);
} // SlugDiIngombro

/* ------------------------------------------------------------------------ *

	TrovaSlugDiIngombro

* ------------------------------------------------------------------------ */
errorixtension XTAPI TrovaSlugDiIngombro(int16 colonna) throw()
{
	// handle al codice da associare al box ingombro
	handlecodiceingombro lHndlCodiceIngombro = NULL;
	// identificatore del box trovato
	boxid lIdentificatoreBoxTrovato = 0;
	
	gErroreXtension = CreaHandle((Handle *) &lHndlCodiceIngombro, sizeof(codiceingombro));
	if (gErroreXtension != kNessunErrore) 
	{
		// impossibile alloccare memoria
		return(gErroreXtension);
	}
	
	(**lHndlCodiceIngombro).colonna = colonna;
	lIdentificatoreBoxTrovato = findslug(OBJECTSLUG,
										  kSlugDiIngombro,
										  (Handle) lHndlCodiceIngombro);
	LiberaHandle((Handle *) &lHndlCodiceIngombro);
	if (lIdentificatoreBoxTrovato != 0) 
	{
		xtset_curbox(lIdentificatoreBoxTrovato);
		return(kNessunErrore);
	} 
	else 
	{
		// non ha trovato alcun box
		return(kErroreRicercaIngombro);
	}
} // TrovaSlugDiIngombro

/* ------------------------------------------------------------------------ *

	SlugDiPax

* ------------------------------------------------------------------------ */
errorixtension XTAPI SlugDiPax() throw()
{
	// handle al codice da associare al box corrente
	handlemarcatorepax lHndlMarcatorePax = NULL;
	
	gErroreXtension = CreaHandle((Handle *) &lHndlMarcatorePax, sizeof(marcatorepax));
	if (gErroreXtension != kNessunErrore) 
	{
		// impossibile alloccare memoria
		return(gErroreXtension);
	}
	
	(**lHndlMarcatorePax).marcatorePax = 3;
	setslug(NULL, OBJECTSLUG, kSlugDiPax, (Handle) lHndlMarcatorePax);					
	verydirty();
	LiberaHandle((Handle *) &lHndlMarcatorePax);

	return(kNessunErrore);

} // SlugDiPax

/* ------------------------------------------------------------------------ *

	TrovaSlugDiPax

* ------------------------------------------------------------------------ */
errorixtension XTAPI TrovaSlugDiPax(boxid *boxtrovato) throw()
{
	handlemarcatorepax lHndlMarcatorePax = NULL;
	/* handle al codice da associare al box corrente */
	boxid lIdentificatoreBoxTrovato = 0;
	/* identificatore del box trovato */	
	
	//resetto il parametro
	*boxtrovato = 0L;

	gErroreXtension = CreaHandle((Handle *) &lHndlMarcatorePax, sizeof(marcatorepax));
	if (gErroreXtension != kNessunErrore) 
	{
		// impossibile alloccare memoria
		return(gErroreXtension);
	}
	
	(** lHndlMarcatorePax).marcatorePax = 3;
	lIdentificatoreBoxTrovato = findslug(OBJECTSLUG, kSlugDiPax, (Handle) lHndlMarcatorePax);
	LiberaHandle((Handle *) &lHndlMarcatorePax);
	if (lIdentificatoreBoxTrovato != 0)
	{
		// imposto il valore di ritorno
		*boxtrovato = lIdentificatoreBoxTrovato;

		// e ritorno il messaggio che esisteva gia'
		return(kErroreSlugSimboloPaxPresente);
	}
	else
	{
		/* non ha trovato alcun box */
		return(kNessunErrore);
	}
} // TrovaSlugDiPax

/* ------------------------------------------------------------------------ *

	RimuoviSlugDiPax

* ------------------------------------------------------------------------ */
void XTAPI RimuoviSlugDiPax(boxid vecchiobox) throw()
{
	setslug( vecchiobox, OBJECTSLUG, kSlugDiPax, (Handle) NULL );
	verydirty();
} // RimuoviSlugDiPax
