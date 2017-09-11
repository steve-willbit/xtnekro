
/* ----------------------------------------------------------------------------------- *

	DocInfo.cpp

	11 Aprile 2005						Fabrizio

	Descrizione:
	Contiene delle procedure per prelevare informazioni sul box corrente
	
	$Log: not supported by cvs2svn $
	Revision 1.1  2005/04/20 07:14:18  taretto
	importazione files
	

* ----------------------------------------------------------------------------------- */

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

#include "Coord.h"
#include "Errori.h"
#include "Pref.h"
#include "XTNecro.h"

#include "DocInfo.h"

// CONSTS
// CONSTS
// CONSTS

#define kPrimoSpread 1
#define kZeroCinquePunti 0x00008000

// STATICS GLOBALS
// STATICS GLOBALS
// STATICS GLOBALS

// record che contiene le informazioni sulla pagina
static XTUNewPageInfo *gPtrInfoPagina = NULL;

static Fixed gAltezzaPagina;
static Fixed gLarghezzaPagina;
static bool8 gDoubleSided;
static DocID gCurDoc;

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	PrendiDocInfo

* ------------------------------------------------------------------------ */
errorixtension XTAPI PrendiDocInfo() throw()
{
	// identificatore del boxid dello spread
	spreadboxid  lIdBoxSpread = 0;
	// punatore allo spread 
	xtboxptr lPtrBoxSpread = NULL;
	// handle ai dati delle pagine
	XTUPageDataHandle lHndlDatiPagina = NULL;
	// doc corrente
	DocID gCurDoc;
	xtget_curdoc(&gCurDoc);
	
	if (gCurDoc >= 0) 
	{
		// sono sicuro che e' aperto un documento
		// annullo le info precendenti
		gAltezzaPagina = 0;
		gLarghezzaPagina = 0;
		gDoubleSided = 0;
		
		// (ri)prendo le info
		DocRef DRef = XTGetDocRefFromDocID(gCurDoc);
		XTGetDocPageLength(DRef, &gAltezzaPagina);
		XTGetDocPageWidth(DRef, &gLarghezzaPagina);
		XTGetDoubleSided(DRef, &gDoubleSided);
		
		if (gPtrInfoPagina != NULL) 
		{
			DisposePtr((Ptr) gPtrInfoPagina);
			gPtrInfoPagina = NULL;
		}

		// creo il puntatore per le info della pagina 
		gPtrInfoPagina = (XTUNewPageInfo *) NewPtrClear(sizeof(XTUNewPageInfo));
		if (gPtrInfoPagina == NULL) {
			// errore nell'allocazione della memoria 
			return(kErroreMemoria);
		}

		// prendo spread box id
		XTGetSpreadBox(DRef, kPrimoSpread, &lIdBoxSpread);
		
		// prendo puntatore al box
		lPtrBoxSpread = getxtbox(NULL, lIdBoxSpread, FALSE);  
		lHndlDatiPagina = lPtrBoxSpread->g.pagestuff;
		(*gPtrInfoPagina).top = (**lHndlDatiPagina).pgs[0].top;		
		(*gPtrInfoPagina).left = (**lHndlDatiPagina).pgs[0].left;		
		(*gPtrInfoPagina).bottom = (**lHndlDatiPagina).pgs[0].bottom;		
		(*gPtrInfoPagina).right = (**lHndlDatiPagina).pgs[0].right;		
		(*gPtrInfoPagina).colcount = (**lHndlDatiPagina).pgs[0].colcount;		
		(*gPtrInfoPagina).gutter = (**lHndlDatiPagina).pgs[0].gutter;		
		
		// dispose del puntatore all spread box
		disposextbox(lPtrBoxSpread,TRUE);  
		
	} else 
	{
		return(kErroreDelCodice);
	}
	
	return(kNessunErrore);
} // PrendiDocInfo

