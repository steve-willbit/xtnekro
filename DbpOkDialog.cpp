/* ------------------------------------------------------------------------ *

	DbpOkDialog.cpp

	Copyright © Sinedita S.r.l 2003. All Rights Reserved.

* ------------------------------------------------------------------------ */

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpOkDialog.h"

// DEFINES
// DEFINES
// DEFINES

#if MACOS
/*!
	@defined		OKALERTID
	@abstract		dialog message window resource id.
	@discussion		Defines dialog message window resource id. Only for Macintosh.
*/
#define OKALERTID 512

/*!
	@defined		OKCANCELALERTID
	@abstract		dialog message window resource id.
	@discussion		Defines dialog message window resource id. Only for Macintosh.
*/
#define OKCANCELALERTID 513

// id di risorse di alert dialog da utilizzare per eventuali future implementazioni
#define LARGEROKALERTID 534
#define LARGEROKCANCELALERTID 540
#define OKNOCANCELALERTID 544
#endif // MACOS

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	DBP::OkDialog

* ------------------------------------------------------------------------ */
void XTAPI DBP::OkDialog(const std::string& iString, const int16 iType) throw()
{
	assert(0 != iString[0]);

#if MACOS
	Str255 msg = "\p";
	CSTRCPY((char*) msg, iString.substr(0,255).c_str());
	C2PSTR((char*) msg);
	GrafPtr lGrafPtr = NULL;
	GetPort(&lGrafPtr);
	QXStringRef stringRef = NULL; 
	QXStringCreateFromLocalString(msg, 0, STRLEN(msg), &stringRef);
	XTParamText(NULL, stringRef, NULL, NULL);
	QXStringDestroy(stringRef); 
	XTAlert(iType, OKALERTID, NULL, NULL);
	SetPort(lGrafPtr);
#endif // MACOS

#if WINOS
	HWND hwnd = NULL;
	xtget_hwndmainframe(&hwnd);
	assert(NULL != hwnd);

	MessageBox(hwnd, (LPSTR) iString.c_str(), (LPSTR) "", iType | MB_OK);
#endif // WINDOS
	
	return;
	
} // OkDialog

/* ------------------------------------------------------------------------ *

	DBP::OkDialog

* ------------------------------------------------------------------------ */
void XTAPI DBP::OkDialog(const char* iString, const int16 iType) throw()
{
	assert(NULL != iString);
	assert(0 != iString[0]);

#if MACOS	
	Str255 msg = "";
	std::strncpy((char*) msg, iString, 255);
	C2PSTR((char*) msg);
	GrafPtr lGrafPtr = NULL;
	GetPort(&lGrafPtr);
	QXStringRef stringRef = NULL; 
	QXStringCreateFromLocalString(msg, 0, STRLEN(msg), &stringRef);
	XTParamText(NULL, stringRef, NULL, NULL);
	QXStringDestroy(stringRef); 
	XTAlert(iType, OKALERTID, NULL, NULL);
	SetPort(lGrafPtr);
#endif // MACOS
	
#if WINOS
	HWND hwnd = NULL;
	xtget_hwndmainframe(&hwnd);
	assert(NULL != hwnd);

	MessageBox(hwnd, (LPSTR) iString, (LPSTR) "", iType | MB_OK);
#endif // WINDOS

	return;
	
} // DBP::OkDialog

/* ------------------------------------------------------------------------ *

	DBP::OkDialog

* ------------------------------------------------------------------------ */
void XTAPI DBP::OkDialog(const uchar* iString, const int16 iType) throw()
{
	assert(NULL != iString);
	assert(0 != iString[0]);

#if MACOS
	Str255 msg = "";
	STRNCPY(msg, iString, 255);	
	GrafPtr lGrafPtr = NULL;
	GetPort(&lGrafPtr);
	QXStringRef stringRef = NULL; 
	QXStringCreateFromLocalString(msg, 0, STRLEN(msg), &stringRef);
	XTParamText(NULL, stringRef, NULL, NULL);
	QXStringDestroy(stringRef); 
	XTAlert(iType, OKALERTID, NULL, NULL);
	SetPort(lGrafPtr);
#endif // MACOS
	
#if WINOS
	HWND hwnd = NULL;
	xtget_hwndmainframe(&hwnd);
	assert(NULL != hwnd);

	MessageBox(hwnd, (LPSTR) iString, (LPSTR) "", iType | MB_OK);
#endif // WINDOS

	return;
	
} // DBP::OkDialog

