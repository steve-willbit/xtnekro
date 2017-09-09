/* ------------------------------------------------------------------------ *

	DbpConsts.cpp

	Copyright © Sinedita S.r.l 2003. All Rights Reserved.

* ------------------------------------------------------------------------ */

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpConsts.h"

// NAMESPACE DBP
// NAMESPACE DBP
// NAMESPACE DBP

namespace DBP
{

// GLOBALS
// GLOBALS
// GLOBALS

	const char kCopyDeskFileExtension[] = ".QCD";

	const char kXpressFileExtension[] = ".QXP";
	const char kPdfFileExtension[] = ".PDF";
	const char kPsFileExtension[] = ".PS";
	const char kXmlFileExtension[] = ".XML";
	const char kJpegFileExtension[] = ".JPG";
	const char kLogFileExtension[] = ".LOG";

	const char kXtensionFolder[] = "XTensions";
	
	const char kReturnChar = '\r';
	const char kNewLineChar = '\n';
	const char kTabChar = '\t';
	const char kHyphenChar = '-';
	const char kSlashChar = '/';
	const char kSpaceChar = ' ';
	const char kDotChar = '.';
	const char kCommaChar = ',';
	const char kUnderlineChar = '_';
	const char kOpenAngular = '<';
	const char kCloseAngular = '>';

	const char kMacDirSeparatorChar = ':';
	const char kWinDirSeparatorChar = '\\';
	const char kUnixDirSeparatorChar = '/';
	
	const char kDirSeparatorChar = '\\';

} // namespace DBP

/* ------------------------------------------------------------------------ *

	$Log: not supported by cvs2svn $
	Revision 1.18  2008/07/17 15:23:21  marchese
	versione SEAT.13
	
	Revision 1.17  2007/06/01 16:12:25  mansueto
	Modifiche per compatibilitˆ QuarkCopyDesk
	
	Revision 1.16  2007/01/17 08:55:39  taretto
	compilazione su qxp70
	
	Revision 1.15  2006/10/04 13:14:03  taretto
	porting a QXP70
	
	Revision 1.14  2005/10/20 08:03:41  marchese
	modifiche gestione progetto UNIMEDIA su QXP411
	
	Revision 1.13  2005/06/09 14:19:56  taretto
	no message
	
	Revision 1.12  2004/11/22 17:07:14  marchese
	definizione della macro assert
	
	Revision 1.11  2004/05/25 13:29:54  marchese
	aggiunto la costante per il carattere di tab
	
	Revision 1.10  2003/07/28 07:56:32  marchese
	per compilare DAIPO sotto MacOS X
	
	Revision 1.9  2003/07/17 09:43:21  taretto
	aggiunti costanti parentesi angolari
	
	Revision 1.8  2003/05/07 07:16:21  marchese
	spostati gli standard includes nei file .h
	
	Revision 1.7  2003/02/27 16:22:42  marchese
	corretto un errore di definizione di una costante
	
	Revision 1.6  2003/02/06 13:42:39  marchese
	modifiche solo nei commenti per testare il funzionamento la di HeaderDoc (7.2).
	
	Revision 1.5  2002/12/02 11:06:09  marchese
	aggiunta la costante kNewLineChar per la gestione dei documenti dati sia MacOS sia WinOS
	
	Revision 1.4  2002/11/18 10:39:57  taretto
	correzione sulla keyword log
	
	Revision 1.3  2002/11/14 09:45:57  marchese
	tolto un ritorno a capo nella gestione delle keyword Log
	
	Revision 1.2  2002/11/14 09:37:48  marchese
	aggiunto la keyword Log
	
* ------------------------------------------------------------------------ */