/* ------------------------------------------------------------------------ *

	PrendiMargineAlto

* ------------------------------------------------------------------------ */
Fixed XTAPI PrendiMargineAlto() throw()
{
	// if (gPtrDocInfoRec != NULL) 
	if (gCurDoc >= 0)
	{
		 // sono sicuro che e' aperto un documento
		return((*gPtrInfoPagina).top - kZeroCinquePunti + gXtPreferences.documentoPref.altezzaTestata);
	}
	 else 
	 {
		// non c'e' nessun documento aperto correntemente
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
		return(0);
	}
} // PrendiMargineAlto

/* ------------------------------------------------------------------------ *

	PrendiMargineBasso

* ------------------------------------------------------------------------ */
Fixed XTAPI PrendiMargineBasso() throw()
{
	Fixed lAltezzaPagina = 0;
	Fixed lMargineBasso = 0;
	Fixed lFondoColonna = 0;
	
	if (gCurDoc >= 0) 
	{
		// sono sicuro che e' aperto un documento
		lMargineBasso = (*gPtrInfoPagina).bottom - kZeroCinquePunti;
		lFondoColonna = gAltezzaPagina - lMargineBasso;
		return(lFondoColonna);
	} 
	else 
	{
		// non c'e' nessun documento aperto correntemente
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
		return(0);
	}
} // PrendiMargineBasso

/* ------------------------------------------------------------------------ *

	PrendiMargineSinistro

* ------------------------------------------------------------------------ */
Fixed XTAPI PrendiMargineSinistro() throw()
{
	if (gCurDoc >= 0) 
	{
		// sono sicuro che e' aperto un documento
		return((*gPtrInfoPagina).left - kZeroCinquePunti);
	} 
	else 
	{
		// non c'e' nessun documento aperto correntemente
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
		return(0);
	}
} // PrendiMargineSinistro

/* ------------------------------------------------------------------------ *

	PrendiMargineDestro

* ------------------------------------------------------------------------ */
Fixed XTAPI PrendiMargineDestro() throw()
{
	Fixed lMargineDestro = 0;
	
	if (gCurDoc >= 0)  
	{
		// sono sicuro che e' aperto un documento
		lMargineDestro = gLarghezzaPagina - (*gPtrInfoPagina).right - kZeroCinquePunti;
		return(lMargineDestro);
	} 
	else 
	{
		//non c'e' nessun documento aperto correntemente
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
		return(0);
	}
} // PrendiMargineDestro

/* ------------------------------------------------------------------------ *

	PrendiLarghezzaColonna

* ------------------------------------------------------------------------ */
Fixed XTAPI PrendiLarghezzaColonna() throw()
{
	int16 lNumeroColonne = 0;
	Fixed lLarghezzaPagina = 0;
	Fixed lMargineSinistro = 0;
	Fixed lMargineDestro = 0;
	Fixed lDistanzaTraColonne = 0;
	Fixed lLarghezzaColonna = 0;
	
	if (gCurDoc >= 0) 
	{
		// sono sicuro che e' aperto un documento 		
		lNumeroColonne = (*gPtrInfoPagina).colcount;
		lLarghezzaPagina = gLarghezzaPagina;
		lMargineSinistro = (*gPtrInfoPagina).left - kZeroCinquePunti;
		lMargineDestro = (*gPtrInfoPagina).right - kZeroCinquePunti;
		lDistanzaTraColonne = (*gPtrInfoPagina).gutter;
		lLarghezzaColonna = lLarghezzaPagina - lMargineSinistro - lMargineDestro;
		lLarghezzaColonna -= (lDistanzaTraColonne * (long) (lNumeroColonne - 1));

		// variabile d'utilita
		uchar tmpstr[kDimensioneStringhe] = "";
		wsprintf((char*)tmpstr, "Errore di divisione per zero.\nSorgente: %s, Linea: %d.",__FILE__, __LINE__);

		return(lLarghezzaColonna / (long) lNumeroColonne);
	} 
	else 
	{
		// non c'e' nessun documento aperto correntemente
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
		return(0);
	}
} // PrendiLarghezzaColonna

