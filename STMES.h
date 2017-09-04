/*!
	@header			StampaMessaggi.h
	@abstract		rapporto
	@discussion 		Contiene una procedura per stampare una stringa formattata sul file di
					rapporto delle operazioni eseguite dall'Xtension e una procedura per cancellare
					il file. Dato che le stringhe usate sul file di report sono memorizzate
					nel file di risorse dell'Xtension e' necessario richiamare la procedura che carica
					le stringhe prima di usare la procedura di stampa.
	
					5 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
*/
#ifndef StampaMessaggi_h
#define StampaMessaggi_h

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			StampaSuRapporto
	@abstract 		gestione file report.
	@discussion		stampa sul file di rapporto

					6 Aprile 2005		Fabrizio.

	@param 			stringa stringa C da stampare
	@result  			nessuno
*/
void XTAPI StampaSuRapporto(char *stringa) throw();
void XTAPI StampaSuRapporto(uchar *stringa) throw();

/*!
	@function			ConcatenaAllaStringaMessaggio
	@abstract 		gestione file report.
	@discussion		Concatena alla stringa gStringaMessaggio la stringa passata

					6 Aprile 2005		Fabrizio.

	@param 			stringamessaggio messaggio da aggiungere alla variabile gStringaMessaggio
	@result  			nessuno
*/
void XTAPI ConcatenaAllaStringaMessaggio(uchar *stringamessaggio) throw();

/*!
	@function			StampaMessaggio
	@abstract 		gestione file report.
	@discussion		Stampa il messaggio passato sul file del rapporto

					6 Aprile 2005		Fabrizio.

	@param 			messaggio codice del messaggio da stampare
	@result  			nessuno
*/
void XTAPI StampaMessaggio(messaggixtension messaggio) throw();

/*!
	@function			VisualizzaMessaggio
	@abstract 		gestione file report.
	@discussion		Visualizza il messaggio passato sulla paletta dell'Xtension

					6 Aprile 2005		Fabrizio.

	@param 			messaggio codice del messaggio da stampare
	@result  			nessuno
*/
void XTAPI VisualizzaMessaggio(messaggixtension messaggio) throw();

/*!
	@function			DaiMessaggio
	@abstract 		gestione file report.
	@discussion		Stampa il messaggio passato sul file di report e sulla paletta dell'Xtension

					6 Aprile 2005		Fabrizio.

	@param 			messaggio codice del messaggio da stampare
	@result  			nessuno
*/
void XTAPI DaiMessaggio(messaggixtension messaggio) throw();

#endif // StampaMessaggi_h 
