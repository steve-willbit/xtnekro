/****************************************************************************
**
**	XP_XTLegacyE6.h
**
** This file translates some pre QuarkXPress 6.0 XTension functions to 
** QuarkXPress 6.0 XTensions.
**
** Copyright (c) 2003 - 2006 Quark Technology Partnership.
** All Rights Reserved.
**
**
\****************************************************************************/

#ifndef _XP_XTLEGACYE6_H_
#define _XP_XTLEGACYE6_H_

EXTERN_XT_GLOBALS
EXTERN_QXSTRING_GLOBALS
EXTERN_XUNICODE_GLOBALS

#if defined(__cplusplus) || defined(inline)
#define C_INLINE inline
#else
#define C_INLINE static
#endif

#define SKIPWORK (0x4000)

typedef int32 XTContentSlugDataType;
enum {
	kContentSlugType_Box,	/* Type of content slug required for a box */
	kContentSlugType_Story	/* Type of content slug required for a story */
};

typedef union {
	boxid boxID;	/* kContentSlugType_Box */
	xehandle xeH;		/* kContentSlugType_Story */
} XTContentSlugTypeData;

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


/* old XTension substitutes; replace them with new function with DocID */
C_INLINE DocUID XTAPI xtGetDocUID(DocID docID)
{
	return ((DocUID)XTGetDocRefFromDocID(docID));
}

C_INLINE void XTAPI verydirty(void)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTSetDocumentDirtyness(XTGetDocRefFromDocID(curDocID),VERYDIRTY);
}

C_INLINE XTError XTAPI xtseq2page(int16 p,bool16 checkshowdef,pagenumdata *returndata)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetPageNumData(XTGetDocRefFromDocID(curDocID),
			p,checkshowdef,returndata)));
}

C_INLINE spreadboxid XTAPI getsprdbox(int16 spreadNum)
{
	spreadboxid spreadBox = 0;
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTGetSpreadBox(XTGetDocRefFromDocID(curDocID),spreadNum,&spreadBox);
	return (spreadBox);
}

C_INLINE XTError XTAPI xtget_showmaster(uint8 *xtd_value)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetShowMaster(XTGetDocRefFromDocID(curDocID),
			xtd_value)));
}

C_INLINE XTError XTAPI xtset_showmaster(uint8 xtd_value)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetShowMaster(XTGetDocRefFromDocID(curDocID),
		xtd_value)));
}

C_INLINE XTError XTAPI xtget_dispdefspread(int16 *xtd_value)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetDispDefSpread(XTGetDocRefFromDocID(curDocID),
			xtd_value)));
}

C_INLINE XTError XTAPI xtset_dispdefspread(int16 xtd_value)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetDispDefSpread(XTGetDocRefFromDocID(curDocID),
		xtd_value)));
}

C_INLINE int16 XTAPI showdefault(int16 newdefault,bool16 update)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTShowDefault(XTGetDocRefFromDocID(curDocID),newdefault,
			update));
}

C_INLINE void XTAPI redrawpage(int16 page)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTRedrawPage(XTGetDocRefFromDocID(curDocID),page);
}

C_INLINE int16 XTAPI xtnumsprds(void)
{
	DocID curDocID = INVALDOC;
	int16 numSpreads = -1;

	xtget_curdoc(&curDocID);
	XTNumSpreads(XTGetDocRefFromDocID(curDocID),FALSE,&numSpreads);
	return (numSpreads);
}

C_INLINE int16 XTAPI xtnumpages(void)
{
	DocID curDocID = INVALDOC;
	int16 numPages = -1;

	xtget_curdoc(&curDocID);
	XTNumPages(XTGetDocRefFromDocID(curDocID),FALSE,&numPages);
	return (numPages);
}

C_INLINE boxid XTAPI firstbox(int16 spreadNum,int16 skipAnch)
{
	DocID curDocID = INVALDOC;
	boxid boxID = 0;

	xtget_curdoc(&curDocID);
	XTFirstBox(XTGetDocRefFromDocID(curDocID),spreadNum,skipAnch,&boxID);
	return (boxID);
}

C_INLINE int16 XTAPI SetPageName(int16 pageNum,const uchar *pageNameStr)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTSetPageName(XTGetDocRefFromDocID(curDocID),pageNum,
			pageNameStr));
}

C_INLINE OSErr XTAPI GetPageName(int16 pageNum,uchar *pageNameStr)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);	
	return (XTAPIERRToOSErr(XTGetPageName(XTGetDocRefFromDocID(curDocID),
			pageNum,pageNameStr)));
}

C_INLINE OSErr XTAPI GetPageNameEx(int16 pageNum,uchar *pageNameStr,
		bool8 createName)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToOSErr(XTGetPageNameEx(XTGetDocRefFromDocID(curDocID),
			pageNum,pageNameStr,createName)));
}

C_INLINE bool8 XTAPI IsValidPageName(int16 pageNum,const uchar *pageNameStr)
{
	DocID curDocID = INVALDOC;
	bool8 isValid = FALSE;

	xtget_curdoc(&curDocID);
	XTIsValidPageName(XTGetDocRefFromDocID(curDocID),pageNum,pageNameStr,
			&isValid);
	return (isValid);
}

C_INLINE void XTAPI XEDrawAll(xehandle xeH)
{
	if (xeH == NULL) {
		textboxid boxP = NULL;

		xtget_curbox(&boxP);
		if (istextbox(boxP)) {
			xegetinfo(boxP, &xeH, NULL, NULL, NULL);
		}
	}

	if (xeH != NULL) {
		XEDrawSelection(xeH, kSelCalculate);
	}
}

C_INLINE void XTAPI xedrawall(void)
{
	XEDrawAll(NULL);
}

C_INLINE bool8 XTAPI isdbldef(int16 spreadNum)
{
	DocID curDocID = INVALDOC;
	bool8 result = FALSE;

	xtget_curdoc(&curDocID);
	XTIsDoublesidedDefault(XTGetDocRefFromDocID(curDocID),spreadNum,&result);
	return (result);
}

C_INLINE int16 XTAPI numpages(void)
{
	DocID curDocID = INVALDOC;
	int16 result = -1;

	xtget_curdoc(&curDocID);
	NumPages(XTGetDocRefFromDocID(curDocID),&result);
	return (result);
}

C_INLINE int16 XTAPI numsprds(void)
{
	DocID curDocID = INVALDOC;
	int16 result = -1;

	xtget_curdoc(&curDocID);
	NumSpreads(XTGetDocRefFromDocID(curDocID),&result);
	return (result);
}

C_INLINE bool8 XTAPI IsPrintDoc(void)
{
	DocID curDocID = INVALDOC;
	bool8 result = FALSE;

	xtget_curdoc(&curDocID);
	XTIsPrintDoc(XTGetDocRefFromDocID(curDocID),&result);
	return (result);
}

C_INLINE XTError XTAPI CompareExportSetting(boxid box1P,boxid box2P,
		bool8 *sameSetting)
{
	ImageExportSettingFlags flags = (COMPARE_FILTERXTID|COMPARE_FILTER_SETTINGS);

	return (XTAPIERRToXTError(XTCompareExportSettings(box1P,box2P,flags,
			sameSetting)));
}

C_INLINE OSErr xtgetboxname(boxid theBoxID, uchar *name)
{
	assert( FALSE && "xtgetboxname" );
	return( (OSErr) -1 );
}

C_INLINE boxid XTAPI xtboxnameexists(uchar *name,BoxUID id,int16 spread)
{
	assert( FALSE && "xtboxnameexists" );
	return( NULL );
}

C_INLINE XTError XTAPI xtget_maxboxnameid(BoxUID *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetMaxBoxNameID(XTGetDocRefFromDocID(curDocID),
			returnValue)));
}

C_INLINE boxid XTAPI xtfinduniqueid(BoxUID uniqueID)
{
	DocID curDocID = INVALDOC;
	boxid boxID = 0;

	xtget_curdoc(&curDocID);
	XTFindUniqueID(XTGetDocRefFromDocID(curDocID),uniqueID,&boxID);
	return (boxID);
}

C_INLINE boxid XTAPI xtfindboxbyid(BoxUID boxUID,int16 spread)
{
	DocID curDocID = INVALDOC;
	boxid boxID = 0;

	xtget_curdoc(&curDocID);
	XTFindBoxByID(XTGetDocRefFromDocID(curDocID),boxUID,spread,&boxID);
	return (boxID);
}

C_INLINE bool8 XTAPI setrequiredexts(int32 xtid,int16 version,uint8 flags)
{
	DocID curDocID = INVALDOC;
	bool8 success = FALSE;

	xtget_curdoc(&curDocID);
	XTSetRequiredExts(XTGetDocRefFromDocID(curDocID),xtid,version,flags,
			&success);
	return (success);
}

C_INLINE bool8 XTAPI delrequiredexts(int32 xtid)
{
	DocID curDocID = INVALDOC;
	bool8 success = FALSE;

	xtget_curdoc(&curDocID);
	XTDeleteRequiredExts(XTGetDocRefFromDocID(curDocID),xtid,&success);
	return (success);
}

C_INLINE void XTAPI recalcdoc(bool16 recalc)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTRecalcDoc(XTGetDocRefFromDocID(curDocID),recalc);
}

