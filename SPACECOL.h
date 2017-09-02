/*!
	@header			SpazioColonna.h
	@abstract		gestione spazio colonna
	@discussion 		Contiene le procedure usate per calcolare i margini su un documento con
					box gia' posizionati
	
					12 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
*/
#ifndef SpazioColonna_h
#define SpazioColonna_h

// ENUMS
// ENUMS
// ENUMS

typedef enum 
{
	kNessunaChiusura,
	kPreferenzeRispettate,
	kInserireAncora,
	kPreferenzeNonRispettate
} tipochiusura;

typedef enum 
{
	kAperta,
	kQuasiChiusa,
	kChiusa
} chiusuracolonna;

// STRUCT
// STRUCT
// STRUCT

typedef struct 
{
	int16 pagina;
	int16 colonna;
	Fixed margineAlto;
	Fixed margineBasso;
	Fixed spazioDisponibile;
	Fixed spazioOttimale;
	Fixed spazioOccupato;
	Fixed spazioTraNecrologi;
	Fixed spazioTraParagrafi;
	int32 numeroParagrafiDiversi;
	int32 numeroParagrafi;
	int32 numeroLinee;
	int16 numeroNecrologi;
	int16 numeroNecrologiDefunti;
	int16 numeroNecrologiFondoDefunti;
	chiusuracolonna	chiusuraColonna;
	tipochiusura	tipoChiusura;
	necrologio *testaNecrologi;
	necrologio *codaNecrologi;
	defunto *testaDefunti;
	defunto *codaDefunti;
	defunto *testaFondoDefunti;
	defunto *codaFondoDefunti;
} spaziocolonna, *spaziocolonnaPtr, **spaziocolonnaHandle ;

// EXTERN GLOBALS
// EXTERN GLOBALS
// EXTERN GLOBALS


// memorizza per ogni colonna lo spazio disponibile, lo spazio ottimale e lo spazio occupato
// per permettere la disposizione dei necrologi
extern spaziocolonnaPtr gPtrSpazioColonna;

// per memorizzare i box fuori pagina
extern spaziocolonnaPtr gPtrSpazioColonnaFuori;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			PrendiSpazioColonna
	@abstract		gestisce info colonna.
	@discussion		Prende lo spazio colonna in base alla pagina e alla colonna passata

					13 Aprile 2005 - Fabrizio Taretto.

	@param			pagina - pagina di cui si vuole le info sullo spazio colonna
	@param			colonna colonna su cui si vuole le info sullo spazio colonna
	@result			ritorna il puntatore allo spazio colonna desiderato.
*/
spaziocolonna* XTAPI PrendiSpazioColonna(int16 pagina, int16 colonna) throw();

/*! 
	@function			PrendiSpazioColonnaFuori
	@abstract		gestisce info colonna.
	@discussion		Prende lo spazio colonna per i box che non sono riuscito a mettere nella pagina

					13 Aprile 2005 - Fabrizio Taretto.

	@param			nessuno
	@result			ritorna il puntatore allo spazio colonna desiderato
*/
spaziocolonna* XTAPI PrendiSpazioColonnaFuori() throw();

/*! 
	@function			PrendiSpazioOttimaleDiPagina
	@abstract		gestisce info colonna.
	@discussion		Prende lo spazio ottimale di pagina in base alla pagina e alla colonna passata.
					Questo valore non prende in cosiderazione la colonna passata.

					13 Aprile 2005 - Fabrizio Taretto.

	@param			pagina pagina di cui si vuole lo spazio disponibile di pagina
	@param			colonna colonna su cui si vuole lo spazio disponibile di pagina
	@result			ritorna lo spazio ottimale di pagina
*/
Fixed XTAPI PrendiSpazioOttimaleDiPagina(int16 pagina, int16 colonna) throw();

