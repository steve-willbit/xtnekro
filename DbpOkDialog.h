/*!
	@header			DbpOkDialog.h
	@abstract 		simple dialog message window.
	@discussion 	This header contains the definition of a simple dialog message
					window.
					
					Copyright © Sinedita S.r.l 2002. All Rights Reserved.

*/


#if !defined(dbpokdialog_h)
#define dbpokdialog_h

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


