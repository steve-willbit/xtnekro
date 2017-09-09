/*!
	@header			DbpResource.h
	@abstract 		define general resource functions.
	@discussion 	This header defines general XT resource functions.
	
					Copyright © Sinedita S.r.l 2002. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.23  2007/02/23 09:45:09  taretto
					merge con porting QXP7
					
					Revision 1.22  2006/10/04 13:14:04  taretto
					porting a QXP70
					
					Revision 1.21  2005/02/23 15:15:10  marchese
					modifiche per la lettura delle stringhe da risorse su una std::string
					
					Revision 1.20  2004/11/22 17:07:15  marchese
					definizione della macro assert
					
					Revision 1.19  2004/04/23 07:19:48  marchese
					corretto commento alla funzione UseXTensionResourceFile()
					
					Revision 1.18  2004/03/23 08:58:07  marchese
					creata la funzione DBP::UseXTensionResourceFile()
					
					Revision 1.17  2004/03/22 16:43:58  taretto
					porting sclass macos
					
					Revision 1.16  2003/09/11 14:52:40  marchese
					modifiche per compilare sotto MacOS X
					
					Revision 1.15  2003/09/05 13:25:00  taretto
					prova di compilazione MacOSX
					
					Revision 1.14  2003/08/06 15:08:54  marchese
					modifiche per compilare sotto MacOS X e gestire senza errori le MSL del C++
					
					Revision 1.13  2003/07/28 08:12:33  marchese
					per compilare DAIPO sotto MacOS X
					
					Revision 1.12  2003/07/15 12:59:27  taretto
					compilazione osx
					
					Revision 1.11  2003/07/03 15:35:00  taretto
					prova di compilazione MacOSX
					
					Revision 1.10  2003/06/13 15:15:52  marchese
					aggiunto il campo abstract nei commenti
					
					Revision 1.9  2003/05/19 10:10:06  taretto
					no message
					
					Revision 1.8  2003/05/12 10:28:18  taretto
					aggiunta "GetComboBoxRow" & "MakeStringForDialog"
					
					Revision 1.7  2003/05/07 07:16:24  marchese
					spostati gli standard includes nei file .h
					
					Revision 1.6  2002/11/20 14:34:18  marchese
					aggiunto al progetto if file DbpResource.cpp
					
					Revision 1.5  2002/11/18 10:39:59  taretto
					correzione sulla keyword log
					
					Revision 1.4  2002/11/14 09:45:58  marchese
					tolto un ritorno a capo nella gestione delle keyword Log
					
					Revision 1.3  2002/11/14 09:39:19  marchese
					aggiunto la keyword Log
					
*/

#if !defined(dbpresource_h)
#define dbpresource_h

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"

// DEFINES
// DEFINES
// DEFINES

/*!
	@define			LANGUAGE
	@abstract		language manager.
	@discussion		Builds a different resource ids base on the current Xpress language
*/
#define LANGUAGE (gXTensionLanguage * 100)

// GLOBALS
// GLOBALS
// GLOBALS

// the QuarkXPress language (only valid after XT_SETLANGUAGE call)
extern int16 gXTensionLanguage;

// NAMESPACE DBP
// NAMESPACE DBP
// NAMESPACE DBP

namespace DBP
{

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

#if MACOS
	/*! 
		@function		GetXTensionResourceFile
		@abstract		get the resource id of the xt.
		@discussion		Prende l'id del file delle risorse dell'XT. Deve essere chiamate al momento dell'avvio dell'XT
						ed e' necessaria solo sotto MACOS. Questa soluzione e per supplire un baco che si e' verificato
						durante lo sviluppo della versione 3.0.1. In futuro sarebbe auspicabile eliminare questa chiamata.

						7 November, 2002 - Stefano Marchese.

		@result			void.
	*/
	void XTAPI GetXTensionResourceFile() throw();
#endif // MACOS

#if MACOS
	/*! 
		@function		UseXTensionResourceFile
		@abstract		set the resource id of the xt.
		@discussion		Per impostare l'id della risorsa dell'xt. Funziona esclusivamente se e' stata utilizzata
						la funzione DBP::GetXTensionResourceFile() al momento dell'avvio dell'XTension.

						23 marzo 2004 - Stefano Marchese.

		@result			void.
	*/
	void XTAPI UseXTensionResourceFile() throw();
#endif // MACOS

	/*! 
		@function		GetResourceString
		@abstract		resource string.
		@discussion		Get a string from the resource.

						August 1st, 2002 - Stefano Marchese.

		@param 			s return the resource string. C string on Windows, Pascal string on Macintosh.
		@param 			r the Resource ID. On Mac the resource is 'STR#' type. On Windows the resource is
						'STRINGTABLE' type. 
		@param 			n the index of the string.
		@param 			sz the size of the string. Used only on Windows.
		@result			return an int16 value of the s length.
	*/
	int16 XTAPI GetResourceString(uchar* s, const int16 r, const int16 n, const int16 sz = 256) throw();

	/*! 
		@function		GetResourceString
		@abstract		resource string
		@discussion		Get a string from the resource.

						August 1st, 2002 - Stefano Marchese.

		@param 			s return the resource string. Always a C string.
		@param 			r the Resource ID. On Mac the resource is 'STR#' type. On Windows the resource is
						'STRINGTABLE' type. 
		@param 			n the index of the string.
		@param 			sz the size of the string. Used only on Windows.
		@result			return an int16 value of the s length.
	*/
	int16 XTAPI GetResourceString(char* s, const int16 r, const int16 n, const int16 sz = 256) throw();
		
	/*! 
		@function		GetResourceString
		@abstract		resource string
		@discussion		Get a string from the resource.

						August 1st, 2002 - Stefano Marchese.

		@param 			s return the resource string. Always a C string.
		@param 			r the Resource ID. On Mac the resource is 'STR#' type. On Windows the resource is
						'STRINGTABLE' type. 
		@param 			n the index of the string.
		@param 			sz the size of the string. Used only on Windows.
		@result			return an int16 value of the s length.
	*/
	int16 XTAPI GetResourceString(std::string& s, const int16 r, const int16 n, const int16 sz = 256) throw();

} // namespace DBP

// USING NAMESPACE
// USING NAMESPACE
// USING NAMESPACE

using namespace DBP;

#endif // !defined(dbpresource_h)
