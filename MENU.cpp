/* ------------------------------------------------------------------------ *
	Menu.cpp

	23 marzo 2005 - Fabrizio

	Copyright © Sinedita S.r.l 2003. All Rights Reserved.

	$Log: not supported by cvs2svn $
	Revision 1.9  2005/05/10 09:30:27  taretto
	finestra di about
	
	Revision 1.8  2005/05/09 12:36:45  taretto
	aggiunta protezione HW
	
	Revision 1.7  2005/05/09 09:47:53  taretto
	gestione importazione tutte immagini ok
	
	Revision 1.6  2005/05/04 14:31:20  taretto
	ingombro - immagine campione
	
	Revision 1.5  2005/05/04 12:34:52  taretto
	comandi posiziona e ripristino ok
	
	Revision 1.4  2005/05/02 10:03:14  taretto
	avanzamento lavori porting
	
	Revision 1.3  2005/04/27 16:17:45  taretto
	gestione prepara documento (wap + preparazione)
	
	Revision 1.2  2005/04/20 13:48:52  taretto
	impaginazione ok
	
	Revision 1.1  2005/04/20 07:14:20  taretto
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

#include <cassert>
#include <string>
#include <sstream>

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpHasp.h"
#include "DbpInclude.h"
#include "DbpMenu.h"
#include "DbpOkDialog.h"
#include "DbpResource.h"
#include "DbpUndoEngine.h"
#include "DbpView.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "CalcPos.h"
#include "DocInfo.h"
#include "Errori.h"
#include "ExPos.h"
#include "Immagini.h"
#include "OpXt.h"
#include "Paletta.h"
#include "Pref.h"
#include "PrepDoc.h"
#include "Ripristi.h"
#include "SimbPax.h"
#include "XTNecro.h"

#include "Menu.h"

// CONSTS
// CONSTS
// CONSTS

// per essere sicuro che ci sia solo un documento aperto
#define kUnDocumentoAperto 2

// STATICS
// STATICS
// STATICS

// variabili stato comando
static Boolean gPreparaDocumento = TRUE;
static Boolean gCalcolaPosizionamento = FALSE;
static Boolean gPosiziona = FALSE;
static Boolean gModificaOrdine = FALSE;
static Boolean gRipristinaDati = FALSE;
static Boolean gRipristinaDocumento = FALSE;
static Boolean gControllaImmagini = TRUE;
static Boolean gImpaginaImmagini = TRUE;
static Boolean gAssociaImmagine = TRUE;
static Boolean gCreaImmagineCampione = TRUE;
static bool8 gImpostaSimboloPax = TRUE;
static Boolean gPreferenze = TRUE;

// GLOBALS
// GLOBALS
// GLOBALS

// handle al menu
Menu gNecroMenu;

// identificatori di menu
int16 gNecroMenuId;
uint16 gAbilitaDisabilitaMenuId;
uint16 gPreparaDocMenuId;
uint16 gCalcPosMenuId;
uint16 gPosizionaMenuId;
uint16 gRipristinaDatiUltimaMenuId;
uint16 gRipristinaDocMenuId;
uint16 gCreaImgCampioneMenuId;
uint16 gControllaImgMenuId;
uint16 gImportaImgMenuId;
uint16 gImpostaPaxMenuId;
uint16 gPreferenzeMenuId;
uint16 gMostaNascondiPaletteMenuId;
uint16 gAboutMenuId;

// globale che contiene lo stato del menu abilita
bool8 gFlagAbilita = TRUE;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			IsAbilitareMenu
	@abstract 		test
	@discussion		Per sapere se si deve abilitare o meno un menu'
					
					6 Aprile 2005 - Taretto Fabrizio.

	@param			comandomenu contiene l'identificatore alla voce da menu da controllare
	@result  			nessuno
*/
static XTAPI bool8 IsAbilitareMenu(int32 comandomenu) throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	addmenuitems_callback
	
