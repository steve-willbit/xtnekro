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
#include <vector>

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpConsts.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "XTNecro.h"

#include "Pref.h"

// STATIC
// STATIC
// STATIC

// globale x ultima finestra visualizzata
static int32 gLastRowSelected = 0;

// GLOBALS
// GLOBALS
// GLOBALS

// dichiaro variabile preferenze e la inizializzo con valori di default 
XtPreferences gXtPreferences =
{
	POSAUTORADIOBTNID, // tipo impaginazione
	0, // valore iniziale spazio tra necrologi
	TRUE, // abilita spazio tra necrologi
	0, // incremento max spazio tra necrologi
	0, // decremento max spazio tra necrologi
	TRUE, // abilita spazio tra paragrafi
	0, // incremento max spazio tra paragrafi
	0, // decremento max spazio tra paragrafi
	TRUE, // abilita interlinea
	0, // incremento max interlinea
	0, // decremento max interlinea
	TRUE, // usa anniversari
	TRUE, // usa ringraziamenti
	TRUE, // abilita importazione immagini
	0, // altezza immagini
	0, // larghezza immagini
	"nessuna cartella selezionata", // folder immagini
	"IMM", // estensione elenco immagini
	"EPS", // estensione immagini
	STAMPAAGGREGATIRADIOBTNID, // tipo stampa bozza
	FALSE, // stampa bozza automatica
	"nessun documento selezionato", // documento bozza
	"nessuna cartella selezionata", // cartella bozze
	"nessun documento selezionato", // documento ingombro
	"nessuna cartella selezionata", // cartella ingombri
	"nessun documento selezionato", // documento finale
	"nessuna cartella selezionata", // cartella finale
	PRIMAPAGINARADIOBTNID, // prepara documento	
	ALTOVERSOBASSORADIOBTNID,	// ordinamento impaginazione
	0, // altezza modulo
	0, // distanza moduli
	0, // moduli colonna
	0, // colonne pagina
	0, // altezza testata
	"nessuna cartella selezionata", // cartella dati
	"BOZ", //  estensione da impaginare 1
	TRUE, // impagina estensione 1
	"ING", //  estensione da impaginare 2
	TRUE, // impagina estensione 2
	"NEC", //  estensione da impaginare 3 
	TRUE, // impagina estensione 3
	VERTICALERADIOBTNID, // ottimizzazione verticale
	"nessuna cartella selezionata", // folder temporanei
	"nessuna cartella selezionata" // folder last imp
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
		int16 strLength = stringTmpString.length();
		if (stringTmpString.at(strLength - 1) == DBP::kDirSeparatorChar)
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

	OPENFILENAME ofn;
	uchar szDirName[256];
	uchar szFile[256], szFileTitle[256];
	UINT  i, cbString;
	uchar  chReplace;    // string separator for szFilter
	uchar  szFilter[256];
	uchar	lFILTERSTRING[256];

	// Get the system directory name, and store in szDirName 
	STRCPY(lFILTERSTRING, spiega_filtro);
	STRCAT(lFILTERSTRING,"|");
	STRCAT(lFILTERSTRING,filtro);
	STRCAT(lFILTERSTRING,"|");
	STRCPY(szFilter,lFILTERSTRING);
	cbString = STRLEN(lFILTERSTRING);
	chReplace = szFilter[cbString - 1]; // retrieve wildcard

	for (i = 0; szFilter[i] != '\0'; i++) {
	 if (szFilter[i] == chReplace)
		 szFilter[i] = '\0';
	}

	// Set all structure members to zero.
	STRCPY(szFile,nomeattualefile);
	STRCPY(szDirName,directorydipartenza);
	// ho settato la directory da cui partire
	memset(&ofn, 0, sizeof(OPENFILENAME));
	
	// prendo puntatore alla dialog corrente
	DialogPtr dlgPtr;
	xtget_frontmost(&dlgPtr); 

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = dlgPtr;
	ofn.lpstrFilter = (char*)szFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (char*)szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFileTitle = (char*)szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);
	ofn.lpstrInitialDir = (char*)szDirName;
	ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	bool8 OpenErr = GetOpenFileName(&ofn);
	
	if (OpenErr)
	{
		STRCPY(nomefile, ofn.lpstrFile);
		result= FALSE;
	}
	else
	{
		// e' successo qualche casino o qualcosa non e' andato bene
		result=TRUE;
	}

	return (result);
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
	
_Errore:
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
	
#if WINOS
	int16 pickFolderErr = PickFolderUsingXD(iTitle, ioPath, NULL, &vol, &dirId, NULL, NULL); 
#endif // WINOS
#if MACOS
	int16 pickFolderErr = PickFolderUsingXD(iTitle, ioPath, NULL, &vol, &dirId, NULL);
#endif // MACOS
	
	if (pickFolderErr == noErr)
	{	
		// per prendere il pathname corretto del folder selezionato
		// concateno un file fittizio (Dummy.qxp)
		GetFullPathHFS(ioPath, vol, dirId, (const uchar*)"Dummy.qxp", FALSE);
		// elimino il file dummy
		extractpathname(ioPath);
		// Aggiunta by MANS la conversione in stringa C e viceversa,
		// altrimenti la DeleteEndFilePathSeparator non funzionava.
#if MACOS	
		P2CSTR(ioPath);
#endif // MACOS		
		DeleteEndFilePathSeparator(ioPath);
#if MACOS				
		C2PSTR(ioPath);
#endif // MACOS		
		myPickFolder = TRUE;
	}	
	else
		myPickFolder = FALSE;
	
return(myPickFolder);

} // MyPickFolder


/* ------------------------------------------------------------------------ *

	ImpaginazioneWap
	
* ------------------------------------------------------------------------ */
int32 XTAPI ImpaginazioneWap(xdwapparamptr params) throw()
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
			dlgSetUp->dlgresid = IMPAGINAZIONEDIALOGID;
			
			break;
		}
		
		case XDM_DECLARATIONS:
		{
			// dichiaro i radio bottoni
			xd_btnrad_declare(MANUALERADIOBTNID, POSAUTORADIOBTNID, 0);
			
			break;
		}
		
		case XDM_TABSHOWING:
		{	
			// setto i radio bottoni
			xd_btnrad_setstate((*xtPrefPtr).impaginazionePref.tipoImpaginazione);
			
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
			xd_btnrad_getstate(MANUALERADIOBTNID, &((*xtPrefPtr).impaginazionePref.tipoImpaginazione));
			
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

	GiustificazioneWap
	
* ------------------------------------------------------------------------ */
int32 XTAPI GiustificazioneWap(xdwapparamptr params) throw()
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
			dlgSetUp->dlgresid = GIUSTIFICAZIONEDIALOGID;
			
			break;
		}
		
		case XDM_DECLARATIONS:
		{
			// dichiaro i 3 check box	
			xd_btnchk_declare(SPAZIONECROCHECKID, 0);
			xd_btnchk_declare(SPAZIOPARACHECKID, 0);
			xd_btnchk_declare(INTERLINEACHECKID, 0);
			
			// dichiaro edit box per contenere le misure
			xd_edt_declare(SPAZIONECROVALINIEDTID, XEDTF_STANDARD, ALLOK, VDEFUNITS, FIX(0), FIX(999999), VALUERANGE);
			
			xd_edt_declare(SPAZIONECROINCRMAXEDTID, XEDTF_STANDARD, ALLOK, VDEFUNITS, FIX(0), FIX(999999), VALUERANGE);
			xd_edt_declare(SPAZIONECRODECRMAXEDTID, XEDTF_STANDARD, ALLOK, VDEFUNITS, FIX(0), FIX(999999), VALUERANGE);
			
			xd_edt_declare(SPAZIOPARAINCRMAXEDTID, XEDTF_STANDARD, ALLOK, VDEFUNITS, FIX(0), FIX(999999), VALUERANGE);
			xd_edt_declare(SPAZIOPARADECRMAXEDTID, XEDTF_STANDARD, ALLOK, VDEFUNITS, FIX(0), FIX(999999), VALUERANGE);
			
			xd_edt_declare(INTERLINEAINCRMAXEDTID, XEDTF_STANDARD, ALLOK, VDEFUNITS, FIX(0), FIX(999999), VALUERANGE);
			xd_edt_declare(INTERLINEADECRMAXEDTID, XEDTF_STANDARD, ALLOK, VDEFUNITS, FIX(0), FIX(999999), VALUERANGE);
			
			break;	
		}
		
		case XDM_TABSHOWING:
		{
			xd_edt_set(SPAZIONECROVALINIEDTID, NULL, (*xtPrefPtr).giustificazionePref.valoreIniziale);
			
			// stato gruppo spazio tra necro
			xd_btnchk_setstate(SPAZIONECROCHECKID, (*xtPrefPtr).giustificazionePref.abilitaSpazioTraNecrologi);
			xd_edt_set(SPAZIONECROINCRMAXEDTID, NULL, (*xtPrefPtr).giustificazionePref.incrementoMaxSpazioNecrologi);
			xd_edt_set(SPAZIONECRODECRMAXEDTID, NULL, (*xtPrefPtr).giustificazionePref.decrementoMaxSpazioNecrologi);
			
			// stato gruppo spazio tra paragrafi
			xd_btnchk_setstate(SPAZIOPARACHECKID, (*xtPrefPtr).giustificazionePref.abilitaSpazioTraParagrafi);
			xd_edt_set(SPAZIOPARAINCRMAXEDTID, NULL, (*xtPrefPtr).giustificazionePref.incrementoMaxSpazioParagrafi);
			xd_edt_set(SPAZIOPARADECRMAXEDTID, NULL, (*xtPrefPtr).giustificazionePref.decrementoMaxSpazioParagrafi);
			
			// stato gruppo spazio tra necro
			xd_btnchk_setstate(INTERLINEACHECKID, (*xtPrefPtr).giustificazionePref.abilitaInterlinea);
			xd_edt_set(INTERLINEAINCRMAXEDTID, NULL, (*xtPrefPtr).giustificazionePref.incrementoMaxInterlinea);
			xd_edt_set(INTERLINEADECRMAXEDTID, NULL, (*xtPrefPtr).giustificazionePref.decrementoMaxInterlinea);
			
			// gestisco abilitazione degli edit text
			if ((*xtPrefPtr).giustificazionePref.abilitaSpazioTraNecrologi)
			{
				xd_setcontrolstate(SPAZIONECROINCRMAXEDTID, XCS_ENABLED);
				xd_setcontrolstate(SPAZIONECRODECRMAXEDTID, XCS_ENABLED);
			}
			else
			{
				xd_setcontrolstate(SPAZIONECROINCRMAXEDTID, XCS_DISABLED);
				xd_setcontrolstate(SPAZIONECRODECRMAXEDTID, XCS_DISABLED);
			}
			
			if ((*xtPrefPtr).giustificazionePref.abilitaSpazioTraParagrafi)
			{
				xd_setcontrolstate(SPAZIOPARAINCRMAXEDTID, XCS_ENABLED);
				xd_setcontrolstate(SPAZIOPARADECRMAXEDTID, XCS_ENABLED);
			}
			else
			{
				xd_setcontrolstate(SPAZIOPARAINCRMAXEDTID, XCS_DISABLED);
				xd_setcontrolstate(SPAZIOPARADECRMAXEDTID, XCS_DISABLED);
			}
			
			if ((*xtPrefPtr).giustificazionePref.abilitaInterlinea)
			{
				xd_setcontrolstate(INTERLINEAINCRMAXEDTID, XCS_ENABLED);
				xd_setcontrolstate(INTERLINEADECRMAXEDTID, XCS_ENABLED);
			}
			else
			{
				xd_setcontrolstate(INTERLINEAINCRMAXEDTID, XCS_DISABLED);
				xd_setcontrolstate(INTERLINEADECRMAXEDTID, XCS_DISABLED);
			}
			
			params->result = XDR_NOTHANDLED;
			
			break;
		}
		
		case XDM_USERACTION:
		{
			switch (params->itemid)
			{
				// gestisco abilitazione / disabilitazione degli oggetti
				case SPAZIONECROCHECKID:
				{
					int16 isEnabled = FALSE;
					xd_btnchk_getstate(SPAZIONECROCHECKID, &isEnabled);
					if (isEnabled)
					{
						xd_setcontrolstate(SPAZIONECROINCRMAXEDTID, XCS_ENABLED);
						xd_setcontrolstate(SPAZIONECRODECRMAXEDTID, XCS_ENABLED);
					}
					else
					{
						xd_setcontrolstate(SPAZIONECROINCRMAXEDTID, XCS_DISABLED);
						xd_setcontrolstate(SPAZIONECRODECRMAXEDTID, XCS_DISABLED);
					}
					
					params->result = XDR_NOTHANDLED;
					break;
				}
				
				case SPAZIOPARACHECKID:
				{
					int16 isEnabled = FALSE;
					xd_btnchk_getstate(SPAZIOPARACHECKID, &isEnabled);
					if (isEnabled)
					{
						xd_setcontrolstate(SPAZIOPARAINCRMAXEDTID, XCS_ENABLED);
						xd_setcontrolstate(SPAZIOPARADECRMAXEDTID, XCS_ENABLED);
					}
					else
					{
						xd_setcontrolstate(SPAZIOPARAINCRMAXEDTID, XCS_DISABLED);
						xd_setcontrolstate(SPAZIOPARADECRMAXEDTID, XCS_DISABLED);
					}
					
					params->result = XDR_NOTHANDLED;
					break;
				}
				
				case INTERLINEACHECKID:
				{
					int16 isEnabled = FALSE;
					xd_btnchk_getstate(INTERLINEACHECKID, &isEnabled);
					if (isEnabled)
					{
						xd_setcontrolstate(INTERLINEAINCRMAXEDTID, XCS_ENABLED);
						xd_setcontrolstate(INTERLINEADECRMAXEDTID, XCS_ENABLED);
					}
					else
					{
						xd_setcontrolstate(INTERLINEAINCRMAXEDTID, XCS_DISABLED);
						xd_setcontrolstate(INTERLINEADECRMAXEDTID, XCS_DISABLED);
					}
					
					params->result = XDR_NOTHANDLED;
					break;
				}
				
				default:
				{
					params->result = XDR_NOTHANDLED;
					break;
				}
			}
		}
		
		case XDM_VERIFYCONTROLS:
		{
			// variabile d'utilità per le stringhe
			uchar tmpStr[MAXPATHNAME] = "";
			
			// prendo nuove misure
			xd_edt_get(SPAZIONECROVALINIEDTID, tmpStr, &(*xtPrefPtr).giustificazionePref.valoreIniziale, NULL);
			
			xd_btnchk_getstate(SPAZIONECROCHECKID, &(*xtPrefPtr).giustificazionePref.abilitaSpazioTraNecrologi);
			xd_edt_get(SPAZIONECROINCRMAXEDTID, tmpStr, &(*xtPrefPtr).giustificazionePref.incrementoMaxSpazioNecrologi, NULL);
			xd_edt_get(SPAZIONECRODECRMAXEDTID, tmpStr, &(*xtPrefPtr).giustificazionePref.decrementoMaxSpazioNecrologi, NULL);
			
			xd_btnchk_getstate(SPAZIOPARACHECKID, &(*xtPrefPtr).giustificazionePref.abilitaSpazioTraParagrafi);
			xd_edt_get(SPAZIOPARAINCRMAXEDTID, tmpStr, &(*xtPrefPtr).giustificazionePref.incrementoMaxSpazioParagrafi, NULL);
			xd_edt_get(SPAZIOPARADECRMAXEDTID, tmpStr, &(*xtPrefPtr).giustificazionePref.decrementoMaxSpazioParagrafi, NULL);
			
			xd_btnchk_getstate(INTERLINEACHECKID, &(*xtPrefPtr).giustificazionePref.abilitaInterlinea);
			xd_edt_get(INTERLINEAINCRMAXEDTID, tmpStr, &(*xtPrefPtr).giustificazionePref.incrementoMaxInterlinea, NULL);
			xd_edt_get(INTERLINEADECRMAXEDTID, tmpStr, &(*xtPrefPtr).giustificazionePref.decrementoMaxInterlinea, NULL);
			
			break;
		}
		
		default:
		{
			params->result = XDR_NOTHANDLED;
			break;
		}
	}
	
	return(noErr);
} // GiustificazioneWap

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
			
			NumToString((*xtPrefPtr).documentoPref.moduliColonna,  tmpStr);
			xd_edt_set(MODULICOLANNAEDTID, tmpStr, NULL);
			
			NumToString((*xtPrefPtr).documentoPref.colonnePagina,  tmpStr);
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

