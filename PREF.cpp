/* ------------------------------------------------------------------------ *
	Preferenze.cpp

	25 marzo 2005 - Fabrizio

	Copyright © Sinedita S.r.l 2005. All Rights Reserved.
	
	Descrizione:
	Contiene le procedure per la gestione delle preferenze del sistema.
	Le preferenze vengono visualizzate tramite una dialog. E' anche possibile
	salvare la configurazione ed quindi ricaricarla quando Š necessario.
	In generale alla partenza dell'Xtension vengono caricate le ultime preferenze
	usate nell'ultima sessione di lavoro.

	$Log: not supported by cvs2svn $
	Revision 1.3  2005/04/21 09:48:19  taretto
	tolte mLong per gestire numero moduli e colonne, modificata estensione per salvataggi in QXP
	
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

#include "DbpConsts.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "XTNecro.h"

#include "Pref.h"
#include "ImpaginazioneWap.h"

// GLOBALS
// GLOBALS
// GLOBALS

// dichiaro variabile preferenze e la inizializzo con valori di default 
XtPreferences gXtPreferences =
{
	ImpaginazioneWap::POSAUTORADIOBTNID, // tipo impaginazione
	mFixed(5), // valore iniziale spazio tra necrologi
	TRUE, // abilita spazio tra necrologi
	mFixed(2), // incremento max spazio tra necrologi
	mFixed(1), // decremento max spazio tra necrologi
	TRUE, // abilita spazio tra paragrafi
	mFixed(1), // incremento max spazio tra paragrafi
	mFixed(1), // decremento max spazio tra paragrafi
	TRUE, // abilita interlinea
	 mFixed(1) + (65536 / 2), // incremento max interlinea
	 mFixed(1), // decremento max interlinea
	TRUE, // usa anniversari
	TRUE, // usa ringraziamenti
	TRUE, // abilita importazione immagini
	mFixed(105), // altezza immagini
	mFixed(90), // larghezza immagini
#ifdef _DEBUG
	"C:\\SE\\NECRO\\IMG", // folder immagini
#else
	"\\\\ARCHSRV\\OUT_NECRO", // folder immagini
#endif // _DEBUG
	"IMM", // estensione elenco immagini
	"EPS", // estensione immagini
	STAMPAAGGREGATIRADIOBTNID, // tipo stampa bozza
	FALSE, // stampa bozza automatica
#ifdef _DEBUG
	"C:\\SE\\NECRO\\BOZ.QXP", // documento bozza
	"C:\\SE\\NECRO\\BOZ", // cartella bozze
	"C:\\SE\\NECRO\\ING.QXP", // documento ingombro
	"C:\\SE\\NECRO\\ING", // cartella ingombri
	"C:\\SE\\NECRO\\FIN.QXP", // documento finale
	"C:\\SE\\NECRO\\FIN", // cartella finale
#else
	"\\\\DATASERVER\\SE\\NECRO\\GABBIE\\BOZZAA4_9X.QXP", // documento bozza
	"\\\\DATASERVER\\SE\\NECRO\\BOZZE", // cartella bozze
	"\\\\DATASERVER\\SE\\NECRO\\GABBIE\\INGOMBRO6C_9X.QXP", // documento ingombro
	"\\\\DATASERVER\\SE\\NECRO\\INGOMBRI", // cartella ingombri
	"\\\\DATASERVER\\SE\\NECRO\\GABBIE\\FINALE6C_9X.QXP", // documento finale
	"\\\\DATASERVER\\SE\\NECRO\\DEFINITIVI", // cartella finale
#endif // _DEBUG
	PRIMAPAGINARADIOBTNID, // prepara documento	
	ALTOVERSOBASSORADIOBTNID,	// ordinamento impaginazione
	mFixed(117), // altezza modulo
	mFixed(9), // distanza moduli
	12, // moduli colonna
	8, // colonne pagina
	mFixed(23) + (65536 * 711 / 1000), // altezza testata
#ifdef _DEBUG
	"C:\\SE\\NECRO\\DAT", // cartella dati
#else
	"\\\\DATASERVER\\SE\\NECRO\\DATI", // cartella dati
#endif // _DEBUG
	"BOZ", //  estensione da impaginare 1
	TRUE, // impagina estensione 1
	"ING", //  estensione da impaginare 2
	TRUE, // impagina estensione 2
	"NEC", //  estensione da impaginare 3 
	TRUE, // impagina estensione 3
	VERTICALERADIOBTNID, // ottimizzazione verticale
#ifdef _DEBUG
	"C:\\SE\\NECRO\\TEMP", // folder temporanei
	"C:\\SE\\NECRO\\LASTIMP" // folder last imp
#else
	"\\\\DATASERVER\\SE\\NECRO\\TEMP", // folder temporanei
	"\\\\DATASERVER\\SE\\NECRO\\ULTIMA" // folder last imp
#endif // _DEBUG
};

// EX preferenza interna (kLongIngombroTotale) per ora la uso come globale
extern double gLongIngombroTotale = 0;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			DeleteEndFilePathSeparator
	@abstract 		elimina separatore finale
	@discussion		elimina separatore finale della stringa passata in input se presente -da SmartClass-
	
					30 Marzo 2005 - Taretto Fabrizio.

	@param			ioString
	@result  			nessuno
*/
static void XTAPI DeleteEndFilePathSeparator(uchar* ioString) throw();

/*! 
	@function			GetFileGenerico
	@abstract		apertura file
	@discussion		apre una dialog per selezionare un file e ne restituisce il path completo in nomefile.
					Attenzione: non viene aperto nessun file ma soltanto selezionato.
					Ritorna il nome del file selezionato. -da SmartClass-	

					1 aprile 2005 - Fabrizio.

	@param 			nomefile.
	@param 			directorydipartenza.
	@param 			nomeattualefile.
	@param 			szTitle.
	@param 			spiega_filtro.
	@param 			filtro.
	@result			Ritorna FALSE se il FSSpec passato e' andato tutto ok.
*/
static bool8 XTAPI GetFileGenerico(uchar* nomefile, uchar* directorydipartenza, uchar* nomeattualefile, uchar* szTitle,
								uchar* spiega_filtro, uchar* filtro) throw();

/*!
	@function			SelezionaFileDialog
	@abstract		FSSpec
	@abstract		seleziona file
	@discussion		apre una dialog per selezionare un file e ne restituisce il path completo in nomefile.
					Attenzione: non viene aperto nessun file ma soltanto selezionato.
					Ritorna il nome del file selezionato. -da SmartClass-	

					1 aprile 2005 - Fabrizio.

	@param 			percorsoCompleto.
	@result			Ritorna TRUE se il FSSpec passato è andato tutto ok.
*/
static bool8 XTAPI SelezionaFileDialog(uchar* iPercorsoCompleto) throw();

/*!
	@function			MyPickFolder
	@abstract 		pickfolder
	@discussion		Funzione per l'utilizzo corretto della chiamatapickfolder di XPress su MacOs e WinOS -da SmartClass-
	
					30 Marzo 2005 - Taretto Fabrizio.

	@param			iTitle titolo della finestra pickfolder.
	@param			ioPath path di partenza in input, path di ritorno in output.
	@result  			TRUE se e' andato tutto bene, FALSE in tutti i casi di errore
*/
static bool8 XTAPI MyPickFolder(uchar* iTitle, uchar* ioPath) throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	DeleteEndFilePathSeparator

