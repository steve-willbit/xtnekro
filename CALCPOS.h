/*!
	@header			CalcoloPosizionamento.h
	@abstract		posizionamento annunci
	@discussion 		Contiene le descrizioni delle procedure per il calcolo del posizionamento
					dei necrologi.
	
					13 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.1  2005/04/20 07:14:18  taretto
					importazione files
					
*/
#ifndef CalcPos_h
#define CalcPos_h

#ifndef InfoNecrologi_h
#include "InfoNec.h"
#endif // InfoNecrologi_h

#ifndef SpazioColonna_h
#include "SpaceCol.h"
#endif // InfoNecrologi_h

// ENUMS
// ENUMS
// ENUMS

typedef enum 
{
	kNulla,
	kRestringere,
	kAllargare
} cosafare;


// STRUCT
// STRUCT
// STRUCT

typedef struct lisdef 
{
	defunto *ptrDefunto;
	spaziocolonna *ptrSpazioColonna;
	struct lisdef *precedente;
	struct lisdef *successivo;
} listadef;

typedef struct lisnec 
{
	necrologio *ptrNecrologio;
	spaziocolonna *ptrSpazioColonna;
	struct lisnec *precedente;
	struct lisnec *successivo;
} listanec;

typedef struct 
{
	Ptr puntatore;
	DoubleFixed altezzaDoubleFixed;
} altezza;

// EXTERN GLOBALS
// EXTERN GLOBALS
// EXTERN GLOBALS

// spazio rimanete per fare il riordino dei necrologi
extern Fixed gSpazioRimanente;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			PrendiUltimaColonna
	@abstract 		posizionamento annunci
	@discussion		Prende l'ultima colonna con spazio disponibile

					13 Aprile 2005 - Fabrizio.

	@param 			ptrpagina ritorna la pagina trovataa 
	@param			ptrcolonna ritorna la colonna trovata		
	@return			ritorna l'eventuale errore dell'xtension 				
*/
errorixtension XTAPI PrendiUltimaColonna(int16 *ptrpagina, int16 *ptrcolonna) throw();

/*!
	@function			CosaFare
	@abstract 		posizionamento annunci
	@discussion		In base alle altezze passate determinare se Š necessario allargare o
					restringere il box

					13 Aprile 2005 - Fabrizio.

	@param 			ingombro altezza del box corrente
	@param			spaziodisponibile nuova altezza del box corrente
	@param			differenza differenza sempre positiva		
	@return			ritorna che cosa bisogna fare
*/
cosafare XTAPI CosaFare(Fixed ingombro, Fixed spaziodisponibile, Fixed *differenza) throw();

/*!
	@function			PrendiUltimaColonna
	@abstract 		posizionamento annunci
	@discussion		Prende il primo necrologio della colonna

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alle info della colonna corrente
	@return			ritorna il puntatore al necrologio			
*/
necrologio* XTAPI PrendiPrimoNecrologioSpazioColonna(spaziocolonna *ptrspaziocolonna) throw();

