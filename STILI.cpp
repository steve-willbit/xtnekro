/* ------------------------------------------------------------------------ *

	Stili.cpp

	12 Aprile 2005					Fabrizio

	Descrizione:
	Contiene le procedure usate per calcolare lo spazio disponibile su un
	documento con box gia' posizionati
	
	$Log: not supported by cvs2svn $
	Revision 1.1  2005/04/20 07:14:22  taretto
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
#include "Stili.h"

// STATIC GLOBALS
// STATIC GLOBALS
// STATIC GLOBALS

// variabile per sapere se si e' gia' verfificato un'errore sullo stile
static int32 gErroreSulloStile = FALSE;

// descrittore che contiene le caratteristiche del filetto
caratteristichefiletto gCaratteristicheFiletto = {FALSE, 0, 0, 0, 0, 0, 0};

// PROTOTYPES
// PROTOTYPEs
// PROTOTYPES

/*! 
	@function			CercaNomeStileForte
	@abstract		gestione stili
	@discussion		Cerca lo stile passato come parametro nella tabella degli stili correntemente
					definti. Se esiste ne ritorna l'indice altrimenti ritorna 0

					15 Aprile 2005 - Fabrizio 

	@param			nomestile puntatore ad una stringa C che contiene il nome dello stile di paregrafo
					di cui cercare l'indice
	@result			ritorna l'indice dello stile ricercato
*/
static AssetUID CercaNomeStileForte(uchar* nomestile) throw();

/*! 
	@function			CercaNomeStileDebole
	@abstract		gestione stili
	@discussion		Cerca lo stile passato come parametro nella tabella degli stili correntemente
					definti. Se esiste ne ritorna l'indice altrimenti ritorna 0

					15 Aprile 2005 - Fabrizio 

	@param			nomestile puntatore ad una stringa C che contiene il nome dello stile di carattere
					di cui cercare l'indice
	@result			ritorna l'indice dello stile ricercato
*/
static AssetUID CercaNomeStileDebole(uchar* nomestile) throw();

/*! 
	@function			ApplicaStileDeboleSuTesto
	@abstract		gestione stili
	@discussion		La routine preleva dallo stile di cui e' passato come parametro l'indice,
					e le applica in modo debole alla fine del flusso di testo corrente. Nel caso
					in cui non ci sia lo stile non fa nulla. Non c'e' bisogno neanche di segnalare
					un errore poich‚ e' gia' stato fatto nella precedente procedura che ricerca
					lo stile in base al nome

					15 Aprile 2005 - Fabrizio 

	@param			indicestile indice dello stile da applicare
	@result			nessuno
*/
static void XTAPI ApplicaStileDeboleSuTesto(AssetUID indicestile) throw();


// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	CercaNomeStileForte

* ------------------------------------------------------------------------ */
static AssetUID CercaNomeStileForte(uchar* nomestile) throw()
{
 	AssetUID lIndiceStilePara = 0;

	QXStringRef stileRef = NULL;
	QXStringCreateFromCString((char*) nomestile, 0, (int32) CSTRLEN((char*) nomestile), &stileRef);

	DocID docID = INVAL_DOCID;
	xtget_curdoc(&docID);
	DocRef docRef = XTGetDocRefFromDocID(docID);
	AssetSpaceRef docAssetSpaceRef = XTGetDocAssetSpace(docRef);
	if (docAssetSpaceRef == INVAL_ASSETSPACEREF) {
		docAssetSpaceRef = XTCreateDefaultAssetSpace();
	}

	bool8 found = FALSE;

	int32 index = 0;   
	int32 assetCount = XTGetAssetCount(docAssetSpaceRef, kAssetParaStyle);   
	for(index = 0; index < assetCount; index++) {   
		QXStringRef  assetNameRef = NULL;   
		AssetUID paraAssetUID = INVAL_ASSETUID;   

		APIERR apiErr = XTGetAsset(docAssetSpaceRef, kAssetParaStyle, index, &paraAssetUID);    
		if ( apiErr == ERR_SUCCESS && paraAssetUID != INVAL_ASSETUID) { 

			apiErr = XTGetAssetName(docAssetSpaceRef, kAssetParaStyle, paraAssetUID, &assetNameRef);     
			
			QXStringCompareResult cmpResult;   
			QXStringICompare(stileRef, 0, kNoPos, assetNameRef, 0, kNoPos, kQXSCaseCompareDefault, &cmpResult);

			QXStringDestroy(assetNameRef);   

			if ( cmpResult == kQXStringEqualTo ) {
				lIndiceStilePara = (AssetUID) index;
				found = TRUE;
				break;
			}
		}   
	}   

	QXStringDestroy(stileRef);

	if ( !found ) {
		lIndiceStilePara = INVAL_ASSETUID;

		ConcatenaAllaStringaErrore(nomestile);
		DaiErrore(kErroreStileNonDefinito);
		gErroreSulloStile = TRUE;
	}
	else {
		lIndiceStilePara++;
	}

	return(lIndiceStilePara);

} // CercaNomeStileForte