* ------------------------------------------------------------------------ */
static void XTAPI DeleteEndFilePathSeparator(uchar* ioString) throw()
{
	assert(ioString != NULL);
	
	if (STRLEN(ioString) > 0)
	{
		std::string stringTmpString((char*)(ioString));
		int16 strLength = (int16) stringTmpString.length();
		if (stringTmpString.at(strLength - 1) == kDirSeparatorChar)
		{
			stringTmpString.erase((strLength - 1), strLength);
		}
	
		CSTRCPY(ioString, stringTmpString.c_str());
	}
} // DeleteEndFilePathSeparator

/* ------------------------------------------------------------------------ *

	GetFileGenerico

* ------------------------------------------------------------------------ */
static bool8 XTAPI GetFileGenerico(uchar* nomefile, uchar* directorydipartenza, uchar* nomeattualefile, 
					  uchar* szTitle, uchar* spiega_filtro, uchar* filtro) throw()
{
	bool8 result=FALSE;

	//OPENFILENAME ofn;
	//uchar szDirName[256];
	//uchar szFile[256], szFileTitle[256];
	//UINT  i, cbString;
	//uchar  chReplace;    // string separator for szFilter
	//uchar  szFilter[256];
	//uchar	lFILTERSTRING[256];

	//// Get the system directory name, and store in szDirName 
	//STRCPY(lFILTERSTRING, spiega_filtro);
	//STRCAT(lFILTERSTRING,"|");
	//STRCAT(lFILTERSTRING,filtro);
	//STRCAT(lFILTERSTRING,"|");
	//STRCPY(szFilter,lFILTERSTRING);
	//cbString = STRLEN(lFILTERSTRING);
	//chReplace = szFilter[cbString - 1]; // retrieve wildcard

	//for (i = 0; szFilter[i] != '\0'; i++) {
	// if (szFilter[i] == chReplace)
	//	 szFilter[i] = '\0';
	//}

	//// Set all structure members to zero.
	//STRCPY(szFile,nomeattualefile);
	//STRCPY(szDirName,directorydipartenza);
	//// ho settato la directory da cui partire
	//memset(&ofn, 0, sizeof(OPENFILENAME));
	//
	//// prendo puntatore alla dialog corrente
	//DialogPtr dlgPtr;
	//xtget_frontmost(&dlgPtr); 

	//ofn.lStructSize = sizeof(OPENFILENAME);
	//ofn.hwndOwner = dlgPtr;
	//ofn.lpstrFilter = (char*)szFilter;
	//ofn.nFilterIndex = 1;
	//ofn.lpstrFile = (char*)szFile;
	//ofn.nMaxFile = sizeof(szFile);
	//ofn.lpstrFileTitle = (char*)szFileTitle;
	//ofn.nMaxFileTitle = sizeof(szFileTitle);
	//ofn.lpstrInitialDir = (char*)szDirName;
	//ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	//
	//bool8 OpenErr = GetOpenFileName(&ofn);
	//
	//if (OpenErr)
	//{
	//	STRCPY(nomefile, ofn.lpstrFile);
	//	result= FALSE;
	//}
	//else
	//{
	//	// e' successo qualche casino o qualcosa non e' andato bene
	//	result=TRUE;
	//}

	//return (result);

	static FSSpec sCurDirFSSpec; // Qui va messa la cartella di partenza
    OSErr err = noErr;
    
    uchar getFileName[256] = "\0";
    strcpy((char *)getFileName, (char*) directorydipartenza);
    strcat((char *)getFileName, (char*) nomeattualefile);
    
    memset(&sCurDirFSSpec, '\0', sizeof(FSSpec));
    FSMakeFSSpec(0, 0L, directorydipartenza, &sCurDirFSSpec);
    
    XFileInfoRef fileInfo = NULL;
    XTCreateEmptyXFileInfoRef(&fileInfo);
    
    XFileInfoRef startLocationFileInfo = NULL;
    XTCreateEmptyXFileInfoRef(&startLocationFileInfo);
    XTSetFSSpecInXFileInfoRef(startLocationFileInfo, &sCurDirFSSpec);
    
    LPEXTUFILEARRAY typeList;
    LPEXTUFILETYPE  ptrList;
    typeList = (LPEXTUFILEARRAY) NewPtrClear( 2*sizeof(LPEXTUFILETYPE) );
    typeList[ 0 ] = ptrList = (LPEXTUFILETYPE) NewPtrClear(sizeof(EXTUFILETYPE) );
    typeList[ 1 ] = 0L;
    
    QXStringRef titleRef = NULL;
    QXStringCreateFromCString("Nekro Document", 0,
                                  (int32) CSTRLEN("Nekro Document"), &titleRef);
    
    QXStringRef tempStrRef = NULL;
    QXStringCreateFromCString("Nekro Document", 0,
                                  (int32) CSTRLEN("Nekro Document"), &tempStrRef);
    
    QXStringExtractIntoUniChars(tempStrRef, 0, kNoPos, &(ptrList->tname[0]), 0);
    ptrList->tdata[ 0 ].ftype = PROJTYPE;
    ptrList->tdatacount = 1;
    
    err = XTUGetFileUsingXD(fileInfo, startLocationFileInfo, titleRef, titleRef, NULL, NULL, typeList);
    
    DisposePtr((::Ptr) ptrList );
    DisposePtr((::Ptr) typeList);

	QXStringDestroy(titleRef);
    QXStringDestroy(tempStrRef);

    XTDisposeXFileInfoRef(startLocationFileInfo);

	int32 navResponse = kInvalidState;
    XTGetNavResponseFromXFileInfoRef(fileInfo, &navResponse);

    if ( kUserPressedOk == navResponse )
    {
        FSSpec fileSpec;
        XTGetFSSpecFromXFileInfoRef(fileInfo,&fileSpec);
        
        QXStringRef fileNameRef = NULL;
        QXStringRef fullPathRef = NULL;
        QXStringCreateFromLocalString(fileSpec.name, 0, STRLEN(fileSpec.name), &fileNameRef);
        XTUGetFullPathEX(fileSpec.vRefNum, fileSpec.parID, fileNameRef, &fullPathRef);
        
        uchar outFullPath[kMaxLongFileName];
        QXString2CStr(fullPathRef, (char*) outFullPath);
        
        QXStringDestroy(fileNameRef);
        QXStringDestroy(fullPathRef);
        
        XTDisposeXFileInfoRef(fileInfo);
        fileInfo=NULL;
        
        strcpy((char*) nomefile, (char *)outFullPath);
        result = TRUE;
    }
    else {
        XTDisposeXFileInfoRef(fileInfo);
        result = FALSE;
    }

	return(result);

} // GetFileGenerico 