/*!
	@function			PrendiNecrologioSuccessivoSpazioColonna
	@abstract 		posizionamento annunci
	@discussion		Prende il necrologio sucessivo della colonna

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alle info della colonna corrente
	@param			ptrnecrologio - puntatore al necrologio corrente
	@return			ritorna il puntatore al necrologio successivo		
*/
necrologio* XTAPI PrendiNecrologioSuccessivoSpazioColonna(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw();

/*!
	@function			ImpostaColonnaPrecedente
	@abstract 		posizionamento annunci
	@discussion		Imposta la colonna precedente

					28 Aprile 2005 - Fabrizio.

	@param 			ptrpagina - ritorna la pagina decrementata
	@param			ptrcolonna - ritorna la colonna decrementata
	@return			ritorna l'eventuale errore dell'xtension	
*/
errorixtension XTAPI ImpostaColonnaPrecedente(int16 *ptrpagina, int16 *ptrcolonna) throw();

/*!
	@function			ImpostaColonnaSuccessiva
	@abstract 		posizionamento annunci
	@discussion		Imposta la colonna successiva

					28 Aprile 2005 - Fabrizio.

	@param 			ptrpagina - ritorna la pagina decrementata
	@param			ptrcolonna - ritorna la colonna incrementata
	@return			ritorna l'eventuale errore dell'xtension	
*/
errorixtension XTAPI ImpostaColonnaSuccessiva(int16 *ptrpagina, int16 *ptrcolonna) throw();

/*!
	@function			CercaPrimaColonnaDaChiudere
	@abstract 		posizionamento annunci
	@discussion		Cerca la prima colonna da chiudere a partire dai valori passat

					28 Aprile 2005 - Fabrizio.

	@param 			ptrpagina - ritorna la pagina della colonna da chiudere
	@param			ptrcolonna - ritorna la colonna da chiudere
	@return			ritorna l'eventuale errore dell'xtension	
*/
errorixtension XTAPI CercaPrimaColonnaDaChiudere(int16 *ptrpagina, int16 *ptrcolonna) throw();

/*!
	@function			CercaUltimaColonnaDaChiudere
	@abstract 		posizionamento annunci
	@discussion		Cerca l'ultima colonna da chiudere a partire dai valori passati

					28 Aprile 2005 - Fabrizio.

	@param 			ptrpagina - ritorna la pagina della colonna da chiudere
	@param			ptrcolonna - ritorna la colonna da chiudere
	@return			ritorna l'eventuale errore dell'xtension	
*/
errorixtension XTAPI CercaUltimaColonnaDaChiudere(int16 *ptrpagina, int16 *ptrcolonna) throw();

/*!
	@function			InserisciNecrologioInCoda
	@abstract 		posizionamento annunci
	@discussion		Inserisce in coda , nella colonna individuata, il nercrologio passato

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alla colonna in cui si vuole inserire
	@param			ptrnecrologio - puntatore al necrologio da inserire
	@return			nessuno
*/
void XTAPI InserisciNecrologioInCoda(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw();

/*!
	@function			RimuoviNecrologio
	@abstract 		posizionamento annunci
	@discussion		Rimuove dalla colonna il necrologio passato

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alla colonna in cui si vuole rimuovere
	@param			ptrnecrologio - puntatore al necrologio da rimuovere
	@return			nessuno
*/
void XTAPI RimuoviNecrologio(spaziocolonna *ptrspaziocolonna, necrologio *ptrnecrologio) throw();

/*!
	@function			InserisciDefuntoInTestaFondo
	@abstract 		posizionamento annunci
	@discussion		Inserisce inserisce in testa nella colonna individuata il defunto

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alla colonna in cui si vuole inserire
	@param			ptrdefunto - puntatore al defunto da inserire
	@return			nessuno
*/
void XTAPI InserisciDefuntoInTestaFondo(spaziocolonna *ptrspaziocolonna, defunto *ptrdefunto) throw();

/*!
	@function			InserisciDefuntoInCodaFondo
	@abstract 		posizionamento annunci
	@discussion		Inserisce inserisce in coda nella colonna individuata il defunto

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alla colonna in cui si vuole inserire
	@param			ptrdefunto - puntatore al defunto da inserire
	@return			nessuno
*/
void XTAPI  InserisciDefuntoInCodaFondo(spaziocolonna *ptrspaziocolonna, defunto *ptrdefunto) throw();

/*!
	@function			InserisciDefuntoInOrdineFondo
	@abstract 		posizionamento annunci
	@discussion		Inserisce inserisce in ordine in fondo nella colonna individuata il defunto

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alla colonna in cui si vuole inserire
	@param			ptrdefunto - puntatore al defunto da inserire
	@return			nessuno
*/
void XTAPI InserisciDefuntoInOrdineFondo(spaziocolonna *ptrspaziocolonna, defunto *ptrdefunto) throw();

/*!
	@function			InserisciDefuntoInCoda
	@abstract 		posizionamento annunci
	@discussion		Inserisce inserisce in coda nella colonna individuata il defunto

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna  puntatore alla colonna in cui si vuole inserire
	@param			ptrdefunto puntatore al defunto da inserire
	@return			nessuno
*/
void XTAPI InserisciDefuntoInCoda(spaziocolonna *ptrspaziocolonna, defunto *ptrdefunto) throw();

/*!
	@function			InserisciDefuntoInOrdine
	@abstract 		posizionamento annunci
	@discussion		Inserisce inserisce in coda nella colonna individuata il defunto

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna  puntatore alla colonna in cui si vuole inserire
	@param			ptrdefunto puntatore al defunto da inserire
	@return			nessuno
*/
void XTAPI InserisciDefuntoInOrdine(spaziocolonna *ptrspaziocolonna, defunto *ptrdefunto) throw();

/*!
	@function			RimuoviDefunto
	@abstract 		posizionamento annunci
	@discussion		Rimuove dalla colonna il defunto passato

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna  puntatore alla colonna in cui si vuole inserire
	@param			ptrdefunto puntatore al defunto da inserire
	@return			nessuno
*/
void XTAPI RimuoviDefunto(spaziocolonna *ptrspaziocolonna, defunto *ptrdefunto) throw();

/*!
	@function			ControllaChiusuraColonna
	@abstract 		posizionamento annunci
	@discussion		Controlla, con l'aggiunta del necrologio passato, che la colonna corrente sia chiusa
					nel rispetto delle preferenze di giustificazione

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna  puntatore alla colonna in cui si vuole inserire
	@param			ptrchiusura - puntatore per ritornare la chiusura
	@return			nessuno
*/
errorixtension XTAPI ControllaChiusuraColonna(spaziocolonna *ptrspaziocolonna, tipochiusura *ptrchiusura, Fixed *ptrdifferenza) throw();

/*!
	@function			PrendiAllargamento
	@abstract 		posizionamento annunci
	@discussion		Prende lo spazio recuperabile sulla colonna

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alle info delle colonna da controllare
	@return			ritorna l'allargamento possibile
*/
Fixed XTAPI PrendiAllargamento(spaziocolonna *ptrspaziocolonna) throw();

/*!
	@function			PrendiRestringimento
	@abstract 		posizionamento annunci
	@discussion		Prende il restringimento della colonna corrente

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alle info delle colonna da controllare
	@return			ritorna l'allargamento possibile
*/
Fixed XTAPI PrendiRestringimento(spaziocolonna *ptrspaziocolonna) throw();

/*!
	@function			PrendiRestringimento
	@abstract 		posizionamento annunci
	@discussion		Calcola il posizionamento dei necrologi in base ai dati dei necrologi
					correnti

					28 Aprile 2005 - Fabrizio.

	@param 			ptrspaziocolonna - puntatore alle info delle colonna da controllare
	@return			ritorna l'eventuale errore dell'xtension
*/
errorixtension XTAPI CalcolaPosizionamento() throw();

#endif // CalcPos_h