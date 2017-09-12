/* ------------------------------------------------------------------------ *

	Paletta.cpp

	Copyright © Sinedita S.r.l 2005. All Rights Reserved.

	6 Aprile 2005			Fabrizio

	$Log: not supported by cvs2svn $
	Revision 1.2  2005/05/02 10:03:14  taretto
	avanzamento lavori porting
	
	Revision 1.1  2005/04/20 07:14:20  taretto
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
#include "DbpResource.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "Memoria.h"
#include "Errori.h"
#include "ResStr.h"
#include "XTNecro.h"

#include "InfoPaletteWap.h"
#include "Paletta.h"

// GLOBALS
// GLOBALS
// GLOBALS

bool8 gInfoPaletteShowingFlag = FALSE;
wlochandle gInfoPaletteWLocHandle = NULL;
xdlgid gInfoPaletteId = NULL;
std::vector<std::string> gListaPalette;

// STATICS
// STATICS
// STATICS

//per ricordare ultima percentuale ottenuta
static int16 gUltimaPercentuale = 0;

// per la stringa sulla progress bar
static uchar gAttualeImpaginazione[256] = "";

// per contenere percentuale barra avanzamento
static int16 gPercentuale = 0;

// numero di intervalli della barra di avanzamento
static const xdtime kMsDelay = 100;

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	Visualizza
	
* ------------------------------------------------------------------------ */
void XTAPI Visualizza(const uchar* iString) throw()
{
	assert(NULL != iString);
	assert(0 != iString[0]);

	// copio stringa all'interno della lista
	std::string tmpString((char*) iString);
	gListaPalette.push_back(tmpString); 
	
	if (gInfoPaletteShowingFlag)
	{		
		// prova per correggere problemino su palette bianche
		GrafPtr tmpGrafPtr;
		GetPort(&tmpGrafPtr);
		int16 savedResFile = CurResFile();
		SetPort(InfoPaletteWap::GetInstance().GetMessagePaletteGrafPtr());		
		
		APIERR Err = xd_sendmessage
		(
			gInfoPaletteId,
			XDM_CONTROL,
			InfoPaletteWap::GetInstance().GetListaInfoId(), 
			NULL,
			NULL,
			NULL
		);
		
		UseResFile(savedResFile);
		SetPort(tmpGrafPtr);
	}
	
} // Visualizza


/* ------------------------------------------------------------------------ *

	MostraNascondiPaletteInfo
	
* ------------------------------------------------------------------------ */
void XTAPI MostraNascondiPaletteInfo() throw()
{
	if (gInfoPaletteShowingFlag)
	{
		assert(NULL != gInfoPaletteId);
	
		if (noErr == xd_pushdlg(gInfoPaletteId)) 
		{
			xd_close(CLOSE_CANCEL, 0, NULL);
			xd_popdlg();
		}
	}
	else
	{
		//xd_createpalette(_XT_INFOPALETTEWAP, (uint32) gInfoPaletteWLocHandle, &gInfoPaletteId);
		XDCreatePaletteWithCBCode(_XT_INFOPALETTEWAP, (void*) new InfoPaletteWap(), &gInfoPaletteId);
	}

} // MostraNascondiPaletteInfo

/* ------------------------------------------------------------------------ *

	OpenInfoPalette
	
* ------------------------------------------------------------------------ */
void XTAPI OpenInfoPalette() throw()
{
	int32 result = noErr;

	if (gInfoPaletteShowingFlag)
	{
		assert(NULL != gInfoPaletteId);
		
		if (noErr == xd_pushdlg(gInfoPaletteId)) 
		{
			xd_close(CLOSE_CANCEL, 0, NULL);
			xd_popdlg();
		}
	}

	//xd_createpalette(_XT_INFOPALETTEWAP, (uint32) gInfoPaletteWLocHandle, &gInfoPaletteId);
	XDCreatePaletteWithCBCode(_XT_INFOPALETTEWAP, (void*) new InfoPaletteWap(), &gInfoPaletteId);
	
} // OpenPaginationPalette

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

