/*!
	@header			Impaginazione.h
	@abstract		impaginazione annunci
	@discussion 		Contiene le descrizioni delle procedure usate per eseguire l'impaginazione
					dei necrologi
	
					11 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
*/
#ifndef Impaginazione_h
#define Impaginazione_h

// PROTOTYPES 
// PROTOTYPES
// PROTOTYPES

/*!
	@function			StampaIngombro
	@abstract 		gestione impaginazione.
	@discussion		Stampa dei messaggi dell'ingombro appena calcolato

					11 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI StampaIngombro() throw();

/*!
	@function			StampaIngombro
	@abstract 		gestione impaginazione.
	@discussion		E' la procedura che si preoccupa di eseguire la scansione di tutto il
					documento da impaginare e di creare le strutture dati necessarie per
					eseguire l'impaginazione.
					Per determinare se si Š finito di analizzare l'handle non ci si basa
					sulla sua dimensione, ma sul fatto che sia terminato con un carattere di
					fine file '\0'. All'inizio, proprio per questo motivo l'handle viene
					aumentato di una dimensione per aggiungere questo carattere.
					Gli errori di sintassi del file da impaginare non sono bloccanti e non
					fanno uscire da Xpress.
					Se si tratta di una impaginazione di bozza o per il calcolo
					dell'ingombro si torna ad aspettare file da 4D.
					Nel caso in cui si tratti di impaginazione definitiva viene disabilitata
					ogni tipo di attesa fino a quando l'utente non riabilita l'impaginazione.

					14 Aprile 2005 - Fabrizio.

	@param 			handle puntatore all'handle che contiene il documento da impaginare
	@result  			ritorna l'eventuale errore dell'Xtension
*/
errorixtension XTAPI Impagina(Handle handle) throw();

#pragma mark -
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

errorixtension ImpaginaAppWare(Handle *handle);

#endif // // NECRO v1
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif // Impaginazione_h 
