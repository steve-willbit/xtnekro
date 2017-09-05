
/* ------------------------------------------------------------------------ *

	Documenti.cpp

	11 Aprile 2005 				Fabrizio

	Descrizione:
	Contiene le procedure usate per aprire, salvare e chiudere i documenti
	Xpress

	$Log: not supported by cvs2svn $
	Revision 1.3  2005/05/09 07:40:56  taretto
	stampa bozze ok
	
	Revision 1.2  2005/04/20 13:48:52  taretto
	impaginazione ok
	
	Revision 1.1  2005/04/20 07:14:18  taretto
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
#include "Pref.h"
#include "OpXt.h"
#include "DocInfo.h"
#include "PresFile.h"
#include "Memoria.h"
#include "XTNecro.h"

#include "Document.h"

// GLOBALS
// GLOBALS
// GLOBALS

pathhandle 	gHndlPathDocumento = NULL;
uchar 		gNomeDocumento[80] = "";

// STATIC GLOBALS
// STATIC GLOBALS
// STATIC GLOBALS

// working directory dell'ultima cartella usata
static int16 gWDCartella = 0;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			Salva
	@abstract		gestione documento XPress
	@discussion		Salva il documento corrente con il nome passato e nella cartella specificata
					come parametro

					13 Aprile 2005 - Fabrizio.

	@param			nomedocumento contiene il nome da usare per salvare il documento corrente
	@param			idcartella contiene l'identificatore della cartella dove salvare
	@param			wdcartella contiene il working directory della cartella dove salvare
	@result			ritorna l'eventuale errore dell'Xtensione
*/
static errorixtension XTAPI Salva(uchar *nomedocumento, int32 idcartella, int16 wdcartella) throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	ApriDocumento

* ------------------------------------------------------------------------ */
errorixtension XTAPI ApriDocumento(uchar *nomedocumento) throw()
{
  	FSSpec specTemplateLocation;
  	OSErr fsMakeErr = FSMakeFSSpec(0, 0, nomedocumento, &specTemplateLocation);
    	
    	XTError checkOpen = XTOpenDocFSp(&specTemplateLocation, SUPPRESSWARNINGS | SKIPMISSINGFONT | SKIPASKUSEPREFS | USEDOCPREFS);
    	if (checkOpen != noErr)
    	{
    		// errore sulla apertura del documento da aprire
		ConcatenaAllaStringaErrore(nomedocumento);
		return(kErroreAperturaDocumento);
    	}
    	
    	gErroreXtension = PrendiDocInfo();
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nel prelevare le info del documento
		return(gErroreXtension);
	}

	// per visualizzare correttamente la finestra
	// updatewnd((WindowPtr)xtdocwptr); <- nelle nouve versioni di XPress non dovrebbe + servire
	
	return(kNessunErrore);
} // ApriDocumento

/* ------------------------------------------------------------------------ *

	ChiudiDocumento

* ------------------------------------------------------------------------ */
void XTAPI ChiudiDocumento() throw()
{
	GrafPtr docPtr;	
	xtget_docwptr(&docPtr);
	HWND docHWND = GetPortHWND(docPtr);
	closewnd(docHWND, CLOSENOSAVE);
	
} // ChiudiDocumento

/* ------------------------------------------------------------------------ *

	Salva

/* ------------------------------------------------------------------------ */
static errorixtension XTAPI Salva(uchar *nomedocumento, int32 idcartella, int16 wdcartella) throw()
{
	// reperisco project id
	DocID curDoc;
    	xtget_curdoc(&curDoc);
    	ProjectID projectID;
    	XTGetLayoutSpaceProjectID(XTGetDocRefFromDocID((DocID) curDoc), &projectID);
	
	// copio il nome passato
	STRCPY(gNomeDocumento, nomedocumento);
	
	// creo l'handle
	gErroreXtension = CreaHandle((Handle *) &gHndlPathDocumento, sizeof(pathrec) + MAXPATHNAME + 1);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nell'allocazione di memoria
		return(gErroreXtension);
	}
	
	FSSpec spec;
	int16 makeFSSpecErr = FSMakeFSSpec(0, 0, gNomeDocumento, &spec);

	XFileInfoRef fileInfoRef;
	QXStringRef pathRef = NULL;
	QXStringCreateFromCString((char*) gNomeDocumento, 0, (int32) CSTRLEN(gNomeDocumento), &pathRef);
	APIERR apiErr = XTCreateEmptyXFileInfoRef(&fileInfoRef);
	apiErr = XTSetFileTypeInXFileInfoRef(fileInfoRef, XPRJ);
	apiErr = XTSetUniPathInXFileInfoRef(fileInfoRef, pathRef);
	
	STRCPY((*gHndlPathDocumento)->longpath, gNomeDocumento);
	(*gHndlPathDocumento)->volnum = spec.vRefNum;
	(*gHndlPathDocumento)->dirnum = spec.parID;