/* ------------------------------------------------------------------------ *

	DBP::LargerOkDialog

* ------------------------------------------------------------------------ */
void XTAPI DBP::LargerOkDialog(const uchar* iString, const int16 iType) throw()
{
	assert(NULL != iString);
	assert(0 != iString[0]);

#if MACOS
	Str255 msg = "";
	STRNCPY(msg, iString, 255);	
	GrafPtr lGrafPtr = NULL;
	GetPort(&lGrafPtr);
	QXStringRef stringRef = NULL; 
	QXStringCreateFromLocalString(msg, 0, STRLEN(msg), &stringRef);
	XTParamText(NULL, stringRef, NULL, NULL);
	QXStringDestroy(stringRef); 
	XTAlert(iType,LARGEROKALERTID, NULL, NULL);
	SetPort(lGrafPtr);
#endif // MACOS
	
#if WINOS
	HWND hwnd = NULL;
	xtget_hwndmainframe(&hwnd);
	assert(NULL != hwnd);

	MessageBox(hwnd, (LPSTR) iString, (LPSTR) "", iType | MB_OK);
#endif // WINDOS
	
	return;
	
} // DBP::LargerOkDialog

/* ------------------------------------------------------------------------ *

	DBP::OkCancelDialog

* ------------------------------------------------------------------------ */
int16 XTAPI DBP::OkCancelDialog(const std::string& iString, const int16 iType) throw()
{
	assert(0 != iString[0]);

	int16 res = 0;
#if MACOS
	Str255 msg = "\p";
	CSTRCPY((char*) msg, iString.substr(0,255).c_str());
	C2PSTR((char*) msg);
	GrafPtr lGrafPtr = NULL;
	GetPort(&lGrafPtr);
	QXStringRef stringRef = NULL; 
	QXStringCreateFromLocalString(msg, 0, STRLEN(msg), &stringRef);
	XTParamText(NULL, stringRef, NULL, NULL);
	QXStringDestroy(stringRef); 
	res = XTAlert(iType, OKCANCELALERTID, NULL, NULL);
	SetPort(lGrafPtr);
#endif // MACOS

#if WINOS
	HWND hwnd = NULL;
	xtget_hwndmainframe(&hwnd);
	assert(NULL != hwnd);

	res = MessageBox(hwnd, (LPSTR) iString.c_str(), (LPSTR) "", iType | MB_OKCANCEL);
#endif // WINDOS

	return(res);

} // DBP::OkCancelDialog

/* ------------------------------------------------------------------------ *

	DBP::OkCancelDialog

* ------------------------------------------------------------------------ */
int16 XTAPI DBP::OkCancelDialog(const char* iString, const int16 iType) throw()
{
	assert(NULL != iString);
	assert(0 != iString[0]);
	
	int16 res = 0;

#if MACOS	
	Str255 msg = "";
	std::strncpy((char*) msg, iString, 255);
	C2PSTR((char*) msg);
	GrafPtr lGrafPtr = NULL;
	GetPort(&lGrafPtr);
	ParamText("\p", msg, "\p", "\p");
	QXStringRef stringRef = NULL; 
	QXStringCreateFromLocalString(msg, 0, STRLEN(msg), &stringRef);
	XTParamText(NULL, stringRef, NULL, NULL);
	QXStringDestroy(stringRef); 
	res = XTAlert(iType, OKCANCELALERTID, NULL, NULL);
	SetPort(lGrafPtr);
#endif // MACOS
	
#if WINOS
	HWND hwnd = NULL;
	xtget_hwndmainframe(&hwnd);
	assert(NULL != hwnd);

	res = MessageBox(hwnd, (LPSTR) iString, (LPSTR) "", iType | MB_OK);
#endif // WINDOS

	return(res);

} //  DBP::OkCancelDialog

