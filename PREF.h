/*!
	@header			Preferenze.h
	@abstract		preferenze dell'XT
	@discussion 		Contiene le descrizione delle procedure per la gestione delle preferenze
					del sistema.
	
					25 Marzo 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.2  2005/04/20 13:48:52  taretto
					impaginazione ok
					
					Revision 1.1  2005/04/20 07:14:20  taretto
					importazione files
					
*/

// INCLUDES
// INCLUDES
// INCLUDES

#ifndef Pref_h
#define Pref_h

#ifndef dbpresource_h
#include "DbpResource.h"
#endif // dbpinclude_h

// STRUCT
// STRUCT
// STRUCT

/*!
	@typedef			ImpaginazionePref
	@abstract		preferenze dell' XTension.
	@discussion		preferenze di impaginazione
*/
typedef struct
{
	dlgitemid tipoImpaginazione;	
} ImpaginazionePref, *ImpaginazionePrefPtr, **ImpaginazionePrefHndl;

/*!
	@typedef			GiustificazionePref
	@abstract		preferenze dell' XTension.
	@discussion		preferenze di giustificazione
*/
typedef struct
{
	Fixed valoreIniziale;
	int16 abilitaSpazioTraNecrologi;
	Fixed incrementoMaxSpazioNecrologi;
	Fixed decrementoMaxSpazioNecrologi;
	int16 abilitaSpazioTraParagrafi;
	Fixed incrementoMaxSpazioParagrafi;
	Fixed decrementoMaxSpazioParagrafi;
	int16 abilitaInterlinea;
	Fixed incrementoMaxInterlinea;
	Fixed decrementoMaxInterlinea;
} GiustificazionePref, *GiustificazionePrefPtr, **GiustificazionePrefHndl;

/*!
	@typedef			RiempitiviPref
	@abstract		preferenze dell' XTension.
	@discussion		preferenze di riempitivi 
*/
typedef struct
{
	int16 usaAnniversari;
	int16 usaRingraziamenti;	
} RiempitiviPref, *RiempitiviPrefPtr, **RiempitiviPrefHndl;

/*!
	@typedef			ImmaginiPref
	@abstract		preferenze dell' XTension.
	@discussion		preferenze di immagine 
*/
typedef struct
{
	int16 abilitaImportImag;
	Fixed altezzaImg;
	Fixed larghezzaImg;
	uchar folderImg[MAXPATHNAME];
	uchar estensioneElenco[MAXPATHNAME];
	uchar estensioneImg[MAXPATHNAME];
} ImmaginiPref, *ImmaginiPrefPtr, **ImmaginiPrefHndl;

/*!
	@typedef			BozzaPref
	@abstract		preferenze dell' XTension.
	@discussion		preferenze di bozza
*/
typedef struct
{
	dlgitemid tipoStampaBozza;
	int16 stampaAutomatica;
	uchar docPathBozza[MAXPATHNAME];
	uchar folderPathBozze[MAXPATHNAME];
} BozzaPref, *BozzaPrefPtr, **BozzaPrefHndl;

/*!
	@typedef			IngombroPref
	@abstract		preferenze dell' XTension.
	@discussion		preferenze di ingombro
*/
typedef struct
{
	uchar docPathIngombro[MAXPATHNAME];
	uchar folderPathIngombri[MAXPATHNAME];
} IngombroPref, *IngombroPrefPtr, **IngombroPrefHndl;

/*!
	@typedef			FinalePref
	@abstract		preferenze dell' XTension.
	@discussion		preferenze di doc finale
*/
typedef struct
{
	uchar docPathFinale[MAXPATHNAME];
	uchar folderPathFinale[MAXPATHNAME];
	dlgitemid preparaDocumento;
	dlgitemid posizionamentoIngombri;
	
} FinalePref, *FinalePrefPtr, **FinalePrefHndl;

/*!
	@typedef			DocumentoPref
	@abstract		preferenze dell' XTension.
	@discussion		preferenze di dimensione documento
*/
typedef struct
{
	Fixed altezzaModulo;
	Fixed distanzaModuli;
	int16 moduliColonna;
	int16 colonnePagina;
	Fixed altezzaTestata;
} DocumentoPref, *DocumentoPrefPtr, **DocumentoPrefHndl;

