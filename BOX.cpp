/* ------------------------------------------------------------------------ *

	Box.cpp

	15 Aprile 2005						Fabrizio

	Descrizione:
	Contiene le procedure usate per eseguire la creazione dei box di testo

	$Log: not supported by cvs2svn $
	Revision 1.5  2005/05/11 16:01:15  taretto
	versione 2.0.0.1
	
	Revision 1.4  2005/05/04 12:34:50  taretto
	comandi posiziona e ripristino ok
	
	Revision 1.3  2005/04/27 16:17:45  taretto
	gestione prepara documento (wap + preparazione)
	
	Revision 1.2  2005/04/21 09:48:19  taretto
	tolte mLong per gestire numero moduli e colonne, modificata estensione per salvataggi in QXP
	
	Revision 1.1  2005/04/20 07:14:18  taretto
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

#include "DbpInclude.h"
#include "DbpOkDialog.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

// #include "SimbPax.h"

#include "Errori.h"
#include "DocInfo.h"
#include "Coord.h"
#include "ReadFile.h"
#include "Pref.h"
#include "AppWare.h"
#include "Immagini.h"
#include "GesCom.h"
#include "Justify.h"
#include "Stili.h"
#include "ResStr.h"
#include "Testo.h"
#include "GesSlug.h"
#include "InfoNec.h"
#include "SimbPax.h"
#include "SpaceCol.h"

#include "FinaleWap.h"

#include "Box.h"

// CONSTS
// CONSTS
// CONSTS

// quando c'e' un coflitto nella selezione in cui si vuole prendere
// un attributo di testo ritorna questo valore
#define kConflitto 0x80000000

// per copiare alla fine del flusso di teso
#define kFineFlussoTesto -1

// STATICS GLOBALS
// STATICS GLOBALS
// STATICS GLOBALS

// nel riposizionare i box e necessario ricordare il box impaginato
// precedentetemente
static boxid gBoxPrecedente = 0;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			SpostaBoxCorrente
	@abstract 		gestione box
	@discussion		Sposta il box corrente sulle nuove coordinate. E' una procedura usata dallle
					procedure che verificano la giusta posizione del box appena impaginato. E' usata
					nella bozza
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
static void XTAPI SpostaBoxCorrente() throw();

/*!
	@function			CreaBoxTestoCollegato
	@abstract 		gestione box
	@discussion		Crea un box di testo su una nuova colonna collegato al precedente. E' una procedura
					usata solo nell'impaginazione di bozza per poter vedere tutto il necrologio
					nell'eventualita' che sia piu' lungo della colonna del documento di bozza scelto
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
static void XTAPI CreaBoxTestoCollegato() throw();

/*!
	@function			ModificaAltezzaBoxAGiustezza
	@abstract 		gestione box
	@discussion		Modifica l'altezza del box corrente in modo che il testo sia giustezza.
					Se l'operazione riesce ritorna TRUE e la nuova altezza nel parametro.
					Altrimeni ritorna FALSE.
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nuovaaltezza ritorna la nuova altezza del box
	@result			ritorna TRUE se l'operazione e' andata a buon fine. FALSE se Š necessario
					spostare il box sulla colonna successiva
*/
static bool8 XTAPI ModificaAltezzaBoxAGiustezza(Fixed *nuovaaltezza) throw();

/*!
	@function			PrendiPosizioneTestoDaTagliare
	@abstract 		gestione box
	@discussion		Procedura che prende la posizione e la lunghezza del testo da tagliare
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			ptrinizio per ritornare la posizione di inizio del testo da tagliare
	@param			ptrfine per restituire la posizione di fine del testo da tagliare
	@result			nessuno
*/
static void XTAPI PrendiPosizioneTestoDaTagliare(int32 *ptrinizio, int32 *ptrfine) throw();

/*!
	@function			TagliaTesto
	@abstract 		gestione box
	@discussion		In base alla posizione passata taglia il testo da box corrente
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			inizio punto di inizio del taglio
	@param			fine punto di fine del taglio
	@result			nessuno
*/
static void XTAPI TagliaTesto(int32 inizio, int32 fine) throw();

/*!
	@function			GestioneBoxLunghiIngombro
	@abstract 		gestione box
	@discussion		Per la gestione dei box piu' lunghi di una colonna
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
static void XTAPI GestioneBoxLunghiIngombro() throw();

/*!
	@function			GestioneBoxLunghiDefinitivo
	@abstract 		gestione box
	@discussion		Per la gestione dei box piu' lunghi di una colonna
					
					15 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware per salvare le informazioni del box corrente
	@result			nessuno
*/
static void XTAPI GestioneBoxLunghiDefinitivo(appware *ptrappware) throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	SpostaBoxCorrente

* ------------------------------------------------------------------------ */
static void XTAPI SpostaBoxCorrente() throw()
{
	// puntatore al box di testo
	xtboxptr lPtrBoxTesto = NULL;
	// per posizionare il box nella pagina corrente
	pagenumdata lDatiPagina;
	// per leggere l'altezza del box di testo corrente
	Fixed lAltezzaBox = 0;
	// per prendere la coordinata alta del box da posizionare
	Fixed lCoordinataAlta = 0;
	
	// prendo il box di testo corrente
	lPtrBoxTesto = getxtbox(NULL, NULL, FALSE);
	sprdfrect2page(&lPtrBoxTesto->t.box.s.r.fr);
	
	lAltezzaBox = lPtrBoxTesto->t.box.s.r.fr.bottom - lPtrBoxTesto->t.box.s.r.fr.top;
	lCoordinataAlta = PrendiCoordinataAlta();
	if (lCoordinataAlta != PrendiMargineAlto()) 
		lCoordinataAlta += gXtPreferences.giustificazionePref.valoreIniziale;
	lPtrBoxTesto->t.box.s.r.fr.top = lCoordinataAlta;
	lPtrBoxTesto->t.box.s.r.fr.bottom = lPtrBoxTesto->t.box.s.r.fr.top + lAltezzaBox;
	lPtrBoxTesto->t.box.s.r.fr.left = PrendiCoordinataSinistra();
	lPtrBoxTesto->t.box.s.r.fr.right = lPtrBoxTesto->t.box.s.r.fr.left +
									   PrendiLarghezzaColonna();
	
	xtseq2page(PrendiPaginaCorrente(), FALSE, &lDatiPagina);
	pagefrect2sprd(&lDatiPagina, &lPtrBoxTesto->t.box.s.r.fr);

	setxtbox(lPtrBoxTesto, NULL, FALSE, TRUE);
	disposextbox( lPtrBoxTesto, TRUE);
} // SpostaBoxCorrente

/* ------------------------------------------------------------------------ *

	CreaBoxTestoCollegato

* ------------------------------------------------------------------------ */
static void XTAPI CreaBoxTestoCollegato() throw()
{
	// puntatore al box di testo 
	xtboxptr lPtrBoxTesto = NULL;
	// per posizionare il box nella pagina corrente
	pagenumdata lDatiPagina;
	// coordinate del box
	fixedrect lCoordinateBox = {0, 0, 0, 0};
	// identificatore del box di testo creato
	boxid lIdBoxTesto = 0;
	// per prendere la coordinata alta del box da posizionare
	Fixed lCoordinataAlta = 0;
	// identificatore del box di testo da cui fare il collegamento
	boxid lIdBoxDiTestoPrecedente = 0;
	
	// prendo box corrente
	boxid curbox = NULL;
	xtget_curbox(&curbox);
	
	// imposto il vecchio box di testo
	lIdBoxDiTestoPrecedente = curbox;
	
	// calcolo le coordinate del box
	lCoordinataAlta = PrendiCoordinataAlta();
	if (lCoordinataAlta != PrendiMargineAlto()) 
		lCoordinataAlta += gXtPreferences.giustificazionePref.valoreIniziale;
	lCoordinateBox.top = lCoordinataAlta;
	lCoordinateBox.left = PrendiCoordinataSinistra();
	lCoordinateBox.bottom = lCoordinateBox.top + kFixedAltezzaInterna;
	lCoordinateBox.right = lCoordinateBox.left + PrendiLarghezzaColonna();

	// creo il box di testo
	DocID curdoc;
	xtget_curdoc(&curdoc);
	DocRef DRef = XTGetDocRefFromDocID(curdoc);
	XTNewXTBox(DRef, CT_TEXT, SH_RECT, &lPtrBoxTesto);
	sprdfrect2page(&lPtrBoxTesto->t.box.s.r.fr);
	
	lPtrBoxTesto->t.box.s.r.fr = lCoordinateBox;
	lPtrBoxTesto->t.textinset[TOP_INSET] = 0;
	lPtrBoxTesto->t.textinset[LEFT_INSET] = 0;
	lPtrBoxTesto->t.textinset[BOTTOM_INSET] = 0;
	lPtrBoxTesto->t.textinset[RIGHT_INSET] = 0;
	
	fixedrect outsetnull = {0, 0, 0, 0};
	lPtrBoxTesto->t.box.runaround.outset = outsetnull; // <- vecchio text outset

	xtget_curbox(&curbox);
	
	xtseq2page(PrendiPaginaCorrente(), FALSE, &lDatiPagina);
	pagefrect2sprd(&lDatiPagina, &lPtrBoxTesto->t.box.s.r.fr);	
	XTInstallBox(DRef, lPtrBoxTesto, NULL , curbox, TRUE, &lIdBoxTesto);
	
	disposextbox(lPtrBoxTesto,TRUE);
	
	// creo il collegamento del testo
	xelinkbox(lIdBoxDiTestoPrecedente,lIdBoxTesto,TRUE);
	xecalc(XEDOALL);
	
	xtset_curbox(lIdBoxTesto);
} // CreaBoxTestoCollegato