/* ------------------------------------------------------------------------ *

	SelezionaFileDialog

* ------------------------------------------------------------------------ */
static bool8 XTAPI SelezionaFileDialog(uchar* iPercorsoCompleto) throw()
{
	
	uchar 		lnomefile[MAXPATHNAME] = "",
				lStringaPerControlli[MAXPATHNAME] = "",
				nomeFile[12] = "",
				nomewd[MAXPATHNAME] = "";
	int16		llunghezzaStringa = 0,
				linizio = 0,
				i=0,
				k=0;
	bool8		lrisultato=FALSE;

	// parsing per il riconoscimento del nome del file e del relativo percorso utili per impostare
	// la working directory nella prossima dialog
	// controllo se e' gia stato selezionato qualcosa
	if (STRCMP(iPercorsoCompleto,  (uchar*)"nessun documento selezionato"))
	{
		STRCPY(lStringaPerControlli, iPercorsoCompleto);
		llunghezzaStringa = STRLEN(iPercorsoCompleto);
		i = llunghezzaStringa;
			
		while(!(lStringaPerControlli[i] == kWinDirSeparatorChar))
		{
			// incremento fino a fermarmi a quello giusto
			i--;
		}
		
		linizio=i;
		// formulo la stringa con la wd
		for(i=0;i<=linizio;i++)
		{
			nomewd[k] = lStringaPerControlli[i];
			k++;
			nomeFile[k] = '\0';
		}
		nomewd[k] = '\0';
		// formulo la stringa con il nome del file
		k=0;
		for(i=linizio+1;i<=llunghezzaStringa;i++)
		{
			nomeFile[k] = lStringaPerControlli[i];
			k++;
		}
		nomeFile[k] = '\0';
	
	}// fine del controllo
	else
	{
   		// non c'era percorso prima
		// STRCPY(nomewd, DBP::PPP((char*)(**gHndlPreferenze).cartellaBaseEdizioni));
		// STRCAT(nomewd, "\p..");
	}
	
	// fine del Parsing
	lrisultato =  GetFileGenerico((uchar*)lnomefile
									,(uchar*)nomewd
									,(uchar*)nomeFile
									,(uchar*)"Apri"
									,(uchar*)"QXP File"
									,(uchar*)"*.qxp*");
	
	// se tutto ok prendo il pathname del file selezionato
	if(lrisultato == noErr)
	{
		// copio il percorso altrimenti non faccio nulla
		STRCPY(iPercorsoCompleto, lnomefile);
		return(noErr);
	}
	else
	{
		// c'e' stato un qualche errore nella selezione dei file
		return(-1);
	}	
	
	// gestione errore
	return(noErr);
	
} // SelezionaFileDialog

/* ------------------------------------------------------------------------ *

	MyPickFolder

* ------------------------------------------------------------------------ */
static bool8 XTAPI MyPickFolder
(
	uchar* iTitle, 
	uchar* ioPath
) throw()
{
	int16 vol; 
	int32 dirId; 
	bool8 myPickFolder = FALSE;
	
    QXStringRef titleRef = NULL;
    QXStringCreateFromCString((char*) iTitle, 0,
                                  (int32) CSTRLEN((char*) iTitle), &titleRef);
    
    QXStringRef pathRef = NULL;
    QXStringCreateFromCString((char*) ioPath, 0,
                                  (int32) CSTRLEN((char*) ioPath), &pathRef);
 #if WINOS
	int16 pickFolderErr = XTUPickFolderUsingXD(titleRef, pathRef, NULL, &vol, &dirId, NULL, NULL); 
#endif // WINOS
#if MACOS
	int16 pickFolderErr = PickFolderUsingXD(iTitle, ioPath, NULL, &vol, &dirId, NULL);
#endif // MACOS
	
	if (pickFolderErr == noErr)
	{	
		QXStringRef inputFileNameRef;
		QXStringCreateFromCString("Dummy.qxp", 0, (int32) CSTRLEN("Dummy.qxp"), &inputFileNameRef);

		QXMutableStringRef outputFullPathRef = NULL;
		QXMutableStringCreateEmpty(&outputFullPathRef);

		// per prendere il pathname corretto del folder selezionato
		// concateno un file fittizio (Dummy.qxp)
		XTUGetFullPath(vol, dirId, inputFileNameRef, outputFullPathRef);

		QXStringDestroy(inputFileNameRef);

		// elimino il file dummy
		//extractpathname(ioPath);

		QXStringRef pathRef = NULL;
		XTUExtractPathName((QXStringRef) outputFullPathRef, &pathRef);
		
		QXMutableStringDestroy(outputFullPathRef);

		QXString2CStr(pathRef, (char*) ioPath);

		QXStringDestroy(pathRef);

		DeleteEndFilePathSeparator(ioPath);
		myPickFolder = TRUE;
	}	
	else
		myPickFolder = FALSE;
	
	QXStringDestroy(titleRef);
	QXStringDestroy(pathRef);

	return(myPickFolder);

} // MyPickFolder

/* ------------------------------------------------------------------------ *

	RiempitiviWap
	
* ------------------------------------------------------------------------ */
int32 XTAPI RiempitiviWap(xdwapparamptr params) throw()
{
	static XtPreferencesPtr xtPrefPtr = NULL;
	xdlgsetupptr dlgSetUp;
	
	params->result = XDR_HANDLED;
	
	switch(params->opcode) 
	{
		case XDM_DIALOGSETUP:
		{
			dlgSetUp = (xdlgsetupptr) params->param1;
			xtPrefPtr = (XtPreferencesPtr) params->param2;
			dlgSetUp->dlgresid = RIEMPITIVIDIALOGID;
			
			break;
		}
		
		case XDM_DECLARATIONS:
		{
			// dichiaro check box
			xd_btnchk_declare(USAANNIVERSARICHECKID, 0);
			xd_btnchk_declare(USARINGRAZIAMENTICHECKID, 0);
			
			break;
		}
		
		case XDM_TABSHOWING:
		{
			// stato check box
			xd_btnchk_setstate(USAANNIVERSARICHECKID, (*xtPrefPtr).riempitiviPref.usaAnniversari);
			xd_btnchk_setstate(USARINGRAZIAMENTICHECKID, (*xtPrefPtr).riempitiviPref.usaRingraziamenti);
			
			params->result = XDR_NOTHANDLED;
			
			break;
		}
		
		case XDM_LOADCONTROLS:
		{	
			params->result = XDR_HANDLED;			
			break;
		}
		
		case XDM_VERIFYCONTROLS:
		{	
			// prendo valore check box
			xd_btnchk_getstate(USAANNIVERSARICHECKID, &(*xtPrefPtr).riempitiviPref.usaAnniversari);
			xd_btnchk_getstate(USARINGRAZIAMENTICHECKID, &(*xtPrefPtr).riempitiviPref.usaRingraziamenti);
			
			break;
		}
		
		default:
		{
			params->result = XDR_NOTHANDLED;
			break;
		}
	}
	
	return(noErr);
} // RiempitiviWap

