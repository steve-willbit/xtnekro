/****************************************************************************
**
** XP_XTLegacyE7.h
**
** This file translates some pre XPress6 XTension functions to XPress7
** XTensions.
**
** Copyright (c) 2004 - 2006 Quark Technology Partnership.
** All Rights Reserved.
**
**
\****************************************************************************/

#ifndef _XP_XTLEGACYINT7_H_
#define _XP_XTLEGACYINT7_H_

EXTERN_XT_GLOBALS
EXTERN_QXSTRING_GLOBALS
EXTERN_XUNICODE_GLOBALS

#if defined(__cplusplus) || defined(inline)
#define C_INLINE inline
#else
#define C_INLINE static
#endif

#define ERRNOBOXNAME 1
#define ERRNOTOWNER 2
#define ERRBADPROPERTY 3
#define ERRBOXLOCKED 4
#define BOXNAMELOCK 1						/* Property code: Lock name flag */

struct charstylerec {								/* Character Style Sheet */
	styleinfo style;
	txtattrib t;								/* Text attributes of style */
};
typedef struct charstylerec charstylerec;
typedef charstylerec** cstylehandle;

typedef struct {
	uchar listname[MAXLISTNAMELEN];
	int16 id;
	listdatarec ldata;
} listsrec,*listsrecptr,**listshandle;


#ifndef __GNUC__
#undef setmode
C_INLINE void setmode(TOOLMODE newmode)
{
	XTSetToolMode(newmode);
}

#undef getmode
C_INLINE TOOLMODE getmode(void)
{
	return XTGetToolMode();
}
#endif

#undef skiphidden
C_INLINE int32 XTAPI skiphidden(xehandle xeH, int32 offset,
		int16 direction, int16 skiptypes)
{
	int32 newOffset = BADCHARPOS;
	
	XTHiddenTextSkip(xeH, offset, direction, skiptypes, &newOffset);
	
	return newOffset;
}

#undef skipback1char
C_INLINE int32 XTAPI skipback1char(xehandle xeH, int32 offset)
{
	int32 newOffset = BADCHARPOS;
	
	XTHiddenTextSkipChar(xeH, offset, BACKWARD, SKIPTO_HIDDEN_OR_VISIBLE, &newOffset);

	return newOffset;
}	

#undef skip1char
C_INLINE int32 XTAPI skip1char(xehandle xeH,int32 offset)
{
	int32 newOffset = BADCHARPOS;
	
	XTHiddenTextSkipChar(xeH, offset, FORWARD, SKIPTO_HIDDEN_OR_VISIBLE, &newOffset);
	
	return newOffset;
}

#undef findopcode
C_INLINE bool8 XTAPI findopcode(xehandle xeH, int32 *cptr, int16 direction, 
		int32 searchop, int32 *opfound, int32 searchend)
{
	bool8 found = FALSE;
	
	XTHiddenTextFindMarker(xeH, cptr, searchend, searchop, opfound, direction,
			FALSE, &found);

	return found;
}

#undef specialfindopcode
C_INLINE bool8 XTAPI specialfindopcode(xehandle xeH, int32 *offsetP, int16 direction,
		int32 searchOp, int32 *opFound, int32 searchEnd)
{
	bool8 found = FALSE;
	
	XTHiddenTextFindMarker(xeH, offsetP, searchEnd, searchOp, opFound, 
			direction, TRUE, &found);

	return found;
}

#undef XEGetPrevVisCharOffset
C_INLINE int32 XTAPI XEGetPrevVisCharOffset(xehandle xeH, int32 startOffset)
{
	int32 newOffset = BADCHARPOS;
	
	XTHiddenTextSkipChar(xeH, startOffset, BACKWARD, SKIPTO_VISIBLE_ONLY, &newOffset);

	return newOffset;
}

#undef XEGetNextVisCharOffset
C_INLINE int32 XTAPI XEGetNextVisCharOffset(xehandle xeH, int32 startOffset)
{
	int32 newOffset = BADCHARPOS;
	
	XTHiddenTextSkipChar(xeH, startOffset, FORWARD, SKIPTO_VISIBLE_ONLY, &newOffset);

	return newOffset;
}

#undef h_getchar
// define below

#undef inserthidden
C_INLINE void XTAPI inserthidden(xehandle xeH, int32 offset, int32 opCode,
		int32 ignoredLen, uchar *ignoredData, txtattrib *ignoredAttribP)
{
	XTHiddenTextInsert(xeH, offset, opCode, ignoredLen, ignoredData, 
			(txtattrib*)ignoredAttribP);
}

#undef h_delchars
C_INLINE bool8 XTAPI h_delchars(xehandle xeH, int32 start, int32 end,
	uint16 deleteFlags, bool16 replaceHidden)
{
	bool8 success = FALSE;
	
	XTHiddenTextDeleteContent(xeH, start, end, deleteFlags, (bool8)replaceHidden, &success);
	
	return success ? FALSE : TRUE;
}

#undef cleanselrange
C_INLINE void XTAPI cleanselrange(xehandle xeH, int32 *start, int32 *end, 
		int32 opCode)
{
	XTHiddenTextDelete(xeH, start, end, opCode);
}