C_INLINE XTError XTAPI xtget_curpage(int16 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetCurPage(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtget_tabspechndl(tabspechandle *returnValue)
{
	assert( FALSE && "xtget_tabspechndl" );
	return( (XTError) -1 );

}

C_INLINE XTError XTAPI xtget_validdoc(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetValidDoc(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_validdoc(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetValidDoc(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_dirtyflag(uint8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetDirtyFlag(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtget_newflag(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetNewFlag(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtget_nonnativedoc(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetNonNativeDoc(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtget_convfromold(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetConvertFromOld(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtget_istempfile(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetIsTempFile(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtget_origfrefnum(int16 *returnValue)
{
	assert( FALSE && "xtget_origfrefnum" );
	return( (XTError) -1 );
}

C_INLINE XTError XTAPI xtget_wassinglelang(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetWasSingleLanguage(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtget_multilingdoc(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetMultiLingual(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_multilingdoc(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetMultiLingual(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_docdoublesided(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetDoubleSided(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtget_signaturehndl(Handle *returnValue)
{
	assert( FALSE && "xtget_signaturehndl" );
	return( (XTError) -1 );
}

C_INLINE XTError XTAPI xtset_signaturehndl(Handle newValue)
{
	assert( FALSE && "xtset_signaturehndl" );
	return( (XTError) -1 );
}

C_INLINE XTError XTAPI xtget_spreadorigin(fixedpoint *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetSpreadOrigin(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_spreadorigin(fixedpoint *newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetSpreadOrigin(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_pageorigin(fixedpoint *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetPageOrigin(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_pageorigin(fixedpoint *newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetPageOrigin(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_istemplate(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetIsTemplate(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_istemplate(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetIsTemplate(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_docvrefnum(int16 *returnValue)
{
	assert( FALSE && "xtget_docvrefnum" );
	return( (XTError) -1 );
}

C_INLINE XTError XTAPI xtset_docvrefnum(int16 newValue)
{
	assert( FALSE && "xtset_docvrefnum" );
	return( (XTError) -1 );
}

C_INLINE XTError XTAPI xtget_doclastsaveas(uint8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetLastSaveAs(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE bool8 XTAPI IsLibraryDoc(void)
{
	DocID curDocID = INVALDOC;
	bool8 result = FALSE;
	
	xtget_curdoc(&curDocID);
	XTIsLibraryDoc(XTGetDocRefFromDocID(curDocID),&result);
	return (result);
}

C_INLINE XTError XTAPI xtget_fractional(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetFractional(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_fractional(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetFractional(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_breakemdash(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetBreakEMDash(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_breakemdash(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetBreakEMDash(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_radix(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetRadix(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_radix(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetRadix(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_oldunderline(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetOldUnderline(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_oldunderline(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetOldUnderline(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_absuloffset(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetAbsULOffset(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_absuloffset(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetAbsULOffset(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_absulthick(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetAbsULThick(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_absulthick(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetAbsULThick(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_spaceafterdrop(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetSpaceAfterDrop(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_spaceafterdrop(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetSpaceAfterDrop(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_absstrikewidth(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetAbsStrikeWidth(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_absstrikewidth(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetAbsStrikeWidth(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_absstrike2width(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetAbsStrikeSecondWidth(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_absstrike2width(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetAbsStrikeSecondWidth(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_docguidesinfront(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetGuidesInFront(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_docguidesinfront(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetGuidesInFront(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_dockeeplead(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetKeepLeading(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_dockeeplead(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetKeepLeading(
			XTGetDocRefFromDocID(curDocID),newValue)));
}


C_INLINE XTError XTAPI xtget_uloffset(Fixed *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetULOffset(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_uloffset(Fixed newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetULOffset(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_ulthickness(Fixed *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetULThickness(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_ulthickness(Fixed newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetULThickness(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_flowver(int16 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetFlowVersion(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtget_strikeoffsetwidth(Fixed *offset,Fixed *width)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetStrikeOffsetWidth(
			XTGetDocRefFromDocID(curDocID),offset,width)));
}

C_INLINE XTError XTAPI xtset_strikeoffsetwidth(Fixed offset,Fixed width)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetStrikeOffsetWidth(
			XTGetDocRefFromDocID(curDocID),offset,width)));
}

C_INLINE XTError XTAPI xtget_strike2offsetwidth(Fixed *offset,Fixed *width)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetStrikeSecondOffsetWidth(
			XTGetDocRefFromDocID(curDocID),offset,width)));
}

C_INLINE XTError XTAPI xtset_strike2offsetwidth(Fixed offset,Fixed width)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetStrikeSecondOffsetWidth(
			XTGetDocRefFromDocID(curDocID),offset,width)));
}

C_INLINE XTError XTAPI xtget_vertscaleleading(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetVertScaleLeading(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_vertscaleleading(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetVertScaleLeading(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE XTError XTAPI xtget_allowrunon(bool8 *returnValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTGetAllowRunOn(
			XTGetDocRefFromDocID(curDocID),returnValue)));
}

C_INLINE XTError XTAPI xtset_allowrunon(bool8 newValue)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return (XTAPIERRToXTError(XTSetAllowRunOn(
			XTGetDocRefFromDocID(curDocID),newValue)));
}

C_INLINE void XTAPI xeactivate(void)
{
	boxid curboxID = 0;

	xtget_curbox(&curboxID);
	XEActivate(curboxID);
}

C_INLINE void XTAPI gettattribs(uint32 index,txtattrib *t,int16 *styleID)
{
	DocID curDocID = INVALDOC;
	xecollection collection;

	xtget_curdoc(&curDocID);
	XTGetDocStoryCollection(XTGetDocRefFromDocID(curDocID),&collection);
	XTGetTextAttributes(collection,index,t,styleID);
}

C_INLINE void XTAPI getpattribs(uint32 index,paraattrib *p,int16 *styleID)
{
	DocID curDocID = INVALDOC;
	xecollection collection;

	xtget_curdoc(&curDocID);
	XTGetDocStoryCollection(XTGetDocRefFromDocID(curDocID),&collection);
	XTGetParagraphAttributes(collection,index,p,styleID);
}

C_INLINE int16 XTAPI gettabspec(int16 tabindex,tabspec *tspec)
{
	assert( FALSE && "gettabspec" );
	return( -1 );
}

C_INLINE void XTAPI missingfonterror(Handle msnglsthndl,int16 thefont)
{
	DocID curDocID = INVALDOC;
	ProjectID projectID = INVALPROJECT;
	DocRef docRef;

	xtget_curdoc(&curDocID);
	docRef = XTGetDocRefFromDocID(curDocID);
	XTGetLayoutSpaceProjectID(docRef,&projectID);
	XTShowMissingFontError(msnglsthndl,thefont,projectID,
			docRef);
}

C_INLINE int32 XTAPI decrement(xehandle xeH,int32 c)
{
	return (XEGetPrevVisCharOffset(xeH,c));
}

C_INLINE int32 XTAPI increment(xehandle xeH,int32 c)
{
	return (XEGetNextVisCharOffset(xeH,c));
}

// New to 6.0, but still Legacy for internal XTs
C_INLINE XTError XTAPI xtget_LayoutSpaceProjectID(DocID docID,ProjectID *projectID)
{
	return (XTAPIERRToXTError(XTGetLayoutSpaceProjectID(
			XTGetDocRefFromDocID(docID),projectID)));
}

C_INLINE boxid XTAPI findslug(int16 slugtype, OSType type, Handle slughndl)
{
	DocID curDocID = INVALDOC;
	boxid boxID = 0;

	xtget_curdoc(&curDocID);
	XTFindBoxSlug(XTGetDocRefFromDocID(curDocID), slugtype, type, slughndl, &boxID);
	return (boxID);
} 

C_INLINE void XTAPI curboxsprdorigin(void)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTCurBoxSpreadOrigin(XTGetDocRefFromDocID(curDocID));
}


C_INLINE void XTAPI setsprdorigin(int16 spread)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTSetSprdOrigin(XTGetDocRefFromDocID(curDocID),spread);
}

C_INLINE void XTAPI deletepages(int16 startPage,int16 endPage,int16 options)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTDeletePages(XTGetDocRefFromDocID(curDocID),startPage,endPage,options);
}

C_INLINE void XTAPI getscaledbbox(boxid bptr,Rect *r,bool8 incoutline,
	int16 fullrects)
{
	assert( FALSE && "getscaledbbox" );
	return;
}

C_INLINE void XTAPI removemultselect(void)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTRemoveMultSelect(XTGetDocRefFromDocID(curDocID));
}

C_INLINE void XTAPI doccontrect(Rect *r)	
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTDocContRect(XTGetDocRefFromDocID(curDocID),r);
}

C_INLINE void XTAPI getdisppagenum(int16 pageseq,uchar *thestr)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTGetDispPageNum(XTGetDocRefFromDocID(curDocID),pageseq,thestr);
}

C_INLINE XTSlugCollection XTAPI GetSlugCollection(boxid boxID,int16 slugType)
{
	XTSlugCollection slugCollection = INVAL_SLUGCOLLECTION;

	switch (slugType) {
	case PROJECTSLUG:
		slugCollection = XTGetProjectSlugCollection((ProjectID)boxID);
		break;

	case DOCSLUG:
		{
			DocID curDocID = INVALDOC;

			xtget_curdoc(&curDocID);
			slugCollection = XTGetDocSlugCollection(XTGetDocRefFromDocID(curDocID));
		}
		break;

	case PAGESLUG:
		{
			DocID curDocID = INVALDOC;

			xtget_curdoc(&curDocID);
			slugCollection = XTGetPageSlugCollection(XTGetDocRefFromDocID(curDocID),LoWord((int32) boxID));
		}
		break;
	
	case OBJECTSLUG:
	case SPREADSLUG:
		if (boxID == (boxid)0) {
			xtget_curbox(&boxID);
		}
		slugCollection = XTGetBoxSlugCollection(boxID);
		break;

	case CONTENTSLUG:
		if (boxID == (boxid)0) {
			xtget_curbox(&boxID);
		}
		slugCollection = XTGetContentSlugCollection(boxID);
		break;
	}
	return (slugCollection);
}

C_INLINE bool8 XTAPI hasslug(boxid boxID, int16 slugType, OSType slugID)
{
	bool8 hasSlug = FALSE;

	XTHasSlug(GetSlugCollection(boxID,slugType), slugID, &hasSlug);
	return (hasSlug);
}

C_INLINE Handle XTAPI getslug(boxid boxID, int16 slugType, OSType slugID)
{
	Handle slugHandle = NULL;
	
	XTGetSlug(GetSlugCollection(boxID,slugType), slugID, &slugHandle);
	return (slugHandle);
}

C_INLINE int32 XTAPI getslugdata(boxid boxID, int16 slugType, OSType slugID,
		uchar *ptr, int32 count)
{
	int32 bytesRead = count;
	APIERR err = XTGetSlugData(GetSlugCollection(boxID,slugType), slugID, ptr, &bytesRead);

	return(err == ERR_SUCCESS ? bytesRead : -1);
}

C_INLINE int32 XTAPI setslug(boxid boxID, int16 slugType, OSType slugID,
		Handle slugHandle)
{
	APIERR err = XTSetSlug(GetSlugCollection(boxID,slugType), slugID, slugHandle);

	return (XTAPIERRToXTError(err));
}

C_INLINE int32 XTAPI setslugdata(boxid boxID, int16 slugType, OSType slugID,
		uchar *slugData, int32 slugDataLen)
{
	APIERR err = XTSetSlugData(GetSlugCollection(boxID,slugType), slugID, slugData,
			slugDataLen);

	return (XTAPIERRToXTError(err));
}

C_INLINE void XTAPI pagept2sprd(pagenumdata *pageDataP, Fixed *hValueP,
		Fixed *vValueP)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTPageToSpreadPoint(XTGetDocRefFromDocID(curDocID), pageDataP, hValueP,
			vValueP);
}

C_INLINE void XTAPI pagefrect2sprd(pagenumdata *pageDataP,fixedrect *fixedRectP)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTPageToSpreadFixedRect(XTGetDocRefFromDocID(curDocID), pageDataP,
			fixedRectP);
}

C_INLINE void XTAPI sprdfrect2page(fixedrect *fixedRectP)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTSpreadToPageFixedRect(XTGetDocRefFromDocID(curDocID), fixedRectP);
}

C_INLINE void XTAPI rulersprd2pg(Fixed *hValueP,Fixed *vValueP)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTSpreadToPageRuler(XTGetDocRefFromDocID(curDocID), hValueP, vValueP);
}

C_INLINE xtboxptr XTAPI newxtbox(int32 content, int16 shape,
		bool16 userdefaults)
{
	DocRef docRef = INVAL_DOCREF;
	xtboxptr xtNewBoxP = NULL;

	if (!userdefaults) {
		docRef = GLOBAL_DOCREF;
	}
	else {
		DocID curDocID = INVALDOC;

		xtget_curdoc(&curDocID);
		docRef = XTGetDocRefFromDocID(curDocID);
	}
	XTNewXTBox(docRef, content, shape, &xtNewBoxP);

	return (xtNewBoxP);
}

C_INLINE void XTAPI getfullmastername(int16 masterSpreadID, uchar *name)
{
	assert( FALSE && "getfullmastername" );
	return;
}

C_INLINE int16 XTAPI xtDeleteMasterPage(int16 pageIndex)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	return ((int16)(XTDeleteMasterPage(XTGetDocRefFromDocID(curDocID),
			pageIndex)));
}

C_INLINE OSErr XTAPI savetextpict(pathhandle pathhndl, int32 flags)
{
	assert( FALSE && "savetextpict" );
	return( (OSErr) -1 );
}  

C_INLINE void XTAPI redrawpic()
{
	boxid curboxID = 0;

	xtget_curbox(&curboxID);
	if ((curboxID != 0) && isapicture(curboxID)) {
		invalbox(curboxID, CONTONLY, FALSE);
	}
	XTReDrawPicture(NULL);
}

C_INLINE DocID XTAPI GetDocIDFromBox(boxid theBox)
{
	return XTGetDocIDFromDocRef(XTGetDocRefFromBox(theBox));
}

C_INLINE Handle XTAPI sprd2pages(int16 spreadNum)
{
	DocID curDocID = INVALDOC;
	Handle pageSequenceH = NULL;

	xtget_curdoc(&curDocID);
	XTSpreadToPages(XTGetDocRefFromDocID(curDocID), spreadNum, &pageSequenceH);

	return (pageSequenceH);
}

C_INLINE XTError XTAPI xtget_curdefspread(int16 *defaultSpreadID)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	
	return (XTAPIERRToXTError(XTGetCurDefaultSpread(XTGetDocRefFromDocID(curDocID),
			defaultSpreadID)));
}

C_INLINE XTError XTAPI xtset_curdefspread(int16 defaultSpreadID)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	
	return (XTAPIERRToXTError(XTSetCurDefaultSpread(XTGetDocRefFromDocID(curDocID),
			defaultSpreadID)));
}

C_INLINE boxid XTAPI xtinstallbox(xtboxptr xtboxdata,int16 spreadid,boxid theboxid,
		bool16 doUpdates)
{
	DocID curDocID = INVALDOC;
	boxid newBoxID = 0;
	
	xtget_curdoc(&curDocID);
	XTInstallBox(XTGetDocRefFromDocID(curDocID), xtboxdata, spreadid, theboxid, doUpdates,
			&newBoxID);

	return (newBoxID);
}

C_INLINE void XTAPI AdjustMaxObjectScroll(bool8 redraw)
{
	DocID curDocID = INVALDOC;
	
	xtget_curdoc(&curDocID);
	XTAdjustMaxObjectScroll(XTGetDocRefFromDocID(curDocID), redraw);
}


C_INLINE void XTAPI getmastername(int16 masterPageID, uchar *name)
{
	assert( FALSE && "getmastername" );
	return;
}

C_INLINE bool8 XTAPI gotomaster(int16 masterPage)
{
	DocID curDocID = INVALDOC;
	bool8 retVal = FALSE;

	xtget_curdoc(&curDocID);
	
	if (XTGotoMaster(XTGetDocRefFromDocID(curDocID), masterPage) == ERR_SUCCESS) {
		retVal = TRUE;
	}
	return (retVal);
}

C_INLINE bool8 XTAPI offpage(boxid theBox, int16 page)
{
	bool8 offPage = TRUE;

	XTIsBoxOffPage(theBox, page, &offPage);

	return (offPage);
}

C_INLINE void XTAPI scrollcontents(int16 deltaX, int32 deltaY)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTScrollContents(XTGetDocRefFromDocID(curDocID), deltaX, deltaY);
}

C_INLINE bool8 XTAPI interdoccopypages(DocID srcDocID, DocID dstDocID,
		Handle pageH, int16 dstPage, int16 options)
{
	bool8 retVal = FALSE;
	APIERR err = XTInterDocCopyPages(XTGetDocRefFromDocID(srcDocID),
			XTGetDocRefFromDocID(dstDocID), pageH, dstPage, options);

	if (err == ERR_SUCCESS) {
		retVal = TRUE;
	}
	return (retVal);
}    
           
C_INLINE bool8 XTAPI rightmaster(int16 page)
{
	DocID curDocID = INVALDOC;
	bool8 retVal = FALSE;

	xtget_curdoc(&curDocID);
	XTHasRightMaster(XTGetDocRefFromDocID(curDocID), page, &retVal);
	return (retVal);
}

C_INLINE int16 XTAPI nummastersprds(void)
{
	DocID curDocID = INVALDOC;
	int16 numSpreads = 0;

	xtget_curdoc(&curDocID);
	XTNumMasterSpreads(XTGetDocRefFromDocID(curDocID), &numSpreads);
	return (numSpreads);
}

C_INLINE int16 XTAPI nummasterpages(void)
{
	DocID curDocID = INVALDOC;
	int16 numPages = 0;

	xtget_curdoc(&curDocID);
	XTNumMasterPages(XTGetDocRefFromDocID(curDocID), &numPages);
	return (numPages);
}

C_INLINE bool8 XTAPI createmaster(bool16 doublesided, bool16 createdef,
		uchar *name)
{
	assert( FALSE && "createmaster" );
	return( FALSE );
}

C_INLINE bool8 XTAPI applymaster2pages(int16 newmaster, int16 from, int16 to)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	if (XTApplyMasterToPages(XTGetDocRefFromDocID(curDocID), newmaster, from,
			to) == ERR_SUCCESS) {
		return (TRUE);
	}
	return (FALSE);
}

C_INLINE int16 XTAPI getpageseq(int16 priority, uchar *pageNumBufferP)
{
	assert ( FALSE && "getpageseq" );
	return( -1 );
}

C_INLINE void XTAPI xegetallattribs(void)
{
	boxid curBoxID = 0;

	xtget_curbox(&curBoxID);
	if (istextbox(curBoxID)) {
		xehandle xeH = NULL;

		xegetinfo(curBoxID,&xeH,NULL,NULL,NULL);
		XEGetAllAttribs(xeH);
	}
}

#undef xesetcalc
C_INLINE void XTAPI xesetcalc(textboxid textBoxID,int32 c,int32 count,
		int32 pend,int16 opcode)
{
	if (istextbox(textBoxID)) {
		xehandle xeH = NULL;

		xegetinfo(textBoxID,&xeH,NULL,NULL,NULL);
		XESetCalc(xeH,c,count,pend,opcode);
	}
}

#undef xesetsel
C_INLINE void XTAPI xesetsel(int32 start,int32 end,bool16 drawsel)
{
	boxid curBoxID = 0;

	xtget_curbox(&curBoxID);
	if (istextbox(curBoxID)) {
		xehandle xeH = NULL;

		xegetinfo(curBoxID,&xeH,NULL,NULL,NULL);
		XESetSel(xeH,start,end,drawsel);
	}
}

#undef xesetattrib
C_INLINE void XTAPI xesetattrib(int16 which,Fixed what)
{
	boxid curBoxID = 0;

	xtget_curbox(&curBoxID);
	if (istextbox(curBoxID)) {
		xehandle xeH = NULL;

		TextAttribParamType t;
		t.fixedValue = what;
		xegetinfo(curBoxID,&xeH,NULL,NULL,NULL);
		XESetAttrib(xeH,which,t);
	}
}

#undef xtgetxetstuff
C_INLINE int16 XTAPI xtgetxetstuff(textboxid textBoxID,uint32 *tRecLen,trechandle *tRecH)
{
	if (istextbox(textBoxID)) {
		xehandle xeH = NULL;

		xegetinfo(textBoxID,&xeH,NULL,NULL,NULL);
		return XEGetTRec(xeH,tRecLen,tRecH);
	}
	return paramErr;
}

#undef xtgetxepstuff
C_INLINE int16 XTAPI xtgetxepstuff(textboxid textBoxID,uint32 *pRecLen,
		prechandle *pRecH)
{
	if (istextbox(textBoxID)) {
		xehandle xeH = NULL;

		xegetinfo(textBoxID,&xeH,NULL,NULL,NULL);
		return XEGetPRec(xeH,pRecLen,pRecH);
	}
	return paramErr;
}

C_INLINE int16 XTAPI GetXETStuff2(xehandle xeH, uint32 *tRecLen,
		trechandle *tRecH)
{
	int16 xeError = noErr;

	xeError = XEGetTRec(xeH,tRecLen,tRecH);
	return (xeError);
}

C_INLINE int16 XTAPI GetXEPStuff2(xehandle xeH, uint32 *pRecLen,
		prechandle *pRecH)
{
	int16 xeError = noErr;

	xeError = XEGetPRec(xeH,pRecLen,pRecH);
	return (xeError);
}

#if MACOS
C_INLINE int16 cstrcmp2(uchar *s1, uchar *s2)
{
	return (cstrcmp(s1, s2));
}
#endif

C_INLINE void XTAPI getpagerect(int16 pagenum, Rect *pagerect)
{
	assert( FALSE && "getpagerect" );
	return;
}

C_INLINE bool8 XTAPI xtinsertpages(int16 page, int16 toadd, int16 options,
		textboxid frombox, int16 mastersprdnum)
{
	DocID curDocID = INVALDOC;
	
	xtget_curdoc(&curDocID);
	if (XTInsertPages(XTGetDocRefFromDocID(curDocID), page, toadd, options,
			frombox, mastersprdnum) == ERR_SUCCESS) {
		return (TRUE);
	}
	return (FALSE);
}

#undef insertpages
C_INLINE bool8 XTAPI insertpages(int16 page, int16 toadd, int16 options,
		textboxid frombox)
{
	DocID curDocID = INVALDOC;
	int16 mastersprdnum;

	xtget_curdoc(&curDocID);
	xtget_curdefspread(&mastersprdnum);
	if (XTInsertPages(XTGetDocRefFromDocID(curDocID), page, toadd, options, frombox,
			mastersprdnum) == ERR_SUCCESS) {
		return (TRUE);
	}
	return (FALSE);
}

C_INLINE bool8 XTAPI gotopage(int16 p)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	if (XTGotoPage(XTGetDocRefFromDocID(curDocID), p) == ERR_SUCCESS ) {
		return (TRUE);
	}
	return (FALSE);
}

C_INLINE bool8 XTAPI setsectionstart(int16 pageNum, int16 secPageNum, int16 pagefmt,
		uchar *prefixstr)
{
	assert( FALSE && "setsectionstart" );
	return( FALSE );
}

C_INLINE void sendbehind(bool16 oneLayer)
{
	boxid curBoxID = 0;

	xtget_curbox(&curBoxID);
	XTSendBehind(curBoxID, (bool8)oneLayer);
}

C_INLINE void bringforward(bool16 oneLayer)
{
	boxid curBoxID = 0;

	xtget_curbox(&curBoxID);
	XTBringForward(curBoxID, (bool8)oneLayer);
}

C_INLINE int16 scrolldoc(int16 stype,Point scrlpt,RgnHandle prevclip1,
		RgnHandle prevclip2,RgnHandle prevclip3)
{
	DocID curDocID = INVALDOC;
	bool8 haveScrolled = FALSE;

	xtget_curdoc(&curDocID);
	XTScrollDoc(XTGetDocRefFromDocID(curDocID), stype, scrlpt, prevclip1,
			prevclip2, prevclip3, &haveScrolled);

	return (haveScrolled);
}

C_INLINE void setzeroorigin()
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTSetZeroOrigin(XTGetDocRefFromDocID(curDocID));
}

C_INLINE void XTAPI setdocscale(Fixed scale, int16 redraw, Rect *dragRectP)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTSetDocScale(XTGetDocRefFromDocID(curDocID), scale, redraw, dragRectP);

}

C_INLINE XTError XTAPI GetMasterUpdateItemStatus(int16 itemDefID,
		int32 *curStatus, boxid cmpBox, bool8 *isLinked)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	if (XTGetMasterUpdateItemStatus(XTGetDocRefFromDocID(curDocID), itemDefID,
		curStatus, cmpBox, isLinked) == ERR_SUCCESS) {
		return (noErr);
	}
	return (XToperationfailed);
}

C_INLINE void XTAPI xeunlinkbox(textboxid textBoxID, int16 type, bool16 flag)
{
	XEUnLinkBox(textBoxID, type);
}

//||| jwadsworth 06-Mar-2003 - These should only be in the internal legacy header
#undef XTHasProjectSlug
C_INLINE APIERR XTAPI XTHasProjectSlug(ProjectID projectID, OSType slugID,
		bool8 *hasSlug)
{
	return XTHasSlug(XTGetProjectSlugCollection(projectID),
			slugID, hasSlug);
}
#undef XTHasDocumentSlug
C_INLINE APIERR XTAPI XTHasDocumentSlug(DocRef docRef, OSType slugID, bool8 *hasSlug)
{
	return XTHasSlug(XTGetDocSlugCollection(docRef),
			slugID, hasSlug);
}
#undef XTHasPageSlug
C_INLINE APIERR XTAPI XTHasPageSlug(DocRef docRef, int16 pageNumber, OSType slugID,
		bool8 *hasSlug)
{
	return XTHasSlug(XTGetPageSlugCollection(docRef,pageNumber),
			slugID, hasSlug);
}
#undef XTHasObjectSlug
C_INLINE APIERR XTAPI XTHasObjectSlug(boxid boxP, OSType slugID, bool8 *hasSlug)
{
	return XTHasSlug(XTGetBoxSlugCollection(boxP), slugID, hasSlug);
}
#undef XTHasContentSlug
C_INLINE APIERR XTAPI XTHasContentSlug(XTContentSlugTypeData slugTypeData,
		XTContentSlugDataType dataType, OSType slugID, bool8 *hasSlug)
{
	if (dataType == kContentSlugType_Box) {
		return XTHasSlug(XTGetContentSlugCollection(slugTypeData.boxID),
				slugID, hasSlug);
	}
	else {
		return XTHasSlug(XTGetStorySlugCollection(slugTypeData.xeH),
				slugID, hasSlug);
	}
}
#undef XTHasSpreadSlug
C_INLINE APIERR XTAPI XTHasSpreadSlug(spreadboxid spreadBoxP, OSType slugID,
		bool8 *hasSlug)
{
	return XTHasSlug(XTGetBoxSlugCollection(spreadBoxP), slugID, hasSlug);
}
#undef XTGetProjectSlug
C_INLINE APIERR XTAPI XTGetProjectSlug(ProjectID projectID,OSType slugID,
		Handle *slugHP)
{
	return XTGetSlug(XTGetProjectSlugCollection(projectID), slugID, slugHP);
}
#undef XTGetDocumentSlug
C_INLINE APIERR XTAPI XTGetDocumentSlug(DocRef docRef,OSType slugID,
		Handle *slugHP)
{
	return XTGetSlug(XTGetDocSlugCollection(docRef), slugID, slugHP);
}
#undef XTGetPageSlug
C_INLINE APIERR XTAPI XTGetPageSlug(DocRef docRef,
		int16 pageNumber,OSType slugID,Handle *slugHP)
{
	return XTGetSlug(XTGetPageSlugCollection(docRef,pageNumber), slugID, slugHP);
}
#undef XTGetObjectSlug
C_INLINE APIERR XTAPI XTGetObjectSlug(boxid boxP,OSType slugID,
		Handle *slugHP)
{
	return XTGetSlug(XTGetBoxSlugCollection(boxP), slugID, slugHP);
}
#undef XTGetContentSlug
C_INLINE APIERR XTAPI XTGetContentSlug(XTContentSlugTypeData slugTypeData,
		XTContentSlugDataType dataType,OSType slugID,Handle *slugHP)
{
	if (dataType == kContentSlugType_Box) {
		return XTGetSlug(XTGetContentSlugCollection(slugTypeData.boxID), 
				slugID, slugHP);
	}
	else {
		return XTGetSlug(XTGetStorySlugCollection(slugTypeData.xeH), 
				slugID, slugHP);
	}
}
#undef XTGetSpreadSlug
C_INLINE APIERR XTAPI XTGetSpreadSlug(spreadboxid boxP,
		OSType slugID,Handle *slugHP)
{
	return XTGetSlug(XTGetBoxSlugCollection(boxP), slugID, slugHP);
}
#undef XTGetProjectSlugData
C_INLINE APIERR XTAPI XTGetProjectSlugData(ProjectID projectID,OSType slugID,
		int32 dataLength,uchar *buffer,int32 *bytesReadP)
{
	APIERR err = XTGetSlugData(XTGetProjectSlugCollection(projectID), slugID, 
			buffer, &dataLength);

	if (bytesReadP != NULL) {
		*bytesReadP = dataLength;
	}
	return err;
}
#undef XTGetDocumentSlugData
C_INLINE APIERR XTAPI XTGetDocumentSlugData(DocRef docRef,OSType slugID,
		int32 dataLength,uchar *buffer,int32 *bytesReadP)
{
	APIERR err = XTGetSlugData(XTGetDocSlugCollection(docRef), slugID, 
			buffer, &dataLength);

	if (bytesReadP != NULL) {
		*bytesReadP = dataLength;
	}
	return err;
}
#undef XTGetPageSlugData
C_INLINE APIERR XTAPI XTGetPageSlugData(DocRef docRef,int16 pageNumber,
		OSType slugID,int32 dataLength,uchar *buffer,int32 *bytesReadP)
{
	APIERR err = XTGetSlugData(XTGetPageSlugCollection(docRef,pageNumber), slugID, 
			buffer, &dataLength);

	if (bytesReadP != NULL) {
		*bytesReadP = dataLength;
	}
	return err;
}
#undef XTGetObjectSlugData
C_INLINE APIERR XTAPI XTGetObjectSlugData(boxid boxP,OSType slugID,
		int32 dataLength,uchar *buffer,int32 *bytesReadP)
{
	APIERR err = XTGetSlugData(XTGetBoxSlugCollection(boxP), slugID, 
			buffer, &dataLength);

	if (bytesReadP != NULL) {
		*bytesReadP = dataLength;
	}
	return err;
}
#undef XTGetContentSlugData
C_INLINE APIERR XTAPI XTGetContentSlugData(XTContentSlugTypeData slugTypeData,
		XTContentSlugDataType dataType,OSType slugID,
		int32 dataLength,uchar *buffer,int32 *bytesReadP)
{
	APIERR err = ERR_FAILURE;
	
	if (dataType == kContentSlugType_Box) {
		err = XTGetSlugData(XTGetContentSlugCollection(slugTypeData.boxID), 
			slugID, buffer, &dataLength);
	}
	else {
		err = XTGetSlugData(XTGetStorySlugCollection(slugTypeData.xeH), 
			slugID, buffer, &dataLength);
	}

	if (bytesReadP != NULL) {
		*bytesReadP = dataLength;
	}
	return err;
}
#undef XTGetSpreadSlugData
C_INLINE APIERR XTAPI XTGetSpreadSlugData(spreadboxid spreadBoxP,
		OSType slugID,int32 dataLength,uchar *buffer,int32 *bytesReadP)
{
	APIERR err = XTGetSlugData(XTGetBoxSlugCollection(spreadBoxP), slugID, 
			buffer, &dataLength);

	if (bytesReadP != NULL) {
		*bytesReadP = dataLength;
	}
	return err;
}
#undef XTSetProjectSlug
C_INLINE APIERR XTAPI XTSetProjectSlug(ProjectID projectID,Handle projSlugH,
		OSType slugID)
{
	return XTSetSlug(XTGetProjectSlugCollection(projectID), slugID, projSlugH);
}
#undef XTSetDocumentSlug
C_INLINE APIERR XTAPI XTSetDocumentSlug(DocRef docRef,Handle docSlugH,
		OSType slugID)
{
	return XTSetSlug(XTGetDocSlugCollection(docRef), slugID, docSlugH);
}
#undef XTSetPageSlug
C_INLINE APIERR XTAPI XTSetPageSlug(DocRef docRef,int16 pageNumber,
		Handle pageSlugH,OSType slugID)
{
	return XTSetSlug(XTGetPageSlugCollection(docRef,pageNumber), slugID, pageSlugH);
}
#undef XTSetObjectSlug
C_INLINE APIERR XTAPI XTSetObjectSlug(boxid boxP,Handle objSlugH,
		OSType slugID)
{
	return XTSetSlug(XTGetBoxSlugCollection(boxP), slugID, objSlugH);
}
#undef XTSetContentSlug
C_INLINE APIERR XTAPI XTSetContentSlug(XTContentSlugTypeData slugTypeData,
		XTContentSlugDataType dataType,Handle contSlugH,OSType slugID)
{
	if (dataType == kContentSlugType_Box) {
		return XTSetSlug(XTGetContentSlugCollection(slugTypeData.boxID), slugID, contSlugH);
	}
	else {
		return XTSetSlug(XTGetStorySlugCollection(slugTypeData.xeH), slugID, contSlugH);
	}
}
#undef XTSetSpreadSlug
C_INLINE APIERR XTAPI XTSetSpreadSlug(spreadboxid spreadBoxP,Handle spreadSlugH,
		OSType slugID)
{
	return XTSetSlug(XTGetBoxSlugCollection(spreadBoxP), slugID, spreadSlugH);
}

#undef XTSetProjectSlugData
C_INLINE APIERR XTAPI XTSetProjectSlugData(ProjectID projectID,
		const uchar *slugDataP,int32 dataSize,OSType slugID)
{
	return XTSetSlugData(XTGetProjectSlugCollection(projectID), slugID, slugDataP, 
			dataSize);
}
#undef XTSetDocumentSlugData
C_INLINE APIERR XTAPI XTSetDocumentSlugData(DocRef docRef,const uchar *slugDataP,
		int32 dataSize,OSType slugID)
{
	return XTSetSlugData(XTGetDocSlugCollection(docRef), slugID, slugDataP, 
			dataSize);
}
#undef XTSetPageSlugData
C_INLINE APIERR XTAPI XTSetPageSlugData(DocRef docRef,int16 pageNumber,
		const uchar *slugDataP,int32 dataSize,OSType slugID)
{
	return XTSetSlugData(XTGetPageSlugCollection(docRef,pageNumber), slugID, 
			slugDataP, dataSize);
}
#undef XTSetObjectSlugData
C_INLINE APIERR XTAPI XTSetObjectSlugData(boxid boxP,const uchar *slugDataP,
		int32 dataSize,OSType slugID)
{
	return XTSetSlugData(XTGetBoxSlugCollection(boxP), slugID, slugDataP, 
			dataSize);
}
#undef XTSetContentSlugData
C_INLINE APIERR XTAPI XTSetContentSlugData(XTContentSlugTypeData slugTypeData,
		XTContentSlugDataType dataType,const uchar *slugDataP,
		int32 dataSize,OSType slugID)
{
	if (dataType == kContentSlugType_Box) {
		return XTSetSlugData(XTGetContentSlugCollection(slugTypeData.boxID), 
				slugID, slugDataP, dataSize);
	}
	else {
		return XTSetSlugData(XTGetStorySlugCollection(slugTypeData.xeH), 
				slugID, slugDataP, dataSize);
	}
}
#undef XTSetSpreadSlugData
C_INLINE APIERR XTAPI XTSetSpreadSlugData(spreadboxid spreadBoxP,
		const uchar *slugDataP,int32 dataSize,OSType slugID)
{
	return XTSetSlugData(XTGetBoxSlugCollection(spreadBoxP), slugID, slugDataP, 
			dataSize);
}
 
C_INLINE void dohandles(handleid *marP,bool16 pdrawhandles)
{
	/* This wrapper DOES NOT emulate the legacy dohandles exactly - we work on
	** curbox rather than the current document's curbox. The box passed within
	** handleid (if any) is ignored.
	*/
	boxid curBoxID = 0;

	xtget_curbox(&curBoxID);
	if (marP != NULL) {
		XTBoxHandleRec handleRec;

		zerodata(&handleRec, sizeof(XTBoxHandleRec));
		handleRec.sh = marP->sh;
		handleRec.conindex = marP->conindex;
		handleRec.ptindex = marP->ptindex;
		handleRec.t = marP->t;
		handleRec.dist = marP->dist;
		handleRec.verttype = marP->verttype;
		handleRec.handletype = marP->handletype;
		XTDoHandles(curBoxID, &handleRec, (bool8)pdrawhandles);
	}
	else {
		XTDoHandles(curBoxID, NULL, (bool8)pdrawhandles);
	}
}

C_INLINE boxid XTAPI replicateitems(DocID sourceDocID, boxid sourceBox,
		DocID targetDocID, boxid prevBox)
{
	boxid newBox = 0;

	XTReplicateItems(XTGetDocRefFromDocID(sourceDocID), sourceBox,
			XTGetDocRefFromDocID(targetDocID), prevBox, &newBox);

	return (newBox);
}

C_INLINE XTError XTAPI xtget_editpoly(int8 *editPolyP)
{
	APIERR err = ERR_SUCCESS;
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	err = XTGetEditPolyMode(XTGetDocRefFromDocID(curDocID), editPolyP);

	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtset_editpoly(int16 editPoly)
{
	APIERR err = ERR_SUCCESS;
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	err = XTSetEditPolyMode(XTGetDocRefFromDocID(curDocID), editPoly);

	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtget_invis(bool8 *invisP)
{
	APIERR err = ERR_SUCCESS;
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	err = XTGetInvisibleShown(XTGetDocRefFromDocID(curDocID), invisP);

	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtset_invis(bool8 invisShown)
{
	APIERR err = ERR_SUCCESS;
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	err = XTSetInvisibleShown(XTGetDocRefFromDocID(curDocID), invisShown);

	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtget_thumbview(bool8 *thumbViewP)
{
	APIERR err = ERR_SUCCESS;
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	err = XTGetThumbView(XTGetDocRefFromDocID(curDocID), thumbViewP);

	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtset_thumbview(bool8 thumbView)
{
	APIERR err = ERR_SUCCESS;
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	err = XTSetThumbView(XTGetDocRefFromDocID(curDocID), thumbView);

	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtget_masterpageshown(uint8 *masterPageShownP)
{
	APIERR err = ERR_SUCCESS;
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	err = XTGetMasterPageShown(XTGetDocRefFromDocID(curDocID), masterPageShownP);

	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtget_docspreadwidth(Fixed *spreadWidthP)
{
	APIERR err = ERR_SUCCESS;
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	err = XTGetDocSpreadWidth(XTGetDocRefFromDocID(curDocID), spreadWidthP);

	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtget_docspreadlength(Fixed *spreadLengthP)
{
	APIERR err = ERR_SUCCESS;
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	err = XTGetDocSpreadLength(XTGetDocRefFromDocID(curDocID), spreadLengthP);

	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtget_docpagewidth(Fixed *pageWidthP)
{
	APIERR err = ERR_SUCCESS;
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	err = XTGetDocPageWidth(XTGetDocRefFromDocID(curDocID), pageWidthP);

	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtget_docpagelength(Fixed *pageLengthP)
{
	APIERR err = ERR_SUCCESS;
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	err = XTGetDocPageLength(XTGetDocRefFromDocID(curDocID), pageLengthP);

	return (XTAPIERRToXTError(err));
}

C_INLINE void XTAPI movepages(int16 startpage, int16 endpage, int16 destpage,
		int16 options)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTMovePages(XTGetDocRefFromDocID(curDocID), startpage, endpage, destpage,
		options);
}
     
C_INLINE void XTAPI xedrawsel(textboxid boxP, int32 start, int32 end)
{
	bool8 drawEntireChain = FALSE;

	if (boxP == 0) {
		xtget_curbox(&boxP);
		drawEntireChain = TRUE;
	}
	if (boxP != 0 && istextbox(boxP)) {
		XTDrawSelection(boxP, start, end, drawEntireChain, kSelCalculate);
	}
}
   
C_INLINE XTError XTAPI xtget_docscale(Fixed *docScaleP)
{
	DocID curDocID = INVALDOC;
	APIERR err = ERR_SUCCESS;

	xtget_curdoc(&curDocID);
	err = XTGetDocScale(XTGetDocRefFromDocID(curDocID), docScaleP);
	
	return (XTAPIERRToXTError(err));
}

#undef xtset_docscale
C_INLINE XTError XTAPI xtset_docscale(Fixed scale)
{
	DocID curDocID = INVALDOC;
	APIERR err = ERR_SUCCESS;

	xtget_curdoc(&curDocID);
	XTSetDocScale(XTGetDocRefFromDocID(curDocID), scale, TRUE, NULL);
	
	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtget_annotationflag(uint8 *annotFlagP)
{
	DocID curDocID = INVALDOC;
	APIERR err = ERR_SUCCESS;

	xtget_curdoc(&curDocID);
	err = XTGetAnnotationFlag(XTGetDocRefFromDocID(curDocID), annotFlagP);
	
	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtget_agatesize(Fixed *agateSizeP)
{
	DocID curDocID = INVALDOC;
	APIERR err = ERR_SUCCESS;

	xtget_curdoc(&curDocID);
	err = XTGetAgateSize(XTGetDocRefFromDocID(curDocID), agateSizeP);
	
	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtset_agatesize(Fixed agateSize)
{
	DocID curDocID = INVALDOC;
	APIERR err = ERR_SUCCESS;

	xtget_curdoc(&curDocID);
	err = XTSetAgateSize(XTGetDocRefFromDocID(curDocID), agateSize);
	
	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtget_ruler(bool8 *rulerValueP)
{
	DocID curDocID = INVALDOC;
	APIERR err = ERR_SUCCESS;

	xtget_curdoc(&curDocID);
	err = XTGetRuler(XTGetDocRefFromDocID(curDocID), rulerValueP);
	
	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtset_ruler(bool8 rulerValue)
{
	DocID curDocID = INVALDOC;
	APIERR err = ERR_SUCCESS;

	xtget_curdoc(&curDocID);
	err = XTSetRuler(XTGetDocRefFromDocID(curDocID), rulerValue);
	
	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtget_snapenable(bool8 *snapToGuidesP)
{
	DocID curDocID = INVALDOC;
	APIERR err = ERR_SUCCESS;

	xtget_curdoc(&curDocID);
	err = XTGetSnapToGuides(XTGetDocRefFromDocID(curDocID), snapToGuidesP);
	
	return (XTAPIERRToXTError(err));
}

C_INLINE XTError XTAPI xtset_snapenable(bool8 snapToGuides)
{
	DocID curDocID = INVALDOC;
	APIERR err = ERR_SUCCESS;

	xtget_curdoc(&curDocID);
	err = XTSetSnapToGuides(XTGetDocRefFromDocID(curDocID), snapToGuides);
	
	return (XTAPIERRToXTError(err));
}

C_INLINE void XTAPI getdoclocation(uchar *name, int16 *vRefNum, int32 *parID, bool8 *savedState)
{
	if (name != NULL && vRefNum != NULL && parID != NULL && savedState != NULL) {
		DocRef docRef = INVAL_DOCREF;
		XFileInfoRef fileInfoRef;
		DocID curDocID = INVALDOC;
		int32 bufferSize = kMaxLongFileName;

		XTCreateEmptyXFileInfoRef(&fileInfoRef);
		xtget_curdoc(&curDocID);
		docRef = XTGetDocRefFromDocID(curDocID);
		XTGetDocLocation(docRef, fileInfoRef, savedState);
		XTGetVolumeIDFromXFileInfoRef(fileInfoRef, vRefNum);
		XTGetParentIDFromXFileInfoRef(fileInfoRef, parID);
		XTGetFileNameFromXFileInfoRef(fileInfoRef, name, &bufferSize);
		CSTRTOLOCAL(name);
		XTDisposeXFileInfoRef(fileInfoRef);
	}
}

C_INLINE Fixed XTAPI getparafmt(xehandle xeH, paragraph_attribute which,
		paraattrib *whatP)
{
	if (xeH != NULL) {
		Fixed retVal = 0;
		int32 start = 0;
		int32 end = 0;

		xegetstoryinfo(xeH, &start, &end, NULL);
		XTGetParaFmt(xeH, start, end, which, whatP, &retVal);

		return (retVal);
	}
	return (0);
}

C_INLINE void XTAPI setparafmt(xehandle xeH, paragraph_attribute which, 
		int32 what,	paraspec *paraSpecP)
{
	if (xeH != NULL) {
		int32 start = 0;
		int32 end = 0;

		ParaFmtParamType p;
		p.fixedValue = what;
		xegetstoryinfo(xeH, &start, &end, NULL);
		XTSetParaFmt(xeH, start, end, which, p, paraSpecP);
	}
}

C_INLINE XTError XTAPI xtget_slopwidth(Fixed *slopWidth)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	if (XTGetSlopWidth(XTGetDocRefFromDocID(curDocID), slopWidth)
		== ERR_SUCCESS) {
		return (noErr);
	}
	return (XTbaddataptr);
}

C_INLINE XTError XTAPI xtset_slopwidth(Fixed slopWidth)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	if (XTSetSlopWidth(XTGetDocRefFromDocID(curDocID), slopWidth)
		== ERR_SUCCESS) {
		return (noErr);
	}
	return (XTbaddataptr);
}

C_INLINE XTError XTAPI xtset_sloplength(Fixed slopLength)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	if (XTSetSlopLength(XTGetDocRefFromDocID(curDocID), slopLength)
		== ERR_SUCCESS) {
		return (noErr);
	}
	return (XTbaddataptr);
}

C_INLINE XTError XTAPI xtget_sloplength(Fixed *slopLength)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	if (XTGetSlopLength(XTGetDocRefFromDocID(curDocID), slopLength)
		== ERR_SUCCESS) {
		return (noErr);
	}
	return (XTbaddataptr);
}

C_INLINE XTError XTAPI xtget_doclocked(bool8 *docLocked)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	if (XTGetDocLocked(XTGetDocRefFromDocID(curDocID), docLocked)
		== ERR_SUCCESS) {
		return (noErr);
	}
	return (XTbaddataptr);
}

C_INLINE XTError XTAPI xtget_discarding(bool8 *discarding)
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	if (XTGetDiscarding(XTGetDocRefFromDocID(curDocID), discarding)
		== ERR_SUCCESS) {
		return (noErr);
	}
	return (XTbaddataptr);
}

#undef xtget_scrlxyval2
C_INLINE XTError XTAPI xtget_scrlxyval2(int16 *x,int32 *y) 
{
	assert( FALSE && "xtget_scrlxyval2" );
	return( (XTError) -1 );
}

#undef xtset_scrlxyval2
C_INLINE XTError XTAPI xtset_scrlxyval2(int16 x,int32 y)
{
	assert( FALSE && "xtset_scrlxyval2" );
	return( (XTError) -1 );
}

#undef xtget_scrlxymax2
C_INLINE XTError XTAPI xtget_scrlxymax2(int16 *x,int32 *y) 
{
	assert( FALSE && "xtget_scrlxymax2" );
	return( (XTError) -1 );
}

#undef xtset_scrlxymax2
C_INLINE XTError XTAPI xtset_scrlxymax2(int16 x,int32 y)
{
	assert( FALSE && "xtset_scrlxymax2" );
	return( (XTError) -1 );
}

C_INLINE OSErr XTAPI gettextpict(pathhandle pathH, int32 flags)
{
	assert( FALSE && "gettextpict" );
	return( (OSErr) -1 );
}

C_INLINE DocID XTAPI cbid()
{
	return (CBDOC);
}



C_INLINE XTError xtget_docpvars_402(printvars *pvars)
{
	DocRef docRef = INVAL_DOCREF;
	DocID curDocID = INVALDOC;
	APIERR err = ERR_SUCCESS;

	xtget_curdoc(&curDocID);
	docRef = (curDocID < 0) ? GLOBAL_DOCREF : XTGetDocRefFromDocID(curDocID);
	err = XTGetDocPrintVars(docRef, pvars);

	return (XTAPIERRToXTError(err));
}

#undef xtget_docpvars
C_INLINE XTError xtget_docpvars(printvars *pvars)
{
	DocRef docRef = INVAL_DOCREF;
	DocID curDocID = INVALDOC;
	APIERR err = ERR_SUCCESS;

	xtget_curdoc(&curDocID);
	docRef = (curDocID < 0) ? GLOBAL_DOCREF : XTGetDocRefFromDocID(curDocID);
	err = XTGetDocPrintVars(docRef, pvars);

	return (XTAPIERRToXTError(err));
}

C_INLINE XTError xtset_docpvars_402(printvars *pvars)
{
	DocRef docRef = INVAL_DOCREF;
	DocID curDocID = INVALDOC;
	APIERR err = ERR_SUCCESS;

	xtget_curdoc(&curDocID);
	docRef = (curDocID < 0) ? GLOBAL_DOCREF : XTGetDocRefFromDocID(curDocID);
	err = XTSetDocPrintVars(docRef, pvars);

	return (XTAPIERRToXTError(err));
}

#undef xtset_docpvars
C_INLINE XTError xtset_docpvars(printvars *pvars)
{
	DocRef docRef = INVAL_DOCREF;
	DocID curDocID = INVALDOC;
	APIERR err = ERR_SUCCESS;

	xtget_curdoc(&curDocID);
	docRef = (curDocID < 0) ? GLOBAL_DOCREF : XTGetDocRefFromDocID(curDocID);
	err = XTSetDocPrintVars(docRef, pvars);

	return (XTAPIERRToXTError(err));
}

C_INLINE XTError xtget_syspvars_402(printvars *pvars)
{
	return (XTAPIERRToXTError(XTGetSysPrintVars(pvars)));
}

#undef xtget_syspvars
C_INLINE XTError xtget_syspvars(printvars *pvars)
{
	return (XTAPIERRToXTError(XTGetSysPrintVars(pvars)));
}

C_INLINE XTError xtset_syspvars_402(printvars *pvars)
{
	return (XTAPIERRToXTError(XTSetSysPrintVars(pvars)));
}

#undef xtset_syspvars
C_INLINE XTError xtset_syspvars(printvars *pvars)
{
	return (XTAPIERRToXTError(XTSetSysPrintVars(pvars)));
}

#undef DuplicateFileHFS
C_INLINE int16 XTAPI DuplicateFileHFS(int16 srcVRefNum,int32 srcDirID,
		const uchar *srcName,int16 dstVRefNum,int32 dstDirID,
		const uchar *dstName,bool8 copyData,bool8 copyRes,bool8 keepDates)
{
	uchar srcFileName[kMaxLongFileName];
	uchar destFileName[kMaxLongFileName];
	XFileInfoRef srcFileInfoRef;
	XFileInfoRef destFileInfoRef;
	APIERR xFileErr = ERR_SUCCESS;
	int16 error = noErr;

#if WINOS
	strcpy_s((char *)srcFileName, kMaxLongFileName, (const char *)srcName);
	strcpy_s((char *)destFileName, kMaxLongFileName, (const char *)dstName);
#else // if WINOS
	LOCALTOCSTRCPY(srcFileName, srcName);
	LOCALTOCSTRCPY(destFileName, dstName);
#endif // if WINOS
	// convert HFS information into a sorce XFileInfoRef.
	xFileErr = XTCreateXFileInfoRef(srcVRefNum, srcDirID, srcFileName, 0, FALSE,
			FALSE, &srcFileInfoRef);
	if (xFileErr != ERR_SUCCESS) {
		error = memFullErr;
		goto cleanup;
	}

	// convert HFS information into a destination XFileInfoRef.
	xFileErr = XTCreateXFileInfoRef(dstVRefNum, dstDirID, destFileName, 0, FALSE,
			FALSE, &destFileInfoRef);
	if (xFileErr != ERR_SUCCESS) {
		error = memFullErr;
		goto cleanup;
	}

	// call bottleneck.
	xFileErr = XTDuplicateFile(srcFileInfoRef, destFileInfoRef, copyData,
			copyRes, keepDates, kShowDialog);
	error = XTAPIERRToOSErr(xFileErr);

cleanup:
	XTDisposeXFileInfoRef(srcFileInfoRef);
	XTDisposeXFileInfoRef(destFileInfoRef);

	return (error);
} // DuplicateFileHFS

C_INLINE int16 XTAPI DuplicateFileFSp(const FSSpec *srcFSSpecP,
		const FSSpec *dstFSSpecP,bool8 copydata,bool8 copyres,bool8 keepdates)
{
	XFileInfoRef srcFileInfoRef;
	XFileInfoRef destFileInfoRef;
	APIERR xFileErr = ERR_SUCCESS;
	int16 error = noErr;

	// convert HFS information into a sorce XFileInfoRef.
	xFileErr = XTCreateEmptyXFileInfoRef(&srcFileInfoRef);
	if (xFileErr != ERR_SUCCESS) {
		error = memFullErr;
		goto cleanup;
	}

	// convert HFS information into a destination XFileInfoRef.
	xFileErr = XTCreateEmptyXFileInfoRef(&destFileInfoRef);
	if (xFileErr != ERR_SUCCESS) {
		error = memFullErr;
		goto cleanup;
	}

	xFileErr = XTSetFSSpecInXFileInfoRef(srcFileInfoRef, srcFSSpecP);
	if (xFileErr != ERR_SUCCESS) {
		error = XTAPIERRToOSErr(xFileErr);
		goto cleanup;
	}

	xFileErr = XTSetFSSpecInXFileInfoRef(destFileInfoRef, dstFSSpecP);
	if (xFileErr != ERR_SUCCESS) {
		error = XTAPIERRToOSErr(xFileErr);
		goto cleanup;
	}

	// call bottleneck.
	xFileErr = XTDuplicateFile(srcFileInfoRef, destFileInfoRef, copydata,
			copyres, keepdates, kShowDialog);
	error = XTAPIERRToOSErr(xFileErr);

cleanup:
	XTDisposeXFileInfoRef(srcFileInfoRef);
	XTDisposeXFileInfoRef(destFileInfoRef);

	return error;
} // DuplicateFileHFS

C_INLINE bool8 XTAPI GetPictStatusStr(int16 thepicstatus, uchar *statstr,bool8 moreinfo)
{
	assert( FALSE && "GetPictStatusStr" );
	return( FALSE );
}

C_INLINE APIERR XTAPI GetPictDisplayPageNameStr(picboxid picBoxPtr, int16 pageNum,
		uchar *pageName)
{
	QXStringRef pageNameQXStr = NULL;
	int32 uniCharCount = 0;
	bool8 isConverted = FALSE;
	int32 charCount = 0;
	UniChar *uniCharBuffer = NULL;
	APIERR apiErr = ERR_SUCCESS;
	
	apiErr = XTGetBoxPageNameStr(picBoxPtr, &pageNameQXStr);
	if (apiErr != ERR_SUCCESS) {
		goto cleanup;
	}

	apiErr = QXStringGetLength(pageNameQXStr, &uniCharCount);
	if (apiErr != ERR_SUCCESS) {
		goto cleanup;
	}

	apiErr = QXStringGetBuffer(pageNameQXStr, &uniCharBuffer);
	if (apiErr != ERR_SUCCESS) {
		goto cleanup;
	}

	charCount = uniCharCount + 1;
	apiErr = XUFromUnicodePreAlloc(uniCharCount, (const UniChar *)uniCharBuffer,
			&charCount, pageName, &isConverted, kUTextEncodingPlatform);
	if (apiErr != ERR_SUCCESS) {
		goto cleanup;
	}

	CSTRTOLOCAL(pageName);

cleanup:
	if (pageNameQXStr != NULL) {
		QXStringDestroy(pageNameQXStr);
	}

	return (apiErr);
}

#if WINOS
C_INLINE bool8 XTAPI linedraw(popupinforec *popupinfo, popuphandlerinfoptr phiptr)
{
	return (XTPopupLineDraw(popupinfo,phiptr));
}
#else
C_INLINE bool8 XTAPI linedraw(popupinforec *popupinfo,int16 *leftshift,Rect *box)
{
	return (XTPopupLineDraw(popupinfo,leftshift,box));
}
#endif

#if MACOS
C_INLINE RgnHandle XTAPI gettextflowrgn(boxid thebox)
{
	return(XTGetTextFlowRgn(thebox));
}

C_INLINE void scrollxticon(uchar *name, int16 iconid)
{
	XTScrollXTIcon(name,iconid);
}

C_INLINE void getscaledcontrgn(boxid bptr,RgnHandle rgnhndl)
{
	XTGetScaledContRgn(bptr,rgnhndl);
}
#endif // #if MACOS

C_INLINE colordbhandle XTAPI color_makedbcopy(colordbhandle dbh)
{
	colordbhandle dbCopy = NULL;

	if (dbh == NULL) {
		dbh = color_getcurdocdb();
	}
	XTColorMakeDBCopy(dbh, TRUE, &dbCopy);
	return (dbCopy);
}

#undef curdocname
C_INLINE const uchar *XTAPI curdocname(void) 
{
	static Str255 name;
	DocID curDocID = INVALDOC;

	QXStringRef nameRef = NULL;
	int32 uniCharCount = 0;
	bool8 isConverted = FALSE;
	int32 charCount = 0;
	UniChar *uniCharBuffer = NULL;

	xtget_curdoc(&curDocID);
	XTGetLayoutSpaceName(XTGetDocRefFromDocID(curDocID), &nameRef);

	QXStringGetLength(nameRef, &uniCharCount);
	QXStringGetBuffer(nameRef, &uniCharBuffer);

	charCount = uniCharCount + 1;

	XUFromUnicodePreAlloc(uniCharCount, (const UniChar *)uniCharBuffer, 
			&charCount, (uchar *)name, &isConverted, kUTextEncodingPlatform);
	CSTRTOLOCAL(name);
	QXStringDestroy(nameRef);

	return (name);
}

#undef alittledirty
C_INLINE void XTAPI alittledirty(void) 
{
	DocID curDocID = INVALDOC;

	xtget_curdoc(&curDocID);
	XTSetDocumentDirtyness(XTGetDocRefFromDocID(curDocID),ALITTLEDIRTY);
}

#undef setdoc
C_INLINE xehandle XTAPI setdoc(WindowPtr windowP,int16 work)
{
	DocRef docRef = INVAL_DOCREF;
		
	XTGetActiveLayoutSpaceFromWindow(windowP,0, &docRef);
	if (docRef != INVAL_DOCREF) {
		DocID docID = XTGetDocIDFromDocRef(docRef);

		if (docID != INVALDOC) {
			boxid curBoxID = 0;

			setcurdoc(docID&~(work ? 0 : SKIPWORK));
			xtget_curbox(&curBoxID);
			if (curBoxID != 0) {
				curboxsprdorigin();
				if (istextbox(curBoxID)) {
					xehandle xeH = NULL;

					xegetinfo(curBoxID,&xeH,NULL,NULL,NULL);
					return xeH;
				}
			}
			return NULL;
		}
	}
	setcurdoc(-1);
	setzeroorigin();
	SetPortWindowPort(windowP);

	return NULL;
}

#undef isalib
C_INLINE bool8 XTAPI isalib(DocID docid) 
{
	bool8 library = FALSE;

	XTIsLibraryDoc(XTGetDocRefFromDocID(docid),&library);

	return (library);
}

#undef OEPutChars
C_INLINE APIERR XTAPI OEPutChars(xehandle xeH,int32 c,int32 ccount,uchar *buff,
		const globaltxtvars *txtAttPxehandle) 
{
	return (XEPutChars(xeH,c,ccount,buff,txtAttPxehandle));
}

#undef OEPutChar
C_INLINE APIERR XTAPI OEPutChar(xehandle xeH,int32 c,uint16 ch) 
{
	return (XEPutChar(xeH,c,ch));
}

#undef XTXEActivate
C_INLINE APIERR XTAPI XTXEActivate(textboxid boxID) 
{
	return (XEActivate(boxID));
}

#undef XTGetStoryByUniqueID
C_INLINE APIERR XTAPI XTGetStoryByUniqueID(StoryViewUID storyID,
		xecollection collection, xehandle *story) 
{	
	return (XTFindStoryView(storyID,collection,story));
}


#undef xedelchars
C_INLINE APIERR XTAPI xedelchars(boxid curBoxID,int32 start,int32 end)
{
	APIERR err = ERR_INVALPARAM1;

	if (istextbox(curBoxID)) {
		xehandle xeH = NULL;

		xegetinfo(curBoxID,&xeH,NULL,NULL,NULL);
		err = XEDelChars(xeH,start,end);
	}
	
	return err;
	
}

#undef GetPrevVisCharOffset
C_INLINE int32 XTAPI GetPrevVisCharOffset(xehandle xeH,int32 offset) 
{
	return (XEGetPrevVisCharOffset(xeH,offset));
}

#undef GetNextVisCharOffset
C_INLINE int32 XTAPI GetNextVisCharOffset(xehandle xeH,int32 offset) 
{
	return (XEGetNextVisCharOffset(xeH,offset));
}

#undef OEGetAllAttribs
C_INLINE void XTAPI OEGetAllAttribs(xehandle xeH) 
{
	XEGetAllAttribs(xeH);
}

#undef OESetAttrib
C_INLINE void XTAPI OESetAttrib(xehandle xeH,int16 which,Fixed what) 
{
	TextAttribParamType t;
	t.fixedValue = what;
	XESetAttrib(xeH,which,t);
}

#undef storydirty
C_INLINE bool8 XTAPI storydirty(void) 
{
	boxid curBoxID = 0;
	
	xtget_curbox(&curBoxID);
	if (istextbox(curBoxID)) {
		xehandle xeH = NULL;

		xegetinfo(curBoxID,&xeH,NULL,NULL,NULL);
		return (XEGetStoryDirty(xeH));
	}
	else return (FALSE);
}

#undef StoryDirty
C_INLINE bool8 XTAPI StoryDirty(xehandle xeH) 
{
	return (XEGetStoryDirty(xeH));
}

#undef XEGetStoryAttb
C_INLINE bool8 XTAPI XEGetStoryAttb(xehandle xeH,int16 which) 
{
	Fixed result = 0;
	XEGetStoryAttrib(xeH,which,&result);
	return( (bool8)result );
}

#undef XESetStoryAttb
C_INLINE bool8 XTAPI XESetStoryAttb(xehandle xeH,int16 which,bool16 newState) 
{
	return (XESetStoryAttrib(xeH, which,newState));
}

#undef xegetstoryattb
C_INLINE bool8 XTAPI xegetstoryattb(textboxid boxID,int16 which) 
{
	xehandle xeH = NULL;

	xegetinfo(boxID,&xeH,NULL,NULL,NULL);
	Fixed result = 0;
	XEGetStoryAttrib(xeH,which,&result);
	return( (bool8) result );
}

#undef xesetstoryattb
C_INLINE bool8 XTAPI xesetstoryattb(textboxid boxID,int16 which, bool16 newstate) 
{
	xehandle xeH = NULL;

	xegetinfo(boxID,&xeH,NULL,NULL,NULL);
	return (XESetStoryAttrib(xeH,which,newstate));
}

#undef xegetstorylock
C_INLINE bool8 XTAPI xegetstorylock(textboxid boxID) 
{
	xehandle xeH = NULL;

	xegetinfo(boxID,&xeH,NULL,NULL,NULL);
	return (XEGetStoryLock(xeH));
}

#undef xesetstorylock
C_INLINE bool8 XTAPI xesetstorylock(textboxid boxID, bool16 newstate) 
{
	xehandle xeH = NULL;

	xegetinfo(boxID,&xeH,NULL,NULL,NULL);
	return (XESetStoryLock(xeH,newstate));
}

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

#endif /* _XP_XTLEGACYE6_H_ */
