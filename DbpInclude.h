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

// QXP2017
// QXP2017
// QXP2017

#include "Main.h"

// LEGACY: QXP60
// LEGACY: QXP60
// LEGACY: QXP60

extern "C" {
#include "XP_XTLegacy.h" 
#include "XP_XTLegacyE7.h" 
#include "XP_XTLegacyE6.h" 
}

// always the first: they must be present in the project files
#include "XTConfig.h"
#include "QXPConfig.h"

#endif // !defined(dbpinclude_h)