#undef xtget_docflexspacesize
C_INLINE XTError XTAPI xtget_docflexspacesize(Fixed *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocFlexSpaceSize(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docflexspacesize
C_INLINE XTError XTAPI xtset_docflexspacesize(Fixed xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocFlexSpaceSize(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_doccicerosize
C_INLINE XTError XTAPI xtget_doccicerosize(Fixed *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocCiceroSize(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_doccicerosize
C_INLINE XTError XTAPI xtset_doccicerosize(Fixed xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocCiceroSize(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docautoleading
C_INLINE XTError XTAPI xtget_docautoleading(Fixed *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocAutoLeading(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docautoleading
C_INLINE XTError XTAPI xtset_docautoleading(Fixed xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocAutoLeading(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docallcapsaccents
C_INLINE XTError XTAPI xtget_docallcapsaccents(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocAllCapsAccents(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docallcapsaccents
C_INLINE XTError XTAPI xtset_docallcapsaccents(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocAllCapsAccents(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docgridinc
C_INLINE XTError XTAPI xtget_docgridinc(Fixed *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocGridInc(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docgridinc
C_INLINE XTError XTAPI xtset_docgridinc(Fixed xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocGridInc(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docgridoffset
C_INLINE XTError XTAPI xtget_docgridoffset(Fixed *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocGridOffset(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docgridoffset
C_INLINE XTError XTAPI xtset_docgridoffset(Fixed xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocGridOffset(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docpercent
C_INLINE XTError XTAPI xtget_docpercent(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocPercent(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docpercent
C_INLINE XTError XTAPI xtset_docpercent(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocPercent(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docpointsize
C_INLINE XTError XTAPI xtget_docpointsize(Fixed *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocPointSize(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docpointsize
C_INLINE XTError XTAPI xtset_docpointsize(Fixed xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocPointSize(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docsmallcapshvscale
C_INLINE XTError XTAPI xtget_docsmallcapshvscale(Fixed *hscale, Fixed *vscale)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocSmallCapsHVScale(
				XTGetDocRefFromDocID(docID), hscale, vscale)));
}

#undef xtset_docsmallcapshvscale
C_INLINE XTError XTAPI xtset_docsmallcapshvscale(Fixed hscale, Fixed vscale)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocSmallCapsHVScale(
				XTGetDocRefFromDocID(docID), hscale, vscale)));
}

#undef xtget_docsuboffsethvscale
C_INLINE XTError XTAPI xtget_docsuboffsethvscale(Fixed *offset, Fixed *hscale,
		Fixed *vscale)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocSubOffsetHVScale(
				XTGetDocRefFromDocID(docID), offset, hscale, vscale)));
}

#undef xtset_docsuboffsethvscale
C_INLINE XTError XTAPI xtset_docsuboffsethvscale(Fixed offset, Fixed hscale,
		Fixed vscale)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocSubOffsetHVScale(
				XTGetDocRefFromDocID(docID), offset, hscale, vscale)));
}

#undef xtget_docsuperiorhvscale
C_INLINE XTError XTAPI xtget_docsuperiorhvscale(Fixed *hscale, Fixed *vscale)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocSuperiorHVScale(XTGetDocRefFromDocID(docID),
			hscale, vscale)));
}

#undef xtset_docsuperiorhvscale
C_INLINE XTError XTAPI xtset_docsuperiorhvscale(Fixed hscale, Fixed vscale)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocSuperiorHVScale(XTGetDocRefFromDocID(docID),
			hscale, vscale)));
}

#undef xtget_docsuperoffsethvscale
C_INLINE XTError XTAPI xtget_docsuperoffsethvscale(Fixed *offset, Fixed *hscale,
		Fixed *vscale)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocSuperOffsetHVScale(
				XTGetDocRefFromDocID(docID), offset, hscale, vscale)));
}

#undef xtset_docsuperoffsethvscale
C_INLINE XTError XTAPI xtset_docsuperoffsethvscale(Fixed offset, Fixed hscale,
		Fixed vscale)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocSuperOffsetHVScale(
				XTGetDocRefFromDocID(docID), offset, hscale, vscale)));
}

#undef xtget_doctrueemspace
C_INLINE XTError XTAPI xtget_doctrueemspace(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocTrueEMSpace(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_doctrueemspace
C_INLINE XTError XTAPI xtset_doctrueemspace(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocTrueEMSpace(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docuseligatures
C_INLINE XTError XTAPI xtget_docuseligatures(uint16 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocUseLigatures(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docuseligatures
C_INLINE XTError XTAPI xtset_docuseligatures(uint16 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocUseLigatures(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docautoconstrain
C_INLINE XTError XTAPI xtget_docautoconstrain(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocAutoConstrain(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docautoconstrain
C_INLINE XTError XTAPI xtset_docautoconstrain(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocAutoConstrain(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docframeposition
C_INLINE XTError XTAPI xtget_docframeposition(uint8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocFramePosition(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docframeposition
C_INLINE XTError XTAPI xtset_docframeposition(uint8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocFramePosition(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docmastermode
C_INLINE XTError XTAPI xtget_docmastermode(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocMasterMode(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docmastermode
C_INLINE XTError XTAPI xtset_docmastermode(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocMasterMode(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docpginsertmode
C_INLINE XTError XTAPI xtget_docpginsertmode(uint8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocPgInsertMode(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docpginsertmode
C_INLINE XTError XTAPI xtset_docpginsertmode(uint8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocPgInsertMode(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docautotrapfactor
C_INLINE XTError XTAPI xtget_docautotrapfactor(Fixed *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocAutoTrapFactor(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docautotrapfactor
C_INLINE XTError XTAPI xtset_docautotrapfactor(Fixed xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocAutoTrapFactor(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docautotrapindetvalue
C_INLINE XTError XTAPI xtget_docautotrapindetvalue(Fixed *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocAutoTrapIndetValue(
			XTGetDocRefFromDocID(docID), xtd_value)));
}

#undef xtset_docautotrapindetvalue
C_INLINE XTError XTAPI xtset_docautotrapindetvalue(Fixed xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocAutoTrapIndetValue(
			XTGetDocRefFromDocID(docID), xtd_value)));
}

#undef xtget_docignorewhite
C_INLINE XTError XTAPI xtget_docignorewhite(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocIgnoreWhite(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docignorewhite
C_INLINE XTError XTAPI xtset_docignorewhite(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocIgnoreWhite(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docoverprintlimit
C_INLINE XTError XTAPI xtget_docoverprintlimit(Fixed *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocOverPrintLimit(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docoverprintlimit
C_INLINE XTError XTAPI xtset_docoverprintlimit(Fixed xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocOverPrintLimit(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docprocesstrap
C_INLINE XTError XTAPI xtget_docprocesstrap(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocProcessTrap(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docprocesstrap
C_INLINE XTError XTAPI xtset_docprocesstrap(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocProcessTrap(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_doctrappingknockoutlimit
C_INLINE XTError XTAPI xtget_doctrappingknockoutlimit(Fixed *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocTrappingKnockoutLimit(
			XTGetDocRefFromDocID(docID), xtd_value)));
}

#undef xtset_doctrappingknockoutlimit
C_INLINE XTError XTAPI xtset_doctrappingknockoutlimit(Fixed xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocTrappingKnockoutLimit(
			XTGetDocRefFromDocID(docID), xtd_value)));
}

#undef xtget_doctrappingmethod
C_INLINE XTError XTAPI xtget_doctrappingmethod(uint8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocTrappingMethod(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_doctrappingmethod
C_INLINE XTError XTAPI xtset_doctrappingmethod(uint8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocTrappingMethod(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docdeftabwidth
C_INLINE XTError XTAPI xtget_docdeftabwidth(Fixed *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocDefTabWidth(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docdeftabwidth
C_INLINE XTError XTAPI xtset_docdeftabwidth(Fixed xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocDefTabWidth(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docrighttoleft
C_INLINE XTError XTAPI xtget_docrighttoleft(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocRightToLeft(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docrighttoleft
C_INLINE XTError XTAPI xtset_docrighttoleft(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocRightToLeft(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_sharpengray
C_INLINE XTError XTAPI xtget_sharpengray(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetSharpenGray(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_sharpengray
C_INLINE XTError XTAPI xtset_sharpengray(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetSharpenGray(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_createdintruetype
C_INLINE XTError XTAPI xtget_createdintruetype(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetCreatedInTrueType(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_createdintruetype
C_INLINE XTError XTAPI xtset_createdintruetype(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetCreatedInTrueType(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docdefcolcount
C_INLINE XTError XTAPI xtget_docdefcolcount(int16 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocDefColCount(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docdefcolcount
C_INLINE XTError XTAPI xtset_docdefcolcount(int16 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocDefColCount(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docdefmars
C_INLINE XTError XTAPI xtget_docdefmars(Fixed *top, Fixed *bottom, Fixed *left,
		Fixed *right)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocDefMars(XTGetDocRefFromDocID(docID),
			top, bottom, left, right)));
}

#undef xtset_docdefmars
C_INLINE XTError XTAPI xtset_docdefmars(Fixed top, Fixed bottom, Fixed left,
		Fixed right)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocDefMars(XTGetDocRefFromDocID(docID),
			top, bottom, left, right)));
}

#undef xtget_dochvmeasure
C_INLINE XTError XTAPI xtget_dochvmeasure(uint8 *hmeasure,uint8 *vmeasure)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocHVMeasure(XTGetDocRefFromDocID(docID),
			hmeasure, vmeasure)));
}

#undef xtset_dochvmeasure
C_INLINE XTError XTAPI xtset_dochvmeasure(uint8 hmeasure,uint8 vmeasure)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocHVMeasure(XTGetDocRefFromDocID(docID),
			hmeasure, vmeasure)));
}

#undef xtset_docmakepreview
C_INLINE XTError XTAPI xtset_docmakepreview(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocMakePreview(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_doctypeset
C_INLINE XTError XTAPI xtget_doctypeset(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocTypeSet(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_doctypeset
C_INLINE XTError XTAPI xtset_doctypeset(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocTypeSet(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docspreadruler
C_INLINE XTError XTAPI xtget_docspreadruler(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocSpreadRuler(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docspreadruler
C_INLINE XTError XTAPI xtset_docspreadruler(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocSpreadRuler(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_annotationflag
C_INLINE XTError XTAPI xtset_annotationflag(uint8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetAnnotationFlag(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef deletebox
C_INLINE void deletebox(boxid bptr, bool8 doreflow)
{
	XTDeleteBox(bptr, doreflow);
}

#undef deletegroupbox
C_INLINE void deletegroupbox(boxid gpbox)
{
	XTDeleteGroupBox(gpbox);
}

#undef recalcgpbox
C_INLINE void recalcgpbox(boxid gpbox)
{
	XTRecalcGroupBox(gpbox, TRUE);
}

#undef setboxtype
C_INLINE boxid setboxtype(boxid thebox, int16 contenttype)
{
	boxid newBox = (boxid)0;
	
	XTSetBoxType(thebox, &newBox, contenttype);

	return newBox;
}

#undef xtmovebox
C_INLINE XTError xtmovebox(boxid boxID, Fixed dx, Fixed dy, bool16 kidsToo)
{
	return (XTAPIERRToXTError(XTMoveBox(boxID, dx, dy, kidsToo)));
}
#undef xtget_boxanonecolor
C_INLINE XTError xtget_boxanonecolor(boxid theboxid, bool8 *newnonecolor)
{
	return (XTAPIERRToXTError(XTGetBoxANoneColor(theboxid, newnonecolor)));
}

#undef xtset_boxanonecolor
C_INLINE XTError xtset_boxanonecolor(boxid theboxid, bool8 oldanonecolor)
{
	return (XTAPIERRToXTError(XTSetBoxANoneColor(theboxid, oldanonecolor)));
}

#undef xtget_boxasupprint
C_INLINE XTError xtget_boxasupprint(boxid theboxid, bool8 *newsupprint)
{
	return (XTAPIERRToXTError(XTGetBoxASupPrint(theboxid, newsupprint)));
}

#undef xtset_boxasupprint
C_INLINE XTError xtset_boxasupprint(boxid theboxid, bool8 oldsupprint)
{
	return (XTAPIERRToXTError(XTSetBoxASupPrint(theboxid, oldsupprint)));
}

#undef xtget_boxcolor
C_INLINE XTError xtget_boxcolor(boxid theboxid, colorid *newcolor)
{
	return (XTAPIERRToXTError(XTGetBoxColor(theboxid, newcolor)));
}

#undef xtset_boxcolor
C_INLINE XTError xtset_boxcolor(boxid theboxid, colorid oldcolor)
{
	return (XTAPIERRToXTError(XTSetBoxColor(theboxid, oldcolor)));
}

#undef xtget_boxshade
C_INLINE XTError xtget_boxshade(boxid theboxid, Fixed *newshade)
{
	return (XTAPIERRToXTError(XTGetBoxShade(theboxid, newshade)));
}

#undef xtset_boxshade
C_INLINE XTError xtset_boxshade(boxid theboxid, Fixed oldshade)
{
	return (XTAPIERRToXTError(XTSetBoxShade(theboxid, oldshade)));
}

#undef xtget_boxangle
C_INLINE XTError xtget_boxangle(boxid theboxid, Fixed *newangle)
{
	return (XTAPIERRToXTError(XTGetBoxAngle(theboxid, newangle)));
}

#undef xtset_boxangle
C_INLINE XTError xtset_boxangle(boxid theboxid, Fixed oldangle)
{
	return (XTAPIERRToXTError(XTSetBoxAngle(theboxid, oldangle)));
}

#undef xtget_boxskew
C_INLINE XTError xtget_boxskew(boxid theboxid, Fixed *newskew)
{
	return (XTAPIERRToXTError(XTGetBoxSkew(theboxid, newskew)));
}

#undef xtset_boxskew
C_INLINE XTError xtset_boxskew(boxid theboxid, Fixed oldskew)
{
	return (XTAPIERRToXTError(XTSetBoxSkew(theboxid, oldskew)));
}

#undef xtsetboxrasterize
C_INLINE OSErr xtsetboxrasterize(boxid boxId, uint16 rasterizeValue, uint16 isChangeable)
{
	OSErr retErr = noErr;

	if (XTGetDocRefFromBox(boxId) == INVAL_DOCREF) {
		retErr = ERRNOBOXNAME;
	}
	else {
		retErr = XTAPIERRToOSErr(
				XTSetBoxRasterize(boxId, rasterizeValue, isChangeable));		
	}

	return retErr;
}

#undef DelAllPicShapePathsByXTid
C_INLINE XTError XTAPI DelAllPicShapePathsByXTid(boxid pictureBox,
		XTid xtID)
{
	return(XTAPIERRToXTError(XTDelAllPicShapePathsByXTid(pictureBox, xtID)));
}

#undef DelPicShapePath
C_INLINE XTError XTAPI DelPicShapePath(boxid pictureBox, uint32 pathID)
{
	return(XTAPIERRToXTError(XTDelPicShapePath(pictureBox, pathID)));
}

#undef positionpict
C_INLINE void XTAPI positionpict(boxid theBox,int16 flags)
{
	XTPositionPicture(theBox, flags, TRUE);
}

#undef offsetbox
C_INLINE void XTAPI offsetbox(Fixed dx, Fixed dy)
{
	boxid curboxP;

	xtget_curbox(&curboxP);
	XTOffsetBox(curboxP, dx, dy);
}

#undef SetPicShapePath
C_INLINE XTError XTAPI SetPicShapePath(XTid xtID, boxid pictureBox,
		ShapeHandle path, uint32 *pathID, const RGBColor *color, PspType type,
		Handle xtData, bool8 select, int16 flags)
{
	return(XTAPIERRToXTError(XTSetPicShapePath(xtID, pictureBox, path, pathID,
			color, type, xtData, select, flags)));
}

#undef reimportpic
C_INLINE int16 XTAPI reimportpic(picboxid importpicptr,bool8 importall)
{
	return(XTAPIERRToOSErr(XTReimportPic(importpicptr, importall)));
}

#undef picchange
C_INLINE void XTAPI picchange(picboxid pbptr,uint32 flags)
{
	XTPicChange(pbptr, flags);
}

#undef xtgetboxrasterize
C_INLINE OSErr XTAPI xtgetboxrasterize(boxid boxID, uint16 *rasterizeValueP,
		uint16 *isChangeableP)
{
	OSErr retErr = noErr;

	if (XTGetDocRefFromBox(boxID) == INVAL_DOCREF) {
		retErr = ERRNOBOXNAME;
	}
	else {
		retErr = XTAPIERRToOSErr(
				XTGetBoxRasterize(boxID, rasterizeValueP, isChangeableP));
	}

	return retErr;
}

#undef xtsetboxname
C_INLINE APIERR XTAPI xtsetboxname(boxid theBoxIDP, uchar *newBoxNameP)
{
	assert( FALSE && "xtsetboxname" );
	return( (APIERR) -1 );
}

#undef constraingroup
C_INLINE void XTAPI constraingroup(boxid bptr)
{
	XTConstrainGroup(bptr);
}

#undef pasteanchitems
C_INLINE APIERR XTAPI pasteanchitems(int16 sourceDoc, int16 sourcePage)
{
	return(XTPasteAnchItems(XTGetDocRefFromDocID(sourceDoc), sourcePage));
}

#undef pasteitemsatxy
C_INLINE bool8 XTAPI pasteitemsatxy(DocID sourceDoc,int16 sourceSprd,boxid xBoxP,
		Fixed x, Fixed y)
{
	APIERR apiErr = ERR_FAILURE;
	apiErr = XTPasteItemsAtXY(XTGetDocRefFromDocID(sourceDoc), sourceSprd,
			xBoxP, x, y);
	return ((apiErr == ERR_SUCCESS) ? TRUE : FALSE);
}

#undef xtset_pboxagreekit
C_INLINE XTError XTAPI xtset_pboxagreekit(boxid theboxid, bool8 oldgreekit)
{
	return (XTAPIERRToXTError(XTSetPictBoxGreekIt(theboxid, oldgreekit)));
}

#undef xtget_pboxagreekit
C_INLINE XTError XTAPI xtget_pboxagreekit(boxid theboxid, bool8 *newgreekit)
{
	return (XTAPIERRToXTError(XTGetPictBoxGreekIt(theboxid, newgreekit)));
}

#undef xtset_pboxasuppicdraw
C_INLINE XTError XTAPI xtset_pboxasuppicdraw(boxid theboxid, bool8 oldsuppicdraw)
{
	return (XTAPIERRToXTError(XTSetSuppressPictDraw(theboxid, oldsuppicdraw)));
}

#undef xtget_pboxasuppicdraw
C_INLINE XTError XTAPI xtget_pboxasuppicdraw(boxid theboxid, bool8 *newsuppicdraw)
{
	return (XTAPIERRToXTError(XTGetSuppressPictDraw(theboxid, newsuppicdraw)));
}

#undef xtset_pboxasuppicprint
C_INLINE XTError XTAPI xtset_pboxasuppicprint(boxid theboxid, bool8 oldsuppicprint)
{
	return (XTAPIERRToXTError(XTSetSuppressPictPrint(theboxid, oldsuppicprint)));
}

#undef xtget_pboxasuppicprint
C_INLINE XTError XTAPI xtget_pboxasuppicprint(boxid theboxid, bool8 *newsuppicprint)
{
	return (XTAPIERRToXTError(XTGetSuppressPictPrint(theboxid, newsuppicprint)));
}

#undef XTSetPictureColor
C_INLINE APIERR XTAPI XTSetPictureColor(boxid theboxid, colorid newValue)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);

	XTSetPictureViewColor(pViewRef, newValue);

	API_EXIT;
}

#undef XTGetPictureColor
C_INLINE APIERR XTAPI XTGetPictureColor(boxid theboxid, colorid *newpiccolor)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);
	PARAM_CHECK(2, newpiccolor != NULL);

	XTGetPictureViewColor(pViewRef, newpiccolor);

	API_EXIT;
}

#undef XTSetPictureShade
C_INLINE APIERR XTAPI XTSetPictureShade(boxid theboxid, Fixed oldpicshade)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);

	XTSetPictureViewShade(pViewRef, oldpicshade);

	API_EXIT;
}

#undef XTGetPictureShade
C_INLINE APIERR XTAPI XTGetPictureShade(boxid theboxid, Fixed *newpicshade)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);
	PARAM_CHECK(2, newpicshade != NULL);

	XTGetPictureViewShade(pViewRef, newpicshade);

	API_EXIT;
}

#undef XTSetPictureAngle
C_INLINE APIERR XTAPI XTSetPictureAngle(boxid theboxid, Fixed oldpicangle)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid,  &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);

	XTSetPictureViewAngle(pViewRef, oldpicangle);

	API_EXIT;
}

#undef XTGetPictureAngle
C_INLINE APIERR XTAPI XTGetPictureAngle(boxid theboxid, Fixed *newpicangle)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);
	PARAM_CHECK(2, newpicangle != NULL);

	XTGetPictureViewAngle(pViewRef, newpicangle);

	API_EXIT;
}

#undef XTSetPictureSkew
C_INLINE APIERR XTAPI XTSetPictureSkew(boxid theboxid, Fixed oldpicskew)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);

	XTSetPictureViewSkew(pViewRef, oldpicskew);

	API_EXIT;
}

#undef XTGetPictureSkew
C_INLINE APIERR XTAPI XTGetPictureSkew(boxid theboxid, Fixed *newpicskew)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);
	PARAM_CHECK(2, newpicskew != NULL);

	XTGetPictureViewSkew(pViewRef, newpicskew);

	API_EXIT;
}

#undef XTSetPictureXScroll
C_INLINE APIERR XTAPI XTSetPictureXScroll(boxid theboxid, Fixed oldxscroll)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);

	XTSetPictureViewXScroll(pViewRef, oldxscroll);

	API_EXIT;
}

#undef XTGetPictureXScroll
C_INLINE APIERR XTAPI XTGetPictureXScroll(boxid theboxid, Fixed *newxscroll)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);
	PARAM_CHECK(2, newxscroll != NULL);

	XTGetPictureViewXScroll(pViewRef, newxscroll);

	API_EXIT;
}

#undef XTSetPictureYScroll
C_INLINE APIERR XTAPI XTSetPictureYScroll(boxid theboxid, Fixed oldyscroll)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);

	XTSetPictureViewYScroll(pViewRef, oldyscroll);

	API_EXIT;
}

#undef XTGetPictureYScroll
C_INLINE APIERR XTAPI XTGetPictureYScroll(boxid theboxid, Fixed *newyscroll)
{

	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);
	PARAM_CHECK(2, newyscroll != NULL);

	XTGetPictureViewYScroll(pViewRef, newyscroll);

	API_EXIT;
}

#undef XTSetPictureXScale
C_INLINE APIERR XTAPI XTSetPictureXScale(boxid theboxid, Fixed oldxscale)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);

	XTSetPictureViewXScale(pViewRef, oldxscale);

	API_EXIT;
}

#undef XTGetPictureXScale
C_INLINE APIERR XTAPI XTGetPictureXScale(boxid theboxid, Fixed *newxscale)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);
	PARAM_CHECK(2, newxscale != NULL);

	XTGetPictureViewXScale(pViewRef, newxscale);

	API_EXIT;
}

#undef XTSetPictureYScale
C_INLINE APIERR XTAPI XTSetPictureYScale(boxid theboxid, Fixed oldyscale)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);

	XTSetPictureViewYScale(pViewRef, oldyscale);

	API_EXIT;
}

#undef XTGetPictureYScale
C_INLINE APIERR XTAPI XTGetPictureYScale(boxid theboxid, Fixed *newyscale)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);
	PARAM_CHECK(2, newyscale != NULL);

	XTGetPictureViewYScale(pViewRef, newyscale);

	API_EXIT;
}

#undef XTSetPictureAuxXScale
C_INLINE APIERR XTAPI XTSetPictureAuxXScale(boxid theboxid, Fixed oldauxxscale)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);

	XTSetPictureViewAuxXRatio(pViewRef, oldauxxscale);

	API_EXIT;
}

#undef XTGetPictureAuxXScale
C_INLINE APIERR XTAPI XTGetPictureAuxXScale(boxid theboxid, Fixed *newauxxscale)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);
	PARAM_CHECK(2, newauxxscale != NULL);

	XTGetPictureViewAuxXRatio(pViewRef, newauxxscale);

	API_EXIT;
}

#undef XTSetPictureAuxYScale
C_INLINE APIERR XTAPI XTSetPictureAuxYScale(boxid theboxid, Fixed oldauxyscale)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid,  &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);

	XTSetPictureViewAuxYRatio(pViewRef, oldauxyscale);

	API_EXIT;
}

#undef XTGetPictureAuxYScale
C_INLINE APIERR XTAPI XTGetPictureAuxYScale(boxid theboxid, Fixed *newauxyscale)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);
	PARAM_CHECK(2, newauxyscale != NULL);

	XTGetPictureViewAuxYRatio(pViewRef, newauxyscale);

	API_EXIT;
}

#undef XTSetPictureContrast
C_INLINE APIERR XTAPI XTSetPictureContrast(boxid theboxid, uint8 oldcontrast)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);

	XTSetPictureViewContrast(pViewRef, oldcontrast);

	API_EXIT;
}

#undef XTGetPictureContrast
C_INLINE APIERR XTAPI XTGetPictureContrast(boxid theboxid, uint8 *newContrastP)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);
	PARAM_CHECK(2, newContrastP != NULL);

	XTGetPictureViewContrast(pViewRef, newContrastP);

	API_EXIT;
}

#undef xtset_pboxauxxscale
C_INLINE XTError XTAPI xtset_pboxauxxscale(boxid theboxid, Fixed oldauxxscale)
{
	return (XTAPIERRToXTError(XTSetPictureAuxXScale(theboxid, oldauxxscale)));
}

#undef xtget_pboxauxxscale
C_INLINE XTError XTAPI xtget_pboxauxxscale(boxid theboxid, Fixed *newauxxscale)
{
	return (XTAPIERRToXTError(XTGetPictureAuxXScale(theboxid, newauxxscale)));
}

#undef xtset_pboxauxyscale
C_INLINE XTError XTAPI xtset_pboxauxyscale(boxid theboxid, Fixed oldauxyscale)
{
	return (XTAPIERRToXTError(XTSetPictureAuxYScale(theboxid, oldauxyscale)));
}

#undef xtget_pboxauxyscale
C_INLINE XTError XTAPI xtget_pboxauxyscale(boxid theboxid, Fixed *newauxyscale)
{
	return (XTAPIERRToXTError(XTGetPictureAuxYScale(theboxid, newauxyscale)));
}

#undef xtset_pboxpicangle
C_INLINE XTError XTAPI xtset_pboxpicangle(boxid theboxid, Fixed oldpicangle)
{
	return (XTAPIERRToXTError(XTSetPictureAngle(theboxid, oldpicangle)));
}

#undef xtget_pboxpicangle
C_INLINE XTError XTAPI xtget_pboxpicangle(boxid theboxid, Fixed *newpicangle)
{
	return (XTAPIERRToXTError(XTGetPictureAngle(theboxid, newpicangle)));
}

#undef xtset_pboxpiccolor
C_INLINE XTError XTAPI xtset_pboxpiccolor(boxid theboxid, colorid oldpiccolor)
{
	return (XTAPIERRToXTError(XTSetPictureColor(theboxid, oldpiccolor)));
}

#undef xtget_pboxpiccolor
C_INLINE XTError XTAPI xtget_pboxpiccolor(boxid theboxid, colorid *newpiccolor)
{
	return (XTAPIERRToXTError(XTGetPictureColor(theboxid, newpiccolor)));
}

#undef xtset_pboxpicshade
C_INLINE XTError XTAPI xtset_pboxpicshade(boxid theboxid, Fixed oldpicshade)
{
	return (XTAPIERRToXTError(XTSetPictureShade(theboxid, oldpicshade)));
}

#undef xtget_pboxpicshade
C_INLINE XTError XTAPI xtget_pboxpicshade(boxid theboxid, Fixed *newpicshade)
{
	return (XTAPIERRToXTError(XTGetPictureShade(theboxid, newpicshade)));
}

#undef xtset_pboxpicskew
C_INLINE XTError XTAPI xtset_pboxpicskew(boxid theboxid, Fixed oldpicskew)
{
	return (XTAPIERRToXTError(XTSetPictureSkew(theboxid, oldpicskew)));
}

#undef xtget_pboxpicskew
C_INLINE XTError XTAPI xtget_pboxpicskew(boxid theboxid, Fixed *newpicskew)
{
	return (XTAPIERRToXTError(XTGetPictureSkew(theboxid, newpicskew)));
}

#undef xtset_pboxxscale
C_INLINE XTError XTAPI xtset_pboxxscale(boxid theboxid, Fixed oldxscale)
{
	return (XTAPIERRToXTError(XTSetPictureXScale( theboxid, oldxscale)));
}

#undef xtget_pboxxscale
C_INLINE XTError XTAPI xtget_pboxxscale(boxid theboxid, Fixed *newxscale)
{
	return (XTAPIERRToXTError(XTGetPictureXScale(theboxid, newxscale)));
}

#undef xtset_pboxxscroll
C_INLINE XTError XTAPI xtset_pboxxscroll(boxid theboxid, Fixed oldxscroll)
{
	return (XTAPIERRToXTError(XTSetPictureXScroll(theboxid, oldxscroll)));
}

#undef xtget_pboxxscroll
C_INLINE XTError XTAPI xtget_pboxxscroll(boxid theboxid, Fixed *newxscroll)
{
	return (XTAPIERRToXTError(XTGetPictureXScroll(theboxid, newxscroll)));
}

#undef xtset_pboxyscroll
C_INLINE XTError XTAPI xtset_pboxyscroll(boxid theboxid, Fixed oldyscroll)
{
	return (XTAPIERRToXTError(XTSetPictureYScroll(theboxid, oldyscroll)));
}

#undef xtget_pboxyscroll
C_INLINE XTError XTAPI xtget_pboxyscroll(boxid theboxid, Fixed *newyscroll)
{
	return (XTAPIERRToXTError(XTGetPictureYScroll(theboxid, newyscroll)));
}


#undef xtset_pboxyscale
C_INLINE XTError XTAPI xtset_pboxyscale(boxid theboxid, Fixed oldyscale)
{
	return (XTAPIERRToXTError(XTSetPictureYScale(theboxid, oldyscale)));
}

#undef xtget_pboxyscale
C_INLINE XTError XTAPI xtget_pboxyscale(boxid theboxid, Fixed *newyscale)
{
	return (XTAPIERRToXTError(XTGetPictureYScale(theboxid, newyscale)));
}

#undef disposextbox
C_INLINE void XTAPI disposextbox(xtboxptr xtBoxP, bool16 trashIt)
{
	XTDisposeXTBox(xtBoxP, trashIt);
}

#undef setstyleontext
C_INLINE bool8 XTAPI setstyleontext(AssetUID assetUID, bool8 pStyle, bool8 reCalc)
{
	xehandle xeH = NULL;
	boxid curBoxID = 0;
	bool8 result = FALSE;
	
	xtget_curbox(&curBoxID);
	if (istextbox(curBoxID)) {
		xegetinfo(curBoxID, &xeH, NULL, NULL, NULL);
		result = (XTSetStyleOnText(assetUID, pStyle, reCalc, xeH)
				== ERR_SUCCESS ? TRUE : FALSE);
	}

	return result;
}

#undef setcharstyleontext
C_INLINE bool8 XTAPI setcharstyleontext(AssetUID assetUID, bool8 reCalc)
{
	xehandle xeH = NULL;
	boxid curBoxID = 0;
	bool8 result = FALSE;
	
	xtget_curbox(&curBoxID);
	if ((curBoxID != 0) && istextbox(curBoxID)) {
		xegetinfo(curBoxID, &xeH, NULL, NULL, NULL);
		result = (XTSetStyleOnText(assetUID, FALSE, reCalc, xeH)
				== ERR_SUCCESS ? TRUE : FALSE);
	}

	return result;
}

#undef setparastyleontext
C_INLINE bool8 XTAPI setparastyleontext(AssetUID assetUID, bool8 reCalc)
{
	xehandle xeH = NULL;
	boxid curBoxID = 0;
	bool8 result = FALSE;
	
	xtget_curbox(&curBoxID);
	if ((curBoxID != 0) && istextbox(curBoxID)) {
		xegetinfo(curBoxID, &xeH, NULL, NULL, NULL);
		result = (XTSetStyleOnText(assetUID, TRUE, reCalc, xeH)
				== ERR_SUCCESS ? TRUE : FALSE);
	}

	return result;
}

#undef xt_detachdef
C_INLINE APIERR XTAPI xt_detachdef(boxid theBoxId)
{
	return(XTDetachDef(theBoxId));
}

#undef xtset_masterpgname
C_INLINE XTError XTAPI xtset_masterpgname(boxid sprdbox, uchar* name)
{
	assert( FALSE && "xtset_masterpgname" );
	return( XTError(-1) );
}

#undef xtset_docautokern
C_INLINE XTError XTAPI xtset_docautokern(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocAutoKern(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docautokern
C_INLINE XTError XTAPI xtget_docautokern(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocAutoKern(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_dockern2space
C_INLINE XTError XTAPI xtset_dockern2space(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocKern2Space(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_dockern2space
C_INLINE XTError XTAPI xtget_dockern2space(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocKern2Space(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_dockernabove
C_INLINE XTError XTAPI xtset_dockernabove(Fixed xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocKernAbove(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_dockernabove
C_INLINE XTError XTAPI xtget_dockernabove(Fixed *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocKernAbove(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docligaturelimit
C_INLINE XTError XTAPI xtset_docligaturelimit(Fixed xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTSetDocLigatureLimit(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docligaturelimit
C_INLINE XTError XTAPI xtget_docligaturelimit(Fixed *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetDocLigatureLimit(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef settextoutset
C_INLINE void XTAPI settextoutset(boxid bptr, const fixedrect *outsetfr)
{
	XTSetTextOutset(bptr, outsetfr);
}
/* Legacy loshandle macro -
** los handles are deprecated in 7.0, and are only built on demand.
*/
#define LOSINDEX(a,b) ((**(loshandle)(a))+(b))

#undef MaxObjectScroll
C_INLINE void XTAPI MaxObjectScroll(spreadboxid spreadBoxP, uint32 *hScrollP,
		uint32 *vScrollP)
{
	XTMaxObjectScroll(spreadBoxP, (uint64*) hScrollP, (uint64*) vScrollP, VIEW100, FALSE);
}

#undef kernchecksum
C_INLINE int32 XTAPI kernchecksum(int16 *kernTableP)
{
	int32 checkSum = 0L;

	XTGetKernCheckSum(kernTableP, &checkSum);

	return checkSum;
}

#if MACOS
#undef XTGetScaledContRgn
C_INLINE void XTAPI XTGetScaledContRgn(boxid boxID,RgnHandle rgnhndl)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(XTGetDocRefFromBox(boxID), &docViewRef);
	XTViewGetScaledContRgn(docViewRef, boxID, &rgnhndl);
}
#endif

#undef XTGetPageNumData
C_INLINE APIERR XTAPI XTGetPageNumData(DocRef docRef, int16 p,
		bool16 checkshowdef, pagenumdata *returndata)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTViewGetPageNumData(docViewRef, p, checkshowdef, returndata);
}

#undef XTGetDispPageNum
C_INLINE APIERR XTAPI XTGetDispPageNum(DocRef docRef, int16 pageSeq,
		uchar *theStr)
{
	DocViewRef docViewRef = kInvalDocViewRef;
	QXStringRef pageName = NULL;
	uint8 showDefault = FALSE;
	int16 pageNumAbs = pageSeq;
	bool8 isConverted = FALSE;
	int32 charCount = 0;
	int32 uniCharCount = 0;
	UniChar *uniCharBuffer = NULL;
	API_ENTRY

	PARAM_CHECK(1, docRef != INVAL_DOCREF);
	PARAM_CHECK(2, pageSeq >= 1);
	PARAM_CHECK(3, theStr != NULL);

	XTGetDocViewRef(docRef, &docViewRef);

	PARAM_CHECK(1, docViewRef != kInvalDocViewRef);

	XTGetViewShowMaster(docViewRef, &showDefault);
	if (!showDefault) {
		int16 numMasterPages = 0;

		XTNumMasterPages(docRef, &numMasterPages);
		pageNumAbs += numMasterPages;
	}

	SET_TECHERR(XTGetDisplayPageNumAbsolute(docRef, pageNumAbs, &pageName), ERR_SUCCESS);
	EXIT_ON_ERROR(TECHERR != ERR_SUCCESS);
	
	QXStringGetLength(pageName, &uniCharCount);
	QXStringGetBuffer(pageName, &uniCharBuffer);

	charCount = uniCharCount + 1;

	SET_TECHERR(XUFromUnicodePreAlloc(uniCharCount,
			(const UniChar *)uniCharBuffer, &charCount, (uchar *)theStr,
			&isConverted, kUTextEncodingPlatform), ERR_SUCCESS);
	CSTRTOLOCAL(theStr);
	QXStringDestroy(pageName);

	API_EXIT;
}

#undef XTGetPageSlugCollection
C_INLINE XTSlugCollection XTAPI XTGetPageSlugCollection(DocRef docRef,
		int16 pageNumber)
{
	XTSlugCollection slugCollection = INVAL_SLUGCOLLECTION;
	DocViewRef docViewRef = kInvalDocViewRef;
	XTGetDocViewRef(docRef, &docViewRef);

	if (docViewRef != kInvalDocViewRef && pageNumber >= 1) {
		uint8 showDefault = FALSE;
		int16 pageNumAbs = pageNumber;


		XTGetViewShowMaster(docViewRef, &showDefault);
		if (!showDefault) {
			int16 numMasterPages = 0;

			XTNumMasterPages(docRef, &numMasterPages);
			pageNumAbs += numMasterPages;
		}
		slugCollection = XTGetPageSlugCollectionAbsolute(docRef, pageNumAbs);
	}

	return slugCollection;
}

#undef xtgetpageinfo
C_INLINE void XTAPI xtgetpageinfo(boxid boxID, fixedrect *fboundr,
	bool16 docustom, bool16 doseparateinfo, bool16 docolorinfo,
	int16 pictureoutput, pspageauxinforec *xtauxinfoptr)
{
	XTGetPageInfo(boxID, fboundr, docustom, doseparateinfo,
		docolorinfo, pictureoutput, xtauxinfoptr);
}

#undef XTGotoMaster
C_INLINE APIERR XTAPI XTGotoMaster(DocRef docRef, int16 masterPage)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTViewGotoMaster(docViewRef, masterPage);
}

#undef cursorposition
C_INLINE int32 XTAPI cursorposition(boxid boxID)
{
	int32 charPos = 0;
	DocViewRef docViewRef = kInvalDocViewRef;
	EventRecord *eventP;

	xtget_curevent(&eventP);
	XTGetDocViewRef(XTGetDocRefFromBox(boxID), &docViewRef);
	XTGetCursorPositionInStory(docViewRef, (textboxid)boxID, eventP->where,
			&charPos, NULL, NULL, NULL);

	return (charPos);
}

#undef XTSetThumbView
C_INLINE APIERR XTAPI XTSetThumbView(DocRef docRef, bool8 thumbView)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTSetViewThumbView(docViewRef, thumbView);
}

#undef XTGetThumbView
C_INLINE APIERR XTAPI XTGetThumbView(DocRef docRef, bool8 *thumbViewP)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTGetViewThumbView(docViewRef, thumbViewP);
}

#undef XTSetInvisibleShown
C_INLINE APIERR XTAPI XTSetInvisibleShown(DocRef docRef, bool8 showInvis)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTSetViewInvisibleShown(docViewRef, showInvis);
}

#undef XTGetInvisibleShown
C_INLINE APIERR XTAPI XTGetInvisibleShown(DocRef docRef, bool8 *showInvisP)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTGetViewInvisibleShown(docViewRef, showInvisP);
}

#undef XTGetMasterPageShown
C_INLINE APIERR XTAPI XTGetMasterPageShown(DocRef docRef, uint8 *masterPageShownP)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTGetViewMasterPageShown(docViewRef, masterPageShownP);
}

#undef XTGetShowMaster
C_INLINE APIERR XTAPI XTGetShowMaster(DocRef docRef,uint8 *xtd_value)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTGetViewShowMaster(docViewRef, xtd_value);
}

#undef XTSetShowMaster
C_INLINE APIERR XTAPI XTSetShowMaster(DocRef docRef,uint8 xtd_value)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTSetViewShowMaster(docViewRef, xtd_value);
}

#undef XTGetEditPolyMode
C_INLINE APIERR XTAPI XTGetEditPolyMode(DocRef docRef, int8 *editPolyModeP)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTGetViewEditPolyMode(docViewRef, editPolyModeP);
}

#undef XTSetEditPolyMode
C_INLINE APIERR XTAPI XTSetEditPolyMode(DocRef docRef, int16 editPolyMode)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTSetViewEditPolyMode(docViewRef, editPolyMode);
}

#undef XTGetSnapToGuides
C_INLINE APIERR XTAPI XTGetSnapToGuides(DocRef docRef, bool8 *snapToGuidesP)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTGetViewSnapToGuides(docViewRef, snapToGuidesP);
}

#undef XTSetSnapToGuides
C_INLINE APIERR XTAPI XTSetSnapToGuides(DocRef docRef, bool8 snapToGuides)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTSetViewSnapToGuides(docViewRef, snapToGuides);
}

#undef XTGetRuler
C_INLINE APIERR XTAPI XTGetRuler(DocRef docRef, bool8 *rulerVisibleP)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTGetViewRuler(docViewRef, rulerVisibleP);
}

#undef XTSetRuler
C_INLINE APIERR XTAPI XTSetRuler(DocRef docRef, bool8 rulerVisible)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTSetViewRuler(docViewRef, rulerVisible);
}

#undef XTSetDocScale
C_INLINE APIERR XTAPI XTSetDocScale(DocRef docRef, Fixed scale, int16 redraw,
		const Rect *dragRectP)
{
	DocViewRef docViewRef = kInvalDocViewRef;
	Fixed newScale = scale;
	Fixed displayScale = FIX(0);

	XTGetDocViewRef(docRef, &docViewRef);
	XTGetDocDisplayDPIScale(docRef, &displayScale);

#if WINOS
	if (newScale != VIEWTHUMB && newScale != VIEWFIT) {
		newScale = FixDiv(newScale, displayScale);
	}
#endif // if WINOS

	return XTSetViewDocScale(docViewRef, newScale, redraw, dragRectP);
}

#undef XTGetDocScale
C_INLINE APIERR XTAPI XTGetDocScale(DocRef docRef, Fixed *docScaleP)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTGetViewDocScale(docViewRef, docScaleP);
}

#undef XTGetScrollXYVal
C_INLINE APIERR XTAPI XTGetScrollXYVal(DocRef docRef, int16 *xP, int32 *yP)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTGetViewScrollXYVal(docViewRef, (int64*) xP, (int64*) yP);
}

#undef XTSetScrollXYVal
C_INLINE APIERR XTAPI XTSetScrollXYVal(DocRef docRef, int16 x, int32 y)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTSetViewScrollXYVal(docViewRef, x, y);
}

#undef XTGetScrollXYMax
C_INLINE APIERR XTAPI XTGetScrollXYMax(DocRef docRef, int16 *xP, int32 *yP)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTGetViewScrollXYMax(docViewRef, (int64*) xP, (int64*) yP);
}

#undef XTSetScrollXYMax
C_INLINE APIERR XTAPI XTSetScrollXYMax(DocRef docRef, int16 x, int32 y)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTSetViewScrollXYMax(docViewRef, x, y);
}

#undef XTGetDispDefSpread
C_INLINE APIERR XTAPI XTGetDispDefSpread(DocRef docRef,int16 *xtd_value)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTGetViewDispDefSpread(docViewRef, xtd_value);
}

#undef XTSetDispDefSpread
C_INLINE APIERR XTAPI XTSetDispDefSpread(DocRef docRef,int16 xtd_value)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTSetViewDispDefSpread(docViewRef, xtd_value);
}

#undef XTGetCurPage
C_INLINE APIERR XTAPI XTGetCurPage(DocRef docRef, int16 *returnValue)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTGetViewCurPage(docViewRef, returnValue);
}

#undef xtset_docfitinwindow
C_INLINE XTError XTAPI xtset_docfitinwindow(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = kInvalDocViewRef;
	XTError err = noErr;

	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
		err = XTAPIERRToXTError(XTSetViewFitInWindow(docViewRef, xtd_value));
	}
	return err;
}

#undef xtget_docfitinwindow
C_INLINE XTError XTAPI xtget_docfitinwindow(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = kInvalDocViewRef;
	XTError err = noErr;

	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
		err = XTAPIERRToXTError(XTGetViewFitInWindow(docViewRef, xtd_value));
	}
	return err;
}


#undef xtset_docshowguides
C_INLINE XTError XTAPI xtset_docshowguides(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = kInvalDocViewRef;
	XTError err = noErr;

	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
		err = XTAPIERRToXTError(XTSetViewShowGuides(docViewRef, xtd_value));
	}
	return err;
}

#undef xtset_docshowgrid
C_INLINE XTError XTAPI xtset_docshowgrid(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = kInvalDocViewRef;
	XTError err = noErr;

	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
		err = XTAPIERRToXTError(XTSetViewShowGrid(docViewRef, xtd_value));
	}
	return err;
}

#undef xtget_docshowgrid
C_INLINE XTError XTAPI xtget_docshowgrid(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = kInvalDocViewRef;
	XTError err = noErr;

	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
		err = XTAPIERRToXTError(XTGetViewShowGrid(docViewRef, xtd_value));
	}
	return err;
}

#undef xtget_docshowguides
C_INLINE XTError XTAPI xtget_docshowguides(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = kInvalDocViewRef;
	XTError err = noErr;

	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
		err = XTAPIERRToXTError(XTGetViewShowGuides(docViewRef, xtd_value));
	}
	return err;
}

#undef xtset_docgreekpicts
C_INLINE XTError XTAPI xtset_docgreekpicts(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = kInvalDocViewRef;
	XTError err = noErr;

	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
		err = XTAPIERRToXTError(XTSetViewGreekPicts(docViewRef, xtd_value));
	}
	return err;
}

#undef xtget_docgreekpicts
C_INLINE XTError XTAPI xtget_docgreekpicts(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = kInvalDocViewRef;
	XTError err = noErr;

	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
		err = XTAPIERRToXTError(XTGetViewGreekPicts(docViewRef, xtd_value));
	}
	return err;
}

#undef xtset_docgreekenabled
C_INLINE XTError XTAPI xtset_docgreekenabled(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;
	XTError err = noErr;

	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		DocRef docRef = XTGetDocRefFromDocID(docID);
		err = XTAPIERRToXTError(XTSetGreekEnabled(docRef, xtd_value));
	}
	return err;
}

#undef xtget_docgreekenabled
C_INLINE XTError XTAPI xtget_docgreekenabled(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;
	XTError err = noErr;

	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		DocRef docRef = XTGetDocRefFromDocID(docID);
		err = XTAPIERRToXTError(XTGetGreekEnabled(docRef, xtd_value));
	}
	return err;
}

#undef xtset_docgreekbelow
C_INLINE XTError XTAPI xtset_docgreekbelow(Fixed xtd_value)
{
	DocID docID = INVAL_DOCID;
	DocRef docRef = INVAL_DOCREF;

	xtget_curdoc(&docID);
	docRef = XTGetDocRefFromDocID(docID);
	return XTAPIERRToXTError(XTSetDocGreekBelow(docRef, xtd_value));
}

#undef XTDoHandles
C_INLINE APIERR XTAPI XTDoHandles(boxid boxID, const XTBoxHandleRec *handleRecP,
		bool8 pdrawhandles)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(XTGetDocRefFromBox(boxID), &docViewRef);
	return XTViewDoHandles(docViewRef, boxID, handleRecP, pdrawhandles);
}

#undef XTShowDefault
C_INLINE int16 XTAPI XTShowDefault(DocRef docRef,int16 newdefault,bool16 update)
{
	DocViewRef docViewRef = kInvalDocViewRef;
	int16 retXTShowViewDefault;

	XTGetDocViewRef(docRef, &docViewRef);
	retXTShowViewDefault = (int16)XTShowViewDefault(docViewRef, newdefault,
			update);
	return retXTShowViewDefault;
}

#undef XTDocContRect
C_INLINE APIERR XTAPI XTDocContRect(DocRef docRef, Rect *r)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTGetViewContRect(docViewRef, r);
}

#undef inshapehandle
C_INLINE bool16 XTAPI inshapehandle(handleid *mar)
{
	bool8 result = FALSE;
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = kInvalDocViewRef;

	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
		Point p;
		XTViewInShapeHandle(docViewRef, mar, p,&result);
	}
	return result;
}