* ------------------------------------------------------------------------ */
int32 XTAPI addmenuitems_callback(xtaddmenuitemsrec* cbparam)
{
	assert(NULL != cbparam);

	if (gNecroMenu)
	{
		// Xpress is calling this function as a result of a language change
		// delete the previuos menu in preparation of creating the new one
		DBP::DeleteMenu(gNecroMenu);
	}
	
	// creo XTNecro menu
	Str255 tempStr = "";
	DBP::GetResourceString(tempStr, MENUSTRID, NECROMENUID);	
	DBP::CreateMenu(tempStr, gNecroMenuId, gNecroMenu);
	
	// abilita / disabilita
	DBP::GetResourceString(tempStr, MENUSTRID, gFlagAbilita ?  ABILITAMENUID : DISABILITAMENUID);
	gAbilitaDisabilitaMenuId = DBP::AppendMenuItem(gNecroMenu, tempStr);
	
	DBP::AppendMenuSeparator(gNecroMenu);
	
	// prepara documento
	DBP::GetResourceString(tempStr, MENUSTRID, PREPARADOCUMENTOMENUID);
	gPreparaDocMenuId = DBP::AppendMenuItem(gNecroMenu, tempStr);
	
	// calcola posizionamento
	DBP::GetResourceString(tempStr, MENUSTRID, CALCPOSMENUID);
	gCalcPosMenuId = DBP::AppendMenuItem(gNecroMenu, tempStr);
	
	// posiziona
	DBP::GetResourceString(tempStr, MENUSTRID, POSIZIONAMENUID);
	gPosizionaMenuId = DBP::AppendMenuItem(gNecroMenu, tempStr);
	
	DBP::AppendMenuSeparator(gNecroMenu);
	
	// ripristina dati ultima
	DBP::GetResourceString(tempStr, MENUSTRID, RIPRISTINADATIULTIMAMENUID);
	gRipristinaDatiUltimaMenuId = DBP::AppendMenuItem(gNecroMenu, tempStr);
		
	// ripristina doc	
	DBP::GetResourceString(tempStr, MENUSTRID, RIPRISTINADOCMENUID);
	gRipristinaDocMenuId = DBP::AppendMenuItem(gNecroMenu, tempStr);
	
	DBP::AppendMenuSeparator(gNecroMenu);
	
	// crea immagine campione
	DBP::GetResourceString(tempStr, MENUSTRID, CREAIMGCAMPIONEMENUID);
	gCreaImgCampioneMenuId = DBP::AppendMenuItem(gNecroMenu, tempStr);
	
	// controlla immagine
	DBP::GetResourceString(tempStr, MENUSTRID, CONTROLLAIMGMENUID);
	gControllaImgMenuId = DBP::AppendMenuItem(gNecroMenu, tempStr);
	
	// importa immagini
	DBP::GetResourceString(tempStr, MENUSTRID, IMPORTAIMGMENUID);
	gImportaImgMenuId = DBP::AppendMenuItem(gNecroMenu, tempStr);
	
	DBP::AppendMenuSeparator(gNecroMenu);
	
	// imposta simbolo pax
	DBP::GetResourceString(tempStr, MENUSTRID, IMPOSTAPAXMENUID);
	gImpostaPaxMenuId = DBP::AppendMenuItem(gNecroMenu, tempStr);
	
	DBP::AppendMenuSeparator(gNecroMenu);
	
	// mostra / nascondi palette
	DBP::GetResourceString(tempStr, MENUSTRID, gInfoPaletteShowingFlag ? NASCONDIINFOPALETTEMENUID : MOSTRAIFOPALETTEMENUID);
	gMostaNascondiPaletteMenuId = DBP::AppendMenuItem(gNecroMenu, tempStr);
	
	DBP::AppendMenuSeparator(gNecroMenu);
	
	// preferenze
	DBP::GetResourceString(tempStr, MENUSTRID, PREFERENZEMENUID);
	gPreferenzeMenuId = DBP::AppendMenuItem(gNecroMenu, tempStr);
	
	DBP::AppendMenuSeparator(gNecroMenu);
	
	// about
	DBP::GetResourceString(tempStr, MENUSTRID, ABOUTMENUID);
	gAboutMenuId = DBP::AppendMenuItem(gNecroMenu, tempStr);
					
	return(noErr);

} // addmenuitems_callback

