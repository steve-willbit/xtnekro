/*!
	@header			XTConfig.h
	@abstract 		costanti di precompilazione delle XTensions. 
	@discussion 	Questo header file contiene le principali costanti di precompilazione
					delle XTension.
	
					26 giugno 2002 - Stefano Marchese: creazione del file.
	
					Copyright © Sinedita S.r.l 2002. All Rights Reserved.

					$Log: not supported by cvs2svn $
*/


#if !defined(xtconfig_h)
#define xtconfig_h

// DEFINES
// DEFINES
// DEFINES

// only for cleaning
#undef _DEBUG
#undef NDEBUG
#undef _DEMO
#undef NDEMO

/*!
	@defined		_DEBUG
	@abstract		versione di debug.
	@discussion		Per abilitare anche la compilazione del codice della versione di debug.
*/
#define _DEBUG

/*!
	@defined		NDEMO
	@abstract		versione non demo.
	@discussion		Per disabilitare la compilazione della versione demo.
*/
#define NDEMO

#endif // !defined(xtconfig_h)

