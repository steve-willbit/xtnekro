
/* ------------------------------------------------------------------------ *

	SimboloPax.cpp

	11 Maggio 2005				Fabrizio

	Descrizione:
	Contiene le procedure per impostare un box come campione per il simbolo 
	della pax 
	
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
#include <string>
#include <sstream>
#include <string.h>

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "Errori.h"
#include "OpXt.h"
#include "ReadFile.h"
#include "AppWare.h"
#include "Gesslug.h"
#include "Immagini.h"
#include "SimbPax.h"

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	ImpostaSimboloPax

* ------------------------------------------------------------------------ */
errorixtension XTAPI ImpostaSimboloPax() throw()
{	
	boxid ilVecchioBox = 0L;
	// per cercare un eventuale simbolo gia' definito
	
	// verifico che non esista gia' un box indicato come simbolo della pax
	gErroreXtension = TrovaSlugDiPax(&ilVecchioBox);
	if (gErroreXtension == kErroreSlugSimboloPaxPresente)
	{
		// ho trovato un box campione
		// lo rimuovo
		RimuoviSlugDiPax(ilVecchioBox);
		
		//mostro il messaggio
		MostraErrore(kErroreSlugSimboloPaxPresente);
	}
	else if (gErroreXtension != kNessunErrore)
	{
		// ho trovato il box immagine campione
		return(gErroreXtension);
	}
	
	//imposto sul box corrente il marcatore del simbolo
	gErroreXtension = SlugDiPax();
	if (gErroreXtension != kNessunErrore)
	{
		return(gErroreXtension);
	}
	
	return (kNessunErrore);
} // ImpostaSimboloPax