/* ------------------------------------------------------------------------ *

	PrendiAltezzaColonna

* ------------------------------------------------------------------------ */
Fixed XTAPI PrendiAltezzaColonna() throw()
{
	Fixed lAltezzaPagina = 0;
	Fixed lMargineAlto = 0;
	Fixed lMargineBasso = 0;
	
	if (gCurDoc >= 0) 
	{
		// sono sicuro che e' aperto un documento
		lAltezzaPagina = gAltezzaPagina;
		lMargineAlto = (*gPtrInfoPagina).top - kZeroCinquePunti;
		lMargineBasso = (*gPtrInfoPagina).bottom - kZeroCinquePunti;
		return(lAltezzaPagina - lMargineAlto - lMargineBasso - gXtPreferences.documentoPref.altezzaTestata);
	} 
	else 
	{
		// non c'e' nessun documento aperto correntemente
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
		return(0);
	}
} // PrendiAltezzaColonna

/* ------------------------------------------------------------------------ *

	PrendiNumeroColonne

* ------------------------------------------------------------------------ */
int16 XTAPI PrendiNumeroColonne() throw()
{
	if (gCurDoc >= 0) 
	{
		// sono sicuro che e' aperto un documento
		return((*gPtrInfoPagina).colcount);
	} 
	else 
	{
		// non c'e' nessun documento aperto correntemente
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
		return(0);
	}
} // PrendiNumeroColonne

/* ------------------------------------------------------------------------ *

	PrendiDistanzaColonne

* ------------------------------------------------------------------------ */
Fixed XTAPI PrendiDistanzaColonne() throw()
{
	if (gCurDoc >= 0) 
	{
		// sono sicuro che e' aperto un documento
		return((*gPtrInfoPagina).gutter);
	} 
	else 
	{
		// non c'e' nessun documento aperto correntemente
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
		return(0);
	}
} // PrendiDistanzaColonne

/* ------------------------------------------------------------------------ *

	IsPagineAffiancate

* ------------------------------------------------------------------------ */
bool8 XTAPI IsPagineAffiancate() throw()
{
	if (gCurDoc >= 0) 
	{
		// sono sicuro che e' aperto un documento
		return(gDoubleSided);
	} 
	else 
	{
		// ritorno TRUE per bloccare l'impaginazione
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
		return(TRUE);
	}
} // IsPagineAffiancate

/* ------------------------------------------------------------------------ *

	ImpostaScalaDocumento

* ------------------------------------------------------------------------ */
void XTAPI ImpostaScalaDocumento(Fixed *percetuale) throw()
{
	Fixed docScale;
	Fixed lPercentuale = 0;
	
	if (gCurDoc >= 0) 
	{
		// sono sicuro che e' aperto un documento
		lPercentuale = (*percetuale);
		DocRef DRef = XTGetDocRefFromDocID(gCurDoc);
		XTGetDocScale(DRef, &docScale);
		(*percetuale) = docScale;
		docScale = lPercentuale;
		XTSetDocScale(DRef, docScale, 1, NULL);
	} 
	else 
	{
		// richiesta di impostare la scala del documento senza documento aperto
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
	}
} // ImpostaScalaDocumento

/* ------------------------------------------------------------------------ *

	ImpostaFinestraDocumento

* ------------------------------------------------------------------------ */
void XTAPI ImpostaFinestraDocumento(int16 *larghezza, int16 *altezza) throw()
{
	int16       lAltezza = 0;
	int16 	lLarghezza = 0;
	Rect 	lRect;

	if (gCurDoc >= 0) 
	{
		// sono sicuro che c'e' aperto un documento
		GrafPtr	docWPtr = NULL;
		xtget_docwptr(&docWPtr);
		HWND docHWND = GetPortHWND(docWPtr);	
		
		GetWindowRect(docHWND, (RECT FAR *)&lRect );

		lAltezza = (*altezza);
		lLarghezza = (*larghezza);
		(*larghezza) = lRect.right - lRect.left;
		(*altezza) = lRect.bottom - lRect.top;
		SetWindowPos(docHWND, HWND_TOP, 0,0,lLarghezza, lAltezza, SWP_NOMOVE);

		// update della finestra
		updatewnd((WindowPtr) docHWND);

	} 
	else 
	{
		// richiesta di impostare la dimensione della finestra del documento
		// senza documento aperto
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
	}
} // ImpostaFinestraDocumento