/* ------------------------------------------------------------------------ *

	ModificaAltezzaBoxAGiustezza

* ------------------------------------------------------------------------ */
static bool8 XTAPI ModificaAltezzaBoxAGiustezza(Fixed *nuovaaltezza) throw()
{
	// handle al LOS del box di testo corrente
	loshandle lHndlLos = NULL;
	// lunghezza del LOS corrente
	int32 lLunghezzaLos = 0;
	// puntatore al box di testo corrente
	xtboxptr lPtrBoxTesto = NULL;
	// identificatore del box di testo corrente
	boxid lIdBoxCorrente = 0;
	// per spostare il box nella pagina corretta
	pagenumdata lDatiPagina;
	// posizione del primo carattere del flusso di testo del box
	int32 lPosizioneUltimoCarattere = 0;
	// coordinate del box alta a sinistra
	fixedpoint lCoordinateBox = {0, 0};
	// per sapere se c'e' dell'overflow del testo del box
	Fixed lSituazioneTesto = 0;
	// altezza dell'ultima riga per aggioornare il bottom del box 
	Fixed lAltezzaUltimaLinea = 0;
	// altezza del box di testo
	Fixed lAltezzaBox = 0;
	// per memorizzare il risultato dell'operazione
	Boolean lBool= FALSE;
		
	// se l'ultimo carattere e' un return lo tolgo 
	TogliRitornoACapo();

	// aumento la lunghezza del box di testo fino in fondo alla colonna
	lPtrBoxTesto = getxtbox(NULL, NULL, FALSE);
	sprdfrect2page(&lPtrBoxTesto->t.box.s.r.fr);
	// cambio le coordinate solo se il bottom del documento e' piu' basso del
	// bottom del box corrente
	if (PrendiMargineBasso() - kFixedMinimoSpazioTraNecrologi > lPtrBoxTesto->t.box.s.r.fr.bottom)
		lPtrBoxTesto->t.box.s.r.fr.bottom = PrendiMargineBasso() - kFixedMinimoSpazioTraNecrologi;
	xtseq2page(PrendiPaginaCorrente(), FALSE, &lDatiPagina);
	pagefrect2sprd(&lDatiPagina, &lPtrBoxTesto->t.box.s.r.fr);
	setxtbox(lPtrBoxTesto, NULL, FALSE, TRUE);
	disposextbox(lPtrBoxTesto, TRUE);
	xecalc(XEDOALL);

	// prendo boxid
	boxid curbox;
	xtget_curbox(&curbox);

	// prende il LOS del box di testo corrente
	lIdBoxCorrente = curbox;
	lPosizioneUltimoCarattere = 0;
	lHndlLos = (loshandle) NewHandle(0);
	xtgetlos(&lIdBoxCorrente, &lPosizioneUltimoCarattere, FALSE, &lCoordinateBox, &lHndlLos);
	lLunghezzaLos = GetHandleSize((Handle) lHndlLos) / sizeof(losrec);
	
	lSituazioneTesto = (**lHndlLos)[lLunghezzaLos - 1].leading;
	   
	if (lSituazioneTesto == ENDOTXT) 
	{
		// tutto il testo nel box e visibile
		lAltezzaUltimaLinea = (**lHndlLos)[lLunghezzaLos - 2].linev +
							  (**lHndlLos)[lLunghezzaLos - 2].descent;
		
		lPtrBoxTesto = getxtbox(NULL,NULL,FALSE);
		lAltezzaBox = lAltezzaUltimaLinea + 
					  lPtrBoxTesto->t.textinset[BOTTOM_INSET] +
					  lPtrBoxTesto->t.textinset[TOP_INSET] +
					  + (2 * lPtrBoxTesto->t.box.frame.width/*thickness*/);
					  
		lPtrBoxTesto->t.box.s.r.fr.bottom = lPtrBoxTesto->t.box.s.r.fr.top +
											lAltezzaBox;

										  
		setxtbox(lPtrBoxTesto, NULL, FALSE, TRUE);
		disposextbox(lPtrBoxTesto,TRUE);
		xecalc(FALSE);

		lBool = TRUE;
		(*nuovaaltezza) = lAltezzaBox;
	} 
	else 
	{
		// ho allungato il testo fino alla fine della colonna
		// ma c'e' del testo che ancora non si vede bene
		lBool = FALSE;
		(*nuovaaltezza) = 0;
	}
	
	DisposeHandle((Handle) lHndlLos);
	return(lBool);
} // ModificaAltezzaBoxAGiustezza

/* ------------------------------------------------------------------------ *

	PrendiPosizioneTestoDaTagliare
	
* ------------------------------------------------------------------------ */
static void XTAPI PrendiPosizioneTestoDaTagliare(int32 *ptrinizio, int32 *ptrfine) throw()
{
	// handle al LOS del box di testo corrente
	loshandle lHndlLos = NULL;
	// lunghezza del LOS corrente
	int32 lLunghezzaLos = 0;
	// indice per sc orrere il LOS array
	int32 lIndiceLOS = 0;
	// identificatore del box di testo corrente
	boxid lIdBoxCorrente = 0;
	// posizione del primo carattere del flusso di testo del box
	int32 lPosizioneUltimoCarattere = 0;
	// coordinate del box alta a sinistra
	fixedpoint lCoordinateBox = {0, 0};
	// handle al flusso di testo corrente
	xehandle lHndlXE = NULL;
	// inizio del testo attualmente selezionato
	int32 lInizioSelezione = 0;
	// fine del testo attualmente selezionato
	int32 lFineSelezione = 0;
	// lunghezza del flusso di testo corrente
	int32 lLunghezzaTesto = 0;
	// inizio del testo da tagliare
	int32 lInizioTestoDaTagliare = 0;
	// fine del testo da tagliare
	int32 lFineTestoDaTagliare = 0;
	// carattere da controllare
	uchar lCarattere = 0;
	
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	// prende il LOS del box di testo corrente
	lIdBoxCorrente = curbox;
	lPosizioneUltimoCarattere = 0;
	lHndlLos = (loshandle) NewHandle(0);
	xtgetlos(&lIdBoxCorrente, &lPosizioneUltimoCarattere, FALSE, &lCoordinateBox, &lHndlLos);
	lLunghezzaLos = GetHandleSize((Handle) lHndlLos) / sizeof(losrec);
	
	// prendo la selezione corrente
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	
	// imposto l'indice per scorrere il LOS array
	lIndiceLOS = 2;
	
	// volgio essere sicuro la la riga precedente dove tagliare finisca con un 
	// fine paragrafo
	while (1) 
	{
		// prendo la posizione dell'ultimo carattere visibile
		lInizioTestoDaTagliare = (**lHndlLos)[lLunghezzaLos - lIndiceLOS].chpos;
		
		// controllo che l'ultimo carattere sia un return
		lCarattere = h_getchar(lHndlXE, lInizioTestoDaTagliare - 1);
		if (ISPARACHAR(lCarattere)) 
			break;
		if (lIndiceLOS == lLunghezzaLos) 
			break;
		// incremento l'indice
		lIndiceLOS++;
	}

	// calcolo la lunghezza del testo da tagliare
	lFineTestoDaTagliare = lLunghezzaTesto;

	// libero la memoria del LOS Handle
	DisposeHandle((Handle) lHndlLos);
	
	// ritorno dei valori
	(*ptrinizio) = lInizioTestoDaTagliare;
	(*ptrfine) = lFineTestoDaTagliare;
	
	// ripristino la selezione iniziale
	xesetsel(0, 0, TRUE);
	xecalc(XEDOALL);
	xesetsel(lInizioSelezione, lFineSelezione, FALSE);
} // PrendiPosizioneTestoDaTagliare

/* ------------------------------------------------------------------------ *

	TagliaTesto

* ------------------------------------------------------------------------ */
static void XTAPI TagliaTesto(int32 inizio, int32 fine) throw()
{
	// handle al flusso di testo corrente
	xehandle lHndlXE = NULL;
	// inizio del testo attualmente selezionato
	int32 lInizioSelezione = 0;
	// fine del testo attualmente selezionato
	int32 lFineSelezione = 0;
	// lunghezza del flusso di testo corrente
	int32 lLunghezzaTesto = 0;
	
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	// prendo la selezione corrente
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	// cancello il testo fuori dal box
	xedelchars(curbox, inizio, fine);
	xecalc(XEDOALL);

	// ripristino la selezione iniziale
	if (lInizioSelezione >= inizio) 
	{
		lInizioSelezione -= (fine - inizio);
		lFineSelezione -= (fine - inizio);
	}
	xesetsel(0, 0, TRUE);
	xecalc(XEDOALL);
	xesetsel(lInizioSelezione, lFineSelezione, FALSE);
} // TagliaTesto

