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
#include "Menu.h"

#include "ImpaginazioneWap.h"
#include "BozzaWap.h"
#include "FinaleWap.h"
#include "DisposizioneWap.h"

// GLOBALS
// GLOBALS
// GLOBALS

// dichiaro variabile preferenze e la inizializzo con valori di default 
XtPreferences gXtPreferences;

// EX preferenza interna (kLongIngombroTotale) per ora la uso come globale
extern double gLongIngombroTotale = 0;

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	DeleteEndFilePathSeparator

* ------------------------------------------------------------------------ */
void XTAPI DeleteEndFilePathSeparator(uchar* ioString) throw()
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
bool8 XTAPI GetFileGenerico(uchar* nomefile, uchar* directorydipartenza, uchar* nomeattualefile, 
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
    QXStringCreateFromCString((char*) szTitle, 0,
                                  (int32) CSTRLEN((char*)szTitle), &titleRef);
    
    QXStringRef tempStrRef = NULL;
    QXStringCreateFromCString("QXP", 0,
                                  (int32) CSTRLEN("QXP"), &tempStrRef);
    
    QXStringExtractIntoUniChars(tempStrRef, 0, kNoPos, &(ptrList->tname[0]), 0);
    ptrList->tdata[ 0 ].ftype = PROJTYPE;
    ptrList->tdatacount = 1;
    
    err = XTUGetFileUsingXD(fileInfo, startLocationFileInfo, titleRef, NULL, NULL, NULL, typeList);
    
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
bool8 XTAPI SelezionaFileDialog(uchar* iPercorsoCompleto) throw()
{
	
	uchar 		lnomefile[MAXPATHNAME] = "",
				lStringaPerControlli[MAXPATHNAME] = "",
				nomeFile[MAXPATHNAME] = "",
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
	if (lrisultato)
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
bool8 XTAPI MyPickFolder(uchar* iTitle, uchar* ioPath) throw()
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

#ifdef _DEBUG
/* ------------------------------------------------------------------------ *

	IniziaPreferenzeDiDebug

* ------------------------------------------------------------------------ */
void XTAPI IniziaPreferenzeDiDebug() throw()
{
	gXtPreferences.impaginazionePref.tipoImpaginazione = ImpaginazioneWap::POSAUTORADIOBTNID; // tipo impaginazione

	gXtPreferences.giustificazionePref.valoreIniziale = VStringToFixed("3.528mm"); // valore iniziale spazio tra necrologi
	gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi = TRUE; // abilita spazio tra necrologi
	gXtPreferences.giustificazionePref.incrementoMaxSpazioNecrologi = VStringToFixed("0.706mm"); // incremento max spazio tra necrologi
	gXtPreferences.giustificazionePref.decrementoMaxSpazioNecrologi	= VStringToFixed("0.353mm"); // decremento max spazio tra necrologi
	gXtPreferences.giustificazionePref.abilitaSpazioTraParagrafi = TRUE; // abilita spazio tra paragrafi
	gXtPreferences.giustificazionePref.incrementoMaxSpazioParagrafi = VStringToFixed("0.529mm"); // incremento max spazio tra paragrafi
	gXtPreferences.giustificazionePref.decrementoMaxSpazioParagrafi	= VStringToFixed("0.706mm"); // decremento max spazio tra paragrafi
	gXtPreferences.giustificazionePref.abilitaInterlinea = TRUE; // abilita interlinea
	gXtPreferences.giustificazionePref.incrementoMaxInterlinea = VStringToFixed("0.529mm"); // incremento max interlinea
	gXtPreferences.giustificazionePref.decrementoMaxInterlinea = VStringToFixed("0.176mm"); // decremento max interlinea
	
	gXtPreferences.riempitiviPref.usaAnniversari = TRUE; // usa anniversari
	gXtPreferences.riempitiviPref.usaRingraziamenti = TRUE; // usa ringraziamenti
	
	gXtPreferences.immaginiPref.abilitaImportImag = TRUE; // abilita importazione immagini
	gXtPreferences.immaginiPref.altezzaImg = VStringToFixed("41.6mm"); // altezza immagini
	gXtPreferences.immaginiPref.larghezzaImg = VStringToFixed("29.9mm"); // larghezza immagini
#ifdef _DEBUG
	CSTRCPY(gXtPreferences.immaginiPref.folderImg, "C:\\SE\\NECRO\\IMG"); // folder immagini
#else
	CSTRCPY(gXtPreferences.immaginiPref.folderImg, "\\\\ARCHSRV\\OUT_NECRO"); // folder immagini
#endif // _DEBUG
	CSTRCPY(gXtPreferences.immaginiPref.estensioneElenco, "IMM"); // estensione elenco immagini
	CSTRCPY(gXtPreferences.immaginiPref.estensioneImg, "EPS"); // estensione immagini
	
	gXtPreferences.bozzaPref.tipoStampaBozza = BozzaWap::STAMPAAGGREGATIRADIOBTNID; // tipo stampa bozza
	gXtPreferences.bozzaPref.stampaAutomatica = FALSE; // stampa bozza automatica
#ifdef _DEBUG
	CSTRCPY(gXtPreferences.bozzaPref.docPathBozza, "C:\\SE\\NECRO\\BOZ.QXP"); // documento bozza
	CSTRCPY(gXtPreferences.bozzaPref.folderPathBozze, "C:\\SE\\NECRO\\BOZ"); // cartella bozze
#else
	CSTRCPY(gXtPreferences.bozzaPref.docPathBozza, "\\\\DATASERVER\\SE\\NECRO\\GABBIE\\BOZZAA4_9X.QXP"); // documento bozza
	CSTRCPY(gXtPreferences.bozzaPref.folderPathBozze, "\\\\DATASERVER\\SE\\NECRO\\BOZZE"); // cartella bozze
#endif // _DEBUG

#ifdef _DEBUG
	CSTRCPY(gXtPreferences.ingombroPref.docPathIngombro, "C:\\SE\\NECRO\\ING.QXP"); // documento ingombro
	CSTRCPY(gXtPreferences.ingombroPref.folderPathIngombri, "C:\\SE\\NECRO\\ING"); // cartella ingombri
#else
	CSTRCPY(gXtPreferences.ingombroPref.docPathIngombro, "\\\\DATASERVER\\SE\\NECRO\\GABBIE\\INGOMBRO6C_9X.QXP"); // documento ingombro
	CSTRCPY(gXtPreferences.ingombroPref.folderPathIngombri, "\\\\DATASERVER\\SE\\NECRO\\INGOMBRI"); // cartella ingombri
#endif // _DEBUG

#ifdef _DEBUG
	CSTRCPY(gXtPreferences.finalePref.docPathFinale, "C:\\SE\\NECRO\\FIN.QXP"); // documento finale
	CSTRCPY(gXtPreferences.finalePref.folderPathFinale, "C:\\SE\\NECRO\\FIN"); // cartella finale
#else
	CSTRCPY(gXtPreferences.finalePref.docPathFinale, "\\\\DATASERVER\\SE\\NECRO\\GABBIE\\FINALE6C_9X.QXP"); // documento finale
	CSTRCPY(gXtPreferences.finalePref.folderPathFinale, "\\\\DATASERVER\\SE\\NECRO\\DEFINITIVI"); // cartella finale
#endif // _DEBUG
	gXtPreferences.finalePref.preparaDocumento = FinaleWap::ULTIMAPAGINARADIOBTNID; // prepara documento	
	gXtPreferences.finalePref.posizionamentoIngombri = FinaleWap::BASSOVERSOALTOTADIOBTNID;	// posizionamentoingombri
	
	gXtPreferences.documentoPref.altezzaModulo = VStringToFixed("22.966mm"); // altezza modulo
	gXtPreferences.documentoPref.distanzaModuli = VStringToFixed("3.281mm"); // distanza moduli
	gXtPreferences.documentoPref.moduliColonna = 16; // moduli colonna
	gXtPreferences.documentoPref.colonnePagina = 6; // colonne pagina
	gXtPreferences.documentoPref.altezzaTestata = VStringToFixed("13.123mm"); // altezza testata

#ifdef _DEBUG
	CSTRCPY(gXtPreferences.datiPref.folderDati, "C:\\SE\\NECRO\\DAT"); // cartella dati
#else
	CSTRCPY(gXtPreferences.datiPref.folderDati, "\\\\DATASERVER\\SE\\NECRO\\DATI"); // cartella dati
#endif // _DEBUG
	CSTRCPY(gXtPreferences.datiPref.estensione1, "BOZ"); //  estensione da impaginare 1
	gXtPreferences.datiPref.impaginaDocEstensione1 = TRUE; // impagina estensione 1
	CSTRCPY(gXtPreferences.datiPref.estensione2, "ING"); //  estensione da impaginare 2
	gXtPreferences.datiPref.impaginaDocEstensione2 = TRUE; // impagina estensione 2
	CSTRCPY(gXtPreferences.datiPref.estensione3, "NEC"); //  estensione da impaginare 3 
	gXtPreferences.datiPref.impaginaDocEstensione3 = TRUE; // impagina estensione 3

	gXtPreferences.disposizionePref.tipoDisposizione = DisposizioneWap::VERTICALERADIOBTNID; // ottimizzazione verticale

#ifdef _DEBUG
	CSTRCPY(gXtPreferences.cartellePref.cartellaTemporanei, "C:\\SE\\NECRO\\TMP"); // folder temporanei
	CSTRCPY(gXtPreferences.cartellePref.cartellaLastImp, "C:\\SE\\NECRO\\LST"); // folder last imp
#else
	CSTRCPY(gXtPreferences.cartellePref.cartellaTemporanei, "\\\\DATASERVER\\SE\\NECRO\\TEMP"); // folder temporanei
	CSTRCPY(gXtPreferences.cartellePref.cartellaLastImp, "\\\\DATASERVER\\SE\\NECRO\\ULTIMA"); // folder last imp
#endif // _DEBUG
} // IniziaPreferenzeDiDebug
#endif // _DEBUG
