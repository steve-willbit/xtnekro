/* ------------------------------------------------------------------------ *
	PreparazioneDocumento.cpp

	25 marzo 2005 - Fabrizio

	Copyright © Sinedita S.r.l 2005. All Rights Reserved.
	
	Descrizione:
	Contiene tutte le procedure per preparare il documento.

	$PreparazioneDocumento.cpp$

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

#include <stdio.h>
#include <string.h>
#include <cassert>
#include <vector>
#include <sstream>

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "Box.h"
#include "DocInfo.h"
#include "Errori.h"
#include "GesSlug.h"
#include "Pref.h"
#include "SpaceCol.h"
#include "XTNecro.h"

#include "PrepDoc.h"

// CONSTS
// CONSTS
// CONSTS

#define kMastroSingolaVuota 1

// TYPEDEF
// TYPEDEF
// TYPEDEF

typedef struct 
{
	int16 colonna;
	int16 moduli;
} infoingombri;

// STATICS GLOBALS
// STATICS GLOBALS
// STATICS GLOBALS

// stringa d'utilita'
static uchar tmpstr[kDimensioneStringhe];

// per memorizzare il numero di moduli per ogni colonna
static infoingombri *gPtrInfoIngombri = NULL;

// per il popup per scegliere il numero delle pagine da creare
static MenuHandle gHndlMenuPopUpPagine = NULL;

// per il popup per associare un mastro alle pagine
static MenuHandle gHndlMenuPopUpPagineMastro = NULL;

// per il popup per scegliere il numero delle colonne da creare
static MenuHandle gHndlMenuPopUpColonne = NULL;

// per il popup per scegliere il numero dei moduli da creare
static MenuHandle gHndlMenuPopUpModuli = NULL;

// per il check per cancellare l'associazione delle pagine mastro
static int16 gCheckCancella = FALSE;

// per il popup per associare un mastro alle pagine
static MenuHandle gHndlMenuPopUpNomiMastro = NULL;

// per il check per fare la compensazione
static int16 gCheckCompensa = FALSE;

// per il popup per scegliere il numero delle colonne della prima pagina
static MenuHandle gHndlMenuPopUpColonnaPrimaPagina = NULL;

// per il popup per scegliere il numero dei moduli della prima pagina
static MenuHandle gHndlMenuPopUpModuliPrimaPagina = NULL;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			ImpostaPreparaDocumento
	@abstract		preparazione documento
	@discussion		Imposta gli item per prepara documento

					21 Aprile 2005 - Fabrizio Taretto.

	@param			nessuno.
	@result			nessuno.
*/
static void XTAPI ImpostaPreparaDocumento() throw();

/*! 
	@function			InserisciColonneDiIngombro
	@abstract		preparazione documento
	@discussion		Inserisco tutti i box di ingombro

					26 Aprile 2005 - Fabrizio Taretto.

	@param			nessuno.
	@result			nessuno.
*/
static void XTAPI InserisciColonneDiIngombro() throw();

/*! 
	@function			CancelloBoxIngombro
	@abstract		preparazione documento
	@discussion		Cancello tutti i box di ingombro

					21 Aprile 2005 - Fabrizio Taretto.

	@param			nessuno.
	@result			nessuno.
*/
static void XTAPI CancelloBoxIngombro() throw();

/*! 
	@function			InserisciPagineColonneEModuli
	@abstract		preparazione documento
	@discussion		Inserisce le pagine al documento corrente. Il documento finale sara' sempre
					formato dai parametri passati. Se il documento ha gia' piu' di una pagina le pagine
					in piu' vengono cancellate

					27 Aprile 2005 - Fabrizio Taretto.

	@param			pagine - pagine da inserire
	@param			colonne - colonne da inserire
	@param			moduli - moduli da inserire
	@param			cancella - per sapere se cancellare l'associazione alle pagine mastro.
	@result			nessuno.
*/
static void XTAPI InserisciPagineColonneEModuli(int32 pagine, int32 colonne, int32 moduli, Boolean cancella) throw();

/*! 
	@function			ModificoIngombroPrimaPagina
	@abstract		preparazione documento
	@discussion		Togli dall'ingombro della prima pagina i moduli passati.

					27 Aprile 2005 - Fabrizio Taretto.

	@param			moduli - moduli da togliere
	@result			nessuno.
*/
static void XTAPI ModificoIngombroPrimaPagina(int16 moduli) throw();

/*! 
	@function			AssociaPaginaMastro
	@abstract		preparazione documento
	@discussion		Associa una pagina mastro ad una pagina del documento

					27 Aprile 2005 - Fabrizio Taretto.

	@param			pagina - pagina a cui applicare la mastro
	@param			mastro - mastro da applicare
	@param			compensa - TRUE se si deve compensare sulla prima pagina
	@result			nessuno.
*/
static void XTAPI AssociaPaginaMastro(int16 pagina, int16 mastro, Boolean compensa) throw();

/*! 
	@function			ModificaPrimaPagina
	@abstract		preparazione documento
	@discussion		Modifica la prima pagina inserendo gli ingombri necessari

					27 Aprile 2005 - Fabrizio Taretto.

	@param			colonna - colonna in cui inserire l'ingombro
	@param			moduli - numero dei moduli da inserire
	@result			nessuno.
*/
static void XTAPI ModificaPrimaPagina(int16 colonna, int16 moduli) throw();

/*! 
	@function			ImpostaPagineMastro
	@abstract		preparazione documento
	@discussion		Imposta gli item per associare un mastro alle pagine

					21 Aprile 2005 - Fabrizio Taretto.

	@param			ptrfinestra puntatore alla finestra.
	@result			nessuno.
*/
static void XTAPI ImpostaPagineMastro() throw();

/*! 
	@function			ImpostaPrimaPagina
	@abstract		preparazione documento
	@discussion		Imposta gli item per modificare la prima pagina

					22 Aprile 2005 - Fabrizio Taretto.

	@param			ptrfinestra puntatore alla finestra.
	@result			nessuno.
*/
static void XTAPI ImpostaPrimaPagina() throw();

/*! 
	@function			AggiornaPagine
	@abstract		preparazione documento
	@discussion		Aggiorna il popup per dire quante pagine inserire nella prepara documento

					22 Aprile 2005 - Fabrizio Taretto.

	@param			nessuno
	@result			nessuno.
*/
static void XTAPI AggiornaPagine() throw();

/*! 
	@function			AggiornaPagineMastro
	@abstract		preparazione documento
	@discussion		Aggiorna il popup per associare alle pagine del documento crrete le pagine mastro

					22 Aprile 2005 - Fabrizio Taretto.

	@param			param2 item da aggiornare
	@result			nessuno.
*/
static void XTAPI AggiornaPagineMastro() throw();

/*! 
	@function			CercaBoxIngombro
	@abstract		preparazione documento
	@discussion		Cerca sul documento corrente i box di ingombro.

					21 Aprile 2005 - Fabrizio Taretto.

	@param			nessuno.
	@result			nessuno.
*/
static void XTAPI CercaBoxIngombro() throw();


// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	ImpostaPreparaDocumento

