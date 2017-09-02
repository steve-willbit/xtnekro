/*!
	@header			XTNecro.h
	@abstract		main
	@discussion 		Include del main del progetto.
	
					22 Marzo 2005 - Fabrizio.
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.2  2005/05/09 12:36:45  taretto
					aggiunta protezione HW
					
					Revision 1.1  2005/04/20 07:14:23  taretto
					importazione files
							
*/

#ifndef XTNecro_h
#define XTNecro_h

#ifndef Errori_h
#include "Errori.h"
#endif // Errori_h

// messaggi realtivi alla protezione
#define NOPROTECTIONMESSAGE {\
	char msg[256] = "";\
	std::ostringstream s;\
	s << "PROTEZIONE HARDWARE NON TROVATA: impossibile lavorare con XTNecro.";\
	DBP::OkDialog(s.str());\
}

// messaggio di attenzione per protezione disabilitata
#define PROTECTIONDISABLEMESSAGE {\
	char msg[256] = "";\
	std::ostringstream s;\
	s << "PROTEZIONE HARDWARE DISABILITATA: XTNecro sara' caricata correttamente.";\
	DBP::OkDialog(s.str());\
}

// stringa memorizzata memoria della chiave HASP4 USB
#define XTNECROHASPSTR "SPAGNA 160398"

// CONSTS
// CONSTS
// CONSTS

// per tornare alla prima pagina
#define kPrimaPagina 1

// pagine mastro
#define kPaginaMastroBianca 1
#define kPaginaMastroA 3

// per visualizzare il documento all’impaginazione
// per visualizzare il documento all'impaginazione
#define kVistaImpaginazioneBozze (65536 / 5)
#define kVistaImpaginazioneIngombro (65536 * 8)
#define kVistaImpaginazioneDefinitivo (65536 * 8)
#define kVistaControlloImmagini (65536 / 5)

// massimo long
#define kMaxUnsignedLong (4294967295)
#define kMaxLong (2147483647)
#define kMaxDoubleFixed (1.7e308)

// costanti per i tipi
#define EPS 0x00535045L
#define TIFF 0x00464954L
#define BMP 0x00504D42L
#define XTG 0x00475458L

// GLOBALS
// GLOBALS
// GLOBALS

// l'indirizzo di base per la gestione delle WAP
extern int32 gXtWapCbCode;

// volume di partenza di QuarkXpress
extern int16 gNumeroVolumeXpress;

// per prendere il nome della cartella di Xpress
extern uchar gNomeCartellaXpress[];

// FSSpec cartella XTension
extern FSSpec gFSSpecXtension;

// stringa di uso generale
extern uchar gStringaC[MAXPATHNAME];

// contiene errore dell'XT
extern errorixtension gErroreXtension;

// TYPEDEF
// TYPEDEF
// TYPEDEF

typedef double DoubleFixed;

// MACROS
// MACROS
// MACROS

// per la conversione da long a fixed
#define mLong(x) ((long) x >> 16)
#define mResto(x) ((long) x % 65536)
#define mFixed(x) ((long) x << 16)
#define mDoubleFixed2Long(x) (x / (DoubleFixed) 65536)

// DEFINES
// DEFINES
// DEFINES

// per definire la dimensione delle variabili che contengono le stringhe
#define kDimensioneStringhe 256

// per definire lunghezza codice defunto
#define kLunghezzaCodice 256


/*!
	@defined			NUMBEROFWAPS
	@abstract		number of waps of this XTension.
	@discussion		Defines the number of the XTension WAPS.
*/
#define NUMBEROFWAPS 15

/*!
	@defined			_XT_PREFERENZEWAP
	@abstract		WAP.
	@discussion		Il cbcode per la finestra delle preferenze
*/
#define _XT_PREFERENZEWAP gXtWapCbCode

/*!
	@defined			_XT_IMPAGINAZIONEWAP
	@abstract		WAP.
	@discussion		Il cbcode per la finestra impaginazione
*/
#define  _XT_IMPAGINAZIONEWAP (gXtWapCbCode + 1)

/*!
	@defined			_XT_GIUSTIFICAZIONEWAP
	@abstract		WAP.
	@discussion		Il cbcode per la finestra giustificazione
*/
#define  _XT_GIUSTIFICAZIONEWAP (gXtWapCbCode + 2)

/*!
	@defined			_XT_RIEMPITIVIWAP
	@abstract		WAP.
	@discussion		Il cbcode per la finestra riempitivi
*/
#define  _XT_RIEMPITIVIWAP (gXtWapCbCode + 3)

/*!
	@defined			_XT_IMMAGINIWAP
	@abstract		WAP.
	@discussion		Il cbcode per la finestra immagini 
*/
#define  _XT_IMMAGINIWAP (gXtWapCbCode + 4)

/*!
	@defined			_XT_BOZZAWAP
	@abstract		WAP.
	@discussion		Il cbcode per la finestra bozza 
*/
#define  _XT_BOZZAWAP (gXtWapCbCode + 5)

/*!
	@defined			_XT_INGOMBROWAP
	@abstract		WAP.
	@discussion		Il cbcode per la finestra ingombro
*/
#define  _XT_INGOMBROWAP (gXtWapCbCode + 6)

/*!
	@defined			_XT_FINALEWAP
	@abstract		WAP.
	@discussion		Il cbcode per la finestra finale 
*/
#define  _XT_FINALEWAP (gXtWapCbCode + 7)

/*!
	@defined			_XT_DOCUMENTOWAP
	@abstract		WAP.
	@discussion		Il cbcode per la finestra documento 
*/
#define  _XT_DOCUMENTOWAP (gXtWapCbCode + 8)

/*!
	@defined			_XT_DATIWAP
	@abstract		WAP.
	@discussion		Il cbcode per la finestra dati 
*/
#define  _XT_DATIWAP (gXtWapCbCode + 9)

/*!
	@defined			_XT_DISPOSIZIONEWAP
	@abstract		WAP.
	@discussion		Il cbcode per la finestra disposizione
*/
#define  _XT_DISPOSIZIONEWAP (gXtWapCbCode + 10)

/*!
	@defined			_XT_CARTELLEWAP
	@abstract		WAP.
	@discussion		Il cbcode per la finestra prepara documento
*/
#define  _XT_CARTELLEWAP (gXtWapCbCode + 11)

/*!
	@defined			_XT_PREPARADOCWAP
	@abstract		WAP.
	@discussion		Il cbcode per la finestra prepara documento
*/
#define  _XT_PREPARADOCWAP (gXtWapCbCode + 12)

/*!
	@defined			_XT_PREPARADOCWAP
	@abstract		WAP.
	@discussion		Il cbcode per la palette
*/
#define  _XT_INFOPALETTEWAP (gXtWapCbCode + 13)

/*!
	@defined			_XT_ABOUTWAP
	@abstract		WAP.
	@discussion		Il cbcode per la palette
*/
#define _XT_ABOUTWAP (gXtWapCbCode + 14)

#endif // XTNecro_h