/* Costanti */
/* Costanti */
/* Costanti */

//#define kTipoPosizionePaletta 'WLOC'
/* tipo della risorsa che memorizza la posizione della paletta */
#define rWindowPaletta 30000
/* identificatore di risorsa della paletta dell'Xtension */
#define iListaPaletta 5
/* identificatore della lista della paletta */

#ifdef OPERATION
#define kDimensioneLista 50
/* dimensione della lista dei messaggi della paletta */
#define kDimensioneRigaPaletta 128
/* dimensione della riga della lista della paletta */

#define kDimensioneScrollBar 15

/* Descrittori */
/* Descrittori */
/* Descrittori */

typedef struct {
	Boolean isVedibile;
	Byte reserved;
	Point posizione;
} posizionepaletta;

typedef struct {
	char *ptrStringheLista[kDimensioneLista];
	int identificatorePaletta;
	ListHandle hndlLista;
	posizionepaletta **hndlPosizionePaletta;
	WindowPtr ptrPaletta;
} paletta;


static paletta gPalettaXtension;
/* tutte le info della paletrta delle necrologie */
static Boolean gIsPosizionamento = FALSE;
/* per sapere se visualizzando la stringa del posizionamento */

static char gStrStoLavorando[4] = "-\\|/";
static int gIndiceStoLavorando = 0;

#endif
/* Variabili */
/* Variabili */
/* Variabili */




FARPROC wapinst;
HWND waphndl;
bool16 showingflag = FALSE;
HWND 	 		 WAPPtr;
wlochandle wlochndl;

#ifdef OPERATION
static void CreaListaPaletta(void);
static void CreaListaPaletta(void)

/* ------------------------------------------------------------------------ *

	CreaListaPaletta

	14 Novembre 1995						Stefano

	Descrizione:
	Crea la lista e i suoi elementi si per la fase di inizializzazione che
	per quella di aggiornamento

	Parametri:
	nessuno
	
	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */

{
	int i = 0;
	/* indice per i for */
	short lTipoLista = 0;
	/* riceve il tipo dell'elemento scelto, cioŠ della lista  */
	ControlHandle 	lHndlLista = NULL;
	/* riceve l'handle alla lista */
	Rect lCoordinateLista = {0,0,0,0};
	/* riceve le coordinate locali dell'elemento scelto, cioŠ della lista */
	Rect lDimensioniLista = {0,0,0,0};
	/* per definire le dimensioni della lista */
	Point lDimensioneElemento = {0,0};
	/* per definire la dimensione di ogni elemento della lista */
	Point lElemento = {0,0};
	/* per scorrere la lista */
   	FontInfo lFontInfo = {0,0,0,0};
   	/* per prendere le informazioni riguardo il font della dialog */
   	Size lLunghezza = 0;
   	/* per prendere la lunghezza della stringa da inserire nella lista */

	/* prendo la posizione dell'user item dove inserire la lista */
	GetDItem((DialogPtr) gPalettaXtension.ptrPaletta, iListaPaletta, &lTipoLista, 
			 (Handle *) &lHndlLista, &lCoordinateLista);
			 
	/* tolgo lo spazio per la scroll bar */
	lCoordinateLista.right -= kDimensioneScrollBar;
	/* prendo le info del font della paletta */
	GetFontInfo(&lFontInfo);
	/* calcolo la dimensione dell'elemento della lista */
	lDimensioneElemento.v = lFontInfo.ascent + lFontInfo.descent;
	lDimensioneElemento.h = lCoordinateLista.right - lCoordinateLista.left;

	/* diminuisco le coordinate di un punto */
	InsetRect(&lCoordinateLista, 1, 1);
	/* scelgo le dimentioni della lista, cio‚ una lista con una sola colonna */
	SetRect(&lDimensioniLista, 0, 0, 1, 0);   
	gPalettaXtension.hndlLista = LNew(&lCoordinateLista,
									  &lDimensioniLista,
									  lDimensioneElemento,
									  0,
									  (WindowPtr) gPalettaXtension.ptrPaletta,
							  		  FALSE,
							  		  FALSE,
							  		  FALSE,
							  		  TRUE);
	/* imposto le preferenze della lista per la selezione degli elementi */						  		  
	(**gPalettaXtension.hndlLista).selFlags = lNoNilHilite ||
											  lUseSense ||
											  lNoRect ||
											  lNoExtend ||
											  lNoDisjoint ||
											  lExtendDrag ||
											  lOnlyOne;

	/* inserisco gli elementi nella lista */
	lElemento.h = lElemento.v = 0;
	/* si parte da zero poich‚ la list Š stata creata con nessun elemento */
	for (i = 0; i < kDimensioneLista; i++) {
		lLunghezza = strlen(gPalettaXtension.ptrStringheLista[i]);
		LAddRow(1, lElemento.v, gPalettaXtension.hndlLista);
		LSetCell(gPalettaXtension.ptrStringheLista[i],
				 (short) lLunghezza,
				 lElemento,
				 gPalettaXtension.hndlLista);
		lElemento.v++;	
	}
	
	/* visualizzo l'ultimo elemento della lista */
	lElemento.h = 0; 
	lElemento.v = kDimensioneLista - 1;
	LScroll(-1000, -1000, gPalettaXtension.hndlLista);
	LScroll(lElemento.h, lElemento.v, gPalettaXtension.hndlLista); /* */
	LDoDraw(TRUE, gPalettaXtension.hndlLista);
}
#endif


