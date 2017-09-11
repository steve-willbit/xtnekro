/*!
	@header			DbpPref.h
	@abstract 		define general resource functions.
	@discussion 	This header defines general XT resource functions.
	
					Copyright © Sinedita S.r.l 2002. All Rights Reserved.
					
*/

#if !defined(dbppref_h)
#define dbppref_h

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
		@function		SavePalettePos
		@abstract		salva la posizione della palette.
		@discussion		Alla chiusura di una palette ne memeorizza la posizione in modo da poterla riaprire nello stesso punto
						in cui e' stata chiusa.

						25 marzo 2003 - Fabrizio Taretto.

		@param			iWLocHandle handle posizione della palette.
		@param			iShowingFlag flag per sapere se la palette è aperta o chiusa.	
		@result			void.
	*/

	void XTAPI SavePalettePos
	(
#if 0 // QXP70 
		const uchar* iPrefNameKey,
		const OSType iResID,
#endif // 0 // QXP70
		const wlochandle iWLocHandle, 
		const bool8 iShowingFlag
	) throw();
	/*! 
		@function		LoadPalettePos
		@abstract		carica la posizione della palette.
		@discussion		Carica la posizione della palette per poterla visualizzare nella posizione in cui era stata chiusa.

						25 marzo 2003 - Fabrizio Taretto.

		@param			iWLocHandle handle posizione della palette.
		@param			iShowingFlag flag per sapere se la palette è aperta o chiusa.
		@param			iResID id della risorsa della palette -solo MacOS-
		@param			iNameKey nome della palette -solo WinOS-
		@result			void.
	*/	
	void XTAPI LoadPalettePos
(
#if 0 // QXP70
	const uchar* iPrefNameKey,
#endif // 0 QXP70
	wlochandle& ioWLocHandle,
	bool8& ioShowingFlag,
#if MACOS // 
	const OSType iResID,
#endif // MACOS 
	const uchar* iNameKey
) throw();

#if 0
	/*! 
		@function		SavePreferences
		@abstract		salva le preferenze.
		@discussion		salva le preferenze associate all'XT.

						25 marzo 2003 - Fabrizio Taretto.

		@param			iPrefPtr puntatore alla struttura contenente le preferenze.
		@param			iPrefSize dimensione delle preferenze.
		@param			iPrefNameKey nome della stringa utilizzata come chiave di registro -solo WinOs-.
		@param			iPrefRegKey chiave di registro in cui salvare le preferenze dell'XT -solo WinOs-.
		@param			iPrefResType tipo di risorsa in cui salvare le preferenze -solo MacOS-.
		@param			iPrefResID id della risorsa in cui salvare le preferenze -solo MacOS-.	
		@result			void.
	*/
	void XTAPI SavePreferences
	(
		const Ptr iPrefPtr,
		const Size iPrefSize,
		const uchar* iPrefNameKey,
#if WINOS
		const uchar* iPrefRegKey
#endif // WINOS
#if MACOS
		const OSType iPrefResType,
		const OSType iPrefResID
#endif // MACOS
	) throw();


	/*! 
		@function		LoadPreferences
		@abstract		carica le preferenze.
		@discussion		carica le preferenze associate all'XT.

						25 marzo 2003 - Fabrizio Taretto.

		@param			iPrefPtr puntatore alla struttura contenente le preferenze.
		@param			iPrefSize dimensione delle preferenze.
		@param			iPrefNameKey nome della stringa utilizzata come chiave di registro.
		@param			iPrefRegKey chiave di registro in cui salvare le preferenze dell'XT -solo WinOs-.
		@param			iPrefResType tipo di risorsa in cui salvare le preferenze -solo MacOS-.
		@param			iPrefResID id della risorsa in cui salvare le preferenze -solo MacOS-.	
		@result			void.
	*/
	void XTAPI LoadPreferences
	(
		Ptr iPrefPtr,
		const Size iPrefSize,
		const uchar* iPrefNameKey,
#if WINOS
		const uchar* iPrefRegKey
#endif // WINOS
#if MACOS
		const OSType iPrefResType,
		const OSType iPrefResID
#endif // MACOS
	) throw();
#endif // 0

	/*! 
		@function		SavePreferences
		@abstract		salva le preferenze.
		@discussion		salva le preferenze associate all'XT.

						25 marzo 2003 - Fabrizio Taretto.

		@param			iPrefPtr puntatore alla struttura contenente le preferenze.
		@param			iPrefSize dimensione delle preferenze.
		@param			iPrefNameKey nome della stringa utilizzata come chiave di registro -solo WinOs-.
		@param			iPrefResType tipo di risorsa in cui salvare le preferenze -solo MacOS-.
		@param			iPrefResID id della risorsa in cui salvare le preferenze -solo MacOS-.	
		@result			void.
	*/
	void XTAPI SavePreferences
	(
	const Ptr iPrefPtr,
	const Size iPrefSize,
	const uchar* iPrefNameKey,
	const OSType iPrefResType,
	const OSType iPrefResID
) throw();

	
	/*! 
		@function		LoadPreferences
		@abstract		carica le preferenze.
		@discussion		carica le preferenze associate all'XT.

						25 marzo 2003 - Fabrizio Taretto.

		@param			iPrefPtr puntatore alla struttura contenente le preferenze.
		@param			iPrefSize dimensione delle preferenze.
		@param			iPrefNameKey nome della stringa utilizzata come chiave di registro.
		@param			iPrefRegKey chiave di registro in cui salvare le preferenze dell'XT -solo WinOs-.
		@param			iPrefResType tipo di risorsa in cui salvare le preferenze -solo MacOS-.
		@param			iPrefResID id della risorsa in cui salvare le preferenze -solo MacOS-.	
		@result			void.
	*/
	void XTAPI LoadPreferences
	(
		Ptr iPrefPtr,
		const Size iPrefSize,
		const OSType iPrefResType,
		const OSType iPrefResID,
		const uchar* iPrefName
	) throw();
} // namespace DBP

// USING NAMESPACE
// USING NAMESPACE
// USING NAMESPACE

using namespace DBP;

#endif // !defined(dbppref_h)