/* ------------------------------------------------------------------------ *

	AumentaAltezzaBox

* ------------------------------------------------------------------------ */
void XTAPI AumentaAltezzaBox() throw()
{
	// handle al LOS del box di testo corrente
	loshandle lHndlLos = NULL;
	// lunghezza del LOS corrente
	int32 lLunghezzaLos = 0;
	// puntatore al box di testo corrente
	xtboxptr lPtrBoxTesto = NULL;
	// identificatore del box di testo corrente
	boxid lIdBoxCorrente = 0;
	// posizione del primo carattere del flusso di testo del box
	int32 lPosizioneUltimoCarattere = 0;
	// coordinate del box alta a sinistra
	fixedpoint lCoordinateBox = {0, 0};
	// per sapere se c'e' dell'overflow del testo del box
	Fixed lSituazioneTesto = 0;
	// altezza dell'ultima riga per aggioornare il bottom del box
	Fixed lAltezzaUltimaLinea = 0;
	// altezza del box di testo
	Fixed lAltezzaBox = 0;
	// per memorizzare il risultato dell'operazione
	Boolean lFinito = FALSE;
	
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	while (1)
	{
		// aumento la lunghezza del box di testo di un modulo
		lPtrBoxTesto = getxtbox(NULL, NULL, FALSE);
		lPtrBoxTesto->t.box.s.r.fr.bottom +=kFixedAltezzaInterna;
		setxtbox(lPtrBoxTesto, NULL, FALSE, TRUE);
		disposextbox(lPtrBoxTesto, TRUE);
		xecalc(FALSE);

		// prende il LOS del box di testo corrente
		lIdBoxCorrente = curbox;
		lPosizioneUltimoCarattere = 0;
		lHndlLos = (loshandle) NewHandle(0);
		xtgetlos(&lIdBoxCorrente, &lPosizioneUltimoCarattere, FALSE, &lCoordinateBox, &lHndlLos);
		lLunghezzaLos = GetHandleSize((Handle) lHndlLos) / sizeof(losrec);

		lSituazioneTesto = (**lHndlLos)[lLunghezzaLos - 1].leading;

		if (lSituazioneTesto == ENDOTXT)
		{
			// tutto il testo nel box e visibile
			lAltezzaUltimaLinea = (**lHndlLos)[lLunghezzaLos - 2].linev +
								  (**lHndlLos)[lLunghezzaLos - 2].descent;

			lPtrBoxTesto = getxtbox(NULL,NULL,FALSE);
			lAltezzaBox = lAltezzaUltimaLinea +
						  lPtrBoxTesto->t.textinset[BOTTOM_INSET] +
						  lPtrBoxTesto->t.textinset[TOP_INSET] +
						  + (2 * lPtrBoxTesto->t.box.frame.width);

			lPtrBoxTesto->t.box.s.r.fr.bottom = lPtrBoxTesto->t.box.s.r.fr.top +
												lAltezzaBox;

			setxtbox(lPtrBoxTesto, NULL, FALSE, TRUE);
			disposextbox(lPtrBoxTesto,TRUE);
			xecalc(FALSE);

			lFinito = TRUE;
		}
		else
		{
			// ho allungato il box ma c'e' del testo che ancora non si vede bene
			lFinito = FALSE;
		}

		DisposeHandle((Handle) lHndlLos);

		if (lFinito == TRUE) 
			break;
	}
} // AumentaAltezzaBox

/* ------------------------------------------------------------------------ *

	GestioneBoxLunghiIngombro

* ------------------------------------------------------------------------ */
static void XTAPI GestioneBoxLunghiIngombro() throw()
{
	// box corrente
	boxid lBoxPrincipale = NULL;
	// box coolegato al box corrente
	boxid lBoxCollegato = NULL;
	// altezza del box di testo
	Fixed lAltezzaBox = 0;
	// posizione di inizio del testo da tagliare
	int32 lInizioTestoDaTagliare = 0;
	// posizione di fine del testo da tagliare
	int32 lFineTestoDaTagliare = 0;
	// per ritornare il risultato della procedura
	bool8 lBool = FALSE;
	// per salvare lo strumento corrente
	int16 lVecchioStrumento = 0;
	
	// salvo lo strumento corrente
	lVecchioStrumento = getmode();

	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);

	// salvo il boxid corrente
	lBoxPrincipale = curbox;

	// prendo le info sul testo da tagliare
	PrendiPosizioneTestoDaTagliare(&lInizioTestoDaTagliare, &lFineTestoDaTagliare);

	setmode(CONTENTSMODE);
	updatemenus(TRUE);
	xesetsel( lInizioTestoDaTagliare, lFineTestoDaTagliare, FALSE );
	updatemenus(TRUE);
	docommand(((uint32)EDITMENU_ID<<16)+M_COPY);
	
	// incremento la coordinata sinistra di una colonna
	DecrementaColonna();

	// creo il nuovo box di testo collegato
	CreaBoxTesto();
	
	xtget_curbox(&curbox);
	lBoxCollegato = curbox;

	// inserisco in testa il filetto
	ApplicaStileForte(gStrStileFiletto);

	// inserisco un ritorno a capo
	InserisciTesto((uchar*)"\r", 1);

	// copio il testo
	setmode(CONTENTSMODE);
	updatemenus(TRUE);
	xesetsel( 1, 1, FALSE );
	updatemenus(TRUE);
	docommand(((uint32)EDITMENU_ID<<16)+M_PASTE);

	// taglio i testo da box principale
	xtset_curbox(lBoxPrincipale);
	TagliaTesto(lInizioTestoDaTagliare, lFineTestoDaTagliare);

	// verifico l'altezza del box principale
	lBool = ModificaAltezzaBoxAGiustezza(&lAltezzaBox);

	if (lBool == TRUE) 
	{
		// incremento dell'ingombro totale
		IncrementaIngombroTotale(lAltezzaBox);

		// incremento dell'ingombro totale della distanza tra i box
		if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE)
			IncrementaSpazioTraNecrologiTotale(kFixedMinimoSpazioTraNecrologi);
		else 
			IncrementaSpazioTraNecrologiTotale(gXtPreferences.giustificazionePref.valoreIniziale);


	} 
	else 
	{
		// devo segnalare che non ho potuto allargare poich‚ occupa tutta una colonna
		sprintf((char*)gStringaC, "%s %s", PrendiCodiceDefunto(), PrendiCodiceNecrologio());
		ConcatenaAllaStringaErrore(gStringaC);
		DaiErrore(kErroreBoxConSbordo);
	}

	// proseguo la verifica sul box collegato
	xtset_curbox(lBoxCollegato);
	setmode(lVecchioStrumento);
} // GestioneBoxLunghiIngombro

/* ------------------------------------------------------------------------ *

	GestioneBoxLunghiDefinitivo

* ------------------------------------------------------------------------ */
static void XTAPI GestioneBoxLunghiDefinitivo(appware *ptrappware) throw()
{
	// box corrente
	boxid lBoxPrincipale = NULL;
	// box coolegato al box corrente
	boxid lBoxCollegato = NULL;
	// altezza del box di testo
	Fixed lAltezzaBox = 0;
	// posizione di inizio del testo da tagliare
	int32 lInizioTestoDaTagliare = 0;
	// posizione di fine del testo da tagliare
	int32 lFineTestoDaTagliare = 0;
	// per ritornare il risultato della procedura
	Boolean lBool = FALSE;
	// spazio tra i paragrafi 
	Fixed lSpazioTraParagrafi = 0;
	// numero paragrafi con spazio diverso da zero
	int32 lNumeroParagrafiDiversi = 0;
	// numero paragrafi
	int32 lNumeroParagrafi = 0;
	// numero linee 
	int32 lNumeroLinee = 0;
	// allargamento 
	Fixed lAllargamento = 0;
	// restrigimento 
	Fixed lRestringimento = 0;
	// per salvare lo strumento corrente
	int16 lVecchioStrumento = 0;
	
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);

	// salvo lo strumento corrente
	lVecchioStrumento = getmode();

	// salvo il boxid corrente
	lBoxPrincipale = curbox;

	// prendo le info sul testo da tagliare
	PrendiPosizioneTestoDaTagliare(&lInizioTestoDaTagliare, &lFineTestoDaTagliare);

	setmode(CONTENTSMODE);
	updatemenus(TRUE);
	xesetsel( lInizioTestoDaTagliare, lFineTestoDaTagliare, FALSE );
	updatemenus(TRUE);
	docommand(((uint32)EDITMENU_ID<<16)+M_COPY);

	// incremento la coordinata sinistra di una colonna
	DecrementaColonna();

	// creo il nuovo box di testo collegato
	CreaBoxTesto();
	xtget_curbox(&curbox);
	lBoxCollegato = curbox;

	setmode(CONTENTSMODE);

	// inserisco in testa il filetto
	ApplicaStileForte(gStrStileFiletto);

	// inserisco un ritorno a capo
	InserisciTesto((uchar*)"\r", 1);

	// copio il testo
	setmode(CONTENTSMODE);
	updatemenus(TRUE);
	xesetsel( 1, 1, FALSE );
	updatemenus(TRUE);
	docommand(((uint32)EDITMENU_ID<<16)+M_PASTE);

	// taglio il testo da box principale
	xtset_curbox(lBoxPrincipale);
	TagliaTesto(lInizioTestoDaTagliare, lFineTestoDaTagliare);

	// verifico l'altezza del box principale
	lBool = ModificaAltezzaBoxAGiustezza(&lAltezzaBox);

	if (lBool == TRUE) 
	{
		// incremento dell'ingombro totale
		IncrementaIngombroTotale(lAltezzaBox);

		// incremento dell'ingombro totale della distanza tra i box
		if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE)
			IncrementaSpazioTraNecrologiTotale(kFixedMinimoSpazioTraNecrologi);
		else 
			IncrementaSpazioTraNecrologiTotale(gXtPreferences.giustificazionePref.valoreIniziale);

	} 
	else 
	{
		// devo segnalare che non ho potuto allargare poich‚ occupa tutta una colonna
		sprintf((char*)gStringaC, "%s %s", (char*)PrendiCodiceDefunto(), (char*)PrendiCodiceNecrologio());
		ConcatenaAllaStringaErrore(gStringaC);
		DaiErrore(kErroreBoxConSbordo);
	}

	// imposto l'altezza box
	ImpostaAltezzaBox(ptrappware, lAltezzaBox);

	// imposto lo spazio tra i paragrafi
	lSpazioTraParagrafi = PrendiSpazioTraParagrafi();
	ImpostaSpazioTraParagrafi(ptrappware, lSpazioTraParagrafi);

	// imposto il numero di paragrafi diversi da zero
	lNumeroParagrafiDiversi = PrendiNumeroParagrafiDiversi();
	ImpostaNumeroParagrafiDiversi(ptrappware, lNumeroParagrafiDiversi);

	// imposto il numero di paragrafi
	lNumeroParagrafi = PrendiNumeroParagrafi();
	ImpostaNumeroParagrafi(ptrappware, lNumeroParagrafi);

	// imposto il numero di linee
	lNumeroLinee = PrendiNumeroLinee();
	ImpostaNumeroLinee(ptrappware, lNumeroLinee);

	// imposto l'allargamento
	lAllargamento = gXtPreferences.giustificazionePref.incrementoMaxSpazioNecrologi * lNumeroParagrafi;
	lAllargamento += gXtPreferences.giustificazionePref.incrementoMaxInterlinea * lNumeroLinee;
	if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE)
		lAllargamento += kFixedDifferenzaSpazioTraNecrologi;
	IncrementaAllargamentoTotale(lAllargamento);
	ImpostaAllargamento(ptrappware, lAllargamento);

	// imposto il restringimento
	lRestringimento = gXtPreferences.giustificazionePref.decrementoMaxSpazioParagrafi * lNumeroParagrafiDiversi;
	if (lRestringimento > lSpazioTraParagrafi) 
		lRestringimento = lSpazioTraParagrafi;
	lRestringimento += gXtPreferences.giustificazionePref.decrementoMaxInterlinea * lNumeroLinee;
	IncrementaRestringimentoTotale(lRestringimento);
	ImpostaRestringimento(ptrappware, lRestringimento);

	// imposto il collegamento
	ImpostaCollegato(ptrappware, TRUE);

	// imposto lo slug di appware al box appena creato
	gErroreXtension = SlugDiAppWare(ptrappware);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nell'assegnare lo slug al box corrente
		MostraErrore(gErroreXtension);
		return;
	}

	// inserisce le info nella struttura per fare il posizionamento
	InserisciInfoNecrologio(ptrappware);

	// proseguo la verifica sul box collegato
	xtset_curbox(lBoxCollegato);

	// imposto lo slug del codice al box collegato
	gErroreXtension = SlugDiCodice();
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nell'assegnare lo slug al box corrente 
		MostraErrore(gErroreXtension);
		return;
	}

	// le info di appware sono le stesse di prima tranne per il codice
	ImpostaCodiceBox(ptrappware, PrendiCodiceBox());

	//reimposto lo strumento che c'era
	setmode(lVecchioStrumento);
} // GestioneBoxLunghiDefinitivo