* ------------------------------------------------------------------------ */
static void XTAPI ImpostaPreparaDocumento() throw()
{
	// indice per scorrere le voci del menu
	int16 lColonneDoc;
	int16 lIndiceVoci = 0;
	// per calcolare il numero di pagine da inserire
	int16 lNumeroPagine = 0;
	// per calcolare le pagine di ingombro
	uint32 lPagine = 0;
	// per calcolare le colonne di ingombro
	uint32 lColonne = 0;
	// per calcolare i moduli di ingombro
	uint32 lModuli = 0;
	// per corregere i moduli da preparare
	Fixed lCorrezione = 0; 
	
	// calcolo le pagine, colonne e moduli
	CalcolaPagineColonneEModuli(gLongIngombroTotale,
								&lPagine,
								&lColonne,
								NULL,
								&lModuli);
								
	// calcolo la correnzione dei moduli
	lCorrezione = gXtPreferences.documentoPref.moduliColonna * gXtPreferences.documentoPref.altezzaModulo;
	lCorrezione -= PrendiAltezzaColonna();
	lCorrezione *= (lColonne + 1) * lPagine * PrendiNumeroColonne();

	assert(0 != gXtPreferences.documentoPref.altezzaModulo);

	lCorrezione /= gXtPreferences.documentoPref.altezzaModulo;

	lColonneDoc =  PrendiNumeroColonne();

	// calcolo pagine colonne e moduli
	assert(lColonneDoc * gXtPreferences.documentoPref.moduliColonna);

	lPagine = lModuli / (lColonneDoc * gXtPreferences.documentoPref.moduliColonna);
	lModuli -= (lPagine * lColonneDoc * gXtPreferences.documentoPref.moduliColonna);

	assert(0 != gXtPreferences.documentoPref.moduliColonna);

	lColonne = lModuli / gXtPreferences.documentoPref.moduliColonna;
	lModuli -= (lColonne * gXtPreferences.documentoPref.moduliColonna);
	
	// controllo quante pagine inserire nel menu
	lNumeroPagine = (numpages() > lPagine ? numpages() : lPagine);							
								
	// aggiungo le voci al menu' delle pagine
	std::vector<int32> listaPagine;
	for (lIndiceVoci = 0; lIndiceVoci <= lNumeroPagine + 1; lIndiceVoci++) 
	{
		listaPagine.push_back(lIndiceVoci);
	}								
	for (int16 i=0; i < listaPagine.size(); i++)
	{
		// per conversione interi
		std::ostringstream listValueStr;
		listValueStr << listaPagine.at(i);
		xrow row;
		// disabilito il redraw
		xd_lst_enableupdates(PAGINEPREPDOCPOPID, TRUE);
		xd_lst_addrow(PAGINEPREPDOCPOPID, IR_BOTTOM ,&row);
		xd_lst_setrowtext(PAGINEPREPDOCPOPID, row, (uchar*) listValueStr.str().c_str());	
		xd_lst_enableupdates(PAGINEPREPDOCPOPID, FALSE);
	}
	// setto la selezione
	if (lPagine != 0)
		xd_lst_setselection(PAGINEPREPDOCPOPID, lPagine);
	else
		xd_lst_setselection(PAGINEPREPDOCPOPID, 0);
	
	// aggiungo le voci al menu delle colonne
	std::vector<int32> listaColonne;
	for (lIndiceVoci = 0; lIndiceVoci < PrendiNumeroColonne(); lIndiceVoci++) 
	{
		listaColonne.push_back(lIndiceVoci);
	}								
	for (int16 i=0; i < listaColonne.size(); i++)
	{
		// per conversione interi
		std::ostringstream listValueStr;
		listValueStr << listaColonne.at(i);
		xrow row;
		xd_lst_enableupdates(COLONNEPREPDOCPOPID, TRUE);
		xd_lst_addrow(COLONNEPREPDOCPOPID, IR_BOTTOM ,&row);
		xd_lst_setrowtext(COLONNEPREPDOCPOPID, row, (uchar*) listValueStr.str().c_str());	
		xd_lst_enableupdates(COLONNEPREPDOCPOPID, false);
	}
	// setto la selezione
	if (lColonne >= PrendiNumeroColonne()) 
		xd_lst_setselection(COLONNEPREPDOCPOPID, 0);
	else
		xd_lst_setselection(COLONNEPREPDOCPOPID, lColonne);
	
	// aggiungo le voci al menu dei moduli
	std::vector<int32> listaModuli;
	for (lIndiceVoci = 0; lIndiceVoci < gXtPreferences.documentoPref.moduliColonna; lIndiceVoci++) 
	{
		listaModuli.push_back(lIndiceVoci);
	}								
	for (int16 i=0; i < listaModuli.size(); i++)
	{
		// per conversione interi
		std::ostringstream listValueStr;
		listValueStr << listaModuli.at(i);
		xrow row;
		xd_lst_enableupdates(MODULIPREPDOCPOPID, TRUE);
		xd_lst_addrow(MODULIPREPDOCPOPID, IR_BOTTOM ,&row);
		xd_lst_setrowtext(MODULIPREPDOCPOPID, row, (uchar*) listValueStr.str().c_str());	
		xd_lst_enableupdates(MODULIPREPDOCPOPID, FALSE);
	}
	// setto la selezione
	if (lModuli >= gXtPreferences.documentoPref.moduliColonna)
		xd_lst_setselection(MODULIPREPDOCPOPID, 1);
	else
		xd_lst_setselection(MODULIPREPDOCPOPID, lModuli);
	
	// imposto il check per la cancellazione delle mastro
	xd_btnchk_setstate(CANCELLAMASTROCHECKID, gCheckCancella);
	
} // ImpostaPreparaDocumento

/* ------------------------------------------------------------------------ *

	InserisciColonneDiIngombro

* ------------------------------------------------------------------------ */
static void XTAPI InserisciColonneDiIngombro() throw()
{
	// indice per scorrere le colonne
	int16 lColonna = 0;
	// per prendere le info sugli ingombri
	infoingombri *lPtrInfoIngombri = NULL;
	
	for (lColonna = 0; lColonna < PrendiNumeroColonne(); lColonna++) 
	{
		// imposto il puntatore
		lPtrInfoIngombri = gPtrInfoIngombri + lColonna;

		if ((*lPtrInfoIngombri).moduli == 0) 
		{
			(*lPtrInfoIngombri).colonna = 0;
		} 
		else 
		{
			// inserisco l'ingombro
			(*lPtrInfoIngombri).colonna = lColonna + 1;
			InserisciColonnaDiIngombro(lColonna + 1, (*lPtrInfoIngombri).moduli);
		}
	}
	xtset_curbox(NULL);
} // InserisciColonneDiIngombro

/* ------------------------------------------------------------------------ *

	CancelloBoxIngombro

* ------------------------------------------------------------------------ */
static void XTAPI CancelloBoxIngombro() throw()
{
	for (int16 sprd = 1; sprd <= numsprds();sprd++) 
	{
		boxid theBox = firstbox(sprd, TRUE);
		while(theBox != NULL)
		{
			if (hasslug(theBox, OBJECTSLUG, kSlugDiIngombro))
			{
				boxid boxToDelete = theBox;
				theBox = nextbox(boxToDelete, TRUE);
				deletebox(boxToDelete, TRUE);
			}
			else
				theBox = nextbox(theBox, TRUE);
		}
	}
	
} // CancelloBoxIngombro

