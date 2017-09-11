/* ------------------------------------------------------------------------ *

	DbpResource.cpp

	Copyright © Sinedita S.r.l 2003. All Rights Reserved.

* ------------------------------------------------------------------------ */

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpResource.h"

// DEFINES
// DEFINES
// DEFINES

#if MACOS
// constante utilizzata per inizializzare la globale che memorizza l'id
// delle risorse dell'XTension
#define INVALIDRESOURCEID (-1)
#endif // MACOS

// GLOBALS
// GLOBALS
// GLOBALS

// the QuarkXPress language (only valid after XT_SETLANGUAGE call)
int16 gXTensionLanguage = verItaly;

// STATICS
// STATICS
// STATICS

#if MACOS
// the XTension resource id
static int16 gXTensionResourceId = INVALIDRESOURCEID;
#endif // MACOS

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

#if MACOS
/* ------------------------------------------------------------------------ *

	DBP::GetXTensionResourceFile

* ------------------------------------------------------------------------ */
void XTAPI DBP::GetXTensionResourceFile() throw()
{
	gXTensionResourceId = CurResFile();
} // DBP::GetResourceString
#endif // MACOS

#if MACOS
/* ------------------------------------------------------------------------ *

	DBP::UseXTensionResourceFile

* ------------------------------------------------------------------------ */
void XTAPI DBP::UseXTensionResourceFile() throw()
{
	// controlla se l'id delle risorse dell'xt e' valido
	assert(INVALIDRESOURCEID != gXTensionResourceId);

	UseResFile(gXTensionResourceId);
} // DBP::UseXTensionResourceFile
#endif // MACOS

/* ------------------------------------------------------------------------ *

	DBP::GetResourceString

* ------------------------------------------------------------------------ */
int16 XTAPI DBP::GetResourceString(uchar* s, const int16 r, const int16 n, const int16 sz) throw()
{
	assert(NULL != s);	
#if MACOS
	UseResFile(gXTensionResourceId);
	GetIndString(s, r + LANGUAGE, n);
	return(STRLEN(s));
#endif // MACOS

#if WINOS
	return(LoadString(hinst,r + LANGUAGE + n,(char*)s,sz));
#endif // WINOS

} // DBP::GetResourceString

/* ------------------------------------------------------------------------ *

	DBP::GetResourceString

* ------------------------------------------------------------------------ */
int16 XTAPI DBP::GetResourceString(char* s, const int16 r, const int16 n, const int16 sz) throw()
{
	assert(NULL != s);
#if MACOS
	UseResFile(gXTensionResourceId);
	GetIndString((uchar*) s, r + LANGUAGE, n);
	P2CSTR((uchar*) s);
	return(CSTRLEN(s));
#endif // MACOS

#if WINOS
	return(LoadString(hinst,r + LANGUAGE + n,(char*)s, sz));
#endif // WINOS

} // DBP::GetResourceString

/* ------------------------------------------------------------------------ *

	DBP::GetResourceString

* ------------------------------------------------------------------------ */
int16 XTAPI DBP::GetResourceString(std::string& s, const int16 r, const int16 n, const int16 sz) throw()
{
	char m[256] = "";
	
	int16 toRet = DBP::GetResourceString(m, r, n, sz);
	s = m;

	return(toRet);

} // DBP::GetResourceString

/* ------------------------------------------------------------------------ *

	$Log: not supported by cvs2svn $
	Revision 1.13  2008/07/17 15:30:08  marchese
	versione SEAT.13
	
	Revision 1.12  2005/11/08 09:27:27  marchese
	per la versione 1.1.0.3 del CPS 1.1
	
	Revision 1.11  2005/02/23 15:15:10  marchese
	modifiche per la lettura delle stringhe da risorse su una std::string
	
	Revision 1.10  2004/11/22 17:07:15  marchese
	definizione della macro assert
	
	Revision 1.9  2004/04/23 07:19:48  marchese
	corretto commento alla funzione UseXTensionResourceFile()
	
	Revision 1.8  2004/03/23 08:58:07  marchese
	creata la funzione DBP::UseXTensionResourceFile()
	
	Revision 1.7  2004/03/22 16:43:58  taretto
	porting sclass macos
	
	Revision 1.6  2003/10/23 10:12:04  marchese
	inziazializzazione della globale della lingua con verItaly (prima era verUS)
	
	Revision 1.5  2003/07/03 15:35:00  taretto
	prova di compilazione MacOSX
	
	Revision 1.4  2003/05/19 10:10:06  taretto
	no message
	
	Revision 1.3  2003/05/12 10:28:18  taretto
	aggiunta "GetComboBoxRow" & "MakeStringForDialog"
	
	Revision 1.2  2003/05/07 07:16:24  marchese
	spostati gli standard includes nei file .h
	
	Revision 1.1  2002/11/20 14:34:18  marchese
	aggiunto al progetto if file DbpResource.cpp
		
* ------------------------------------------------------------------------ */




