/* ------------------------------------------------------------------------ *

	CreaBoxCodiceDefunto

* ------------------------------------------------------------------------ */
void XTAPI CreaBoxCodiceDefunto() throw()
{
	// puntatore al box di testo
	xtboxptr lPtrBoxTesto = NULL;
	// per posizionare il box nella pagina corrente
	pagenumdata lDatiPagina;
	// coordinate del box
	fixedrect lCoordinateBox = {0, 0, 0, 0};
	// identificatore del box di testo creato
	boxid lIdBoxTesto = 0;
		
	// calcolo le coordinate del box 
	lCoordinateBox.bottom = PrendiMargineAlto();
	lCoordinateBox.right = PrendiMargineDestro();
	lCoordinateBox.top = lCoordinateBox.bottom - gXtPreferences.documentoPref.altezzaModulo;
	lCoordinateBox.left = lCoordinateBox.right - PrendiLarghezzaColonna();
	
	// creo il box di testo
	DocID curdoc;
	xtget_curdoc(&curdoc);
	DocRef DRef = XTGetDocRefFromDocID(curdoc);
	XTNewXTBox(DRef, CT_TEXT, SH_RECT, &lPtrBoxTesto);
	sprdfrect2page(&lPtrBoxTesto->t.box.s.r.fr);
	
	lPtrBoxTesto->t.box.s.r.fr = lCoordinateBox;
	lPtrBoxTesto->t.textinset[TOP_INSET] = 0;
	lPtrBoxTesto->t.textinset[LEFT_INSET] = 0;
	lPtrBoxTesto->t.textinset[BOTTOM_INSET] = 0;
	lPtrBoxTesto->t.textinset[RIGHT_INSET] = 0;
	
	fixedrect outsetnull = {0, 0, 0, 0};
	lPtrBoxTesto->t.box.runaround.outset = outsetnull; // <- vecchio text outset
	
	xtseq2page(PrendiPaginaCorrente(), FALSE, &lDatiPagina);	
	pagefrect2sprd(&lDatiPagina, &lPtrBoxTesto->t.box.s.r.fr);

	XTInstallBox(DRef, lPtrBoxTesto,NULL,NULL,TRUE, &lIdBoxTesto);
	
	// lIdBoxTesto = xtinstallbox(lPtrBoxTesto, NULL, NULL, TRUE);

	disposextbox(lPtrBoxTesto,TRUE);
	xtset_curbox(lIdBoxTesto);
} // CreaBoxCodiceDefunto

/* ------------------------------------------------------------------------ *

	VerificaAltezzaBoxCodiceDefunto

* ------------------------------------------------------------------------ */
void XTAPI VerificaAltezzaBoxCodiceDefunto() throw()
{
	// handle al LOS del box di testo corrente
	loshandle lHndlLos = NULL;
	// lunghezza del LOS corrente
	int32 lLunghezzaLos = 0;
	// puntatore al box di testo corrente
	xtboxptr lPtrBoxTesto = NULL;
	// identificatore del box di testo corrente
	boxid lIdBoxCorrente = 0;
	// per spostare il box nella pagina corretta
	pagenumdata lDatiPagina;
	// posizione del primo carattere del flusso di testo del box
	int32 lPosizioneUltimoCarattere = 0;
	// coordinate del box alta a sinistra
	fixedpoint lCoordinateBox = {0, 0};
	// per sapere se c'e' dell'overflow del testo del box
	Fixed lSituazioneTesto = 0;
	// altezza dell'ultima riga per aggioornare il bottom del box
	Fixed lAltezzaUltimaLinea = 0;
	// altezza del box di testo
	Fixed lAltezzaBox = 0;
	
	// aumento la lunghezza del box di testo fino in cima alla colonna
	lPtrBoxTesto = getxtbox(NULL, NULL, FALSE);
	sprdfrect2page(&lPtrBoxTesto->t.box.s.r.fr);
	// cambio le coordinate del top
	lPtrBoxTesto->t.box.s.r.fr.top = 0;
	xtseq2page(PrendiPaginaCorrente(), FALSE, &lDatiPagina);	
	pagefrect2sprd(&lDatiPagina, &lPtrBoxTesto->t.box.s.r.fr);
	setxtbox(lPtrBoxTesto, NULL, FALSE, TRUE);
	disposextbox(lPtrBoxTesto, TRUE);
	xecalc(FALSE);

	// prende il LOS del box di testo corrente
	boxid curbox;
	xtget_curbox(&curbox);
	lIdBoxCorrente = curbox;
	lPosizioneUltimoCarattere = 0;
	lHndlLos = (loshandle) NewHandle(0);
	xtgetlos(&lIdBoxCorrente, &lPosizioneUltimoCarattere, FALSE, &lCoordinateBox, &lHndlLos);
	lLunghezzaLos = GetHandleSize((Handle) lHndlLos) / sizeof(losrec);
	
	lSituazioneTesto = (**lHndlLos)[lLunghezzaLos - 1].leading;
	   
	if (lSituazioneTesto == ENDOTXT) 
	{
		// tutto il testo nel box e visibile
		lAltezzaUltimaLinea = (**lHndlLos)[lLunghezzaLos - 2].linev +
							  (**lHndlLos)[lLunghezzaLos - 2].descent;
		
		lPtrBoxTesto = getxtbox(NULL,NULL,FALSE);
		lAltezzaBox = lAltezzaUltimaLinea + 
					  lPtrBoxTesto->t.textinset[BOTTOM_INSET] +
					  lPtrBoxTesto->t.textinset[TOP_INSET] +
					  + (2L * lPtrBoxTesto->t.box.frame.width);

		lPtrBoxTesto->t.box.s.r.fr.top = lPtrBoxTesto->t.box.s.r.fr.bottom -
										 lAltezzaBox;
										  
		setxtbox(lPtrBoxTesto, NULL, FALSE, TRUE);
		disposextbox(lPtrBoxTesto,TRUE);
		xecalc(FALSE);

	}
	
	DisposeHandle((Handle) lHndlLos);
} // VerificaAltezzaBoxCodiceDefunto