#undef snapcursor
C_INLINE void XTAPI snapcursor(bool16 snap)
{
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = kInvalDocViewRef;

	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
		XTViewSnapCursor(docViewRef, snap);
	}
}

#undef visboxarea
C_INLINE Area XTAPI visboxarea(boxid boxID, int16 vistype, int16 areatype)
{
	Area area;
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(XTGetDocRefFromBox(boxID), &docViewRef);
	XTViewVisBoxArea(docViewRef, boxID, vistype, &area);
	return area;
}

#undef mkboxclip
C_INLINE ShapeHandle XTAPI mkboxclip(boxid boxID)
{
	ShapeHandle shapeH = NULL;
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(XTGetDocRefFromBox(boxID), &docViewRef);
	XTViewMakeClipBox(docViewRef, boxID, &shapeH);
	return shapeH;
}

#undef XTGetScaledBBox
C_INLINE APIERR XTAPI XTGetScaledBBox(boxid boxID,Rect *r,bool8 incoutline,
		int16 fullrects)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(XTGetDocRefFromBox(boxID), &docViewRef);
	XGRectF xgr;
	return XTViewGetScaledBBox(docViewRef, boxID, &xgr, incoutline, fullrects);
}

#undef gotobox
C_INLINE bool8 XTAPI gotobox(boxid boxID)
{
	bool8 retValue = FALSE;
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(XTGetDocRefFromBox(boxID), &docViewRef);
	XTViewGotoBox(docViewRef, boxID, FALSE, &retValue);
	return retValue;
}

#undef xt_DrawBoxLinkage
C_INLINE XTError XTAPI xt_DrawBoxLinkage(boxid fromBoxID, boxid toBoxID)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(XTGetDocRefFromBox(fromBoxID), &docViewRef);
	return XTAPIERRToXTError(XTViewDrawBoxLinkage(docViewRef, fromBoxID,
			toBoxID));
}

#undef showsel
C_INLINE void XTAPI showsel(Rect *r,textboxid boxID)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(XTGetDocRefFromBox(boxID), &docViewRef);
	XTViewShowSel(docViewRef, r, boxID);
}

#undef XTGotoPage
C_INLINE APIERR XTAPI XTGotoPage(DocRef docRef, int16 p)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTViewGotoPage(docViewRef, p);
}

#undef XTSetSprdOrigin
C_INLINE APIERR XTAPI XTSetSprdOrigin(DocRef docRef,int16 spread)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTSetViewSpreadOrigin(docViewRef, spread);
}

#undef XTCurBoxSpreadOrigin
C_INLINE APIERR XTAPI XTCurBoxSpreadOrigin(DocRef docRef)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTCurBoxSpreadViewOrigin(docViewRef);
}

#undef XTScrollContents
C_INLINE APIERR XTAPI XTScrollContents(DocRef docRef, int16 deltaX, int32 deltaY)
{
	bool8 scrolled = FALSE;
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTScrollViewContents(docViewRef, deltaX, deltaY, &scrolled);
}

#undef XTScrollDoc
C_INLINE APIERR XTAPI XTScrollDoc(DocRef docRef, int16 stype, Point scrlpt,
		RgnHandle prevclip1, RgnHandle prevclip2, RgnHandle prevclip3,
		bool8 *scrolledP)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTScrollView(docViewRef, stype, scrlpt, scrolledP);
}

#undef XTGetPageRect
C_INLINE APIERR XTAPI XTGetPageRect(DocRef docRef, int16 pagenum, Rect *pagerect)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	XGRectF xgr;
	return XTGetViewPageRect(docViewRef, pagenum, &xgr);
}

#undef XTSetZeroOrigin
C_INLINE APIERR XTAPI XTSetZeroOrigin(DocRef docRef)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTSetViewZeroOrigin(docViewRef);
}

#undef XTWhichBox
C_INLINE APIERR XTAPI XTWhichBox(const DocRef docRef, const xpoint hitLocation,
		const uint32 keyModifiers, const TOOLMODE toolPaletteMode,
		const boxid startingBox, const boxid currentBox,
		const bool8 searchChildren, boxid *foundBoxP)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	XGPointF hl;
	hl.mX = hitLocation.h;
	hl.mY = hitLocation.h;
	ToolRef tr;
	return XTViewWhichBox(docViewRef, hl, keyModifiers, tr,
			startingBox, foundBoxP);
}

#undef SetPicShapePathVisibility
C_INLINE XTError XTAPI SetPicShapePathVisibility(XTid xtID,bool8 visible,
		PspVisibleOverride override,int16 flags)
{
	XTError err = noErr;

	if ((flags&SETVISALLVALID) != 0) {
		DocID docID = INVAL_DOCID;
		DocViewRef docViewRef = kInvalDocViewRef;

		xtget_curdoc(&docID);
		if (docID != INVAL_DOCID) {
			XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
			err = XTAPIERRToXTError(XTSetPicShapePathVisibility(docViewRef, xtID,
					visible, override, flags));
		}
	}
	return err;
}

#undef AnyPicShapePathIsVisibleByXTid
C_INLINE bool8 XTAPI AnyPicShapePathIsVisibleByXTid(boxid boxID,XTid xtID)
{
	bool8 result = FALSE;
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(XTGetDocRefFromBox(boxID), &docViewRef);
	XTAnyPicShapePathIsVisibleByXTid(docViewRef, boxID, xtID, &result);
	return result; 
}

#undef GetPicShapePathVisibility
C_INLINE void XTAPI GetPicShapePathVisibility(XTid xtID,bool8 *visible,
		PspVisibleOverride *override)
{
	if (override != NULL) {
		*override = PSPV_NOOVERRIDE;
	}
	{
		DocID docID = INVAL_DOCID;
		DocViewRef docViewRef = kInvalDocViewRef;

		xtget_curdoc(&docID);
		if (docID != INVAL_DOCID) {
			XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
			XTGetPicShapePathVisibility(docViewRef, xtID, visible, override);
		}
	}
}

#undef XTGetMasterUpdateItemStatus
C_INLINE APIERR XTAPI XTGetMasterUpdateItemStatus(DocRef docRef, int16 itemDefID,
		int32 *curStatus, boxid boxID, bool8 *isLinked)
{
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(docRef, &docViewRef);
	return XTGetMasterPageUpdateItemStatus(docViewRef, itemDefID, curStatus,
			boxID, isLinked);
}

#undef scaleftoi
C_INLINE int16 XTAPI scaleftoi(Fixed fx)
{
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = kInvalDocViewRef;
	Fixed docScale = VIEW100;

	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
		XTGetViewDocScale(docViewRef, &docScale);
	}
	return XTViewScaleFToI(fx, docScale);
}

#undef rotmoveto
C_INLINE void XTAPI rotmoveto(Fixed fx,Fixed fy,const xform *mat)
{
	assert( FALSE && "rotmoveto" );
	return;

	//DocID docID = INVAL_DOCID;
	//DocViewRef docViewRef = kInvalDocViewRef;
	//Fixed docScale = VIEW100;

	//xtget_curdoc(&docID);
	//if (docID != INVAL_DOCID) {
	//	XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
	//	XTGetViewDocScale(docViewRef, &docScale);
	//	XTViewRotMoveTo(fx, fy, docScale, mat);
	//}
}