/*! 
	@function			GetOptimunAvailableSpaceFromDocument
	@abstract		Get the available space from the whole document.
	@discussion		<b>PURPOSE:</b> gets the available space through the entire current document.<br>
					<b>REQUIRE:</b> uses the number of the page and the number of the column where
					to start the sum of the available space.<br>
					<b>PROMISE:</b> returns the space in double variable but in Fixed notation.

					March 20th, 2002 - Stefano Marchese.

	@param 			iPage the number of the page where to start the sum of the space available.
					The start page is included.
	@param 			iColumn the number of the column where to start the sum of the space available.
					The start column is included in the sum.
	@result			the space available in a double variable but in Fixed notation.
*/
DoubleFixed XTAPI GetOptimunAvailableSpaceFromDocument(int16 iPage, int16 iColumn) throw();

/*! 
	@function			CalcolaSpazioOttimale
	@abstract		gestisce info colonna.
	@discussion		Calcola lo spazio ottimale per ogni colonna in base allo spazio
	disponibile 		e all'ingombro appena calcolato

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			nessuno
	@result			ritorna l'eventuale errrore dell'XT
*/
errorixtension XTAPI CalcolaSpazioOttimale() throw();

/*! 
	@function			InizializzaAllargamentoTotale
	@abstract		gestisce info colonna.
	@discussion		Per impostare l'allargamento a zero

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			nessuno
	@result			nessuno
*/
void XTAPI InizializzaAllargamentoTotale() throw();

/*! 
	@function			IncrementaAllargamentoTotale
	@abstract		gestisce info colonna.
	@discussion		Per calcolare l'allargamento totale dei necrologi

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			allargamento allargamento da sommare
	@result			nessuno
*/
void XTAPI IncrementaAllargamentoTotale(Fixed allargamento) throw();

/*! 
	@function			ImpostaAllargamentoTotale
	@abstract		gestisce info colonna.
	@discussion		Per impostare l'allargamento totale dei necrologi

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			allargamento allargamento da impostare
	@result			nessuno
*/
void XTAPI ImpostaAllargamentoTotale(uint32 allargamento) throw();

/*! 
	@function			ImpostaAllargamentoTotale
	@abstract		gestisce info colonna.
	@discussion		Ritorna l'allargamento totale appena calcolato

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			nessuno
	@result			l'allargamento in long
*/
uint32 PrendiAllargamentoTotale() throw();

/*! 
	@function			InizializzaRestringimentoTotale
	@abstract		gestisce info colonna.
	@discussion		Per impostare il restringimento a zero

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			nessuno
	@result			l'allargamento in long
*/
void XTAPI InizializzaRestringimentoTotale() throw();

/*! 
	@function			IncrementaRestringimentoTotale
	@abstract		gestisce info colonna.
	@discussion		Per calcolare il restringimento totale dei necrologi

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			restringimento restringimento da sommare
	@result			nessuno
*/
void XTAPI IncrementaRestringimentoTotale(Fixed restringimento) throw();

/*! 
	@function			ImpostaRestringimentoTotale
	@abstract		gestisce info colonna.
	@discussion		Per impostare il restringimento totale dei necrologi

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			estringimento restringimento da impostare
	@result			nessuno
*/
void XTAPI ImpostaRestringimentoTotale(uint32 restringimento) throw();

/*! 
	@function			PrendiRestringimentoTotale
	@abstract		gestisce info colonna.
	@discussion		Ritorna il restringimento totale appena calcolato

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			nessuno
	@result			il restringimento in long
*/
uint32 PrendiRestringimentoTotale() throw();

/*! 
	@function			LiberaNecrologiSpazioColonna
	@abstract		gestisce info colonna.
	@discussion		Dopo aver eseguito il posizionamento libero lo spazio della colonna

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			nessuno
	@result			nessuno
*/
void XTAPI LiberaNecrologiSpazioColonna() throw();

/*! 
	@function			LiberaNecrologiSpazioColonnaFuori
	@abstract		gestisce info colonna.
	@discussion		Dopo aver eseguito il posizionamento libero lo spazio della colonna fuori

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			nessuno
	@result			nessuno
*/
void XTAPI LiberaNecrologiSpazioColonnaFuori() throw();

