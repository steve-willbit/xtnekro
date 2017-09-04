/*!
	@header			Paletta.h
	@abstract 		paletta dell'XT.
	@discussion 		Gestione della paletta con le informazioni di impaginazione dell'XT.

					6 Aprile 2005			Fabrizio

					Copyright © Sinedita S.r.l 2005. All Rights Reserved.
					
					$Log: not supported by cvs2svn $					
*/

#ifndef Paletta_h
#define Paletta_h

#ifndef dbpresource_h
#include "DbpResource.h"
#endif // dbpinclude_h

#include<vector>

// DIFINES
// DIFINES
// DIFINES

#define INFOPALETTEID (21030 + LANGUAGE)
#define LISTAINFOID 1
#define EXTENDEDMESSAGETEXTID 2
#define STATICTXTONBARID 3

// CONSTS
// CONSTS
// CONSTS

// numero massimo di righe contenute sulle listbox
#define kMaxNumberOfRow 500

// EXTERN
// EXTERN
// EXTERN

extern bool8 gInfoPaletteShowingFlag;
extern wlochandle gInfoPaletteWLocHandle;
extern xdlgid gInfoPaletteId;
extern std::vector<std::string> gListaPalette;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			Visualizza
	@abstract		paletta di infroazioni.
	@discussion		Visualizza un messaggio sulla paletta di info.
					Richiede una stringa C. La stringa non puo' essere vuota.

					24 Novembre 2003 - Fabrizio Taretto.

	@param			iString stringa da visualizzare sulla paletta.
	@result			nessuno.
*/
void XTAPI Visualizza(const uchar* iString) throw();

/*! 
	@function			MostraNascondiPaletteInfo
	@abstract		mostra o nasconde la paletta informazioni.
	@discussion		mostra o nasconde la paletta informazioni.

					20 Novembre 2003 - Fabrizio Taretto.

	@param			nessuno.
	@result			nessuno.
*/
void XTAPI MostraNascondiPaletteInfo() throw();

/*! 
	@function			OpenInfoPalette
	@abstract		gestione dalla apertura paletta informazioni.
	@discussion		gestisce l'apertura della palette iformazioni di SmartClass.

					20 Novembre 2003 - Fabrizio Taretto.

	@param			nessuno.
	@result			nessuno.
*/
void XTAPI OpenInfoPalette() throw();

/*! 
	@function			InfoPaletteWap
	@abstract		gestione dalla paletta informazioni.
	@discussion		Visualizza le informazioni di impaginazione sulla paletta.

					20 Novembre 2003 - Fabrizio Taretto.

	@param			cbparam pointer to the xdwapparamrec record (see XDK Documentation).
	@result			always noErr.
*/
int32 XTAPI InfoPaletteWap(xdwapparamptr params) throw();

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

/* Procedure */
/* Procedure */
/* Procedure */
/* Costanti */
#define appname "NecroFloatingXT"

extern bool16 		showingflag;
extern wlochandle wlochndl;
extern FARPROC 	wapinst;
extern HWND			waphndl;
extern HWND 	 	WAPPtr;


errorixtension InizializzaPaletta(void);
void ChiudiPaletta(void);
void LoadPaletta(void);
bool16 CALLBACK WAPPaletta(HWND thewptr, int16, PARAM1, PARAM2);
void Visualizza(char *);
void VisualizzaStoLavorando(void);

#endif // 0 NECRO v1
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif // Paletta_h