
/* ------------------------------------------------------------------------ *

	Suono.c

	27 Ottobre 1995						Stefano

	Descrizione:
	Contiene una procedura per i suoni

* ------------------------------------------------------------------------ */

/* Required Includes ********************************************************/
#include PROJECT_HEADERS
#if WINOS
#pragma hdrstop		// force Visual C++ precompiled header
#endif

#include "Include.h"

#include "Errori.h"
#include "Suono.h"

void Suona(short idrisorsasuono)
/* ------------------------------------------------------------------------ *

	Suona

	27 Ottobre 1995						Stefano

	Descrizione:
	Suona il suono passato nel parametro

	Parametri:
	idrisorsasuono - identificatore di risorsa 'snd' del suono da suonare
	
	Ritorno:
	nessuno
	
* ------------------------------------------------------------------------ */

{
#if kNEW
#else

	SndChannelPtr lPtrCanale = NULL;
	SndListHandle lHndlSuono = NULL;
	
	lHndlSuono = (SndListHandle) GetResource(soundListRsrc, idrisorsasuono);
	if (lHndlSuono != NULL) {
		gErrore = SndNewChannel(&lPtrCanale, 0, 0, 0);
		HLock((Handle) lHndlSuono);
		gErrore = SndPlay(lPtrCanale, lHndlSuono, FALSE);
		HUnlock((Handle) lHndlSuono);
		gErrore = SndDisposeChannel(lPtrCanale, FALSE);
	}
#endif /* kNEW */
}

