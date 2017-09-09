/*!
	@header			DbpConsts.h
	@abstract 		Costanti moduli DBP.
	@discussion 	Questo header file contiene tutte le definizioni delle costanti
					utilizzabili in una XT.
	
					Copyright © Sinedita S.r.l 2002. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.8  2003/07/17 09:43:21  taretto
					aggiunti costanti parentesi angolari
					
					Revision 1.7  2003/05/07 07:16:21  marchese
					spostati gli standard includes nei file .h
					
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
	
*/

#if !defined(dbpconsts_h)
#define dbpconsts_h

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"

// NAMESPACE DBP
// NAMESPACE DBP
// NAMESPACE DBP

namespace DBP
{

// GLOBALS
// GLOBALS
// GLOBALS

	/*!
		@const			kXpressFileExtension
		@abstract		estensione file.
		@discussion		Costante con l'estensione .QXD dei documenti QuarkXPress.
	*/
	extern const char kXpressFileExtension[];

	/*!
		@const			kPdfFileExtension
		@abstract		estensione file.
		@discussion		Costante con l'estensione .PDF dei documenti Adobe Acrobat.
	*/
	extern const char kPdfFileExtension[];

	/*!
		@const			kPdfFileExtension
		@abstract		estensione file.
		@discussion		Costante con l'estensione .PS dei documenti PostScript.
	*/
	extern const char kPsFileExtension[];

	/*!
		@const			kXmlFileExtension
		@abstract		estensione file.
		@discussion		Costante con l'estensione .XML dei documenti eXtensible Markup Language.
	*/
	extern const char kXmlFileExtension[];

	/*!
		@const			kJpegFileExtension
		@abstract		estensione file.
		@discussion		Costante con l'estensione .JPG dei documenti immagine JPEG.
	*/
	extern const char kJpegFileExtension[];

	/*!
		@const			kLogFileExtension
		@abstract		estensione file.
		@discussion		Costante con l'estensione .LOG per la stampa dei file di log dell'XT.
	*/
	extern const char kLogFileExtension[];
	
	/*!
		@const			kXtensionFolder
		@abstract		nomi cartelle.
		@discussion		Costante il nome della cartella dell'XTension di QuarkXPress "XTension".
	*/
	extern const char kXtensionFolder[];

#if MACOS
	/*!
		@const			kMenuSeparator
		@abstract		menu (solo MacOS).
		@discussion		Costante utilizzata per inserire il separatore tar i comandi di un menu'.
	*/
	extern const uchar kMenuSeparator[];
#endif // MACOS

	/*!
		@const			kReturnChar
		@abstract		caratteri.
		@discussion		Costante di ritorno a capo (codice ASCII esadecimale 0x0D).
	*/
	extern const char kReturnChar;

	/*!
		@const			kNewLineChar
		@abstract		caratteri.
		@discussion		Costante di nuova linea (codice ASCII esadecimale 0x0A).
	*/
	extern const char kNewLineChar;

	/*!
		@const			kTabChar
		@abstract		caratteri.
		@discussion		Costante per il carattere di tabulazione (codice ASCII esadecimale 0x09).
	*/
	extern const char kTabChar;

	/*!
		@const			kHyphenChar
		@abstract		caratteri.
		@discussion		Costante trattino '-'.
	*/
	extern const char kHyphenChar;

	/*!
		@const			kSlashChar
		@abstract		caratteri.
		@discussion		Costante slash '/'.
	*/
	extern const char kSlashChar;

	/*!
		@const			kBackSlashChar
		@abstract		caratteri.
		@discussion		Costante backslash '\'.
	*/
	extern const char kBackSlashChar;

	/*!
		@const			kSpaceChar
		@abstract		caratteri.
		@discussion		Costante di spazio ' '.
	*/
	extern const char kSpaceChar;

	/*!
		@const			kDotChar
		@abstract		caratteri.
		@discussion		Costante punto '.'.
	*/
	extern const char kDotChar;

	/*!
		@const			kCommaChar
		@abstract		caratteri.
		@discussion		Costante virgola ','.
	*/
	extern const char kCommaChar;

	/*!
		@const			kUnderlineChar
		@abstract		caratteri.
		@discussion		Costante underscore '_'.
	*/
	extern const char kUnderlineChar;
	
	/*!
		@const			kOpenAngular
		@abstract		caratteri.
		@discussion		Costante underscore '<'.
	*/
	extern const char kOpenAngular;
	
	/*!
		@const			kCloseAngular
		@abstract		caratteri.
		@discussion		Costante underscore '>'.
	*/
	extern const char kCloseAngular;
	

#if MACOS
	/*!
		@const			kEllipsisChar
		@abstract		caratteri (solo MacOS).
		@discussion		Costante puntini di sospensione.
	*/
	extern const char kEllipsisChar;
#endif // MACOS
	
	/*!
		@const			kMacDirSeparatorChar
		@abstract		separatori nomi cartelle.
		@discussion		Costante separatore nomi cartelle MacOS ':'.
	*/
	extern const char kMacDirSeparatorChar;

	/*!
		@const			kWinDirSeparatorChar
		@abstract		separatori nomi cartelle.
		@discussion		Costante separatore nomi cartelle WinOS '\'.
	*/
	extern const char kWinDirSeparatorChar;

	/*!
		@const			kUnixDirSeparatorChar
		@abstract		separatori nomi cartelle.
		@discussion		Costante separatore nomi cartelle UNIX '/'.
	*/
	extern const char kUnixDirSeparatorChar;

	/*!
		@const			kDirSeparatorChar
		@abstract		separatori nomi cartelle.
		@discussion		Costante separatore valido su MacOS/WinOS. Su MacOS vale ':'. Su WinOS vale '\'.
	*/
	extern const char kDirSeparatorChar;

} // namespace DBP

// USING NAMESPACE
// USING NAMESPACE
// USING NAMESPACE

using namespace DBP;

#endif // dbpconsts_h