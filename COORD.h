/*!
	@header			Coordinate.h
	@abstract		impaginazione annunci
	@discussion 		Contiene la descrizione delle procedure per la gestione delle coordinate
					per eseguire il posizionamento in pagina dei box dei necrologi
	
					11 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
*/

#ifndef Coord_h
#define Coord_h

// ENUMS
// ENUMS
// ENUMS

typedef enum 
{
	kNuovaColonna,
	kNuovaPagina
} tipospostamento;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			ImpostaCoordinateIniziali
	@abstract 		gestione info impaginazione.
	@discussion		Per impostare l'impaginazione sulla prima pagina. E' usata dall'impaginazione
					di BOZZA e con INGOMBRO poich‚ non si preoccupa di andare a verificare 
					ulteriori ingombri che non dovrebbero mai esserci.

					11 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI ImpostaCoordinateIniziali() throw();

/*!
	@function			ImpostaCoordinateInizialiDefinitivo
	@abstract 		gestione info impaginazione.
	@discussion		Per impostare l'impaginazione sulla prima pagina. E' usata dall'impaginazione
					DEFINITIVA impaginando i box fuori dalla pagina

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI ImpostaCoordinateInizialiDefinitivo() throw();

/*!
	@function			ImpostaCoordinateBoxImmagineCampione
	@abstract 		gestione info impaginazione.
	@discussion		Per impostare la creazione del box immagine campione

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI ImpostaCoordinateBoxImmagineCampione() throw();

/*!
	@function			InizializzaIngombroTotale
	@abstract 		gestione info impaginazione.
	@discussion		Per impostare l'ingombro a zero

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI InizializzaIngombroTotale() throw();

/*!
	@function			IncrementaIngombroTotale
	@abstract 		gestione info impaginazione.
	@discussion		Per calcolare l'ingombro totale dei necrologi

					12 Aprile 2005 - Fabrizio.

	@param 			ingombro valore con cui incrementare l'ingombro totale
	@result  			nessuno
*/
void XTAPI IncrementaIngombroTotale(Fixed ingombro) throw();

/*!
	@function			ImpostaIngombroTotale
	@abstract 		gestione info impaginazione.
	@discussion		Per impostare l'ingombro totale dei necrologi

					12 Aprile 2005 - Fabrizio.

	@param 			ingombro ingombro da impostare
	@result  			nessuno
*/
void XTAPI ImpostaIngombroTotale(uint32 ingombro) throw();

/*!
	@function			ImpostaIngombroTotale
	@abstract 		gestione info impaginazione.
	@discussion		Ritorna l'ingombro totale appena calcolato

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			ingombro
*/
uint32 PrendiIngombroTotale() throw();

/*!
	@function			InizializzaSpazioTraNecrologiTotale
	@abstract 		gestione info impaginazione.
	@discussion		Per impostare lo spazio tra i necrologi a zero

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void InizializzaSpazioTraNecrologiTotale() throw();

/*!
	@function			IncrementaSpazioTraNecrologiTotale
	@abstract 		gestione info impaginazione.
	@discussion		Per calcolare lo spazio tra i necrologi totale dei necrologi

					12 Aprile 2005 - Fabrizio.

	@param 			spaziotranecrologi spazio tra necrologi da sommare
	@result  			nessuno
*/
void XTAPI IncrementaSpazioTraNecrologiTotale(Fixed spaziotranecrologi) throw();

/*!
	@function			ImpostaSpazioTraNecrologiTotale
	@abstract 		gestione info impaginazione.
	@discussion		Per impostare lo spazio tra i necrologi totale dei necrologi

					12 Aprile 2005 - Fabrizio.

	@param 			spaziotranecrologi spazio tra necrologi da impostare
	@result  			nessuno
*/
void XTAPI ImpostaSpazioTraNecrologiTotale(uint32 spaziotranecrologi) throw();

/*!
	@function			PrendiSpazioTraNecrologiTotale
	@abstract 		gestione info impaginazione.
	@discussion		Ritorna lo spazio tra i necrologi totale appena calcolato

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			lo spazio tra i necrologi in long
*/
uint32 XTAPI PrendiSpazioTraNecrologiTotale() throw();

/*!
	@function			PrendiCoordinataAlta
	@abstract 		gestione info impaginazione.
	@discussion		Ritorna la coordinata alta corrente per posizionare i box dei necrologi

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			la coordinata in fixed
*/
Fixed XTAPI PrendiCoordinataAlta() throw();

