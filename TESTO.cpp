/* ------------------------------------------------------------------------ *

	Testo.cpp

	15 Aprile 2005 - Fabrizio

	Descrizione:
	Contiene le procedure usate per eseguire l'importazione del testo nel box
	corrente
	
	$Log: not supported by cvs2svn $

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

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "Errori.h"

#include "Testo.h"

/* ------------------------------------------------------------------------ *

	InserisciTesto

* ------------------------------------------------------------------------ */
void XTAPI InserisciTesto(uchar* testo, int32 lunghezza) throw()
{
	// handle al flusso di testo corrente
	xehandle lHndlXE = NULL;
	// inizio del testo attualmente selezionato
	int32 lInizioSelezione = 0;
	// fine del testo attualmente selezionato
	int32 lFineSelezione = 0;
	// lunghezza del flusso di testo corrente
	int32 lLunghezzaTesto = 0;
	
	// leggo le informazioni del flusso di testo corrente
	xegetinfo(NULL, &lHndlXE, &lInizioSelezione, &lFineSelezione, &lLunghezzaTesto);
	
	// inserisco il testo
	xeputchars(lLunghezzaTesto, lunghezza, (uchar *) testo);
	xecalc(XEDOALL);
} // InserisciTesto