/* ------------------------------------------------------------------------ *

	DBP::OkCancelDialog

* ------------------------------------------------------------------------ */
int16 XTAPI DBP::OkCancelDialog(const uchar* iString, const int16 iType) throw()
{
	assert(NULL != iString);
	assert(0 != iString[0]);
	
	int16 res = 0;

#if MACOS
	Str255 msg = "";
	STRNCPY(msg, iString, 255);	
	GrafPtr lGrafPtr = NULL;
	GetPort(&lGrafPtr);
	QXStringRef stringRef = NULL; 
	QXStringCreateFromLocalString(msg, 0, STRLEN(msg), &stringRef);
	XTParamText(NULL, stringRef, NULL, NULL);
	QXStringDestroy(stringRef); 
	res = XTAlert(iType, OKCANCELALERTID, NULL, NULL);
	SetPort(lGrafPtr);
#endif // MACOS
	
#if WINOS
	HWND hwnd = NULL;
	xtget_hwndmainframe(&hwnd);
	assert(NULL != hwnd);

	res = MessageBox(hwnd, (LPSTR) iString, (LPSTR) "", iType | MB_OKCANCEL);
#endif // WINDOS

	return(res);
	
} // DBP::OkCancelDialog

/* ------------------------------------------------------------------------ *

	DBP::OkNoCancelDialog

* ------------------------------------------------------------------------ */
int16 XTAPI DBP::OkNoCancelDialog(const uchar* iString, const int16 iType) throw()
{
	assert(NULL != iString);
	assert(0 != iString[0]);
	
	int16 res = 0;

#if MACOS
	Str255 msg = "";
	STRNCPY(msg, iString, 255);	
	GrafPtr lGrafPtr = NULL;
	GetPort(&lGrafPtr);
	QXStringRef stringRef = NULL; 
	QXStringCreateFromLocalString(msg, 0, STRLEN(msg), &stringRef);
	XTParamText(NULL, stringRef, NULL, NULL);
	QXStringDestroy(stringRef); 
	res = XTAlert(iType, OKCANCELALERTID, NULL, NULL);
	SetPort(lGrafPtr);
#endif // MACOS

#if WINOS
	HWND hwnd = NULL;
	xtget_hwndmainframe(&hwnd);
	assert(NULL != hwnd);

	res = MessageBox(hwnd, (LPSTR) iString, (LPSTR) "", iType | MB_YESNOCANCEL);
#endif // WINDOS

	return(res);
	
} // DBP::OkCancelDialog

/* ------------------------------------------------------------------------ *

	$Log: not supported by cvs2svn $
	Revision 1.10  2008/07/17 15:29:35  marchese
	versione SEAT.13
	
	Revision 1.9  2005/11/08 09:27:27  marchese
	per la versione 1.1.0.3 del CPS 1.1
	
	Revision 1.8  2005/03/02 09:20:07  marchese
	corretta la costante MB_YESNOCANCEL
	
	Revision 1.7  2005/03/01 11:05:35  taretto
	aggiunto comando chiudi al menu XTTB
	
	Revision 1.6  2004/11/22 17:07:15  marchese
	definizione della macro assert
	
	Revision 1.5  2004/01/14 09:26:52  taretto
	aggiunta castizzazione su messagebox
	
	Revision 1.4  2003/05/07 07:16:23  marchese
	spostati gli standard includes nei file .h
	
	Revision 1.3  2003/05/06 16:02:34  taretto
	spostati include di sistema sul .h
	
	Revision 1.2  2003/02/27 16:29:42  marchese
	modificata la definizione di una costante (non e' attualmente utilizzata nel progetto)
	
	Revision 1.1  2002/11/20 14:33:47  marchese
	aggiunti al progetto if file DbpOkDialog.cpp
		
* ------------------------------------------------------------------------ */

