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
	@defined			PREFERENZEDIALOGID
	@abstract		resource id.
	@discussion		risorsa della dialog di preferenze.
*/
#define PREFERENZEDIALOGID 					(20000 + LANGUAGE)
#define BOTTONEANNULLA 						2
#define LISTAPREFERENZEID 					3
#define TABCONTROLPREFERENZE 				4	

/*!
	@defined			IMPAGINAZIONEDIALOGID
	@abstract		resource id.
	@discussion		risorsa della dialog di preferenze d'impaginazione.
*/
#define IMPAGINAZIONEDIALOGID 				(20010 + LANGUAGE)
#define MANUALERADIOBTNID 					2
#define SEQFILEDATIRADIOBTNID 				3
#define POSAUTORADIOBTNID	 				4

/*!
	@defined			GIUSTIFICAZIONEDIALOGID
	@abstract		resource id.
	@discussion		risorsa della dialog di preferenze di giustificazione.
*/
#define GIUSTIFICAZIONEDIALOGID				(20020 + LANGUAGE)
#define SPAZIONECROCHECKID					2
#define SPAZIONECROVALINIEDTID				3
#define SPAZIONECROINCRMAXEDTID			4
#define SPAZIONECRODECRMAXEDTID			5
#define SPAZIOPARACHECKID					6
#define SPAZIOPARAINCRMAXEDTID				7
#define SPAZIOPARADECRMAXEDTID				8
#define INTERLINEACHECKID					9
#define INTERLINEAINCRMAXEDTID				10
#define INTERLINEADECRMAXEDTID				11

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

/*!
	@defined			ABOUTNECRODIALOGID
	@abstract		resource id.
	@discussion		This is the resource id of the Info dialog.
*/
#define ABOUTNECRODIALOGID (21040 + LANGUAGE)
#define ABOUTNECROLOGOID 1
#define VERSIONID 2
#define ABOUTNECROLOGOBMP 20000


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
	@function			ImpaginazioneWap
	@abstract		finestra di dialogo di preferenze.
	@discussion		Visualizza la finestra di dialogo per la gestione delle preferenze.

					25 Marzo 2005 - Fabrizio.

	@param			cbparam puntatore a xdwapparamrec record (see XDK Documentation).
	@result			void.
*/
int32 XTAPI ImpaginazioneWap(xdwapparamptr params) throw();

/*! 
	@function			GiustificazioneWap
	@abstract		finestra di dialogo di preferenze.
	@discussion		Visualizza la finestra di dialogo per la gestione delle preferenze.

					25 Marzo 2005 - Fabrizio.

	@param			cbparam puntatore a xdwapparamrec record (see XDK Documentation).
	@result			void.
*/
int32 XTAPI GiustificazioneWap(xdwapparamptr params) throw();

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

/*! 
	@function			PreferenzeWap
	@abstract		finestra di dialogo di preferenze.
	@discussion		Visualizza la finestra di dialogo per la gestione delle preferenze.

					25 Marzo 2005 - Fabrizio.

	@param			cbparam puntatore a xdwapparamrec record (see XDK Documentation).
	@result			void.
*/
int32 XTAPI PreferenzeWap(xdwapparamptr params) throw();

/*! 
	@function			AboutNecroWap
	@abstract		finestra di dialogo di visualizzazione crediti.
	@discussion		finestra di dialogo di visualizzazione crediti.

					10 Maggio 2005 - Fabrizio.

	@param			params puntatore a xdwapparamrec record (see XDK Documentation).
	@result			void.
*/
int32 XTAPI AboutNecroWap(xdwapparamptr params) throw();

#endif // Pref_hs

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

/* Costanti */
/* Costanti */
/* Costanti */