/* ------------------------------------------------------------------------ *

	ImmaginiWap
	
* ------------------------------------------------------------------------ */
int32 XTAPI ImmaginiWap(xdwapparamptr params) throw()
{
	static XtPreferencesPtr xtPrefPtr = NULL;
	xdlgsetupptr dlgSetUp;
	
	params->result = XDR_HANDLED;
	
	switch(params->opcode) 
	{
		case XDM_DIALOGSETUP:
		{
			dlgSetUp = (xdlgsetupptr) params->param1;
			xtPrefPtr = (XtPreferencesPtr) params->param2;
			dlgSetUp->dlgresid = IMMAGINIDIALOGID;
			
			break;
		}
		
		case XDM_DECLARATIONS:
		{
			// dichiaro check box
			xd_btnchk_declare(IMPORTAZIONEIMGCHECKID, 0);
			
			// dichiaro edit text per contenere dimensione immagine
			xd_edt_declare(ALTEZZAIMGEDTID, XEDTF_STANDARD, ALLOK, VDEFUNITS, FIX(0), FIX(999999), VALUERANGE);
			xd_edt_declare(LARGHEZZAIMGEDTID, XEDTF_STANDARD, ALLOK, VDEFUNITS, FIX(0), FIX(999999), VALUERANGE);
			
			// edit text per contenere path cartella immagine
			xd_edt_declare(FOLDERIMGEDTID, XEDTF_READONLY | XEDTF_MULTILINE, ALLOK, STRINGS, NULL, NULL, NULL);
			
			// bottone sfoglia
			xd_btnpsh_declare(SFOGLIAIMGBTNID, FALSE, CHSIM_NONE);
			
			// edit text
			xd_edt_declare(ESTENSIONEELENCOEDTID, XEDTF_STANDARD, ALLOK, STRINGS, NULL, NULL, NULL);
			xd_edt_declare(ESTENSIONEIMGEDTID, XEDTF_STANDARD, ALLOK, STRINGS, NULL, NULL, NULL);
			
			break;
		}
		
		case XDM_TABSHOWING:
		{
			// setto l'edit text con il fullpath del doc bozza
			xd_edt_set(FOLDERIMGEDTID, (*xtPrefPtr).immaginiPref.folderImg, NULL );
			
			// stato check box
			xd_btnchk_setstate(IMPORTAZIONEIMGCHECKID, (*xtPrefPtr).immaginiPref.abilitaImportImag);
			
			// edit per contenere dimensioni immagini
			xd_edt_set(ALTEZZAIMGEDTID, NULL, (*xtPrefPtr).immaginiPref.altezzaImg);
			xd_edt_set(LARGHEZZAIMGEDTID, NULL, (*xtPrefPtr).immaginiPref.larghezzaImg);
			
			// edit per estensioni
			xd_edt_set(ESTENSIONEELENCOEDTID, (*xtPrefPtr).immaginiPref.estensioneElenco, NULL);
			xd_edt_set(ESTENSIONEIMGEDTID, (*xtPrefPtr).immaginiPref.estensioneImg, NULL);
			
			params->result = XDR_NOTHANDLED;
			
			break;
		}
		
		case XDM_LOADCONTROLS:
		{	
			params->result = XDR_HANDLED;			
			break;
		}
		
		case XDM_USERACTION:
		{
			switch (params->itemid)
			{
				case SFOGLIAIMGBTNID:
				{
					// variabile per gestione pickfolder
					uchar thePath[MAXPATHNAME];
					
					// prende il percorso preesistente e glielo passa in modo che la wd sia già ben settata
					uchar cartellaImgSelezionata[MAXPATHNAME] = "";
					xd_edt_get(FOLDERIMGEDTID, cartellaImgSelezionata, NULL, NULL);	
					
					// gestisco la dialog di seleziona directory
					if (STRLEN(cartellaImgSelezionata) > 0)	
						STRCPY(thePath, cartellaImgSelezionata);
					else
						STRCPY(thePath, (*xtPrefPtr).immaginiPref.folderImg);
						
					// apro finestra per selezione cartella		
					bool8 pickFolderOk = MyPickFolder((uchar*)"Seleziona Cartella Immagini:",  thePath);
					
					if (pickFolderOk == TRUE)
					{
						// setto l'edit textcon il nome della cartella edizione appena selezionata
						STRCAT(thePath, kDirSeparatorChar);
						xd_edt_set(FOLDERIMGEDTID, thePath, 0); 	
						
						STRCPY((*xtPrefPtr).immaginiPref.folderImg, thePath);
					}
					else
					{
						break;
					}			
				}
			}
		}
		
		case XDM_VERIFYCONTROLS:
		{	
			// prendo valore edt box
			xd_edt_get(ESTENSIONEELENCOEDTID, (*xtPrefPtr).immaginiPref.estensioneElenco, NULL, NULL);
			xd_edt_get(ESTENSIONEIMGEDTID, (*xtPrefPtr).immaginiPref.estensioneImg, NULL, NULL);
			
			// prendo valore check box
			xd_btnchk_getstate(IMPORTAZIONEIMGCHECKID, &(*xtPrefPtr).immaginiPref.abilitaImportImag);
			
			// variabile d'utilità per le stringhe
			uchar tmpStr[MAXPATHNAME] = "";	
			
			// prendo nuove misure
			xd_edt_get(ALTEZZAIMGEDTID, tmpStr, &(*xtPrefPtr).immaginiPref.altezzaImg, NULL);
			xd_edt_get(LARGHEZZAIMGEDTID, tmpStr, &(*xtPrefPtr).immaginiPref.larghezzaImg, NULL);
			
			break;
		}
		
		default:
		{
			params->result = XDR_NOTHANDLED;
			break;
		}
	}
	
	return(noErr);
} // ImmaginiWap

