/* ------------------------------------------------------------------------ *
	PreparazioneDocumento.cpp

	25 marzo 2005 - Fabrizio

	Copyright © Sinedita S.r.l 2005. All Rights Reserved.
	
	Descrizione:
	Contiene tutte le procedure per preparare il documento.

	$PreparazioneDocumento.cpp$

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

#include "Box.h"
#include "DocInfo.h"
#include "Errori.h"
#include "GesSlug.h"
#include "Pref.h"
#include "SpaceCol.h"
#include "XTNecro.h"

#include "FinaleWap.h"

#include "PreparaDocWap.h"
#include "PrepDoc.h"

// CONSTS
// CONSTS
// CONSTS

#define kMastroSingolaVuota 1

// TYPEDEF
// TYPEDEF
// TYPEDEF

typedef struct 
{
	int16 colonna;
	int16 moduli;
} infoingombri;

// STATICS GLOBALS
// STATICS GLOBALS
// STATICS GLOBALS

// per memorizzare il numero di moduli per ogni colonna
static infoingombri *gPtrInfoIngombri = NULL;

//// per il popup per scegliere il numero delle pagine da creare
//static MenuHandle gHndlMenuPopUpPagine = NULL;
//
//// per il popup per associare un mastro alle pagine
//static MenuHandle gHndlMenuPopUpPagineMastro = NULL;
//
//// per il popup per scegliere il numero delle colonne da creare
//static MenuHandle gHndlMenuPopUpColonne = NULL;
//
//// per il popup per scegliere il numero dei moduli da creare
//static MenuHandle gHndlMenuPopUpModuli = NULL;
//
//// per il popup per associare un mastro alle pagine
//static MenuHandle gHndlMenuPopUpNomiMastro = NULL;
//
//// per il popup per scegliere il numero delle colonne della prima pagina
//static MenuHandle gHndlMenuPopUpColonnaPrimaPagina = NULL;
//
//// per il popup per scegliere il numero dei moduli della prima pagina
//static MenuHandle gHndlMenuPopUpModuliPrimaPagina = NULL;

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	InserisciColonneDiIngombro

* ------------------------------------------------------------------------ */
void XTAPI InserisciColonneDiIngombro() throw()
{
	// indice per scorrere le colonne
	int16 lColonna = 0;
	// per prendere le info sugli ingombri
	infoingombri *lPtrInfoIngombri = NULL;
	
	for (lColonna = 0; lColonna < PrendiNumeroColonne(); lColonna++) 
	{
		// imposto il puntatore
		lPtrInfoIngombri = gPtrInfoIngombri + lColonna;

		if ((*lPtrInfoIngombri).moduli == 0) 
		{
			(*lPtrInfoIngombri).colonna = 0;
		} 
		else 
		{
			// inserisco l'ingombro
			(*lPtrInfoIngombri).colonna = lColonna + 1;
			InserisciColonnaDiIngombro(lColonna + 1, (*lPtrInfoIngombri).moduli);
		}
	}
	xtset_curbox(NULL);
} // InserisciColonneDiIngombro

/* ------------------------------------------------------------------------ *

	CancelloBoxIngombro

* ------------------------------------------------------------------------ */
void XTAPI CancelloBoxIngombro() throw()
{
	for (int16 sprd = 1; sprd <= numsprds();sprd++) 
	{
		boxid theBox = firstbox(sprd, TRUE);
		while(theBox != NULL)
		{
			if (hasslug(theBox, OBJECTSLUG, kSlugDiIngombro))
			{
				boxid boxToDelete = theBox;
				theBox = nextbox(boxToDelete, TRUE);
				deletebox(boxToDelete, TRUE);
			}
			else
				theBox = nextbox(theBox, TRUE);
		}
	}
	
} // CancelloBoxIngombro