/* ------------------------------------------------------------------------ *

	CercaNomeStileDebole

* ------------------------------------------------------------------------ */
static AssetUID CercaNomeStileDebole(uchar* nomestile) throw()
{
 	AssetUID lIndiceStileChar = 0;

	QXStringRef stileRef = NULL;
	QXStringCreateFromCString((char*) nomestile, 0, (int32) CSTRLEN((char*) nomestile), &stileRef);

	DocID docID = INVAL_DOCID;
	xtget_curdoc(&docID);
	DocRef docRef = XTGetDocRefFromDocID(docID);
	AssetSpaceRef assetSpaceRef = XTGetDocAssetSpace(docRef);
	if (assetSpaceRef == INVAL_ASSETSPACEREF) {
		assetSpaceRef = XTCreateDefaultAssetSpace();
	}

	bool8 found = FALSE;

	int32 index = 0;   
	int32 assetCount = XTGetAssetCount(assetSpaceRef, kAssetCharStyle);   
	for(index = 0; index < assetCount; index++) {   
		QXStringRef  assetNameRef = NULL;   
		AssetUID charAssetUID = INVAL_ASSETUID;   

		APIERR apiErr = XTGetAsset(assetSpaceRef, kAssetCharStyle, index, &charAssetUID);    
		if ( apiErr == ERR_SUCCESS && charAssetUID != INVAL_ASSETUID) { 

			apiErr = XTGetAssetName(assetSpaceRef, kAssetCharStyle, charAssetUID, &assetNameRef);     
			
			QXStringCompareResult cmpResult;   
			QXStringICompare(stileRef, 0, kNoPos, assetNameRef, 0, kNoPos, kQXSCaseCompareDefault, &cmpResult);

			QXStringDestroy(assetNameRef);   

			if ( cmpResult == kQXStringEqualTo ) {
				lIndiceStileChar = (AssetUID) index;
				found = TRUE;
				break;
			}
		}   
	}   

	XTDeleteAssetSpace(assetSpaceRef);

	QXStringDestroy(stileRef);

	if ( !found ) {
		lIndiceStileChar = (AssetUID) -1;

		ConcatenaAllaStringaErrore(nomestile);
		DaiErrore(kErroreStileNonDefinito);
		gErroreSulloStile = TRUE;
	}
	else {
		lIndiceStileChar++;
	}

	return(lIndiceStileChar);

} // CercaNomeStiledebole