/* preferenze esterne */
#define kRadioTipoPreferenze (**gHndlPreferenze).tipoPreferenze
#define kRadioTipoDefinitiva (**gHndlPreferenze).tipoDefinitiva
#define kCheckModificaInterlinea (**gHndlPreferenze).modificaInterlinea
#define kFixedIncrementoInterlinea (**gHndlPreferenze).incrementoInterlinea
#define kFixedDecrementoInterlinea (**gHndlPreferenze).decrementoInterlinea
#define kCheckModificaSpazioTraParagrafi (**gHndlPreferenze).modificaSpazioTraParagrafi
#define kFixedIncrementoSpazioTraParagrafi (**gHndlPreferenze).incrementoSpazioTraParagrafi
#define kFixedDecrementoSpazioTraParagrafi (**gHndlPreferenze).decrementoSpazioTraParagrafi
#define kCheckModificaSpazioTraNecrologi (**gHndlPreferenze).modificaSpazioTraNecrologi
#define kFixedIncrementoSpazioTraNecrologi (**gHndlPreferenze).incrementoSpazioTraNecrologi
#define kFixedDecrementoSpazioTraNecrologi (**gHndlPreferenze).decrementoSpazioTraNecrologi
#define kFixedSpazioTraNecrologi (**gHndlPreferenze).spazioTraNecrologi
#define kCheckUsaRingraziamenti (**gHndlPreferenze).usaRingraziamenti
#define kCheckUsaAnniversari (**gHndlPreferenze).usaAnniversari
#define kCheckImportaImmagini (**gHndlPreferenze).importaImmagini
#define kFixedAltezzaImmagini (**gHndlPreferenze).altezzaImmagini
#define kFixedLarghezzaImmagini (**gHndlPreferenze).larghezzaImmagini
#define kStrCartellaImmagini (**gHndlPreferenze).cartellaImmagini
#define kStrEstensioneElencoImmagini (**gHndlPreferenze).estensioneElencoImmagini
#define kStrEstensioneImmagini (**gHndlPreferenze).estensioneImmagini
#define kRadioTipoStampaBozze (**gHndlPreferenze).tipoStampaBozze
#define kCheckStampaInAutomatico (**gHndlPreferenze).stampaInAutomatico
#define kStrDocumentoBozza (**gHndlPreferenze).documentoBozza
#define kStrDocumentoIngombro (**gHndlPreferenze).documentoIngombro
#define kStrDocumentoDefinitivo (**gHndlPreferenze).documentoDefinitivo
#define kRadioTipoPreparazioneDocumento (**gHndlPreferenze).tipoPreparazioneDocumento
#define kRadioTipoPosizioneIngombri (**gHndlPreferenze).tipoPosizioneIngombri
#define kFixedAltezzaModulo (**gHndlPreferenze).altezzaModulo
#define kFixedDistanzaModuli (**gHndlPreferenze).distanzaModuli
#define kFixedModuliPerColonna (**gHndlPreferenze).moduliPerColonna
#define kFixedColonnePerPagina (**gHndlPreferenze).colonnePerPagina
#define kFixedAltezzaTestata (**gHndlPreferenze).altezzaTestata
#define kStrCartellaDati (**gHndlPreferenze).cartellaDati
#define kStrEstensioneBozza (**gHndlPreferenze).estensioneBozza
#define kCheckImpaginaBozza (**gHndlPreferenze).impaginaBozza
#define kStrEstensioneIngombro (**gHndlPreferenze).estensioneIngombro
#define kCheckImpaginaIngombro (**gHndlPreferenze).impaginaIngombro
#define kStrEstensioneDefinitivo (**gHndlPreferenze).estensioneDefinitivo
#define kCheckImpaginaDefinitivo (**gHndlPreferenze).impaginaDefinitivo

/* preferenze interne */
#define kVersione (**gHndlPreferenze).versione
#define kStrCartellaTmp (**gHndlPreferenze).cartellaTmp
#define kStrCartellaBozze (**gHndlPreferenze).cartellaBozze
#define kStrCartellaIngombri (**gHndlPreferenze).cartellaIngombri
#define kStrCartellaDefinitivi (**gHndlPreferenze).cartellaDefinitivi
#define kStrCartellaDaAppWare (**gHndlPreferenze).cartellaDaAppWare
#define kStrCartellaPerAppWare (**gHndlPreferenze).cartellaPerAppWare
#define kStrDocumentoPerAppWare (**gHndlPreferenze).documentoPerAppWare
#define kStrApplicazioneAppWare (**gHndlPreferenze).applicazioneAppWare
#define kStrCartellaPreferenze (**gHndlPreferenze).cartellaPreferenze
#define kStrCartellaUltimaImpaginazione (**gHndlPreferenze).cartellaUltimaImpaginazione
#define kCheckAbilitaImpaginazione (**gHndlPreferenze).abilitaImpaginazione
#define kCheckImmagineCentrata (**gHndlPreferenze).immagineCentrata
#define kCheckImmagineProporzionata (**gHndlPreferenze).immagineProporzionata
#define kLongIntervalloControllo (**gHndlPreferenze).intervalloControllo
#define kFixedVariazioneAltezza (**gHndlPreferenze).variazioneAltezza
#define kLongIngombroTotale (**gHndlPreferenze).ingombroTotale
#define kFixedAltezzaInterna (**gHndlPreferenze).altezzaInterna