/* ------------------------------------------------------------------------ *

	InserisciPagineColonneEModuli

* ------------------------------------------------------------------------ */
void XTAPI InserisciPagineColonneEModuli(int32 pagine, int32 colonne, int32 moduli, Boolean cancella) throw()
{
	// per sapere il risultato dell'inserimento delle pagine
	Boolean lRisultato = FALSE;
	// per scorrerele colonne
	int16 lColonna = 0;
	// per prendere le info sugli ingombri
	infoingombri *lPtrInfoIngombri = NULL;
	// numero delle colonne di ingombro
	int16 lNumeroColonneDiIngombro = 0;
	// numero dei moduli di ingombro
	int16 lNumeroModuliDiIngombro = 0;
	// per scorrerele le pagine
	int16 lPagina = 0;
	
	// controllo quante pagine devo inserire
	if (colonne != 0 || moduli != 0) 
		pagine++;

	// controllo se devo inserire o cancellare delle pagine
	if (pagine < numpages()) 
	{
		// devo cancellare delle pagine
		deletepages(pagine + 1, numpages(), 0);
	} 
	else if (pagine > numpages()) 
	{
		// devo aggiungere delle pagine
		 DocID curDocID = INVALDOC;
		 int16 numPages = -1;
		 DocRef DRef;
		 xtget_curdoc(&curDocID);
		 DRef = XTGetDocRefFromDocID(curDocID);
		 XTNumPages(DRef,FALSE,&numPages);
		lRisultato = XTInsertPages(DRef, numPages, pagine - numPages, APPLYDEFAULT+RUNDONE/*8*/, NULL, kPaginaMastroA);
		// lRisultato = insertpages2(numpages(), pagine - numpages(),  8, NULL, kPaginaMastroA);
		if (lRisultato != noErr) 
		{
			// visualizzo l'errore
			MostraErrore(kErroreInserimentoPagine);
			return;
		}
	}

	// cancello tutti i box di ingombro sulla prima pagina
	CancelloBoxIngombro();

	// vado sulla pagina dove creo gli ingombri
	if (gXtPreferences.finalePref.preparaDocumento == FinaleWap::PRIMAPAGINARADIOBTNID)
	{
		gotopage(kPrimaPagina);
	}
	else
	{
		gotopage(numpages());
	}

	if (colonne != 0 || moduli != 0) 
	{
		// calcolo quanti box di colonna inserire
		if (colonne != 0 || moduli != 0) 
		{
			lNumeroColonneDiIngombro = PrendiNumeroColonne() - colonne;
			if (moduli != 0) 
				lNumeroColonneDiIngombro--;
		}

		// calcolo quando deve essere alto il box su una colonna
		if (moduli != 0) 
		{
			lNumeroModuliDiIngombro = gXtPreferences.documentoPref.moduliColonna - moduli;
		}

		// calcolo i moduli per le colonne
		for (lColonna = 0; lColonna < PrendiNumeroColonne(); lColonna++) 
		{
			// controllo se inserisco in coda o in testa
			if (gXtPreferences.finalePref.preparaDocumento == FinaleWap::PRIMAPAGINARADIOBTNID)
			{
				// imposto il puntatore
				lPtrInfoIngombri = gPtrInfoIngombri + lColonna;
			}
			else
			{
				// imposto il puntatore
				lPtrInfoIngombri = gPtrInfoIngombri + (PrendiNumeroColonne() - lColonna - 1);
			}

			if (lColonna < lNumeroColonneDiIngombro)
				(*lPtrInfoIngombri).moduli = gXtPreferences.documentoPref.moduliColonna;
			else if (lColonna == lNumeroColonneDiIngombro)
				(*lPtrInfoIngombri).moduli = lNumeroModuliDiIngombro;
			else (*lPtrInfoIngombri).moduli = 0;
		}

		// inserisco i box di ingombro
		InserisciColonneDiIngombro();

	}

	// aggiorno le palette
	updatepagepalette();

	if (cancella == TRUE) 
	{
		// tolgo l'associazione alle pagine mastro
		for (lPagina = 2; lPagina <= numpages(); lPagina++) 
		{
			// applico la pagina mastro vuota
			applymaster2pages(kMastroSingolaVuota, lPagina, lPagina);
		}
	}
} // InserisciPagineColonneEModuli