/* ------------------------------------------------------------------------ *

	PreferenzeWap
	
* ------------------------------------------------------------------------ */
int32 XTAPI PreferenzeWap(xdwapparamptr params) throw()
{
	static xrect windowLocation = {-1,-1,-1,-1};
	static XtPreferences xtPreferences;

	xdlgsetupptr dlgSetUp;
	DialogPtr dlgPtr;
	xtget_frontmost(&dlgPtr);
	
	params->result = XDR_HANDLED;
	
	switch(params->opcode) 
	{			
		case XDM_DIALOGSETUP:
		{
			dlgSetUp = (xdlgsetupptr) params->param1;
			dlgSetUp->dlgresid = PREFERENZEDIALOGID;
			
			break;
		}
		
		case XDM_DECLARATIONS:
		{
			// tab control
			xd_tab_declare(TABCONTROLPREFERENZE, XTABF_NOHEADERS);
			
			// dichiaro lista
			xd_lstlbx_declare(LISTAPREFERENZEID, 0 , NULL);
			
			params->result = XDR_HANDLED;
			
			break;		
		}
		
		case XDM_LOADCONTROLS:
		{
			// carico stato preferenze corrente
			xtPreferences = gXtPreferences;
			
			// carico le dialog da inserire all'interno dei vari tab
			xd_tab_addtab(TABCONTROLPREFERENZE, _XT_IMPAGINAZIONEWAP, (int32) &xtPreferences, NULL);
			xd_tab_addtab(TABCONTROLPREFERENZE, _XT_GIUSTIFICAZIONEWAP, (int32) &xtPreferences, NULL);
			xd_tab_addtab(TABCONTROLPREFERENZE, _XT_RIEMPITIVIWAP, (int32) &xtPreferences, NULL);
			xd_tab_addtab(TABCONTROLPREFERENZE, _XT_IMMAGINIWAP, (int32) &xtPreferences, NULL);
			xd_tab_addtab(TABCONTROLPREFERENZE, _XT_BOZZAWAP, (int32) &xtPreferences, NULL);
			xd_tab_addtab(TABCONTROLPREFERENZE, _XT_INGOMBROWAP, (int32) &xtPreferences, NULL);
			xd_tab_addtab(TABCONTROLPREFERENZE, _XT_FINALEWAP, (int32) &xtPreferences, NULL);
			xd_tab_addtab(TABCONTROLPREFERENZE, _XT_DOCUMENTOWAP, (int32) &xtPreferences, NULL);
			xd_tab_addtab(TABCONTROLPREFERENZE, _XT_DATIWAP, (int32) &xtPreferences, NULL);
			xd_tab_addtab(TABCONTROLPREFERENZE, _XT_DISPOSIZIONEWAP, (int32) &xtPreferences, NULL);
			xd_tab_addtab(TABCONTROLPREFERENZE, _XT_CARTELLEWAP, (int32) &xtPreferences, NULL);
			
			// setto il tab corrente in avvio
			xd_tab_setcurrent(TABCONTROLPREFERENZE, 0);
			
			// carico la lista
			std::vector<std::string> listaPreferenze;
			listaPreferenze.reserve(11);
			listaPreferenze.push_back("Impaginazione");
			listaPreferenze.push_back("Giustificazione");
			listaPreferenze.push_back("Riempitivi");
			listaPreferenze.push_back("Immagini");
			listaPreferenze.push_back("Bozza");
			listaPreferenze.push_back("Ingombro");
			listaPreferenze.push_back("Definitivo");
			listaPreferenze.push_back("Documento");
			listaPreferenze.push_back("Dati");
			listaPreferenze.push_back("Disposizione");
			listaPreferenze.push_back("Cartelle");
			
			xrow row;
			for (int16 i=0; i < listaPreferenze.size(); i++)
			{
				xd_lst_addrow(LISTAPREFERENZEID, IR_BOTTOM ,&row);
				xd_lst_setrowtext(LISTAPREFERENZEID, row, (uchar*) listaPreferenze.at(i).c_str());	
			}
			
			// setto la selezione
			xd_lst_setselection(LISTAPREFERENZEID, gLastRowSelected);
			xd_tab_setcurrent(TABCONTROLPREFERENZE, gLastRowSelected);
						
			// set the posistion of the dialog
			if (-1 != windowLocation.top
				&& -1 != windowLocation.left
				&& -1 != windowLocation.bottom
				&& -1 != windowLocation.right)
			{
				xd_setwindowscreenrect(&windowLocation);
			}
			
			params->result = XDR_HANDLED;
			
			break;
		}
		
		case XDM_USERACTION:
		{
			switch (params->itemid)
			{
				case LISTAPREFERENZEID:
				{      
					xrow rowSel;
					xd_lst_getselection(LISTAPREFERENZEID, &rowSel);

					static xrow curTab = gLastRowSelected;
					if (curTab != rowSel)
					{
						// cambio la tab window con quella selezionata
						xd_tab_setcurrent(TABCONTROLPREFERENZE, rowSel);
						curTab = rowSel;
					}

					params->result = XDR_NOTHANDLED;
					break;				
				}
				
				default:
				{
					params->result = XDR_NOTHANDLED;
					break;
				}	
			}

			break;
		}
		
		case XDM_DOVERB:
		{			
			// copio le nuove preferenze
			gXtPreferences = xtPreferences;
			
			break;
		}
		
		case XDM_CANCEL:
		{
			break;
		}
		
		case XDM_DEINIT:
		{
			// get the position of this dialog
			xd_getwindowscreenrect(&windowLocation);
			
			// prendo l'ultima selezione della lista
			xd_lst_getselection(LISTAPREFERENZEID, &gLastRowSelected);
			
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

	AboutNecroWap
	
* ------------------------------------------------------------------------ */
int32 XTAPI AboutNecroWap(xdwapparamptr params) throw()
{
	xdlgsetupptr dlgSetUp;
	static xmetalist necroLogo = NULL;
	
	
	params->result = XDR_HANDLED;
	
	switch (params->opcode)
	{	
		case XDM_DIALOGSETUP:
		{
			dlgSetUp = (xdlgsetupptr) params->param1;
			dlgSetUp->dlgresid = ABOUTNECRODIALOGID;
			
			break;
		}
		case XDM_DECLARATIONS:
		{
#if WINOS			
			xd_txt_declare(ABOUTNECROLOGOID, XTXTF_CENTERJUSTIFY | XTXTF_METALIST);
#endif // WINOS		
			xd_txt_declare(VERSIONID, XTXTF_SMALLFONT);
			
			break;
		}
		case XDM_LOADCONTROLS:
		{
#if WINOS
			xml_createlist(&necroLogo);
			xml_insertpict(necroLogo, INSMETA_ATBEGINNING, XMLF_NOHILITE, ABOUTNECROLOGOBMP,NULL);
			xd_txt_setmetalist(ABOUTNECROLOGOID, necroLogo); 

			xrect windowRect = {0, 0, 0, 0};
			xd_getclientscreenrect(&windowRect);

			xrect itemRect = {0, 0, 0, 0}; 
			xd_getcontrolclientrect(ABOUTNECROLOGOID, &itemRect);
			windowRect.bottom = windowRect.top + itemRect.bottom + 3;
			windowRect.right = windowRect.left + itemRect.right + 3;
			xd_setclientscreenrect(&windowRect);
#endif // WINOS			

			break;		
		}
		case XDM_BUTTONDOWN:
		{
			xd_close(CLOSE_CANCEL, 0, NULL);
			break;
		}
		default:
		{
			params->result = XDR_NOTHANDLED;
			break;
		}		
	}
	
	return(noErr);
	
} // AboutSmartClassWap

#pragma mark -
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

#include <dos.h>
#include <dir.h>
#include <string.h>
#include "Include.h"
#include "Errori.h"
#include "Define.h"
#include "PresFile.h"
#include "Memoria.h"
#include "ResStr.h"
//#include "OpString.h"
#include "Pref.h"
//#include "SelFold.h"
//#include "CartPref.h"

#include "Debug.h"

/* Costanti */
/* Costanti */
/* Costanti */

#define kAPPWARE 1

#define kTipoPreferenze 0x6E465250
#define rIdPreferenze 20000
/* per le preferenze dell'Xtension */

#define kTipoAppWare   0x4A505040    /*'APPL'*/
/*
#if kAPPWARE
#define kCreatoreAppWare 'APNA'
/* per controllare il tipo dell'applicazione AppWare vera */
/*#else
#define kCreatoreAppWare 'aplt'
/* per controllare il tipo dell'applicazione AppWare script */
/*
#endif
*/
#define rFinestraPreferenze 31000
/* identificatore della finestra delle preferenze */

#define kMinimoIncrementoInterlinea 0
#define kMassimoIncrementoInterlinea mFixed(30L)
#define kMinimoDecrementoInterlinea 0
#define kMassimoDecrementoInterlinea mFixed(30L)
#define kMinimoIncrementoSpazioTraParagrafi 0
#define kMassimoIncrementoSpazioTraParagrafi mFixed(30L)
#define kMinimoDecrementoSpazioTraParagrafi 0
#define kMassimoDecrementoSpazioTraParagrafi mFixed(30L)
#define kMinimoIncrementoSpazioTraNecrologi 0
#define kMassimoIncrementoSpazioTraNecrologi mFixed(30L)
#define kMinimoDecrementoSpazioTraNecrologi 0
#define kMassimoDecrementoSpazioTraNecrologi mFixed(30L)
#define kMinimoSpazioTraNecrologi 0
#define kMassimoSpazioTraNecrologi mFixed(30L)
#define kMinimaAltezzaImmagini mFixed(1L)
#define kMassimaAltezzaImmagini mFixed(200L)
#define kMinimaLarghezzaImmagini mFixed(1L)
#define kMassimaLarghezzaImmagini mFixed(200L)
#define kMinimaAltezzaModulo mFixed(1L)
#define kMassimaAltezzaModulo mFixed(200L)
#define kMinimaDistanzaModuli mFixed(0)
#define kMassimaDistanzaModuli mFixed(200L)
#define kMinimaModuliPerColonna mFixed(1L)
#define kMassimaModuliPerColonna mFixed(200L)
#define kMinimaColonnePerPagina mFixed(1L)
#define kMassimaColonnePerPagina mFixed(15L)
#define kMinimaAltezzaTestata mFixed(0L)
#define kMassimaAltezzaTestata mFixed(100L)
#define kMinimoNumeroTentativi mFixed(1L)
#define kMassimoNumeroTentativi mFixed(100L)
/* per i controlli su valori Fixed editiati nella finestre di preferenza */

#define kUltimaVersione 5
/* ultima versione delle preferenze */

/* Variabili Globali */
/* Variabili Globali */
/* Variabili Globali */

handlepreferenze gHndlPreferenze = NULL;
/* handle che contiene tutte le preferenze dell'Xtension */

/* Variabili */
/* Variabili */
/* Variabili */

// static Point gPosizioneFinestraPreferenze = {32767,32767};
/* posizione della finestra */

static handlepreferenze gHndlFinestraPreferenze = NULL;
/* per la gestione della finestra delle preferenze e non salvare in memoria */
/* se l'utente sceglie annulla */

static void LeggiNecroIni(void);
static void LeggiNecroIni(void)
/* ------------------------------------------------------------------------ *

	LeggiNecroIni

	2 Aprile 1996						Stefano

	Descrizione:
	Legge le impostazioni registrare sul file *.ini

	Parametri:
	nessuno

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	 unsigned char szBuf[kDimensioneStringhe] = "";
	 /* per leggere i valori del file *.ini */


	if (GetPrivateProfileString( "Preferenze", "Cartella Bozze", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrCartellaBozze, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Cartella Ingombri", "", szBuf, sizeof(szBuf), gConfigurationFile) != 0)
	{
		strcpy(kStrCartellaIngombri, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Cartella Definitivi", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrCartellaDefinitivi, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Cartella daAppW", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrCartellaDaAppWare, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Cartella perAppW", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrCartellaPerAppWare, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Programma Lista", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrApplicazioneAppWare, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Cartella Ultima Impaginazione", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrCartellaUltimaImpaginazione, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Cartella Preferenze", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrCartellaPreferenze, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Cartella Immagini", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrCartellaImmagini, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Estensione Elenco Immagini", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrEstensioneElencoImmagini, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Estensione Immagini", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrEstensioneImmagini, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Documento Bozza", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrDocumentoBozza, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Documento Ingombro", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrDocumentoIngombro, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Documento Definitivo", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrDocumentoDefinitivo, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Cartella Dati", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrCartellaDati, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Estensione Bozze", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrEstensioneBozza, szBuf);
	}


	if (GetPrivateProfileString( "Preferenze", "Estensione Ingombri", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrEstensioneIngombro, szBuf);
	}

	if (GetPrivateProfileString( "Preferenze", "Estensione Definitivi", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
	{
		strcpy(kStrEstensioneDefinitivo, szBuf);
	}

  if (GetPrivateProfileString( "Preferenze", "Cartella Temporanea", "", szBuf, sizeof(szBuf), gConfigurationFile ) != 0)
  {
		strcpy(kStrCartellaTmp, szBuf);
  }

} /* LeggiNecroIni */

static void ScriviNecroIni(void);
static void ScriviNecroIni(void)
/* ------------------------------------------------------------------------ *

	ScriviNecroIni

	2 Aprile 1996						Stefano

	Descrizione:
	Scrive le impostazioni sul file *.ini

	Parametri:
	nessuno

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	 unsigned char szBuf[kDimensioneStringhe] = "";
	 /* per scrivere sul file necro.ini */

	if (strlen(kStrCartellaBozze) != 0)
	{
		WritePrivateProfileString("Preferenze", "Cartella Bozze", kStrCartellaBozze, gConfigurationFile);
	}

	if (strlen(kStrCartellaIngombri) != 0)
	{
		WritePrivateProfileString("Preferenze", "Cartella Ingombri", kStrCartellaIngombri, gConfigurationFile);
	}

	if (strlen(kStrCartellaDefinitivi) != 0)
	{
		WritePrivateProfileString("Preferenze", "Cartella Definitivi", kStrCartellaDefinitivi, gConfigurationFile);
	}

	if (strlen(kStrCartellaDaAppWare) != 0)
	{
		WritePrivateProfileString("Preferenze", "Cartella daAppW", kStrCartellaDaAppWare, gConfigurationFile);
	}

	if (strlen(kStrCartellaPerAppWare) != 0)
	{
		WritePrivateProfileString("Preferenze", "Cartella perAppW", kStrCartellaPerAppWare, gConfigurationFile);
	}

	if (strlen(kStrApplicazioneAppWare) != 0)
	{
		WritePrivateProfileString("Preferenze", "Programma Lista", kStrApplicazioneAppWare, gConfigurationFile);
	}

	if (strlen(kStrCartellaUltimaImpaginazione) != 0 )
	{
		WritePrivateProfileString("Preferenze", "Cartella Ultima Impaginazione", kStrCartellaUltimaImpaginazione, gConfigurationFile);
	}

	if (strlen(kStrCartellaPreferenze) != 0)
	{
		WritePrivateProfileString("Preferenze", "Cartella Preferenze", kStrCartellaPreferenze, gConfigurationFile);
	}

	if (strlen(kStrCartellaImmagini) != 0)
	{
		WritePrivateProfileString("Preferenze", "Cartella Immagini", kStrCartellaImmagini, gConfigurationFile);
	}

	if (strlen(kStrEstensioneElencoImmagini) != 0)
	{
		WritePrivateProfileString("Preferenze", "Estensione Elenco Immagini", kStrEstensioneElencoImmagini, gConfigurationFile);
	}

	/* scrive la preferenza anche se è uguale a zero */
	WritePrivateProfileString("Preferenze", "Estensione Immagini", kStrEstensioneImmagini, gConfigurationFile);

	if (strlen(kStrDocumentoBozza) != 0)
	{
		WritePrivateProfileString("Preferenze", "Documento Bozza", kStrDocumentoBozza, gConfigurationFile);
	}

	if (strlen(kStrDocumentoIngombro) != 0)
	{
		WritePrivateProfileString("Preferenze", "Documento Ingombro", kStrDocumentoIngombro, gConfigurationFile);
	}

	if (strlen(kStrDocumentoDefinitivo) != 0)
	{
		WritePrivateProfileString("Preferenze", "Documento Definitivo", kStrDocumentoDefinitivo, gConfigurationFile);
	}

	if (strlen(kStrCartellaDati) != 0)
	{
		WritePrivateProfileString("Preferenze", "Cartella Dati", kStrCartellaDati, gConfigurationFile);
	}

	if (strlen(kStrEstensioneBozza) != 0)
	{
		WritePrivateProfileString("Preferenze", "Estensione Bozze", kStrEstensioneBozza, gConfigurationFile);
	}

	if (strlen(kStrEstensioneIngombro) != 0)
	{
		WritePrivateProfileString("Preferenze", "Estensione Ingombri", kStrEstensioneIngombro, gConfigurationFile);
	}

	if (strlen(kStrEstensioneDefinitivo) != 0)
	{
		WritePrivateProfileString("Preferenze", "Estensione Definitivi", kStrEstensioneDefinitivo, gConfigurationFile);
	}

} /* ScriviNecroIni */

static void CopiaPreferenze(void);
static void CopiaPreferenze(void)
/* ------------------------------------------------------------------------ *

	CopiaPreferenze

	3 Aprile 1997						Stefano

	Descrizione:
	Imposto i valori di default delle preferenze.

	Parametri:
	nessuno

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	/* preferenze interne */
	kVersione = kUltimaVersione;
	strcpy(kStrCartellaBozze, "c:\\necro\\Bozze");
	strcpy(kStrCartellaIngombri, "c:\\necro\\Ingombri");
	strcpy(kStrCartellaDefinitivi, "c:\\necro\\Definiti");
//	strcpy(kStrCartellaDaAppWare, "c:\\necro\\daAppW");
//	strcpy(kStrCartellaPerAppWare, "c:\\necro\\perAppW");
//	strcpy(kStrDocumentoPerAppWare, "Prova.nec");
//	strcpy(kStrApplicazioneAppWare, "c:\\necro\\Appl\\Lista.exe");

//	strcpy(kStrCartellaPreferenze, "c:\\necro\\Pref");
	strcpy(kStrCartellaUltimaImpaginazione, "c:\\necro\\lastimp");
	kCheckAbilitaImpaginazione = TRUE;
	kCheckImmagineCentrata = TRUE;
	kCheckImmagineProporzionata = TRUE;
	kLongIntervalloControllo = 30;
	kFixedVariazioneAltezza = mFixed(1);
	kLongIngombroTotale = 0;
	kFixedAltezzaInterna = mFixed(20);
	kRadioTipoPreparazioneDocumento = iRadioPreparaInCoda;

	/* preferenze esterne */
	kRadioTipoPreferenze = iRadioImpaginazione;
	kRadioTipoDefinitiva = iRadioPosizionamentoAutomatico;

	kCheckModificaSpazioTraNecrologi = TRUE;
	kFixedIncrementoSpazioTraNecrologi = mFixed(2);
	kFixedDecrementoSpazioTraNecrologi = mFixed(1);
	kCheckModificaSpazioTraParagrafi = TRUE;
	kFixedIncrementoSpazioTraParagrafi = mFixed(1);
	kFixedDecrementoSpazioTraParagrafi = mFixed(1);
	kCheckModificaInterlinea = TRUE;
	kFixedIncrementoInterlinea = mFixed(1) + (65536 / 2);
	kFixedDecrementoInterlinea = mFixed(1);
	kFixedSpazioTraNecrologi = mFixed(5);

	kCheckUsaRingraziamenti = TRUE;
	kCheckUsaAnniversari = TRUE;

	kCheckImportaImmagini = TRUE;
	//kFixedAltezzaImmagini = mFixed(141) + (65536 * 732 / 1000);
	//kFixedLarghezzaImmagini = mFixed(99) + (65536 * 213 / 1000);
	kFixedAltezzaImmagini = mFixed(105);
	kFixedLarghezzaImmagini = mFixed(90);
	strcpy(kStrCartellaImmagini, "c:\\necro\\Immagini");
	strcpy(kStrEstensioneElencoImmagini, "IMM");
	strcpy(kStrEstensioneImmagini, "TIF");

	kRadioTipoStampaBozze = iRadioStampaAggregati;
	kCheckStampaInAutomatico = FALSE;
	strcpy(kStrDocumentoBozza, "c:\\necro\\Gabbie\\BozzaA4.qxp");

	strcpy(kStrDocumentoIngombro, "c:\\necro\\Gabbie\\Ingombro.qxp");

	strcpy(kStrDocumentoDefinitivo, "c:\\necro\\Gabbie\\Finale.qxp");
	kRadioTipoPreparazioneDocumento = iRadioPreparaInCoda;
	kRadioTipoPosizioneIngombri = iRadioIngombriInBasso;

	kFixedAltezzaModulo = mFixed(117);
	kFixedDistanzaModuli = mFixed(9);
	kFixedModuliPerColonna = mFixed(12);
	kFixedColonnePerPagina = mFixed(8);
	kFixedAltezzaTestata = mFixed(23) + (65536 * 711 / 1000);

	strcpy(kStrCartellaDati, "c:\\necro\\Dati");

	strcpy(kStrEstensioneBozza, "BOZ");
	kCheckImpaginaBozza = TRUE;
	strcpy(kStrEstensioneIngombro, "ING");
	kCheckImpaginaIngombro = TRUE;
	strcpy(kStrEstensioneDefinitivo, "NEC");
	kCheckImpaginaDefinitivo = TRUE;

} /* CopiaPreferenze */

errorixtension InizializzaPreferenze(void)
/* ------------------------------------------------------------------------ *

	InizializzaPreferenze

	24 Ottobre 1995						Stefano

	Descrizione:
	Legge le preferenze memorizzate nel file delle risorse di tipo PREF e di
	identificatore 20000. Se per qualche motivo non Š possibile accedere al file
	delle risorse si usano le preferenze scritte nel codice senza segnalare alcun
	errore.

	Parametri:
	nessuno

	Ritorno:
	ritorna l'errore di xtension eventuale

* ------------------------------------------------------------------------ */
{
	/* leggo i valori delle preferenze dalle risorse */
	gHndlPreferenze = (handlepreferenze) GetResource(kTipoPreferenze, rIdPreferenze);
	if (gHndlPreferenze == NULL)  {
		/* crea l'handle per contenere le preferenze */
		gErroreXtension = CreaHandle((Handle *) &gHndlPreferenze, sizeof(preferenze));
		if (gErroreXtension != kNessunErrore) {
			/* errore sulla allocazione della memoria */
			StampaDebug("CreaHandle", (long) gErroreXtension);
			return(gErroreXtension);
		}

		/* copio i valori di default delle preferenze */
		CopiaPreferenze();

		/* leggo il file Necro.ini */
		LeggiNecroIni();

		/* aggiungo la risorsa */
		AddResource((Handle) gHndlPreferenze,	kTipoPreferenze,
															rIdPreferenze,
															"Preferenze Xtension");
	} else {

		/* controllo la versione */
		if (kVersione != kUltimaVersione)
		{
			/* libero l'handle precedente */
			LiberaHandle(&gHndlPreferenze);

			/* crea l'handle per contenere le preferenze */
			gErroreXtension = CreaHandle((Handle *) &gHndlPreferenze, sizeof(preferenze));
			if (gErroreXtension != kNessunErrore) {
				/* errore sulla allocazione della memoria */
				StampaDebug("CreaHandle", (long) gErroreXtension);
				return(gErroreXtension);
			}

			/* carico di nuovo i valori delle preferenze poiché c'è una diversità di versione */
			CopiaPreferenze();
		}

		/* leggo il file necro.ini con le impostazioni */
		LeggiNecroIni();

		/* lock dell'handle delle preferenze */
		HLock((Handle) gHndlPreferenze);
	}

	/* preparo l'handle da usare per la gestione della dialog delle preferenze */
	gErroreXtension = CreaHandle((Handle *) &gHndlFinestraPreferenze, sizeof(preferenze));
	if (gErroreXtension != kNessunErrore) {
		/* errore sulla allocazione della memoria */
		StampaDebug("AlloccaHandle", (long) gErroreXtension);
		return(gErroreXtension);
	}

	return(kNessunErrore);
}


BOOL GetDocFile( SFReply FAR *, Str255 );
BOOL GetDocFile( SFReply FAR *rep, Str255 szTitle )
{
BOOL 				result;
LPEXTFILEARRAY list;
LPEXTFILETYPE  ptr;
int16 			err;



	list =(LPEXTFILEARRAY)NewPtr( 2 * sizeof(LPEXTFILETYPE) );
	list[ 0 ] = ptr = (LPEXTFILETYPE)NewPtrClear(sizeof(EXTFILETYPE) );
	list[ 1 ] = 0L;

	strcpy( &(ptr->tname[ 0 ]), (LPSTR)"XPress Doc" );
	ptr->tdatacount = 1;
	ptr->tdata[ 0 ].ftype = XDOC;


	result = getfile( (LPSTR)szTitle,
					0,
					(LPVOID)NULL,
					0,
					(SFReply far *)rep,
					(LPVOID)NULL,
					(LPSTR)NULL,
					(LPSTR)NULL,
					(LPEXTFILEARRAY)list
				);
	 DisposePtr((Ptr) ptr );
	 DisposePtr((Ptr) list);
	 return( result );
}

errorixtension ControllaPreferenze(void)
/* ------------------------------------------------------------------------ *

	ControllaPreferenze

	30 Ottobre 1995						Stefano

	Descrizione:
	Controlla che tutte le preferenze necessarie per eseguire l'impaginazione
	siano impostate correttamente. La procedura si preoccupa di stampare sul
	di rapporto le eventuali incongruenze.

	Parametri:
	nessuno

	Ritorno:
	ritorna l'eventuale errore nel controllo delle preferenze

* ------------------------------------------------------------------------ */

{
	int lContatoreErrori = 0;
	/* serve per sapere se si Š verificato un errore */
	struct ffblk myffblk;

	/* verifico la presenza della cartella dei file da impaginare */
	gErrore = PresenzaCartella(kStrCartellaDati);
	if (gErrore != noErr) {
		/* errore manca la cartella dei file da impaginare */
		StampaDebug("PresenzaCartella", gErrore);
		ConcatenaAllaStringaErrore(kStrCartellaDati);
		StampaErrore(kErrorePresenzaCartella);
		lContatoreErrori++;
	}

#if kNEW
	
	/* verifico la presenza della cartella per i temporanei */
	gErrore = PresenzaCartella(PercorsoCompleto(kStrCartellaTmp));
	if (gErrore != noErr) {
		/* errore manca la cartella per i temporanei */
		StampaDebug("PresenzaCartella", (long) gErrore);
		ConcatenaAllaStringaErrore(kStrCartellaTmp);
		StampaErrore(kErrorePresenzaCartella);
		lContatoreErrori++;
	}
	
#endif /* kNEW */
	
	/* verifico la presenza della cartella per le bozze */
	gErrore = PresenzaCartella(kStrCartellaBozze);
	if (gErrore != noErr) {
		/* errore manca la cartella per le bozze */
		StampaDebug("PresenzaCartella", (long) gErrore);
		ConcatenaAllaStringaErrore(PercorsoCompleto(kStrCartellaBozze));
		StampaErrore(kErrorePresenzaCartella);
		lContatoreErrori++;
	}
	
	/* verifico la presenza della cartella per gli ingombri */
	gErrore = PresenzaCartella(kStrCartellaIngombri);
	if (gErrore != noErr) {
		/* errore manca la cartella per gli ingombri */
		StampaDebug("PresenzaCartella", (long) gErrore);
		ConcatenaAllaStringaErrore(PercorsoCompleto(kStrCartellaIngombri));
		StampaErrore(kErrorePresenzaCartella);
		lContatoreErrori++;
	}

	/* verifico la presenza della cartella per i definitivi */
	gErrore = PresenzaCartella(kStrCartellaDefinitivi);
	if (gErrore != noErr) {
		/* errore manca la cartella per i definitivi */
		StampaDebug("PresenzaCartella", (long) gErrore);
		ConcatenaAllaStringaErrore(PercorsoCompleto(kStrCartellaDefinitivi));
		StampaErrore(kErrorePresenzaCartella);
		lContatoreErrori++;
	}

#if kOLD

	/* verifico la presenza della cartella per i documenti da AppWare */
	gErrore = PresenzaCartella(kStrCartellaDaAppWare);
	if (gErrore != noErr) {
		/* errore manca la cartella per i documenti da AppWare */
		StampaDebug("PresenzaCartella", (long) gErrore);
		ConcatenaAllaStringaErrore(PercorsoCompleto(kStrCartellaDaAppWare));
		StampaErrore(kErrorePresenzaCartella);
		lContatoreErrori++;
	}

	/* verifico la presenza della cartella per i documenti a AppWare */
	gErrore = PresenzaCartella(kStrCartellaPerAppWare);
	if (gErrore != noErr) {
		/* errore manca la cartella per i documenti per AppWare */
		StampaDebug("PresenzaCartella", (long) gErrore);
		ConcatenaAllaStringaErrore(PercorsoCompleto(kStrCartellaPerAppWare));
		StampaErrore(kErrorePresenzaCartella);
		lContatoreErrori++;
	}

	/* verifico la presenza dell'applicazione AppWare */
	gErrore = findfirst( kStrApplicazioneAppWare, &myffblk, FA_ARCH );
//	gErrore = CreaFInfoFile(PercorsoCompleto(kStrApplicazioneAppWare), &lFInfoFile);
	if (gErrore != noErr) {
		/* errore manca il documento bozza campione */
		StampaDebug("CreaFInfoFile", (long) gErrore);
		ConcatenaAllaStringaErrore(kStrApplicazioneAppWare);
		StampaErrore(kErrorePresenzaFile);
		lContatoreErrori++;
	}/* else {
		if (lFInfoFile.ftype != kTipoAppWare) {
			/* errore sulle caratteristiche dell'applicazione AppWare */ /*
			StampaDebug("lFInfoFile.ftype != kTipoAppWare", 0);
			ConcatenaAllaStringaErrore(PercorsoCompleto(kStrApplicazioneAppWare));
			StampaErrore(kErroreSulTipoApplicazione);
			lContatoreErrori++;
		}
	} */

#endif /* kOLD */

	/* verifico la presenza della cartella per salvare le preferenze */
	gErrore = PresenzaCartella(kStrCartellaPreferenze);
	if (gErrore != noErr) {
		/* errore manca la cartella per salvare le preferenze  */
		StampaDebug("PresenzaCartella", (long) gErrore);
		ConcatenaAllaStringaErrore(PercorsoCompleto(kStrCartellaPreferenze));
		StampaErrore(kErrorePresenzaCartella);
		lContatoreErrori++;
	}

	/* verifico la presenza della cartella dei file immagini */
	gErrore = PresenzaCartella(kStrCartellaImmagini);
	if (gErrore != noErr) {
		/* errore manca la cartella dei file immagini */
		StampaDebug("PresenzaCartella", 0);
		ConcatenaAllaStringaErrore(kStrCartellaImmagini);
		StampaErrore(kErrorePresenzaCartella);
		lContatoreErrori++;
	}

	/* verifico la presenza del documento bozza campione */
	gErrore = findfirst( kStrDocumentoBozza, &myffblk, FA_ARCH );
//	gErrore = CreaFInfoFile(kStrDocumentoBozza, &lFInfoFile);
	if (gErrore != noErr) {
		/* errore manca il documento bozza campione */
		StampaDebug("CreaFInfoFile", (long) gErrore);
		ConcatenaAllaStringaErrore(kStrDocumentoBozza);
		StampaErrore(kErrorePresenzaFile);
		lContatoreErrori++;
	}/* else {
		if (lFInfoFile.ftype != DOCTYPE ) {
			/* errore sulle caratteristiche del documento bozza campione *//*
			StampaDebug("lFInfoFile.ftype != DOCTYPE", 0);
			ConcatenaAllaStringaErrore(kStrDocumentoBozza);
			StampaErrore(kErroreSulTipoFile);
			lContatoreErrori++;
		}
	} */

	/* verifico la presenza del documento ingombro campione */
	gErrore = findfirst( kStrDocumentoIngombro, &myffblk, FA_ARCH );
//	gErrore = CreaFInfoFile(kStrDocumentoIngombro, &lFInfoFile);
	if (gErrore != noErr) {
		/* errore manca il documento ingombro campione */
		StampaDebug("CreaFInfoFile", (long) gErrore);
		ConcatenaAllaStringaErrore(kStrDocumentoIngombro);
		StampaErrore(kErrorePresenzaFile);
		lContatoreErrori++;
	}/* else {
		if (lFInfoFile.ftype != DOCTYPE) {
			/* errore sulle caratteristiche del documento ingombro campione *//*
			StampaDebug("lFInfoFile.ftype != DOCTYPE", 0);
			ConcatenaAllaStringaErrore(kStrDocumentoIngombro);
			StampaErrore(kErroreSulTipoFile);
			lContatoreErrori++;
		}
	} */

	/* verifico la presenza della cartella per salvare l'ultima impaginazione */
	gErrore = PresenzaCartella(kStrCartellaUltimaImpaginazione);
	if (gErrore != noErr) {
		/* errore manca la cartella per salvare l'ultima impaginazione  */
		StampaDebug("PresenzaCartella", (long) gErrore);
		ConcatenaAllaStringaErrore(PercorsoCompleto(kStrCartellaUltimaImpaginazione));
		StampaErrore(kErrorePresenzaCartella);
		lContatoreErrori++;
	}


	if (lContatoreErrori != 0) return(kErroreControlloPreferenze);
	else return(kNessunErrore);
}

void ChiudiPreferenze(void)
/* ------------------------------------------------------------------------ *

	SalvaPreferenze

	24 Ottobre 1995						Stefano

	Descrizione:
	Salva le preferenze correnti nel file delle risorse

	Parametri:
	nessuno

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */

{
	/* scrivo il file necro.ini */
	ScriviNecroIni();

	/* salva lo stato corrente delle preferenze */
	if (gHndlPreferenze != NULL) {
		ChangedResource((Handle)gHndlPreferenze);
		WriteResource((Handle)gHndlPreferenze);
		HUnlock((Handle) gHndlPreferenze);
		ReleaseResource((Handle) gHndlPreferenze);
	}

	/* libera le preferenze usate nella finestra delle preferenze */
	LiberaHandle((Handle *) &gHndlFinestraPreferenze);
}

static void ImpostaValoriFinestraPreferenza(DialogPtr ptrfinestra,
											 handlepreferenze hndlpreferenze);
static void ImpostaValoriFinestraPreferenza(DialogPtr ptrfinestra,
											handlepreferenze hndlpreferenze)
/* ------------------------------------------------------------------------ *

	ImpostaValoriFinestraPreferenza

	8 Dicembre 1995						Stefano

	Descrizione:
	Inizializza la finestra delle preferenze in base ai valori in memoria

	Parametri:
	ptrfinestra - puntatore alla finestra delle preferenze
	hndlpreferenze -  handle alle preferenze da usare per impostare i valori
	della finestra delle preferenze

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	/* inizializzo il radio button per scegliere la preferenza */
	setradio((DialogPtr) ptrfinestra, iRadioImpaginazione, iRadioDati,
			 (**hndlpreferenze).tipoPreferenze);

	/* inizializzo il radio button sul tipo di impaginazione definitiva */
	setradio((DialogPtr) ptrfinestra, iRadioManuale, iRadioPosizionamentoAutomatico,
			 (**hndlpreferenze).tipoDefinitiva);

	/* inizializzo il check sulla giustificazione con lo spazio tra i necrologi */
	setcheck((DialogPtr) ptrfinestra, iCheckModificaSpazioTraNecrologi,
			 (**hndlpreferenze).modificaSpazioTraNecrologi);

	/* inizializzo il campo sull'incremento sullo spazio tra i necrologi */
	fsetfield((DialogPtr) ptrfinestra, iFixedIncrementoSpazioTraNecrologi,  DEFUNITS, 1,
			  (**hndlpreferenze).incrementoSpazioTraNecrologi);

	/* inizializzo il campo sul decremento sullo spazio tra i necrologi */
	fsetfield((DialogPtr) ptrfinestra, iFixedDecrementoSpazioTraNecrologi,  DEFUNITS, 1,
			  (**hndlpreferenze).decrementoSpazioTraNecrologi);

	/* inizializzo il check sulla giustificazione con lo spazio tra i paragrafi */
	setcheck((DialogPtr) ptrfinestra, iCheckModificaSpazioTraParagrafi,
			 (**hndlpreferenze).modificaSpazioTraParagrafi);

	/* inizializzo il campo sull'incremento dello spazio tra i paragrafi */
	fsetfield((DialogPtr) ptrfinestra, iFixedIncrementoSpazioTraParagrafi,  DEFUNITS, 1,
			  (**hndlpreferenze).incrementoSpazioTraParagrafi);

	/* inizializzo il campo sul decremento dello spazio tra i paragrafi */
	fsetfield((DialogPtr) ptrfinestra, iFixedDecrementoSpazioTraParagrafi,  DEFUNITS, 1,
			  (**hndlpreferenze).decrementoSpazioTraParagrafi);

	/* inizializzo il check sulla giustificazione con l'interlinea */
	setcheck((DialogPtr) ptrfinestra, iCheckModificaInterlinea,
			 (**hndlpreferenze).modificaInterlinea);

	/* inizializzo il campo sull'incremento dell'interlinea */
	fsetfield((DialogPtr) ptrfinestra, iFixedIncrementoInterlinea,  DEFUNITS, 1,
			  (**hndlpreferenze).incrementoInterlinea);

	/* inizializzo il campo sul decremento dell'interlinea */
	fsetfield((DialogPtr) ptrfinestra, iFixedDecrementoInterlinea,  DEFUNITS, 1,
			  (**hndlpreferenze).decrementoInterlinea);

	/* inizializzo il campo spazio tra i necrologi */
	fsetfield((DialogPtr) ptrfinestra, iFixedSpazioTraNecrologi,  DEFUNITS, 1,
			  (**hndlpreferenze).spazioTraNecrologi);

	/* inizializzo il check sull'uso dei ringraziamenti come riempitivi */
	setcheck((DialogPtr) ptrfinestra, iCheckUsaRingraziamenti,
			 (**hndlpreferenze).usaRingraziamenti);

	/* inizializzo il check sull'uso degli anniversari come riempitivi */
	setcheck((DialogPtr) ptrfinestra, iCheckUsaAnniversari,
			 (**hndlpreferenze).usaAnniversari);

	/* inizializzo il check sull'impotazione automatica delle immagini */
	setcheck((DialogPtr) ptrfinestra, iCheckImportaImmagini,
			 (**hndlpreferenze).importaImmagini);

	/* inizializzo il campo sull'altezza dell'immagini */
	fsetfield((DialogPtr) ptrfinestra, iFixedAltezzaImmagini,  DEFUNITS, 1,
			  (**hndlpreferenze).altezzaImmagini);

	/* inizializzo il campo sul larghezza dell'immagini */
	fsetfield((DialogPtr) ptrfinestra, iFixedLarghezzaImmagini,  DEFUNITS, 1,
			  (**hndlpreferenze).larghezzaImmagini);

	/* inizializzo il nome della cartella delle immagini */
	setfield((DialogPtr) ptrfinestra, iStrCartellaImmagini,
			 (**hndlpreferenze).cartellaImmagini);

	/* inizializzo l'estensione dell'elenco delle immagini */
	setfield((DialogPtr) ptrfinestra, iStrEstensioneElencoImmagini,
			 (**hndlpreferenze).estensioneElencoImmagini);

	/* inizializzo l'estensione delle immagini */
	setfield((DialogPtr) ptrfinestra, iStrEstensioneImmagini,
			 (**hndlpreferenze).estensioneImmagini);

	/* inizializzo il radio button sul tipo di stampa bozza */
	setradio((DialogPtr) ptrfinestra, iRadioStampaSingolo, iRadioStampaAggregati,
			 (**hndlpreferenze).tipoStampaBozze);

	/* inizializzo il check sulla stampa in  automatico */
	setcheck((DialogPtr) ptrfinestra, iCheckStampaInAutomatico,
			 (**hndlpreferenze).stampaInAutomatico);

	/* inizializzo il nome del documento di bozza */
	setfield((DialogPtr) ptrfinestra, iStrDocumentoBozza,
			 (**hndlpreferenze).documentoBozza);

	/* inizializzo il nome del documento di ingombro */
	setfield((DialogPtr) ptrfinestra, iStrDocumentoIngombro,
			 (**hndlpreferenze).documentoIngombro);

	/* inizializzo il nome del documento di definitivo */
	setfield((DialogPtr) ptrfinestra, iStrDocumentoDefinitivo,
			 (**hndlpreferenze).documentoDefinitivo);

	/* inizializzo il radio button sul tipo di preparazione dei documenti */
	setradio((DialogPtr) ptrfinestra, iRadioPreparaInTesta, iRadioPreparaInCoda,
			 (**hndlpreferenze).tipoPreparazioneDocumento);

	/* inizializzo il radio button sul tipo di posizionamento degli ingombri */
	setradio((DialogPtr) ptrfinestra, iRadioIngombriInAlto, iRadioIngombriInBasso,
			 (**hndlpreferenze).tipoPosizioneIngombri);

	/* inizializzo il campo altezza modulo */
	fsetfield((DialogPtr) ptrfinestra, iFixedAltezzaModulo,  DEFUNITS, 1,
			  (**hndlpreferenze).altezzaModulo);

	/* inizializzo il campo distanza tra i moduli */
	fsetfield((DialogPtr) ptrfinestra, iFixedDistanzaModuli,  DEFUNITS, 1,
			  (**hndlpreferenze).distanzaModuli);

	/* inizializzo il campo moduli per pagina */
	fsetfield((DialogPtr) ptrfinestra, iFixedModuliPerColonna,  NUMBER, 0,
			  (**hndlpreferenze).moduliPerColonna);

	/* inizializzo il campo colonne per pagina */
	fsetfield((DialogPtr) ptrfinestra, iFixedColonnePerPagina,  NUMBER, 0,
			  (**hndlpreferenze).colonnePerPagina);

	/* inizializzo il campo altezza testata */
	fsetfield((DialogPtr) ptrfinestra, iFixedAltezzaTestata,  DEFUNITS, 1,
			  (**hndlpreferenze).altezzaTestata);

	/* inizializzo il nome della cartella dati */
	setfield((DialogPtr) ptrfinestra, iStrCartellaDati,
			 (**hndlpreferenze).cartellaDati);

	/* inizializzo l'estensione della bozza */
	setfield((DialogPtr) ptrfinestra, iStrEstensioneBozza,
			 (**hndlpreferenze).estensioneBozza);

	/* inizializzo il check sull'impaginazione della bozza */
	setcheck((DialogPtr) ptrfinestra, iCheckImpaginaBozza,
			 (**hndlpreferenze).impaginaBozza);

	/* inizializzo l'estensione dell'ingombro */
	setfield((DialogPtr) ptrfinestra, iStrEstensioneIngombro,
			 (**hndlpreferenze).estensioneIngombro);

	/* inizializzo il check sull'impaginazione dell'ingombro */
	setcheck((DialogPtr) ptrfinestra, iCheckImpaginaIngombro,
			 (**hndlpreferenze).impaginaIngombro);

	/* inizializzo l'estensione del definitivo */
	setfield((DialogPtr) ptrfinestra, iStrEstensioneDefinitivo,
			 (**hndlpreferenze).estensioneDefinitivo);

	/* inizializzo il check sull'impaginazione del definitivo */
	setcheck((DialogPtr) ptrfinestra, iCheckImpaginaDefinitivo,
			 (**hndlpreferenze).impaginaDefinitivo);

}

static void VisualizzaSpazioTraNecrologi(DialogPtr ptrfinestra, Boolean davisualizzare);
static void VisualizzaSpazioTraNecrologi(DialogPtr ptrfinestra, Boolean davisualizzare)
/* ------------------------------------------------------------------------ *

	VisualizzaSpazioTraNecrologi

	8 Dicembre 1995						Stefano

	Descrizione:
	Visualizza gli item associati al check sulla giustificazione con spazio tra gli
	necrologi

	Parametri:
	ptrfinestra - puntatore alla finestra delle preferenze
	davisualizzare -  boolena per sapere se Š necessario visualizzare o nascondere

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	int i = 0;
	/* indice per i for */

	if (davisualizzare == TRUE) {
		/* visualizzo */
		for (i = iFixedIncrementoSpazioTraNecrologi - 1;
			 i <= iFixedDecrementoSpazioTraNecrologi; i++)
			ShowDItem(ptrfinestra, i);
	} else {
		/* nascondo */
		for (i = iFixedIncrementoSpazioTraNecrologi - 1;
			 i <= iFixedDecrementoSpazioTraNecrologi; i++)
			HideDItem(ptrfinestra, i);
	}
}

static void VisualizzaSpazioTraParagrafi(DialogPtr ptrfinestra, Boolean davisualizzare);
static void VisualizzaSpazioTraParagrafi(DialogPtr ptrfinestra, Boolean davisualizzare)
/* ------------------------------------------------------------------------ *

	VisualizzaSpazioTraParagrafi

	8 Dicembre 1995						Stefano

	Descrizione:
	Visualizza gli item associati al check sulla giustificazione con
	lo spazio tra i Paragrafi

	Parametri:
	ptrfinestra - puntatore alla finestra delle preferenze
	davisualizzare -  boolena per sapere se Š necessario visualizzare o nascondere

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	int i = 0;
	/* indice per i for */

	if (davisualizzare == TRUE) {
		/* visualizzo */
		for (i = iFixedIncrementoSpazioTraParagrafi - 1;
			 i <= iFixedDecrementoSpazioTraParagrafi; i++)
			ShowDItem(ptrfinestra, i);
	} else {
		/* nascondo */
		for (i = iFixedIncrementoSpazioTraParagrafi - 1;
			 i <= iFixedDecrementoSpazioTraParagrafi; i++)
			HideDItem(ptrfinestra, i);
	}
}

static void VisualizzaInterlinea(DialogPtr ptrfinestra, Boolean davisualizzare);
static void VisualizzaInterlinea(DialogPtr ptrfinestra, Boolean davisualizzare)
/* ------------------------------------------------------------------------ *

	VisualizzaInterlinea

	8 Dicembre 1995						Stefano

	Descrizione:
	Visualizza gli item associati al check sulla giustificazione con interlinea

	Parametri:
	ptrfinestra - puntatore alla finestra delle preferenze
	davisualizzare -  boolena per sapere se Š necessario visualizzare o nascondere

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	int i = 0;
	/* indice per i for */

	if (davisualizzare == TRUE) {
		/* visualizzo */
		for (i = iFixedIncrementoInterlinea - 1; i <= iFixedDecrementoInterlinea; i++)
			ShowDItem(ptrfinestra, i);
	} else {
		/* nascondo */
		for (i = iFixedIncrementoInterlinea - 1; i <= iFixedDecrementoInterlinea; i++)
			HideDItem(ptrfinestra, i);
	}
}