void LoadPaletta(void)
{

	Rect box;

	wlochndl = loadpalettepos(0,box,0,appname);
			/* All we test here is whether or not we should be showing initially */
//	showingflag = TRUE;
	showingflag = (**wlochndl).a.locslug;

}



errorixtension InizializzaPaletta(void)
/* ------------------------------------------------------------------------ *

	InizializzaPaletta

	14 Novembre 1995						Stefano

	Descrizione:
	Inizializza le strutture della paletta dell'xtension

	Parametri:
	nessuno

	Ritorno:
	ritorna l'errore di xtension eventuale

* ------------------------------------------------------------------------ */

{

		if (showingflag) {
			wapinst = MakeProcInstance((FARPROC)WAPPaletta,hinst);
			waphndl = CreateFPaletteParam(hinst, "DIALOG_30000", hwndmainframe,
					(FARPROC)wapinst, (DWORD)NULL);
			registerfpalette(waphndl);
		}

#ifdef OPERATION


	int	i = 0;
	/* indice per il for */
	
	/* leggo la posizione della paletta dalle risorse */
	gPalettaXtension.hndlPosizionePaletta = (posizionepaletta **)
											GetResource(kTipoPosizionePaletta,
														rWindowPaletta);
	if (gPalettaXtension.hndlPosizionePaletta == NULL) {
		/* creo l'ahndle che contiene la posizione della paletta */
		/* crea l'handle per contenere le preferenze */
		gErroreXtension = AlloccaHandle((Handle *) &(gPalettaXtension.hndlPosizionePaletta),
										 sizeof(posizionepaletta));
		if (gErroreXtension != kNessunErrore) {
			/* errore sulla allocazione della memoria */
			StampaDebug("AlloccaHandle", (long) gErroreXtension);
			return(gErroreXtension);
		}

		/* inserisco i valori di default */
		(**gPalettaXtension.hndlPosizionePaletta).isVedibile = FALSE;
		(**gPalettaXtension.hndlPosizionePaletta).reserved = 0;
		(**gPalettaXtension.hndlPosizionePaletta).posizione.h = 300;
		(**gPalettaXtension.hndlPosizionePaletta).posizione.v = 300;
		AddResource((Handle) gPalettaXtension.hndlPosizionePaletta,
					kTipoPosizionePaletta,
					rWindowPaletta,
					"Paletta Xtension");
	} else HLock((Handle) gPalettaXtension.hndlPosizionePaletta);

	/* Controllo la posizione della paletta */
	if (PtInRgn((*gPalettaXtension.hndlPosizionePaletta)->posizione,
				GetGrayRgn()) == FALSE) {
		/* la posizione della paletta non Š visibile sullo schermo */
		(**gPalettaXtension.hndlPosizionePaletta).posizione.h = 72;
		(**gPalettaXtension.hndlPosizionePaletta).posizione.v = 72;
	}

	/* inizializzazione del record globale associato alla palette di debug */
	for (i = 0; i < kDimensioneLista; i++) {
		gPalettaXtension.ptrStringheLista[i] = NULL;
		gPalettaXtension.ptrStringheLista[i] = (char *) NewPtrClear(kDimensioneRigaPaletta);
		if (gPalettaXtension.ptrStringheLista[i] == NULL) {
			/* errore nell'allocazione della memoria */
			StampaDebug("NewPtrClear", 0);
			return(kErroreMemoria);
		}
	}
	gPalettaXtension.identificatorePaletta = -1;
	gPalettaXtension.hndlLista = NULL;
	gPalettaXtension.ptrPaletta = (WindowPtr)(-1L);
	(**gPalettaXtension.hndlPosizionePaletta).isVedibile = FALSE;
#endif
}

