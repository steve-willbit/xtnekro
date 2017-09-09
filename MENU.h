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

#if 0

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			addmenuitems_callback
	@abstract 		callback per la chiamata di _XT_ADDMENUITEMS.
	@discussion		XPress chiama questa funzione quando l'XTension deve aggiungere i
					suoi comandi di menu.

					22 marzo 2005 - Fabrizio.

	@param 			cbparam puntatore alla struttura xtaddmenuitemsrec (cfr. la documentazione del XDK).
	@result  			ritorna noErr.
*/
int32 XTAPI addmenuitems_callback(xtaddmenuitemsrec* cbparam);

/*!
	@function			updatemenuitems_callback
	@abstract 		callback for _XT_UPDATEMENUITEMS call.
	@discussion		XPress calls this callback functions when the XTension has to enable or disable the menu items.

					22 marzo 2005 - Fabrizio.

	@param 			cbparam pointer to the xtupdatemenuitemsrec record (see XDK documentation).
	@result  			returns noErr.
*/
int32 XTAPI updatemenuitems_callback(xtupdatemenuitemsrec* cbparam);

/*!
	@function		domenuitem_callback
	@abstract 		callback per la chiamata di _XT_DOMENUITEM.
	@discussion		XPress chiama questa funzione quando l'XTension deve eseguire un
					comando di menu.

					22 marzo 2005 - Fabrizio.

	@param 			cbparam puntatore alla struttura xtdomenuitemrec (cfr. la documentazione del XDK).
	@result  		ritorna noErr.
*/

int32 XTAPI domenuitem_callback(xthandlecommandrec* cbparam);

/*!
	@function		handlecommand_callback
	@abstract 		callback per la chiamata di _XT_HANDLECOMMAND.
	@discussion		XPress chiama questa funzione quando l'XTension deve eseguire un comando di menu.
					Solo in QXP60.

					22 marzo 2005 - Fabrizio.

	@param 			cbparam puntatore alla struttura xthandlecommandrec  (cfr. la documentazione del XDK).
	@result  		ritorna noErr.
*/
int32 XTAPI handlecommand_callback(xthandlecommandrec *cbparam);

/*!
	@function			AggiornaMenu
	@abstract 		gestione menu
	@discussion		Per aggiornare tutte le voci del menu dell'Xtension

					8 Aprile 2005			Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI AggiornaMenu() throw();

#endif // 0

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