/* ------------------------------------------------------------------------ *

	AssociaPaginaMastro
	
* ------------------------------------------------------------------------ */
void XTAPI AssociaPaginaMastro(int16 pagina, int16 mastro, Boolean compensa) throw()
{
	// per calcolare quanti moduli sone presenti sulla pagina
	int16 lNumeroModuliPagina = 0;
	// per calcolare quanti moduli sono presenti sulla pagina mastro
	int16 lNumeroModuliPaginaMastro = 0;
	// per calcolare quanti moduli devo inserire
	int16 lNumeroModuliDaAggiungere = 0;
	// per calcolare quanti moduli sono presenti sulla prima magina
	int16 lNumeroModuliPrimaPagina = 0;
	// posizione del mastro da applicare
	int16 lMastro = 0;
	// per scorrerele colonne
	int16 lColonna = 0;
	// per prendere le info sugli ingombri
	infoingombri *lPtrInfoIngombri = NULL;
	
	// alla prima pagina non posso cambiare il mastro
	 if (pagina == 1) 
	 {
	 	MostraErrore(kErroreAssociaMastro);
	 	return;
	 }
	
	// calcolo la posizione della pagina mastro
	lMastro = mastro + kPaginaMastroA - 1;
	
	// controllo se devo compensare
	if (compensa == TRUE) 
	{
		// prendo l'ingombro della pagina a cui applicare la pagina mastro
		PrendiModuliPagina(pagina, &lNumeroModuliPagina);

		// ricerca dei box presenti sulla pagina mastro
		PrendiModuliPaginaMastro(lMastro, &lNumeroModuliPaginaMastro);
		lNumeroModuliDaAggiungere = lNumeroModuliPagina - lNumeroModuliPaginaMastro;

		// determinare quanti moduli sono presenti sulla prima pagina 
		for (lColonna = 0; lColonna < PrendiNumeroColonne(); lColonna++) 
		{
			// imposto il puntatore
			lPtrInfoIngombri = gPtrInfoIngombri + lColonna;
			lNumeroModuliPrimaPagina += (*lPtrInfoIngombri).moduli;
		}

		// controllo che sia possibile compensare
		if ((lNumeroModuliDaAggiungere + lNumeroModuliPrimaPagina) >
			(PrendiNumeroColonne() * gXtPreferences.documentoPref.moduliColonna)) 
		{
			// visualizzo l'errore
			MostraErrore(kImpossibileCompensare);
			return;
		}

		if (lNumeroModuliDaAggiungere < 0 &&
			(lNumeroModuliDaAggiungere * -1) > lNumeroModuliPrimaPagina) 
		{
			// non e' possibile compensare

			// visualizzo l'errore
			MostraErrore(kImpossibileCompensare);
			return;
		}

		// modifico l'ingombro della prima pagina
		ModificoIngombroPrimaPagina(lNumeroModuliDaAggiungere);
	}

	// applico la pagina mastro 
	applymaster2pages(lMastro, pagina, pagina);

	// aggiorno le palette
	updatepagepalette();
} // AssociaPaginaMastro