/* ------------------------------------------------------------------------ *

	updatemenuitems_callback
	
* ------------------------------------------------------------------------ */
int32 XTAPI updatemenuitems_callback(xtupdatemenuitemsrec* cbparam)
{
	assert(NULL != cbparam);

	// force to redraw the QuarkXPress menu bar	
	cbparam->redraw = TRUE;
	
	// prendo docID corrente
	DocID curDoc = INVALDOC;
	xtget_curdoc(&curDoc);
	
	DBP::DisableMenuItem(gNecroMenu, gPreparaDocMenuId);
	DBP::DisableMenuItem(gNecroMenu, gCalcPosMenuId);
	DBP::DisableMenuItem(gNecroMenu, gPosizionaMenuId);
	DBP::DisableMenuItem(gNecroMenu, gRipristinaDatiUltimaMenuId);
	DBP::DisableMenuItem(gNecroMenu, gRipristinaDocMenuId);
	DBP::DisableMenuItem(gNecroMenu, gCreaImgCampioneMenuId);
	DBP::DisableMenuItem(gNecroMenu, gControllaImgMenuId);
	DBP::DisableMenuItem(gNecroMenu, gImportaImgMenuId);
	DBP::DisableMenuItem(gNecroMenu, gImpostaPaxMenuId);

// Mostra/Nascondi palette informazioni
// Mostra/Nascondi palette informazioni
// Mostra/Nascondi palette informazioni

	// sempre abilitato	
	// update the info palette menu command	
	Str255 menuStr = "";
	DBP::GetResourceString(menuStr, MENUSTRID, gInfoPaletteShowingFlag ? NASCONDIINFOPALETTEMENUID : MOSTRAIFOPALETTEMENUID);
	DBP::SetMenuCommandText(gNecroMenu, gMostaNascondiPaletteMenuId, menuStr);

	return(noErr);

} // updatemenuitems_callback

