/*!
	@header			GestioneComandi.h
	@abstract		gestione dei comandi d'impaginazione
	@discussion 		Contiene la descrizione delle procedure usate per la gestione dei comandi
					di impaginazione
	
					14 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
*/
#ifndef GestioneComandi_h
#define GestioneComandi_h

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			PrendiCodiceDefunto
	@abstract		gestione comandi impaginazione
	@discussion		Restituisce il codice del defunto corrente

					14 Aprile 2005 - Fabrizio.

	@param			nessuno
	@result			ritorna la stringa C che contiene il codice del defunto corrente.
*/
uchar* XTAPI PrendiCodiceDefunto() throw();

/*! 
	@function			PrendiCodiceNecrologio
	@abstract		gestione comandi impaginazione
	@discussion		Restituisce il codice del necrologio corrente

					14 Aprile 2005 - Fabrizio.

	@param			nessuno
	@result			ritorna la stringa C che contiene il codice del necrologio corrente
*/
uchar* XTAPI PrendiCodiceNecrologio() throw();

/*! 
	@function			GestioneComandiBozzaSingoli
	@abstract		gestione comandi impaginazione
	@discussion		Per la gestione dei comandi di impaginazione sui documenti di bozza un
					necrologio per pagina

					14 Aprile 2005 - Fabrizio.

	@param			ptrquattrod puntatore al comando di impaginazione da gestire
	@result			nessuno
*/
void XTAPI GestioneComandiBozzaSingoli(quattrod *ptrquattrod) throw();

/*! 
	@function			GestioneComandiBozzaAggregati
	@abstract		gestione comandi impaginazione
	@discussion		Per la gestione dei comandi di impaginazione sui documenti di bozza aggregati

					14 Aprile 2005 - Fabrizio.

	@param			ptrquattrod puntatore al comando di impaginazione da gestire
	@result			nessuno
*/
void XTAPI GestioneComandiBozzaAggregati(quattrod *ptrquattrod) throw();

/*! 
	@function			GestioneComandiIngombro
	@abstract		gestione comandi impaginazione
	@discussion		Gestione dei comandi di impaginazione per il calcolo dell'ingombro

					14 Aprile 2005 - Fabrizio.

	@param			ptrquattrod puntatore al comando di impaginazione da gestire
	@result			nessuno
*/
void XTAPI GestioneComandiIngombro(quattrod *ptrquattrod) throw();

/*! 
	@function			GestioneComandiDefinitivoManuale
	@abstract		gestione comandi impaginazione
	@discussion		Gestione dei comandi di impaginazione defintiva manuale. I box vengono tutti
					messi sulla prima pagina senza alcuna particolare attenzione.
					Ci si preoccupa di creare tutte le informazioni necessarie per poi eseguire il
					posizionamento definitivo

					14 Aprile 2005 - Fabrizio.

	@param			ptrquattrod puntatore al comando di impaginazione da gestire
	@result			nessuno
*/
void XTAPI GestioneComandiDefinitivoManuale(quattrod *ptrquattrod) throw();

/*! 
	@function			GestioneComandiDefinitivoManuale
	@abstract		gestione comandi impaginazione
	@discussion		Per la gestione dei comandi di impaginazione definitivo sequenziale

					14 Aprile 2005 - Fabrizio.

	@param			ptrquattrod puntatore al comando di impaginazione da gestire
	@result			nessuno
*/
void XTAPI GestioneComandiDefinitivoSequenza(quattrod *ptrquattrod) throw();

/*! 
	@function			GestioneComandiDefinitivoManuale
	@abstract		gestione comandi impaginazione
	@discussion		Gestione dei comandi di impaginazione defintiva. E' la prima fase dell'impaginazione
					quando i box vengono tutti messi sulla prima pagina senza alcuna particolare attenzione.
					Ci si preoccupa di creare tutte le informazioni necessarie per poi eseguire il
					posizionamento definitivo

					14 Aprile 2005 - Fabrizio.

	@param			ptrquattrod puntatore al comando di impaginazione da gestire
	@param			ptrappware puntatore alla struttura da mettere sul box come slug
	@result			nessuno
*/
void GestioneComandiDefinitivoAutomatico(quattrod *ptrquattrod, appware *ptrappware) throw();

#endif // GestioneComandi_