//	gErrore = xtsave(gHndlPathDocumento, FALSE, FALSE); <-- already commented in QXP60
	
	if ((makeFSSpecErr == noErr) || (makeFSSpecErr == fnfErr))
	{
		makeFSSpecErr = FSpDelete(&spec);
		if (makeFSSpecErr == (fBsyErr))
		{
			// se sto cercando di salvare il file aperto correntemente faccio un save normale
			 
			// gErrore = xtsave(gHndlPathDocumento, FALSE, FALSE);
			gErrore = XTSave(fileInfoRef, FALSE);
		}
		else if ((makeFSSpecErr == noErr) || (makeFSSpecErr == fnfErr))
		{
			// APIERR checkSave = XTSaveProject(projectID, gHndlPathDocumento, DOC_6_0_VER, &gErrore);	
			APIERR checkSave = XTUSaveProject(projectID, fileInfoRef, DOCCRVER, &gErrore);	
		}
	}
	else
	{
		// gestione errore
		gErrore = kErroreSalvaDocumentoCorrente;
	}
	
	LiberaHandle((Handle *) &gHndlPathDocumento);

	QXStringDestroy(pathRef);
    XTDisposeXFileInfoRef(fileInfoRef);
	
	if (gErrore != noErr)
	{
		// errore sul save del documento
		return(kErroreSalvaDocumentoCorrente);
	}

	return(kNessunErrore);
} // Salva

/* ------------------------------------------------------------------------ *

	SalvaDocument

* ------------------------------------------------------------------------ */
errorixtension XTAPI SalvaDocumento(uchar* nomecartella, uchar* nomedocumento) throw()
{
	// identificatore della cartella dove salvare il documento
	int32 lIdentificatoreCartella = 0;
	
	uchar strTemp[kDimensioneStringhe] = "";
	STRCPY( strTemp, nomecartella );
	
	// taglio il nome per essere sicuro che non sia piu' lungo di 31 caratteri <- nn dovrebbe + servire
/*
	if (STRLEN(nomedocumento) > 12) 
		nomedocumento[12] = '\0';
*/

	if(strTemp[STRLEN( strTemp ) - 1] != '\\' )
		STRCAT( strTemp, "\\" );
	STRCAT(strTemp, nomedocumento);
	
	// salvo il documento
	gErroreXtension = Salva(strTemp, lIdentificatoreCartella, gWDCartella);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore sul salvataggio del documento
		return(gErroreXtension);
	}

	return(kNessunErrore);
} // SalvaDocument

