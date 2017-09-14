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

// DEFINES
// DEFINES
// DEFINES

/*!
	@defined			RIEMPITIVIDIALOGID
	@abstract		resource id.
	@discussion		risorsa della dialog di preferenze dei riempitivi.
*/
#define RIEMPITIVIDIALOGID					(20030 + LANGUAGE)
#define USAANNIVERSARICHECKID				2
#define USARINGRAZIAMENTICHECKID			3

/*!
	@defined			IMMAGINIDIALOGID
	@abstract		resource id.
	@discussion		risorsa della dialog di preferenze di immagine.
*/
#define IMMAGINIDIALOGID						(20040 + LANGUAGE)
#define IMPORTAZIONEIMGCHECKID				2
#define ALTEZZAIMGEDTID						3
#define LARGHEZZAIMGEDTID					4
#define FOLDERIMGEDTID						5
#define SFOGLIAIMGBTNID						6
#define ESTENSIONEELENCOEDTID				7
#define ESTENSIONEIMGEDTID					8

/*!
	@defined			BOZZADIALOGID
	@abstract		resource id.
	@discussion		risorsa della dialog di preferenze di stampa bozza.
*/
#define BOZZADIALOGID						(20050 + LANGUAGE)
#define ABILITASTAMPAAUTOCHECKID			2
#define STAMPASINGOLARADIOBTNID			3
#define STAMPAAGGREGATIRADIOBTNID			4
#define DOCBOZZAEDTID						5
#define SFOGLIADOCBOZZABTNID				6
#define FOLDERBOZZEEDTID					7
#define SFOGLIAFOLDERBOZZEBTNID				8

/*!
	@defined			INGOMBRODIALOGID
	@abstract		resource id.
	@discussion		risorsa della dialog di preferenze di ingombro.
*/
#define INGOMBRODIALOGID					(20060 + LANGUAGE)
#define DOCINGOMBROEDTID					2
#define SFOGLIADOCINGOMBROBTNID			3
#define FOLDERINGOMBRIEDTID					4
#define SFOGLIAFOLDERINGOMBRIBTNID			5

/*!
	@defined			FINALEDIALOGID
	@abstract		resource id.
	@discussion		risorsa della dialog di preferenze di doc finale.
*/
#define FINALEDIALOGID						(20070 + LANGUAGE)
#define DOCFINALEEDTID						2
#define SFOGLIADOCFINALEBTNID				3
#define PRIMAPAGINARADIOBTNID				4
#define ULTIMAPAGINARADIOBTNID				5
#define ALTOVERSOBASSORADIOBTNID			6
#define BASSOVERSOALTOTADIOBTNID			7
#define FOLDERFINALEEDTID					8
#define SFOGLIAFOLDERFINALEBTNID				9

/*!
	@defined			DOCUMENTODIALOGID
	@abstract		resource id.
	@discussion		risorsa della dialog di preferenze di dimensione doc.
*/
#define DOCUMENTODIALOGID					(20080 + LANGUAGE)
#define ALTEZZAMODULOEDTID					2
#define DISTANZAMODULIEDTID					3
#define MODULICOLANNAEDTID					4
#define COLONNEPAGINAEDTID					5
#define ALTEZZATESTATAEDTID					6

/*!
	@defined			DATIDIALOGID
	@abstract		resource id.
	@discussion		risorsa della dialog di preferenze dati input.
*/
#define DATIDIALOGID							(20090 + LANGUAGE)
#define CARTELLADATIEDTID					2
#define SFOGLIACARTELLADATIBTNID			3
#define ESTENSIONEDOC1EDTID					4
#define ESTENSIONEDOC1CHECKID				5
#define ESTENSIONEDOC2EDTID					6
#define ESTENSIONEDOC2CHECKID				7
#define ESTENSIONEDOC3EDTID					8
#define ESTENSIONEDOC3CHECKID				9

/*!
	@defined			DISPOSIZIONEDIALOGID
	@abstract		resource id.
	@discussion		risorsa della dialog di preferenze di disposizione annunci.
*/
#define DISPOSIZIONEDIALOGID				(21000 + LANGUAGE)
#define VERTICALERADIOBTNID					2
#define ORIZZONTALERADIOBTNID				3