void ChiudiPaletta(void)
/* ------------------------------------------------------------------------ *

	ChiudiPaletta

	14 Novembre 1995						Stefano

	Descrizione:
	Chiudi le strutture della paletta dell'xtension

	Parametri:
	nessuno

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */

{

  Rect box;

  wlochndl = loadpalettepos(0,box,0,appname);
  /* All we save here is the showing flag. */
  (**wlochndl).a.locslug = showingflag;
  savepalettepos(wlochndl, 0);

#ifdef OPERATION

	int i = 0;
	/* indice per il for */

	/* salva la posizione della paletta dell'xtension */
	if (gPalettaXtension.hndlPosizionePaletta != NULL) {
		ChangedResource((Handle) gPalettaXtension.hndlPosizionePaletta);
		WriteResource((Handle) gPalettaXtension.hndlPosizionePaletta);
		HUnlock((Handle) gPalettaXtension.hndlPosizionePaletta);
		ReleaseResource((Handle) gPalettaXtension.hndlPosizionePaletta);
	}
	
	/* libero i puntatori della lista della paletta */
	for (i = 0; i < kDimensioneLista; i++) {
		if (gPalettaXtension.ptrStringheLista[i] != NULL) {
			DisposPtr((Ptr) gPalettaXtension.ptrStringheLista[i]);
			gPalettaXtension.ptrStringheLista[i] = NULL;
		}
	}

	DealloccaHandle((Handle *) &gPalettaXtension.hndlLista);
#endif


}




/****************************************************************************/
/*																			*/
/*  WAPPaletta	  		                       									*/
/*																			*/
/*  floating palette WAP per l'impaginazione.								*/
/*																			*/
/*  Entry																	*/
/*		--opcode, window pointer and a parameter.							*/
/*																			*/
/*  Exit:  None.															*/
/****************************************************************************/

