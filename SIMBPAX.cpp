/* ------------------------------------------------------------------------ *

	SimboloPax.cpp

	11 Maggio 2005				Fabrizio

	Descrizione:
	Contiene le procedure per impostare un box come campione per il simbolo 
	della pax 
	
	$Log: not supported by cvs2svn $

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