/*! 
	@function			PrendiSpazioDisponibileTotale
	@abstract		gestisce info colonna.
	@discussion		Ritorna lo spazio disponibile totale appena calcolato

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			nessuno
	@result			ritorno l'eventuale errore dell'xtensione
*/
uint32 PrendiSpazioDisponibileTotale() throw();

/*! 
	@function			InizializzaSpazioDisponibileTotale
	@abstract		gestisce info colonna.
	@discussion		Per impostare l'allargamento a zero

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			nessuno
	@result			nessuno
*/
void XTAPI InizializzaSpazioDisponibileTotale() throw();

/*! 
	@function			IncrementaSpazioDisponibileTotale
	@abstract		gestisce info colonna.
	@discussion		Per calcolare l'allargamento totale dei necrologi

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			spaziodisponibile spazio da sommare
	@result			nessuno
*/
void XTAPI IncrementaSpazioDisponibileTotale(Fixed spaziodisponibile) throw();

/*! 
	@function			PrendiAltezzaColonnaOttimale
	@abstract		gestisce info colonna.
	@discussion		Ritorna l'altezza della colonna ottimale calcota dalla procedura
					CalcolaSpazioOttimale()

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			nessuno
	@result			ritorna l'altezza in Fixed
*/
Fixed XTAPI PrendiAltezzaColonnaOttimale() throw();

/*! 
	@function			CalcolaPagineColonneEModuli
	@abstract		gestisce info colonna.
	@discussion		Trasforma in pagine, colonne e moduli uno spazio

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			spazio spazio da trasformare
	@param 			ptrpagine per ritornare le pagine
	@param 			ptrcolonne per ritornare le colonne
	@param 			ptrfrazionecolonna per ritornare la frazione di colonna
	@param 			ptrmoduli per ritornare i moduli
	@result			nessuno
*/
void XTAPI  CalcolaPagineColonneEModuli(uint32 spazio, uint32 *ptrpagine,uint32 *ptrcolonne, 
							  uint32 *ptrfrazionecolonna,  uint32 *ptrmoduli) throw();

/*! 
	@function			ScegliColonna
	@abstract		gestisce info colonna.
	@discussion		Sceglie la colonna migliore per il defunto passato

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			ptrspazicolonna puntatore alle info delle colonne da analizzare
	@param 			numerocolonne numero delle colonne
	@param 			indicespazicolonne per ritornare l'indice della colonna scelta
	@result			nessuno
*/
void XTAPI ScegliColonna(spaziocolonna *ptrspazicolonna, int16 numerocolonne, int16 *indicespazicolonne) throw();

/*! 
	@function			CalcolaPunti
	@abstract		gestisce info colonna.
	@discussion		Trasforma i punti fixed in punti e frazione di punti

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			spazio valore da convertire
	@param 			intero  parte intera
	@param 			frazione parte frazionaria
	@result			ritorna il numero delle pagine se c'e' un documento aperto altrimenti 0
*/
void XTAPI CalcolaPunti(Fixed spazio, int32 *intero, int32 *frazione) throw();

/*! 
	@function			ImpostaNumeroPagine
	@abstract		gestisce info colonna.
	@discussion		Imposta il numero delle pagine del documento su cui si Š fatto il calcolo dello
					spazio disponibile

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			numeropagine numero pagine da impostare
	@result			nessuno
*/
void XTAPI ImpostaNumeroPagine(int16 numeropagine) throw();

/*! 
	@function			PrendiNumeroPagine
	@abstract		gestisce info colonna.
	@discussion		Restituisce il numero delle pagine del documento su cui si Š fatto il calcolo dello
					spazio disponibile

					13 Aprile 2005 - Fabrizio Taretto.

	@param 			nessuno
	@result			ritorna il numero delle pagine se c'e' un documento aperto altrimenti 0
*/
int16 XTAPI PrendiNumeroPagine() throw();

#endif // InfoNecrologi_h