/* ------------------------------------------------------------------------ *

	InserisciPagineColonneEModuli

* ------------------------------------------------------------------------ */
static void XTAPI InserisciPagineColonneEModuli(int32 pagine, int32 colonne, int32 moduli, Boolean cancella) throw()
{
	// per sapere il risultato dell'inserimento delle pagine
	Boolean lRisultato = FALSE;
	// per scorrerele colonne
	int16 lColonna = 0;
	// per prendere le info sugli ingombri
	infoingombri *lPtrInfoIngombri = NULL;
	// numero delle colonne di ingombro
	int16 lNumeroColonneDiIngombro = 0;
	// numero dei moduli di ingombro
	int16 lNumeroModuliDiIngombro = 0;
	// per scorrerele le pagine
	int16 lPagina = 0;
	
	// controllo quante pagine devo inserire
	if (colonne != 0 || moduli != 0) 
		pagine++;

	// controllo se devo inserire o cancellare delle pagine
	if (pagine < numpages()) 
	{
		// devo cancellare delle pagine
		deletepages(pagine + 1, numpages(), 0);
	} 
	else if (pagine > numpages()) 
	{
		// devo aggiungere delle pagine
		 DocID curDocID = INVALDOC;
		 int16 numPages = -1;
		 DocRef DRef;
		 xtget_curdoc(&curDocID);
		 DRef = XTGetDocRefFromDocID(curDocID);
		 XTNumPages(DRef,FALSE,&numPages);
		lRisultato = XTInsertPages(DRef, numPages, pagine - numPages, APPLYDEFAULT+RUNDONE/*8*/, NULL, kPaginaMastroA);
		// lRisultato = insertpages2(numpages(), pagine - numpages(),  8, NULL, kPaginaMastroA);
		if (lRisultato != noErr) 
		{
			// visualizzo l'errore
			MostraErrore(kErroreInserimentoPagine);
			return;
		}
	}

	// cancello tutti i box di ingombro sulla prima pagina
	CancelloBoxIngombro();

	// vado sulla pagina dove creo gli ingombri
	if (gXtPreferences.finalePref.preparaDocumento == PRIMAPAGINARADIOBTNID)
	{
		gotopage(kPrimaPagina);
	}
	else
	{
		gotopage(numpages());
	}

	if (colonne != 0 || moduli != 0) 
	{
		// calcolo quanti box di colonna inserire
		if (colonne != 0 || moduli != 0) 
		{
			lNumeroColonneDiIngombro = PrendiNumeroColonne() - colonne;
			if (moduli != 0) 
				lNumeroColonneDiIngombro--;
		}

		// calcolo quando deve essere alto il box su una colonna
		if (moduli != 0) 
		{
			lNumeroModuliDiIngombro = gXtPreferences.documentoPref.moduliColonna - moduli;
		}

		// calcolo i moduli per le colonne
		for (lColonna = 0; lColonna < PrendiNumeroColonne(); lColonna++) 
		{
			// controllo se inserisco in coda o in testa
			if (gXtPreferences.finalePref.preparaDocumento == PRIMAPAGINARADIOBTNID)
			{
				// imposto il puntatore
				lPtrInfoIngombri = gPtrInfoIngombri + lColonna;
			}
			else
			{
				// imposto il puntatore
				lPtrInfoIngombri = gPtrInfoIngombri + (PrendiNumeroColonne() - lColonna - 1);
			}

			if (lColonna < lNumeroColonneDiIngombro)
				(*lPtrInfoIngombri).moduli = gXtPreferences.documentoPref.moduliColonna;
			else if (lColonna == lNumeroColonneDiIngombro)
				(*lPtrInfoIngombri).moduli = lNumeroModuliDiIngombro;
			else (*lPtrInfoIngombri).moduli = 0;
		}

		// inserisco i box di ingombro
		InserisciColonneDiIngombro();

	}

	// aggiorno le palette
	updatepagepalette();

	if (cancella == TRUE) 
	{
		// tolgo l'associazione alle pagine mastro
		for (lPagina = 2; lPagina <= numpages(); lPagina++) 
		{
			// applico la pagina mastro vuota
			applymaster2pages(kMastroSingolaVuota, lPagina, lPagina);
		}
	}
} // InserisciPagineColonneEModuli

/* ------------------------------------------------------------------------ *

	AssociaPaginaMastro
	
* ------------------------------------------------------------------------ */
static void XTAPI AssociaPaginaMastro(int16 pagina, int16 mastro, Boolean compensa) throw()
{
	// per calcolare quanti moduli sone presenti sulla pagina
	int16 lNumeroModuliPagina = 0;
	// per calcolare quanti moduli sono presenti sulla pagina mastro
	int16 lNumeroModuliPaginaMastro = 0;
	// per calcolare quanti moduli devo inserire
	int16 lNumeroModuliDaAggiungere = 0;
	// per calcolare quanti moduli sono presenti sulla prima magina
	int16 lNumeroModuliPrimaPagina = 0;
	// posizione del mastro da applicare
	int16 lMastro = 0;
	// per scorrerele colonne
	int16 lColonna = 0;
	// per prendere le info sugli ingombri
	infoingombri *lPtrInfoIngombri = NULL;
	
	// alla prima pagina non posso cambiare il mastro
	 if (pagina == 1) 
	 {
	 	MostraErrore(kErroreAssociaMastro);
	 	return;
	 }
	
	// calcolo la posizione della pagina mastro
	lMastro = mastro + kPaginaMastroA - 1;
	
	// controllo se devo compensare
	if (compensa == TRUE) 
	{
		// prendo l'ingombro della pagina a cui applicare la pagina mastro
		PrendiModuliPagina(pagina, &lNumeroModuliPagina);

		// ricerca dei box presenti sulla pagina mastro
		PrendiModuliPaginaMastro(lMastro, &lNumeroModuliPaginaMastro);
		lNumeroModuliDaAggiungere = lNumeroModuliPagina - lNumeroModuliPaginaMastro;

		// determinare quanti moduli sono presenti sulla prima pagina 
		for (lColonna = 0; lColonna < PrendiNumeroColonne(); lColonna++) 
		{
			// imposto il puntatore
			lPtrInfoIngombri = gPtrInfoIngombri + lColonna;
			lNumeroModuliPrimaPagina += (*lPtrInfoIngombri).moduli;
		}

		// controllo che sia possibile compensare
		if ((lNumeroModuliDaAggiungere + lNumeroModuliPrimaPagina) >
			(PrendiNumeroColonne() * gXtPreferences.documentoPref.moduliColonna)) 
		{
			// visualizzo l'errore
			MostraErrore(kImpossibileCompensare);
			return;
		}

		if (lNumeroModuliDaAggiungere < 0 &&
			(lNumeroModuliDaAggiungere * -1) > lNumeroModuliPrimaPagina) 
		{
			// non e' possibile compensare

			// visualizzo l'errore
			MostraErrore(kImpossibileCompensare);
			return;
		}

		// modifico l'ingombro della prima pagina
		ModificoIngombroPrimaPagina(lNumeroModuliDaAggiungere);
	}

	// applico la pagina mastro 
	applymaster2pages(lMastro, pagina, pagina);

	// aggiorno le palette
	updatepagepalette();
} // AssociaPaginaMastro