/* ------------------------------------------------------------------------ *

	BozzaWap
	
* ------------------------------------------------------------------------ */
int32 XTAPI BozzaWap(xdwapparamptr params) throw()
{
	static XtPreferencesPtr xtPrefPtr = NULL;
	xdlgsetupptr dlgSetUp;
	
	params->result = XDR_HANDLED;
	
	switch(params->opcode) 
	{
		case XDM_DIALOGSETUP:
		{
			dlgSetUp = (xdlgsetupptr) params->param1;
			xtPrefPtr = (XtPreferencesPtr) params->param2;
			dlgSetUp->dlgresid = BOZZADIALOGID;
			
			break;
		}
		
		case XDM_DECLARATIONS:
		{
			// dichiaro check box
			xd_btnchk_declare(ABILITASTAMPAAUTOCHECKID, 0);
			
			// dichiaro gruppo radio bottoni
			xd_btnrad_declare(STAMPASINGOLARADIOBTNID, STAMPAAGGREGATIRADIOBTNID, 0);
			
			// edit text per path doc bozza
			xd_edt_declare(DOCBOZZAEDTID, XEDTF_READONLY | XEDTF_MULTILINE, ALLOK, STRINGS, NULL, NULL, NULL);
			
			// bottone
			xd_btnpsh_declare(SFOGLIADOCBOZZABTNID, FALSE, CHSIM_NONE);
			
			// edit per path folder bozze
			xd_edt_declare(FOLDERBOZZEEDTID, XEDTF_READONLY | XEDTF_MULTILINE, ALLOK, STRINGS, NULL, NULL, NULL);
			
			// bottone
			xd_btnpsh_declare(SFOGLIAFOLDERBOZZEBTNID, FALSE, CHSIM_NONE);
			
			break;
		}
		
		case XDM_TABSHOWING:
		{
			// setto l'edit text con il fullpath del doc bozza
			xd_edt_set(DOCBOZZAEDTID, (*xtPrefPtr).bozzaPref.docPathBozza, NULL );
			xd_edt_set(FOLDERBOZZEEDTID, (*xtPrefPtr).bozzaPref.folderPathBozze, NULL );
			
			// setto stato radio bottoni
			xd_btnrad_setstate((*xtPrefPtr).bozzaPref.tipoStampaBozza);
			
			// stato check box
			xd_btnchk_setstate(ABILITASTAMPAAUTOCHECKID, (*xtPrefPtr).bozzaPref.stampaAutomatica);
			
			params->result = XDR_NOTHANDLED;
			
			break;
		}
		
		case XDM_LOADCONTROLS:
		{	
			params->result = XDR_HANDLED;			
			break;
		}
		
		case XDM_USERACTION:
		{
			switch (params->itemid)
			{
				case SFOGLIADOCBOZZABTNID:
				{
					int32 error = noErr;
					uchar filePathTmp[MAXPATHNAME] = "";
					
					// prende il percorso preesistente e glielo passa in modo che la wd sia già ben settata
					xd_edt_get(DOCBOZZAEDTID, filePathTmp, NULL, NULL);									
					error = SelezionaFileDialog(filePathTmp);
					
					// testo nuovo valore edit text
					xd_edt_set(DOCBOZZAEDTID, filePathTmp, 0);
					STRCPY((*xtPrefPtr).bozzaPref.docPathBozza, filePathTmp);
					
					break;	
				}
				
				case SFOGLIAFOLDERBOZZEBTNID:
				{
					// variabile per gestione pickfolder
					uchar thePath[MAXPATHNAME];
					
					// prende il percorso preesistente e glielo passa in modo che la wd sia già ben settata
					uchar cartellaBozzeSelezionata[MAXPATHNAME] = "";
					xd_edt_get(FOLDERBOZZEEDTID, cartellaBozzeSelezionata, NULL, NULL);	
					
					// gestisco la dialog di seleziona directory
					if (STRLEN(cartellaBozzeSelezionata) > 0)	
						STRCPY(thePath, cartellaBozzeSelezionata);
					else
						STRCPY(thePath, (*xtPrefPtr).bozzaPref.folderPathBozze);
						
					// apro finestra per selezione cartella		
					bool8 pickFolderOk = MyPickFolder((uchar*)"Seleziona Cartella Bozze:",  thePath);
					
					if (pickFolderOk == TRUE)
					{
						// setto l'edit textcon il nome della cartella edizione appena selezionata
						STRCAT(thePath, kDirSeparatorChar);
						xd_edt_set(FOLDERBOZZEEDTID, thePath, 0); 	
						
						STRCPY((*xtPrefPtr).bozzaPref.folderPathBozze, thePath);
					}
					else
					{
						break;
					}
					
					break;	
				}
			}
		}
		
		case XDM_VERIFYCONTROLS:
		{
			// prendo stato dei radio bottoni 
			xd_btnrad_getstate(STAMPASINGOLARADIOBTNID, &((*xtPrefPtr).bozzaPref.tipoStampaBozza));
			
			// prendo stato check box
			xd_btnchk_getstate(ABILITASTAMPAAUTOCHECKID, &(*xtPrefPtr).bozzaPref.stampaAutomatica);
			
			break;
		}
		
		default:
		{
			params->result = XDR_NOTHANDLED;
			break;
		}
	}
	
	return(noErr);
} // BozzaWap

/* ------------------------------------------------------------------------ *

	IngombroWap
	
* ------------------------------------------------------------------------ */
int32 XTAPI IngombroWap(xdwapparamptr params) throw()
{
	static XtPreferencesPtr xtPrefPtr = NULL;
	xdlgsetupptr dlgSetUp;
	
	params->result = XDR_HANDLED;
	
	switch(params->opcode) 
	{
		case XDM_DIALOGSETUP:
		{
			dlgSetUp = (xdlgsetupptr) params->param1;
			xtPrefPtr = (XtPreferencesPtr) params->param2;
			dlgSetUp->dlgresid = INGOMBRODIALOGID;
			
			break;
		}
		
		case XDM_DECLARATIONS:
		{
			// edit text per path doc ingombro
			xd_edt_declare(DOCINGOMBROEDTID, XEDTF_READONLY | XEDTF_MULTILINE, ALLOK, STRINGS, NULL, NULL, NULL);
			
			// bottone
			xd_btnpsh_declare(SFOGLIADOCINGOMBROBTNID, FALSE, CHSIM_NONE);
			
			// edit text per path doc ingombro
			xd_edt_declare(FOLDERINGOMBRIEDTID, XEDTF_READONLY | XEDTF_MULTILINE, ALLOK, STRINGS, NULL, NULL, NULL);
			
			// bottone
			xd_btnpsh_declare(SFOGLIAFOLDERINGOMBRIBTNID, FALSE, CHSIM_NONE);
				
			break;
		}
		
		case XDM_TABSHOWING:
		{
			// setto l'edit text con il fullpath del folder ingombri
			xd_edt_set(DOCINGOMBROEDTID, (*xtPrefPtr).ingombroPref.docPathIngombro, NULL );
			
			// setto l'edit text con il fullpath della cartella ingombri
			xd_edt_set(FOLDERINGOMBRIEDTID, (*xtPrefPtr).ingombroPref.folderPathIngombri, NULL );
			
			params->result = XDR_NOTHANDLED;
			
			break;
		}
		
		case XDM_LOADCONTROLS:
		{	
			params->result = XDR_HANDLED;			
			break;
		}
		
		case XDM_USERACTION:
		{
			switch (params->itemid)
			{
				case SFOGLIADOCINGOMBROBTNID:
				{
					int32 error = noErr;
					uchar filePathTmp[MAXPATHNAME] = "";
					
					// prende il percorso preesistente e glielo passa in modo che la wd sia già ben settata
					xd_edt_get(DOCINGOMBROEDTID, filePathTmp, NULL, NULL);									
					error = SelezionaFileDialog(filePathTmp);
					
					// testo nuovo valore edit text
					xd_edt_set(DOCINGOMBROEDTID, filePathTmp, 0);
					STRCPY((*xtPrefPtr).ingombroPref.docPathIngombro, filePathTmp);
				
					break;
				}
				
				case SFOGLIAFOLDERINGOMBRIBTNID:
				{
					// variabile per gestione pickfolder
					uchar thePath[MAXPATHNAME];
					
					// prende il percorso preesistente e glielo passa in modo che la wd sia già ben settata
					uchar cartellaIngombriSelezionata[MAXPATHNAME] = "";
					xd_edt_get(FOLDERINGOMBRIEDTID, cartellaIngombriSelezionata, NULL, NULL);	
					
					// gestisco la dialog di seleziona directory
					if (STRLEN(cartellaIngombriSelezionata) > 0)	
						STRCPY(thePath, cartellaIngombriSelezionata);
					else
						STRCPY(thePath, (*xtPrefPtr).ingombroPref.folderPathIngombri);
						
					// apro finestra per selezione cartella		
					bool8 pickFolderOk = MyPickFolder((uchar*)"Seleziona Cartella Ingombri:",  thePath);
					
					if (pickFolderOk == TRUE)
					{
						// setto l'edit textcon il nome della cartella edizione appena selezionata
						STRCAT(thePath, kDirSeparatorChar);
						xd_edt_set(FOLDERINGOMBRIEDTID, thePath, 0); 	
						
						STRCPY((*xtPrefPtr).ingombroPref.folderPathIngombri, thePath);
					}
					else
					{
						break;
					}
					
					break;	
				}
			}
		}
		
		default:
		{
			params->result = XDR_NOTHANDLED;
			break;
		}
	}
	
	return(noErr);
} // IngombroWap