/* ------------------------------------------------------------------------ *

	ApplicaStileDeboleSuTesto

* ------------------------------------------------------------------------ */
static void XTAPI ApplicaStileDeboleSuTesto(AssetUID indicestile) throw()
{
/*
 	charstylerec lStile;
	
	if (getcharstylebyindex(indicestile, &lStile) == TRUE) 
	{ 							  								  		
		//xesetattrib(T_FACE, PLAIN);
		//xesetattrib(T_FACE, lStile.t.face);
		//xesetattrib(T_SIZE, lStile.t.size);
		//xesetattrib(T_FONT, lStile.t.font);
		//xesetattrib(T_COLOR, lStile.t.a.color);
		//xesetattrib(T_SHADE, lStile.t.shade);
		//xesetattrib(T_KERN, lStile.t.kern );
		//xesetattrib(T_TRACK, lStile.t.track);
		//xesetattrib(T_BASESHIFT, lStile.t.baseshift);
		//xesetattrib(T_HSCALE,lStile.t.hscale);
		// dovrebbe essere preferibile fare un set generale piuttosto che settare
		// singolarmente gli attributi
		setcharstylebyindex(indicestile, &lStile, TRUE);
	}
/**/

	xehandle lHndlXE = NULL;
	int32 lInizioSelezione = 0;
	int32 lFineSelezione = 0;
	int32 lLunghezzaTesto = 0;

	boxid curbox;
	xtget_curbox(&curbox);
	xegetinfo(curbox, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	xesetsel(lLunghezzaTesto, lLunghezzaTesto, FALSE);

	XTSetCharStyleOnText((AssetUID) -1, FALSE, lHndlXE);
	XTSetCharStyleOnText(indicestile, FALSE, lHndlXE);

} // ApplicaStileDeboleSuTesto

/* ------------------------------------------------------------------------ *

	ApplicaStileForte

* ------------------------------------------------------------------------ */
void XTAPI ApplicaStileForte(uchar *nomestile) throw()
{
	xehandle lHndlXE = NULL;
	int32 lInizioSelezione = 0;
	int32 lFineSelezione = 0;
	int32 lLunghezzaTesto = 0;
	AssetUID lIndiceStile = 0;
	
	// porto il cursore alla fine del flusso di testo corrente
	boxid curbox;
	xtget_curbox(&curbox);
	xegetinfo(curbox, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	xesetsel(lLunghezzaTesto, lLunghezzaTesto, FALSE);

	// cerca lo stile da applicare
	lIndiceStile = CercaNomeStileForte(nomestile);
	
	// applica lo stile in modo forte
	XTSetStyleOnText((AssetUID) -1, TRUE, FALSE, lHndlXE);
	XTSetStyleOnText(lIndiceStile, TRUE, FALSE, lHndlXE);

} // ApplicaStileForte

/* ------------------------------------------------------------------------ *

	ApplicaStileDebole

* ------------------------------------------------------------------------ */
void XTAPI ApplicaStileDebole(uchar *nomestile) throw()
{
	xehandle lHndlXE = NULL;
	int32 lInizioSelezione = 0;
	int32 lFineSelezione = 0;
	int32 lLunghezzaTesto = 0;
	AssetUID lIndiceStile = 0;
	
	// porto il cursore alla fine del flusso di testo corrente
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	xesetsel(lLunghezzaTesto, lLunghezzaTesto, FALSE);
	
	// cerca lo stile da applicare
	lIndiceStile = CercaNomeStileDebole(nomestile);
	
	// applica lo stile in modo debole
	ApplicaStileDeboleSuTesto(lIndiceStile);

} // ApplicaStileDebole

/* ------------------------------------------------------------------------ *

	InizializzaErroreSulloStile

* ------------------------------------------------------------------------ */
void XTAPI InizializzaErroreSulloStile() throw()
{
	gErroreSulloStile = FALSE;
} // InizializzaErroreSulloStile

/* ------------------------------------------------------------------------ *

	ImpostaCaratteristicheFiletto

* ------------------------------------------------------------------------ */
void XTAPI ImpostaCaratteristicheFiletto(uchar *nomestile) throw()
{
	AssetUID lIndiceStile = INVAL_ASSETUID;
 	XTUParaStyleRec lStile; // vecchio stylerec
 	bool8 lVecchioErroreSulloStile = FALSE;
 	
 	// prendo l'impostazione sull'errore sullo stile
 	lVecchioErroreSulloStile = gErroreSulloStile;
 	gErroreSulloStile = TRUE;
	
	// cerca lo stile da applicare
	lIndiceStile = CercaNomeStileForte(nomestile);

	if ( lIndiceStile != INVAL_ASSETUID ) { 
		DocID docID = INVAL_DOCID;
		xtget_curdoc(&docID);
		DocRef docRef = XTGetDocRefFromDocID(docID);
		AssetSpaceRef docAssetSpaceRef = XTGetDocAssetSpace(docRef);
		if (docAssetSpaceRef == INVAL_ASSETSPACEREF) {
			docAssetSpaceRef = XTCreateDefaultAssetSpace();
		}

		zerodata(&lStile, sizeof(XTUParaStyleRec));
		APIERR apiErr = XTUGetParaStyleInfo(docAssetSpaceRef, lIndiceStile, &lStile);
		if ( ERROR_SUCCESS == apiErr ) 
		{
			// controllo che lo stile abbia un filetto superiore e
			// che la lughezza non sia sul testo 
			if (lStile.p.a.ruleabove == TRUE && lStile.p.a.rabovetextlen == FALSE) 
			{							  								  		
				gCaratteristicheFiletto.definito = TRUE;
				gCaratteristicheFiletto.spessore = lStile.p.rabove.width;
				gCaratteristicheFiletto.stile = lStile.p.rabove.style;
				gCaratteristicheFiletto.colore = lStile.p.rabove.qColor.mColorID;
				gCaratteristicheFiletto.intensita = lStile.p.rabove.qColor.mShade;
				gCaratteristicheFiletto.rientroSinistro = lStile.p.rabove.leftindent;
				gCaratteristicheFiletto.rientroDestro = lStile.p.rabove.rightindent;
				return;
			}
		}
	}
	
	// se lo stile non e' definito oppure
	// se lo stile e' definito ma non ha un filetto superiore
	gCaratteristicheFiletto.definito = FALSE;
	gCaratteristicheFiletto.spessore = 0;
	gCaratteristicheFiletto.stile = 0;
	gCaratteristicheFiletto.colore = 0;
	gCaratteristicheFiletto.intensita = 0;
	gCaratteristicheFiletto.rientroSinistro = 0;
	gCaratteristicheFiletto.rientroDestro = 0;
	
	// ripristino l'impostazione sull'errore di stile
	gErroreSulloStile = lVecchioErroreSulloStile;
} // ImpostaCaratteristicheFiletto