/* ------------------------------------------------------------------------ *

	ModificoIngombroPrimaPagina

* ------------------------------------------------------------------------ */
static void XTAPI ModificoIngombroPrimaPagina(int16 moduli) throw()
{
	// per scorrerele colonne
	int16 lColonna = 0;
	// per prendere le info sugli ingombri
	infoingombri *lPtrInfoIngombri = NULL;
	// contiene i moduli per la colonna
	int16 lModuliPerColonna = 0;
	// moduli da compensare
	int16 lModuli = 0;
	
	// non devo fare alcuna modifica
	if (moduli == 0) 
		return;
	lModuli = moduli;
	
	// cancello tutti i box di ingombro sulla prima pagina
	CancelloBoxIngombro();
	
	if (lModuli < 0) 
	{
		// devo compensare restringendo
		for (lColonna = PrendiNumeroColonne() - 1; lColonna >= 0; lColonna--) 
		{
			// imposto il puntatore
			lPtrInfoIngombri = gPtrInfoIngombri + lColonna;
			
			lModuliPerColonna = (*lPtrInfoIngombri).moduli;
			if (lModuliPerColonna == 0) continue;
			
			lModuliPerColonna += lModuli;
			if (lModuliPerColonna < 0) 
			{
				(*lPtrInfoIngombri).moduli = 0;
				lModuli = lModuliPerColonna;
			} 
			else 
			{
				(*lPtrInfoIngombri).moduli = lModuliPerColonna;
				break;
			}
		}
	} 
	else 
	{
		// devo compensare allargando
		for (lColonna = 0; lColonna < PrendiNumeroColonne(); lColonna++) 
		{
			// imposto il puntatore
			lPtrInfoIngombri = gPtrInfoIngombri + lColonna;

			lModuliPerColonna = (*lPtrInfoIngombri).moduli;
			if (lModuliPerColonna == gXtPreferences.documentoPref.moduliColonna) continue;

			lModuliPerColonna += lModuli;
			if (lModuliPerColonna > gXtPreferences.documentoPref.moduliColonna) 
			{
				(*lPtrInfoIngombri).moduli = gXtPreferences.documentoPref.moduliColonna;
				lModuli = lModuliPerColonna - gXtPreferences.documentoPref.moduliColonna;
			} 
			else 
			{
				(*lPtrInfoIngombri).moduli = lModuliPerColonna;
				break;
			}
		}
	}

	// inserisco i box di ingombro
	InserisciColonneDiIngombro();
	
} // ModificoIngombroPrimaPagina

/* ------------------------------------------------------------------------ *

	ModificaPrimaPagina

* ------------------------------------------------------------------------ */
static void XTAPI ModificaPrimaPagina(int16 colonna, int16 moduli) throw()
{
	// per prendere le info sugli ingombri
	infoingombri *lPtrInfoIngombri = NULL;
	// numero dei moduli di ingombro
	int16 lNumeroModuliDiIngombro = 0;

	// cancello tutti i box di ingombro sulla prima pagina
	CancelloBoxIngombro();

	// imposto il puntatore
	lPtrInfoIngombri = gPtrInfoIngombri + colonna - 1;

	// calcolo i moduli di ingombro
	lNumeroModuliDiIngombro = gXtPreferences.documentoPref.moduliColonna - moduli;

	// inserisco il nuovo valore della colonna
	(*lPtrInfoIngombri).moduli = lNumeroModuliDiIngombro;

	// inserisco i box di ingombro
	InserisciColonneDiIngombro();
} // ModificaPrimaPagina

/* ------------------------------------------------------------------------ *

	ImpostaPagineMastro

* ------------------------------------------------------------------------ */
static void XTAPI ImpostaPagineMastro() throw()
{
	// indice per scorrere le voci del menu
	int16 lIndiceVoci = 0;
	// per contare le pagine mastro
	int16 lNumeroNomiMastro = 0;

	// aggiungo una pagina al menu
	xrow row;
	xd_lst_enableupdates(PAGINEMASTROPOPID, TRUE);
	xd_lst_addrow(PAGINEMASTROPOPID, IR_BOTTOM ,&row);
	xd_lst_setrowtext(PAGINEMASTROPOPID, row, (uchar*) "1");
	xd_lst_enableupdates(PAGINEMASTROPOPID, FALSE);
	xd_lst_setselection(PAGINEMASTROPOPID, 0);

	// aggiungo i nomi delle pagine mastro
	lNumeroNomiMastro = nummastersprds();
	std::vector<std::string> listaMastri;
	for (lIndiceVoci = kPaginaMastroA; lIndiceVoci <= lNumeroNomiMastro; lIndiceVoci++) 
	{
		tmpstr[0] = '\0';
		getmastername(lIndiceVoci, tmpstr);
		std::string tmpString;
		tmpString = (char*)tmpstr;
		listaMastri.push_back(tmpString);
	}
	
	for (int16 i=0; i < listaMastri.size(); i++)
	{
		xrow row;
		xd_lst_enableupdates(MASTROPOPID, TRUE);
		xd_lst_addrow(MASTROPOPID, IR_BOTTOM ,&row);
		xd_lst_setrowtext(MASTROPOPID, row, (uchar*) listaMastri.at(i).c_str());	
		xd_lst_enableupdates(MASTROPOPID, FALSE);
	}

	// seleziono la voce di menu
	xd_lst_setselection(MASTROPOPID, 0);

	// imposto il check della compensazione
	xd_btnchk_setstate(COMPENSAZIONECHECKID, gCheckCompensa);

	// disabilito il bottone di associa
	if (listaMastri.size() <= 1)
		xd_setcontrolstate(ASSOCIABTNID, XCS_DISABLED);
	else
		xd_setcontrolstate(ASSOCIABTNID, XCS_ENABLED);

} // ImpostaPagineMastro