/* ------------------------------------------------------------------------ *

	ModificoIngombroPrimaPagina

* ------------------------------------------------------------------------ */
void XTAPI ModificoIngombroPrimaPagina(int16 moduli) throw()
{
	// per scorrerele colonne
	int16 lColonna = 0;
	// per prendere le info sugli ingombri
	infoingombri *lPtrInfoIngombri = NULL;
	// contiene i moduli per la colonna
	int16 lModuliPerColonna = 0;
	// moduli da compensare
	int16 lModuli = 0;
	
	// non devo fare alcuna modifica
	if (moduli == 0) 
		return;
	lModuli = moduli;
	
	// cancello tutti i box di ingombro sulla prima pagina
	CancelloBoxIngombro();
	
	if (lModuli < 0) 
	{
		// devo compensare restringendo
		for (lColonna = PrendiNumeroColonne() - 1; lColonna >= 0; lColonna--) 
		{
			// imposto il puntatore
			lPtrInfoIngombri = gPtrInfoIngombri + lColonna;
			
			lModuliPerColonna = (*lPtrInfoIngombri).moduli;
			if (lModuliPerColonna == 0) continue;
			
			lModuliPerColonna += lModuli;
			if (lModuliPerColonna < 0) 
			{
				(*lPtrInfoIngombri).moduli = 0;
				lModuli = lModuliPerColonna;
			} 
			else 
			{
				(*lPtrInfoIngombri).moduli = lModuliPerColonna;
				break;
			}
		}
	} 
	else 
	{
		// devo compensare allargando
		for (lColonna = 0; lColonna < PrendiNumeroColonne(); lColonna++) 
		{
			// imposto il puntatore
			lPtrInfoIngombri = gPtrInfoIngombri + lColonna;

			lModuliPerColonna = (*lPtrInfoIngombri).moduli;
			if (lModuliPerColonna == gXtPreferences.documentoPref.moduliColonna) continue;

			lModuliPerColonna += lModuli;
			if (lModuliPerColonna > gXtPreferences.documentoPref.moduliColonna) 
			{
				(*lPtrInfoIngombri).moduli = gXtPreferences.documentoPref.moduliColonna;
				lModuli = lModuliPerColonna - gXtPreferences.documentoPref.moduliColonna;
			} 
			else 
			{
				(*lPtrInfoIngombri).moduli = lModuliPerColonna;
				break;
			}
		}
	}

	// inserisco i box di ingombro
	InserisciColonneDiIngombro();
	
} // ModificoIngombroPrimaPagina

/* ------------------------------------------------------------------------ *

	ModificaPrimaPagina

* ------------------------------------------------------------------------ */
void XTAPI ModificaPrimaPagina(int16 colonna, int16 moduli) throw()
{
	// per prendere le info sugli ingombri
	infoingombri *lPtrInfoIngombri = NULL;
	// numero dei moduli di ingombro
	int16 lNumeroModuliDiIngombro = 0;

	// cancello tutti i box di ingombro sulla prima pagina
	CancelloBoxIngombro();

	// imposto il puntatore
	lPtrInfoIngombri = gPtrInfoIngombri + colonna - 1;

	// calcolo i moduli di ingombro
	lNumeroModuliDiIngombro = gXtPreferences.documentoPref.moduliColonna - moduli;

	// inserisco il nuovo valore della colonna
	(*lPtrInfoIngombri).moduli = lNumeroModuliDiIngombro;

	// inserisco i box di ingombro
	InserisciColonneDiIngombro();
} // ModificaPrimaPagina


/* ------------------------------------------------------------------------ *

	CercaBoxIngombro

* ------------------------------------------------------------------------ */
void XTAPI CercaBoxIngombro() throw()
{
	// indice per scorrere le colonne
	int16 lColonna = 0;
	// per prendere le info sugli ingombri
	infoingombri *lPtrInfoIngombri = NULL;
	// per prendere l'altezza del box di ingombro
	Fixed lAltezza = 0;
	
	for (lColonna = 0; lColonna < PrendiNumeroColonne(); lColonna++) 
	{
		// imposto il puntatore
		lPtrInfoIngombri = gPtrInfoIngombri + lColonna;
		
		// cerco il box di ingombro 
		gErroreXtension = TrovaSlugDiIngombro(lColonna + 1);
		if (gErroreXtension == kNessunErrore)
		{
			(*lPtrInfoIngombri).colonna = lColonna + 1;

			// prendo l'altezza del box corrente
			lAltezza = PrendiAltezzaBox();
			
			// controllo l'altezza del box
			if (lAltezza == PrendiAltezzaColonna())
			{
				// e' occupata tutta la colonna
				(*lPtrInfoIngombri).moduli = gXtPreferences.documentoPref.moduliColonna;
			}
			else
			{
				// calcolo i moduli occupati
				assert (0 != (gXtPreferences.documentoPref.altezzaModulo + gXtPreferences.documentoPref.distanzaModuli));

				(*lPtrInfoIngombri).moduli = lAltezza /	  (gXtPreferences.documentoPref.altezzaModulo + gXtPreferences.documentoPref.distanzaModuli);
			}
		}
		else if (gErroreXtension == kErroreRicercaIngombro)
		{
			(*lPtrInfoIngombri).colonna = 0;
			(*lPtrInfoIngombri).moduli = 0;
		}
		else
		{
			DaiErrore(gErroreXtension);
			return;
		}
	}
	xtset_curbox(NULL);
} // CercaBoxIngombro