bool16 CALLBACK WAPPaletta(
	HWND thewptr,
	int16 message,
	PARAM1 param1,
	PARAM2 param2)
{
	bool16 result = FALSE;
	Rect 			box;
	RECT 			r;
	int				itemHit;		/* elemento della dialog su cui si  fatto un click */
 //	Cell			theCell;		/* per catturare l'eventuale elemento della */
									/* lista scelto */
	Str255			tempStr;		/* stringa di comodo */
	static int		lastSelect = -1;
									/* ultima selezione sulla list degli errori */

	switch (message) {
		case WM_INITDIALOG:
			GetWindowRect(thewptr,&box);
			wlochndl = loadpalettepos(0,box,0,appname);
			SetWindowPos(thewptr,NULL,(*wlochndl)->wloc.h,(*wlochndl)->wloc.v,
				0,0, SWP_NOZORDER|SWP_NOSIZE);

			showingflag = TRUE;
			WAPPtr = thewptr;
//			SendDlgItemMessage((DialogPeek)WAPPtr, ID_300000_LISTBOX, LB_RESETCONTENT, 0, 0L);

			break;

		case WM_COMMAND:
	case WM_DESTROY:
		savepalettepos(wlochndl, 0);
		wlochndl = NULL;
		break;

/* This will come the close box control. */
	case WM_CLOSE:
		unregisterfpalette(waphndl);
		DestroyWindow(waphndl);
		FreeProcInstance(wapinst);
		showingflag = FALSE;
		result=TRUE;
		break;

	case WM_MOVE:
		{
			GetWindowRect(thewptr,&r);
			wlochndl = loadpalettepos(0,box,0,appname);
			(*wlochndl)->wloc.h = r.left;
			(*wlochndl)->wloc.v = r.top;
		}
		break;

	}

	return (result);
}

/****************************************************************************/
/*																			*/
/*  PageClearList                                                      		*/
/*																			*/
/*  Cancella i messaggi dalla list della palette di impaginazione.         	*/
/*																			*/
/*  Entry: None.															*/
/*																			*/
/*  Exit:  None.															*/
/*																			*/
/*  Global Variables														*/
/*		--Str255 PageString[PAGE_DIM]: array delle strignhe visualizzate	*/
/*			nella lista della palette di impaginazione;						*/
/*		--myboxid PageBoxId[PAGE_DIM]: array per memorizzare le 			*/
/*			agli errori di importazione delle immagini in box;				*/
/*		--WindowPeek PageWPtr: puntatore alla palette di impaginazione;		*/
/*		--wlochandle PageWLocHndl: handle alle caratteristiche della 		*/
/*			palette di impaginazione;										*/
/*		--ListHandle PageList: handle alla lista della palette di 			*/
/*			impaginazione;													*/
/*		--int MyPageId: identificatore della palette di impaginazione.		*/
/*																			*/
/****************************************************************************/

void PageClearList(void)

{
	GrafPtr			oldPort;
	WindowPeek	   	theWPtr;

	Point			theCell;
	FontInfo		dFont;
	int				i;
	Str255			tempStr;
	short			len;

	SendDlgItemMessage((DialogPeek)WAPPtr, ID_300000_LISTBOX, LB_RESETCONTENT, 0, 0L);

}
/****************************************************************************/
/*																			*/
/*  PagePrintf	                                                      		*/
/*																			*/
/*  Printf sulla palette di impaginazione dei messaggi generici.           	*/
/*																			*/
/*  Entry:																	*/
/*		-str: messaggio da visualizzare.      								*/
/*																			*/
/*  Exit:  None.															*/
/*																			*/
/*  Global Variables														*/
/*		--Str255 PageString[PAGE_DIM]: array delle strignhe visualizzate	*/
/*			nella lista della palette di impaginazione;						*/
/*		--myboxid PageBoxId[PAGE_DIM]: array per memorizzare le 			*/
/*			agli errori di importazione delle immagini in box;				*/
/*		--WindowPeek WAPPtr: puntatore alla palette di impaginazione;		*/
/*		--wlochandle PageWLocHndl: handle alle caratteristiche della 		*/
/*			palette di impaginazione;										*/
/*		--ListHandle PageList: handle alla lista della palette di 			*/
/*			impaginazione;													*/
/*		--int MyPageId: identificatore della palette di impaginazione.		*/
/*																			*/
/****************************************************************************/

void Visualizza(char *str)

