/* ------------------------------------------------------------------------ *

	DbpPref.cpp

	Copyright © Sinedita S.r.l 2003. All Rights Reserved.
	
* ------------------------------------------------------------------------ */

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpPref.h"

// the resource type where to save the xtension preferences
#define XTWLOC 'WLOC'

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	DBP::SavePalettePos

* ------------------------------------------------------------------------ */
void XTAPI DBP::SavePalettePos(
#if 0 // QXP70
		const uchar* iPrefNameKey,
		const OSType iResID,
#endif // QXP70	
		const wlochandle iWLocHandle, 
		const bool8 iShowingFlag
	) throw()
{
	assert(NULL != iWLocHandle);
#if WINOS
	(*iWLocHandle)->reserved.xtslug = (uchar) iShowingFlag; 
	savepalettepos(iWLocHandle, 0);
#endif // WINOS

#if MACOS 
#if 0 // QXP70	
	PrefsFileID pfID;
	int32 pfLength;
	OSErr pfErr = xtPrefsOpen(iPrefNameKey, kResourceFork, fsRdWrPerm, &pfID);
	if (pfErr == noErr) 
	{
		xtPrefsUseResFile(pfID);
		(wlochandle) Get1Resource(XTWLOC, iResID);
		
		(*iWLocHandle)->reserved = iShowingFlag;
		ChangedResource((Handle) iWLocHandle);
		xtPrefsWrite(pfID, &pfLength, (uchar *)*iWLocHandle);
		xtPrefsClose(pfID, kDataFork);
	}
#else	
	(*iWLocHandle)->reserved = iShowingFlag;
	ChangedResource((Handle) iWLocHandle);
	WriteResource((Handle) iWLocHandle);
	ReleaseResource((Handle) iWLocHandle);
#endif // MACOS
#endif // 0 // QXP70
} // DBP::SavePalettePos

/* ------------------------------------------------------------------------ *

	DBP::LoadPalettePos

* ------------------------------------------------------------------------ */
void XTAPI DBP::LoadPalettePos
(
#if 0 // QXP70
	const uchar* iPrefNameKey,
#endif // 0 // QXP70
	wlochandle& ioWLocHandle,
	bool8& ioShowingFlag,
#if MACOS // 
	const OSType iResID,
#endif // MACOS
	const uchar* iNameKey
) throw()
{
#if 0 // QXP70 // <- da rivedere, non funge uso e il vecchio sistema
	PrefsFileID oldResFile = xtPrefsCurResFile();
	PrefsFileID pfID;
	XTError pfErr = xtPrefsOpen(iPrefNameKey,kResourceFork,fsRdWrPerm,&pfID);
	assert(pfErr == noErr);
	xtPrefsUseResFile(pfID);
	
	// get the last position of the palette
	wlochandle wlh = (wlochandle) Get1Resource(XTWLOC, iResID);
	if (!wlh)
	{
		// create a new resource with initialized to defaults 
		wlh = (wlochandle) NewHandleClear(sizeof(wlocrec));
		assert(NULL != wlh);
		
#if MACOS		
		(*wlh)->reserved = FALSE;
#endif // MACOS		
		(*wlh)->wloc.h = 100;
		(*wlh)->wloc.v = 100;

		AddResource((Handle) wlh, XTWLOC, iResID, iNameKey);
		WriteResource((Handle) wlh);
	}
	else
	{
	
		// if the palette is located off the current desktop put it at a default location
		if (!PtInRgn((*wlh)->wloc, GetGrayRgn()))
		{
			(*wlh)->wloc.h = 100;
			(*wlh)->wloc.v = 100;
		}
		ioShowingFlag = (*wlh)->reserved;
	
		ioWLocHandle = (wlochandle) NewHandleClear(sizeof(wlocrec));
		assert(NULL != ioWLocHandle);
		// (*ioWLocHandle) = (*wlh);		
		MEMCPY((*ioWLocHandle), (*wlh), sizeof(wlocrec));
		DisposeHandle((Handle)wlh);
	}
	xtPrefsClose(pfID,kResourceFork);
	xtPrefsUseResFile(oldResFile);
	
#else
#if WINOS
	Rect box = {-1,-1,-1,-1};
	//ioWLocHandle = (0, box, 0, iNameKey); 

	QXStringRef nameKeyRef = NULL;
	QXStringCreateFromCString((char*) iNameKey, 0, (int32) CSTRLEN(iNameKey), &nameKeyRef);

	XTULoadPalettePosition(0, box, 0, nameKeyRef, &ioWLocHandle);

	QXStringDestroy(nameKeyRef);
	nameKeyRef = NULL;

	//  check to see if the palette is already open
	ioShowingFlag = (*ioWLocHandle)->reserved.xtslug;

	if
	(
		-1 == (*ioWLocHandle)->wloc.h
		&& -1 == (*ioWLocHandle)->wloc.v
	)
	{
		(*ioWLocHandle)->wloc.h = 100;
		(*ioWLocHandle)->wloc.v = 100;
	}
#endif  // WINOS

#if MACOS
	// get the last position of the palette
	ioWLocHandle = (wlochandle) Get1Resource(XTWLOC, iResID);
	if (!ioWLocHandle)
	{
		// create a new resource with initialized to defaults */
		ioWLocHandle = (wlochandle) NewHandleClear(sizeof(wlocrec));
		assert(NULL != ioWLocHandle);
		
		(*ioWLocHandle)->reserved = FALSE;
		(*ioWLocHandle)->wloc.h = 100;
		(*ioWLocHandle)->wloc.v = 100;

		AddResource((Handle) ioWLocHandle, XTWLOC, iResID, iNameKey);
	}

	// if the palette is located off the current desktop put it at a default location
	if (!PtInRgn((*ioWLocHandle)->wloc, GetGrayRgn()))
	{
		(*ioWLocHandle)->wloc.h = 100;
		(*ioWLocHandle)->wloc.v = 100;
	}
	ioShowingFlag = (*ioWLocHandle)->reserved;
#endif  // MACOS
#endif // QXP70
} // DBP::LoadPalettePos

