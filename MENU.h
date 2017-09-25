/*!
	@header			Menu.h
	@abstract		menu
	@discussion 		Contiene le procedure per aggiornare e gestire i menu dell'xtension.
	
					23 Marzo 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
*/

#ifndef menu_h
#define menu_h

// DEFINES
// DEFINES
// DEFINES

// menu string resource id
#define MENUSTRID 20000
#define NECROMENUID 						1
#define ABILITAMENUID 					2
#define DISABILITAMENUID 					3
#define PREPARADOCUMENTOMENUID 		4
#define CALCPOSMENUID 					5
#define POSIZIONAMENUID					6
#define RIPRISTINADATIULTIMAMENUID		7
#define RIPRISTINADOCMENUID				8
#define CREAIMGCAMPIONEMENUID			9
#define CONTROLLAIMGMENUID				10
#define IMPORTAIMGMENUID				11
#define ASSOCIAIMGMENUID				12
#define IMPOSTAPAXMENUID				13
#define PREFERENZEMENUID					14
#define MOSTRAIFOPALETTEMENUID			15
#define NASCONDIINFOPALETTEMENUID		16
#define ABOUTMENUID						17

// GLOBALS
// GLOBALS
// GLOBALS

// handle al menu
extern Menu gNecroMenu;

// identificatori di menu
extern int16 gNecroMenuId;
extern uint16 gAbilitaDisabilitaMenuId;
extern uint16 gPreparaDocMenuId;
extern uint16 gCalcPosMenuId;
extern uint16 gPosizionaMenuId;
extern uint16 gRipristinaDatiUltimaMenuId;
extern uint16 gRipristinaDocMenuId;
extern uint16 gCreaImgCampioneMenuId;
extern uint16 gControllaImgMenuId;
extern uint16 gImportaImgMenuId;
extern uint16 gAssociaImgMenuId;
extern uint16 gImpostaPaxMenuId;
extern uint16 gPreferenzeMenuId;
extern uint16 gMostaNascondiPaletteMenuId;
extern uint16 gAboutMenuId;

// globale che contiene lo stato del menu abilita
extern bool8 gFlagAbilita;

// INLINES
// INLINES
// INLINES

inline int DebugMessageBox(LPCSTR p0, LPCSTR p1, LPCSTR p2, LPCSTR p3) {
#ifdef _DEBUG
	HWND hWnd = NULL;
	xtget_hwndmainframe(&hWnd);

	std::string s;
	if ( NULL != p0 ) {
		s.append(p0);
	}
	if ( NULL != p1 ) {
		s.append(p1);
	}
	if ( NULL != p2 ) {
		s.append(p2);
	}
	if ( NULL != p3 ) {
		s.append(p3);
	}

	return MessageBox(hWnd, (LPCSTR) s.c_str(), (LPCSTR) "XTNEKRO", MB_OK);
#endif // _DEBUG
#ifdef NDEBUG
	return(0);
#endif // NDEBUG
}
inline int DebugMessageBox(LPCSTR p0) { return DebugMessageBox(NULL, p0, NULL, NULL); }
inline int DebugMessageBox(LPCSTR p0, LPCSTR p1) { return DebugMessageBox(p0, p1, NULL, NULL); }
inline int DebugMessageBox(LPCSTR p0, LPCSTR p1, LPCSTR p2) { return DebugMessageBox(p0, p1, p2, NULL); }

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			IsAbilitareMenu
	@abstract 		test
	@discussion		Per sapere se si deve abilitare o meno un menu'
					
					6 Aprile 2005 - Taretto Fabrizio.

	@param			comandomenu contiene l'identificatore alla voce da menu da controllare
	@result  			nessuno
*/
bool8 XTAPI IsAbilitareMenu(int32 comandomenu) throw();

/*!
	@function			AggiornaMenu
	@abstract 		gestione menu
	@discussion		Per aggiornare tutte le voci del menu dell'Xtension

					8 Aprile 2005			Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI AggiornaMenu() throw();

/*!
	@function			AbilitaMenu
	@abstract 		gestione menu
	@discussion		Per abilitare un menu

					6 Aprile 2005			Fabrizio.

	@param 			comandomenu	contiene l'identificatore alla voce da menu' da abilitare
	@result  			nessuno
*/
void XTAPI AbilitaMenu(int32 comandomenu) throw();

/*!
	@function			DisabilitaMenu
	@abstract 		gestione menu
	@discussion		Per abilitare un menu

					8 Aprile 2005			Fabrizio.

	@param 			comandomenu	contiene l'identificatore alla voce da menu' da disabilitare
	@result  			nessuno
*/
void XTAPI  DisabilitaMenu(int32  comandomenu) throw();

/*!
	@function			PreparaDocumentoEseguiMenu
*/
void XTAPI PreparaDocumentoEseguiMenu() throw();

/*!
	@function			CalcolaPosizionamentoEseguiMenu
*/
void XTAPI CalcolaPosizionamentoEseguiMenu() throw();

/*!
	@function			PosizionaEseguiMenu
*/
void XTAPI PosizionaEseguiMenu() throw();

/*!
	@function			PaletteInfoEseguiMenu
*/
void XTAPI PaletteInfoEseguiMenu() throw();

/*!
	@function			PreferenzeEseguiMenu
*/
void XTAPI PreferenzeEseguiMenu() throw();

#endif // menu_h

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1
typedef enum {
	kAbilitaDisabilitaImpaginazione = 1,

	kPreparaDocumento = 3,
	kCalcolaPosizionamento = 4,
	kPosiziona = 5,

	kRipristinaDati = 7,
	kRipristinaDocumento = 8,

	kCreaImmagineCampione = 10,
	kControllaImmagini = 11,
	kImpaginaImmagini = 12,
	kAssociaImmagine = 13,
// --> x como
	kImpostaSimboloPax = 15,

	kPreferenze = 17,
// <--	
} comandomenuxt;

/* ordine delle voci di menù dell'Xtension */

/* Costanti */
/* Costanti */
/* Costanti */

#define kMenuNecro 19
/* posizione della stringa da usare nella barra dei menù */
#define kMostraPalettaInfo 20
#define kNascondiPalettaInfo 21
/* per la paletta delle info */

#define kAbilitaImpaginazione 1
#define kDisabilitaImpaginazione 18
/* per il menu di abilitazione dell'impaginazione */

/* Procedure */
/* Procedure */
/* Procedure */

#endif // NECRO v1
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/