#undef getboxsprd
C_INLINE int16 XTAPI getboxsprd(boxid boxID)
{
	int16 spreadNum = INVALSPREAD;
	DocViewRef docViewRef = kInvalDocViewRef;

	XTGetDocViewRef(XTGetDocRefFromBox(boxID), &docViewRef);
	XTViewGetSpreadNumber(docViewRef, boxID, &spreadNum);
	return spreadNum;
}

#undef xtmousepage
C_INLINE XTError XTAPI xtmousepage(pagenumdata *returndata)
{
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = kInvalDocViewRef;
	XTError err = noErr;

	if (!returndata) {
		err = XTbaddataptr;
	}
	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
		err = XTAPIERRToXTError(XTViewMousePage(docViewRef, returndata));
	}
	return err;
}

#undef xtgetsnappedevent
C_INLINE XTError XTAPI xtgetsnappedevent(int16 mask, EventRecord *eventP,
		bool16 hsnap, bool16 vsnap, fixedpoint *offset, fixedpoint *size,
		fixedpoint *mousePos)
{
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = kInvalDocViewRef;
	XTError err = noErr;

	if ((!mousePos) || (!size) || (!offset)) {
		err = XTbaddataptr;
	}
	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
		err = XTAPIERRToXTError(XTViewSnapEvent(docViewRef, eventP, mask, 
				(bool8)hsnap, (bool8)vsnap, offset, size, mousePos));
	}
	return err;
}

#undef xtGetGuideByIndex
C_INLINE XTError XTAPI xtGetGuideByIndex(int16 pageseq, int16 guideType,
		int16 index, guidelinerec *guide, bool16 spreadguides)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return XTAPIERRToXTError(XTGetDocGuideByIndex(XTGetDocRefFromDocID(docID),
			pageseq, guideType, index, guide, spreadguides));
}

#undef xtAddGuide
C_INLINE int16 XTAPI xtAddGuide(int16 pageSeq, int16 guideType,
		const guidelinerec *guideLine, bool16 spreadGuides)
{
	DocID docID = INVAL_DOCID;
	int16 guideIndex = 0;
	int16 count = 0;
	uint8 isMasterShown = FALSE;
	DocRef docRef = INVAL_DOCREF;

	xtget_curdoc(&docID);
	if (docID != INVAL_DOCID) {
		docRef = XTGetDocRefFromDocID(docID);

		XTGetShowMaster(docRef, &isMasterShown);
		if (!isMasterShown) {
			spreadGuides
					? XTNumMasterSpreads(docRef, &count)
					: XTNumMasterPages(docRef, &count);
			pageSeq += count;
		}
		XTAddGuide(docRef, pageSeq, guideType, guideLine, spreadGuides,
				&guideIndex);
	}
	return guideIndex;
}

#undef XEGetStoryLock
C_INLINE bool8 XTAPI XEGetStoryLock(xehandle xeH)
{
	Fixed value = 0;
	XEGetStoryAttrib(xeH, S_LOCK,&value);
	return( (bool8)value );
}

#undef XESetStoryLock
C_INLINE bool8 XTAPI XESetStoryLock(xehandle xeH, bool16 newstate)
{
	return (XESetStoryAttrib(xeH, S_LOCK, newstate));
}

#undef getdoclocked
C_INLINE bool8 XTAPI getdoclocked(void)
{
	DocID docID = INVAL_DOCID;
	bool8 isLocked = FALSE;

	xtget_curdoc(&docID);

	if (docID != INVAL_DOCID) {
		DocRef docRef = INVAL_DOCREF;

		docRef = XTGetDocRefFromDocID(docID);
		if (docRef != INVAL_DOCREF) {
			LockableItem lockableItem;

			lockableItem.lockType = kLayoutLock;
			lockableItem.item.docRef = docRef;
			XTGetItemLockState(&lockableItem, &isLocked);
		}
	}

	return (isLocked);
}

#undef setdoclocked
C_INLINE void XTAPI setdoclocked(bool16 lockstate)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);

	if (docID != INVAL_DOCID) {
		DocRef docRef = INVAL_DOCREF;

		docRef = XTGetDocRefFromDocID(docID);
		if (docRef != INVAL_DOCREF) {
			LockableItem lockableItem;

			lockableItem.lockType = kLayoutLock;
			lockableItem.item.docRef = docRef;
			XTSetItemLockState(INVAL_XTID, &lockableItem, lockstate);
		}
	}
}

#undef piclocked
C_INLINE bool8 XTAPI piclocked(picboxid boxID)
{
	bool8 isLocked = FALSE;

	if (isapicture(boxID)) {
		PictureViewRef pictViewRef = kInvalPictureViewRef;

		XTGetPictureView(boxID, &pictViewRef);
		if(pictViewRef != kInvalPictureViewRef) {
			LockableItem lockableItem;

			lockableItem.lockType = kPictureFormattingLock;
			lockableItem.item.pictViewRef = pictViewRef;
			XTGetItemLockState(&lockableItem, &isLocked);
		}
	}

	return (isLocked);
}

#undef setpiclock
C_INLINE bool8 XTAPI setpiclock(picboxid boxID, bool16 newlock)
{
	bool8 isLocked = FALSE;

	if (boxID == NULL) {
		xtget_curbox(&boxID);
	}
	if (isapicture(boxID)) {
		PictureViewRef pictViewRef = kInvalPictureViewRef;

		XTGetPictureView(boxID, &pictViewRef);
		if(pictViewRef != kInvalPictureViewRef) {
			LockableItem lockableItem;

			lockableItem.lockType = kPictureFormattingLock;
			lockableItem.item.pictViewRef = pictViewRef;
			XTSetItemLockState(INVAL_XTID, &lockableItem, newlock);
			isLocked = newlock;
		}
	}

	return isLocked;
}

#undef XTGetDocLocked
C_INLINE APIERR XTAPI XTGetDocLocked(DocRef docRef, bool8 *docLockedP)
{
	LockableItem lockableItem;

	API_ENTRY

	PARAM_CHECK(2, docLockedP != NULL);
	*docLockedP = FALSE;

	PARAM_CHECK(1, docRef != INVAL_DOCREF && docRef != GLOBAL_DOCREF);

	lockableItem.lockType = kLayoutLock;
	lockableItem.item.docRef = docRef;
	XTGetItemLockState(&lockableItem, docLockedP);

	API_EXIT;
}

#undef xtset_boxalocked
C_INLINE XTError XTAPI xtset_boxalocked(boxid boxID, bool8 oldLocked)
{
	if (boxID != 0) {
		LockableItem lockableItem;

		beginUndo(UNDOITEM, 0, boxID);
		lockableItem.item.boxID = boxID;

		lockableItem.lockType = kSizeLock;
		XTSetItemLockState(INVAL_XTID, &lockableItem, oldLocked);

		lockableItem.lockType = kLocationLock;
		XTSetItemLockState(INVAL_XTID, &lockableItem, oldLocked);

		lockableItem.lockType = kDeleteLock;
		XTSetItemLockState(INVAL_XTID, &lockableItem, oldLocked);
		endUndo();
	}

	return noErr;
}

#undef xtget_boxalocked
C_INLINE XTError XTAPI xtget_boxalocked(boxid boxID, bool8 *newLocked)
{
	if (newLocked == NULL) {
		return XTbaddataptr;
	}
	*newLocked = FALSE;
	if (boxID != 0) {
		LockableItem lockableItem;

		lockableItem.item.boxID = boxID;
		lockableItem.lockType = kSizeLock;
		XTGetItemLockState(&lockableItem, newLocked);

		if (*newLocked != TRUE) {
			lockableItem.lockType = kDeleteLock;
			XTGetItemLockState(&lockableItem, newLocked);
		}
	}
	return noErr;
}

#undef xtaddstyle
C_INLINE bool8 XTAPI xtaddstyle(int16 *index, Handle theStyleH, bool8 pStyle)
{
	assert( FALSE && "xtaddstyle" );
	return( FALSE );

	//bool8 isStyleAdded = FALSE;
	//DocID docID = INVAL_DOCID;
	//AssetUID assetUID = INVAL_ASSETUID;
	//DocRef docRef = INVAL_DOCREF;
	//AssetSpaceRef assetSpaceRef = INVAL_ASSETSPACEREF;

	//if (theStyleH == NULL) {
	//	return (FALSE);
	//}
	//xtget_curdoc(&docID);
	//docRef = XTGetDocRefFromDocID(docID);
	//assetSpaceRef = XTGetDocAssetSpace(docRef);

	//if (assetSpaceRef == INVAL_ASSETSPACEREF) {
	//	assetSpaceRef = XTCreateDefaultAssetSpace();
	//}

	//if (pStyle) {
	//	isStyleAdded = (ERR_SUCCESS == XTNewParaStyleAsset(assetSpaceRef,
	//			(parastylerec *)*theStyleH, &assetUID)) ? TRUE : FALSE;
	//	*index = (int16)(XTGetAssetCount(assetSpaceRef, kAssetParaStyle)-1);
	//}
	//else {
	//	isStyleAdded = (ERR_SUCCESS == XTNewCharStyleAsset(assetSpaceRef,
	//			(charstylerec *)*theStyleH, &assetUID)) ? TRUE : FALSE;
	//	*index = (int16)(XTGetAssetCount(assetSpaceRef,	kAssetCharStyle)-1);
	//}

	//if (docRef == GLOBAL_DOCREF) {
	//	if (isStyleAdded) {
	//		XTUpdateAssetPrefs(assetSpaceRef, pStyle ? kAssetParaStyle
	//				: kAssetCharStyle);
	//	}
	//	XTDeleteAssetSpace(assetSpaceRef);
	//}

	//return (isStyleAdded);
}

#undef xtaddcharstyle
C_INLINE bool8 XTAPI xtaddcharstyle(int16 *index, cstylehandle theCharStyleH)
{
	return (xtaddstyle(index, (Handle)theCharStyleH, FALSE));
}

#undef xtaddparastyle
C_INLINE bool8 XTAPI xtaddparastyle(int16 *index,pstylehandle theParaStyleH)
{
	return (xtaddstyle(index, (Handle)theParaStyleH, TRUE));
}


#undef delstylebyindex
C_INLINE bool8 XTAPI delstylebyindex(int16 index, bool8 pStyle)
{
	bool8 isStyleDeleted = FALSE;
	DocID docID = INVAL_DOCID;
	AssetUID assetUID = INVAL_ASSETUID;
	DocRef docRef = INVAL_DOCREF;
	AssetSpaceRef assetSpaceRef = INVAL_ASSETSPACEREF;

	xtget_curdoc(&docID);
	docRef = XTGetDocRefFromDocID(docID);
	assetSpaceRef = XTGetDocAssetSpace(docRef);

	if (assetSpaceRef == INVAL_ASSETSPACEREF) {
		assetSpaceRef = XTCreateDefaultAssetSpace();
	}

	XTGetAsset(assetSpaceRef, pStyle ? kAssetParaStyle : kAssetCharStyle,
			index, &assetUID);
	isStyleDeleted = (ERR_SUCCESS == XTDeleteAsset(assetSpaceRef, pStyle
			? kAssetParaStyle : kAssetCharStyle, assetUID, INVAL_ASSETUID)) ? TRUE : FALSE;

	if (docRef == GLOBAL_DOCREF) {
		if (isStyleDeleted) {
			XTUpdateAssetPrefs(assetSpaceRef, pStyle ? kAssetParaStyle
					: kAssetCharStyle);
		}
		XTDeleteAssetSpace(assetSpaceRef);
	}

	return (isStyleDeleted);
}

#undef delcharstylebyindex
C_INLINE bool8 XTAPI delcharstylebyindex(int16 index)
{
	return (delstylebyindex(index, FALSE));
}

#undef delparastylebyindex
C_INLINE bool8 XTAPI delparastylebyindex(int16 index)
{
	return (delstylebyindex(index, TRUE));
}

#undef XTGetParaStyleByIndex
C_INLINE APIERR XTAPI XTGetParaStyleByIndex(DocRef docRef, int16 index,
		parastylerec *outStyleInfoP, bool8 *styleFoundP)
{
	assert( FALSE && "XTGetParaStyleByIndex" );
	return( (APIERR) -1);

	//APIERR err = ERR_FAILURE;
	//AssetUID assetUID = INVAL_ASSETUID;
	//AssetSpaceRef assetSpaceRef = INVAL_ASSETSPACEREF;

	//*styleFoundP = FALSE;
	//assetSpaceRef = XTGetDocAssetSpace(docRef);

	//if (assetSpaceRef == INVAL_ASSETSPACEREF) {
	//	assetSpaceRef = XTCreateDefaultAssetSpace();
	//}

	//if (outStyleInfoP != NULL) {
	//	zerodata(outStyleInfoP, sizeof(parastylerec));
	//}

	//XTGetAsset(assetSpaceRef, kAssetParaStyle, index, &assetUID);
	//err = XTGetParaStyleInfo(assetSpaceRef, assetUID, outStyleInfoP);
	//
	//if (err == ERR_SUCCESS) {
	//	*styleFoundP = TRUE;
	//}
	//if (outStyleInfoP->tab.tabs != NULL) {
	//	HandToHand((Handle *)&(outStyleInfoP)->tab.tabs);
	//}
	//if (docRef == GLOBAL_DOCREF) {
	//	XTDeleteAssetSpace(assetSpaceRef);
	//}

	//return (err);
}


#undef XTGetCharStyleByIndex
C_INLINE APIERR XTAPI XTGetCharStyleByIndex(DocRef docRef, int16 index,
		charstylerec *outStyleInfoP, bool8 *styleFoundP)
{
	assert( FALSE && "XTGetCharStyleByIndex" );
	return( (APIERR) -1 );

	//APIERR err = ERR_FAILURE;
	//AssetUID assetUID = INVAL_ASSETUID;
	//AssetSpaceRef assetSpaceRef = INVAL_ASSETSPACEREF;

	//*styleFoundP = FALSE;
	//assetSpaceRef = XTGetDocAssetSpace(docRef);

	//if (assetSpaceRef == INVAL_ASSETSPACEREF) {
	//	assetSpaceRef = XTCreateDefaultAssetSpace();
	//}

	//if (outStyleInfoP != NULL) {
	//	zerodata(outStyleInfoP, sizeof(charstylerec));
	//}

	//XTGetAsset(assetSpaceRef, kAssetCharStyle, index, &assetUID);
	//err = XTGetCharStyleInfo(assetSpaceRef, assetUID, outStyleInfoP);

	//if (err == ERR_SUCCESS) {
	//	*styleFoundP = TRUE;
	//}
	//if (docRef == GLOBAL_DOCREF) {
	//	XTDeleteAssetSpace(assetSpaceRef);
	//}

	//return (err);
}

#undef getstylebyindex2
C_INLINE bool8 XTAPI getstylebyindex2(int16 index, styleinfo *theStyle,
		bool8 pStyle)
{
	bool8 styleFound = FALSE;
	DocID docID = INVAL_DOCID;
	DocRef docRef = INVAL_DOCREF;

	xtget_curdoc(&docID);
	docRef = XTGetDocRefFromDocID(docID);

	if (pStyle) {
		parastylerec paraStyle;

		XTGetParaStyleByIndex(docRef, index, (theStyle != NULL) ? &paraStyle
				: NULL, &styleFound);
		if (theStyle != NULL) {
			/* TODO KSharma March 31, 2003 Do we want to give entire parastyle
			** info ?
			*/
			BlockMove(&paraStyle, theStyle, sizeof(parastylerec));
		}
	}
	else {
		charstylerec txtStyle;

		XTGetCharStyleByIndex(docRef, index, (theStyle != NULL) ? &txtStyle
				: NULL, &styleFound);
		if (theStyle != NULL) {
			/* TODO KSharma March 31, 2003 Do we want to give entire textstyle
			** info ?
			*/
			BlockMove(&txtStyle, theStyle, sizeof(charstylerec));
		}
	}

	return (styleFound);
}

#undef getcharstylebyindex
C_INLINE bool8 XTAPI getcharstylebyindex(int16 index, charstylerec *thecharstyle)
{
	return (getstylebyindex2(index, (styleinfo *)thecharstyle, FALSE));
}

#undef getparastylebyindex
C_INLINE bool8 XTAPI getparastylebyindex(int16 index,
		parastylerec *theparastyle)
{
	return (getstylebyindex2(index, (styleinfo *)theparastyle, TRUE));
}

#undef setstylebyindex
C_INLINE bool8 XTAPI setstylebyindex(int16 index, styleinfo *theStyle,
		bool8 pStyle, bool8 updateDoc)
{
	assert( FALSE && "setstylebyindex" );
	return( FALSE );

	//bool8 success = FALSE;
	//DocID docID = INVAL_DOCID;
	//AssetUID assetUID = INVAL_ASSETUID;
	//DocRef docRef = INVAL_DOCREF;
	//AssetSpaceRef assetSpaceRef = INVAL_ASSETSPACEREF;
	//
	///* TODO vmahajan 16-Sep-2004, This variable updateDoc is not used,
	//** it will removed in future.
	//*/
	//updateDoc = FALSE;
	//xtget_curdoc(&docID);
	//docRef = XTGetDocRefFromDocID(docID);
	//assetSpaceRef = XTGetDocAssetSpace(docRef);

	//if (assetSpaceRef == INVAL_ASSETSPACEREF) {
	//	assetSpaceRef = XTCreateDefaultAssetSpace();
	//}

	//XTGetAsset(assetSpaceRef, pStyle ? kAssetParaStyle : kAssetCharStyle, index,
	//		&assetUID);
	//if (pStyle) {
	//	success = (ERR_SUCCESS == XTSetParaStyleInfo(assetSpaceRef, assetUID,
	//			(parastylerec *)theStyle)) ? TRUE : FALSE;
	//}
	//else {
	//	success = (ERR_SUCCESS == XTSetCharStyleInfo(assetSpaceRef, assetUID,
	//			(charstylerec *)theStyle)) ? TRUE : FALSE;
	//}

	//if (docRef == GLOBAL_DOCREF) {
	//	if (success) {
	//		XTUpdateAssetPrefs(assetSpaceRef, pStyle ? kAssetParaStyle
	//				: kAssetCharStyle);
	//	}
	//	XTDeleteAssetSpace(assetSpaceRef);
	//}

	//return (success);
}

#undef setparastylebyindex
C_INLINE bool8 XTAPI setparastylebyindex(int16 index,
		parastylerec *theParaStyle, bool8 callxecalc)
{
	return (setstylebyindex(index, (styleinfo *)theParaStyle, TRUE, callxecalc));
}


#undef setcharstylebyindex
C_INLINE bool8 XTAPI setcharstylebyindex(int16 index,
		charstylerec *theCharStyle, bool8 callxecalc)
{
	return (setstylebyindex(index, (styleinfo *)theCharStyle, FALSE,
			callxecalc));
}


#undef xtaddlist
C_INLINE XTError XTAPI xtaddlist(listsrec *newList, int16 *index)
{
	assert( FALSE && "xtaddlist" );
	return( (XTError) -1 );

	//XTError retval = XToperationfailed;
	//DocID docID = INVAL_DOCID;
	//AssetUID assetUID = INVAL_ASSETUID;
	//DocRef docRef = INVAL_DOCREF;
	//AssetSpaceRef assetSpaceRef = INVAL_ASSETSPACEREF;

	//xtget_curdoc(&docID);
	//docRef = XTGetDocRefFromDocID(docID);
	//assetSpaceRef = XTGetDocAssetSpace(docRef);

	//if (assetSpaceRef == INVAL_ASSETSPACEREF) {
	//	assetSpaceRef = XTCreateDefaultAssetSpace();
	//}

	//retval = (XTAPIERRToXTError(XTNewListsAsset(assetSpaceRef, newList,
	//		&assetUID)));
	//*index = (int16)(XTGetAssetCount(assetSpaceRef,	kAssetLists)-1);

	//if (docRef == GLOBAL_DOCREF) {
	//	if (noErr == retval) {
	//		XTUpdateAssetPrefs(assetSpaceRef, kAssetLists);
	//	}
	//	XTDeleteAssetSpace(assetSpaceRef);
	//}

	//return (retval);
}