/*!
	@typedef			DatiPref
	@abstract		preferenze dell' XTension.
	@discussion		preferenze di dati 
*/
typedef struct
{
	uchar folderDati[MAXPATHNAME];
	uchar estensione1[5];  // <- solitamente BOZ
	int16 impaginaDocEstensione1;
	uchar estensione2[5]; // <- solitamente ING
	int16 impaginaDocEstensione2; 
	uchar estensione3[5]; // <-solitamente NRC
	int16 impaginaDocEstensione3;
} DatiPref, *DatiPrefPtr, **DatiPrefHndl;

/*!
	@typedef			DisposizionePref
	@abstract		preferenze dell' XTension.
	@discussion		preferenze di disposizione annunci
*/
typedef struct
{
	dlgitemid tipoDisposizione;
}DisposizionePref, *DisposizionePrefPtr,  **DisposizionePrefHndl;

/*!
	@typedef			CartellePref
	@abstract		preferenze dell' XTension.
	@discussion		selezione cartelle utilità
*/
typedef struct
{
	uchar cartellaTemporanei[MAXPATHNAME];
	uchar cartellaLastImp[MAXPATHNAME];
}CartellePref, *CartellePrefPtr,  **CartellePrefHndl;

/*!
	@typedef			XtPreferences
	@abstract		preferenze dell' XTension.
	@discussion		struttura che contiene le preferenze di XTNecro
	@field			impaginazionePref
	@field			giustificazionePref
	@field			riempitiviPref
	@field			immaginiPref
	@field			bozzaPref
	@field			ingombroPref
	@field			finalePref
	@field			documentoPref
	@field			datiPref
	@field			disposizionePref
*/
typedef struct
{
	ImpaginazionePref impaginazionePref;
	GiustificazionePref giustificazionePref;
	RiempitiviPref riempitiviPref;
	ImmaginiPref immaginiPref;
	BozzaPref bozzaPref;
	IngombroPref ingombroPref;
	FinalePref finalePref;
	DocumentoPref documentoPref;
	DatiPref datiPref;
	DisposizionePref disposizionePref;	
	CartellePref cartellePref;
} XtPreferences, *XtPreferencesPtr, **XtPreferencesHandle; 

// EXTERN GLOBALS
// EXTERN GLOBALS
// EXTERN GLOBALS

extern XtPreferences gXtPreferences;

extern double gLongIngombroTotale;

// CONSTS
// CONSTS
// CONSTS

// preferenze interne
#define kCheckAbilitaImpaginazione 		TRUE
#define kCheckImmagineCentrata		TRUE
#define kCheckImmagineProporzionata	TRUE 
#define kLongIntervalloControllo 			20
#define kFixedVariazioneAltezza 			mFixed(1)
#define kFixedAltezzaInterna 			mFixed(20);

// preferenze ricavate
#define kFixedMinimoSpazioTraNecrologi (gXtPreferences.giustificazionePref.valoreIniziale - gXtPreferences.giustificazionePref.decrementoMaxSpazioNecrologi)
#define kFixedMassimoSpazioTraNecrologi (gXtPreferences.giustificazionePref.valoreIniziale + gXtPreferences.giustificazionePref.incrementoMaxSpazioNecrologi)
#define kFixedDifferenzaSpazioTraNecrologi (gXtPreferences.giustificazionePref.incrementoMaxSpazioNecrologi + gXtPreferences.giustificazionePref.decrementoMaxSpazioNecrologi)

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
void XTAPI DeleteEndFilePathSeparator(uchar* ioString) throw();

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
bool8 XTAPI GetFileGenerico(uchar* nomefile, uchar* directorydipartenza, uchar* nomeattualefile, uchar* szTitle,
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
bool8 XTAPI SelezionaFileDialog(uchar* iPercorsoCompleto) throw();

/*!
	@function			MyPickFolder
	@abstract 		pickfolder
	@discussion		Funzione per l'utilizzo corretto della chiamatapickfolder di XPress su MacOs e WinOS -da SmartClass-
	
					30 Marzo 2005 - Taretto Fabrizio.

	@param			iTitle titolo della finestra pickfolder.
	@param			ioPath path di partenza in input, path di ritorno in output.
	@result  			TRUE se e' andato tutto bene, FALSE in tutti i casi di errore
*/
bool8 XTAPI MyPickFolder(uchar* iTitle, uchar* ioPath) throw();

#ifdef _DEBUG
/*!
	@function IniziaPreferenzeDiDebug
*/
void XTAPI IniziaPreferenzeDiDebug() throw();
#endif // _DEBUG


#endif // Pref_hs