/* ------------------------------------------------------------------------ *

	FinaleWap
	
* ------------------------------------------------------------------------ */
int32 XTAPI FinaleWap(xdwapparamptr params) throw()
{
	static XtPreferencesPtr xtPrefPtr = NULL;
	xdlgsetupptr dlgSetUp;
	
	params->result = XDR_HANDLED;
	
	switch(params->opcode) 
	{
		case XDM_DIALOGSETUP:
		{
			dlgSetUp = (xdlgsetupptr) params->param1;
			xtPrefPtr = (XtPreferencesPtr) params->param2;
			dlgSetUp->dlgresid = FINALEDIALOGID;
			
			break;
		}
		
		case XDM_DECLARATIONS:
		{
			// edit text per path doc finale
			xd_edt_declare(DOCFINALEEDTID, XEDTF_READONLY | XEDTF_MULTILINE, ALLOK, STRINGS, NULL, NULL, NULL);
			
			// bottone
			xd_btnpsh_declare(SFOGLIADOCFINALEBTNID, FALSE, CHSIM_NONE);
			
			// 2 gruppi di radio bottoni 
			xd_btnrad_declare(PRIMAPAGINARADIOBTNID, ULTIMAPAGINARADIOBTNID, 0);
			xd_btnrad_declare(ALTOVERSOBASSORADIOBTNID, BASSOVERSOALTOTADIOBTNID, 0);
			
			// edit text per path doc finale
			xd_edt_declare(FOLDERFINALEEDTID, XEDTF_READONLY | XEDTF_MULTILINE, ALLOK, STRINGS, NULL, NULL, NULL);
			
			// bottone
			xd_btnpsh_declare(SFOGLIAFOLDERFINALEBTNID, FALSE, CHSIM_NONE);
			
			
			break;
		}
		
		case XDM_TABSHOWING:
		{
			// setto l'edit text con il fullpath del doc definitivo
			xd_edt_set(DOCFINALEEDTID, (*xtPrefPtr).finalePref.docPathFinale, NULL );
			
			// setto l'edit text con il fullpath del folder definitivo
			xd_edt_set(FOLDERFINALEEDTID, (*xtPrefPtr).finalePref.folderPathFinale, NULL );
			
			// setto i 2 gruppi di radio bottoni
			xd_btnrad_setstate((*xtPrefPtr).finalePref.preparaDocumento);
			xd_btnrad_setstate((*xtPrefPtr).finalePref.posizionamentoIngombri);
			
			params->result = XDR_NOTHANDLED;
			
			break;
		}
		
		case XDM_LOADCONTROLS:
		{	
			params->result = XDR_HANDLED;			
			break;
		}
		
		case XDM_USERACTION:
		{
			switch (params->itemid)
			{
				case SFOGLIADOCFINALEBTNID:
				{
					int32 error = noErr;
					uchar filePathTmp[MAXPATHNAME] = "";
					
					// prende il percorso preesistente e glielo passa in modo che la wd sia già ben settata
					xd_edt_get(DOCFINALEEDTID, filePathTmp, NULL, NULL);									
					error = SelezionaFileDialog(filePathTmp);
					
					// testo nuovo valore edit text
					xd_edt_set(DOCFINALEEDTID, filePathTmp, 0);
					STRCPY((*xtPrefPtr).finalePref.docPathFinale, filePathTmp);	
					
					break;
				}
				
				case SFOGLIAFOLDERFINALEBTNID:
				{
					// variabile per gestione pickfolder
					uchar thePath[MAXPATHNAME];
					
					// prende il percorso preesistente e glielo passa in modo che la wd sia già ben settata
					uchar cartellaFinaleSelezionata[MAXPATHNAME] = "";
					xd_edt_get(FOLDERFINALEEDTID, cartellaFinaleSelezionata, NULL, NULL);	
					
					// gestisco la dialog di seleziona directory
					if (STRLEN(cartellaFinaleSelezionata) > 0)	
						STRCPY(thePath, cartellaFinaleSelezionata);
					else
						STRCPY(thePath, (*xtPrefPtr).finalePref.folderPathFinale);
						
					// apro finestra per selezione cartella		
					bool8 pickFolderOk = MyPickFolder((uchar*)"Seleziona Cartella Definitivi:",  thePath);
					
					if (pickFolderOk == TRUE)
					{
						// setto l'edit textcon il nome della cartella edizione appena selezionata
						STRCAT(thePath, kDirSeparatorChar);
						xd_edt_set(FOLDERFINALEEDTID, thePath, 0); 	
						
						STRCPY((*xtPrefPtr).finalePref.folderPathFinale, thePath);
					}
					else
					{
						break;
					}
					
					break;	
				}
			}
		}
		
		case XDM_VERIFYCONTROLS:
		{
			// prendo stato dei radio bottoni 
			xd_btnrad_getstate(PRIMAPAGINARADIOBTNID, &((*xtPrefPtr).finalePref.preparaDocumento));
			xd_btnrad_getstate(ALTOVERSOBASSORADIOBTNID, &((*xtPrefPtr).finalePref.posizionamentoIngombri));
			
			break;
		}
		
		default:
		{
			params->result = XDR_NOTHANDLED;
			break;
		}
	}
	
	return(noErr);
} // FinaleWap

