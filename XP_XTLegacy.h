/****************************************************************************
**
**	XP_XTLegacy.h
**
\****************************************************************************/

#ifndef _XP_XTLEGACY_H_
#define _XP_XTLEGACY_H_

EXTERN_XT_GLOBALS
EXTERN_QXSTRING_GLOBALS
EXTERN_XUNICODE_GLOBALS

#if defined(__cplusplus) || defined(inline)
#define C_INLINE inline
#else
#define C_INLINE static
#endif

/***************************************************************************\
**
** structure used for placing a picture in a picture box						
**
\***************************************************************************/
typedef struct {
	int16 type;									/* 1 = EPS, 2 = TIFF */
	int16 volnum;
	int32 dirnum;
   uchar longpath[1];
} pathrec,**pathhandle;

#if _MSC_VER
typedef pathrec **constpathhandle;		/* stupid VC++ */
#else
typedef const pathrec **constpathhandle;
#endif
#define SIZEOFPATHREC (offsetof(pathrec,longpath))

// QXP2017

#define IF_NOT_ERR_SUCCESS_BREAK(e) if ( ERR_SUCCESS != e ) {break;}
#define IF_NULL_BREAK(p) if ( NULL == p ) {break;}
#define IF_NOT_NULL_BREAK(p) if ( NULL != p ) {break;}
#define IF_TRUE_BREAK(b) if ( b ) {break;}
#define IF_FALSE_BREAK(b) if ( !b ) {break;}
#define IF_ERR_BREAK(e) if ( e ) {break;}
#define IF_NO_ERR_BREAK(e) if ( noErr == e ) {break;}

#undef SysBeep
C_INLINE void SysBeep(int16 i)
{
	MessageBeep(MB_ICONERROR);
}

#undef QXString2CStr
C_INLINE void XTAPI QXString2CStr(QXStringRef& iQXString, char *oCStr) {
    
    oCStr[0] = 0;
    
    int32 uniCharCountLocalStr;
    QXStringGetLength(iQXString, &uniCharCountLocalStr);
    if (uniCharCountLocalStr > 0) {
        UniChar *uniCharNameBuffer;
        QXStringGetBuffer(iQXString, &uniCharNameBuffer);
        
        uint8 *uCharStr = NULL;
        
        XTextEncoding xTextEncoding = kUTextEncodingPlatform;
        int32 charCount = 0;
        bool8 isConverted = FALSE;
        APIERR err = XUFromUnicode(uniCharCountLocalStr, uniCharNameBuffer, &charCount, &uCharStr, &isConverted, xTextEncoding);
        if (err == ERR_SUCCESS)  {
            CSTRCPY(oCStr, uCharStr);
            XUReleaseCharBuffer(uCharStr);
        }
    }
    
    // QXStringDestroy(iQXString); // fatta fuori la destroy della QXStringRef
    
    return;
    
} // QXString2CStr

#undef FileExist
C_INLINE bool8 FileExist(const char* iFilePath, XFileInfoRef* oXFileInfoRef)
{
	assert( NULL != oXFileInfoRef );
    assert( NULL != iFilePath );
    
    bool8 fileExist = FALSE;
    
    QXStringRef fullPathRef = NULL;
    XFileInfoRef fullPathFileInfoRef = NULL;
    
    do {
        APIERR apiErr = QXStringCreateFromCString(iFilePath, 0, (int32) CSTRLEN(iFilePath), &fullPathRef);
        IF_NOT_ERR_SUCCESS_BREAK(apiErr);
        
        apiErr = XTCreateXFileInfoRefFromFullPath(fullPathRef, 0, FALSE, TRUE, &fullPathFileInfoRef);
        IF_NOT_ERR_SUCCESS_BREAK(apiErr);
        
        bool8 isValid = FALSE;
        apiErr = XTIsValidXFileInfoRef(fullPathFileInfoRef, &isValid);
        IF_NOT_ERR_SUCCESS_BREAK(apiErr);
        IF_FALSE_BREAK(isValid);
        
        apiErr = XTFileExist(fullPathFileInfoRef, &fileExist);
        IF_NOT_ERR_SUCCESS_BREAK(apiErr);
    }
    while (0);
    
    if ( fullPathRef ) {
        QXStringDestroy(fullPathRef);
    }
    
    if ( fullPathFileInfoRef && oXFileInfoRef ) {
        (*oXFileInfoRef) = fullPathFileInfoRef;
    }
    
    return(fileExist);
}

#undef xd_lst_setrowtext
C_INLINE APIERR xd_lst_setrowtext(dlgitemid itemid, xrow row, uchar* text)
{
	QXStringRef textRef = NULL;
	QXStringCreateFromCString((char*) text, 0, (int32) CSTRLEN(text), &textRef);
	APIERR apiErr = XDULstSetRowText(itemid, row, textRef);
	QXStringDestroy(textRef);
	textRef = NULL;

	return(apiErr);
} 

#undef xd_edt_set
C_INLINE APIERR xd_edt_set(dlgitemid itemid, uchar* text, Fixed fix)
{
	APIERR apiErr = ERR_SUCCESS;
	if ( NULL != text ) {
		QXStringRef textRef = NULL;
		QXStringCreateFromCString((char*) text, 0, (int32) CSTRLEN(text), &textRef);
		apiErr = XDUEditSet(itemid, textRef, NULL);
		QXStringDestroy(textRef);
		textRef = NULL;
	}
	else {
		apiErr = XDUEditSet(itemid, NULL, fix);
	}
	return(apiErr);
}

#undef xd_lst_getrowstring
C_INLINE APIERR xd_lst_getrowstring(dlgitemid itemid, xrow row, uchar* text)
{
	QXStringRef textRef = NULL;
	APIERR apiErr = XDULstGetRowString(itemid, row, &textRef);
	if ( NULL != textRef ) {
		QXString2CStr(textRef, (char*) text);
		QXStringDestroy(textRef);
		textRef = NULL;
	}

	return(apiErr);
}

#undef xd_edt_get
C_INLINE APIERR xd_edt_get(dlgitemid itemid, uchar* text, Fixed* fix, void* ptr)
{
	bool8 invalid = TRUE;
	QXStringRef textRef = NULL;
	APIERR apiErr = XDUEditGet(itemid, &textRef, fix, &invalid);
	if ( NULL != textRef ) {
		QXString2CStr(textRef, (char*) text);
		QXStringDestroy(textRef);
		textRef = NULL;
	}

	return(apiErr);
}

#undef xd_tab_addtab
C_INLINE APIERR xd_tab_addtab(dlgitemid itemid,xtcbcode cbcode, int32 param,uint32 flags)
{
	APIERR apiErr = XDTabAddTabWithCBCode(itemid, cbcode, (void*) param, flags);

	return(apiErr);
}
#endif /* _XP_XTLEGACY_H_ */
