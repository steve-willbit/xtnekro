/*!
	@header			XTConfig.h
	@abstract 		costanti di precompilazione delle XTensions 
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
	@defined		NDEBUG
	@abstract		Versione di release.
	@discussion		Per abilitare la compilazione dell'eventuale codice della versione di release.
*/
#define NDEBUG

/*!
	@defined		NDEMO
	@abstract		versione non demo.
	@discussion		Per abilitare la compilazione dell'eventuale codice della versione finale.
*/
#define NDEMO

#endif // !defined(xtconfig_h)

