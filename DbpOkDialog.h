/*!
	@header			DbpOkDialog.h
	@abstract 		simple dialog message window.
	@discussion 	This header contains the definition of a simple dialog message
					window.
					
					Copyright © Sinedita S.r.l 2002. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.17  2007/02/23 09:45:09  taretto
					merge con porting QXP7
					
					Revision 1.16  2006/10/04 13:14:04  taretto
					porting a QXP70
					
					Revision 1.15  2005/03/01 11:05:36  taretto
					aggiunto comando chiudi al menu XTTB
					
					Revision 1.14  2004/11/22 17:07:15  marchese
					definizione della macro assert
					
					Revision 1.13  2003/08/06 15:08:54  marchese
					modifiche per compilare sotto MacOS X e gestire senza errori le MSL del C++
					
					Revision 1.12  2003/07/28 08:08:42  marchese
					per compilare DAIPO sotto MacOS X
					
					Revision 1.11  2003/07/04 16:21:38  marchese
					eliminato l'errore introdotto con il tentativo di compilazione sotto QXP60 e MACOS X
					
					Revision 1.10  2003/07/03 15:35:00  taretto
					prova di compilazione MacOSX
					
					Revision 1.9  2003/05/07 07:16:23  marchese
					spostati gli standard includes nei file .h
					
					Revision 1.8  2003/05/06 16:02:34  taretto
					spostati include di sistema sul .h
					
					Revision 1.7  2002/11/20 14:33:48  marchese
					aggiunti al progetto if file DbpOkDialog.cpp
					
					Revision 1.6  2002/11/18 10:39:59  taretto
					correzione sulla keyword log
					
					Revision 1.5  2002/11/14 09:45:58  marchese
					tolto un ritorno a capo nella gestione delle keyword Log
					
					Revision 1.4  2002/11/14 09:39:19  marchese
					aggiunto la keyword Log
	
*/


#if !defined(dbpokdialog_h)
#define dbpokdialog_h

// STANDARD INCLUDES
// STANDARD INCLUDES
// STANDARD INCLUDES

#if QXP60 || QCD35
#if defined(__MWERKS__) && defined(__MACH__)
	#define TARGET_API_MAC_OSX 1
	#include <MSL MacHeadersMach-O.h>
#endif // defined(__MWERKS__) && defined(__MACH__)
#endif // QXP60 || QCD35

#if QXP70
#if defined(__MWERKS__) && defined(__MACH__)
	#define TARGET_API_MAC_OSX 1
	#include <MSL MacHeadersMach-O.h>
#elif __GNUC__
	#define TARGET_API_MAC_OSX 1
	#include <carbon/carbon.h> 
#endif // ifdef __MWERKS__
#endif // QXP70

#include <string>

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"

// NAMESPACE DBP
// NAMESPACE DBP
// NAMESPACE DBP

namespace DBP
{

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

	/*! 
		@function		OkDialog
		@abstract		show a dialog message window.
		@discussion		This function displays a simple dialog with a message and the OK button.

						August 20th, 2002 - Stefano Marchese.

		@param			iString string to the message.
		@param			iType the type of the message dialog. The possibly values are, stopIcon,
						noteIcon, cautionIcon.
		@result			void.
	*/
	void XTAPI OkDialog(const std::string& iString, const int16 iType = stopIcon) throw();

	/*! 
		@function		OkDialog
		@abstract		show a dialog message window.
		@discussion		This function displays a simple dialog with a message and the OK button.

						August 20th, 2002 - Stefano Marchese.

		@param			iString pointer to the message. Must be a C string.
		@param			iType the type of the message dialog. The possibly values are, stopIcon,
						noteIcon, cautionIcon.
		@result			void.
	*/
	void XTAPI OkDialog(const char* iString, const int16 iType = stopIcon) throw();

	/*! 
		@function		OkDialog
		@abstract		show a dialog message window.
		@discussion		This function displays a simple dialog with a message and the OK button.

						August 27th, 2002 - Stefano Marchese.

		@param			iString pointer to the message. C string on Windows. Pascal string on Macintosh.
		@param			iType the type of the message dialog. The possibly values are, stopIcon,
						noteIcon, cautionIcon.
		@result			void.
	*/
	void XTAPI OkDialog(const uchar* iString, const int16 iType = stopIcon) throw();

	/*! 
		@function		LargeOkDialog
		@abstract		show a dialog message window.
		@discussion		This function displays a simple dialog with a message and the OK button.

						November 22th, 2004 - Stefano Marchese.

		@param			iString pointer to the message. C string on Windows. Pascal string on Macintosh.
		@param			iType the type of the message dialog. The possibly values are, stopIcon,
						noteIcon, cautionIcon.
		@result			void.
	*/
	void XTAPI LargerOkDialog(const uchar* iString, const int16 iType = stopIcon) throw();

	/*! 
		@function		OkCancelDialog
		@abstract		show a dialog message window.
		@discussion		This function displays a simple dialog with a message, the OK button and the CANCEL
						button.

						November 6th, 2002 - Stefano Marchese.

		@param			iString string to the message.
		@param			iType the type of the message dialog. The possibly values are, stopIcon,
						noteIcon, cautionIcon.
		@result			the id of the OK button or the id of the CANCEL button.
	*/
	int16 XTAPI OkCancelDialog(const std::string& iString, const int16 iType = noteIcon) throw();

	/*! 
		@function		OkCancelDialog
		@abstract		show a dialog message window.
		@discussion		This function displays a simple dialog with a message, the OK button and the CANCEL
						button.

						November 6th, 2002 - Stefano Marchese.

		@param			iString pointer to the message. Must be a C string.
		@param			iType the type of the message dialog. The possibly values are, stopIcon,
						noteIcon, cautionIcon.
		@result			the id of the OK button or the id of the CANCEL button.
	*/
	int16 XTAPI OkCancelDialog(const char* iString, const int16 iType = noteIcon) throw();

	/*! 
		@function		OkCancelDialog
		@abstract		show a dialog message window.
		@discussion		This function displays a simple dialog with a message, the OK button and the CANCEL
						button.

						November 6th, 2002 - Stefano Marchese.

		@param			iString pointer to the message. C string on Windows. Pascal string on Macintosh.
		@param			iType the type of the message dialog. The possibly values are, stopIcon,
						noteIcon, cautionIcon.
		@result			the id of the OK button or the id of the CANCEL button.
	*/
	int16 XTAPI OkCancelDialog(const uchar* iString, const int16 iType = noteIcon) throw();
	
	/*! 
		@function		OkNoCancelDialog
		@abstract		show a dialog message window.
		@discussion		This function displays a simple dialog with a message, the OK button and the CANCEL
						button.

						Febrary 1th, 2005 - Fabrizio Taretto.

		@param			iString pointer to the message. C string on Windows. Pascal string on Macintosh.
		@param			iType the type of the message dialog. The possibly values are, stopIcon,
						noteIcon, cautionIcon.
		@result			the id of the OK button or the id of the CANCEL button.
	*/
	int16 XTAPI OkNoCancelDialog(const uchar* iString, const int16 iType = noteIcon) throw();

} // namespace DBP

#endif // !defined(dbpokdialog_h)