static void Visualizza(DialogPtr ptrfinestra, short tipopreferenza);
static void Visualizza(DialogPtr ptrfinestra, short tipopreferenza)
/* ------------------------------------------------------------------------ *

	Visualizza

	8 Dicembre 1995						Stefano

	Descrizione:
	Visualizza gli item associati al radio button passato

	Parametri:
	ptrfinestra - puntatore alla finestra delle preferenze

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	int i = 0;
	/* idice per i for */

	switch (tipopreferenza) {
		case iRadioImpaginazione:

			for (i = iPrimoItem; i < iStaticGiustificazione; i++)
				ShowDItem(ptrfinestra, i);
 					setradio((DialogPtr) ptrfinestra,
							 iRadioManuale,
							 iRadioPosizionamentoAutomatico,
							 ikRadioTipoDefinitiva);

			break;
		case iRadioGiustificazione:
			ShowDItem(ptrfinestra, iStaticGiustificazione);
			ShowDItem(ptrfinestra, iCheckModificaSpazioTraNecrologi);
			VisualizzaSpazioTraNecrologi(ptrfinestra, ikCheckModificaSpazioTraNecrologi);
			ShowDItem(ptrfinestra, iCheckModificaSpazioTraParagrafi);
			VisualizzaSpazioTraParagrafi(ptrfinestra, ikCheckModificaSpazioTraParagrafi);
			ShowDItem(ptrfinestra, iCheckModificaInterlinea);
			VisualizzaInterlinea(ptrfinestra, ikCheckModificaInterlinea);
			ShowDItem(ptrfinestra, iStaticSpazioTraNecrologi);
			ShowDItem(ptrfinestra, iFixedSpazioTraNecrologi);
			break;
		case iRadioRiempitivi:
			for (i = iStaticRiempitivi; i < iStaticImmagini; i++)
				ShowDItem(ptrfinestra, i);
			break;
		case iRadioImmagini:
			for (i = iStaticImmagini; i < iStaticConfigurazione; i++)
				ShowDItem(ptrfinestra, i);
			break;
		case iRadioConfigurazione:
			for (i = iStaticConfigurazione; i < iStaticBozza; i++)
				ShowDItem(ptrfinestra, i);
			break;
		case iRadioBozza:
			for (i = iStaticBozza; i < iStaticIngombro; i++)
				ShowDItem(ptrfinestra, i);
			break;
		case iRadioIngombro:
			for (i = iStaticIngombro; i < iStaticDefinitivo; i++)
				ShowDItem(ptrfinestra, i);
			break;
		case iRadioFinale:
			for (i = iStaticDefinitivo; i < iStaticDocumento; i++)
				ShowDItem(ptrfinestra, i);
			break;
		case iRadioDocumento:
			for (i = iStaticDocumento; i < iStaticDati; i++)
				ShowDItem(ptrfinestra, i);
			break;
		case iRadioDati:
			for (i = iStaticDati; i <= iUltimoItem; i++)
				ShowDItem(ptrfinestra, i);
			break;
		default:
			break;
	}
}