/* preferenze finestra preferenze */
#define ikRadioTipoPreferenze (**gHndlFinestraPreferenze).tipoPreferenze
#define ikRadioTipoDefinitiva (**gHndlFinestraPreferenze).tipoDefinitiva
#define ikCheckModificaInterlinea (**gHndlFinestraPreferenze).modificaInterlinea
#define ikFixedIncrementoInterlinea (**gHndlFinestraPreferenze).incrementoInterlinea
#define ikFixedDecrementoInterlinea (**gHndlFinestraPreferenze).decrementoInterlinea
#define ikCheckModificaSpazioTraParagrafi (**gHndlFinestraPreferenze).modificaSpazioTraParagrafi
#define ikFixedIncrementoSpazioTraParagrafi (**gHndlFinestraPreferenze).incrementoSpazioTraParagrafi
#define ikFixedDecrementoSpazioTraParagrafi (**gHndlFinestraPreferenze).decrementoSpazioTraParagrafi
#define ikCheckModificaSpazioTraNecrologi (**gHndlFinestraPreferenze).modificaSpazioTraNecrologi
#define ikFixedIncrementoSpazioTraNecrologi (**gHndlFinestraPreferenze).incrementoSpazioTraNecrologi
#define ikFixedDecrementoSpazioTraNecrologi (**gHndlFinestraPreferenze).decrementoSpazioTraNecrologi
#define ikFixedSpazioTraNecrologi (**gHndlFinestraPreferenze).spazioTraNecrologi
#define ikCheckUsaRingraziamenti (**gHndlFinestraPreferenze).usaRingraziamenti
#define ikCheckUsaAnniversari (**gHndlFinestraPreferenze).usaAnniversari
#define ikCheckImportaImmagini (**gHndlFinestraPreferenze).importaImmagini
#define ikFixedAltezzaImmagini (**gHndlFinestraPreferenze).altezzaImmagini
#define ikFixedLarghezzaImmagini (**gHndlFinestraPreferenze).larghezzaImmagini
#define ikStrCartellaImmagini (**gHndlFinestraPreferenze).cartellaImmagini
#define ikStrEstensioneElencoImmagini (**gHndlFinestraPreferenze).estensioneElencoImmagini
#define ikStrEstensioneImmagini (**gHndlFinestraPreferenze).estensioneImmagini
#define ikRadioTipoStampaBozze (**gHndlFinestraPreferenze).tipoStampaBozze
#define ikCheckStampaInAutomatico (**gHndlFinestraPreferenze).stampaInAutomatico
#define ikStrDocumentoBozza (**gHndlFinestraPreferenze).documentoBozza
#define ikStrDocumentoIngombro (**gHndlFinestraPreferenze).documentoIngombro
#define ikStrDocumentoDefinitivo (**gHndlFinestraPreferenze).documentoDefinitivo
#define ikRadioTipoPreparazioneDocumento (**gHndlFinestraPreferenze).tipoPreparazioneDocumento
#define ikRadioTipoPosizioneIngombri (**gHndlFinestraPreferenze).tipoPosizioneIngombri
#define ikFixedAltezzaModulo (**gHndlFinestraPreferenze).altezzaModulo
#define ikFixedDistanzaModuli (**gHndlFinestraPreferenze).distanzaModuli
#define ikFixedModuliPerColonna (**gHndlFinestraPreferenze).moduliPerColonna
#define ikFixedColonnePerPagina (**gHndlFinestraPreferenze).colonnePerPagina
#define ikFixedAltezzaTestata (**gHndlFinestraPreferenze).altezzaTestata
#define ikStrCartellaDati (**gHndlFinestraPreferenze).cartellaDati
#define ikStrEstensioneBozza (**gHndlFinestraPreferenze).estensioneBozza
#define ikCheckImpaginaBozza (**gHndlFinestraPreferenze).impaginaBozza
#define ikStrEstensioneIngombro (**gHndlFinestraPreferenze).estensioneIngombro
#define ikCheckImpaginaIngombro (**gHndlFinestraPreferenze).impaginaIngombro
#define ikStrEstensioneDefinitivo (**gHndlFinestraPreferenze).estensioneDefinitivo
#define ikCheckImpaginaDefinitivo (**gHndlFinestraPreferenze).impaginaDefinitivo