#if 0
/* ------------------------------------------------------------------------ *

	DBP::SavePreferences

* ------------------------------------------------------------------------ */
void XTAPI DBP::SavePreferences
(
	const Ptr iPrefPtr,
	const Size iPrefSize,
	const uchar* iPrefNameKey,
#if WINOS
	const uchar* iPrefRegKey
#endif // WINOS
#if MACOS
	const OSType iPrefResType,
	const OSType iPrefResID
#endif // MACOS
) throw()
{
#if WINOS
	// preferences
	HKEY key = NULL;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, (const char*) iPrefRegKey, 0, KEY_SET_VALUE, &key))
	{
		RegSetValueEx(key, (const char*) iPrefNameKey, 0, REG_BINARY, (uchar*) iPrefPtr, iPrefSize);
		RegCloseKey(key);
		key = NULL;
	}
#endif // WINOS

#if MACOS
	// preferences
	Handle xph = Get1Resource(iPrefResType, iPrefResID);
	assert(NULL != xph);
	MEMCPY(*xph, iPrefPtr, iPrefSize);
	ChangedResource((Handle) xph);
	WriteResource((Handle) xph);
	ReleaseResource((Handle) xph);
#endif // MACOS
} // DBP::SavePreferences

/* ------------------------------------------------------------------------ *

	DBP::LoadPreferences

* ------------------------------------------------------------------------ */
void XTAPI DBP::LoadPreferences
(
	Ptr iPrefPtr,
	const Size iPrefSize,
	const uchar* iPrefNameKey,
#if WINOS
	const uchar* iPrefRegKey
#endif // WINOS
#if MACOS
	const OSType iPrefResType,
	const OSType iPrefResID
#endif // MACOS
) throw()
{
#if WINOS
	// get saved preferences (if it exists) from Registry...
	HKEY key = NULL;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, (const char*) iPrefRegKey, 0, KEY_QUERY_VALUE, &key))
	{
		// preferences
		uint32 size = iPrefSize;
		if (ERROR_SUCCESS == RegQueryValueEx(key, (const char*) iPrefNameKey, 0, NULL, (uchar*) iPrefPtr, &size))
		{
		}

		RegCloseKey(key);			
		key = NULL;
	}
	else
	{
		// ...otherwise create a new registry entry, initialized to defaults

		// preferences
		if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, (const char*) iPrefRegKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL))
		{
			RegSetValueEx(key, (const char*) iPrefNameKey, 0, REG_BINARY, (uchar*) iPrefPtr, iPrefSize);
			RegCloseKey(key);
			key = NULL;
		}
	}