/* ------------------------------------------------------------------------ *

	DocumentoWap
	
* ------------------------------------------------------------------------ */
int32 XTAPI DocumentoWap(xdwapparamptr params) throw()
{
	static XtPreferencesPtr xtPrefPtr = NULL;
	xdlgsetupptr dlgSetUp;
	
	// variabile d'utilità per le stringhe
	uchar tmpStr[MAXPATHNAME] = "";
	
	params->result = XDR_HANDLED;
	
	switch(params->opcode) 
	{
		case XDM_DIALOGSETUP:
		{
			dlgSetUp = (xdlgsetupptr) params->param1;
			xtPrefPtr = (XtPreferencesPtr) params->param2;
			dlgSetUp->dlgresid = DOCUMENTODIALOGID;
			
			break;
		}
		
		case XDM_DECLARATIONS:
		{
			// dichiaro edit text per misure documento
			xd_edt_declare(ALTEZZAMODULOEDTID, XEDTF_STANDARD, ALLOK, VDEFUNITS, FIX(0), FIX(999999), VALUERANGE);
			xd_edt_declare(DISTANZAMODULIEDTID, XEDTF_STANDARD, ALLOK, VDEFUNITS, FIX(0), FIX(999999), VALUERANGE);
			xd_edt_declare(MODULICOLANNAEDTID, XEDTF_STANDARD, DIGITSOK,  STRINGS, NULL, NULL, NULL);
			xd_edt_declare(COLONNEPAGINAEDTID, XEDTF_STANDARD, DIGITSOK,  STRINGS, NULL, NULL, NULL);
			xd_edt_declare(ALTEZZATESTATAEDTID, XEDTF_STANDARD, ALLOK, VDEFUNITS, FIX(0), FIX(999999), VALUERANGE);
		
			break;
		}
		
		case XDM_TABSHOWING:
		{	
			// setto l'edit text con il fullpath della cartella dati
			xd_edt_set(ALTEZZAMODULOEDTID, NULL, (*xtPrefPtr).documentoPref.altezzaModulo);
			xd_edt_set(DISTANZAMODULIEDTID, NULL, (*xtPrefPtr).documentoPref.distanzaModuli);
			
			NumToPlatformString((*xtPrefPtr).documentoPref.moduliColonna,  tmpStr);
			xd_edt_set(MODULICOLANNAEDTID, tmpStr, NULL);
			
			NumToPlatformString((*xtPrefPtr).documentoPref.colonnePagina,  tmpStr);
			xd_edt_set(COLONNEPAGINAEDTID, tmpStr, NULL);
			
			xd_edt_set(ALTEZZATESTATAEDTID, NULL, (*xtPrefPtr).documentoPref.altezzaTestata);
			
			params->result = XDR_NOTHANDLED;
		
			break;	
		}
		
		case XDM_LOADCONTROLS:
		{	
			params->result = XDR_HANDLED;			
			break;
		}
		
		case XDM_VERIFYCONTROLS:
		{	
			// variabile d'utilità per conversione misure
			int32 tmpVal;	
			
			// prendo nuove misure
			xd_edt_get(ALTEZZAMODULOEDTID, tmpStr, &(*xtPrefPtr).documentoPref.altezzaModulo, NULL);
			xd_edt_get(DISTANZAMODULIEDTID, tmpStr, &(*xtPrefPtr).documentoPref.distanzaModuli, NULL);
			
			xd_edt_get(MODULICOLANNAEDTID, tmpStr, NULL, NULL);
			StringToNum(tmpStr, &tmpVal);
			(*xtPrefPtr).documentoPref.moduliColonna = tmpVal;
			
			xd_edt_get(COLONNEPAGINAEDTID, tmpStr, NULL, NULL);
			StringToNum(tmpStr, &tmpVal);
			(*xtPrefPtr).documentoPref.colonnePagina = tmpVal;
			
			xd_edt_get(ALTEZZATESTATAEDTID, tmpStr, &(*xtPrefPtr).documentoPref.altezzaTestata, NULL);
				
			break;
		}
		
		default:
		{
			params->result = XDR_NOTHANDLED;
			break;
		}
	}
	
	return(noErr);
} // DocumentoWap

/* ------------------------------------------------------------------------ *

	DatiWap
	
* ------------------------------------------------------------------------ */
int32 XTAPI DatiWap(xdwapparamptr params) throw()
{
	static XtPreferencesPtr xtPrefPtr = NULL;
	xdlgsetupptr dlgSetUp;
	
	params->result = XDR_HANDLED;
	
	switch(params->opcode) 
	{
		case XDM_DIALOGSETUP:
		{
			dlgSetUp = (xdlgsetupptr) params->param1;
			xtPrefPtr = (XtPreferencesPtr) params->param2;
			dlgSetUp->dlgresid = DATIDIALOGID;
			
			break;
		}
		
		case XDM_DECLARATIONS:
		{
			// edit text percorso cartella dati
			xd_edt_declare(CARTELLADATIEDTID, XEDTF_READONLY | XEDTF_MULTILINE, ALLOK, STRINGS, NULL, NULL, NULL);
			
			// bottone
			xd_btnpsh_declare(SFOGLIACARTELLADATIBTNID, FALSE, CHSIM_NONE);
			
			// campi editabili per estensione doc da impaginare
			xd_edt_declare(ESTENSIONEDOC1EDTID, XEDTF_STANDARD, ALLOK, STRINGS, NULL, NULL, NULL);
			xd_edt_declare(ESTENSIONEDOC2EDTID, XEDTF_STANDARD, ALLOK, STRINGS, NULL, NULL, NULL);
			xd_edt_declare(ESTENSIONEDOC3EDTID, XEDTF_STANDARD, ALLOK, STRINGS, NULL, NULL, NULL);
			
			// check box
			xd_btnchk_declare(ESTENSIONEDOC1CHECKID, 0);
			xd_btnchk_declare(ESTENSIONEDOC2CHECKID, 0);
			xd_btnchk_declare(ESTENSIONEDOC3CHECKID, 0);
			
			break;
		}
		
		case XDM_TABSHOWING:
		{	
			// setto l'edit text con il fullpath della cartella dati
			xd_edt_set(CARTELLADATIEDTID, (*xtPrefPtr).datiPref.folderDati, NULL );
			
			// setto i 3 edit text con le possibili estensioni da impaginare
			xd_edt_set(ESTENSIONEDOC1EDTID, (*xtPrefPtr).datiPref.estensione1, NULL);
			xd_edt_set(ESTENSIONEDOC2EDTID, (*xtPrefPtr).datiPref.estensione2, NULL);
			xd_edt_set(ESTENSIONEDOC3EDTID, (*xtPrefPtr).datiPref.estensione3, NULL);
			
			// setto i 3 check box
			xd_btnchk_setstate(ESTENSIONEDOC1CHECKID, (*xtPrefPtr).datiPref.impaginaDocEstensione1);
			xd_btnchk_setstate(ESTENSIONEDOC2CHECKID, (*xtPrefPtr).datiPref.impaginaDocEstensione2);
			xd_btnchk_setstate(ESTENSIONEDOC3CHECKID, (*xtPrefPtr).datiPref.impaginaDocEstensione3);
			
			params->result = XDR_NOTHANDLED;
		
			break;	
		}
	
		case XDM_LOADCONTROLS:
		{	
			params->result = XDR_HANDLED;			
			break;
		}
		
		case XDM_USERACTION:
		{
			switch (params->itemid)
			{
				case SFOGLIACARTELLADATIBTNID:
				{
					// variabile per gestione pickfolder
					uchar thePath[MAXPATHNAME];
					
					// prende il percorso preesistente e glielo passa in modo che la wd sia già ben settata
					uchar cartellaDatiSelezionata[MAXPATHNAME] = "";
					xd_edt_get(CARTELLADATIEDTID, cartellaDatiSelezionata, NULL, NULL);	
					
					// gestisco la dialog di seleziona directory
					if (STRLEN(cartellaDatiSelezionata) > 0)	
						STRCPY(thePath, cartellaDatiSelezionata);
					else
						STRCPY(thePath, (*xtPrefPtr).datiPref.folderDati);
						
					// apro finestra per selezione cartella		
					bool8 pickFolderOk = MyPickFolder((uchar*)"Seleziona Cartella Dati:",  thePath);
					
					if (pickFolderOk == TRUE)
					{
						// setto l'edit textcon il nome della cartella edizione appena selezionata
						STRCAT(thePath, kDirSeparatorChar);
						xd_edt_set(CARTELLADATIEDTID, thePath, 0); 	
						
						STRCPY((*xtPrefPtr).datiPref.folderDati, thePath);
					}
					else
					{
						break;
					}			
				}
			}
		}
		
		case XDM_VERIFYCONTROLS:
		{	
			// prendo valore edt box
			xd_edt_get(ESTENSIONEDOC1EDTID, (*xtPrefPtr).datiPref.estensione1, NULL, NULL);
			xd_edt_get(ESTENSIONEDOC2EDTID, (*xtPrefPtr).datiPref.estensione2, NULL, NULL);
			xd_edt_get(ESTENSIONEDOC3EDTID, (*xtPrefPtr).datiPref.estensione3, NULL, NULL);
			
			// prendo valore check box
			xd_btnchk_getstate(ESTENSIONEDOC1CHECKID, &(*xtPrefPtr).datiPref.impaginaDocEstensione1);
			xd_btnchk_getstate(ESTENSIONEDOC2CHECKID, &(*xtPrefPtr).datiPref.impaginaDocEstensione2);
			xd_btnchk_getstate(ESTENSIONEDOC3CHECKID, &(*xtPrefPtr).datiPref.impaginaDocEstensione3);	
			
			break;
		}
		
		default:
		{
			params->result = XDR_NOTHANDLED;
			break;
		}	
	}
	
	return(noErr);
} // DatiWap