{
	WindowPeek	   	theWPtr;
	int				i;
	Str255			tempStr;
	short			len;

	Str255			theStr;
	/* controlla se la palette  visibile e definita ed eventualmente ci scrive sopra */
	if (WAPPtr && showingflag) {
		SendDlgItemMessage((DialogPeek)WAPPtr, 5, LB_ADDSTRING, 0, (LPSTR)str);
		SendDlgItemMessage( (DialogPeek)WAPPtr, 5, LB_SETCURSEL,
		(DWORD)(SendDlgItemMessage((DialogPeek)WAPPtr, 5, LB_GETCOUNT, 0, (LPSTR)NULL)-1), (LPSTR)NULL );
		UpdateWindow(GetDlgItem(WAPPtr, 5));
	}
}



#ifdef OPERATION
int WAPPaletta(int opcode, WindowPtr thewptr, long param1)
/* ------------------------------------------------------------------------ *

	WAPPaletta

	14 Novembre 1995						Stefano

	Descrizione:
	Procedura di gestione delle operazioni della paletta dell'Xtension

	Parametri:
	nessuno
	
	Ritorno:
	ritorna TRUE se si vuole fa fare un SysBeep, con FALSE Xpress non
	fa sol la nulla

* ------------------------------------------------------------------------ */

#define THEDPEEK ((DialogPeek)thewptr)
#define THEWPEEK ((WindowPeek)thewptr)


{
	WindowPeek tmpwptr,tmpwpeek;
	WindowPtr behindwptr;
	int item,result = FALSE;
	
	BEGINWAP;
	
	if (thewptr) SetPort((WindowPtr) thewptr);

	switch (opcode) {
		case WOPEN:
			/* test per verificare se la palette di debug Š gi… stata aperta */
			tmpwpeek = findpalette(gPalettaXtension.identificatorePaletta);
			if (tmpwpeek) {
				selectwindow((WindowPtr) tmpwpeek);
				break;
			}
			palettewsetup();


			behindwptr = (xtcurdoc >= 0 ? xtdocwptr : (WindowPtr)-1L);
			thewptr =  getnewdialog(rWindowPaletta,
												NULL, 
												behindwptr);
			gPalettaXtension.ptrPaletta = thewptr;
			SetWRefCon((WindowPtr) thewptr,(long) StripAddress(WAPPaletta));
			SetPort(thewptr);
			MoveWindow(thewptr,30000,30000,FALSE);
	

			TextFont(dialogfontid1);
			TextSize(9);
			(*THEDPEEK->textH)->txSize = 9;
			(*THEDPEEK->textH)->txFont = dialogfontid1;
			(*THEDPEEK->textH)->lineHeight = 12;
			(*THEDPEEK->textH)->fontAscent = 10;
	
			/* gestione della lista */
			CreaListaPaletta();
									
			ShowWindow(thewptr);
			if (gPalettaXtension.identificatorePaletta < 0)
				gPalettaXtension.identificatorePaletta = getnewpaletteid();
			
			THEWPEEK->windowKind = gPalettaXtension.identificatorePaletta;

			MoveWindow(thewptr,
					   (**gPalettaXtension.hndlPosizionePaletta).posizione.h,
						(**gPalettaXtension.hndlPosizionePaletta).posizione.v,
					   FALSE);
			BringToFront(thewptr);
			HiliteWindow(thewptr,TRUE);
			
			palettewindows++;

			(**gPalettaXtension.hndlPosizionePaletta).isVedibile = TRUE;
			break;

		case WUPDATE:
			THEWPEEK->windowKind = dialogKind;
			
			if (gPalettaXtension.hndlLista != NULL)
				LUpdate(thewptr->visRgn, gPalettaXtension.hndlLista);

			outlines(thewptr);
	
			DrawDialog(thewptr);
	
	
			THEWPEEK->windowKind = gPalettaXtension.identificatorePaletta;
			break;

		case WCLOSE:
			(**gPalettaXtension.hndlPosizionePaletta).posizione =
				topLeft((thewptr)->portRect);
			LocalToGlobal(&(**gPalettaXtension.hndlPosizionePaletta).posizione);
			
			palettewindows--;
			
			if (shutdown != TRUE)
				(**gPalettaXtension.hndlPosizionePaletta).isVedibile = FALSE;
				
			THEWPEEK->windowKind = dialogKind;
			
			DisposeDialog(thewptr);
			gPalettaXtension.ptrPaletta = NULL;
			break;

		case WCURSOR:
		
			ARROWCURSOR;
			break;

		case WCLICK:
		
			if (thewptr != FrontWindow()) {
				BringToFront(thewptr);
				updatewnd(thewptr);
			}

			LocalToGlobal(&myevent.where);
			
			/* deve continua nel case successivo */

		case WKEY:
		
			THEWPEEK->windowKind = dialogKind;
			
			item = dodialog(THEDPEEK, ALLOK, NULL);
			switch (item) {
				case iListaPaletta:
					if (gPalettaXtension.hndlLista != NULL) {
						GlobalToLocal(&myevent.where);
						LClick(myevent.where,
								myevent.modifiers,
							   gPalettaXtension.hndlLista);
					}
					break;
				default:
					break;
			}
			
			THEWPEEK->windowKind = gPalettaXtension.identificatorePaletta;
			break;

		case WACTIVATE:
		case WDEACTIVATE:
			tmpwptr = THEWPEEK->nextWindow;
			if (tmpwptr &&
				(PALETTEKIND((WindowPeek) tmpwptr) ||
				 DOCUMENTKIND((WindowPeek) tmpwptr))) {
				HiliteWindow((WindowPtr) tmpwptr, (opcode == WACTIVATE));
				activatewnd((WindowPtr) tmpwptr, (opcode == WACTIVATE));
			}
			break;

		case WOTHER:
			result = TRUE;
			break;
	}	
	ENDWAP;
	return (result);
}