static void SpostaTutto(DialogPtr ptrfinestra);
static void SpostaTutto(DialogPtr ptrfinestra)
/* ------------------------------------------------------------------------ *

	SpostaTutto

	8 Dicembre 1995						Stefano

	Descrizione:
	Sposta tutti gli item della dialog

	Parametri:
	ptrfinestra - puntatore alla finestra delle preferenze

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	int i = 0;
	/* indice per il for */
	
	for (i = iPrimoItem; i <= iUltimoItem; i++)
		HideDItem(ptrfinestra, i);
}

static Boolean ControllaValoriGiustificazione(DialogPtr ptrfinestra);
static Boolean ControllaValoriGiustificazione(DialogPtr ptrfinestra)
/* ------------------------------------------------------------------------ *

	ControllaValoriGiustificazione

	8 Dicembre 1995						Stefano

	Descrizione:
	Controllo che i valori associati alla giustificazione siano corretti

	Parametri:
	ptrfinestra - puntatore alla finestra delle preferenze

	Ritorno:
	ritorna FALSE se c'Š un errore

* ------------------------------------------------------------------------ */
{
	int lErrore = 0;
	/* per prendere l'ebventuale errore nella lettura del campo Fixed */
	Fixed lValoreFixed = 0;
	/* per leggere il valore Fixed scritto nella finestra delle preferenze */

	clearerror();

	/* prendo l'incremento massimo per l'interlinea */
	lErrore = fgetfield(ptrfinestra, iFixedIncrementoInterlinea,
						DEFUNITS, 1, &lValoreFixed);
	if (lErrore == 0) {
		giveerror(BADNUMBER);
		SelIText(ptrfinestra, iFixedIncrementoInterlinea, 0, 32767);
		return(FALSE);
	} else {
		rangecheck(lValoreFixed, kMinimoIncrementoInterlinea, kMassimoIncrementoInterlinea,
					DEFUNITS, 1, VALUERANGE, 1);
		if (showerror()) {
			SelIText(ptrfinestra, iFixedIncrementoInterlinea, 0, 32767);
			return(FALSE);
		}
	}
	fsetfield(ptrfinestra, iFixedIncrementoInterlinea, DEFUNITS, 1, lValoreFixed);
	ikFixedIncrementoInterlinea = lValoreFixed;

	/* prendo il decremento massimo per l'interlinea */
	lErrore = fgetfield(ptrfinestra, iFixedDecrementoInterlinea,
						DEFUNITS, 1, &lValoreFixed);
	if (lErrore == 0) {
		giveerror(BADNUMBER);
		SelIText(ptrfinestra, iFixedDecrementoInterlinea, 0, 32767);
		return(FALSE);
	} else {
		rangecheck(lValoreFixed, kMinimoDecrementoInterlinea, kMassimoDecrementoInterlinea,
				   DEFUNITS, 1, VALUERANGE, 1);
		if (showerror()) {
			SelIText(ptrfinestra, iFixedDecrementoInterlinea, 0, 32767);
			return(FALSE);
		}
	}
	fsetfield(ptrfinestra, iFixedDecrementoInterlinea, DEFUNITS, 1, lValoreFixed);
	ikFixedDecrementoInterlinea = lValoreFixed;
		
	/* prendo l'incremento massimo per lo spazio tra i paragrafi */
	lErrore = fgetfield(ptrfinestra, iFixedIncrementoSpazioTraParagrafi,
						DEFUNITS, 1, &lValoreFixed);
	if (lErrore == 0) {
		giveerror(BADNUMBER);
		SelIText(ptrfinestra, iFixedIncrementoSpazioTraParagrafi, 0, 32767);
		return(FALSE);
	} else {
		rangecheck(lValoreFixed, kMinimoIncrementoSpazioTraParagrafi, kMassimoIncrementoSpazioTraParagrafi,
					DEFUNITS, 1, VALUERANGE, 1);
		if (showerror()) {
			SelIText(ptrfinestra, iFixedIncrementoSpazioTraParagrafi, 0, 32767);
			return(FALSE);
		}
	}
	fsetfield(ptrfinestra, iFixedIncrementoSpazioTraParagrafi, DEFUNITS, 1, lValoreFixed);
	ikFixedIncrementoSpazioTraParagrafi = lValoreFixed;
	
	/* prendo il decremento massimo per lo spazio tra i paragrafi */
	lErrore = fgetfield(ptrfinestra, iFixedDecrementoSpazioTraParagrafi,
						DEFUNITS, 1, &lValoreFixed);
	if (lErrore == 0) {
		giveerror(BADNUMBER);
		SelIText(ptrfinestra, iFixedDecrementoSpazioTraParagrafi, 0, 32767);
		return(FALSE);
	} else {
		rangecheck(lValoreFixed, kMinimoDecrementoSpazioTraParagrafi, kMassimoDecrementoSpazioTraParagrafi,
				   DEFUNITS, 1, VALUERANGE, 1);
		if (showerror()) {
			SelIText(ptrfinestra, iFixedDecrementoSpazioTraParagrafi, 0, 32767);
			return(FALSE);
		}
	}
	fsetfield(ptrfinestra, iFixedDecrementoSpazioTraParagrafi, DEFUNITS, 1, lValoreFixed);
	ikFixedDecrementoSpazioTraParagrafi = lValoreFixed;
		
	/* prendo l'incremento massimo per lo spazio tra i necrologi */
	lErrore = fgetfield(ptrfinestra, iFixedIncrementoSpazioTraNecrologi,
						DEFUNITS, 1, &lValoreFixed);
	if (lErrore == 0) {
		giveerror(BADNUMBER);
		SelIText(ptrfinestra, iFixedIncrementoSpazioTraNecrologi, 0, 32767);
		return(FALSE);
	} else {
		rangecheck(lValoreFixed, kMinimoIncrementoSpazioTraNecrologi,
				   kMassimoIncrementoSpazioTraNecrologi,
				   DEFUNITS, 1, VALUERANGE, 1);
		if (showerror()) {
			SelIText(ptrfinestra, iFixedIncrementoSpazioTraNecrologi, 0, 32767);
			return(FALSE);
		}
	}
	fsetfield(ptrfinestra, iFixedIncrementoSpazioTraNecrologi, DEFUNITS, 1, lValoreFixed);
	ikFixedIncrementoSpazioTraNecrologi = lValoreFixed;
	
	/* prendo il decremento massimo per lo spazio tra i necrologi */
	lErrore = fgetfield(ptrfinestra, iFixedDecrementoSpazioTraNecrologi,
						DEFUNITS, 1, &lValoreFixed);
	if (lErrore == 0) {
		giveerror(BADNUMBER);
		SelIText(ptrfinestra, iFixedDecrementoSpazioTraNecrologi, 0, 32767);
		return(FALSE);
	} else {
		rangecheck(lValoreFixed, kMinimoSpazioTraNecrologi, kMassimoSpazioTraNecrologi,
				   DEFUNITS, 1, VALUERANGE, 1);
		if (showerror()) {
			SelIText(ptrfinestra, iFixedDecrementoSpazioTraNecrologi, 0, 32767);
			return(FALSE);
		}
	}	
	fsetfield(ptrfinestra, iFixedDecrementoSpazioTraNecrologi, DEFUNITS, 1, lValoreFixed);
	ikFixedDecrementoSpazioTraNecrologi = lValoreFixed;

	/* prendo lo spazio tra i necrologi */
	lErrore = fgetfield(ptrfinestra, iFixedSpazioTraNecrologi,
						DEFUNITS, 1, &lValoreFixed);
	if (lErrore == 0) {
		giveerror(BADNUMBER);
		SelIText(ptrfinestra, iFixedSpazioTraNecrologi, 0, 32767);
		return(FALSE);
	} else {
		rangecheck(lValoreFixed, kMinimoSpazioTraNecrologi, kMassimoSpazioTraNecrologi,
				   DEFUNITS, 1, VALUERANGE, 1);
		if (showerror()) {
			SelIText(ptrfinestra, iFixedSpazioTraNecrologi, 0, 32767);
			return(FALSE);
		}
	}	
	fsetfield(ptrfinestra, iFixedSpazioTraNecrologi, DEFUNITS, 1, lValoreFixed);
	ikFixedSpazioTraNecrologi = lValoreFixed;
	
	/* verifico che la differenza tra spazio tra annunci e decremento sia uguale o */
	/* maggiore di zero */
	lValoreFixed = ikFixedDecrementoSpazioTraNecrologi;
	rangecheck(lValoreFixed, 0, ikFixedSpazioTraNecrologi, DEFUNITS, 1, VALUERANGE, 1);
	if (showerror()) {
		SelIText(ptrfinestra, iFixedDecrementoSpazioTraNecrologi, 0, 32767);
		return(FALSE);
	}

	return(TRUE);
}