#endif  // WINOS

#if MACOS
	// get preferences from resource (if it exists)...
	
	// preferences
	Handle xph = Get1Resource(iPrefResType, iPrefResID);
	if (!xph)
	{
		// ...otherwise create a new resource, initialized to defaults
		xph = NewHandleClear(iPrefSize);
		assert(NULL != xph);
		
		MEMCPY(*xph, iPrefPtr, iPrefSize);
		AddResource(xph, iPrefResType, iPrefResID, iPrefNameKey);
		WriteResource(xph);
		ReleaseResource(xph);
	}
	else
	{
		MEMCPY(iPrefPtr, *xph, iPrefSize);
		ReleaseResource((Handle) xph);
	}
#endif  // MACOS

} // DBP::LoadPreferences
#endif // 0

/* ------------------------------------------------------------------------ *

	DBP::SavePreferences

* ------------------------------------------------------------------------ */
void XTAPI DBP::SavePreferences
(
	const Ptr iPrefPtr,
	const Size iPrefSize,
	const uchar* iPrefNameKey,
	const OSType iPrefResType,
	const OSType iPrefResID
) throw()
{
	PrefsFileID oldResFile = xtPrefsCurResFile();
	PrefsFileID pfID;

	QXStringRef prefNameKeyRef = NULL;
	QXStringCreateFromCString((char*) iPrefNameKey, 0, (int32) CSTRLEN(iPrefNameKey), &prefNameKeyRef);

	XTError pfErr = XTUPrefsOpen(prefNameKeyRef,kResourceFork, fsRdWrPerm,&pfID);

	QXStringDestroy(prefNameKeyRef);
	prefNameKeyRef = NULL;

	assert(pfErr == noErr);
	xtPrefsUseResFile(pfID);
	
	Handle xph = Get1Resource(iPrefResType, iPrefResID);
	assert(NULL != xph);
	MEMCPY(*xph, iPrefPtr, iPrefSize);
	ChangedResource((Handle) xph);
	WriteResource((Handle) xph);
	ReleaseResource((Handle) xph);

	xtPrefsClose(pfID,kResourceFork);

	xtPrefsUseResFile(oldResFile);
	
} // DBP::SavePreferences

/* ------------------------------------------------------------------------ *

	DBP::LoadPreferences

* ------------------------------------------------------------------------ */
void XTAPI DBP::LoadPreferences
(
	Ptr iPrefPtr,
	const Size iPrefSize,
	const OSType iPrefResType,
	const OSType iPrefResID,
	const uchar* iPrefName
) throw()
{

	PrefsFileID oldResFile = xtPrefsCurResFile();
	PrefsFileID pfID;

	QXStringRef prefNameKeyRef = NULL;
	QXStringCreateFromCString((char*) iPrefName, 0, (int32) CSTRLEN(iPrefName), &prefNameKeyRef);

	XTError pfErr = XTUPrefsOpen(prefNameKeyRef, kResourceFork, fsRdWrPerm, &pfID);

	QXStringDestroy(prefNameKeyRef);
	prefNameKeyRef = NULL;

	assert(pfErr == noErr);
	xtPrefsUseResFile(pfID);
	
	Handle xph = Get1Resource(iPrefResType, iPrefResID);
	if (!xph)
	{
		// ...otherwise create a new resource, initialized to defaults
		xph = NewHandleClear(iPrefSize);
		assert(NULL != xph);
		
		MEMCPY(*xph, iPrefPtr, iPrefSize);
		AddResource(xph, iPrefResType, iPrefResID, iPrefName);
		WriteResource(xph);
		ReleaseResource(xph);
	}
	else
	{
		MEMCPY(iPrefPtr, *xph, iPrefSize);
		ReleaseResource((Handle) xph);
	}

	xtPrefsClose(pfID,kResourceFork);

	xtPrefsUseResFile(oldResFile);

} // DBP::Preferences
