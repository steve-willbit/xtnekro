/*!
	@header			DbpInclude.h
	@abstract 		DBP Include files.
	@discussion 	DBP Includes
	
					This file includes the QuarkXPress header files
					(its contents is only #include "XTInc40.h").

					You have to use this file "as is", only for C++ projects.
					Every file of the dbp411_dbp folder includes this file
	
					Copyright � Sinedita S.r.l 2002. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.16  2007/07/02 13:19:17  taretto
					aggiunta QXP70_s
					
					Revision 1.15  2007/05/23 16:57:54  mansueto
					Modifiche per la compilazione CopyDesk
					
					Revision 1.14  2007/02/23 09:45:09  taretto
					merge con porting QXP7
					
					Revision 1.13  2006/10/04 13:14:04  taretto
					porting a QXP70
					
					Revision 1.12  2006/05/08 18:37:18  taretto
					eliminati conflitti
					
					Revision 1.11  2006/02/27 15:05:11  mansueto
					import librerie e include per copydesk
					
					Revision 1.10  2003/08/06 15:08:54  marchese
					modifiche per compilare sotto MacOS X e gestire senza errori le MSL del C++
					
					Revision 1.9  2003/07/03 15:46:56  marchese
					modifica per ripristinare la compilazione su MacOS X
					
					Revision 1.8  2003/07/03 15:34:59  taretto
					prova di compilazione MacOSX
					
					Revision 1.7  2003/07/03 15:26:25  marchese
					modifiche per compilare su MacOS X
					
					Revision 1.6  2003/06/09 12:14:45  marchese
					- aggiunti gli include per QXP60;  - aggiunti la compilazione in base alla costante __cplusplus
					
					Revision 1.5  2003/05/07 07:16:23  marchese
					spostati gli standard includes nei file .h
					
					Revision 1.4  2002/11/18 10:39:58  taretto
					correzione sulla keyword log
					
					Revision 1.3  2002/11/14 09:45:57  marchese
					tolto un ritorno a capo nella gestione delle keyword Log
					
					Revision 1.2  2002/11/14 09:37:48  marchese
					aggiunto la keyword Log
					
*/

#if !defined(dbpinclude_h)
#define dbpinclude_h

// CONFIG INCLUDES
// CONFIG INCLUDES
// CONFIG INCLUDES

// always the first: they must be present in the project files
#include "XTConfig.h"
#include "QXPConfig.h"

// XPRESS INCLUDE
// XPRESS INCLUDE
// XPRESS INCLUDE

#if kOLD
#if QXP40
#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

	#include "XTInc40.h"

#if defined(__cplusplus)
} // extern "C"
#endif // defined(__cplusplus)
#endif // QXP40

#if QXP50
#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

	#include "XP_XtincE5.h"

#if defined(__cplusplus)
} // extern "C"
#endif // defined(__cplusplus)
#endif // QXP50

#if QXP60
#if defined(__MWERKS__) && defined(__MACH__)
	#define TARGET_API_MAC_OSX 1
	#include <MSL MacHeadersMach-O.h>
#endif // defined(__MWERKS__) && defined(__MACH__)

#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

	#include "XP_XtincE6.h"

#if DDS35
	#include "DDSXTension.h"
#endif // DDS35
	
#if defined(__cplusplus)
} // extern "C"
#endif // defined(__cplusplus)
#endif // QXP60

#if QCD35
#if defined(__MWERKS__) && defined(__MACH__)
	#define TARGET_API_MAC_OSX 1
	#include <MSL MacHeadersMach-O.h>
#endif // defined(__MWERKS__) && defined(__MACH__)

#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

	#include "CDXTinc.h"
	
typedef	int16		DocID;
typedef	int16		DocRef;
#define	INVALDOC	-1

#if defined(__cplusplus)
} // extern "C"
#endif // defined(__cplusplus)
#endif // QCD35

#if QXP70 || QXP70_s
#if defined(__MWERKS__) && defined(__MACH__)
	#define TARGET_API_MAC_OSX 1
	#include <MSL MacHeadersMach-O.h>
#elif __GNUC__
	#define TARGET_API_MAC_OSX 1
	#include <carbon/carbon.h> 
#endif // ifdef __MWERKS__

#ifdef __cplusplus
extern "C" {
#endif
#include "XP_XTIncE7.H"

#if QXP70_s
	#include "DDSXTension.h"
#endif // QXP70_s

#ifdef __cplusplus
}
#endif
#endif // QXP70 || QXP70_s
#endif // kOLD

#endif // !defined(dbpinclude_h)
