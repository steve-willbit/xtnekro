/*!
	@header			QXPConfig.h
	@abstract 		costanti di precompilazione delle XTensions. 
	@discussion 	Questo header file contiene le principali costanti di precompilazione
					delle XTension Daipo per la gestione di Xpress 4.0 e Xpress 5.0.
	
					2 Ottobre 2002 - Stefano Marchese: creazione del file.
	
					Copyright © Sinedita S.r.l 2002. All Rights Reserved.
					
					$Log: not supported by cvs2svn $
					Revision 1.3  2006/02/27 09:45:16  mansueto
					introduzione della define per CopyDesk
					
					Revision 1.2  2003/06/06 09:32:59  marchese
					aggiunto la costante delle XT per QXP60
					
					Revision 1.1.1.1  2003/06/06 09:23:29  marchese
					importazione modulo QXP60
					
					Revision 1.1.1.1  2003/03/19 16:27:00  marchese
					spostamento dei file di configurazione in modo che siano comuni a tutte le XTs
					
*/


#if !defined(qxpconfig_h)
#define qxpconfig_h

// DEFINES
// DEFINES
// DEFINES

#undef QXP40
#undef QXP50
#undef QXP60
#undef QXP60
#undef QXP70
#undef QCD35

/*!
	@defined		QXP40
	@abstract		versione per QuarkXPress 4.0.
	@discussion		Per abilitare o disabilitare la compilazione per QuarkXPress 4.0.
*/

#define QXP40 0

/*!
	@defined		QXP50
	@abstract		versione per QuarkXPress 5.0.
	@discussion		Per abilitare o disabilitare la compilazione per QuarkXPress 5.0.
*/
#define QXP50 0

/*!
	@defined		QXP60
	@abstract		versione per QuarkXPress 6.0.
	@discussion		Per abilitare o disabilitare la compilazione per QuarkXPress 6.0.
*/
#define QXP60 1

/*!
	@defined		QXP70
	@abstract		versione per QuarkXPress 7.0.
	@discussion		Per abilitare o disabilitare la compilazione per QuarkXPress 6.0.
*/
#define QXP70 0

/*!
	@defined		QCD35
	@abstract		versione per QuarkCopyDesk 3.5.
	@discussion		Per abilitare o disabilitare la compilazione per QuarkCopyDesk 3.5.
*/
#define QCD35 0

#endif // !defined(qxpconfig_h)