/* item della finestra delle preferenze */

#if kNEW

#define iRadioImpaginazione 6
#define iRadioGiustificazione 7
#define iRadioRiempitivi 8
#define iRadioImmagini 9
#define iRadioConfigurazione 10
#define iRadioBozza 11
#define iRadioIngombro 12
#define iRadioFinale 13
#define iRadioDocumento 14
#define iRadioDati 15

#define iPrimoItem 16

#define iStaticImpaginazione (iPrimoItem)
#define iRadioManuale (iPrimoItem + 1)
#define iRadioSequenzaDelFile (iPrimoItem + 2)
#define iRadioPosizionamentoAutomatico (iPrimoItem + 3)

#define iStaticGiustificazione (iPrimoItem + 4)
#define iCheckModificaSpazioTraNecrologi (iPrimoItem + 5)
#define iFixedIncrementoSpazioTraNecrologi (iPrimoItem + 7)
#define iFixedDecrementoSpazioTraNecrologi (iPrimoItem + 9)
#define iCheckModificaSpazioTraParagrafi (iPrimoItem + 10)
#define iFixedIncrementoSpazioTraParagrafi (iPrimoItem + 12)
#define iFixedDecrementoSpazioTraParagrafi (iPrimoItem + 14)
#define iCheckModificaInterlinea (iPrimoItem + 15)
#define iFixedIncrementoInterlinea (iPrimoItem + 17)
#define iFixedDecrementoInterlinea (iPrimoItem + 19)
#define iStaticSpazioTraNecrologi (iPrimoItem + 20)
#define iFixedSpazioTraNecrologi (iPrimoItem + 21)

#define iStaticRiempitivi (iPrimoItem + 22)
#define iCheckUsaRingraziamenti (iPrimoItem + 23)
#define iCheckUsaAnniversari (iPrimoItem + 24)

#define iStaticImmagini (iPrimoItem + 25)
#define iCheckImportaImmagini (iPrimoItem + 26)
#define iFixedAltezzaImmagini (iPrimoItem + 28)
#define iFixedLarghezzaImmagini (iPrimoItem + 30)
#define iBottoneScegliCartellaImmagini (iPrimoItem + 31)
#define iStrCartellaImmagini (iPrimoItem + 33)
#define iStrEstensioneElencoImmagini (iPrimoItem + 35)
#define iStrEstensioneImmagini (iPrimoItem + 37)

#define iStaticConfigurazione (iPrimoItem + 38)
#define iBottoneCarica (iPrimoItem + 39)
#define iBottoneRegistra (iPrimoItem + 40)

#define iStaticBozza (iPrimoItem + 41)
#define iRadioStampaSingolo (iPrimoItem + 42)
#define iRadioStampaAggregati (iPrimoItem + 43)
#define iCheckStampaInAutomatico (iPrimoItem + 44)
#define iBottoneScegliDocumentoBozza (iPrimoItem + 45)
#define iStrDocumentoBozza (iPrimoItem + 47)

#define iStaticIngombro (iPrimoItem + 48)
#define iBottoneScegliDocumentoIngombro (iPrimoItem + 49)
#define iStrDocumentoIngombro (iPrimoItem + 51)

#define iStaticDefinitivo (iPrimoItem + 52)
#define iBottoneScegliDocumentoDefinitivo (iPrimoItem + 53)
#define iStrDocumentoDefinitivo (iPrimoItem + 55)
#define iRadioPreparaInTesta (iPrimoItem + 57)
#define iRadioPreparaInCoda (iPrimoItem + 58)
#define iRadioIngombriInAlto (iPrimoItem + 60)
#define iRadioIngombriInBasso (iPrimoItem + 61)