/* ------------------------------------------------------------------------ *

	DisposizioneWap
	
* ------------------------------------------------------------------------ */
int32 XTAPI DisposizioneWap(xdwapparamptr params) throw()
{
	static XtPreferencesPtr xtPrefPtr = NULL;
	xdlgsetupptr dlgSetUp;
	
	params->result = XDR_HANDLED;
	
	switch(params->opcode) 
	{
		case XDM_DIALOGSETUP:
		{
			dlgSetUp = (xdlgsetupptr) params->param1;
			xtPrefPtr = (XtPreferencesPtr) params->param2;
			dlgSetUp->dlgresid = DISPOSIZIONEDIALOGID;
			
			break;
		}
		
		case XDM_DECLARATIONS:
		{
			// dichiaro i due radiobottoni per la disposizione
			xd_btnrad_declare(VERTICALERADIOBTNID, ORIZZONTALERADIOBTNID, 0);
			
			break;
		}
		
		case XDM_TABSHOWING:
		{	
			// setto i radio bottoni per le opzioni della vista durante l'impaginazione
			xd_btnrad_setstate((*xtPrefPtr).disposizionePref.tipoDisposizione);
			
			params->result = XDR_NOTHANDLED;
		
			break;	
		}
		
		case XDM_LOADCONTROLS:
		{	
			params->result = XDR_HANDLED;			
			break;
		}
		
		case XDM_VERIFYCONTROLS:
		{
			// get the status of the view radio buttons
			xd_btnrad_getstate(VERTICALERADIOBTNID, &((*xtPrefPtr).disposizionePref.tipoDisposizione));
			
			break;
		}
		
		default:
		{
			params->result = XDR_NOTHANDLED;
			break;
		}
	}
	
	return(noErr);
} // PreferenzeWap

/* ------------------------------------------------------------------------ *

	CartelleWap
	
* ------------------------------------------------------------------------ */
int32 XTAPI CartelleWap(xdwapparamptr params) throw()
{
	static XtPreferencesPtr xtPrefPtr = NULL;
	xdlgsetupptr dlgSetUp;
	
	params->result = XDR_HANDLED;
	
	switch(params->opcode) 
	{
		case XDM_DIALOGSETUP:
		{
			dlgSetUp = (xdlgsetupptr) params->param1;
			xtPrefPtr = (XtPreferencesPtr) params->param2;
			dlgSetUp->dlgresid = CARTELLEDIALOGID;
			
			break;
		}
		case XDM_DECLARATIONS:
		{	
			// edit text per path folder tmp
			xd_edt_declare(FOLDERTMPEDTID, XEDTF_READONLY | XEDTF_MULTILINE, ALLOK, STRINGS, NULL, NULL, NULL);
			
			// bottone
			xd_btnpsh_declare(SFOGLIAFOLDERTMPBTNID, FALSE, CHSIM_NONE);
			
			// edit per path folder last imp
			xd_edt_declare(FOLDERLASTIMPEDTID, XEDTF_READONLY | XEDTF_MULTILINE, ALLOK, STRINGS, NULL, NULL, NULL);
			
			// bottone
			xd_btnpsh_declare(SFOGLIAFOLDERLASTIMPBTNID, FALSE, CHSIM_NONE);
		}
		case XDM_TABSHOWING:
		{
			// setto gli edit text
			xd_edt_set(FOLDERTMPEDTID, (*xtPrefPtr).cartellePref.cartellaTemporanei, NULL );
			xd_edt_set(FOLDERLASTIMPEDTID, (*xtPrefPtr).cartellePref.cartellaLastImp, NULL );
			
			params->result = XDR_NOTHANDLED;
			
			break;
		}
		case XDM_LOADCONTROLS:
		{	
			params->result = XDR_HANDLED;			
			break;
		}
		
		case XDM_USERACTION:
		{
			switch (params->itemid)
			{
				case SFOGLIAFOLDERTMPBTNID:
				{
					// variabile per gestione pickfolder
					uchar thePath[MAXPATHNAME];
					
					// prende il percorso preesistente e glielo passa in modo che la wd sia già ben settata
					uchar cartellaTmpSelezionata[MAXPATHNAME] = "";
					xd_edt_get(FOLDERTMPEDTID, cartellaTmpSelezionata, NULL, NULL);	
					
					// gestisco la dialog di seleziona directory
					if (STRLEN(cartellaTmpSelezionata) > 0)	
						STRCPY(thePath, cartellaTmpSelezionata);
					else
						STRCPY(thePath, (*xtPrefPtr).cartellePref.cartellaTemporanei);
						
					// apro finestra per selezione cartella		
					bool8 pickFolderOk = MyPickFolder((uchar*)"Seleziona Cartella Temporanei:",  thePath);
					
					if (pickFolderOk == TRUE)
					{
						// setto l'edit textcon il nome della cartella edizione appena selezionata
						STRCAT(thePath, kDirSeparatorChar);
						xd_edt_set(FOLDERTMPEDTID, thePath, 0); 	
						
						STRCPY((*xtPrefPtr).cartellePref.cartellaTemporanei, thePath);
					}
					else
					{
						break;
					}
					
					break;
				}
				
				case SFOGLIAFOLDERLASTIMPBTNID:
				{
					// variabile per gestione pickfolder
					uchar thePath[MAXPATHNAME];
					
					// prende il percorso preesistente e glielo passa in modo che la wd sia già ben settata
					uchar cartellaLastImpSelezionata[MAXPATHNAME] = "";
					xd_edt_get(FOLDERLASTIMPEDTID, cartellaLastImpSelezionata, NULL, NULL);	
					
					// gestisco la dialog di seleziona directory
					if (STRLEN(cartellaLastImpSelezionata) > 0)	
						STRCPY(thePath, cartellaLastImpSelezionata);
					else
						STRCPY(thePath, (*xtPrefPtr).cartellePref.cartellaLastImp);
						
					// apro finestra per selezione cartella		
					bool8 pickFolderOk = MyPickFolder((uchar*)"Seleziona Cartella Ultima Impaginazione:",  thePath);
					
					if (pickFolderOk == TRUE)
					{
						// setto l'edit textcon il nome della cartella edizione appena selezionata
						STRCAT(thePath, kDirSeparatorChar);
						xd_edt_set(FOLDERLASTIMPEDTID, thePath, 0); 	
						
						STRCPY((*xtPrefPtr).cartellePref.cartellaLastImp, thePath);
					}
					else
					{
						break;
					}
					
					break;
				}
			}
		}
		default:
		{
			params->result = XDR_NOTHANDLED;
			break;
		}
	}
	
	return(noErr);
} // CartelleWap