/*!
	@defined			CARTELLEDIALOGID
	@abstract		resource id.
	@discussion		risorsa della dialog di preferenze di selezione cartelle.
*/
#define CARTELLEDIALOGID				(21020 + LANGUAGE)
#define FOLDERTMPEDTID				2
#define SFOGLIAFOLDERTMPBTNID		3
#define FOLDERLASTIMPEDTID			4
#define SFOGLIAFOLDERLASTIMPBTNID	5

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
}ImpaginazionePref, *ImpaginazionePrefPtr, **ImpaginazionePrefHndl;

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
}GiustificazionePref, *GiustificazionePrefPtr, **GiustificazionePrefHndl;

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
	@function			RiempitiviWap
	@abstract		finestra di dialogo di preferenze.
	@discussion		Visualizza la finestra di dialogo per la gestione delle preferenze.

					25 Marzo 2005 - Fabrizio.

	@param			cbparam puntatore a xdwapparamrec record (see XDK Documentation).
	@result			void.
*/
int32 XTAPI RiempitiviWap(xdwapparamptr params) throw();

/*! 
	@function			BozzaWap
	@abstract		finestra di dialogo di preferenze.
	@discussion		Visualizza la finestra di dialogo per la gestione delle preferenze.

					25 Marzo 2005 - Fabrizio.

	@param			cbparam puntatore a xdwapparamrec record (see XDK Documentation).
	@result			void.
*/
int32 XTAPI ImmaginiWap(xdwapparamptr params) throw();

/*! 
	@function			BozzaWap
	@abstract		finestra di dialogo di preferenze.
	@discussion		Visualizza la finestra di dialogo per la gestione delle preferenze.

					25 Marzo 2005 - Fabrizio.

	@param			cbparam puntatore a xdwapparamrec record (see XDK Documentation).
	@result			void.
*/
int32 XTAPI BozzaWap(xdwapparamptr params) throw();

/*! 
	@function			IngombroWap
	@abstract		finestra di dialogo di preferenze.
	@discussion		Visualizza la finestra di dialogo per la gestione delle preferenze.

					25 Marzo 2005 - Fabrizio.

	@param			cbparam puntatore a xdwapparamrec record (see XDK Documentation).
	@result			void.
*/
int32 XTAPI IngombroWap(xdwapparamptr params) throw();

/*! 
	@function			FinaleWap
	@abstract		finestra di dialogo di preferenze.
	@discussion		Visualizza la finestra di dialogo per la gestione delle preferenze.

					25 Marzo 2005 - Fabrizio.

	@param			cbparam puntatore a xdwapparamrec record (see XDK Documentation).
	@result			void.
*/
int32 XTAPI FinaleWap(xdwapparamptr params) throw();

/*! 
	@function			DocumentoWap
	@abstract		finestra di dialogo di preferenze.
	@discussion		Visualizza la finestra di dialogo per la gestione delle preferenze.

					25 Marzo 2005 - Fabrizio.

	@param			cbparam puntatore a xdwapparamrec record (see XDK Documentation).
	@result			void.
*/
int32 XTAPI DocumentoWap(xdwapparamptr params) throw();

/*! 
	@function			DatiWap
	@abstract		finestra di dialogo di preferenze.
	@discussion		Visualizza la finestra di dialogo per la gestione delle preferenze.

					25 Marzo 2005 - Fabrizio.

	@param			cbparam puntatore a xdwapparamrec record (see XDK Documentation).
	@result			void.
*/
int32 XTAPI DatiWap(xdwapparamptr params) throw();

/*! 
	@function			DisposizioneWap
	@abstract		finestra di dialogo di preferenze.
	@discussion		Visualizza la finestra di dialogo per la gestione delle preferenze.

					25 Marzo 2005 - Fabrizio.

	@param			cbparam puntatore a xdwapparamrec record (see XDK Documentation).
	@result			void.
*/
int32 XTAPI DisposizioneWap(xdwapparamptr params) throw();

/*! 
	@function			CartelleWap
	@abstract		finestra di dialogo di preferenze.
	@discussion		Visualizza la finestra di dialogo per la gestione delle preferenze.

					5 Aprile 2005 - Fabrizio.

	@param			cbparam puntatore a xdwapparamrec record (see XDK Documentation).
	@result			void.
*/
int32 XTAPI CartelleWap(xdwapparamptr params) throw();

#endif // Pref_hs