/* ------------------------------------------------------------------------ *

	CreaBoxTesto

	31 Ottobre 1995						Stefano

	Descrizione:
	Crea un box di testo sulle coordinate correnti e lo imposta come box corrente

	Parametri:
	nessuno

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
void XTAPI CreaBoxTesto() throw()
{
	// puntatore al box di testo
	xtboxptr lPtrBoxTesto = NULL;
	// per posizionare il box nella pagina corrente
	pagenumdata lDatiPagina;
	// coordinate del box
	fixedrect lCoordinateBox = {0, 0, 0, 0};
	// identificatore del box di testo creato
	boxid lIdBoxTesto = 0;
	// per prendere la coordinata alta del box da posizionare
	Fixed lCoordinataAlta = 0;
		
	// calcolo le coordinate del box
	lCoordinataAlta = PrendiCoordinataAlta();
	if (lCoordinataAlta != PrendiMargineAlto()) 
		lCoordinataAlta += gXtPreferences.giustificazionePref.valoreIniziale;
	lCoordinateBox.top = lCoordinataAlta;
	lCoordinateBox.left = PrendiCoordinataSinistra();
	lCoordinateBox.bottom = lCoordinateBox.top + kFixedAltezzaInterna;
	lCoordinateBox.right = lCoordinateBox.left + PrendiLarghezzaColonna();
	
	// creo il box di testo
	DocID curdoc;
	xtget_curdoc(&curdoc);
	DocRef DRef = XTGetDocRefFromDocID(curdoc);
	XTNewXTBox(DRef, CT_TEXT, SH_RECT, &lPtrBoxTesto);
	sprdfrect2page(&lPtrBoxTesto->t.box.s.r.fr);
	
	lPtrBoxTesto->t.box.s.r.fr = lCoordinateBox;
	lPtrBoxTesto->t.textinset[TOP_INSET] = 0;
	lPtrBoxTesto->t.textinset[LEFT_INSET] = 0;
	lPtrBoxTesto->t.textinset[BOTTOM_INSET] = 0;
	lPtrBoxTesto->t.textinset[RIGHT_INSET] = 0;
	
	fixedrect outsetnull = {0, 0, 0, 0};
	lPtrBoxTesto->t.box.runaround.outset = outsetnull; // <- vecchio text outset

	xtseq2page(PrendiPaginaCorrente(), FALSE, &lDatiPagina);	
	pagefrect2sprd(&lDatiPagina, &lPtrBoxTesto->t.box.s.r.fr);
	
	XTInstallBox(DRef, lPtrBoxTesto,NULL,NULL,TRUE, &lIdBoxTesto);
	
	// lIdBoxTesto = xtinstallbox(lPtrBoxTesto, NULL, NULL, TRUE);

	disposextbox(lPtrBoxTesto,TRUE);
	xtset_curbox(lIdBoxTesto);
} // CreaBoxTesto

/* ------------------------------------------------------------------------ *

	CreaBoxTesto

* ------------------------------------------------------------------------ */
void XTAPI CreaBoxImmagine() throw()
{
	// puntatore al box di immagine
	xtboxptr lPtrBoxImmagine = NULL;
	// per posizionare il box nella pagina corrente
	pagenumdata lDatiPagina ;
	// coordinate del box
	fixedrect lCoordinateBox = {0, 0, 0, 0};
	// identificatore del box di testo creato
	boxid lIdBoxImmagine = 0;
	// per prendere la coordinata alta del box da posizionare
	Fixed lCoordinataAlta = 0;
		
	// calcolo le coordinate del box
	lCoordinataAlta = PrendiCoordinataAlta();
	if (lCoordinataAlta != PrendiMargineAlto()) 
		lCoordinataAlta += gXtPreferences.giustificazionePref.valoreIniziale;
	lCoordinateBox.top = lCoordinataAlta;
	lCoordinateBox.left = PrendiCoordinataSinistra();
	lCoordinateBox.bottom = lCoordinateBox.top + gXtPreferences.documentoPref.altezzaModulo;
	lCoordinateBox.right = lCoordinateBox.left + PrendiLarghezzaColonna();
	
	// creo il box di testo
	DocID curdoc;
	xtget_curdoc(&curdoc);
	DocRef DRef = XTGetDocRefFromDocID(curdoc);
	XTNewXTBox(DRef, CT_PICT, SH_RECT, &lPtrBoxImmagine);
	sprdfrect2page(&lPtrBoxImmagine->p.box.s.r.fr);
	
	lPtrBoxImmagine->p.box.s.r.fr = lCoordinateBox;	

	xtseq2page(PrendiPaginaCorrente(), FALSE, &lDatiPagina);	
	pagefrect2sprd(&lDatiPagina, &lPtrBoxImmagine->p.box.s.r.fr);
	
	XTInstallBox(DRef, lPtrBoxImmagine,NULL,NULL,TRUE, &lIdBoxImmagine);
	
	// lIdBoxImmagine = xtinstallbox(lPtrBoxImmagine, NULL, NULL, TRUE);

	disposextbox(lPtrBoxImmagine,TRUE);
	xtset_curbox(lIdBoxImmagine);
} // CreaBoxImmagine

/* ------------------------------------------------------------------------ *

	CreaBoxImmagineAncorato

* ------------------------------------------------------------------------ */
void XTAPI CreaBoxImmagineAncorato() throw()
{
	// handle al flusso di testo corrente
	xehandle lHndlXE = NULL;
	// inizio e fine del testo attualmente selezionato
	long lInizioSelezione = 0;
	long lFineSelezione = 0;
	// lunghezza del flusso di testo corrente
	long lLunghezzaTesto = 0;
	// puntatore al box di immagine
	xtboxptr lPtrBoxImmagine = NULL;
	// coordinate del box
	fixedrect lCoordinateBox = {0, 0, 0, 0};
	// identificatore del box di immagine creato
	boxid lIdBoxImmagine = 0;
	// carattere da inserire dopo il box ancorato
	uchar lUltimoCarattere[1] = "";
	
	// porto il cursore alla fine del flusso di testo corrente
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	xesetsel(lLunghezzaTesto, lLunghezzaTesto, FALSE);
	
	// inserimento di un carattere di fine paragrafo mangiato dal box ancorato di testo
	lUltimoCarattere[0] = RETURNCHAR;
	xeputchars(lLunghezzaTesto, 1, lUltimoCarattere);
	xesetsel(lLunghezzaTesto, lLunghezzaTesto, FALSE);

	// calcolo le coordinate del box
	lCoordinateBox.top = 0;
	lCoordinateBox.left = 0;
	lCoordinateBox.bottom = gXtPreferences.immaginiPref.altezzaImg;
	lCoordinateBox.right = gXtPreferences.immaginiPref.larghezzaImg;
	
	// creo il box immagine
	lPtrBoxImmagine = newxtbox(CT_PICT, SH_RECT, TRUE);
	lPtrBoxImmagine->p.box.s.r.fr = lCoordinateBox;
	
	if (PrendiImmagineCampione() == NULL) 
	{
		lPtrBoxImmagine->p.box.frame.width = mFixed(1) / 2;
		lPtrBoxImmagine->p.box.frame.style = SOLIDLINE;
		
		// non c'e' colore di sfondo del box
		lPtrBoxImmagine->p.box.a.nonecolor = TRUE;
	} 
	else 
	{
		lPtrBoxImmagine->p.box.frame.width = (PrendiImmagineCampione())->p.box.frame.width;
		lPtrBoxImmagine->p.box.frame.style = (PrendiImmagineCampione())->p.box.frame.style;
		lPtrBoxImmagine->p.box.a.nonecolor = (PrendiImmagineCampione())->p.box.a.nonecolor;
	}
	
	// il testo esterno al box ancorato e' allineato con la base del box ancorato
	lPtrBoxImmagine->p.box.anchbits.aligntop = 0;
	
	// outset del testo su tutti i lati del box ancorato
	fixedrect outsetnull = {0, 0, 0, 0};
	lPtrBoxImmagine->p.box.runaround.outset = outsetnull; // <- vecchio text outset
		
	lIdBoxImmagine = xtinstallanchbox(lPtrBoxImmagine, NULL, BADCHARPOS);
	disposextbox(lPtrBoxImmagine, TRUE);
	xecalc(XEDOALL);

	XTSetBoxPositionLock(lIdBoxImmagine, TRUE);
		
	xtset_curbox(lIdBoxImmagine);
} // CreaBoxImmagineAncorato