static Boolean ControllaValoriImmagini(DialogPtr ptrfinestra);
static Boolean ControllaValoriImmagini(DialogPtr ptrfinestra)
/* ------------------------------------------------------------------------ *

	ControllaValoriImmagini

	8 Dicembre 1995						Stefano

	Descrizione:
	Controllo che i valori associati alle immagini siano corretti

	Parametri:
	ptrfinestra - puntatore alla finestra delle preferenze

	Ritorno:
	ritorna FALSE se c'Š un errore

* ------------------------------------------------------------------------ */
{
	int lErrore = 0;
	/* per prendere l'ebventuale errore nella lettura del campo Fixed */
	Fixed lValoreFixed = 0;
	/* per leggere il valore Fixed scritto nella finestra delle preferenze */
	
	clearerror();

	/* prendo l'altezza dell'immagini */
	lErrore = fgetfield(ptrfinestra, iFixedAltezzaImmagini,
						DEFUNITS, 1, &lValoreFixed);
	if (lErrore == 0) {
		giveerror(BADNUMBER);
		SelIText(ptrfinestra, iFixedAltezzaImmagini, 0, 32767);
		return(FALSE);
	} else {
		rangecheck(lValoreFixed, kMinimaAltezzaImmagini, kMassimaAltezzaImmagini,
				   DEFUNITS, 1, VALUERANGE, 1);
		if (showerror()) {
			SelIText(ptrfinestra, iFixedAltezzaImmagini, 0, 32767);
			return(FALSE);
		}
	}
	fsetfield(ptrfinestra, iFixedAltezzaImmagini, DEFUNITS, 1, lValoreFixed);
	ikFixedAltezzaImmagini = lValoreFixed;
	
	/* prendo la larghezza dell'immagini */
	lErrore = fgetfield(ptrfinestra, iFixedLarghezzaImmagini,
						DEFUNITS, 1, &lValoreFixed);
	if (lErrore == 0) {
		giveerror(BADNUMBER);
		SelIText(ptrfinestra, iFixedLarghezzaImmagini, 0, 32767);
		return(FALSE);
	} else {
		rangecheck(lValoreFixed, kMinimaLarghezzaImmagini, kMassimaLarghezzaImmagini,
				   DEFUNITS, 1, VALUERANGE, 1);
		if (showerror()) {
			SelIText(ptrfinestra, iFixedLarghezzaImmagini, 0, 32767);
			return(FALSE);
		}
	}
	fsetfield(ptrfinestra, iFixedLarghezzaImmagini, DEFUNITS, 1, lValoreFixed);
	ikFixedLarghezzaImmagini = lValoreFixed;

	/* prendo l'estensione dell'elenco immagini */
	getfield(ptrfinestra, iStrEstensioneElencoImmagini, tmpstr);
	if (strlen(tmpstr) <= 0  || strlen(tmpstr) >= 4) {
		MostraErrore(kErroreEstensione);
		SelIText(ptrfinestra, iStrEstensioneElencoImmagini, 0, 32767);
		return(FALSE);
	}
	setfield(ptrfinestra, iStrEstensioneElencoImmagini, tmpstr);
	tmpstr = strupr( tmpstr );
	strcpy(ikStrEstensioneElencoImmagini, tmpstr);

	/* prendo l'estensione delle immagini */
	getfield(ptrfinestra, iStrEstensioneImmagini, tmpstr);
#if kNEW
	if (strlen(tmpstr) >= 4) {
#else
	if (strlen(tmpstr) <= 0  || strlen(tmpstr) >= 4) {
#endif /* kNEW */
		MostraErrore(kErroreEstensione);
		SelIText(ptrfinestra, iStrEstensioneImmagini, 0, 32767);
		return(FALSE);
	}
	setfield(ptrfinestra, iStrEstensioneImmagini, tmpstr);
	tmpstr = strupr(tmpstr);
	strcpy(ikStrEstensioneImmagini, tmpstr);

	/* prendo la cartella delle immagini */
	getfield(ptrfinestra, iStrCartellaImmagini, tmpstr);
	if (IsCartella(tmpstr) != TRUE)
	{
		MostraErrore(kErrorePresenzaCartellaEditata);
		SelIText(ptrfinestra, iStrCartellaImmagini, 0, 32767);
		return(FALSE);
	}
	strcpy(ikStrCartellaImmagini, tmpstr);


	return(TRUE);
}

static Boolean ControllaValoriDocumento(DialogPtr ptrfinestra);
static Boolean ControllaValoriDocumento(DialogPtr ptrfinestra)
/* ------------------------------------------------------------------------ *

	ControllaValoriDocumento

	16 Dicembre 1995						Stefano

	Descrizione:
	Controllo che i valori associati al documento siano corretti

	Parametri:
	ptrfinestra - puntatore alla finestra delle preferenze

	Ritorno:
	ritorna FALSE se c'Š un errore

* ------------------------------------------------------------------------ */
{
	int lErrore = 0;
	/* per prendere l'eventuale errore nella lettura del campo Fixed */
	Fixed lValoreFixed = 0;
	/* per leggere il valore Fixed scritto nella finestra delle preferenze */
	
	clearerror();

	/* prendo l'altezza dei moduli */
	lErrore = fgetfield(ptrfinestra, iFixedAltezzaModulo,
						DEFUNITS, 1, &lValoreFixed);
	if (lErrore == 0) {
		giveerror(BADNUMBER);
		SelIText(ptrfinestra, iFixedAltezzaModulo, 0, 32767);
		return(FALSE);
	} else {
		rangecheck(lValoreFixed, kMinimaAltezzaModulo, kMassimaAltezzaModulo,
				   DEFUNITS, 1, VALUERANGE, 1);
		if (showerror()) {
			SelIText(ptrfinestra, iFixedAltezzaModulo, 0, 32767);
			return(FALSE);
		}
	}
	fsetfield(ptrfinestra, iFixedAltezzaModulo, DEFUNITS, 1, lValoreFixed);
	ikFixedAltezzaModulo = lValoreFixed;
	
	/* prendo la distanza tra i moduli */
	lErrore = fgetfield(ptrfinestra, iFixedDistanzaModuli,
						DEFUNITS, 1, &lValoreFixed);
	if (lErrore == 0) {
		giveerror(BADNUMBER);
		SelIText(ptrfinestra, iFixedDistanzaModuli, 0, 32767);
		return(FALSE);
	} else {
		rangecheck(lValoreFixed, kMinimaDistanzaModuli, kMassimaDistanzaModuli,
				   DEFUNITS, 1, VALUERANGE, 1);
		if (showerror()) {
			SelIText(ptrfinestra, iFixedDistanzaModuli, 0, 32767);
			return(FALSE);
		}
	}
	fsetfield(ptrfinestra, iFixedDistanzaModuli, DEFUNITS, 1, lValoreFixed);
	ikFixedDistanzaModuli = lValoreFixed;

	/* prendo i moduli per colonna */
	lErrore = fgetfield(ptrfinestra, iFixedModuliPerColonna,
						NUMBER, 0, &lValoreFixed);
	if (lErrore == 0) {
		giveerror(BADNUMBER);
		SelIText(ptrfinestra, iFixedModuliPerColonna, 0, 32767);
		return(FALSE);
	} else {
		rangecheck(lValoreFixed, kMinimaModuliPerColonna, kMassimaModuliPerColonna,
					NUMBER, 0, VALUERANGE, 1);
		if (showerror()) {
			SelIText(ptrfinestra, iFixedModuliPerColonna, 0, 32767);
			return(FALSE);
		}
	}
	fsetfield(ptrfinestra, iFixedModuliPerColonna, NUMBER, 0, lValoreFixed);
	ikFixedModuliPerColonna = lValoreFixed;

	/* prendo le colonne per pagina */
	lErrore = fgetfield(ptrfinestra, iFixedColonnePerPagina,
						NUMBER, 0, &lValoreFixed);
	if (lErrore == 0) {
		giveerror(BADNUMBER);
		SelIText(ptrfinestra, iFixedColonnePerPagina, 0, 32767);
		return(FALSE);
	} else {
		rangecheck(lValoreFixed, kMinimaColonnePerPagina, kMassimaColonnePerPagina,
				   NUMBER, 0, VALUERANGE, 1);
		if (showerror()) {
			SelIText(ptrfinestra, iFixedColonnePerPagina, 0, 32767);
			return(FALSE);
		}
	}
	fsetfield(ptrfinestra, iFixedColonnePerPagina, NUMBER, 0, lValoreFixed);
	ikFixedColonnePerPagina = lValoreFixed;
	
	/* prendo l'altezza testata */
	lErrore = fgetfield(ptrfinestra, iFixedAltezzaTestata,
						DEFUNITS, 1, &lValoreFixed);
	if (lErrore == 0) {
		giveerror(BADNUMBER);
		SelIText(ptrfinestra, iFixedAltezzaTestata, 0, 32767);
		return(FALSE);
	} else {
		rangecheck(lValoreFixed, kMinimaAltezzaTestata, kMassimaAltezzaTestata,
				   DEFUNITS, 1, VALUERANGE, 1);
		if (showerror()) {
			SelIText(ptrfinestra, iFixedAltezzaTestata, 0, 32767);
			return(FALSE);
		}
	}
	fsetfield(ptrfinestra, iFixedAltezzaTestata, DEFUNITS, 1, lValoreFixed);
	ikFixedAltezzaTestata = lValoreFixed;
	
	return(TRUE);
}

static Boolean ControllaValoriDati(DialogPtr ptrfinestra);
static Boolean ControllaValoriDati(DialogPtr ptrfinestra)
/* ------------------------------------------------------------------------ *

	ControllaValoriDati

	16 Dicembre 1995						Stefano

	Descrizione:
	Controllo che i valori associati al documento siano corretti

	Parametri:
	ptrfinestra - puntatore alla finestra delle preferenze

	Ritorno:
	ritorna FALSE se c'Š un errore

* ------------------------------------------------------------------------ */

{
	/* prendo l'estensione della bozza */
	getfield(ptrfinestra, iStrEstensioneBozza, tmpstr);
	if (strlen(tmpstr) <= 0  || strlen(tmpstr) >= 4) {
		MostraErrore(kErroreEstensione);
		SelIText(ptrfinestra, iStrEstensioneBozza, 0, 32767);
		return(FALSE);
	}
	setfield(ptrfinestra, iStrEstensioneBozza, tmpstr);
	tmpstr = strupr( tmpstr );
	strcpy(ikStrEstensioneBozza, tmpstr);

	/* prendo l'estensione dell'ingombro */
	getfield(ptrfinestra, iStrEstensioneIngombro, tmpstr);
	if (strlen(tmpstr) <= 0  || strlen(tmpstr) >= 4) {
		MostraErrore(kErroreEstensione);
		SelIText(ptrfinestra, iStrEstensioneIngombro, 0, 32767);
		return(FALSE);
	}
	setfield(ptrfinestra, iStrEstensioneIngombro, tmpstr);
	tmpstr = strupr( tmpstr );
	strcpy(ikStrEstensioneIngombro, tmpstr);

	/* prendo l'estensione del definitivo */
	getfield(ptrfinestra, iStrEstensioneDefinitivo, tmpstr);
	if (strlen(tmpstr) <= 0  || strlen(tmpstr) >= 4) {
 		MostraErrore(kErroreEstensione);
		SelIText(ptrfinestra, iStrEstensioneDefinitivo, 0, 32767);
		return(FALSE);
	}
	setfield(ptrfinestra, iStrEstensioneDefinitivo, tmpstr);
	tmpstr = strupr( tmpstr );
	strcpy(ikStrEstensioneDefinitivo, tmpstr);

	/* prendo la cartella dei dati */
	getfield(ptrfinestra, iStrCartellaDati, tmpstr);
	if (IsCartella(tmpstr) != TRUE)
	{
		MostraErrore(kErrorePresenzaCartellaEditata);
		SelIText(ptrfinestra, iStrCartellaDati, 0, 32767);
		return(FALSE);
	}
	strcpy(ikStrCartellaDati, tmpstr);

	return(TRUE);
}

//static int FinestraPreferenzeWAP(int opcode,WindowPeek thewptr,int param1);
//static int FinestraPreferenzeWAP(int opcode,WindowPeek thewptr,int param1)
/**************************************************************************/
/*                 	                                                      */
/*  FinestraPreferenzeWAP	                                                      */
/*                                                                        */
//*************************************************************************/
BOOL CALLBACK FinestraPreferenzeWAP( DialogPeek thewptr,
		WORD     opcode,
		PARAM1 	 param1,
		PARAM2	 param2 )


/* ------------------------------------------------------------------------ *

	FinestraPreferenzeWAP

	7 Dicembre 1995						Stefano

	Descrizione:
	WAP di gestione della finestra delle preferenze

	Parametri:
	i paramentri di entrata arrivano da Xpress

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	int lRitorno = FALSE;
	/* per il ritorno della WAP */
	short lBottoneScelto = 0;
	/* per prendere l'identificatore del botto scelto sulla finestra di preferenze */
	Boolean lBool = TRUE;
	/* per controllare i valori Fixed introdotti */
	Fixed lValoreFixed = 0;
//	SFTypeList lSFTypeList;
	/* per la getfile del documento di bozza e di ingombro */
	SFReply lSFReply;
	/* per la getfile del documento di bozza e di ingombro */
	short lVolume = 0;
	/* per la getfullpathname per il documento bozza e di ingombro */
	long lIdentificatoreCartella = 0;
	/* per la getfullpathname per il documento bozza e di ingombro */
	Str255 lDocumentoScelto = "";
	/* per la getfile del documento di bozza e di ingombro */
	BOOL result = TRUE;

	switch (opcode) {
		case WOPEN:
			frontmost = thewptr;
			/* inizializzo le preferenze in base ai volori in memoria */
			/* imposto il record interno delle preferenze */
			ImpostaValoriFinestraPreferenza((DialogPtr) frontmost, gHndlPreferenze);
			(**gHndlFinestraPreferenze) = (**gHndlPreferenze);
			/* sposta tutti gli item */
			SpostaTutto((DialogPtr) frontmost);
			/* visualizza gli item in base al radio button scelto */
			Visualizza((DialogPtr) frontmost, ikRadioTipoPreferenze);
			result = FALSE;
			center_window(thewptr);

//			redisplaywindow(frontmost, gPosizioneFinestraPreferenze, FALSE);
			break;

		case WM_COMMAND:
			lBottoneScelto = param1;
			switch (param1) {
				case iRadioImpaginazione:
				case iRadioGiustificazione:
				case iRadioRiempitivi:
				case iRadioImmagini:
				case iRadioConfigurazione:
				case iRadioBozza:
				case iRadioIngombro:
				case iRadioFinale:
				case iRadioDocumento:
				case iRadioDati:


					if (ikRadioTipoPreferenze == iRadioGiustificazione) {
						/* devo controllore i valori Fixed di giustificazione */
						lBool = ControllaValoriGiustificazione((DialogPtr) thewptr);
						if (lBool == FALSE)
						{
							setradio((DialogPtr) thewptr, 	iRadioImpaginazione,
																		iRadioDati,
																		iRadioGiustificazione);
							ikRadioTipoPreferenze = iRadioGiustificazione;

							break;
						}
					} else if (ikRadioTipoPreferenze == iRadioImmagini) {
						/* devo controllore i valori Fixed delle immagini */
						lBool = ControllaValoriImmagini((DialogPtr) thewptr);
						if (lBool == FALSE)
						{
							setradio((DialogPtr) thewptr, 	iRadioImpaginazione,
																		iRadioDati,
																		iRadioImmagini);
							ikRadioTipoPreferenze = iRadioImmagini;

							break;
						}
					} else if (ikRadioTipoPreferenze == iRadioDocumento) {
						/* devo controllore i valori Fixed del documento */
						lBool = ControllaValoriDocumento((DialogPtr) thewptr);
						if (lBool == FALSE)
						{
							setradio((DialogPtr) thewptr, 	iRadioImpaginazione,
																		iRadioDati,
																		iRadioDocumento);
							ikRadioTipoPreferenze = iRadioDocumento;

							break;
						}
					} else if (ikRadioTipoPreferenze == iRadioDati) {
						/* devo controllore i valori dei dati */
						lBool = ControllaValoriDati((DialogPtr) thewptr);
						if (lBool == FALSE)
						{
							setradio((DialogPtr) thewptr, 	iRadioImpaginazione,
																		iRadioDati,
																		iRadioDati);
							ikRadioTipoPreferenze = iRadioDati;

							break;
						}
					}

					/* imposta il nuovo valore del radio button sul tipo preferenze */
					setradio((DialogPtr) thewptr,
							 iRadioImpaginazione,
							 iRadioDati,
							 lBottoneScelto);
					ikRadioTipoPreferenze = lBottoneScelto;

					/* sposta tutti gli item */
					SpostaTutto((DialogPtr) thewptr);
					/* visualizza gli item in base al radio button scelto */
					Visualizza((DialogPtr) thewptr, lBottoneScelto);
					break;

				case iRadioManuale:
				case iRadioSequenzaDelFile:
				case iRadioPosizionamentoAutomatico:
					/* imposta il nuovo valore del radio button sul tipo impaginazione */
					setradio((DialogPtr) thewptr,
							 iRadioManuale,
							 iRadioPosizionamentoAutomatico,
							 lBottoneScelto);
					ikRadioTipoDefinitiva = lBottoneScelto;
					break;

				case iCheckModificaSpazioTraNecrologi:
					/* imposta il nuovo valoro per il check sulla giustificazione */
					/* con lo spazio tra i necrologi */
					ikCheckModificaSpazioTraNecrologi =
						setcheck((DialogPtr) thewptr,
								 iCheckModificaSpazioTraNecrologi,
								 !ikCheckModificaSpazioTraNecrologi);
					/* controllo se devo visualizza re gli edit text associati */
					/*  allo spazio tra i necrologi */
					VisualizzaSpazioTraNecrologi((DialogPtr) thewptr,
												ikCheckModificaSpazioTraNecrologi);
					break;

				case iCheckModificaSpazioTraParagrafi:
					/* imposta il nuovo valore per il check sulla giustificazione */
					/* con lo spazio tra i paragrafi */
					ikCheckModificaSpazioTraParagrafi = setcheck((DialogPtr) thewptr,
																  iCheckModificaSpazioTraParagrafi,
																  !ikCheckModificaSpazioTraParagrafi);
					/* controllo se devo visualizza re gli edit text associati */
					/*  allo spazio tra i paragrafi */
					VisualizzaSpazioTraParagrafi((DialogPtr) thewptr,
												 ikCheckModificaSpazioTraParagrafi);
					break;

				case iCheckModificaInterlinea:
					/* imposta il nuovo valore per il check sulla giustificazione */
					/* con interlinea */
					ikCheckModificaInterlinea = setcheck((DialogPtr) thewptr,
														 iCheckModificaInterlinea,
														 !ikCheckModificaInterlinea);
					/* controllo se devo visualizza re gli edit text associati */
					/*  all'interlinea */
					VisualizzaInterlinea((DialogPtr) thewptr, ikCheckModificaInterlinea);
					break;

				case iCheckUsaRingraziamenti:
					/* imposta il nuovo valoro per il check sull'uso dei ringraziamenti */
					ikCheckUsaRingraziamenti = setcheck((DialogPtr) thewptr,
														iCheckUsaRingraziamenti,
														!ikCheckUsaRingraziamenti);
					break;

				case iCheckUsaAnniversari:
					/* imposta il nuovo valoro per il check sull'uso degli anniversari */
					ikCheckUsaAnniversari = setcheck((DialogPtr) thewptr,
													 iCheckUsaAnniversari,
													 !ikCheckUsaAnniversari);
					break;

				case iCheckImportaImmagini:
					/* imposta il nuovo valoro per il check sull'importazione delle immagini */
					ikCheckImportaImmagini = setcheck((DialogPtr) thewptr,
												  iCheckImportaImmagini,
												  !ikCheckImportaImmagini);
					break;

				case iBottoneScegliCartellaImmagini:
					/* prendo il nuovo nome della cartella delle immagini */
//					PrendiCartellaSelezionata(kCartellaImmagini, ikStrCartellaImmagini);
					setfield((DialogPtr) thewptr, iStrCartellaImmagini,
							 ikStrCartellaImmagini);
					break;
							
				case iBottoneCarica:
#if kOLD
					lBool = CaricaPreferenze(&gHndlFinestraPreferenze);
					/* cambio le preferenze in base ai volori letti */
					if (lBool == TRUE)
						ImpostaValoriFinestraPreferenza((DialogPtr) thewptr, gHndlFinestraPreferenze);
#else
					MessageBeep(MB_ICONASTERISK);
#endif /* kNEW */

					break;

				case iBottoneRegistra:
#if kOLD
					RegistraPreferenze((DialogPtr) thewptr, gHndlFinestraPreferenze);
#else
					MessageBeep(MB_ICONASTERISK);
#endif /* kNEW */

					break;

				case iRadioStampaSingolo:
				case iRadioStampaAggregati:
					/* imposta il nuovo valore del radio button sul tipo di stampa bozze */
					setradio((DialogPtr) thewptr,
							 iRadioStampaSingolo,
							 iRadioStampaAggregati,
							 lBottoneScelto);
					ikRadioTipoStampaBozze = lBottoneScelto;
					break;

				case iCheckStampaInAutomatico:
					/* imposta il nuovo valore per il check sulla stampa in automatico */
					ikCheckStampaInAutomatico = setcheck((DialogPtr) thewptr,
														 iCheckStampaInAutomatico,
														 !ikCheckStampaInAutomatico);
					break;
					
				case iBottoneScegliDocumentoBozza:
//					lSFTypeList[0] = XDOC;
					LoadString( hinst, ID_STRING2508, (LPSTR)lDocumentoScelto, sizeof(lDocumentoScelto) );

					lBool = GetDocFile( &lSFReply, lDocumentoScelto );
/*					getfile(gStrSelezionaDocumentoBozza,
									0,
									NULL,
									1,
									&lSFTypeList,
									NULL,
									&lSFReply,
									NULL,
									0);

*/
					if (lBool == TRUE) {
						/* Š stato pigianto ok */
						/* cambio il documento bozza campione */
						strcpy(lDocumentoScelto, lSFReply.fName);
//						getfullpath(lDocumentoScelto, lSFReply.vRefNum,
//									&lVolume, &lIdentificatoreCartella);
						strcpy(ikStrDocumentoBozza, lDocumentoScelto);
						setfield((DialogPtr) thewptr, iStrDocumentoBozza, lDocumentoScelto);
					}
					break;

				case iBottoneScegliDocumentoIngombro:
					LoadString( hinst, ID_STRING2510, (LPSTR)lDocumentoScelto, sizeof(lDocumentoScelto) );
					lBool = GetDocFile( &lSFReply, lDocumentoScelto );
/*					lSFTypeList[0] = XDOC;
					lBool = getfile(gStrSelezionaDocumentoIngombro,
									0,
									NULL,
									1,
									&lSFTypeList,
									NULL,
									&lSFReply,
									NULL,
									0);
*/
					if (lBool == TRUE) {
						/* Š stato pigianto ok */
						/* cambio il documento ingombro campione */
						strcpy(lDocumentoScelto, lSFReply.fName);
//						getfullpath(lDocumentoScelto, lSFReply.vRefNum,
//									&lVolume, &lIdentificatoreCartella);
						strcpy(ikStrDocumentoIngombro, lDocumentoScelto);
						setfield((DialogPtr) thewptr, iStrDocumentoIngombro, lDocumentoScelto);
					}
					break;

				case iBottoneScegliDocumentoDefinitivo:
					LoadString( hinst, ID_STRING2515, (LPSTR)lDocumentoScelto, sizeof(lDocumentoScelto) );
					lBool = GetDocFile( &lSFReply, lDocumentoScelto );
/*					lSFTypeList[0] = XDOC;
					lBool = getfile(gStrSelezionaDocumentoIngombro,
									0,
									NULL,
									1,
									&lSFTypeList,
									NULL,
									&lSFReply,
									NULL,
									0);
*/
					if (lBool == TRUE) {
						/* Š stato pigianto ok */
						/* cambio il documento ingombro campione */
						strcpy(lDocumentoScelto, lSFReply.fName);
//						getfullpath(lDocumentoScelto, lSFReply.vRefNum,
//									&lVolume, &lIdentificatoreCartella);
						strcpy(ikStrDocumentoDefinitivo, lDocumentoScelto);
						setfield((DialogPtr) thewptr, iStrDocumentoDefinitivo, lDocumentoScelto);
					}
					break;

				case iRadioPreparaInTesta:
				case iRadioPreparaInCoda:
					/* imposta il nuovo valore del radio button sul tipo di preparazione del documento */
					setradio((DialogPtr) thewptr,
							 iRadioPreparaInTesta,
							 iRadioPreparaInCoda,
							 lBottoneScelto);
					ikRadioTipoPreparazioneDocumento = lBottoneScelto;
					/*
					setradio((DialogPtr) thewptr,
							 iRadioIngombriInAlto,
							 iRadioIngombriInBasso,
							 ikRadioTipoPosizioneIngombri); /**/
					break;

				case iRadioIngombriInAlto:
				case iRadioIngombriInBasso:
					/* imposta il nuovo valore del radio button sul tipo di posizionamento degli ingombri */
					setradio((DialogPtr) thewptr,
							 iRadioIngombriInAlto,
							 iRadioIngombriInBasso,
							 lBottoneScelto);
					ikRadioTipoPosizioneIngombri = lBottoneScelto;
					/*
					setradio((DialogPtr) thewptr,
							 iRadioPreparaInTesta,
							 iRadioPreparaInCoda,
							 ikRadioTipoPreparazioneDocumento); /**/
					break;

				case iBottoneScegliCartellaDati:
					/* prendo il nuovo nome della cartella dei dati */
 //					PrendiCartellaSelezionata(kCartellaDati, ikStrCartellaDati);
					setfield((DialogPtr) thewptr, iStrCartellaDati,
							 ikStrCartellaDati);
					break;
				
				case iCheckImpaginaBozza:
					/* imposta il nuovo valore per il check sull'impaginazione della bozza */
					ikCheckImpaginaBozza = setcheck((DialogPtr) thewptr,
													iCheckImpaginaBozza,
													!ikCheckImpaginaBozza);
					break;

				case iCheckImpaginaIngombro:
					/* imposta il nuovo valore per il check sull'impaginazione dell'ingombro */
					ikCheckImpaginaIngombro = setcheck((DialogPtr) thewptr,
													   iCheckImpaginaIngombro,
													   !ikCheckImpaginaIngombro);
					break;
							
				case iCheckImpaginaDefinitivo:
					/* imposta il nuovo valore per il check sull'impaginazione del definitivo */
					ikCheckImpaginaDefinitivo = setcheck((DialogPtr) thewptr,
														 iCheckImpaginaDefinitivo,
														 !ikCheckImpaginaDefinitivo);
					break;

				case iOk:
					/* devo controllore i valori Fixed di giustificazione */
					lBool = ControllaValoriGiustificazione((DialogPtr) thewptr);
					if (lBool == FALSE) break;

					/* devo controllore i valori Fixed delle immagini */
					lBool = ControllaValoriImmagini((DialogPtr) thewptr);
					if (lBool == FALSE) break;

					/* devo controllore i valori Fixed del documento */
					lBool = ControllaValoriDocumento((DialogPtr) thewptr);
					if (lBool == FALSE) break;

					/* devo controllore i valori dei dati */
					lBool = ControllaValoriDati((DialogPtr) thewptr);
					if (lBool == FALSE) break;

					/* salvo i valori impostati */
					(**gHndlPreferenze) = (**gHndlFinestraPreferenze);

					/* scrivo i valori sul file necro.ini */
					ScriviNecroIni();

					EndDialog(thewptr, NULL);

					break;

				case iAnnulla:

					/* prendo la posizione della finestra */
//					gPosizioneFinestraPreferenze = topLeft(((WindowPtr) thewptr)->portRect);
//					LocalToGlobal(&gPosizioneFinestraPreferenze);
					EndDialog(thewptr, NULL);
					break;
				}
				break;

			default:
			result = FALSE;
		}
		return (result);
}