/* ------------------------------------------------------------------------ *

	ImpostaPrimaPagina

* ------------------------------------------------------------------------ */
static void XTAPI ImpostaPrimaPagina() throw()
{
	// indice per scorrere le voci del menu
	int16 lIndiceVoci = 0;
	
	// aggiungo le voci al menu delle colonne della prima pagina
	std::vector<int32> listaColonneIngombri;
	for (lIndiceVoci = 1; lIndiceVoci <= PrendiNumeroColonne(); lIndiceVoci++) 
	{
		 listaColonneIngombri.push_back(lIndiceVoci);
	}
	
	for (int16 i=0; i < listaColonneIngombri.size(); i++)
	{
		// per conversione interi
		std::ostringstream listValueStr;
		listValueStr << listaColonneIngombri.at(i);
		xrow row;
		xd_lst_enableupdates(COLONNEINGOMBRIPOPID, TRUE);
		xd_lst_addrow(COLONNEINGOMBRIPOPID, IR_BOTTOM ,&row);
		xd_lst_setrowtext(COLONNEINGOMBRIPOPID, row, (uchar*) listValueStr.str().c_str());	
		xd_lst_enableupdates(COLONNEINGOMBRIPOPID, FALSE);
	}

	// seleziono la voce di menu
	xd_lst_setselection(COLONNEINGOMBRIPOPID, 0);

	// aggiungo le voci al menu dei moduli della prima pagina
	std::vector<int32> listaModuliIngombri;
	for (lIndiceVoci = 0; lIndiceVoci <= gXtPreferences.documentoPref.moduliColonna; lIndiceVoci++) 
	{
		listaModuliIngombri.push_back(lIndiceVoci);
	}

	for (int16 i=0; i < listaModuliIngombri.size(); i++)
	{
		// per conversione interi
		std::ostringstream listValueStr;
		listValueStr << listaModuliIngombri.at(i);
		xrow row;
		xd_lst_enableupdates(MODULIINGOMBRIPOPID, TRUE);
		xd_lst_addrow(MODULIINGOMBRIPOPID, IR_BOTTOM ,&row);
		xd_lst_setrowtext(MODULIINGOMBRIPOPID, row, (uchar*) listValueStr.str().c_str());	
		xd_lst_enableupdates(MODULIINGOMBRIPOPID, FALSE);
	}

	// seleziono la voce di menu
	xd_lst_setselection(MODULIINGOMBRIPOPID, 0);
} // ImpostaPrimaPagina

/* ------------------------------------------------------------------------ *

	AggiornaPagine

* ------------------------------------------------------------------------ */
static void XTAPI AggiornaPagine() throw()
{
	// indice per scorrere le voci del menu
	int32 lIndiceVoci = 0;
	// conta il numero delle voci di menu del popup
	int32 lNumeroVoci = 0;
	// per prendere quante pagine ci sono attualmente
	int32 lNumeroPagine = 0;
	
	// prendo il numero delle pagine attuali piu uno
	lNumeroPagine = numpages();

	// conto le voci di menu del popup
	xd_lst_getrowcount(PAGINEPREPDOCPOPID, &lNumeroVoci);

	if (lNumeroVoci > lNumeroPagine) 
	{
		// cancello le pagine in piu'
		for (lIndiceVoci = lNumeroVoci; lNumeroVoci > lNumeroPagine; lNumeroVoci--) 
		{
			xd_lst_deleterow(PAGINEPREPDOCPOPID, lIndiceVoci);
		}
	} 
	else if (lNumeroVoci < lNumeroPagine) 
	{
		// aggiungo le pagine in meno
		for (lIndiceVoci = lNumeroVoci + 1; lIndiceVoci <= lNumeroPagine; lIndiceVoci++) 
		{
			// per conversione interi
			std::ostringstream listValueStr;
			listValueStr << lIndiceVoci;
			xrow row;
			xd_lst_enableupdates(PAGINEPREPDOCPOPID, TRUE);
			xd_lst_addrow(PAGINEPREPDOCPOPID, IR_BOTTOM ,&row);
			xd_lst_setrowtext(PAGINEPREPDOCPOPID, row, (uchar*) listValueStr.str().c_str());
			xd_lst_enableupdates(PAGINEPREPDOCPOPID, FALSE);
		}
	}
	
	// seleziono la voce di menu
	xd_lst_setselection(PAGINEPREPDOCPOPID, 0);
} // AggiornaPagine

/* ------------------------------------------------------------------------ *

	AggiornaPagineMastro

* ------------------------------------------------------------------------ */
static void XTAPI AggiornaPagineMastro() throw()
{
	// indice per scorrere le voci del menu
	int32 lIndiceVoci = 0;
	// conta il numero delle voci di menu del popup
	int32 lNumeroVoci = 0;
	
	// conto le voci di menu del popup
	xd_lst_getrowcount(PAGINEMASTROPOPID, &lNumeroVoci);

	if (lNumeroVoci > numpages()) 
	{
		// cancello le pagine in piu
		for (lIndiceVoci = lNumeroVoci; lNumeroVoci > numpages(); lNumeroVoci--) 
		{
			xd_lst_deleterow(PAGINEMASTROPOPID, lIndiceVoci);
		}
	} 
	else if (lNumeroVoci < numpages()) 
	{
		// aggiungo le pagine in meno
		for (lIndiceVoci = lNumeroVoci + 1; lIndiceVoci <= numpages(); lIndiceVoci++) 
		{
			xrow row;
			std::ostringstream listValueStr;
			listValueStr << lIndiceVoci;
			xd_lst_enableupdates(PAGINEMASTROPOPID, TRUE);
			xd_lst_addrow(PAGINEMASTROPOPID, IR_BOTTOM ,&row);
			xd_lst_setrowtext(PAGINEMASTROPOPID, row, (uchar*)listValueStr.str().c_str());
			xd_lst_enableupdates(PAGINEMASTROPOPID, FALSE);
		}
	}
	// seleziono la voce di menu
	xd_lst_setselection(PAGINEMASTROPOPID, 0);
} // AggiornaPagineMastro

/* ------------------------------------------------------------------------ *

	CercaBoxIngombro

* ------------------------------------------------------------------------ */
static void XTAPI CercaBoxIngombro() throw()
{
	// indice per scorrere le colonne
	int16 lColonna = 0;
	// per prendere le info sugli ingombri
	infoingombri *lPtrInfoIngombri = NULL;
	// per prendere l'altezza del box di ingombro
	Fixed lAltezza = 0;
	
	for (lColonna = 0; lColonna < PrendiNumeroColonne(); lColonna++) 
	{
		// imposto il puntatore
		lPtrInfoIngombri = gPtrInfoIngombri + lColonna;
		
		// cerco il box di ingombro 
		gErroreXtension = TrovaSlugDiIngombro(lColonna + 1);
		if (gErroreXtension == kNessunErrore)
		{
			(*lPtrInfoIngombri).colonna = lColonna + 1;

			// prendo l'altezza del box corrente
			lAltezza = PrendiAltezzaBox();
			
			// controllo l'altezza del box
			if (lAltezza == PrendiAltezzaColonna())
			{
				// e' occupata tutta la colonna
				(*lPtrInfoIngombri).moduli = gXtPreferences.documentoPref.moduliColonna;
			}
			else
			{
				// calcolo i moduli occupati
				assert (0 != (gXtPreferences.documentoPref.altezzaModulo + gXtPreferences.documentoPref.distanzaModuli));

				(*lPtrInfoIngombri).moduli = lAltezza /	  (gXtPreferences.documentoPref.altezzaModulo + gXtPreferences.documentoPref.distanzaModuli);
			}
		}
		else if (gErroreXtension == kErroreRicercaIngombro)
		{
			(*lPtrInfoIngombri).colonna = 0;
			(*lPtrInfoIngombri).moduli = 0;
		}
		else
		{
			DaiErrore(gErroreXtension);
			return;
		}
	}
	xtset_curbox(NULL);
} // CercaBoxIngombro