/* ------------------------------------------------------------------------ *

	CreaBoxTestoAncorato

* ------------------------------------------------------------------------ */
void XTAPI CreaBoxTestoAncorato() throw()
{
	// handle al flusso di testo corrente
	xehandle lHndlXE = NULL;
	// inizio e fine del testo attualmente selezionato 
	int32 lInizioSelezione = 0;
	int32 lFineSelezione = 0;
	// lunghezza del flusso di testo corrente 
	int32 lLunghezzaTesto = 0;
	// puntatore al box di testo 
	xtboxptr lPtrBoxTesto = NULL;
	// coordinate del box 
	fixedrect lCoordinateBox = {0, 0, 0, 0};
	// identificatore del box di testo creato 
	boxid lIdBoxTesto = 0;
	// carattere da inserire dopo il box ancorato 
	uchar lUltimoCarattere[1] = "";
	
	// porto il cursore alla fine del flusso di testo corrente 
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	xesetsel(lLunghezzaTesto, lLunghezzaTesto, FALSE);
	
	// inserimento di un carattere di fine paragrafo mangiato dal box ancorato di testo 
	lUltimoCarattere[0] = RETURNCHAR;
	xeputchars(lLunghezzaTesto, 1, lUltimoCarattere);
	xesetsel(lLunghezzaTesto, lLunghezzaTesto, FALSE);

	// calcolo le coordinate del box 
	lCoordinateBox.top = 0;
	lCoordinateBox.left = 0;
	lCoordinateBox.bottom = gXtPreferences.immaginiPref.altezzaImg;
	lCoordinateBox.right = gXtPreferences.immaginiPref.larghezzaImg;
	
	// creo il box immagine 
	lPtrBoxTesto = newxtbox(CT_TEXT, SH_RECT, TRUE);
	lPtrBoxTesto->t.box.s.r.fr = lCoordinateBox;
	lPtrBoxTesto->t.box.frame.width = mFixed(1);
	lPtrBoxTesto->t.box.frame.style = SOLIDLINE;
	lPtrBoxTesto->t.textalign = TEXTCENTER;
	
	// non c'e' colore di sfondo del box 
	lPtrBoxTesto->t.box.a.nonecolor = TRUE;
	
	// il testo esterno al box ancorato e' allineato con la base del box ancorato 
	lPtrBoxTesto->t.box.anchbits.aligntop = 0;
	
	// outset del testo su tutti i lati del box ancorato 
	fixedrect outsetnull = {0, 0, 0, 0};
	lPtrBoxTesto->t.box.runaround.outset = outsetnull;
		
	lIdBoxTesto = xtinstallanchbox(lPtrBoxTesto, NULL, BADCHARPOS);
	disposextbox(lPtrBoxTesto, TRUE);
	xecalc(XEDOALL);
		
	xtset_curbox(lIdBoxTesto);
} // CreaBoxTestoAncorato


/* ------------------------------------------------------------------------ *

	VerificaAltezzaBoxTestoBozza

* ------------------------------------------------------------------------ */
void XTAPI VerificaAltezzaBoxTestoBozza() throw()
{
	// identificatore del box di testo appena spostato
	boxid lIdSpread = 0;
	// altezza del box di testo
	Fixed lAltezzaBox = 0;
	// risultato dell'operazione di modifica dell'altezza del box
	Boolean lBool = FALSE;
	// per sapere che tipodi spostamento Š stato fatto sul box 
	tipospostamento lTipoSpostamento = kNuovaColonna;
	
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	while (1) 
	{
		lBool = ModificaAltezzaBoxAGiustezza(&lAltezzaBox);		   
		if (lBool == TRUE) 
		{
			// il testo e' completamente visibile
			break;
		} 
		else 
		{
			// il testo non e' completamente visibile
			if (PrendiContatoreBoxInColonna() > 1) 
			{
				// bisogna cambiare colonna o eventualmente la pagina
				lTipoSpostamento = CambiaCoordinate();
				if (lTipoSpostamento == kNuovaPagina) 
				{
					// sposto sul nuovo spread il box corrente
					lIdSpread = getsprdbox(PrendiPaginaCorrente());
					xtrelinkbox(curbox, lIdSpread);
				}
				// sposta il box corrente sulla nuova colonna 
				SpostaBoxCorrente();
			} 
			else 
			{
				// bisogna creare un nuovo box nella colonna successiva e
				// collegare il flusso di testo con il testo corrente
				lTipoSpostamento = CambiaCoordinate();
				CreaBoxTestoCollegato();
			}
		}
	}	
} // VerificaAltezzaBoxTestoBozza

/* ------------------------------------------------------------------------ *

	VerificaAltezzaBoxTestoIngombro

* ------------------------------------------------------------------------ */
void XTAPI VerificaAltezzaBoxTestoIngombro() throw()
{
	// altezza del box di testo
	Fixed lAltezzaBox = 0;
	// risultato dell'operazione di modifica dell'altezza del box
	Boolean lBool = FALSE;
	
	while (1) 
	{
		lBool = ModificaAltezzaBoxAGiustezza(&lAltezzaBox);
			
		if (lBool == TRUE) 
		{
			// incremento dell'ingombro totale
			IncrementaIngombroTotale(lAltezzaBox);
	
			// incremento dell'ingombro totale della distanza tra i box
			if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE)
				IncrementaSpazioTraNecrologiTotale(kFixedMinimoSpazioTraNecrologi);
			else IncrementaSpazioTraNecrologiTotale(gXtPreferences.giustificazionePref.valoreIniziale);
			
			break;	
		} 
		else 
		{
			// il testo non e' completamente visibile
			// gestione del box di testo piu' lunghi della colonna
			GestioneBoxLunghiIngombro();
		}
	}
	// riporto la coordinata sinistra sulla prima colonna
	PrimaColonnaIngombro();
} // VerificaAltezzaBoxTestoIngombro

/* ------------------------------------------------------------------------ *

	VerificaAltezzaBoxTestoDefinitivoManuale

* ------------------------------------------------------------------------ */
void XTAPI VerificaAltezzaBoxTestoDefinitivoManuale() throw()
{
	// altezza del box di testo
	Fixed lAltezzaBox = 0;
	// risultato dell'operazione di modifica dell'altezza del box
	Boolean lBool = FALSE;
	
	lBool = ModificaAltezzaBoxAGiustezza(&lAltezzaBox);
		
	if (lBool == TRUE) 
	{
		// incremento dell'ingombro totale
		IncrementaIngombroTotale(lAltezzaBox);

		// incremento dell'ingombro totale della distanza tra i box
		if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE)
			IncrementaSpazioTraNecrologiTotale(kFixedMinimoSpazioTraNecrologi);
		else 
			IncrementaSpazioTraNecrologiTotale(gXtPreferences.giustificazionePref.valoreIniziale);

	} 
	else 
	{
		// il testo non e' completamente visibile
		// devo segnalare che non ho potuto allargare poiche' occupa tutta una colonna
		sprintf((char*)gStringaC, "%s %s", (char*)PrendiCodiceDefunto(), (char*)PrendiCodiceNecrologio());
		ConcatenaAllaStringaErrore(gStringaC);
		DaiErrore(kErroreBoxConSbordo);
	}
} // VerificaAltezzaBoxTestoDefinitivoManuale

/* ------------------------------------------------------------------------ *

	VerificaAltezzaBoxTestoDefinitivoSequenza

* ------------------------------------------------------------------------ */
void XTAPI VerificaAltezzaBoxTestoDefinitivoSequenza() throw()
{
	// identificatore del box di testo appena spostato 
	boxid lIdSpread = 0;
	// altezza del box di testo
	Fixed lAltezzaBox = 0;
	// risultato dell'operazione di modifica dell'altezza del box
	Boolean lBool = FALSE;
	// per sapere che tipodi spostamento Š stato fatto sul box
	tipospostamento lTipoSpostamento = kNuovaColonna;
	
	// prendi box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	while (1) 
	{
		lBool = ModificaAltezzaBoxAGiustezza(&lAltezzaBox);
		   
		if (lBool == TRUE) 
		{
			// il testo e' completamente visibile
			break;
		} 
		else 
		{
			// il testo non e' completamente visibile
			if (PrendiContatoreBoxInColonna() > 1) 
			{
				// bisogna cambiare colonna o eventualmente la pagina
				lTipoSpostamento = CambiaCoordinate();
				if (lTipoSpostamento == kNuovaPagina) 
				{
					// sposto sul nuovo spread il box corrente
					lIdSpread = getsprdbox(PrendiPaginaCorrente());
					xtrelinkbox(curbox, lIdSpread);
				}
				// sposta il box corrente sulla nuova colonna 
				SpostaBoxCorrente();
			} 
			else 
			{
				// bisogna creare un nuovo box nella colonna successiva e
				// collegare il flusso di testo con il testo corrente
				lTipoSpostamento = CambiaCoordinate();
				CreaBoxTestoCollegato();
			}
		}
	}	
} // VerificaAltezzaBoxTestoDefinitivoSequenza

/* ------------------------------------------------------------------------ *

	VerificaAltezzaBoxTestoDefinitivoAutomatico
	
* ------------------------------------------------------------------------ */
void XTAPI VerificaAltezzaBoxTestoDefinitivoAutomatico(appware *ptrappware) throw()
{
	// altezza del box di testo
	Fixed lAltezzaBox = 0;
	// risultato dell'operazione di modifica dell'altezza del box
	Boolean lBool = FALSE;
	
	while (1) 
	{
	
		lBool = ModificaAltezzaBoxAGiustezza(&lAltezzaBox);
			
		if (lBool == TRUE) 
		{
			// incremento dell'ingombro totale
			IncrementaIngombroTotale(lAltezzaBox);
	
			// incremento dell'ingombro totale della distanza tra i box
			if (gXtPreferences.giustificazionePref.abilitaSpazioTraNecrologi == TRUE)
				IncrementaSpazioTraNecrologiTotale(kFixedMinimoSpazioTraNecrologi);
			else 
				IncrementaSpazioTraNecrologiTotale(gXtPreferences.giustificazionePref.valoreIniziale);
			
			break;
	
		} 
		else 
		{
			// il testo non e' completamente visibile
			// gestione del box di testo piu' lunghi della colonna
			GestioneBoxLunghiDefinitivo(ptrappware);
		}
	}
	
	// riporto la coordinata sinistra fuori pagina
	PrimaColonnaDefinitivo();
} // VerificaAltezzaBoxTestoDefinitivoAutomatico