/* ------------------------------------------------------------------------ *

	handlecommand_callback
	
* ------------------------------------------------------------------------ */
int32 XTAPI handlecommand_callback(xthandlecommandrec *cbparam)
{
	assert(NULL != cbparam);
	
	if (cbparam->commandid == gAbilitaDisabilitaMenuId)
	{
	
#if defined(NDEBUG)
		// controllo la presenza della chiave
		if (!DBP::TestHaspKey(XTNECROHASPSTR))
		{
			// chiave non trovata
			// mostra la dialog di errore
			NOPROTECTIONMESSAGE;

			goto _Exit;
		}
#endif // defined(NDEBUG)		

		CambiaOperazioneCorrente();
	}
	else if (cbparam->commandid == gPreparaDocMenuId)
	{

#if defined(NDEBUG)
		// controllo la presenza della chiave
		if (!DBP::TestHaspKey(XTNECROHASPSTR))
		{
			// chiave non trovata
			// mostra la dialog di errore
			NOPROTECTIONMESSAGE;

			goto _Exit;
		}
#endif // defined(NDEBUG)	
	
		gErroreXtension = PrendiDocInfo();
		if (gErroreXtension != kNessunErrore) 
		{
			// errore sulla lettura delle info sul documento definitivo
			MostraErrore(gErroreXtension);
		}
		else
		{
			DBP::FreezeUndo();
			PreparaDocumento();
			DBP::UnFreezeUndo();	
		}
	}
	else if (cbparam->commandid == gCalcPosMenuId)
	{
		DBP::FreezeUndo();
		gErroreXtension = CalcolaPosizionamento();
		if (gErroreXtension != kNessunErrore) 
		{
			// errore nel calcolare il posizionamento dei necrologi
			MostraErrore(gErroreXtension);
			
			HWND hwnd = NULL;
			xtget_hwndmainframe(&hwnd);
			
			if (IDYES == MessageBox(hwnd, "Si è verificato un errore nel calcolo del posizionamento (controlla i messaggi nella finestra d'informazioni).\rVuoi provare comunque a posizionare i necrologi (Rispondendo sì, si abiliterà il comando Posiziona del menù Necro, dopo il posizionamento controlla l'area di lavoro a sinistra della prima pagina)?", "XTNecro.xxt", MB_ICONQUESTION | MB_YESNO ))
			{
				AbilitaMenu(POSIZIONAMENUID);
			}

			// abilito il menù per rispistinare il documento e i dati
			AbilitaMenu(RIPRISTINADOCMENUID);
			AbilitaMenu(RIPRISTINADATIULTIMAMENUID);
		}
		DBP::UnFreezeUndo();	
	}
	else if (cbparam->commandid == gPosizionaMenuId)
	{
		DBP::FreezeUndo();
		DBP::DisableDocumentRedraw();
		gErroreXtension = Posiziona();
		if (gErroreXtension != kNessunErrore) 
		{
			// errore nel tentare di posizionare i necrologi
			MostraErrore(gErroreXtension);

			// abilito il menù per rispistinare il documento e i dati
			AbilitaMenu(RIPRISTINADOCMENUID);
			AbilitaMenu(RIPRISTINADATIULTIMAMENUID);
		}
		DBP::EnableDocumentRedraw();
		DBP::UnFreezeUndo();	
	}
	else if (cbparam->commandid == gRipristinaDatiUltimaMenuId) 
	{
		DBP::FreezeUndo();
		gErroreXtension = RipristinaDati();
		if (gErroreXtension != kNessunErrore) 
		{
			// errore nel tentare di ripristinare l'ultima impaginazione
			MostraErrore(gErroreXtension);
		}
		DBP::UnFreezeUndo();
	}
	else if (cbparam->commandid == gRipristinaDocMenuId)
	{
		DBP::FreezeUndo();
		gErroreXtension = RipristinaDocumento();
		if (gErroreXtension != kNessunErrore) 
		{
			// errore nel tentare di ripristinare l'ultima impaginazione
			MostraErrore(gErroreXtension);
		}
		DBP::UnFreezeUndo();
	}
	else if (cbparam->commandid == gCreaImgCampioneMenuId)
	{
		DBP::FreezeUndo();
		gErroreXtension = PrendiDocInfo();
		if (gErroreXtension != kNessunErrore) 
		{
			// errore sulla lettura delle info sul documento definitivo
			MostraErrore(gErroreXtension);
		}
		
		// comando per creare l'immagine campione
		gErroreXtension = CreaImmagineCampione();
		if (gErroreXtension != kNessunErrore) 
		{
			// errore nel controllo delle immagini
			MostraErrore(gErroreXtension);
		}
		
		DBP::UnFreezeUndo();
	}
	else if (cbparam->commandid == gControllaImgMenuId)
	{
		// comando controlla le immagini da impaginare
		gErroreXtension = ControllaImmagini();

		if (gErroreXtension != kNessunErrore) 
		{
			// errore nel controllo delle immagini
			MostraErrore(gErroreXtension);
		}
	}
	else if (cbparam->commandid == gImportaImgMenuId)
	{
		 ImpaginaImmagini();
	}
	else if (cbparam->commandid == gImpostaPaxMenuId)
	{
		// comando ImpostaSimboloPax
		gErroreXtension = ImpostaSimboloPax();
		if (gErroreXtension != kNessunErrore)
		{
			// errore nella impostazione del simbolo
			MostraErrore(gErroreXtension);
		}
	}
	else if (cbparam->commandid == gPreferenzeMenuId)
	{
		xd_createdialog(_XT_PREFERENZEWAP, 0, NULL);
	}
	else if (cbparam->commandid == gMostaNascondiPaletteMenuId)
	{
		// apre la finestra delle preferenze
		MostraNascondiPaletteInfo();
	}
	else if (cbparam->commandid == gAboutMenuId)
	{
		xd_createdialog(_XT_ABOUTWAP, 0, NULL);	
	}

_Exit:
	return(noErr);
	
} // domenuitem_callback