#define iStaticDocumento (iPrimoItem + 62)
#define iFixedAltezzaModulo (iPrimoItem + 64)
#define iFixedDistanzaModuli (iPrimoItem + 66)
#define iFixedModuliPerColonna (iPrimoItem + 68)
#define iFixedColonnePerPagina (iPrimoItem + 70)
#define iFixedAltezzaTestata (iPrimoItem + 72)

#define iStaticDati (iPrimoItem + 73)
#define iBottoneScegliCartellaDati (iPrimoItem + 74)
#define iStrCartellaDati (iPrimoItem + 76)
#define iStrEstensioneBozza (iPrimoItem + 78)
#define iCheckImpaginaBozza (iPrimoItem + 79)
#define iStrEstensioneIngombro (iPrimoItem + 80)
#define iCheckImpaginaIngombro (iPrimoItem + 81)
#define iStrEstensioneDefinitivo (iPrimoItem + 82)
#define iCheckImpaginaDefinitivo (iPrimoItem + 83)

#define iUltimoItem (iPrimoItem + 83)

#endif /* kNEW */

/* Descrittori */
/* Descrittori */
/* Descrittori */

typedef struct {

	/* preferenze esterne */
	int tipoPreferenze;
	int tipoDefinitiva;
	Boolean usaUltimoIngombro;
	Boolean modificaInterlinea;
	Fixed incrementoInterlinea;
	Fixed decrementoInterlinea;
	Boolean modificaSpazioTraParagrafi;
	Fixed incrementoSpazioTraParagrafi;
	Fixed decrementoSpazioTraParagrafi;
	Boolean modificaSpazioTraNecrologi;
	Fixed incrementoSpazioTraNecrologi;
	Fixed decrementoSpazioTraNecrologi;
	Fixed spazioTraNecrologi;
	Boolean usaRingraziamenti;
	Boolean usaAnniversari;
	Fixed altezzaImmagini;
	Fixed larghezzaImmagini;
	Boolean importaImmagini;
	char estensioneElencoImmagini[4];
	char estensioneImmagini[4];
	char cartellaImmagini[256];
	int tipoStampaBozze;
	Boolean stampaInAutomatico;
	char documentoBozza[256];
	char documentoIngombro[256];
	char documentoDefinitivo[256];
	int tipoPreparazioneDocumento;
	int tipoPosizioneIngombri;
	Fixed altezzaModulo;
	Fixed distanzaModuli;
	Fixed moduliPerColonna;
	Fixed colonnePerPagina;
	Fixed altezzaTestata;
	char cartellaDati[256];
	char estensioneBozza[4];
	Boolean impaginaBozza;
	char estensioneIngombro[4];
	Boolean impaginaIngombro;
	char estensioneDefinitivo[4];
	Boolean impaginaDefinitivo;

	/* preferenze interne */
	int versione;
	char cartellaTmp[256];
	char cartellaBozze[256];
	char cartellaIngombri[256];
	char cartellaDefinitivi[256];
	char cartellaDaAppWare[256];
	char cartellaPerAppWare[256];
	char documentoPerAppWare[32];
	char applicazioneAppWare[256];
	char cartellaPreferenze[256];
	char cartellaUltimaImpaginazione[256];
	char documentoUltimaImpaginazione[32];
	Boolean abilitaImpaginazione;
	Boolean immagineCentrata;
	Boolean immagineProporzionata;
	unsigned long intervalloControllo;
	Fixed variazioneAltezza;
	unsigned long ingombroTotale;
	Fixed altezzaInterna;

} preferenze, *ptrpreferenze, **handlepreferenze;

/* Variabili Globali */
/* Variabili Globali */
/* Variabili Globali */

extern handlepreferenze gHndlPreferenze;

/* Procedure */
/* Procedure */
/* Procedure */

errorixtension InizializzaPreferenze(void);
errorixtension ControllaPreferenze(void);
void ChiudiPreferenze(void);
BOOL CALLBACK  FinestraPreferenzeWAP( DialogPeek, WORD, PARAM1, PARAM2);
BOOL CALLBACK ImpaginazioneWAP( DialogPeek thewptr, WORD, PARAM1, PARAM2);
void FinestraPreferenze(void);

#endif // 0 NECRO v1

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/