/* ------------------------------------------------------------------------ *

	CreaFiletto

* ------------------------------------------------------------------------ */
void XTAPI CreaFiletto(Fixed coordinatasinistra, Fixed coordinataalta, int16 pagina) throw()
{
	// puntatore al box di testo
	xtboxptr lPtrFiletto = NULL;
	// per posizionare il box nella pagina corrente
	pagenumdata lDatiPagina;
	// coordinate del filetto 
	fixedrect lCoordinateFiletto = {0, 0, 0, 0};
	// identificatore del filetto creato
	boxid lIdFiletto = 0;
	
	// controllo che siano state definite le caratteristiche del filetto
	if (gCaratteristicheFiletto.definito == FALSE) return;

	lPtrFiletto = newxtbox(CT_NONE, SH_LINE, FALSE);
//	sprdpt2page(&lPtrFiletto->l.start.h, &lPtrFiletto->l.start.v);
//	sprdpt2page(&lPtrFiletto->l.end.h, &lPtrFiletto->l.end.v);  <- non dovrebbe + servire questo passaggio
	lPtrFiletto->box.s.l.start.v = coordinataalta;
	lPtrFiletto->box.s.l.start.h = coordinatasinistra;
	lPtrFiletto->box.s.l.end.v = coordinataalta;
	lPtrFiletto->box.s.l.end.h = coordinatasinistra +
						   PrendiLarghezzaColonna() -
						   gCaratteristicheFiletto.rientroSinistro -
						   gCaratteristicheFiletto.rientroDestro;
	lPtrFiletto->box.frame.width = gCaratteristicheFiletto.spessore;
	lPtrFiletto->box.frame.style = gCaratteristicheFiletto.stile;
	lPtrFiletto->box.frame.qColor.mShade = gCaratteristicheFiletto.intensita;
	lPtrFiletto->box.frame.qColor.mColorID = gCaratteristicheFiletto.colore;
	
	xtseq2page(pagina, FALSE, &lDatiPagina);	
	pagept2sprd(&lDatiPagina, &lPtrFiletto->box.s.l.start.h, &lPtrFiletto->box.s.l.start.v);
	pagept2sprd(&lDatiPagina, &lPtrFiletto->box.s.l.end.h, &lPtrFiletto->box.s.l.end.v);
	
	DocID curdoc;
	xtget_curdoc(&curdoc);
	DocRef DRef = XTGetDocRefFromDocID(curdoc);
	
	XTInstallBox(DRef, lPtrFiletto,NULL,NULL,TRUE, &lIdFiletto);
	
	// lIdFiletto = xtinstallbox(lPtrFiletto, NULL, NULL, TRUE);
	
	disposextbox(lPtrFiletto, TRUE);
	xtset_curbox(lIdFiletto);
} // CreaFiletto

/* ------------------------------------------------------------------------ *

	TogliRitornoACapo

* ------------------------------------------------------------------------ */
void XTAPI TogliRitornoACapo() throw()
{
	// handle al flusso di testo corrente
	xehandle lHndlXE = NULL;
	// inizio del testo attualmente selezionato
	int32 lInizioSelezione = 0;
	// fine del testo attualmente selezionato
	int32 lFineSelezione = 0;
	// lunghezza del flusso di testo corrente
	int32 lLunghezzaTesto = 0;
	// ultimo carattere
	uchar lCarattere = 0;
	
	// prendo box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	
	// prendo la selezione corrente
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	
	// controllo che l'ultimo carattere sia un return
	lCarattere = h_getchar(lHndlXE,lLunghezzaTesto - 1);
	if (ISPARACHAR(lCarattere)) 
	{
		xedelchars(curbox, lLunghezzaTesto - 1, lLunghezzaTesto);
		xecalc(XEDOALL);
		if (lInizioSelezione == lLunghezzaTesto) 
			lInizioSelezione--;
		if (lFineSelezione == lLunghezzaTesto) 
			lFineSelezione--;
		lLunghezzaTesto--;
	}
	
	// ripristino la selezione iniziale
	xesetsel(0, 0, TRUE);
	xecalc(XEDOALL);
	xesetsel(lInizioSelezione, lFineSelezione, FALSE);
} // TogliRitornoACapo

/* ------------------------------------------------------------------------ *

	CancellaBox

* ------------------------------------------------------------------------ */
void XTAPI CancellaBox(int16 colonna) throw()
{
	TOOLMODE oldmode = getmode();
	setmode(DRAGMODE);
	
	boxid curbox;
	xtget_curbox(&curbox);
		
	if (colonna == 0) 
		return;
	
	gErroreXtension = TrovaSlugDiIngombro(colonna);	
	if (gErroreXtension == kNessunErrore) 
	{
		deletebox(curbox, TRUE);
	} 
	else 
	{
		setmode(oldmode);
		return;
	}
	
	setmode(oldmode);
} // CancellaBox

/* ------------------------------------------------------------------------ *

	CreaColonnaIngombro

* ------------------------------------------------------------------------ */
void XTAPI CreaColonnaIngombro(Fixed coordinatasinistra, Fixed altezza) throw()
{
	// puntatore al box di ingombro
	xtboxptr lPtrBoxIngombro = NULL;
	// per posizionare il box nella prima pagina
	pagenumdata lDatiPagina;
	// coordinate del box
	fixedrect lCoordinateBox = {0, 0, 0, 0};
	// identificatore del box di ingombro creato
	boxid lIdBoxIngombro = 0;
		
	// calcolo le coordinate del box
	lCoordinateBox.top = PrendiMargineAlto();
	lCoordinateBox.left = coordinatasinistra;
	lCoordinateBox.bottom = lCoordinateBox.top + altezza;
	lCoordinateBox.right = coordinatasinistra + PrendiLarghezzaColonna();
	
	// creo il box di testo
	lPtrBoxIngombro = newxtbox(CT_PICT, SH_RECT, TRUE);
	sprdfrect2page(&lPtrBoxIngombro->p.box.s.r.fr);
	
	lPtrBoxIngombro->p.box.s.r.fr = lCoordinateBox;
	lPtrBoxIngombro->p.box.frame.width = 0;
	lPtrBoxIngombro->p.box.frame.style = SOLIDLINE;

	xtseq2page(kPrimaPagina, FALSE, &lDatiPagina);
	pagefrect2sprd(&lDatiPagina, &lPtrBoxIngombro->p.box.s.r.fr);
	
	DocID curdoc;
	xtget_curdoc(&curdoc);
	DocRef DRef = XTGetDocRefFromDocID(curdoc);
	
	XTInstallBox(DRef, lPtrBoxIngombro,NULL,NULL,TRUE, &lIdBoxIngombro);
	
	// lIdBoxIngombro = xtinstallbox(lPtrBoxIngombro, NULL, NULL, TRUE);

	disposextbox(lPtrBoxIngombro,TRUE);
	xtset_curbox(lIdBoxIngombro);
	
	// metto il box di ingombro sotto tutto */
	sendbehind(FALSE);
} // CreaColonnaIngombro

/* ------------------------------------------------------------------------ *

	InserisciColonnaDiIngombro

* ------------------------------------------------------------------------ */
boxid XTAPI InserisciColonnaDiIngombro(int16 colonna, int16 moduli) throw()
{
	// puntatore al box di ingombro
	xtboxptr lPtrBoxIngombro = NULL;
	// per posizionare il box nella prima pagina
	pagenumdata lDatiPagina;
	// coordinate del box
	fixedrect lCoordinateBox = {0, 0, 0, 0};
	// identificatore del box di ingombro creato
	boxid lIdBoxIngombro = 0;
	// altezza della colonna di ingombro
	Fixed lAltezzaIngombro = 0;
	// coordinata sinistra
	Fixed lCoordinataSinistra = 0;
		
	// calcolo l'altezza
	if (moduli == gXtPreferences.documentoPref.moduliColonna)
	{
		lAltezzaIngombro = PrendiAltezzaColonna();
	}
	else
	{
   	
		lAltezzaIngombro = moduli * gXtPreferences.documentoPref.altezzaModulo;
		lAltezzaIngombro += moduli * gXtPreferences.documentoPref.distanzaModuli;
	}

	if (lAltezzaIngombro == 0) 
		return(NULL);

	// parte dalla prima colonna
	lCoordinataSinistra = PrendiMargineSinistro() +
								 (colonna - 1) * PrendiLarghezzaColonna() +
								 (colonna - 1) * PrendiDistanzaColonne();

	if (gXtPreferences.finalePref.posizionamentoIngombri == FinaleWap::ALTOVERSOBASSORADIOBTNID)
	{
		// calcolo le coordinate del box partendo dall'alto
		lCoordinateBox.top = PrendiMargineAlto();
		lCoordinateBox.left = lCoordinataSinistra;
		lCoordinateBox.bottom = lCoordinateBox.top + lAltezzaIngombro;
		lCoordinateBox.right = lCoordinataSinistra + PrendiLarghezzaColonna();
	}
	else
	{
		// calcolo le coordinate del box partendo dal basso
		lCoordinateBox.bottom = PrendiMargineBasso();
		lCoordinateBox.left = lCoordinataSinistra;
		lCoordinateBox.top = lCoordinateBox.bottom - lAltezzaIngombro;
		lCoordinateBox.right = lCoordinataSinistra + PrendiLarghezzaColonna();
	}

	// creo il box di ingombro
	lPtrBoxIngombro = newxtbox(CT_PICT, SH_RECT, TRUE);
	sprdfrect2page(&lPtrBoxIngombro->p.box.s.r.fr);
	
	lPtrBoxIngombro->p.box.s.r.fr = lCoordinateBox;
	lPtrBoxIngombro->p.box.frame.width = 0;
	lPtrBoxIngombro->p.box.frame.style = SOLIDLINE;

	// controllo se preparare in coda o in testa
	if (gXtPreferences.finalePref.preparaDocumento == FinaleWap::PRIMAPAGINARADIOBTNID)
	{
		// creo il box sulla prima pagina
		xtseq2page(kPrimaPagina, FALSE, &lDatiPagina);
	}
	else
	{
		// creo il box sull'ultima pagina
		xtseq2page(numpages(), FALSE, &lDatiPagina);
	}

	pagefrect2sprd(&lDatiPagina, &lPtrBoxIngombro->p.box.s.r.fr);
	
	DocID curdoc;
	xtget_curdoc(&curdoc);
	DocRef DRef = XTGetDocRefFromDocID(curdoc);
	
	XTInstallBox(DRef, lPtrBoxIngombro,NULL,NULL,TRUE, &lIdBoxIngombro);
	
	// lIdBoxIngombro = xtinstallbox(lPtrBoxIngombro, NULL, NULL, TRUE);

	disposextbox(lPtrBoxIngombro,TRUE);
	xtset_curbox(lIdBoxIngombro);
	
	// imposto lo slug di ingombro
	gErroreXtension = SlugDiIngombro(colonna);
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nella creazione del codice di ingombro
		DaiErrore(gErroreXtension);
		return(0);
	}
	
	return(lIdBoxIngombro);
} // InserisciColonnaDiIngombro