/* ------------------------------------------------------------------------ *

	PreparaDocWap
	
* ------------------------------------------------------------------------ */
int32 XTAPI PreparaDocWap(xdwapparamptr params) throw()
{
	// variabili d'utilita'
	static int16 inhere = 0;
	uchar tmpStr[kDimensioneStringhe] = "";
	
	static xrect windowLocation = {-1,-1,-1,-1};

	xdlgsetupptr dlgSetUp;
	DialogPtr dlgPtr;
	xtget_frontmost(&dlgPtr);
	
	params->result = XDR_HANDLED;
	
	switch(params->opcode) 
	{			
		case XDM_DIALOGSETUP:
		{
			dlgSetUp = (xdlgsetupptr) params->param1;
			dlgSetUp->dlgresid = PREPARADOCDIALOGID;
			
			break;
		}
		case XDM_DECLARATIONS:
		{
			// POP-UP
			
			// prepara documento - pagine	
		 	xd_lstpop_declare(PAGINEPREPDOCPOPID, INVALDLGITEMID, XLSTF_SMALLFONTSIZE, NULL);									
			// prepara documento - colonne					
			xd_lstpop_declare(COLONNEPREPDOCPOPID, INVALDLGITEMID, XLSTF_SMALLFONTSIZE, NULL);					
			// prepara documento - moduli					
			xd_lstpop_declare(MODULIPREPDOCPOPID, INVALDLGITEMID, XLSTF_SMALLFONTSIZE, NULL);					
			// mastro - pagine					
			xd_lstpop_declare(PAGINEMASTROPOPID, INVALDLGITEMID, XLSTF_SMALLFONTSIZE, NULL);					
			// mastro																		
			xd_lstpop_declare(MASTROPOPID, INVALDLGITEMID, XLSTF_SMALLFONTSIZE, NULL);					
			// colonne ingombro										
			xd_lstpop_declare(COLONNEINGOMBRIPOPID, INVALDLGITEMID, XLSTF_SMALLFONTSIZE, NULL);					
			// moduli ingombro					
			xd_lstpop_declare(MODULIINGOMBRIPOPID, INVALDLGITEMID, XLSTF_SMALLFONTSIZE, NULL);					
								
			// CHECK - BOX
			// abilita cancella mastro
			xd_btnchk_declare(CANCELLAMASTROCHECKID, 0);
			// abilita compensazione
			xd_btnchk_declare(COMPENSAZIONECHECKID, 0);
			
			// PUSH BTN
			// prepara
			xd_btnpsh_declare(PREPARABTNID, FALSE, CHSIM_NONE);
			// associa
			xd_btnpsh_declare(ASSOCIABTNID, FALSE, CHSIM_NONE);
			// applica
			xd_btnpsh_declare(APPLICABTNID, FALSE, CHSIM_NONE);
			
			params->result = XDR_HANDLED;
								
			break;
		}
		case XDM_LOADCONTROLS:
		{			
			// imposto campi sezione prepara documento
			ImpostaPreparaDocumento();
			
			// imposta campi pagine mastro
			ImpostaPagineMastro();
			
			// imposta campi prima pagina
			ImpostaPrimaPagina();

			params->result = XDR_HANDLED;			
			break;
		}
		case XDM_GAINFOCUS:
		{
			switch (params->itemid)
			{
				case PAGINEPREPDOCPOPID:
				{
					AggiornaPagine();
					break;
				}
				case PAGINEMASTROPOPID:
				{
					AggiornaPagineMastro();
					break;
				}
				
			}
			break;
		}
		
		case XDM_USERACTION:
		{
			switch (params->itemid)
			{
				case CANCELLAMASTROCHECKID:
				{
					xd_btnchk_getstate(CANCELLAMASTROCHECKID, &gCheckCancella);
					break;
				}
				
				case COMPENSAZIONECHECKID:
				{
					xd_btnchk_getstate(COMPENSAZIONECHECKID, &gCheckCompensa);
					break;
				}
				
				case PREPARABTNID:
				{
					// prendo valori correnti dei pop-up
					xrow row;
					uchar textRow[kDimensioneStringhe] = "";
					
					// pagine
					xd_lst_getselection(PAGINEPREPDOCPOPID, &row);
					xd_lst_getrowtext(PAGINEPREPDOCPOPID, row, 256, textRow);
					int32 pagine;
					StringToNum (textRow, &pagine);
					
					// colonne
					xd_lst_getselection(COLONNEPREPDOCPOPID, &row);
					xd_lst_getrowtext(COLONNEPREPDOCPOPID, row, 256, textRow);
					int32 colonne;
					StringToNum (textRow, &colonne);
					
					// moduli
					xd_lst_getselection(MODULIPREPDOCPOPID, &row);
					xd_lst_getrowtext(MODULIPREPDOCPOPID, row, 256, textRow);
					int32 moduli;
					StringToNum (textRow, &moduli);
					
					InserisciPagineColonneEModuli(pagine, colonne, moduli, gCheckCancella);
					
					break;
				}
				
				case ASSOCIABTNID:
				{
					// prendo valori correnti dei pop-up
					xrow row;
					uchar textRow[kDimensioneStringhe] = "";
					
					// pagina
					xd_lst_getselection(PAGINEMASTROPOPID, &row);
					xd_lst_getrowtext(PAGINEMASTROPOPID, row, 256, textRow);
					int32 paginaMastro;
					StringToNum (textRow, &paginaMastro);
					
					// nome mastro
					xd_lst_getselection(MASTROPOPID, &row);
					int16 masterPageId = row + 1;
					
					AssociaPaginaMastro((int32)paginaMastro, masterPageId, gCheckCompensa);
					break;
				}
				
				case APPLICABTNID:
				{
					// prendo valori correnti dei pop-up
					xrow row;
					uchar textRow[kDimensioneStringhe] = "";
					
					// colonne
					xd_lst_getselection(COLONNEINGOMBRIPOPID, &row);
					xd_lst_getrowtext(COLONNEINGOMBRIPOPID, row, 256, textRow);
					int32 colonne;
					StringToNum (textRow, &colonne);
					
					// moduli
					xd_lst_getselection(MODULIINGOMBRIPOPID, &row);
					xd_lst_getrowtext(MODULIINGOMBRIPOPID, row, 256, textRow);
					int32 moduli;
					StringToNum (textRow, &moduli);
					
					ModificaPrimaPagina(colonne, moduli);
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
			break;
		}
		
		case XDM_DEINIT:
		{
			// get the position of this dialog
			xd_getwindowscreenrect(&windowLocation);
						
			break;
		}
		
		default:
		{
			params->result = XDR_NOTHANDLED;
			break;
		}		
	}
	
	
	return(noErr);
} // PreparaDocWap

/* ------------------------------------------------------------------------ *

	PreparaDocumento

* ------------------------------------------------------------------------ */
void XTAPI PreparaDocumento() throw()
{
	// creo la struttura per memorizzare il numero di moduli per ogni colonna 
	if (gPtrInfoIngombri != NULL) 
	{
		DisposePtr((Ptr)gPtrInfoIngombri);
		gPtrInfoIngombri = NULL;
	}

	gPtrInfoIngombri = (infoingombri *) NewPtrClear(sizeof(infoingombri) *
												    PrendiNumeroColonne());
	if (gPtrInfoIngombri == NULL) 
	{
		MostraErrore(kErroreMemoria);
		return;
	}

	// controllo se preparare in coda o in testa
	if (gXtPreferences.finalePref.preparaDocumento == PRIMAPAGINARADIOBTNID)
	{
		// vado sulla prima pagina
		gotopage(kPrimaPagina);
	}
	else
	{
		// vado sull'ultima pagina
		gotopage(numpages());
	}

	CercaBoxIngombro();

//	dodlgboxparam( hinst, hwndmainframe, (FARPROC)PreparaDocumentoWAP, DIALOG_20150, NULL );
	xd_createdialog(_XT_PREPARADOCWAP, 0, NULL);
}

#pragma mark -
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

/* Costanti */
/* Costanti */
/* Costanti */

//#define rPopUpPagine 21000
//#define rPopUpColonne 21010
//#define rPopUpModuli 21020
/* risorsa di men— per creare i popup del prepara documento */

//#define rPopUpPagineMastro 21030
//#define rPopUpNomiMastro 21040
/* risorsa di men— per creare i popup delle pagine mastro */

//#define rPopUpColonnaPrimaPagina 21050
//#define rPopUpModuliPrimaPagina 21060
/* risorsa di men— per creare i popup delle pagine mastro */


/* item della finestra */

/*
#define iPopUpPagine 4
#define iPopUpColonne 6
#define iPopUpModuli 8
#define iCheckCancella 9
#define iBottonePrepara 10

#define iPopUpPagineMastro 14
#define iPopUpNomiMastro 16
#define iCheckCompensa 17
#define iBottoneAssocia 18

#define iPopUpColonnaPrimaPagina 22
#define iPopUpModuliPrimaPagina 24
#define iBottoneApplica 25

#define kMastroSingolaVuota 1
*/
#define rFinestraPreparaDocumento 20150
/* identificatore della finestra per preparare il documento */

#define iPopUpPagine IDC_20150_COMBOBOX4
#define iPopUpColonne IDC_20150_COMBOBOX6
#define iPopUpModuli IDC_20150_COMBOBOX8
#define iCheckCancella IDC_CHECKBOX9
#define iBottonePrepara IDC_20150_PUSHBUTTON10

#define iPopUpPagineMastro IDC_20150_COMBOBOX14
#define iPopUpNomiMastro IDC_20150_COMBOBOX16
#define iCheckCompensa IDC_CHECKBOX17
#define iBottoneAssocia IDC_20150_PUSHBUTTON18

#define iPopUpColonnaPrimaPagina IDC_20150_COMBOBOX22
#define iPopUpModuliPrimaPagina IDC_20150_COMBOBOX24
#define iBottoneApplica IDC_20150_PUSHBUTTON25



/* per applicare la pagina mastro vuota */


/* Variabili */
/* Variabili */
/* Variabili */

static Point gPosizioneFinestraPreparaDocumento = {32767,32767};
/* posizione della finestra */

//static int gIdPopUpPagine = 0;
//static int gIdPopUpColonne = 0;
//static int gIdPopUpModuli = 0;
/* da usare con l'allochierid() per la creazione del popup */

//static int gIdPopUpPagineMastro = 0;
//static int gIdPopUpNomiMastro = 0;
/* da usare con l'allochierid() per la creazione del popup */

//static int gIdPopUpColonnaPrimaPagina = 0;
//static int gIdPopUpModuliPrimaPagina = 0;
/* da usare con l'allochierid() per la creazione del popup */


//static popupinforec *gPtrPopUpPagine = NULL;
/* sempre per il popup delle pagine */
static popupinforec *gPtrPopUpColonne = NULL;
/* sempre per il popup delle colonne */
//static popupinforec *gPtrPopUpModuli = NULL;
/* sempre per il popup dei moduli */


//static popupinforec *gPtrPopUpPagineMastro = NULL;
/* sempre per il popup per associare un mastro alle pagine */
//static popupinforec *gPtrPopUpNomiMastro = NULL;
/* sempre per il popup per associare un mastro alle pagine */

static popupinforec *gPtrPopUpColonnaPrimaPagina = NULL;
/* sempre per il popup delle colonne della prima pagina */
//static popupinforec *gPtrPopUpModuliPrimaPagina = NULL;
/* sempre per il popup dei moduli della prima pagina */


BOOL CALLBACK PreparaDocumentoWAP( HWND, WORD, PARAM1, PARAM2);
BOOL CALLBACK PreparaDocumentoWAP( HWND thewptr, WORD message, PARAM1 param1, PARAM2 param2)
/* ------------------------------------------------------------------------ *

	PreparaDocumentoWAP

	6 Marzo 1996						Stefano

	Descrizione:
	WAP di gestione della finestra per prepara documento

	Parametri:
	i paramentri di entrata arrivano da Xpress

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	int lRitorno = FALSE;
	/* valore di ritorno della WAP */
	short lBottoneScelto = 0;
	/* per prendere l'identificatore del botto scelto sulla finestra di preferenze */
	BOOL result = TRUE;
 	static int16 inhere = 0;
	int				i;

	switch (message) {
		case WOPEN:
			frontmost = thewptr;
			/* preparo i popup della prepara documento */
			ImpostaPreparaDocumento();

			/* preparo i popup delle pagine mastro */
			ImpostaPagineMastro(frontmost);

			/* preparo i popup della prima pagina */
			ImpostaPrimaPagina(frontmost);
			result = FALSE;
			center_window(thewptr);
			break;

		case WM_COMMAND:
			switch (param1) {

				case iPopUpPagine:
				if ((HIWORD(param2) == CBN_SETFOCUS) && (!inhere))
				{
					AggiornaPagine(param2);
				}
				if ((HIWORD(param2) == CBN_SELCHANGE) && (!inhere)) {
					inhere=1;
					i = SendMessage(LOWORD(param2),CB_GETCURSEL,0,0L);
					if (i == CB_ERR) i = 0;
					SendMessage(LOWORD(param2), CB_GETLBTEXT,i,(LONG)(LPSTR)tmpStr);
					SetWindowText(LOWORD(param2),tmpStr);
					inhere=0;

/*					dopopup((DialogPeek) thewptr,
							iPopUpPagine - 1,
							iPopUpPagine);
*/
					}
					break;

				case iPopUpColonne:
				case PAGINEPREPDOCPOPID:
				{
				/*
					if ((HIWORD(params->param2) == CBN_SELCHANGE) && (!inhere)) 
					{
						inhere=1;
						int32 i;
						i = xd_lst_getselection(PAGINEPREPDOCPOPID, &i);
						if (i == CB_ERR) 
							i = 0;
						xd_lst_getrowstring(PAGINEPREPDOCPOPID, i, tmpStr);
						// SetWindowText(LOWORD(params->param2),tmpStr);
						xd_lst_setrowtext(PAGINEPREPDOCPOPID, i, tmpStr);
						inhere=0;
					}
				*/	
					break;
				}
				if ((HIWORD(param2) == CBN_SELCHANGE) && (!inhere)) {
						inhere=1;
						i = SendMessage(LOWORD(param2),CB_GETCURSEL,0,0L);
						if (i == CB_ERR) i = 0;
						SendMessage(LOWORD(param2), CB_GETLBTEXT,i,(LONG)(LPSTR)tmpStr);
						SetWindowText(LOWORD(param2),tmpStr);
						inhere=0;
					}
					break;

				case iPopUpModuli:
				if ((HIWORD(param2) == CBN_SELCHANGE) && (!inhere)) {
						inhere=1;
						i = SendMessage(LOWORD(param2),CB_GETCURSEL,0,0L);
						if (i == CB_ERR) i = 0;
						SendMessage(LOWORD(param2), CB_GETLBTEXT,i,(LONG)(LPSTR)tmpStr);
						SetWindowText(LOWORD(param2),tmpStr);
						inhere=0;
					}
					break;

				case iCheckCancella:
					gCheckCancella = setcheck((DialogPtr) thewptr,
												iCheckCancella,
												!gCheckCancella);
					break;

				case iBottonePrepara:
					// costruisco il documento
					InserisciPagineColonneEModuli(SendDlgItemMessage((DialogPeek)thewptr,iPopUpPagine,CB_GETCURSEL,0,0L)/*+1*/,
												  SendDlgItemMessage((DialogPeek)thewptr,iPopUpColonne,CB_GETCURSEL,0,0L),
												  SendDlgItemMessage((DialogPeek)thewptr,iPopUpModuli,CB_GETCURSEL,0,0L),
												  gCheckCancella);

					break;

				case iPopUpPagineMastro:
					if ((HIWORD(param2) == CBN_SETFOCUS) && (!inhere))
					{
						AggiornaPagineMastro(param2);
					}
					if ((HIWORD(param2) == CBN_SELCHANGE) && (!inhere)) {
						inhere=1;
						i = SendMessage(LOWORD(param2),CB_GETCURSEL,0,0L);
						if (i == CB_ERR) i = 0;
						else
						{
							SendMessage(LOWORD(param2), CB_GETLBTEXT,i,(LONG)(LPSTR)tmpStr);
							SetWindowText(LOWORD(param2),tmpStr);
/*					dopopup((DialogPeek) thewptr,
							 iPopUpPagineMastro - 1,
							 iPopUpPagineMastro);
*/
							if (i == 0)
								setditemenable((DialogPeek) thewptr, iBottoneAssocia, FALSE);
							else setditemenable((DialogPeek) thewptr, iBottoneAssocia, TRUE);
							inhere=0;
						}
					}
					break;

				case iPopUpNomiMastro:
				if ((HIWORD(param2) == CBN_SELCHANGE) && (!inhere)) {
						inhere=1;
						i = SendMessage(LOWORD(param2),CB_GETCURSEL,0,0L);
						if (i == CB_ERR) i = 0;
						SendMessage(LOWORD(param2), CB_GETLBTEXT,i,(LONG)(LPSTR)tmpStr);
						SetWindowText(LOWORD(param2),tmpStr);
						inhere=0;
					}
					break;

				case iCheckCompensa:
					gCheckCompensa = setcheck((DialogPtr) thewptr,
												iCheckCompensa,
												!gCheckCompensa);
					break;

				case iBottoneAssocia:
					/* associo la pagina mastro selezionata */
					AssociaPaginaMastro(SendDlgItemMessage((DialogPeek)thewptr,iPopUpPagineMastro,CB_GETCURSEL,0,0L) + 1,
										SendDlgItemMessage((DialogPeek)thewptr,iPopUpNomiMastro,CB_GETCURSEL,0,0L) + 1,
										gCheckCompensa);

					break;

				case iPopUpColonnaPrimaPagina:
				if ((HIWORD(param2) == CBN_SELCHANGE) && (!inhere)) {
						inhere=1;
						i = SendMessage(LOWORD(param2),CB_GETCURSEL,0,0L);
						if (i == CB_ERR) i = 0;
						SendMessage(LOWORD(param2), CB_GETLBTEXT,i,(LONG)(LPSTR)tmpStr);
						SetWindowText(LOWORD(param2),tmpStr);
						inhere=0;
					}
					break;

				case iPopUpModuliPrimaPagina:
				if ((HIWORD(param2) == CBN_SELCHANGE) && (!inhere)) {
						inhere=1;
						i = SendMessage(LOWORD(param2),CB_GETCURSEL,0,0L);
						if (i == CB_ERR) i = 0;
						SendMessage(LOWORD(param2), CB_GETLBTEXT,i,(LONG)(LPSTR)tmpStr);
						SetWindowText(LOWORD(param2),tmpStr);
						inhere=0;
					}
					break;
					
				case iBottoneApplica:
					ModificaPrimaPagina(SendDlgItemMessage((DialogPeek)thewptr,iPopUpColonnaPrimaPagina,CB_GETCURSEL,0,0L)+1,
										SendDlgItemMessage((DialogPeek)thewptr,iPopUpModuliPrimaPagina,CB_GETCURSEL,0,0L));
					break;

				case IDOK:

					/* prendo la posizione della finestra */
//					gPosizioneFinestraPreparaDocumento = topLeft(((WindowPtr) thewptr)->portRect);
//					LocalToGlobal(&gPosizioneFinestraPreparaDocumento);

					/* chiudo i popup */
					DestroyMenu(gHndlMenuPopUpPagine);
					DestroyMenu(gHndlMenuPopUpColonne);
					DestroyMenu(gHndlMenuPopUpModuli);

					DestroyMenu(gHndlMenuPopUpPagineMastro);
					DestroyMenu(gHndlMenuPopUpNomiMastro);

					DestroyMenu(gHndlMenuPopUpColonnaPrimaPagina);
					DestroyMenu(gHndlMenuPopUpModuliPrimaPagina);

//					restorewnds((WindowPeek) thewptr,TRUE);

					EndDialog(thewptr, NULL);
					break;

		case IDCANCEL:
				EndDialog(thewptr, NULL);
				break;
			}
			break;

		default:
			result = FALSE;
	}

	return(result);
}

#if kOLD

void AlloccaPopUpPreparaDocumento(void)
/* ------------------------------------------------------------------------ *

	AlloccaPopUpPreparaDocumento

	6 Marzo 1996						Stefano

	Descrizione:
	Per alloccare gli identificatori dei popup menu della prepara documento

	Parametri:
	nessuno

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	/* prepara documento */
	gIdPopUpPagine = allochierid();
	gIdPopUpColonne = allochierid();
	gIdPopUpModuli = allochierid();
	
	/* pagine mastro */
	gIdPopUpPagineMastro = allochierid();
	gIdPopUpNomiMastro = allochierid();

	/* prima pagina */
	gIdPopUpColonnaPrimaPagina = allochierid();
	gIdPopUpModuliPrimaPagina = allochierid();
}

#endif /* kOLD */

#endif // 0 NECRO v1

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/