void Visualizza(char *stringa)
/* ------------------------------------------------------------------------ *

	Visualizza

	14 Novembre 1995						Stefano

	Descrizione:
	Per visualizzare una stringa sulla palette dell'Xtension

	Parametri:
	stringa -  puntatore alla stringa da stampare

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */

{
	GrafPtr lVecchioPorco = NULL;
	/* puntatore alla finestra sotto la palette */
	WindowPtr lPtrPaletta = NULL;
	/* puntatore alla palette */
	Point lElemento = {0,0};
	/* per inserire il messaggio nella lista */
		int	i = 0;
		/* indice per i for */
		Boolean lIsVedibile = FALSE;
		/* per sapere se la paletta Š vedibile */
		Size lLunghezza = 0;
		/* lunghezza delle stringhe da visualizzare */

	/* sposto gli elementi della lista */
	for (i = 0; i < kDimensioneLista - 1; i++) {
		strcpy(gPalettaXtension.ptrStringheLista[i],
				gPalettaXtension.ptrStringheLista[i + 1]);
	}
	/* inserisco un nuovo elemento */
	strcpy(gPalettaXtension.ptrStringheLista[kDimensioneLista - 1], stringa);

	/* prendoi le info sulla visualizzazione della paletta */
	lPtrPaletta = gPalettaXtension.ptrPaletta;
	lIsVedibile = (**gPalettaXtension.hndlPosizionePaletta).isVedibile;
	/* solo se Š aperta scrittura sulla palette */
	if (lPtrPaletta != (WindowPtr)(-1L) && lIsVedibile == TRUE) {
		/* salvo il port corrente */
		GetPort(&lVecchioPorco);
		/* imposto la paletta dell'xtension */
		SetPort((GrafPort *)lPtrPaletta);

		/* spostamento di una linea verso l'alto gli elementi della lista */
		lElemento.h = lElemento.v = 0;
		for (i = 0; i < kDimensioneLista; i++) {
			lLunghezza = strlen(gPalettaXtension.ptrStringheLista[i]);
			LSetCell(gPalettaXtension.ptrStringheLista[i],
					 lLunghezza,
					 lElemento,
					 gPalettaXtension.hndlLista);
			LSetSelect(FALSE, lElemento, gPalettaXtension.hndlLista);
			lElemento.v++;
		}

		/* per visualizzare i nuovi elementi */
		LDoDraw(FALSE, gPalettaXtension.hndlLista);
		lElemento.h = 0;
		lElemento.v = kDimensioneLista - 1;
		LScroll(-1000, -1000, gPalettaXtension.hndlLista);
		LScroll(lElemento.h, lElemento.v, gPalettaXtension.hndlLista); /* */
		LDoDraw(TRUE, gPalettaXtension.hndlLista);

		((WindowPeek) lPtrPaletta)->windowKind = dialogKind;
		DrawDialog((DialogPtr) lPtrPaletta);
		((WindowPeek) lPtrPaletta)->windowKind = gPalettaXtension.identificatorePaletta;
		SetPort(lVecchioPorco);
	}
}

