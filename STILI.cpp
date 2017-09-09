/* ------------------------------------------------------------------------ *

	Stili.cpp

	12 Aprile 2005					Fabrizio

	Descrizione:
	Contiene le procedure usate per calcolare lo spazio disponibile su un
	documento con box gia' posizionati
	
	$Log: not supported by cvs2svn $
	Revision 1.1  2005/04/20 07:14:22  taretto
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

#include <stdio.h>

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "Errori.h"
#include "Stili.h"

// STATIC GLOBALS
// STATIC GLOBALS
// STATIC GLOBALS

// variabile per sapere se si e' gia' verfificato un'errore sullo stile
static int32 gErroreSulloStile = FALSE;

// descrittore che contiene le caratteristiche del filetto
caratteristichefiletto gCaratteristicheFiletto = {FALSE, 0, 0, 0, 0, 0, 0};

// PROTOTYPES
// PROTOTYPEs
// PROTOTYPES

/*! 
	@function			CercaNomeStileForte
	@abstract		gestione stili
	@discussion		Cerca lo stile passato come parametro nella tabella degli stili correntemente
					definti. Se esiste ne ritorna l'indice altrimenti ritorna 0

					15 Aprile 2005 - Fabrizio 

	@param			nomestile puntatore ad una stringa C che contiene il nome dello stile di paregrafo
					di cui cercare l'indice
	@result			ritorna l'indice dello stile ricercato
*/
static int16 CercaNomeStileForte(uchar* nomestile) throw();

/*! 
	@function			CercaNomeStileDebole
	@abstract		gestione stili
	@discussion		Cerca lo stile passato come parametro nella tabella degli stili correntemente
					definti. Se esiste ne ritorna l'indice altrimenti ritorna 0

					15 Aprile 2005 - Fabrizio 

	@param			nomestile puntatore ad una stringa C che contiene il nome dello stile di carattere
					di cui cercare l'indice
	@result			ritorna l'indice dello stile ricercato
*/
static int16 CercaNomeStileDebole(uchar* nomestile) throw();

/*! 
	@function			ApplicaStileDeboleSuTesto
	@abstract		gestione stili
	@discussion		La routine preleva dallo stile di cui e' passato come parametro l'indice,
					e le applica in modo debole alla fine del flusso di testo corrente. Nel caso
					in cui non ci sia lo stile non fa nulla. Non c'e' bisogno neanche di segnalare
					un errore poich‚ e' gia' stato fatto nella precedente procedura che ricerca
					lo stile in base al nome

					15 Aprile 2005 - Fabrizio 

	@param			indicestile indice dello stile da applicare
	@result			nessuno
*/
static void XTAPI ApplicaStileDeboleSuTesto(int16 indicestile) throw();


// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	CercaNomeStileForte

* ------------------------------------------------------------------------ */
static int16 CercaNomeStileForte(uchar* nomestile) throw()
{
 	int16 lIndiceStilePara = 0;

	if (getparastylebyname(nomestile, NULL, &lIndiceStilePara) == TRUE)
	{
		// lo stile e' definito 
		return(lIndiceStilePara);
	} 
	else 
	{
		// lo stile non e' definito
		if (gErroreSulloStile == FALSE) 
		{
			// devo segnalare l'errore perche' e' il primo di questo tipo
			ConcatenaAllaStringaErrore(nomestile);
			DaiErrore(kErroreStileNonDefinito);
			gErroreSulloStile = TRUE;
		}
		return(0);
	}
} // CercaNomeStileForte

/* ------------------------------------------------------------------------ *

	CercaNomeStileDebole

* ------------------------------------------------------------------------ */
static int16 CercaNomeStileDebole(uchar* nomestile) throw()
{
	int16 lIndiceStileChar = 0;

	if (getcharstylebyname(nomestile, NULL, &lIndiceStileChar) == TRUE)
	{
		// lo stile e' definito 
		return(lIndiceStileChar);
	} 
	else 
	{
		// lo stile non e' definito
		if (gErroreSulloStile == FALSE) 
		{
			// devo segnalare l'errore perche' e' il primo di questo tipo
			ConcatenaAllaStringaErrore(nomestile);
			DaiErrore(kErroreStileNonDefinito);
			gErroreSulloStile = TRUE;
		}
		return(0);
	}
} // CercaNomeStiledebole