/* ------------------------------------------------------------------------ *

	ChiudiTuttiIDocumenti

* ------------------------------------------------------------------------ */
errorixtension XTAPI ChiudiTuttiIDocumenti(bool8 devosalvare) throw()
{
	// puntatore alla finestra corrente
	HWND lPtrFinestraCorrente = NULL;
	
	while (1) 
	{
		// conto numero documenti aperti
		int32 openDocs;
		xtget_doccount(&openDocs);
	
		// prendo finestra corrente
		GrafPtr docPtr;	
		xtget_docwptr(&docPtr);
		lPtrFinestraCorrente = GetPortHWND(docPtr);
		
		if (lPtrFinestraCorrente != NULL &&
			openDocs > 1 && DOCUMENTKIND((WindowPeek) lPtrFinestraCorrente) == TRUE) 
		{

			if (devosalvare == TRUE) 
			{
				// leggo il nome del documento corrente
				STRCPY(gNomeDocumento, curdocname());

				// creo l'handle
				gErroreXtension = CreaHandle((Handle *) &gHndlPathDocumento,
												 (long) (sizeof(pathrec) +
															gNomeDocumento[0] + 1));
				if (gErroreXtension != kNessunErrore) 
				{
					// errore nell'allocazione di memoria
					return(gErroreXtension);
				}

				XFileInfoRef fileInfoRef;
				QXStringRef pathRef = NULL;
				QXStringCreateFromCString((char*) gNomeDocumento, 0, (int32) CSTRLEN(gNomeDocumento), &pathRef);
				APIERR apiErr = XTCreateEmptyXFileInfoRef(&fileInfoRef);
				apiErr = XTSetFileTypeInXFileInfoRef(fileInfoRef, XPRJ);
				apiErr = XTSetUniPathInXFileInfoRef(fileInfoRef, pathRef);

				STRCPY((*gHndlPathDocumento)->longpath,(StringPtr) gNomeDocumento);
				(*gHndlPathDocumento)->type = 0;
				(*gHndlPathDocumento)->volnum = gNumeroVolumeXpress;
				(*gHndlPathDocumento)->dirnum = 0;

				// gErrore = xtsave(gHndlPathDocumento, FALSE, FALSE);
				gErrore = XTSave(fileInfoRef, FALSE);
				
				LiberaHandle((Handle *) &gHndlPathDocumento);

				QXStringDestroy(pathRef);
				XTDisposeXFileInfoRef(fileInfoRef);

				if (gErrore != noErr) 
				{
					// errore sul save del documento
					return(kErroreChiusuraTuttiDocumenti);
				}
			}

			ChiudiDocumento();

		} 
		else
		{
			break;	
		} 
	}

	return(kNessunErrore);
} // ChiudiTuttiIDocumenti

/* ------------------------------------------------------------------------ *

	StampaDocumento

* ------------------------------------------------------------------------ */
static void setupprintvars(void);
static void setupprintvars(void)
{
    printvars localprintvars;
	DocID curdoc;
	xtget_curdoc(&curdoc); 
	DocRef dRef = XTGetDocRefFromDocID(curdoc);
	
    XTGetDocPrintVars(dRef, &localprintvars);
    // Set any custom fields in printvars
    localprintvars.ctrl.emptypages = TRUE; // Inlcude empty pages
    localprintvars.ctrl.separate = FALSE; // Color separations off
    // STRCPY(localprintvars.lasrname,STRCONST("Generic B&W"));
    
    /* Make these the current print settings */
    XTSetSysPrintVars(&localprintvars);
} /* End setupprintvars */

/* ------------------------------------------------------------------------ *

	StampaDocumento

* ------------------------------------------------------------------------ */
void XTAPI StampaDocumento() throw()
{
	// controllo che ci sia un documento corrente
	DocID curdoc;
	xtget_curdoc(&curdoc); 
	if (curdoc >= 0) 
	{
//		int32 temp;
//	   	bool8 printingrange = TRUE;
//		prnt_range_inforechndl myprintrange;
		
	    // Setup printing information
	    setupprintvars();
/*	  
	    myprintrange = (prnt_range_inforechndl)NewHandle(SIZEOFPRNTRANGEINFOREC 
	                    + (sizeof(prntrangerec)));
	    temp = GetHandleSize((Handle)myprintrange);
	    (*myprintrange)->rangecnt = 1;
	    // Fill in the prntrangerec records
	    ()*myprintrange)->rangear[0].startseq = 1;
	    (*myprintrange)->rangear[0].endseq = 2;

	    // Give it to QXP
	    xtset_printrange((Handle)myprintrange);
*/		
		// stampo in automatico il documento appena impaginato
		// start_printdoc(PRINT_THRU_NONE, (THPrint) NULL, NULL);
		DocRef docRef = XTGetDocRefFromDocID(curdoc);
		XTPrintDoc(docRef, kOutputUI_SuppressOutputWap);
	}
} // StampaDocumento