void VisualizzaStoLavorando(void)
/* ------------------------------------------------------------------------ *

	VisualizzaStoLavorando

	14 Gennaio 1996						Stefano

	Descrizione:
	Per visualizzare che l'algoritmo di posizionamento sta funzionando

	Parametri:
	nessuno

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */

{
#if kOLD
	GrafPtr lVecchioPorco = NULL;
	/* puntatore alla finestra sotto la palette */
	WindowPtr lPtrPaletta = NULL;
	/* puntatore alla palette */
	Point lElemento = {0,0};
	/* per inserire il messaggio nella lista */
		Boolean lIsVedibile = FALSE;
		/* per sapere se la paletta Š vedibile */
		Size lLunghezza = 0;
		/* lunghezza delle stringhe da visualizzare */
		char lStringa[2] = "\0\0";

		gIndiceStoLavorando++;
		if (gIndiceStoLavorando == 4) gIndiceStoLavorando = 0;
		lStringa[0] = gStrStoLavorando[gIndiceStoLavorando];

	/* inserisco un nuovo elemento */
	strcpy(gPalettaXtension.ptrStringheLista[kDimensioneLista - 1], lStringa);

	/* prendoi le info sulla visualizzazione della paletta */
	lPtrPaletta = gPalettaXtension.ptrPaletta;
	lIsVedibile = (**gPalettaXtension.hndlPosizionePaletta).isVedibile;
	/* solo se Š aperta scrittura sulla palette */
	if (lPtrPaletta != (WindowPtr)(-1L) && lIsVedibile == TRUE) {
		/* salvo il port corrente */
		GetPort(&lVecchioPorco);
		/* imposto la paletta dell'xtension */
		SetPort((GrafPort *)lPtrPaletta);

		/* cambio l'ultima linea della lista */
		lElemento.h = 0;
		lElemento.v = kDimensioneLista - 1;
		lLunghezza = strlen(gPalettaXtension.ptrStringheLista[kDimensioneLista - 1]);
		LSetCell(gPalettaXtension.ptrStringheLista[kDimensioneLista - 1],
				 lLunghezza,
				 lElemento,
				 gPalettaXtension.hndlLista);
		LSetSelect(FALSE, lElemento, gPalettaXtension.hndlLista);

		/* per visualizzare i nuovi elementi */
		LDoDraw(FALSE, gPalettaXtension.hndlLista);
		lElemento.h = 0;
		lElemento.v = kDimensioneLista - 1;
		LScroll(-1000, -1000, gPalettaXtension.hndlLista);
		LScroll(lElemento.h, lElemento.v, gPalettaXtension.hndlLista); /* */
		LDoDraw(TRUE, gPalettaXtension.hndlLista);

		((WindowPeek) lPtrPaletta)->windowKind = dialogKind;
		DrawDialog((DialogPtr) lPtrPaletta);
		((WindowPeek) lPtrPaletta)->windowKind = gPalettaXtension.identificatorePaletta;
		SetPort(lVecchioPorco);
	}
#endif
}
#endif

#endif // 0 NECRO v1
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/