/* ------------------------------------------------------------------------ *

	PreparaDocumento

* ------------------------------------------------------------------------ */
void XTAPI PreparaDocumento() throw()
{
	// creo la struttura per memorizzare il numero di moduli per ogni colonna 
	if (gPtrInfoIngombri != NULL) 
	{
		DisposePtr((Ptr)gPtrInfoIngombri);
		gPtrInfoIngombri = NULL;
	}

	gPtrInfoIngombri = (infoingombri *) NewPtrClear(sizeof(infoingombri) *
												    PrendiNumeroColonne());
	if (gPtrInfoIngombri == NULL) 
	{
		MostraErrore(kErroreMemoria);
		return;
	}

	// controllo se preparare in coda o in testa
	if (gXtPreferences.finalePref.preparaDocumento == FinaleWap::PRIMAPAGINARADIOBTNID)
	{
		// vado sulla prima pagina
		gotopage(kPrimaPagina);
	}
	else
	{
		// vado sull'ultima pagina
		gotopage(numpages());
	}

	CercaBoxIngombro();

	//dodlgboxparam( hinst, hwndmainframe, (FARPROC)PreparaDocumentoWAP, DIALOG_20150, NULL );
	//xd_createdialog(_XT_PREPARADOCWAP, 0, NULL);
	XDCreateDialogWithCBCode(_XT_PREPARADOCWAP, (void *)(new PreparaDocWap()), NULL);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

/* Costanti */
/* Costanti */
/* Costanti */

//#define rPopUpPagine 21000
//#define rPopUpColonne 21010
//#define rPopUpModuli 21020
/* risorsa di men— per creare i popup del prepara documento */

//#define rPopUpPagineMastro 21030
//#define rPopUpNomiMastro 21040
/* risorsa di men— per creare i popup delle pagine mastro */

//#define rPopUpColonnaPrimaPagina 21050
//#define rPopUpModuliPrimaPagina 21060
/* risorsa di men— per creare i popup delle pagine mastro */


/* item della finestra */

/*
#define iPopUpPagine 4
#define iPopUpColonne 6
#define iPopUpModuli 8
#define iCheckCancella 9
#define iBottonePrepara 10

#define iPopUpPagineMastro 14
#define iPopUpNomiMastro 16
#define iCheckCompensa 17
#define iBottoneAssocia 18

#define iPopUpColonnaPrimaPagina 22
#define iPopUpModuliPrimaPagina 24
#define iBottoneApplica 25

#define kMastroSingolaVuota 1
*/
#define rFinestraPreparaDocumento 20150
/* identificatore della finestra per preparare il documento */

#define iPopUpPagine IDC_20150_COMBOBOX4
#define iPopUpColonne IDC_20150_COMBOBOX6
#define iPopUpModuli IDC_20150_COMBOBOX8
#define iCheckCancella IDC_CHECKBOX9
#define iBottonePrepara IDC_20150_PUSHBUTTON10

#define iPopUpPagineMastro IDC_20150_COMBOBOX14
#define iPopUpNomiMastro IDC_20150_COMBOBOX16
#define iCheckCompensa IDC_CHECKBOX17
#define iBottoneAssocia IDC_20150_PUSHBUTTON18

#define iPopUpColonnaPrimaPagina IDC_20150_COMBOBOX22
#define iPopUpModuliPrimaPagina IDC_20150_COMBOBOX24
#define iBottoneApplica IDC_20150_PUSHBUTTON25



/* per applicare la pagina mastro vuota */


/* Variabili */
/* Variabili */
/* Variabili */

static Point gPosizioneFinestraPreparaDocumento = {32767,32767};
/* posizione della finestra */

//static int gIdPopUpPagine = 0;
//static int gIdPopUpColonne = 0;
//static int gIdPopUpModuli = 0;
/* da usare con l'allochierid() per la creazione del popup */

//static int gIdPopUpPagineMastro = 0;
//static int gIdPopUpNomiMastro = 0;
/* da usare con l'allochierid() per la creazione del popup */

//static int gIdPopUpColonnaPrimaPagina = 0;
//static int gIdPopUpModuliPrimaPagina = 0;
/* da usare con l'allochierid() per la creazione del popup */


//static popupinforec *gPtrPopUpPagine = NULL;
/* sempre per il popup delle pagine */
static popupinforec *gPtrPopUpColonne = NULL;
/* sempre per il popup delle colonne */
//static popupinforec *gPtrPopUpModuli = NULL;
/* sempre per il popup dei moduli */


//static popupinforec *gPtrPopUpPagineMastro = NULL;
/* sempre per il popup per associare un mastro alle pagine */
//static popupinforec *gPtrPopUpNomiMastro = NULL;
/* sempre per il popup per associare un mastro alle pagine */

static popupinforec *gPtrPopUpColonnaPrimaPagina = NULL;
/* sempre per il popup delle colonne della prima pagina */
//static popupinforec *gPtrPopUpModuliPrimaPagina = NULL;
/* sempre per il popup dei moduli della prima pagina */


BOOL CALLBACK PreparaDocumentoWAP( HWND, WORD, PARAM1, PARAM2);
BOOL CALLBACK PreparaDocumentoWAP( HWND thewptr, WORD message, PARAM1 param1, PARAM2 param2)
/* ------------------------------------------------------------------------ *

	PreparaDocumentoWAP

	6 Marzo 1996						Stefano

	Descrizione:
	WAP di gestione della finestra per prepara documento

	Parametri:
	i paramentri di entrata arrivano da Xpress

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	int lRitorno = FALSE;
	/* valore di ritorno della WAP */
	short lBottoneScelto = 0;
	/* per prendere l'identificatore del botto scelto sulla finestra di preferenze */
	BOOL result = TRUE;
 	static int16 inhere = 0;
	int				i;

	switch (message) {
		case WOPEN:
			frontmost = thewptr;
			/* preparo i popup della prepara documento */
			ImpostaPreparaDocumento();

			/* preparo i popup delle pagine mastro */
			ImpostaPagineMastro(frontmost);

			/* preparo i popup della prima pagina */
			ImpostaPrimaPagina(frontmost);
			result = FALSE;
			center_window(thewptr);
			break;

		case WM_COMMAND:
			switch (param1) {

				case iPopUpPagine:
				if ((HIWORD(param2) == CBN_SETFOCUS) && (!inhere))
				{
					AggiornaPagine(param2);
				}
				if ((HIWORD(param2) == CBN_SELCHANGE) && (!inhere)) {
					inhere=1;
					i = SendMessage(LOWORD(param2),CB_GETCURSEL,0,0L);
					if (i == CB_ERR) i = 0;
					SendMessage(LOWORD(param2), CB_GETLBTEXT,i,(LONG)(LPSTR)tmpStr);
					SetWindowText(LOWORD(param2),tmpStr);
					inhere=0;

/*					dopopup((DialogPeek) thewptr,
							iPopUpPagine - 1,
							iPopUpPagine);
*/
					}
					break;

				case iPopUpColonne:
				case PAGINEPREPDOCPOPID:
				{
				/*
					if ((HIWORD(params->param2) == CBN_SELCHANGE) && (!inhere)) 
					{
						inhere=1;
						int32 i;
						i = xd_lst_getselection(PAGINEPREPDOCPOPID, &i);
						if (i == CB_ERR) 
							i = 0;
						xd_lst_getrowstring(PAGINEPREPDOCPOPID, i, tmpStr);
						// SetWindowText(LOWORD(params->param2),tmpStr);
						xd_lst_setrowtext(PAGINEPREPDOCPOPID, i, tmpStr);
						inhere=0;
					}
				*/	
					break;
				}
				if ((HIWORD(param2) == CBN_SELCHANGE) && (!inhere)) {
						inhere=1;
						i = SendMessage(LOWORD(param2),CB_GETCURSEL,0,0L);
						if (i == CB_ERR) i = 0;
						SendMessage(LOWORD(param2), CB_GETLBTEXT,i,(LONG)(LPSTR)tmpStr);
						SetWindowText(LOWORD(param2),tmpStr);
						inhere=0;
					}
					break;

				case iPopUpModuli:
				if ((HIWORD(param2) == CBN_SELCHANGE) && (!inhere)) {
						inhere=1;
						i = SendMessage(LOWORD(param2),CB_GETCURSEL,0,0L);
						if (i == CB_ERR) i = 0;
						SendMessage(LOWORD(param2), CB_GETLBTEXT,i,(LONG)(LPSTR)tmpStr);
						SetWindowText(LOWORD(param2),tmpStr);
						inhere=0;
					}
					break;

				case iCheckCancella:
					gCheckCancella = setcheck((DialogPtr) thewptr,
												iCheckCancella,
												!gCheckCancella);
					break;

				case iBottonePrepara:
					// costruisco il documento
					InserisciPagineColonneEModuli(SendDlgItemMessage((DialogPeek)thewptr,iPopUpPagine,CB_GETCURSEL,0,0L)/*+1*/,
												  SendDlgItemMessage((DialogPeek)thewptr,iPopUpColonne,CB_GETCURSEL,0,0L),
												  SendDlgItemMessage((DialogPeek)thewptr,iPopUpModuli,CB_GETCURSEL,0,0L),
												  gCheckCancella);

					break;

				case iPopUpPagineMastro:
					if ((HIWORD(param2) == CBN_SETFOCUS) && (!inhere))
					{
						AggiornaPagineMastro(param2);
					}
					if ((HIWORD(param2) == CBN_SELCHANGE) && (!inhere)) {
						inhere=1;
						i = SendMessage(LOWORD(param2),CB_GETCURSEL,0,0L);
						if (i == CB_ERR) i = 0;
						else
						{
							SendMessage(LOWORD(param2), CB_GETLBTEXT,i,(LONG)(LPSTR)tmpStr);
							SetWindowText(LOWORD(param2),tmpStr);
/*					dopopup((DialogPeek) thewptr,
							 iPopUpPagineMastro - 1,
							 iPopUpPagineMastro);
*/
							if (i == 0)
								setditemenable((DialogPeek) thewptr, iBottoneAssocia, FALSE);
							else setditemenable((DialogPeek) thewptr, iBottoneAssocia, TRUE);
							inhere=0;
						}
					}
					break;

				case iPopUpNomiMastro:
				if ((HIWORD(param2) == CBN_SELCHANGE) && (!inhere)) {
						inhere=1;
						i = SendMessage(LOWORD(param2),CB_GETCURSEL,0,0L);
						if (i == CB_ERR) i = 0;
						SendMessage(LOWORD(param2), CB_GETLBTEXT,i,(LONG)(LPSTR)tmpStr);
						SetWindowText(LOWORD(param2),tmpStr);
						inhere=0;
					}
					break;

				case iCheckCompensa:
					gCheckCompensa = setcheck((DialogPtr) thewptr,
												iCheckCompensa,
												!gCheckCompensa);
					break;

				case iBottoneAssocia:
					/* associo la pagina mastro selezionata */
					AssociaPaginaMastro(SendDlgItemMessage((DialogPeek)thewptr,iPopUpPagineMastro,CB_GETCURSEL,0,0L) + 1,
										SendDlgItemMessage((DialogPeek)thewptr,iPopUpNomiMastro,CB_GETCURSEL,0,0L) + 1,
										gCheckCompensa);

					break;

				case iPopUpColonnaPrimaPagina:
				if ((HIWORD(param2) == CBN_SELCHANGE) && (!inhere)) {
						inhere=1;
						i = SendMessage(LOWORD(param2),CB_GETCURSEL,0,0L);
						if (i == CB_ERR) i = 0;
						SendMessage(LOWORD(param2), CB_GETLBTEXT,i,(LONG)(LPSTR)tmpStr);
						SetWindowText(LOWORD(param2),tmpStr);
						inhere=0;
					}
					break;

				case iPopUpModuliPrimaPagina:
				if ((HIWORD(param2) == CBN_SELCHANGE) && (!inhere)) {
						inhere=1;
						i = SendMessage(LOWORD(param2),CB_GETCURSEL,0,0L);
						if (i == CB_ERR) i = 0;
						SendMessage(LOWORD(param2), CB_GETLBTEXT,i,(LONG)(LPSTR)tmpStr);
						SetWindowText(LOWORD(param2),tmpStr);
						inhere=0;
					}
					break;
					
				case iBottoneApplica:
					ModificaPrimaPagina(SendDlgItemMessage((DialogPeek)thewptr,iPopUpColonnaPrimaPagina,CB_GETCURSEL,0,0L)+1,
										SendDlgItemMessage((DialogPeek)thewptr,iPopUpModuliPrimaPagina,CB_GETCURSEL,0,0L));
					break;

				case IDOK:

					/* prendo la posizione della finestra */
//					gPosizioneFinestraPreparaDocumento = topLeft(((WindowPtr) thewptr)->portRect);
//					LocalToGlobal(&gPosizioneFinestraPreparaDocumento);

					/* chiudo i popup */
					DestroyMenu(gHndlMenuPopUpPagine);
					DestroyMenu(gHndlMenuPopUpColonne);
					DestroyMenu(gHndlMenuPopUpModuli);

					DestroyMenu(gHndlMenuPopUpPagineMastro);
					DestroyMenu(gHndlMenuPopUpNomiMastro);

					DestroyMenu(gHndlMenuPopUpColonnaPrimaPagina);
					DestroyMenu(gHndlMenuPopUpModuliPrimaPagina);

//					restorewnds((WindowPeek) thewptr,TRUE);

					EndDialog(thewptr, NULL);
					break;

		case IDCANCEL:
				EndDialog(thewptr, NULL);
				break;
			}
			break;

		default:
			result = FALSE;
	}

	return(result);
}

#if kOLD

void AlloccaPopUpPreparaDocumento(void)
/* ------------------------------------------------------------------------ *

	AlloccaPopUpPreparaDocumento

	6 Marzo 1996						Stefano

	Descrizione:
	Per alloccare gli identificatori dei popup menu della prepara documento

	Parametri:
	nessuno

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	/* prepara documento */
	gIdPopUpPagine = allochierid();
	gIdPopUpColonne = allochierid();
	gIdPopUpModuli = allochierid();
	
	/* pagine mastro */
	gIdPopUpPagineMastro = allochierid();
	gIdPopUpNomiMastro = allochierid();

	/* prima pagina */
	gIdPopUpColonnaPrimaPagina = allochierid();
	gIdPopUpModuliPrimaPagina = allochierid();
}

#endif /* kOLD */

#endif // 0 NECRO v1

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/