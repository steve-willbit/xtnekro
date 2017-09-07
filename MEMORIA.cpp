/* ------------------------------------------------------------------------ *

	Memoria.cpp

	4 Aprile 2005 - Fabrizio

	Copyright © Sinedita S.r.l 2005. All Rights Reserved.

	Descrizione:
	Contiene due procedure per alloccare e dealloccare handle.

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

#include "Memoria.h"


// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	CreaHandle
	
* ------------------------------------------------------------------------ */
errorixtension XTAPI CreaHandle(Handle *ptrhandle, int32 dimensione) throw()
{
	// creazione dell'handle
  	(*ptrhandle) = (Handle)NewHandleClear (dimensione);

	
	if ((*ptrhandle) == NULL) 
		return(kErroreMemoria);

	// lock dell'handle appena creato
	HLock ((*ptrhandle));

	return(kNessunErrore);
} // CreaHandle


/* ------------------------------------------------------------------------ *

	LiberaTrueHandle

* ------------------------------------------------------------------------ */
void XTAPI LiberaTrueHandle(Handle myhandle) throw()
{
	if (myhandle != NULL) 
	{
		HUnlock(myhandle);
		DisposeHandle(myhandle);
		myhandle = NULL;
	}
} // LiberaTrueHandle

/* ------------------------------------------------------------------------ *

	LiberaHandle

* ------------------------------------------------------------------------ */
void XTAPI LiberaHandle(Handle *ptrhandle) throw()
{
	if ((*ptrhandle) != NULL) 
	{
		HUnlock((*ptrhandle));
		DisposeHandle((*ptrhandle));
		(*ptrhandle) = NULL;
	}
} // LiberaHandle