/* ------------------------------------------------------------------------ *

	ApplicaStileDeboleSuTesto

* ------------------------------------------------------------------------ */
static void XTAPI ApplicaStileDeboleSuTesto(int16 indicestile) throw()
{
 	charstylerec lStile;
	
	if (getcharstylebyindex(indicestile, &lStile) == TRUE) 
	{ 							  								  		
/*
		xesetattrib(T_FACE, PLAIN);
		xesetattrib(T_FACE, lStile.t.face);
		xesetattrib(T_SIZE, lStile.t.size);
		xesetattrib(T_FONT, lStile.t.font);
		xesetattrib(T_COLOR, lStile.t.a.color);
		xesetattrib(T_SHADE, lStile.t.shade);
		xesetattrib(T_KERN, lStile.t.kern );
		xesetattrib(T_TRACK, lStile.t.track);
		xesetattrib(T_BASESHIFT, lStile.t.baseshift);
		xesetattrib(T_HSCALE,lStile.t.hscale);
*/
		// dovrebbe essere preferibile fare un set generale piuttosto che settare
		// singolarmente gli attributi
		setcharstylebyindex(indicestile, &lStile, TRUE);
	}
} // ApplicaStileDeboleSuTesto

/* ------------------------------------------------------------------------ *

	ApplicaStileForte

* ------------------------------------------------------------------------ */
void XTAPI ApplicaStileForte(uchar *nomestile) throw()
{
	xehandle lHndlXE = NULL;
	int32 lInizioSelezione = 0;
	int32 lFineSelezione = 0;
	int32 lLunghezzaTesto = 0;
	int16 lIndiceStile = 0;
	
	// porto il cursore alla fine del flusso di testo corrente
	boxid curbox;
	xtget_curbox(&curbox);
	xegetinfo(curbox, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	xesetsel(lLunghezzaTesto, lLunghezzaTesto, FALSE);

	// cerca lo stile da applicare
	lIndiceStile = CercaNomeStileForte(nomestile);
	
	// applica lo stile in modo forte
	setparastyleontext(-1, FALSE); 
	setparastyleontext(lIndiceStile, FALSE);
} // ApplicaStileForte

/* ------------------------------------------------------------------------ *

	ApplicaStileDebole

* ------------------------------------------------------------------------ */
void XTAPI ApplicaStileDebole(uchar *nomestile) throw()
{
	xehandle lHndlXE = NULL;
	int32 lInizioSelezione = 0;
	int32 lFineSelezione = 0;
	int32 lLunghezzaTesto = 0;
	int16 lIndiceStile = 0;
	
	// porto il cursore alla fine del flusso di testo corrente
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	xesetsel(lLunghezzaTesto, lLunghezzaTesto, FALSE);
	
	// cerca lo stile da applicare
	lIndiceStile = CercaNomeStileDebole(nomestile);
	
	// applica lo stile in modo debole
	ApplicaStileDeboleSuTesto(lIndiceStile);
} // ApplicaStileDebole

/* ------------------------------------------------------------------------ *

	InizializzaErroreSulloStile

* ------------------------------------------------------------------------ */
void XTAPI InizializzaErroreSulloStile() throw()
{
	gErroreSulloStile = FALSE;
} // InizializzaErroreSulloStile

/* ------------------------------------------------------------------------ *

	ImpostaCaratteristicheFiletto

* ------------------------------------------------------------------------ */
void XTAPI ImpostaCaratteristicheFiletto(uchar *nomestile) throw()
{
	int16 lIndiceStile = 0;
 	parastylerec lStile; // vecchio stylerec
 	bool8 lVecchioErroreSulloStile = FALSE;
 	
 	// prendo l'impostazione sull'errore sullo stile
 	lVecchioErroreSulloStile = gErroreSulloStile;
 	gErroreSulloStile = TRUE;
	
	// cerca lo stile da applicare
	lIndiceStile = CercaNomeStileForte(nomestile);
	
	if (getparastylebyindex(lIndiceStile, &lStile) == TRUE) 
	{
		// controllo che lo stile abbia un filetto superiore e
		// che la lughezza non sia sul testo 
		if (lStile.p.a.ruleabove == TRUE && lStile.p.a.rabovetextlen == FALSE) 
		{							  								  		
			gCaratteristicheFiletto.definito = TRUE;
			gCaratteristicheFiletto.spessore = lStile.p.rabove.width;
			gCaratteristicheFiletto.stile = lStile.p.rabove.style;
			gCaratteristicheFiletto.colore = lStile.p.rabove.color;
			gCaratteristicheFiletto.intensita = lStile.p.rabove.shade;
			gCaratteristicheFiletto.rientroSinistro = lStile.p.rabove.leftindent;
			gCaratteristicheFiletto.rientroDestro = lStile.p.rabove.rightindent;
			return;
		}
	}
	
	// se lo stile non e' definito oppure
	// se lo stile e' definito ma non ha un filetto superiore
	gCaratteristicheFiletto.definito = FALSE;
	gCaratteristicheFiletto.spessore = 0;
	gCaratteristicheFiletto.stile = 0;
	gCaratteristicheFiletto.colore = 0;
	gCaratteristicheFiletto.intensita = 0;
	gCaratteristicheFiletto.rientroSinistro = 0;
	gCaratteristicheFiletto.rientroDestro = 0;
	
	// ripristino l'impostazione sull'errore di stile
	gErroreSulloStile = lVecchioErroreSulloStile;
} // ImpostaCaratteristicheFiletto