/* ------------------------------------------------------------------------ *

	IsAbilitareMenu

* ------------------------------------------------------------------------ */
static XTAPI bool8 IsAbilitareMenu(int32 comandomenu) throw()
{
	bool8 lBool = FALSE;

	// i comandi del menu dell'Xtension sono abilitati solo se non e' abilitata
	// l'impaginazione automaica 
	lBool = PrendiOperazioneCorrente() == kCoseDellUtente;
	
	// prendo doc corrente
	DocID curdoc;
	xtget_curdoc(&curdoc);
	
	// prendo mode corrente
	TOOLMODE  mode = getmode();	
	
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	// prendo box type
	uint8 curboxtype, shapetype;
	curboxtype = getboxtypes(NULL, &shapetype);

	// prendo numero doc correntemente aperti
	int32 doccount;
	xtget_doccount(&doccount);
	
	// variabile d'utilita' per contenere il bool che indica se sono sul mastro o meno
	uint8 inmaster;
	
	// ref del doc corrente
	DocRef DRef = XTGetDocRefFromDocID(curdoc);
	
	switch(comandomenu) 
	{
		case PREPARADOCUMENTOMENUID:
			if( curdoc >= 0 )
			{
				// verificare se si sta visializzando la pag mastro
				XTGetShowMaster(DRef, &inmaster);
				
				lBool = lBool && gPreparaDocumento == TRUE &&
						  doccount == kUnDocumentoAperto && inmaster == FALSE;
			}
			else lBool = FALSE;
			break;
		case CALCPOSMENUID:
			lBool = lBool && curdoc >= 0 && gCalcolaPosizionamento == TRUE;
			break;
		case POSIZIONAMENUID:
			lBool = lBool && curdoc >= 0 && gPosiziona == TRUE;
			break;
		case RIPRISTINADOCMENUID:
			lBool = lBool && gRipristinaDocumento == TRUE;
			break;
		case RIPRISTINADATIULTIMAMENUID:
			lBool = lBool && curdoc >= 0 && gRipristinaDati == TRUE;
			break;
		case CREAIMGCAMPIONEMENUID:
			if( curdoc >= 0 )
			{
				// verificare se si sta visializzando la pag mastro
				DocRef Dref = XTGetDocRefFromDocID(curdoc);
				XTGetShowMaster(Dref, &inmaster);
				
				// numero di pag corrente
				int16 curPg;
				XTGetCurPage(DRef, &curPg);
				
				lBool = lBool && gCreaImmagineCampione == TRUE &&
					doccount == kUnDocumentoAperto && inmaster == FALSE &&
					curPg == kPrimaPagina;
			}
			else 
				lBool = FALSE;
			break;
		case CONTROLLAIMGMENUID:
			lBool =  lBool && gControllaImmagini == TRUE;
			break;
		case IMPORTAIMGMENUID:
			lBool = lBool && curdoc >= 0 && gImpaginaImmagini == TRUE;
			break;
		case ASSOCIAIMGMENUID:
			lBool = lBool && curdoc >= 0 && curboxtype != NOBOX &&
					  mode == CONTENTSMODE && isapicture(curbox) && gAssociaImmagine == TRUE;
			break;
			
		case IMPOSTAPAXMENUID:
			lBool = lBool && curdoc >= 0 && curboxtype != NOBOX &&
					mode == CONTENTSMODE && isapicture(curbox) && gImpostaSimboloPax == TRUE;
			break;
			
		default:
			break;
	}
	return(lBool);
} // IsAbilitareMenu

/* ------------------------------------------------------------------------ *

	AbilitaMenu

* ------------------------------------------------------------------------ */
void XTAPI AbilitaMenu(int32 comandomenu) throw()
{
	switch(comandomenu) 
	{
		case PREPARADOCUMENTOMENUID:
			gPreparaDocumento = TRUE;
			break;
		case CALCPOSMENUID:
			gCalcolaPosizionamento = TRUE;
			break;
		case POSIZIONAMENUID:
			gPosiziona = TRUE;
			break;
		case RIPRISTINADATIULTIMAMENUID:
			gRipristinaDati = TRUE;
			break;
		case RIPRISTINADOCMENUID:
			gRipristinaDocumento = TRUE;
			break;
		case CREAIMGCAMPIONEMENUID:
			gCreaImmagineCampione = TRUE;
			break;
		case CONTROLLAIMGMENUID:
			gControllaImmagini = TRUE;
			break;
		case IMPORTAIMGMENUID:
			gImpaginaImmagini = TRUE;
			break;
		case ASSOCIAIMGMENUID:
			gAssociaImmagine = TRUE;
			break;
		case IMPOSTAPAXMENUID:
			gImpostaSimboloPax = TRUE;
			break;	
		default:
			break;
	}
} // AbilitaMenu