#undef xtdellistbyindex
C_INLINE XTError XTAPI xtdellistbyindex(int16 index)
{
	XTError retval = XToperationfailed;
	DocID docID = INVAL_DOCID;
	AssetUID assetUID = INVAL_ASSETUID;
	DocRef docRef = INVAL_DOCREF;
	AssetSpaceRef assetSpaceRef = INVAL_ASSETSPACEREF;

	xtget_curdoc(&docID);
	docRef = XTGetDocRefFromDocID(docID);
	assetSpaceRef = XTGetDocAssetSpace(docRef);

	if (assetSpaceRef == INVAL_ASSETSPACEREF) {
		assetSpaceRef = XTCreateDefaultAssetSpace();
	}

	XTGetAsset(assetSpaceRef, kAssetLists, index, &assetUID);
	retval = (XTAPIERRToXTError(XTDeleteAsset(assetSpaceRef, kAssetLists,
			assetUID, INVAL_ASSETUID)));

	if (docRef == GLOBAL_DOCREF) {
		if (noErr == retval) {
			XTUpdateAssetPrefs(assetSpaceRef, kAssetLists);
		}
		XTDeleteAssetSpace(assetSpaceRef);
	}

	return (retval);
}

#undef xtsetlistbyindex
C_INLINE XTError XTAPI xtsetlistbyindex(int16 index, listsrec *theList)
{
	assert( FALSE && "xtsetlistbyindex" );
	return( (XTError) -1 );

	//XTError retval = XToperationfailed;
	//DocID docID = INVAL_DOCID;
	//AssetUID assetUID = INVAL_ASSETUID;
	//DocRef docRef = INVAL_DOCREF;
	//AssetSpaceRef assetSpaceRef = INVAL_ASSETSPACEREF;

	//xtget_curdoc(&docID);
	//docRef = XTGetDocRefFromDocID(docID);
	//assetSpaceRef = XTGetDocAssetSpace(docRef);

	//if (assetSpaceRef == INVAL_ASSETSPACEREF) {
	//	assetSpaceRef = XTCreateDefaultAssetSpace();
	//}

	//XTGetAsset(assetSpaceRef, kAssetLists, index, &assetUID);
	//retval = (XTAPIERRToXTError(XTSetListsInfo(assetSpaceRef, assetUID,
	//		theList)));	

	//if (docRef == GLOBAL_DOCREF) {
	//	if (noErr == retval) {
	//		XTUpdateAssetPrefs(assetSpaceRef, kAssetLists);
	//	}
	//	XTDeleteAssetSpace(assetSpaceRef);
	//}

	//return (retval);
}

#undef xtgetlistbyindex
C_INLINE XTError XTAPI xtgetlistbyindex(int16 index, listsrec *theList)
{
	assert( FALSE && "xtgetlistbyindex" );
	return( (XTError) -1 );

	//XTError retval = XToperationfailed;
	//DocID docID = INVAL_DOCID;
	//AssetUID assetUID = INVAL_ASSETUID;
	//DocRef docRef = INVAL_DOCREF;
	//AssetSpaceRef assetSpaceRef = INVAL_ASSETSPACEREF;

	//xtget_curdoc(&docID);
	//docRef = XTGetDocRefFromDocID(docID);
	//assetSpaceRef = XTGetDocAssetSpace(docRef);

	//if (assetSpaceRef == INVAL_ASSETSPACEREF) {
	//	assetSpaceRef = XTCreateDefaultAssetSpace();
	//}

	//XTGetAsset(assetSpaceRef, kAssetLists, index, &assetUID);
	//retval = (XTAPIERRToXTError(XTGetListsInfo(assetSpaceRef, assetUID,
	//		theList)));

	//if (docRef == GLOBAL_DOCREF) {
	//	XTDeleteAssetSpace(assetSpaceRef);
	//}

	//return (retval);
}

#undef addhandj
C_INLINE bool8 XTAPI addhandj(int16 *index, Handle newHJRecH)
{
	assert( FALSE && "addhandj" );
	return( FALSE );

	//bool8 isHJAdded = FALSE;
	//DocID docID = INVAL_DOCID;
	//AssetUID assetUID = INVAL_ASSETUID;
	//DocRef docRef = INVAL_DOCREF;
	//AssetSpaceRef assetSpaceRef = INVAL_ASSETSPACEREF;

	//if (newHJRecH == NULL) {
	//	return (FALSE);
	//}
	//xtget_curdoc(&docID);
	//docRef = XTGetDocRefFromDocID(docID);
	//assetSpaceRef = XTGetDocAssetSpace(docRef);

	//if (assetSpaceRef == INVAL_ASSETSPACEREF) {
	//	assetSpaceRef = XTCreateDefaultAssetSpace();
	//}

	//isHJAdded = (ERR_SUCCESS == XTNewHAndJAsset(assetSpaceRef,
	//		&(**(handjrec **)newHJRecH), &assetUID)) ? TRUE : FALSE;
	//*index = (int16)(XTGetAssetCount(assetSpaceRef,	kAssetHAndJ)-1);

	//if (docRef == GLOBAL_DOCREF) {
	//	if (isHJAdded) {
	//		XTUpdateAssetPrefs(assetSpaceRef, kAssetHAndJ);
	//	}
	//	XTDeleteAssetSpace(assetSpaceRef);
	//}

	//return (isHJAdded);
}

#undef delhandjbyindex
C_INLINE bool8 XTAPI delhandjbyindex(int16 index)
{
	bool8 isHJDeleted = FALSE;
	DocID docID = INVAL_DOCID;
	AssetUID assetUID = INVAL_ASSETUID;
	DocRef docRef = INVAL_DOCREF;
	AssetSpaceRef assetSpaceRef = INVAL_ASSETSPACEREF;

	xtget_curdoc(&docID);
	docRef = XTGetDocRefFromDocID(docID);
	assetSpaceRef = XTGetDocAssetSpace(docRef);

	if (assetSpaceRef == INVAL_ASSETSPACEREF) {
		assetSpaceRef = XTCreateDefaultAssetSpace();
	}

	XTGetAsset(assetSpaceRef, kAssetHAndJ, index, &assetUID);
	isHJDeleted = (XTDeleteAsset(assetSpaceRef, kAssetHAndJ, assetUID, INVAL_ASSETUID)
			== ERR_SUCCESS	? TRUE : FALSE);

	if (docRef == GLOBAL_DOCREF) {
		if (isHJDeleted) {
			XTUpdateAssetPrefs(assetSpaceRef, kAssetHAndJ);
		}
		XTDeleteAssetSpace(assetSpaceRef);
	}

	return (isHJDeleted);
}

#undef gethandjptrbyindex
C_INLINE bool8 XTAPI gethandjptrbyindex(int16 index,
		handjrec **dstHAndJH, handjhandle srcHAndJH)
{
	bool8 isHAndJFound = FALSE;

	if (dstHAndJH != NULL) {
		if (srcHAndJH != NULL) {
			size_t numHAndJ = GetHandleSize((Handle)srcHAndJH) / SIZEOFHANDJREC;
	
			if (index >= 0 && index < numHAndJ) {
				*dstHAndJH = &((handjrec *)*srcHAndJH)[index];
				isHAndJFound = (*dstHAndJH != NULL) ? TRUE : FALSE;
			}
		}
	}

	return (isHAndJFound);
}

#undef XTCountCharStyles
C_INLINE APIERR XTAPI XTCountCharStyles(DocRef docRef, int16 *countP)
{
	APIERR apiErr = ERR_FAILURE;
	AssetSpaceRef assetSpaceRef = INVAL_ASSETSPACEREF;

	assetSpaceRef = XTGetDocAssetSpace(docRef);

	if (assetSpaceRef == INVAL_ASSETSPACEREF) {
		assetSpaceRef = XTCreateDefaultAssetSpace();
	}

	if (countP != NULL) {
		*countP = (int16)(XTGetAssetCount(assetSpaceRef, kAssetCharStyle));
		apiErr = ERR_SUCCESS;
	}
	if (docRef == GLOBAL_DOCREF) {
		XTDeleteAssetSpace(assetSpaceRef);
	}

	return (apiErr);
}

#undef XTCountParaStyles
C_INLINE APIERR XTAPI XTCountParaStyles(DocRef docRef, int16 *countP)
{
	APIERR apiErr = ERR_FAILURE;
	AssetSpaceRef assetSpaceRef = INVAL_ASSETSPACEREF;

	assetSpaceRef = XTGetDocAssetSpace(docRef);

	if (assetSpaceRef == INVAL_ASSETSPACEREF) {
		assetSpaceRef = XTCreateDefaultAssetSpace();
	}

	if (countP != NULL) {
		*countP = (int16)(XTGetAssetCount(assetSpaceRef, kAssetParaStyle));
		apiErr = ERR_SUCCESS;
	}
	if (docRef == GLOBAL_DOCREF) {
		XTDeleteAssetSpace(assetSpaceRef);
	}

	return (apiErr);
}

#undef countparastyles
C_INLINE int16 XTAPI countparastyles(void)
{
	int16 count;
	DocID docID = INVAL_DOCID;
	DocRef docRef = INVAL_DOCREF;

	xtget_curdoc(&docID);
	docRef = XTGetDocRefFromDocID(docID);

	XTCountParaStyles(docRef, &count);

	return (count);
}
 
#undef countcharstyles
C_INLINE int16 XTAPI countcharstyles(void)
{
	int16 count;
	DocID docID = INVAL_DOCID;
	DocRef docRef = INVAL_DOCREF;

	xtget_curdoc(&docID);
	docRef = XTGetDocRefFromDocID(docID);

	XTCountCharStyles(docRef, &count);

	return (count);
}

#undef countstyles
C_INLINE int16 XTAPI countstyles(bool8 pStyle)
{
	int16 count;
	DocID docID = INVAL_DOCID;
	DocRef docRef = INVAL_DOCREF;

	xtget_curdoc(&docID);
	docRef = XTGetDocRefFromDocID(docID);
	pStyle ? XTCountParaStyles(docRef, &count) : XTCountCharStyles(docRef, &count);

	return (count);
}

#undef xtget_countstyles
C_INLINE XTError XTAPI xtget_countstyles(int16 *xtd_value)
{
	DocID docID = INVAL_DOCID;
	DocRef docRef = INVAL_DOCREF;

	xtget_curdoc(&docID);
	docRef = XTGetDocRefFromDocID(docID);
	return (XTAPIERRToXTError(XTCountParaStyles(docRef, xtd_value)));
}

#undef deleteanchbox
C_INLINE void deleteanchbox(boxid boxID)
{
	XTDeleteAnchBox(boxID);
}

#undef newbox
C_INLINE bool8 newbox(void)
{
	assert( FALSE && "newbox" );
	return( FALSE );

	//return ((XTNewBox() == ERR_SUCCESS) ? TRUE : FALSE);
}

#undef xtinstallanchbox
C_INLINE boxid XTAPI xtinstallanchbox(xtboxptr dataptr, boxid theBoxID,
		int32 offset)
{
	boxid boxID = 0L;

	XTInstallAnchBox(dataptr, theBoxID, offset, &boxID);

	return boxID;
}

