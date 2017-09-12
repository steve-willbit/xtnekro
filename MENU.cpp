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
#include "DbpOkDialog.h"
#include "DbpResource.h"

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

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	IsAbilitareMenu

* ------------------------------------------------------------------------ */
bool8 XTAPI IsAbilitareMenu(int32 comandomenu) throw()
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