/* ------------------------------------------------------------------------ *

	PrendiModuliPagina
	
* ------------------------------------------------------------------------ */
void XTAPI PrendiModuliPagina(int16 pagina, int16 *ptrnumeromoduli) throw()
{
	// per posizionare il box nella pagina corrente
	pagenumdata lDatiPagina;
	// identificatore dello spread
	boxid lIdentificatoreSpread = 0;
	// puntatore allo spread di pagina
	xtboxptr lPtrBoxSpread = NULL;
	// puntatore alle info sulla pagina corrente
	XTUPageData *lPtrPData= NULL;
	// info sulla pagina corrente
	XTUNewPageInfo lNewPageInfo;
	// posizione della mastro
	int lMastro = 0;
	
	// prendo i dati della pagina corrente 
	xtseq2page(pagina, FALSE, &lDatiPagina);
	lIdentificatoreSpread = getsprdbox(lDatiPagina.spreadnum);
	lPtrBoxSpread = (xtboxptr) lIdentificatoreSpread;
	
	// creo lo spazio per le info di pdata
	lPtrPData = (XTUPageData *) NewPtrClear(sizeof(XTUPageData));
	lPtrPData = *(lPtrBoxSpread->g.pagestuff);
	lNewPageInfo = lPtrPData->pgs[lDatiPagina.pageindex];
	
	// prendo l'identificatore della mastro
	lMastro = lNewPageInfo.defpgid;

	PrendiModuliPaginaMastro(lMastro, ptrnumeromoduli);
} // PrendiModuliPagina


/* ------------------------------------------------------------------------ *

	PrendiModuliPaginaMastro

* ------------------------------------------------------------------------ */
void XTAPI PrendiModuliPaginaMastro(int16 mastro, int16 *ptrnumeromoduli) throw()
{
	// identificatore del box da scorrere
	boxid lIdentificatoreBox = 0;
	// puntatore al box di testo */
	xtboxptr lPtrBox = NULL;
	// per posizionare il box nella pagina corrente
	pagenumdata lDatiPagina;
	// altezza del box 
	Fixed lAltezzaBox = 0;
	// larghezza del box 
	Fixed lLarghezzaBox = 0;
	// moduli di altezza 
	int16 lModuliAltezza = 0;
	// moduli di larghezza
	int16 lModuliLarghezza = 0;
	// per contare i moduli totali
	int16 lContatoreModuli = 0;
	boxid curbox;
	
	// entro nelle pagine mastro
	gotomaster(1);
	
	// prendo il primo box sulla pagina
	lIdentificatoreBox = firstbox(mastro, TRUE);

	while (1) {
	
		if (lIdentificatoreBox == NULL) break;

		xtset_curbox(lIdentificatoreBox);
		xtgetboxpage(lIdentificatoreBox, FALSE, &lDatiPagina);
		
		// prendo il box di testo corrente
		lPtrBox = getxtbox(NULL, NULL, FALSE);

		// prendo box corrente
		xtget_curbox(&curbox);
		
		// verifico che il box non sia all'esterno della pagina
		// e che il box non sia un box creato dall'impaginazione
		if (lDatiPagina.pagenum != LEFTPASTE && 
			lDatiPagina.pagenum != RIGHTPASTE &&
			IsBoxCorrenteMio() == FALSE &&
			isanchbox(curbox) == FALSE &&
			isapicture(curbox) == TRUE) 
		{
			
			// e' un box di ingombro
			// devo calcolare quanti moduli copre
			lAltezzaBox = lPtrBox->p.box.s.r.fr.bottom - 
						  lPtrBox->t.box.s.r.fr.top;


			assert(0 != gXtPreferences.documentoPref.altezzaModulo);


			lModuliAltezza = lAltezzaBox / gXtPreferences.documentoPref.altezzaModulo;
			lLarghezzaBox = lPtrBox->p.box.s.r.fr.right - 
							lPtrBox->t.box.s.r.fr.left;			
			lModuliLarghezza = 0;
			while (1) {
				lModuliLarghezza++;
				lLarghezzaBox -= PrendiLarghezzaColonna();
				lLarghezzaBox -= PrendiDistanzaColonne();
				if (lLarghezzaBox < 0) break;
			} 
			lContatoreModuli += (lModuliLarghezza * lModuliAltezza);
		}
		
		lIdentificatoreBox =  nextbox(lIdentificatoreBox, TRUE);
		disposextbox(lPtrBox, TRUE);
	}
	
	(*ptrnumeromoduli) = lContatoreModuli;
	
	// esco dal modo mastro
	gotomaster(0);
} // PrendiModuliPaginaMastro

/* ------------------------------------------------------------------------ *

	PrendiAltezzaBox

* ------------------------------------------------------------------------ */
Fixed XTAPI PrendiAltezzaBox() throw()
{
	// puntatore al box
	xtboxptr lPtrBox = NULL;
	// risultato
	Fixed lAltezza = 0;
	boxid curbox;
	
	// prendo box corrente
	xtget_curbox(&curbox);

	// controllo se c'e' un box selezionato 
	if (curbox == NULL)
	{
		lAltezza = 0;
	}
	else
	{
		// prendo il box corrente
		lPtrBox = getxtbox(NULL, NULL, FALSE);
	
		// calcolo l'altezza
		lAltezza = lPtrBox->p.box.s.r.fr.bottom - lPtrBox->p.box.s.r.fr.top;

		disposextbox(lPtrBox, TRUE);
	}
		
	return(lAltezza);
	
} // PrendiAltezzaBox

/* ------------------------------------------------------------------------ *

	CopiaIncollaBoxPaxAncorato

* ------------------------------------------------------------------------ */
errorixtension XTAPI CopiaIncollaBoxPaxAncorato() throw()
{
	// handle al flusso di testo corrente
	xehandle lHndlXE = NULL;
	// inizio e fine del testo attualmente selezionato
	int32 lInizioSelezione = 0;
	int32 lFineSelezione = 0;
	// lunghezza del flusso di testo corrente
	int32 lLunghezzaTesto = 0;
	// per salvare e ripristinare il box corrente prima della creazione
	boxid ilVecioBoxCorrente = 0;	
	// puntatore al box di immagine
	xtboxptr lPtrBoxImmagine = NULL;
	// carattere da inserire dopo il box ancorato
	uchar lUltimoCarattere[1] = "";
	// per prendere il boxid del box di pax
	boxid ilBoxDiPax = 0L;
	
	// salvo il box corrente
	boxid curbox;
	xtget_curbox(&curbox);
	ilVecioBoxCorrente = curbox;

	// recupero il box campione da duplicare
	ilBoxDiPax = PrendiImmagineCampionePax();
	if (ilBoxDiPax == 0L)
	{
		//non ho trovato il box di pax
		return(kErroreGenerico);
	}

	// duplico il box campione
	setmode( DRAGMODE );
	// pasteitemsatxy(cbid(), 1, ilBoxDiPax, 0, 0 );
	
	pagenumdata lPageNumData = {0, 0, 0, 0, 0};
	xtgetboxpage(ilVecioBoxCorrente, FALSE, &lPageNumData);
	pasteitemsatxy(getdocbyindex(0), lPageNumData.spreadnum, ilBoxDiPax, 0, 0);
	
	updatemenus(TRUE);
	docommand(((uint32)EDITMENU_ID<<16)+M_CUT);
	setmode(CONTENTSMODE);
	
	xtset_curbox(ilVecioBoxCorrente);

	// porto il cursore alla fine del flusso di testo corrente
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	xesetsel(lLunghezzaTesto, lLunghezzaTesto, FALSE);

	// inserimento di un carattere di fine paragrafo mangiato dal box ancorato
	lUltimoCarattere[0] = RETURNCHAR;
	xeputchars(lLunghezzaTesto, 1, lUltimoCarattere);
	xesetsel(lLunghezzaTesto, lLunghezzaTesto, FALSE);

	// incolla il box copiato
	updatemenus(TRUE);
	docommand(((uint32)EDITMENU_ID<<16)+M_PASTE);
	xecalc(XEDOALL);
	
	return (kNessunErrore);
} // CopiaIncollaBoxPaxAncorato