#undef xtget_docsnapdist
C_INLINE XTError XTAPI xtget_docsnapdist(int16 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return (XTAPIERRToXTError(XTGetDocSnapDist(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docsnapdist
C_INLINE XTError XTAPI xtset_docsnapdist(int16 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return (XTAPIERRToXTError(XTSetDocSnapDist(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_docautoimport
C_INLINE XTError XTAPI xtget_docautoimport(uint8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return (XTAPIERRToXTError(XTGetDocAutoImport(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_docautoimport
C_INLINE XTError XTAPI xtset_docautoimport(uint8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return (XTAPIERRToXTError(XTSetDocAutoImport(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtCopyBoxContent
C_INLINE APIERR xtCopyBoxContent(boxid sourceBoxP, boxid destBoxP)
{
	return (XTCopyBoxContent(sourceBoxP, destBoxP));
}

#undef xtget_pboxcontrast
C_INLINE XTError xtget_pboxcontrast(boxid theboxid, uint8 *newContrast)
{
	return (XTAPIERRToXTError(XTGetPictureContrast(theboxid, newContrast)));
}

#undef xtset_pboxcontrast
C_INLINE XTError xtset_pboxcontrast(boxid theboxid, uint8 newContrast)
{
	return (XTAPIERRToXTError(XTSetPictureContrast(theboxid, newContrast)));
}



#undef xtset_doclockguides
C_INLINE XTError XTAPI xtset_doclockguides(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return (XTAPIERRToXTError(XTSetDocLockGuides(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_doclockguides
C_INLINE XTError XTAPI xtget_doclockguides(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return (XTAPIERRToXTError(XTGetDocLockGuides(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_usedocsettings
C_INLINE XTError XTAPI xtset_usedocsettings(bool8 xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return (XTAPIERRToXTError(XTSetUseDocsettings(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtget_usedocsettings
C_INLINE XTError XTAPI xtget_usedocsettings(bool8 *xtd_value)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return (XTAPIERRToXTError(XTGetUseDocsettings(XTGetDocRefFromDocID(docID),
			xtd_value)));
}

#undef xtset_boxanchbitsaligntop
C_INLINE XTError XTAPI xtset_boxanchbitsaligntop(boxid boxID, bool8 newVal,
		Fixed offsetValue)
{
	return (XTAPIERRToXTError(XTSetBoxAnchBitsAlignTop(
			boxID, newVal, offsetValue)));
}

#undef xtget_boxanchbitsaligntop
C_INLINE XTError XTAPI xtget_boxanchbitsaligntop(boxid boxID, bool8 *newValP,
		Fixed *offsetValueP)
{
	return (XTAPIERRToXTError(XTGetBoxAnchBitsAlignTop(
			boxID, newValP, offsetValueP)));
}

#undef xtset_boxanchbitsnooutline
C_INLINE XTError XTAPI xtset_boxanchbitsnooutline(boxid boxID,
		bool8 newVal)
{
	return (XTAPIERRToXTError(XTSetBoxAnchBitsNoOutline(boxID, newVal)));
}

#undef xtget_boxanchbitsnooutline
C_INLINE XTError XTAPI xtget_boxanchbitsnooutline(boxid boxID,
		bool8 *newValP)
{
	return (XTAPIERRToXTError(XTGetBoxAnchBitsNoOutline(boxID, newValP)));
}

#undef xtset_boxanchbitscantobstruct
C_INLINE XTError XTAPI xtset_boxanchbitscantobstruct(boxid boxID,
		bool8 newVal)
{
	return (XTAPIERRToXTError(XTSetBoxAnchBitsCantObstruct(boxID, newVal)));
}

#undef xtget_boxanchbitscantobstruct
C_INLINE XTError XTAPI xtget_boxanchbitscantobstruct(boxid boxID,
		bool8 *newValP)
{
	return (XTAPIERRToXTError(XTGetBoxAnchBitsCantObstruct(boxID, newValP)));
}

#undef xtset_boxanchbitsflippedlr
C_INLINE XTError XTAPI xtset_boxanchbitsflippedlr(boxid boxID, bool8 newVal)
{
	return (XTAPIERRToXTError(XTSetBoxAnchBitsFlippedLR(boxID, newVal)));
}

#undef xtget_boxanchbitsflippedlr
C_INLINE XTError XTAPI xtget_boxanchbitsflippedlr(boxid boxID, bool8 *newValP)
{
	return (XTAPIERRToXTError(XTGetBoxAnchBitsFlippedLR(boxID, newValP)));
}

#undef xtset_boxanchbitsflippedtb
C_INLINE XTError XTAPI xtset_boxanchbitsflippedtb(boxid boxID, bool8 newVal)
{
	return (XTAPIERRToXTError(XTSetBoxAnchBitsFlippedTB(boxID, newVal)));
}

#undef xtget_boxanchbitsflippedtb
C_INLINE XTError XTAPI xtget_boxanchbitsflippedtb(boxid boxID, bool8 *newValP)
{
	return (XTAPIERRToXTError(XTGetBoxAnchBitsFlippedTB(boxID, newValP)));
}

#undef XTSetPBoxPicType
C_INLINE APIERR XTAPI XTSetPBoxPicType(boxid theboxid, uint8 newValue)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);

	XTSetPictureViewType(pViewRef, newValue);

	API_EXIT;
}

#undef XTGetPBoxPicType
C_INLINE APIERR XTAPI XTGetPBoxPicType(boxid theboxid, uint8 *newValueP)
{
	PictureViewRef pViewRef = kInvalPictureViewRef;

	API_ENTRY;

	XTGetPictureView(theboxid, &pViewRef);

	PARAM_CHECK(1, pViewRef != kInvalPictureViewRef);
	PARAM_CHECK(2, newValueP != NULL);

	XTGetPictureViewType(pViewRef, newValueP);

	API_EXIT;
}


#undef xtset_pboxpictype
C_INLINE XTError XTAPI xtset_pboxpictype(boxid boxID, uint8 newVal)
{
	return (XTAPIERRToXTError(XTSetPBoxPicType(boxID, newVal)));
}

#undef xtget_pboxpictype
C_INLINE XTError XTAPI xtget_pboxpictype(boxid boxID, uint8 *newValP)
{
	return (XTAPIERRToXTError(XTGetPBoxPicType(boxID, newValP)));
}

#undef xtset_tboxbcapheight
C_INLINE XTError XTAPI xtset_tboxbcapheight(boxid boxID, bool8 newVal)
{
	return (XTAPIERRToXTError(XTSetTBoxBCapHeight(boxID, newVal)));
}

#undef xtget_tboxbcapheight
C_INLINE XTError XTAPI xtget_tboxbcapheight(boxid boxID, bool8 *newValP)
{
	return (XTAPIERRToXTError(XTGetTBoxBCapHeight(boxID, newValP)));
}

#undef xtset_tboxbaccentcap
C_INLINE XTError XTAPI xtset_tboxbaccentcap(boxid boxID, bool8 newVal)
{
	return (XTAPIERRToXTError(XTSetTBoxBAccentCap(boxID, newVal)));
}

#undef xtget_tboxbaccentcap
C_INLINE XTError XTAPI xtget_tboxbaccentcap(boxid boxID, bool8 *newValP)
{
	return (XTAPIERRToXTError(XTGetTBoxBAccentCap(boxID, newValP)));
}

#undef xtset_tboxbrunaroundsides
C_INLINE XTError XTAPI xtset_tboxbrunaroundsides(boxid boxID, bool8 newVal)
{
	return (XTAPIERRToXTError(XTSetTBoxBRunaroundSides(boxID, newVal)));
}

#undef xtget_tboxbrunaroundsides
C_INLINE XTError XTAPI xtget_tboxbrunaroundsides(boxid boxID, bool8 *newValP)
{
	return (XTAPIERRToXTError(XTGetTBoxBRunaroundSides(boxID, newValP)));
}

#undef xtset_tboxbnowrap
C_INLINE XTError XTAPI xtset_tboxbnowrap(boxid boxID, bool8 newVal)
{
	return (XTAPIERRToXTError(XTSetTBoxBNoWrap(boxID, newVal)));
}

#undef xtget_tboxbnowrap
C_INLINE XTError XTAPI xtget_tboxbnowrap(boxid boxID, bool8 *newValP)
{
	return (XTAPIERRToXTError(XTGetTBoxBNoWrap(boxID, newValP)));
}

#undef xtset_tboxgutter
C_INLINE XTError XTAPI xtset_tboxgutter(boxid boxID, Fixed newVal)
{
	return (XTAPIERRToXTError(XTSetTBoxGutter(boxID, newVal)));
}

#undef xtget_tboxgutter
C_INLINE XTError XTAPI xtget_tboxgutter(boxid boxID, Fixed *newValP)
{
	return (XTAPIERRToXTError(XTGetTBoxGutter(boxID, newValP)));
}

#undef xtset_tboxtextinset
C_INLINE XTError XTAPI xtset_tboxtextinset(boxid boxID, Fixed newVal[4])
{
	return (XTAPIERRToXTError(XTSetTBoxTextInset(boxID, newVal)));
}

#undef xtget_tboxtextinset
C_INLINE XTError XTAPI xtget_tboxtextinset(boxid boxID, Fixed *newValP[4])
{
	return (XTAPIERRToXTError(XTGetTBoxTextInset(boxID, newValP)));
}

#undef xtset_tboxtextangle
C_INLINE XTError XTAPI xtset_tboxtextangle(boxid boxID, Fixed newVal)
{
	return (XTAPIERRToXTError(XTSetTBoxTextAngle(boxID, newVal)));
}

#undef xtget_tboxtextangle
C_INLINE XTError XTAPI xtget_tboxtextangle(boxid boxID, Fixed *newValP)
{
	return (XTAPIERRToXTError(XTGetTBoxTextAngle(boxID, newValP)));
}

#undef xtset_tboxtextskew
C_INLINE XTError XTAPI xtset_tboxtextskew(boxid boxID, Fixed newVal)
{
	return (XTAPIERRToXTError(XTSetTBoxTextSkew(boxID, newVal)));
}

#undef xtget_tboxtextskew
C_INLINE XTError XTAPI xtget_tboxtextskew(boxid boxID, Fixed *newValP)
{
	return (XTAPIERRToXTError(XTGetTBoxTextSkew(boxID, newValP)));
}

#undef xtset_tboxcolcount
C_INLINE XTError XTAPI xtset_tboxcolcount(boxid boxID, uint8 newVal)
{
	return (XTAPIERRToXTError(XTSetTBoxColCount(boxID, newVal)));
}

#undef xtget_tboxcolcount
C_INLINE XTError XTAPI xtget_tboxcolcount(boxid boxID, uint8 *newValP)
{
	return (XTAPIERRToXTError(XTGetTBoxColCount(boxID, newValP)));
}

#undef xtset_tboxtextalign
C_INLINE XTError XTAPI xtset_tboxtextalign(boxid boxID, uint8 newVal)
{
	return (XTAPIERRToXTError(XTSetTBoxTextAlign(boxID, newVal)));
}

#undef xtget_tboxtextalign
C_INLINE XTError XTAPI xtget_tboxtextalign(boxid boxID, uint8 *newValP)
{
	return (XTAPIERRToXTError(XTGetTBoxTextAlign(boxID, newValP)));
}

#undef xtset_tboxvjustamt
C_INLINE XTError XTAPI xtset_tboxvjustamt(boxid boxID, Fixed newVal)
{
	return (XTAPIERRToXTError(XTSetTBoxVJustAmt(boxID, newVal)));
}

#undef xtget_tboxvjustamt
C_INLINE XTError XTAPI xtget_tboxvjustamt(boxid boxID, Fixed *newValP)
{
	return (XTAPIERRToXTError(XTGetTBoxVJustAmt(boxID, newValP)));
}

#undef xtset_tboxfirstbaseline
C_INLINE XTError XTAPI xtset_tboxfirstbaseline(boxid boxID, Fixed newVal)
{
	return (XTAPIERRToXTError(XTSetTBoxFirstBaseLine(boxID, newVal)));
}

#undef xtget_tboxfirstbaseline
C_INLINE XTError XTAPI xtget_tboxfirstbaseline(boxid boxID, Fixed *newValP)
{
	return (XTAPIERRToXTError(XTGetTBoxFirstBaseLine(boxID, newValP)));
}

#undef xtget_preffsspec
C_INLINE XTError XTAPI xtget_preffsspec(FSSpecPtr xtdValueP)
{
	XFileInfoRef fInfoRef;
	XTError error = noErr;

	if (xtdValueP == NULL) {
		return(XTbaddataptr);
	}

	XTCreateEmptyXFileInfoRef(&fInfoRef);
	error = (XTError)XTGetPrefDirSpecification(fInfoRef);
	if (error == noErr) {
		error = XTGetFSSpecFromXFileInfoRef(fInfoRef, xtdValueP);
	}
	XTDisposeXFileInfoRef(fInfoRef);
	return (error);
}

#undef OutputImageFile
C_INLINE XTError XTAPI OutputImageFile(boxid theBox,
		FSSpecPtr imageFSSpecPtr, OSType imageType, XTid exportXtId,
		fixedrect *rectP, int32 *imageWidthP, int32 *imageHeightP)
{
	XTError err = noErr;
	XFileInfoRef fileInfoRef = NULL;
	APIERR xFileErr = ERR_SUCCESS;
	
	if (theBox == NULL || imageFSSpecPtr == NULL) {
		return (XTbaddataptr);
	}
	XTCreateEmptyXFileInfoRef(&fileInfoRef);
	xFileErr = XTSetFSSpecInXFileInfoRef(fileInfoRef, imageFSSpecPtr);
	if (xFileErr != ERR_SUCCESS) {
		return (XTbaddataptr);
	}

	xFileErr = XTOutputImageFile(theBox, fileInfoRef, imageType, exportXtId,
			rectP, imageWidthP, imageHeightP);
	err = (XTError)xFileErr; // its a XTError.
	XTDisposeXFileInfoRef(fileInfoRef);

	return (err);
}

#undef XTFSpGetLayoutSpaceMediaType
C_INLINE APIERR XTAPI XTFSpGetLayoutSpaceMediaType(FSSpec *projectFSSpecP,
		uint32 docIndex,OSType *mediaTypeP)
{
	XFileInfoRef fileInfoRef = NULL;

	API_ENTRY;

	if (projectFSSpecP == NULL) {
		return(XTbaddataptr);
	}

	if (mediaTypeP == NULL) {
		return(XTbaddataptr);
	}

	XTCreateEmptyXFileInfoRef(&fileInfoRef);
	XTSetFSSpecInXFileInfoRef(fileInfoRef, projectFSSpecP);

	SET_TECHERR(XTGetLayoutSpaceMediaTypeEx(fileInfoRef, docIndex,
			mediaTypeP), noErr);
	XTDisposeXFileInfoRef(fileInfoRef);
	API_EXIT;
}

#undef xtget_programfsspec
C_INLINE XTError XTAPI xtget_programfsspec(FSSpecPtr xtdValueP)
{
	XFileInfoRef fInfoRef = NULL;
	XTError error = noErr;

	if (xtdValueP == NULL) {
		return(XTbaddataptr);
	}

	XTCreateEmptyXFileInfoRef(&fInfoRef);
	zerodata(xtdValueP, sizeof(*xtdValueP));
	error = (XTError)XTGetProgramDirSpecification(fInfoRef);
	if (error == noErr) {
#if WINOS
		SETSTRLEN(xtdValueP->name, 0);
		XTGetVolumeIDFromXFileInfoRef(fInfoRef, &xtdValueP->vRefNum);
		XTGetParentIDFromXFileInfoRef(fInfoRef, &xtdValueP->parID);
#else // if WINOS.
		XTGetFSSpecFromXFileInfoRef(fInfoRef, xtdValueP);
#endif // if WINOS else.
	}
	XTDisposeXFileInfoRef(fInfoRef);

	return error;
}

#undef XTPrintSetFileDestination
C_INLINE APIERR XTAPI XTPrintSetFileDestination(FSSpec *fileSpecP)
{
	APIERR apiErr = ERR_SUCCESS;
	XFileInfoRef fileInfoRef = NULL;

	API_ENTRY;

	if (fileSpecP == NULL) {
		return(XTbaddataptr);
	}

	apiErr = XTCreateEmptyXFileInfoRef(&fileInfoRef);
	SET_TECHERR(apiErr, ERR_SUCCESS);

	if (TECHERR == ERR_SUCCESS) {
		apiErr = XTSetFSSpecInXFileInfoRef(fileInfoRef, fileSpecP);
		SET_TECHERR(apiErr, ERR_SUCCESS);
		if (TECHERR == ERR_SUCCESS) {
			SET_TECHERR(XTSetDestinationFile(fileInfoRef), ERR_SUCCESS);
		}
	}

	XTDisposeXFileInfoRef(fileInfoRef);

	API_EXIT;
}

#undef xtget_tempdirfsspec
C_INLINE XTError XTAPI xtget_tempdirfsspec(FSSpecPtr xtdValueP)
{
	APIERR xFileErr = ERR_SUCCESS;
	XFileInfoRef fileInfoRef = NULL;

	if (xtdValueP == NULL) {
		return(XTbaddataptr);
	}

	XTCreateEmptyXFileInfoRef(&fileInfoRef);

	xFileErr = XTGetTempDirSpecification(fileInfoRef);
	if (xFileErr == ERR_SUCCESS) {
#if WINOS
		SETSTRLEN(xtdValueP->name, 0);
		XTGetVolumeIDFromXFileInfoRef(fileInfoRef, &xtdValueP->vRefNum);
		XTGetParentIDFromXFileInfoRef(fileInfoRef, &xtdValueP->parID);
#else // if WINOS.
		XTGetFSSpecFromXFileInfoRef(fileInfoRef, xtdValueP);
#endif // if WINOS else.
	}
	XTDisposeXFileInfoRef(fileInfoRef);

	return ((XTError)xFileErr);
}

#undef FindFileFSp
C_INLINE int16 XTAPI FindFileFSp(int16 vRefNum,int32 dir,const uchar *flpathP,
		int16 permission,int16 *fRefNumP,FSSpecPtr theFSSpecP)
{
	assert( FALSE && "FindFileFSp" );
	return( (int16) -1 );

	//XFileInfoRef fileInfoRef = NULL;

	//if (theFSSpecP != NULL) {
	//	XTCreateEmptyXFileInfoRef(&fileInfoRef);
	//}
	//SET_TECHERR(XTFindFileSpecification(vRefNum, dir, flpathP, permission,
	//			fRefNumP, fileInfoRef), ERR_SUCCESS);
	//if (TECHERR == ERR_SUCCESS) {
	//	if (fileInfoRef != NULL) {
	//		XTGetFSSpecFromXFileInfoRef(fileInfoRef, theFSSpecP);
	//	}
	//}

	//if (fileInfoRef != NULL) {
	//	XTDisposeXFileInfoRef(fileInfoRef);
	//}

	//return ((OSErr)TECHERR);
}

/* Deprecated - use XEGetUniChars or XTHiddenTextGet. */
#undef xegetchars
C_INLINE APIERR XTAPI xegetchars(xehandle xeH, int32 uniCharOffset, int32 byteCount,
		uint8 *bytesP)
{
	int32 i;
	UniChar *uniCharsP = (UniChar *)NewPtr(sizeof(UniChar) * byteCount);
	APIERR apiErr = ERR_SUCCESS;

	apiErr = XEGetUniChars(xeH, uniCharOffset, byteCount, uniCharsP);
	
	for (i = 0; i < byteCount; i++) {
		bytesP[i] = (uint8)uniCharsP[i];
	}

	DisposePtr((Ptr)uniCharsP);

	return apiErr;
}

C_INLINE uint16 XTAPI h_getchar(xehandle xeH, int32 byteOffset)
{
	uint8 ch = 0;
	int32 newOffset = BADCHARPOS;

	XTHiddenTextSkip(xeH, byteOffset, FORWARD, SKIPPOSTENDPOINT, &newOffset);
	/* Should check for newOffset == BADCHARPOS here. h_getchar didn't. */
	if ( newOffset != BADCHARPOS ) {
		xegetchars(xeH, newOffset, 1, &ch);
	}
	return ch;
}

/* Deprecated - use XEPutUniChars of XTHiddenTextInsert. */
#undef XEPutChars
C_INLINE APIERR XTAPI XEPutChars(xehandle xeH, int32 uniCharOffset,
		int32 byteCount, const uint8 *bytesP,
		const globaltxtvars *textAttributesP)
{
	int32 i;
	APIERR apiErr = ERR_SUCCESS;
	UniChar *uniCharsP = (UniChar *)NewPtr(sizeof(UniChar) * byteCount);
	
	for (i = 0; i < byteCount; i++) {
		uniCharsP[i] = bytesP[i];
	}
		
	apiErr = XEPutUniChars(xeH, uniCharOffset, byteCount, uniCharsP, textAttributesP);
			
	DisposePtr((Ptr)uniCharsP);
	
	return apiErr;
}

/* Deprecated - use XEPutUniChars or XTHiddenTextInsert. */
#undef xeputchar
C_INLINE APIERR XTAPI xeputchar(int32 uniCharOffset, uint16 ch) 
{
	textboxid curBoxID = 0;
	xehandle xeH = NULL;
	UniChar uniChar = ch;
	
	xtget_curbox(&curBoxID);
	xegetinfo(curBoxID, &xeH, NULL, NULL, NULL);
	
	return XEPutUniChars(xeH, uniCharOffset, 1, &uniChar, NULL);
}

/* Deprecated - use XEPutUniChars or XTHiddenTextInsert. */
#undef XEPutChar
C_INLINE APIERR XTAPI XEPutChar(xehandle xeH, int32 uniCharOffset, uint16 ch)
{
	UniChar uniChar = ch;
	return XEPutUniChars(xeH, uniCharOffset, 1, &uniChar, NULL);
}

/* Deprecated - use XEPutUniChars or XTHiddenTextInsert. */
#undef xeputchars
C_INLINE APIERR XTAPI xeputchars(int32 uniCharOffset, int32 byteCount,
		const uint8 *bytesP)
{
	textboxid curBoxID = 0;
	xehandle xeH = NULL;
	
	xtget_curbox(&curBoxID);
	xegetinfo(curBoxID, &xeH, NULL, NULL, NULL);

	return XEPutChars(xeH, uniCharOffset, byteCount, bytesP, NULL);
}

#undef SetVisInd
C_INLINE XTError XTAPI SetVisInd(boxid boxID, XTid xtID, uint32 uniqueID)
{
	return XTAPIERRToXTError(XTSetVisInd(boxID, xtID, uniqueID));
}

#undef RemoveVisInd
C_INLINE XTError XTAPI RemoveVisInd(boxid boxID, XTid xtID, uint32 uniqueID)
{
	return XTAPIERRToXTError(XTRemoveVisInd(boxID, xtID, uniqueID));
}


#undef xelinkbox
C_INLINE bool8 XTAPI xelinkbox(textboxid fbptr, textboxid tbptr,
		bool16 invalflag)
{
	return ((XELinkBox(fbptr, tbptr, invalflag) == ERR_SUCCESS) ? TRUE : FALSE);
}

#undef XTGetFileInfoEx
C_INLINE APIERR XTAPI XTGetFileInfoEx(int16 vrefnum, int32 dirid,
		const uchar *filePathNameP, int32 *creationDateP, int32 *modifyDateP,
		FInfo *fndrInfoP, uint32 *lengthP)
{
	XFileInfoRef fileInfoRef = NULL;

	API_ENTRY;
	PARAM_CHECK(3, filePathNameP != NULL);

	XTCreateXFileInfoRef(vrefnum, dirid, filePathNameP, 0, FALSE, FALSE,
			&fileInfoRef);
	SET_TECHERR(XTGetFileInfo(fileInfoRef, (uint32 *)creationDateP, (uint32 *)modifyDateP,
			fndrInfoP, lengthP), ERR_SUCCESS);
	XTDisposeXFileInfoRef(fileInfoRef);

	API_EXIT;
}

#undef cx_translate
C_INLINE int32 XTAPI cx_translate(colorclassid srcCCID, colorrefptr srcCRef,
		int32 srcDev, Fixed shade, int32 flags, colorclassid dstCCID,
		int32 dstDev, colorrefptr dstCRef)
{
	APIERR error = XTColorSpaceTransform(srcCCID, srcCRef, dstCCID,
			(OutputMediaRef)dstDev, dstCRef, shade, flags);

	return error == ERR_SUCCESS ? 1L : 0L;
}

#undef cx_translatespaces
C_INLINE int32 XTAPI cx_translatespaces(colorclassid srcCCID,
		uint16 *srcCompsP, int32 srcDev, int32 count, int32 flags,
		colorclassid dstCCID, int32 dstDev, uint16 *dstCompsP)
{
	int32 srcCompCount = ccspace_getcomponentcount(srcCCID);
	int32 dstCompCount = ccspace_getcomponentcount(dstCCID);
	int32 result = srcCompCount != 0 && dstCompCount != 0 ? 1L : 0L;
	int32 index = 0;
	Str255 srcCRef;
	Str255 dstCRef;
	APIERR error = ERR_FAILURE;

	if (result != 0) {
		for (; index < count; ++index) {
			zerodata(srcCRef, sizeof(srcCRef));
			result = ccspace_makecref(srcCCID, srcCompsP, srcCRef);
			if (result == 0) {
				break;
			}

			zerodata(dstCRef, sizeof(dstCRef));
			error = XTColorSpaceTransform(srcCCID, srcCRef, dstCCID,
					(OutputMediaRef)dstDev, dstCRef, COLORTXFLAGS_NONE, flags);
			if (error != ERR_SUCCESS) {
				result = 0;
				break;
			}

			result = ccspace_getcomponents(dstCCID, dstCRef, dstCompsP);
			if (result == 0) {
				break;
			}

			srcCompsP += srcCompCount;
			dstCompsP += dstCompCount;
		}
	}

	return result;
}

#undef XTShareBoxContent
C_INLINE APIERR XTAPI XTShareBoxContent(boxid box, SharedContentID *contentID)
{
	SharedItemContentUnion itemUnion;
	LISCollection collection;
	ProjectID projectID = INVALPROJECT;

	API_ENTRY;

	PARAM_CHECK(1, istextbox(box));

	zerodata(&itemUnion,sizeof(itemUnion));
	itemUnion.structSize = sizeof(itemUnion);
	itemUnion.objectType = kSharedStory;
	xegetinfo(box, &itemUnion.a.xeH, NULL, NULL, NULL);

	SET_TECHERR(XTGetLayoutSpaceProjectID(XTGetDocRefFromBox(box), &projectID),
			ERR_SUCCESS);
	SET_TECHERR(XTGetProjectLISCollection(projectID, &collection), ERR_SUCCESS);
	SET_TECHERR(XTLIS_ShareItem(collection, &itemUnion, contentID), ERR_SUCCESS);

	API_EXIT;
}

#undef XTBoxHasSharedContent
C_INLINE bool8 XTAPI XTBoxHasSharedContent(boxid box,
		SharedContentID *contentID)
{
	bool8 isShared = FALSE;
	xehandle xeH;
	DocRef docRef = INVAL_DOCREF;
	ProjectID projectID = INVALPROJECT;
	LISCollection collection;

	if (istextbox(box)) {
		docRef = XTGetDocRefFromBox(box);
		XTGetLayoutSpaceProjectID(docRef, &projectID);

		PARAM_CHECK(1, projectID != INVALPROJECT);

		XTGetProjectLISCollection(projectID, &collection);
		xegetinfo(box, &xeH, NULL, NULL, NULL);
		XTLIS_IsStoryShared(collection, xeH, &isShared, contentID);
	}

	return isShared;
}

#undef XTGetNextSharedContent
C_INLINE APIERR XTAPI XTGetNextSharedContent(ProjectID projectID,
		SharedContentID prevContentID, SharedContentID *nextContentID)
{
	LISCollection collection;

	API_ENTRY;

	SET_TECHERR(XTGetProjectLISCollection(projectID, &collection), ERR_SUCCESS);
	SET_TECHERR(XTLIS_GetNextItem(collection, prevContentID, nextContentID),
			ERR_SUCCESS);

	API_EXIT;
}

#undef XTRemoveSharedContent
C_INLINE APIERR XTAPI XTRemoveSharedContent(ProjectID projectID,
		SharedContentID contentID)
{
	LISCollection collection;

	API_ENTRY;

	SET_TECHERR(XTGetProjectLISCollection(projectID, &collection), ERR_SUCCESS);
	SET_TECHERR(XTLIS_RemoveItem(collection, contentID), ERR_SUCCESS);

	API_EXIT;
}

#undef XTLIS_ContentCount
C_INLINE APIERR XTAPI XTLIS_ContentCount(ProjectID projectID, uint32 *count)
{
	LISCollection collection;

	API_ENTRY;

	SET_TECHERR(XTGetProjectLISCollection(projectID, &collection), ERR_SUCCESS);
	SET_TECHERR(XTLIS_GetCollectionSize(collection, count), ERR_SUCCESS);

	API_EXIT;
}

#undef XTLIS_GetContentByIndex
C_INLINE APIERR XTAPI XTLIS_GetContentByIndex(ProjectID projectID, uint32 index,
		SharedContentID *contentID)
{
	LISCollection collection;

	API_ENTRY;

	SET_TECHERR(XTGetProjectLISCollection(projectID, &collection), ERR_SUCCESS);
	SET_TECHERR(XTLIS_GetItemByIndex(collection, index, contentID), ERR_SUCCESS);

	API_EXIT;
}

#undef XTLIS_IsSharedStory
C_INLINE APIERR XTAPI XTLIS_IsSharedStory(xehandle xeH, bool8 *isShared,
		SharedContentID *contentID)
{
	LISCollection collection;
	ProjectID projectID;

	API_ENTRY;

	PARAM_CHECK(2, isShared != NULL);

	*isShared = FALSE;
	PARAM_CHECK(1, xeH != NULL);

	XTGetCurProject(&projectID);
	PARAM_CHECK(1, projectID != INVALPROJECT);

	SET_TECHERR(XTGetProjectLISCollection(projectID, &collection), ERR_SUCCESS);
	SET_TECHERR(XTLIS_IsStoryShared(collection, xeH, isShared, contentID),
			ERR_SUCCESS);

	API_EXIT;
}

#undef XTLIS_GetSharedItemName
C_INLINE APIERR XTAPI XTLIS_GetSharedItemName(ProjectID projectID,
		SharedContentID contentID, uchar *name)
{
	LISCollection collection;
	QXStringRef nameRef = NULL;
	int32 uniCharCount = 0;
	bool8 isConverted = FALSE;
	int32 charCount = 0;
	UniChar *uniCharBuffer = NULL;

	API_ENTRY;

	SET_TECHERR(XTGetProjectLISCollection(projectID, &collection), ERR_SUCCESS);
	SET_TECHERR(XTGetSharedItemName(contentID, &nameRef), ERR_SUCCESS);

	QXStringGetLength(nameRef, &uniCharCount);
	QXStringGetBuffer(nameRef, &uniCharBuffer);

	charCount = uniCharCount + 1;

	SET_TECHERR(XUFromUnicodePreAlloc(uniCharCount,
			(const UniChar *)uniCharBuffer, &charCount, (uchar *)name,
			&isConverted, kUTextEncodingPlatform), ERR_SUCCESS);
	CSTRTOLOCAL(name);
	QXStringDestroy(nameRef);

	API_EXIT;
}

#undef XTLIS_SetSharedItemName
C_INLINE APIERR XTAPI XTLIS_SetSharedItemName(ProjectID projectID,
		SharedContentID contentID, uchar *name)
{
	LISCollection collection;
	QXStringRef nameRef = NULL;

	PARAM_CHECK(3, name != NULL);

	SET_TECHERR(XTGetProjectLISCollection(projectID, &collection), ERR_SUCCESS);
#if WINOS
	SET_TECHERR(QXStringCreateFromLocalString(name, 0, (int32) strlen((char *)name),
				&nameRef), ERR_SUCCESS);
#else // if WINOS
	SET_TECHERR(QXStringCreateFromLocalString(name, 0, name[0], &nameRef),
			ERR_SUCCESS);
#endif // if WINOS else
	SET_TECHERR(XTLIS_SetItemName(collection, contentID, nameRef),
			ERR_SUCCESS);
	QXStringDestroy(nameRef);

	API_EXIT;
}

#undef XTLIS_GetSharedItemContent
C_INLINE APIERR XTAPI XTLIS_GetSharedItemContent(ProjectID projectID,
		SharedContentID contentID, SharedItemContentUnion *itemContent)
{
	LISCollection collection;

	API_ENTRY;

	SET_TECHERR(XTGetProjectLISCollection(projectID, &collection), ERR_SUCCESS);
	SET_TECHERR(XTGetSharedItemInfo(contentID, itemContent), ERR_SUCCESS);

	API_EXIT;
}

#undef XTLIS_GetBoxesFromSharedContent
C_INLINE APIERR XTAPI XTLIS_GetBoxesFromSharedContent(ProjectID projectID,
		SharedContentID contentID, boxid **boxesH, uint32 *boxCount)
{
	LISCollection collection;

	API_ENTRY;

	SET_TECHERR(XTGetProjectLISCollection(projectID, &collection), ERR_SUCCESS);
	SET_TECHERR(XTLIS_GetItemBoxSet(collection, contentID, boxesH, boxCount),
			ERR_SUCCESS);

	API_EXIT;
}

#undef pasteitems
C_INLINE bool8 XTAPI pasteitems(DocID docID, int16 sourceSprd, boxid boxID)
{
	return (XTPasteItems(XTGetDocRefFromDocID(docID), sourceSprd, boxID)
			== ERR_SUCCESS);
}

#undef XTDoNewProject
C_INLINE APIERR XTAPI XTDoNewProject(OSType mediaType)
{
	return XTNewProjectDialog(mediaType, NULL);
}

#undef xtget_docgreekbelow
C_INLINE XTError XTAPI xtget_docgreekbelow(Fixed *xtd_value)
{
	DocID docID = INVAL_DOCID;
	DocRef docRef = INVAL_DOCREF;

	xtget_curdoc(&docID);
	docRef = XTGetDocRefFromDocID(docID);

	return XTAPIERRToXTError(XTGetDocGreekBelow(docRef, xtd_value));
}

#undef blinkcaret
C_INLINE void XTAPI blinkcaret(textboxid textBoxP)
{
	XTBlinkCaret(textBoxP);
}

#undef xtgetlos
C_INLINE int16 XTAPI xtgetlos(textboxid *theBoxIDP, int32 *startCharP,
		bool16 useStart, fixedpoint *boxTopP, loshandle *losHndlP)
{
	return XTAPIERRToOSErr(XTGetLos(theBoxIDP, startCharP, (bool8)useStart,
			boxTopP, losHndlP));
}

#undef xtSetGuideByIndex
C_INLINE XTError XTAPI xtSetGuideByIndex(int16 pageSeq, int16 guideType,
		int16 index, guidelinerec *guideP, bool16 spreadGuides)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return XTAPIERRToXTError(XTSetDocGuideByIndex(XTGetDocRefFromDocID(docID),
			pageSeq, guideType, index, guideP, spreadGuides));
}

#undef xtDelGuideByIndex
C_INLINE XTError XTAPI xtDelGuideByIndex(int16 pageSeq, int16 guideType,
		int16 guideIndex, bool16 spreadGuides)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return XTAPIERRToXTError(XTDelDocGuideByIndex(XTGetDocRefFromDocID(docID),
			pageSeq, guideType, guideIndex, spreadGuides));
}

#undef setlibtag
C_INLINE void XTAPI setlibtag(int16 thelib, int16 index, uchar *labelstrP)
{
	assert( FALSE && "setlibtag" );
	return;
	//return XTAPIERRToXTError (XTGetTextColumnSpec(boxID,colSpecP));
}

#undef addhyphexcep
C_INLINE bool8 XTAPI addhyphexcep(uchar *word)
{
	QXStringRef nameRef = NULL;
	bool8 wordWasAdded = FALSE;

#if WINOS
	QXStringCreateFromLocalString(word, 0, (int32)strlen((char *)word), &nameRef);
#else // if WINOS
	QXStringCreateFromLocalString(word, 0, word[0], &nameRef);
#endif // if WINOS else
	XTAddHyphenationException(nameRef, &wordWasAdded);

	QXStringDestroy(nameRef);

	return wordWasAdded;
}

#undef ReplaceHyphExcep
C_INLINE void XTAPI ReplaceHyphExcep(uchar *newWord)
{
	QXStringRef nameRef = NULL;

#if WINOS
	QXStringCreateFromLocalString(newWord, 0, (int32)strlen((char *)newWord), &nameRef);
#else // if WINOS
	QXStringCreateFromLocalString(newWord, 0, newWord[0], &nameRef);
#endif // if WINOS else
	XTReplaceHyphenationException(nameRef);

	QXStringDestroy(nameRef);
}

#undef FindHyphExcep
C_INLINE bool8 XTAPI FindHyphExcep(uchar *word, uchar *rtnWord)
{
	QXStringRef inputNameRef = NULL;
	QXMutableStringRef outputNameRef = NULL;
	bool8 wordWasFound = FALSE;

#if WINOS
	QXStringCreateFromLocalString(word, 0, (int32)strlen((char *)word), &inputNameRef);
	QXMutableStringCreateFromLocalString(rtnWord, 0, (int32)strlen((char *)rtnWord),
			&outputNameRef);
#else // if WINOS
	QXStringCreateFromLocalString(word, 0, word[0], &inputNameRef);
	QXMutableStringCreateFromLocalString(rtnWord, 0, rtnWord[0], &outputNameRef);
#endif // if WINOS else
	XTFindHyphenationException(inputNameRef, outputNameRef,	&wordWasFound);

	QXStringDestroy(inputNameRef);
	QXMutableStringDestroy(outputNameRef);

	return wordWasFound;
}

#undef delhyphexcep
C_INLINE void XTAPI delhyphexcep(uchar *word)
{
	QXStringRef nameRef = NULL;

#if WINOS
	QXStringCreateFromLocalString(word, 0, (int32)strlen((char *)word), &nameRef);
#else // if WINOS
	QXStringCreateFromLocalString(word, 0, word[0], &nameRef);
#endif // if WINOS else
	XTDeleteHyphenationException(nameRef);

	QXStringDestroy(nameRef);
}

#undef findhyphexcepbyindex
C_INLINE bool8 XTAPI findhyphexcepbyindex(int32 index, uchar *word)
{
	QXStringRef nameRef = NULL;
	bool8 exceptionWasFound = FALSE;
	bool8 isConverted = FALSE;
	int32 uniCharCount = 0;
	int32 charCount = 0;
	UniChar *uniCharBuffer = NULL;

#if WINOS
	QXStringCreateFromLocalString(word, 0, (int32)strlen((char *)word), &nameRef);
#else // if WINOS
	QXStringCreateFromLocalString(word, 0, word[0], &nameRef);
#endif // if WINOS else
	XTFindHyphenationExceptionByIndex(index, &nameRef, &exceptionWasFound);
	
	QXStringGetLength(nameRef, &uniCharCount);
	QXStringGetBuffer(nameRef, &uniCharBuffer);

	charCount = uniCharCount + 1;

	XUFromUnicodePreAlloc(uniCharCount, (const UniChar *)uniCharBuffer, &charCount,
			(uchar *)word, &isConverted, kUTextEncodingPlatform);
	CSTRTOLOCAL(word);

	QXStringDestroy(nameRef);

	return exceptionWasFound;
}

#undef israsterbox
C_INLINE bool8 XTAPI israsterbox(boxid theboxid)
{
	uint16 rasterizeValue = 0;
	
	XTGetBoxRasterize(theboxid, &rasterizeValue, NULL);
	
	return (rasterizeValue == BOX_RASTERIZED);
}

#undef XTEPSSetFileDestination
C_INLINE APIERR XTAPI XTEPSSetFileDestination(FSSpec *fileSpec)
{
	XFileInfoRef fileInfoRef;

	API_ENTRY;

	PARAM_CHECK(1, fileSpec != NULL);

	XTSetFSSpecInXFileInfoRef(fileInfoRef, fileSpec);
	
	return (XTSetDestinationFile(fileInfoRef));

	API_EXIT;
}

#undef XTOpenDocFSp
C_INLINE XTError XTAPI XTOpenDocFSp(const FSSpec *fileSpecP, uint32 flags)
{
	XFileInfoRef fileInfoRef = NULL;
   APIERR err = ERR_SUCCESS ;

	PARAM_CHECK(1, fileSpecP != NULL);
	
	err = XTCreateEmptyXFileInfoRef(&fileInfoRef);
	if (err == ERR_SUCCESS) {
		err = XTSetFSSpecInXFileInfoRef(fileInfoRef, fileSpecP);
		if (err == ERR_SUCCESS) {
			err = XTOpenDoc(fileInfoRef, flags);
		}
		XTDisposeXFileInfoRef(fileInfoRef);
	}

	return XTAPIERRToXTError(err);

} // End XTOpenDocFSp

#undef XTPrintSetFileDestinationEx
C_INLINE APIERR XTAPI XTPrintSetFileDestinationEx(XFileInfoRef fileInfoRef)
{
	bool8 isValid = FALSE;

	API_ENTRY;

	PARAM_CHECK(1, fileInfoRef != NULL);
	
	XTIsValidXFileInfoRef(fileInfoRef, &isValid);

	PARAM_CHECK(1, isValid);

	SET_TECHERR(XTSetDestinationFile(fileInfoRef), ERR_SUCCESS);

	API_EXIT;
}

#undef xtset_tiffimport
C_INLINE APIERR XTAPI xtset_tiffimport(int16 xtd_value)
{
	return XTSetTiffImport(xtd_value);
}

#undef xtget_tiffimport
C_INLINE APIERR XTAPI xtget_tiffimport(int16 *xtd_value)
{
	return XTGetTiffImport(xtd_value);
}

#undef xtget_printinfo
C_INLINE XTError XTAPI xtget_printinfo(uint16 *xtd_value)
{
	return XTAPIERRToXTError(XTGetPrintInfo((int16 *)xtd_value));
}

#undef xtset_printinfo
C_INLINE XTError XTAPI xtset_printinfo(uint16 xtd_value)
{
	return XTAPIERRToXTError(XTSetPrintInfo(xtd_value));
}

#undef xtget_spinepos
C_INLINE XTError XTAPI xtget_spinepos(int8 *spinePos)
{
	DocID docID = INVAL_DOCID;

	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetSpinePos(XTGetDocRefFromDocID(docID),
			spinePos)));
}

#undef setfileinfo
C_INLINE OSErr XTAPI setfileinfo(int16 vrefnum,int32 dirid,const uchar *filename,
		const int32 *creationdate,const int32 *modifydate,const FInfo *fndrinfo)
{
	XFileInfoRef fileInfoRef = NULL;
	Str255 fileName;
	APIERR apiErr = ERR_SUCCESS;

#if MACOS
	LOCALTOCSTRCPY(fileName, filename);
#else // if MACOS
	strcpy_s((char *)fileName, sizeof(Str255), (const char *)filename);
#endif // if MACOS else
	apiErr = XTCreateXFileInfoRef(vrefnum, dirid, fileName, 0,FALSE,
			FALSE, &fileInfoRef);
	if (apiErr == ERR_SUCCESS) {
		apiErr = XTSetFileInfo(fileInfoRef, (uint32 *)creationdate,
				(uint32 *)modifydate, fndrinfo);
	}
	XTDisposeXFileInfoRef(fileInfoRef);

	return(XTAPIERRToOSErr(apiErr));
}

#undef VisIndsAreGreeked
C_INLINE bool8 XTAPI VisIndsAreGreeked(void)
{
	bool8 retValue = FALSE;
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = NULL;

	xtget_curdoc(&docID);
	XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
	XTViewVisIndsAreGreeked(docViewRef, &retValue);
	return retValue;
}

#undef VisIndsAreVisible
C_INLINE bool8 XTAPI VisIndsAreVisible(void)
{
	bool8 retValue = TRUE;
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = NULL;

	xtget_curdoc(&docID);
	XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
	XTViewVisIndsAreVisible(docViewRef, &retValue);
	return retValue;
}

#undef SetVisIndVisibility
C_INLINE void XTAPI SetVisIndVisibility(bool8 visible, bool8 invalidate)
{
	DocID docID = INVAL_DOCID;
	DocViewRef docViewRef = NULL;

	xtget_curdoc(&docID);
	XTGetDocViewRef(XTGetDocRefFromDocID(docID), &docViewRef);
	XTViewSetVisIndVisibility(docViewRef, visible, invalidate);
}

#undef xtget_picscrollbounds
C_INLINE XTError xtget_picscrollbounds(boxid theBox, bool16 rotCompensate,
		Fixed *minXScroll, Fixed *maxXScroll, Fixed *minYScroll,
		Fixed *maxYScroll)
{
	XTGetPicScrollBounds(theBox, rotCompensate, minXScroll, maxXScroll,
		minYScroll, maxYScroll);

	return noErr;
}

#undef XTGetPictDisplayPageNameStr
C_INLINE APIERR XTAPI XTGetPictDisplayPageNameStr(boxid picBoxPtr,
		int16 pageNum, uchar *pageName)
{
	QXStringRef pageNameQXStr = NULL;
	int32 uniCharCount = 0;
	bool8 isConverted = FALSE;
	int32 charCount = 0;
	UniChar *uniCharBuffer = NULL;
	APIERR apiErr = ERR_SUCCESS;

	PARAM_CHECK(2, pageNum != 0);

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

#undef ExtractFileName
C_INLINE void XTAPI ExtractFileName(uchar *inOutName)
{
	QXMutableStringRef ioName = NULL;
	QXMutableStringCreateEmpty(&ioName);
	XTUExtractFileName(ioName);
	if (ioName) {
		QXStringRef2CStr((QXStringRef&) ioName, (char*) inOutName);
		QXMutableStringDestroy(ioName);
	}
	else {
		inOutName[0] = 0;
	}
}

#undef color_new
C_INLINE colorid XTAPI color_new(colordbhandle dbh, int32 flags)
{
	colorid newColorID = INVALCOLORID;
	
	XTColorNew(dbh, flags, &newColorID);
	
	return newColorID;
}

#undef editlib
C_INLINE void XTAPI editlib(int16 theLib, int16 index, int16 editMsg, bool8 userConfirm)
{
	XTEditLib(XTGetDocRefFromDocID(theLib), index, editMsg, userConfirm);
}

#undef copyitems
C_INLINE bool8 XTAPI copyitems(DocID targetDoc, int16 targetPage)
{
	boxid theBox = 0;
	DocID curDoc = INVAL_DOCID;
	APIERR err = ERR_SUCCESS;

	xtget_curdoc(&curDoc);
	if (curDoc != CBDOC) {
		xtget_curbox(&theBox);
	}

	err = XTCopyItems(theBox, XTGetDocRefFromDocID(curDoc),
			XTGetDocRefFromDocID(targetDoc), targetPage);

	return (err == ERR_SUCCESS);
}

#undef boxesingroup
C_INLINE Handle XTAPI boxesingroup(boxid thegroupid, int16 *countP)
{
	boxid boxP;
	int32 count = 0;
	Handle groupH = NULL;

	if (thegroupid != NULL) {
		boxP = thegroupid;
	}
	else {
		xtget_curbox(&boxP);
	}

	XTGetBoxesInGroup(boxP, &groupH, &count);
	*countP = (int16)count;

	return groupH;
}

#undef XTFindBoxByID
C_INLINE APIERR XTAPI XTFindBoxByID(DocRef docRef, BoxUID theBoxUID,
		int16 spread, boxid *boxP)
{
	return XTFindUniqueID(docRef, theBoxUID, boxP);
}

#undef countlibitems
C_INLINE int16 XTAPI countlibitems(int16 theLib)
{
	int16 numItems = 0;

	XTCountLibraryItems(XTGetDocRefFromDocID(theLib), &numItems);

	return numItems;
}

#undef getboxminimums
C_INLINE void XTAPI getboxminimums(const boxid thebox, const int16 columns,
		Fixed *minwidthP, Fixed *minheightP)
{
	XTGetBoxMinimums(thebox, columns, minwidthP, minheightP);
}

#undef IsValidName
C_INLINE bool8 XTAPI IsValidName(const uchar *nameStr)
{
	bool8 isValid = FALSE;

	QXStringRef nameStrRef = NULL;
#if WINOS
	QXStringCreateFromLocalString(nameStr, 0,
			(int32)strlen((char *)nameStr), &nameStrRef);
#else
	QXStringCreateFromLocalString(nameStr, 0,
			pageNameStr[0], &nameStrRef);
#endif

	XTUIsValidName(nameStrRef, &isValid);

	QXStringDestroy(nameStrRef);

	return isValid;
}

#undef getlibtag
C_INLINE void XTAPI getlibtag(int16 theLib, int16 index, uchar *labelStr)
{
	assert( FALSE && "getlibtag" );
	return;
	//XTGetLibraryItemTag(XTGetDocRefFromDocID(theLib), index, labelStr);
}

#undef deletelibitem
C_INLINE void XTAPI deletelibitem(int16 curLibPage, int16 libNum)
{
	XTDeleteLibItem(XTGetDocRefFromDocID(libNum), curLibPage);
}

#undef HasPicShapePathByXTid
C_INLINE bool8 XTAPI HasPicShapePathByXTid(boxid pictureBox, XTid xtID)
{
	bool8 result = FALSE;
	PictureViewRef pViewRef = kInvalPictureViewRef;

	if (pictureBox != NULL && isapicture(pictureBox) && xtID != INVAL_XTID) {
		XTGetPictureView(pictureBox, &pViewRef);
		XTHasPicShapePathByXTid(pViewRef, xtID, &result);
	}

	return result;
}

#undef sethyphmethod
C_INLINE bool8 XTAPI sethyphmethod(int16 xthyphlanguage,int16 xthyphmethod)
{
	ProjectID curProject = INVALPROJECT;
	APIERR err;
	bool8 success;
	
	XTGetCurProject(&curProject);

	err = XTSetHyphenationMethod(xthyphlanguage, xthyphmethod,
				curProject != INVALPROJECT ? TRUE: FALSE);
	success = (err == ERR_SUCCESS) ? TRUE : FALSE;

	return (success);
}

#undef gethyphmethod
C_INLINE int8 XTAPI gethyphmethod(int16 xthyphlanguage)
{
	int16 method = 0;
	ProjectID curProject = INVALPROJECT;

	XTGetCurProject(&curProject);
	XTGetHyphenationMethod(xthyphlanguage, &method,
			curProject != INVALPROJECT ? TRUE: FALSE);

	return (int8)method;
}

#undef xtCountGuides
C_INLINE int16 XTAPI xtCountGuides(int16 pageSeq, int16 guideType, bool16 spreadGuides)
{
	int16 guideCount = 0;
	DocID docID = INVAL_DOCID;
	
	xtget_curdoc(&docID);
	XTCountGuides(XTGetDocRefFromDocID(docID), pageSeq, guideType, spreadGuides,
			&guideCount);

	return guideCount;
}

#undef xtGetAllGuides
C_INLINE XTError XTAPI xtGetAllGuides(int16 pageSeq, bool16 spreadGuides,
		guidehandle *guideHndl)
{
	DocID docID = INVAL_DOCID;
	
	xtget_curdoc(&docID);
	return(XTAPIERRToXTError(XTGetAllGuides(XTGetDocRefFromDocID(docID), pageSeq,
			spreadGuides, guideHndl)));
}

#undef addtogroup
C_INLINE void XTAPI addtogroup(boxid theBoxID, bool16 docalc)
{
	boxid curBox = NULL;
	xtget_curbox(&curBox);
	XTAddToGroup(curBox, theBoxID, docalc);
}

#undef group
C_INLINE void XTAPI group()
{
	boxid curBox = NULL;
	xtget_curbox(&curBox);
	XTGroupBoxes(curBox, TRUE);
}

#undef ungroup
C_INLINE void XTAPI ungroup()
{
	boxid curBox = NULL;
	xtget_curbox(&curBox);
	XTUngroupBox(curBox);
}

#undef XTGetPageName
C_INLINE APIERR XTAPI XTGetPageName(DocRef docRef, int16 pageNum,
		uchar *pageNameStr)
{
	QXStringRef pageNameStrRef = NULL;
	int32 uniCharCount = 0;
	bool8 isConverted = FALSE;
	int32 charCount = 0;
	UniChar *uniCharBuffer = NULL;
	APIERR err = ERR_SUCCESS;

	err = XTUGetPageName(docRef, pageNum, &pageNameStrRef, TRUE);
	if (err == ERR_SUCCESS) {
		QXStringGetLength(pageNameStrRef, &uniCharCount);
		QXStringGetBuffer(pageNameStrRef, &uniCharBuffer);

		charCount = uniCharCount + 1;

		err = XUFromUnicodePreAlloc(uniCharCount,
				(const UniChar *)uniCharBuffer, &charCount, (uchar *)pageNameStr,
				&isConverted, kUTextEncodingPlatform);
		if (err == ERR_SUCCESS) {
			CSTRTOLOCAL(pageNameStr);
		}
		QXStringDestroy(pageNameStrRef);
	}

	return err;
}

#undef XTGetPageNameEx
C_INLINE APIERR XTAPI XTGetPageNameEx(DocRef docRef, int16 pageNum,
		uchar *pageNameStr, bool8 createName)
{
	QXStringRef pageNameStrRef = NULL;
	int32 uniCharCount = 0;
	bool8 isConverted = FALSE;
	int32 charCount = 0;
	UniChar *uniCharBuffer = NULL;
	APIERR err = ERR_SUCCESS;

	PARAM_CHECK(3, pageNameStr != NULL);

	err = XTUGetPageName(docRef, pageNum, &pageNameStrRef, createName);
	if (err == ERR_SUCCESS) {
		QXStringGetLength(pageNameStrRef, &uniCharCount);
		QXStringGetBuffer(pageNameStrRef, &uniCharBuffer);

		charCount = uniCharCount + 1;

		err = XUFromUnicodePreAlloc(uniCharCount,
				(const UniChar *)uniCharBuffer, &charCount, (uchar *)pageNameStr,
				&isConverted, kUTextEncodingPlatform);
		if (err == ERR_SUCCESS) {
			CSTRTOLOCAL(pageNameStr);
		}
		QXStringDestroy(pageNameStrRef);
	}

	return (err);
}

#undef XTIsValidPageName
C_INLINE APIERR XTAPI XTIsValidPageName(DocRef docRef, int16 pageNum,
		const uchar *pageNameStr, bool8 *isValid)
{
	QXStringRef pageNameStrRef = NULL;
	APIERR err = ERR_SUCCESS;

#if WINOS
	QXStringCreateFromLocalString(pageNameStr, 0,
			(int32)strlen((char *)pageNameStr), &pageNameStrRef);
#else
	QXStringCreateFromLocalString(pageNameStr, 0,
			pageNameStr[0], &pageNameStrRef);
#endif
	err = XTUIsValidPageName(docRef, pageNum, pageNameStrRef, isValid);

	QXStringDestroy(pageNameStrRef);

	return err;
}

#undef XTSetPageName
C_INLINE int16 XTAPI XTSetPageName(DocRef docRef, int16 pageNum,
		const uchar *pageNameStr)
{
	QXStringRef pageNameStrRef = NULL;
	APIERR err = ERR_SUCCESS;

	// To keep the legacy behavior same, we are returning this old value here.
	if (docRef == INVAL_DOCREF) {
		return DIEDIEDIE;
	}
#if WINOS
	QXStringCreateFromLocalString(pageNameStr, 0, (int32)strlen((char *)pageNameStr),
			&pageNameStrRef);
#else
	QXStringCreateFromLocalString(pageNameStr, 0, pageNameStr[0],
			&pageNameStrRef);
#endif
	err = XTUSetPageName(docRef, pageNum, pageNameStrRef);

	QXStringDestroy(pageNameStrRef);

	return (int16)err;
}

#undef XTRenameFileEX
C_INLINE APIERR XTAPI XTRenameFileEX(XFileInfoRef fileInfoRef,
		const QXStringRef newFileName)
{
	return(XTRenameFile(fileInfoRef, newFileName));
}

#undef XTOpenProjectEx
C_INLINE APIERR XTAPI XTOpenProjectEx(XFileInfoRefConst fileInfoRef,
		uint32 openProjectFlags, ProjectID *projectIDP)
{
	return(XTOpenProject(fileInfoRef, openProjectFlags, projectIDP));
}

#undef str2valbufEX
C_INLINE bool8 XTAPI str2valbufEX(int16 unit,int16 direction,
		Fixed *value,const uchar *str,int32 flag)
{
	APIERR err = ERR_SUCCESS;
	QXStringRef inputStringRef = NULL;
	bool8 wasConverted = FALSE;

#if WINOS
	QXStringCreateFromLocalString(str, 0, (int32)strlen((char *)str), &inputStringRef);
#else
	QXStringCreateFromLocalString(str, 0, str[0], &inputStringRef);
#endif
	err = XTStringToFixed(inputStringRef, value, unit, direction, flag,
			&wasConverted);

	QXStringDestroy(inputStringRef);

	return wasConverted;
}

#undef val2strbufEX
C_INLINE void XTAPI val2strbufEX(int16 units, Fixed value,
		uchar *convertedStringP, int32 flags)
{
	APIERR err = ERR_SUCCESS;
	QXStringRef textToGetStringRefP = NULL;
	bool8 wasConvertedP = FALSE, isConverted = FALSE;
	int32 uniCharCount = 0, charCount = 0;
	UniChar *uniCharBuffer = NULL;

	err = XTFixedToString(value, &textToGetStringRefP, units,
			flags, &wasConvertedP);
	if (wasConvertedP == TRUE) {
		QXStringGetLength(textToGetStringRefP, &uniCharCount);
		QXStringGetBuffer(textToGetStringRefP, &uniCharBuffer);

		charCount = uniCharCount + 1;

		err = XUFromUnicodePreAlloc(uniCharCount,
				(const UniChar *)uniCharBuffer, &charCount,
				(uchar *)convertedStringP, &isConverted, kUTextEncodingPlatform);
		if (err == ERR_SUCCESS) {
			CSTRTOLOCAL(convertedStringP);
		}
		QXStringDestroy(textToGetStringRefP);
	}
}

#undef GetImageExportInfo
C_INLINE XTError XTAPI GetImageExportInfo(boxid theBox, bool8 *output,
		bool8 *asImage, uchar *imageName, OSType *imageType,
		uchar *fileExtension, uchar *altText, XTid *xtId)
{
	XTError err = noErr;
	QXStringRef imageNameRef = NULL;
	QXStringRef altTextRef = NULL;
	QXStringRef fileExtensionRef = NULL;
	int32 uniCharCount = 0;
	int32 charCount = 0;
	bool8 isConverted = FALSE;
	UniChar *uniCharBuffer = NULL;
	bool8 isEmpty = FALSE;

	err = XTGetImageExportInfo(theBox, output, asImage,
			imageName != NULL ? &imageNameRef : NULL, imageType,
			fileExtension != NULL ? &fileExtensionRef : NULL,
			altText != NULL ? &altTextRef : NULL, xtId);
	if (err == ERR_SUCCESS) {
		if (imageNameRef != NULL) {
			err = QXStringIsEmpty(imageNameRef, &isEmpty);

			if (!isEmpty) {
				// Conversion to one byte for imageName.
				QXStringGetLength(imageNameRef, &uniCharCount);
				QXStringGetBuffer(imageNameRef, &uniCharBuffer);

				charCount = uniCharCount + 1;

				err = XUFromUnicodePreAlloc(uniCharCount,
						(const UniChar *)uniCharBuffer, &charCount, (uchar *)imageName,
						&isConverted, kUTextEncodingPlatform);
				if (err == ERR_SUCCESS) {
					CSTRTOLOCAL(imageName);
				}

				charCount = 0;
				*uniCharBuffer = NULL;
				uniCharCount = 0;
			}
		}

		if (fileExtensionRef != NULL) {
			err = QXStringIsEmpty(fileExtensionRef, &isEmpty);
			if (!isEmpty) {
				// Conversion to one byte string for fileExtension.
				QXStringGetLength(fileExtensionRef, &uniCharCount);
				QXStringGetBuffer(fileExtensionRef, &uniCharBuffer);

				charCount = uniCharCount + 1;

				err = XUFromUnicodePreAlloc(uniCharCount,
						(const UniChar *)uniCharBuffer, &charCount,
						(uchar *)fileExtension, &isConverted, kUTextEncodingPlatform);
				if (err == ERR_SUCCESS) {
					CSTRTOLOCAL(fileExtension);
				}
				charCount = 0;
				*uniCharBuffer = NULL;
				uniCharCount = 0;
			}
		}

		if (altTextRef != NULL) {
			err = QXStringIsEmpty(altTextRef, &isEmpty);
			if (!isEmpty) {
				// Conversion to one byte string for altText.
				QXStringGetLength(altTextRef, &uniCharCount);
				QXStringGetBuffer(altTextRef, &uniCharBuffer);

				charCount = uniCharCount + 1;

				err = XUFromUnicodePreAlloc(uniCharCount,
						(const UniChar *)uniCharBuffer, &charCount,
						(uchar *)altText, &isConverted, kUTextEncodingPlatform);
				if (err == ERR_SUCCESS) {
					CSTRTOLOCAL(altText);
				}
			}
		}
		// Destory the QXStringRefs.
		if (imageNameRef != NULL) {
			QXStringDestroy(imageNameRef);
		}
		if (fileExtensionRef != NULL) {
			QXStringDestroy(fileExtensionRef);
		}
		if (altTextRef != NULL) {
			QXStringDestroy(altTextRef);
		}
	}

	return err;
}

#undef GetFRefInfoHFS
C_INLINE int16 XTAPI GetFRefInfoHFS(int16 fRefNum, int16 *vRefNumP,
		int32 *dirIDP, uchar *name)
{
	APIERR err = ERR_SUCCESS;
	QXStringRef fileNameP = NULL;
	int32 uniCharCount = 0, charCount = 0;
	bool8 isConverted = FALSE;
	UniChar *uniCharBuffer = NULL;

	err = XTUGetFRefInfoHFS(fRefNum, vRefNumP, dirIDP, &fileNameP);
	if (err == ERR_SUCCESS) {
		QXStringGetLength(fileNameP, &uniCharCount);
		QXStringGetBuffer(fileNameP, &uniCharBuffer);

		charCount = uniCharCount + 1;

		err = XUFromUnicodePreAlloc(uniCharCount, (const UniChar *)uniCharBuffer,
				&charCount, (uchar *)name, &isConverted, kUTextEncodingPlatform);
		if (err == ERR_SUCCESS) {
			CSTRTOLOCAL(name);
		}
		QXStringDestroy(fileNameP);
	}

	return (int16)err;
}

#undef XTGetFileNameFromXFileInfoRef
C_INLINE APIERR XTAPI XTGetFileNameFromXFileInfoRef
		(XFileInfoRefConst inputFileInfoRef, uchar *fileNameP, int32 *bufferSizeP)
{
	APIERR err = ERR_SUCCESS;
	QXStringRef fileNameRefP = NULL;
	int32 uniCharCount = 0;
	bool8 isConverted = FALSE;
	int32 charCount = 0;
	UniChar *uniCharBuffer = NULL;
	bool8 isEmpty = FALSE;

	PARAM_CHECK(2, fileNameP != NULL);
	PARAM_CHECK(3, bufferSizeP != NULL);

	err = XTUGetFileNameFromXFileInfoRef(inputFileInfoRef, &fileNameRefP);
	if (err == ERR_SUCCESS) {
		err = QXStringIsEmpty(fileNameRefP, &isEmpty);
		if (!isEmpty) {
			QXStringGetLength(fileNameRefP, &uniCharCount);
			QXStringGetBuffer(fileNameRefP, &uniCharBuffer);

			charCount = uniCharCount + 1;

			err = XUFromUnicodePreAlloc(uniCharCount,
					(const UniChar *)uniCharBuffer, &charCount,
					(uchar *)fileNameP, &isConverted, kUTextEncodingPlatform);

			QXStringDestroy(fileNameRefP);
		}
		else {
#if MACOS
			LOCALTOCSTRCPY(fileNameP, STRCONST(""));
#else
			strcpy_s((char *)fileNameP, *bufferSizeP, (const char *)(""));
#endif
		}
	}

	return err;
}

#undef XTFSMakeFSSpecEX
C_INLINE APIERR XTAPI XTFSMakeFSSpecEX(int16 volumeID, int32 parentID,
		const uchar *fNameP, FSSpec *createFileSpecP)
{
	return (XTFSMakeFSSpec(volumeID, parentID, fNameP, createFileSpecP));
}

#undef fcomparetostring
C_INLINE uint8 XTAPI fcomparetostring(int16 unit, int16 direction,
		Fixed *value, const uchar *string, int32 flags)
{
	uint8 result = ERRORVAL;

	QXStringRef stringRef = NULL;
#if WINOS
	QXStringCreateFromLocalString(string, 0, (int32)strlen((char *)string), &stringRef);
#else
	QXStringCreateFromLocalString(string, 0, pageNameStr[0], &stringRef);
#endif
	XTUFCompareToString(unit, direction, value, stringRef, flags, &result);

	QXStringDestroy(stringRef);

	return result;
}

#undef fcomparetostringEX
C_INLINE uint8 XTAPI fcomparetostringEX(int16 unit, int16 direction,
		Fixed *value, const uchar *string, int32 flags)
{
	uint8 result = ERRORVAL;

	QXStringRef stringRef = NULL;
#if WINOS
	QXStringCreateFromLocalString(string, 0, (int32)strlen((char *)string), &stringRef);
#else
	QXStringCreateFromLocalString(string, 0, pageNameStr[0], &stringRef);
#endif
	XTUFCompareToString(unit, direction, value, stringRef, flags, &result);

	QXStringDestroy(stringRef);

	return result;
}

#undef fgetfield
C_INLINE int16 XTAPI fgetfield(DialogPtr thewptr, int16 field,
		int16 unit, int16 direction, Fixed *oldvalue, int32 flags)
{
	int16 result = ERRORVAL;

	QXMutableStringRef resultStrRef = NULL;
	XTUGetField(thewptr, field, resultStrRef);
	QXMutableStringCreateEmpty(&resultStrRef);
	//FGetField(thewptr, field, unit, direction, oldvalue, flags, &result);

	QXMutableStringDestroy(resultStrRef);

	return result;
}

#undef fgetfieldEX
C_INLINE int16 XTAPI fgetfieldEX(DialogPtr thewptr, int16 field,
		int16 unit, int16 direction, Fixed *oldvalue, int32 flags)
{
	int16 result = ERRORVAL;

	QXMutableStringRef resultStrRef = NULL;
	QXMutableStringCreateEmpty(&resultStrRef);
	XTUGetField(thewptr, field, resultStrRef);
	//FGetField(thewptr, field, unit, direction, oldvalue, flags, &result);

	QXMutableStringDestroy(resultStrRef);

	return result;
}

#undef fsetfield
C_INLINE void XTAPI fsetfield(DialogPtr thewptr, int16 field,
		int16 unit, int16 direction, Fixed value, int32 flags)
{
	assert( FALSE && "fsetfield" );
	return;
	//FSetField(thewptr, field, unit, direction, value, flags);
}

#undef fsetfieldEX
C_INLINE void XTAPI fsetfieldEX(DialogPtr thewptr, int16 field,
		int16 unit, int16 direction, Fixed value, int32 flags)
{
	assert( FALSE && "fsetfield" );
	return;
	//FSetField(thewptr, field, unit, direction, value, flags);
}

#undef extractflname
C_INLINE void XTAPI extractflname(uchar *fileName)
{
	QXMutableStringRef fileNameStrRef = NULL;
#if WINOS
	QXMutableStringCreateFromLocalString(fileName, 0, (int32)strlen((char *)fileName), &fileNameStrRef);
#else
	QXMutableStringCreateFromLocalString(fileName, 0, fileName[0], &fileNameStrRef);
#endif
	XTUExtractFileName(fileNameStrRef);
	if ( fileNameStrRef ) {
		QXStringRef2CStr((QXStringRef&) fileNameStrRef, (char*) fileName);
		QXMutableStringDestroy(fileNameStrRef);
	}
}

#undef extractpathname
C_INLINE void XTAPI extractpathname(uchar *pathName)
{
	APIERR err = ERR_SUCCESS;
	QXStringRef fullPathRef = NULL;
	QXStringRef pathRef = NULL;
	int32 uniCharCount = 0;
	bool8 isConverted = FALSE;
	int32 charCount = 0;
	UniChar *uniCharBuffer = NULL;

#if WINOS
	QXStringCreateFromLocalString(pathName, 0,
			(int32)strlen((char *)pathName), &fullPathRef);
#else
	QXStringCreateFromLocalString(pathName, 0, pathName[0], &fullPathRef);
#endif
	err = XTUExtractPathName(fullPathRef, &pathRef);

	if (err == ERR_SUCCESS) {
		QXStringGetLength(pathRef, &uniCharCount);
		QXStringGetBuffer(pathRef, &uniCharBuffer);

		charCount = uniCharCount + 1;

		err = XUFromUnicodePreAlloc(uniCharCount, (const UniChar *)uniCharBuffer,
				&charCount, (uchar *)pathName, &isConverted,
				kUTextEncodingPlatform);
		if (err == ERR_SUCCESS) {
			CSTRTOLOCAL(pathName);
		}
		QXStringDestroy(pathRef);
	}
}


#undef xtget_pboxpicftype
C_INLINE XTError XTAPI xtget_pboxpicftype(boxid theBoxID, OSType *picFType)
{
	PictureViewRef picViewRef = kInvalPictureViewRef;

	APIERR apiErr = XTGetPictureView(theBoxID, &picViewRef);
	XTError err = XTAPIERRToXTError(apiErr);

	if (err == noErr && picViewRef != kInvalPictureViewRef) {
		return(XTAPIERRToXTError(XTGetPictureViewFileType(picViewRef, picFType)));
	}

	return err;
}

#undef XTPositionPict
C_INLINE APIERR XTAPI XTPositionPict(boxid theBox, int16 flags)
{
	return (XTPositionPicture(theBox, flags, TRUE));
}

#undef xt_UpdatePath
C_INLINE OSErr XTAPI xt_UpdatePath(boxid boxID, runaroundinfo *newinfo,
		bool8 doclip, bool16 forcerescan)

{
	return (XTUpdatePath(boxID, newinfo, doclip, forcerescan, TRUE));
}
#endif // ifndef _XP_XTLEGACYE7_H_