#if kOLD

void FinestraPreferenze(void)
/* ------------------------------------------------------------------------ *

	FinestraPreferenze

	7 Dicembre 1995						Stefano

	Descrizione:
	Apre la finestra delle preferenze

	Parametri:
	nessuno

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{


//	openwnd((void *) FinestraPreferenzeWAP, NULL );
}

BOOL CALLBACK ImpaginazioneWAP( DialogPeek thewptr, WORD opcode, PARAM1 param1, PARAM2 param2)
/* ------------------------------------------------------------------------ *

	ImpaginazioneWAP

	7 Dicembre 1995						Stefano

	Descrizione:
	WAP di gestione della finestra delle preferenze

	Parametri:
	i paramentri di entrata arrivano da Xpress

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	BOOL result = TRUE;
	short lBottoneScelto = 0;

	switch (opcode) {
		case WOPEN:
			frontmost = thewptr;
			ImpostaValoriFinestraPreferenza((DialogPtr) frontmost, gHndlPreferenze, ikRadioTipoPreferenze);



			center_window( frontmost);
			result = FALSE;
			break;

		case WM_COMMAND:
			lBottoneScelto = param1;
			switch( param1 )
			{
				case iRadioManuale:
				case iRadioSequenzaDelFile:
				case iRadioPosizionamentoAutomatico:
					/* imposta il nuovo valore del radio button sul tipo impaginazione */
					setradio((DialogPtr) thewptr,
							 iRadioManuale,
							 iRadioPosizionamentoAutomatico,
							 lBottoneScelto);
					ikRadioTipoDefinitiva = lBottoneScelto;
					break;

				case iCheckVerificaLista:
					/* imposta il nuovo valoro per il check sulla verifica lista */
					ikCheckVerificaLista = setcheck((DialogPtr) thewptr,
													iCheckVerificaLista,
													!ikCheckVerificaLista);
					break;

			case iOk:
				EndDialog(thewptr, NULL);
			break;

			case iAnnulla:

		/* prendo la posizione della finestra */
//		 gPosizioneFinestraPreferenze = topLeft(((WindowPtr) thewptr)->portRect);
//		 LocalToGlobal(&gPosizioneFinestraPreferenze);
				EndDialog(thewptr, NULL);
			break;
		}
		break;



		default:
		result = FALSE;
	}
	return (result);
}

#endif /* kOLD */

#endif // 0 NECRO v1

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/