/* ------------------------------------------------------------------------ *

	DisabilitaMenu

* ------------------------------------------------------------------------ */
void XTAPI DisabilitaMenu(int32 comandomenu) throw()
{
	switch(comandomenu) 
	{
		case PREPARADOCUMENTOMENUID:
			gPreparaDocumento = FALSE;
			break;
		case CALCPOSMENUID:
			gCalcolaPosizionamento = FALSE;
			break;
		case POSIZIONAMENUID:
			gPosiziona = FALSE;
			break;
		case RIPRISTINADATIULTIMAMENUID:
			gRipristinaDati = FALSE;
			break;
		case RIPRISTINADOCMENUID:
			gRipristinaDocumento = FALSE;
			break;
		case CREAIMGCAMPIONEMENUID:
			gCreaImmagineCampione = FALSE;
			break;
		case CONTROLLAIMGMENUID:
			gControllaImmagini = FALSE;
			break;
		case IMPORTAIMGMENUID:
			gImpaginaImmagini = FALSE;
			break;
		case ASSOCIAIMGMENUID:
			gAssociaImmagine = FALSE;
			break;
		case IMPOSTAPAXMENUID:
			gImpostaSimboloPax = FALSE;
			break;	
		default:
			break;
	}
} //  DisabilitaMenu


/* ------------------------------------------------------------------------ *

	AggiornaMenu

* ------------------------------------------------------------------------ */
void XTAPI AggiornaMenu() throw()
{
	if (IsAbilitareMenu(PREPARADOCUMENTOMENUID) == TRUE) 
		DBP::EnableMenuItem(gNecroMenu, gPreparaDocMenuId);
	else 
		DBP::DisableMenuItem(gNecroMenu, gPreparaDocMenuId);

	if (IsAbilitareMenu(CALCPOSMENUID) == TRUE) 
		DBP::EnableMenuItem(gNecroMenu, gCalcPosMenuId);
	else 
		DBP::DisableMenuItem(gNecroMenu, gCalcPosMenuId);
	
	if (IsAbilitareMenu(POSIZIONAMENUID) == TRUE) 
		DBP::EnableMenuItem(gNecroMenu, gPosizionaMenuId);
	else 
		DBP::DisableMenuItem(gNecroMenu, gPosizionaMenuId);
	
	if (IsAbilitareMenu(RIPRISTINADATIULTIMAMENUID) == TRUE) 
		DBP::EnableMenuItem(gNecroMenu, gRipristinaDatiUltimaMenuId);
	else 
		DBP::DisableMenuItem(gNecroMenu, gRipristinaDatiUltimaMenuId);

	if (IsAbilitareMenu(RIPRISTINADOCMENUID) == TRUE) 
		DBP::EnableMenuItem(gNecroMenu, gRipristinaDocMenuId);
	else 
		DBP::DisableMenuItem(gNecroMenu, gRipristinaDocMenuId);

	if (IsAbilitareMenu(CREAIMGCAMPIONEMENUID) == TRUE) 
		DBP::EnableMenuItem(gNecroMenu, gCreaImgCampioneMenuId);
	else 
		DBP::DisableMenuItem(gNecroMenu, gCreaImgCampioneMenuId);

	if (IsAbilitareMenu(CONTROLLAIMGMENUID) == TRUE) 
		DBP::EnableMenuItem(gNecroMenu, gControllaImgMenuId);
	else 
		DBP::DisableMenuItem(gNecroMenu, gControllaImgMenuId);

	if (IsAbilitareMenu(IMPORTAIMGMENUID) == TRUE) 
		DBP::EnableMenuItem(gNecroMenu, gImportaImgMenuId);
	else 
		DBP::DisableMenuItem(gNecroMenu, gImportaImgMenuId);

	if (IsAbilitareMenu(IMPOSTAPAXMENUID) == TRUE) 
		DBP::EnableMenuItem(gNecroMenu, gImpostaPaxMenuId);
	else 
		DBP::DisableMenuItem(gNecroMenu, gImpostaPaxMenuId);
	
} // AggiornaMenu