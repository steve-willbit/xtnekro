/*!
	@header			Stili.h
	@abstract 		paletta dell'XT.
	@discussion 		Contiene la descrizione delle procedure usate per eseguire l'applicazione
					degli stili di Xpress sul testo da importare

					14 Aprile 2005			Fabrizio

					Copyright © Sinedita S.r.l 2005. All Rights Reserved.
					
					$Log: not supported by cvs2svn $					
*/

#ifndef Stili_h
#define Stili_h

// TYPEDEFS
// TYPEDEFS
// TYPEDEFS

typedef struct 
{
	Boolean definito;
	Fixed spessore;
	uint8 stile;
	colorid colore;
	Fixed intensita;
	Fixed rientroSinistro;
	Fixed rientroDestro;
} caratteristichefiletto;

// EXTERN GLOBALS
// EXTERN GLOBALS
// EXTERN GLOBALS

// descrittore che contiene le caratteristiche del filetto
extern caratteristichefiletto gCaratteristicheFiletto;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES 

/*! 
	@function			ApplicaStileForte
	@abstract		gestione stili
	@discussion		Applica lo stile in modo forte alla fine del flusso di testo corrente. Nel
					caso lo stile indicato non sia presente applica lo stile di default definito
					all'interno di Xpress e stampa un messaggio di segnalazione sul file di
					rapporto

					15 Aprile 2005 - Fabrizio 

	@param			nomestile puntatore ad una stringa C che contiene il nome dello stile forte
					da applicare alla fine del flusso di testo corrente
	@result			nessuno
*/
void XTAPI ApplicaStileForte(uchar *nomestile) throw();

/*! 
	@function			ApplicaStileDebole
	@abstract		gestione stili
	@discussion		Applica lo stile in modo forte alla fine del flusso di testo corrente. Nel
					caso lo stile indicato non sia presente applica lo stile di default definito
					all'interno di Xpress e stampa un messaggio di segnalazione sul file di
					rapporto.

					15 Aprile 2005 - Fabrizio 

	@param			nomestile puntatore ad una stringa C che contiene il nome dello stile debole
					da applicare alla fine del flusso di testo corrente. 
	@result			nessuno
*/
void XTAPI ApplicaStileDebole(uchar *nomestile) throw();

/*! 
	@function			InizializzaErroreSulloStile
	@abstract		gestione stili
	@discussion		Inizializza l'errore sullo stile

					15 Aprile 2005 - Fabrizio 

	@param			nessuno 
	@result			nessuno
*/
void XTAPI InizializzaErroreSulloStile() throw();

/*! 
	@function			ImpostaCaratteristicheFiletto
	@abstract		gestione stili
	@discussion		Legge le caratteristiche del filetto superiore dello stile passato e le carica
					nella variabile globale relativa

					15 Aprile 2005 - Fabrizio 

	@param			nomestile puntatore ad una stringa C che contiene il nome dello stile che contiene
					 filetto superiore 
	@result			nessuno
*/
void XTAPI ImpostaCaratteristicheFiletto(uchar *nomestile) throw();

#endif // Stili_h