/*!
	@function			PrendiCoordinataSinistra
	@abstract 		gestione info impaginazione.
	@discussion		Ritorna la coordinata sisnistra corrente per posizionare i box dei necrologi

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			la coordinata in fixed
*/
Fixed XTAPI PrendiCoordinataSinistra() throw();

/*!
	@function			PrendiPaginaCorrente
	@abstract 		gestione info impaginazione.
	@discussion		Restituisce la pagina corrente su cui effettuare le operazioni di impaginazione

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
int16 PrendiPaginaCorrente() throw();

/*!
	@function			PrendiColonnaCorrente
	@abstract 		gestione info impaginazione.
	@discussion		Restituisce la colonna corrente su cui effettuare le operazioni di impaginazione
	
					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
int16 XTAPI PrendiColonnaCorrente() throw();

/*!
	@function			PrendiContatoreBoxInColonna
	@abstract 		gestione info impaginazione.
	@discussion		Restituisce la colonna corrente su cui effettuare le operazioni di impaginazione
					Serve anche a capire quando e' necessario creare un box di testo collegato al
					precedente nell'impaginazione di BOZZA
					
					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
int16 XTAPI PrendiContatoreBoxInColonna() throw();

/*!
	@function			IncrementaContatoreBoxInColonna
	@abstract 		gestione info impaginazione.
	@discussion		Incrementa di uno il contatore dei box nella colonna corrente
					
					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI IncrementaContatoreBoxInColonna() throw();

/*!
	@function			AggiornaCoordinate
	@abstract 		gestione info impaginazione.
	@discussion		In base al box appena inserito aggiorna le coordinate per posizionare
					il box successivo. Questa procedura viene usata nella BOZZA, INGOMBRO.
					L'aggiornamento viene effettuato in base al box correntemente
					selezionato
					
					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI AggiornaCoordinate() throw();

/*!
	@function			CambiaCoordinate
	@abstract 		gestione info impaginazione.
	@discussion		Aggiorna le coordinate nel caso in cui sia necessario cambiare colonna. Nel caso
					sia necessario cambiare pagina si preoccupa di verificare se e' necessario inserire
					una nuova pagina alla fine del documento corrente. Questa procedura viene
					usata per la BOZZA perch‚ prevede di non tenere conto degli ingombri
					presenti sul documento e perch‚ inserisce una nuova pagina se e' necessario.
					
					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			ritorna se devo spostare il box corrente su una nuova colonna o su una nuova
					pagina
*/
tipospostamento XTAPI CambiaCoordinate() throw();

/*!
	@function			InserisciNuovaPagina
	@abstract 		gestione info impaginazione.
	@discussion		Inserisco una nuova pagina e aggiorno le coordinate. Questa procedura Š usata
					durante l'impaginazione di BOZZA per permettere di iniziare su una nuova
					pagina quando si verifica un cambio di defunto.
					
					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI InserisciNuovaPagina() throw();

/*!
	@function			DecrementaColonna
	@abstract 		gestione info impaginazione.
	@discussion		Usata nell'INGOMBRO e nel DEFINITIVO. Per gestire box di testo pi— lunghi
					di una colonna. Sposta la coordinata sinistra di una colonna.
					
					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI DecrementaColonna() throw();

/*!
	@function			DecrementaColonna
	@abstract 		gestione info impaginazione.
	@discussion		Usata nell'INGOMBRO e nel DEFINITIVO. Per gestire box di testo pi— lunghi
					di una colonna. Sposta la ccordinata sinistra di una colonna.
					
					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI IncrementaColonna() throw();

/*!
	@function			PrimaColonnaIngombro
	@abstract 		gestione info impaginazione.
	@discussion		Usata nell'INGOMBRO. Alla fine della gestione dei box di
					testo piu' lunghi di una colonna riporta la ccordina sinistra alla prima
					colonna.
					
					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI PrimaColonnaIngombro() throw();

/*!
	@function			PrimaColonnaDefinitivo
	@abstract 		gestione info impaginazione.
	@discussion		Usata nel DEFINITIVO. Alla fine della gestione dei box di
					esto piu' lunghi di una colonna riporta la ccordina sinistra furoi pagina.
					
					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI PrimaColonnaDefinitivo() throw();

#endif // Coord_h