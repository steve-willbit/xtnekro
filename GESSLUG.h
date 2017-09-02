/*!
	@header			GestioneSlug.h
	@abstract		posizionamento annunci
	@discussion 		Contiene la descrizione delle procedure usate per la gestione degli slug
					sui box da impaginare
	
					13 Aprile 2005			Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.1  2005/04/20 07:14:19  taretto
					importazione files
					
*/
#ifndef GestioneSlug_h
#define GestioneSlug_h

#ifndef AppWare_h
#include "AppWare.h"
#endif // AppWare_h

// CONSTS
// CONSTS
// CONSTS

#define kTipoSlug 					0x40485554 		/* 'A' */
#define kSlugDiCodice 				0x41485554 		/* kTipoSlug + 1 */
#define kSlugDiImmagine 			0x42485554  		/* kTipoSlug + 2 */
#define kSlugDiAppWare 			0x43485554		/* kTipoSlug + 3 */
#define kSlugDiDocumento 			0x44485554 		/* kTipoSlug + 4 */
#define kSlugDiImmagineCampione 	0x45485554 		/* kTipoSlug + 5  */
#define kSlugDiIngombro 			0x46485554		/* kTipoSlug + 6 */
#define kSlugDiPax				0x47485554 		/* kTipoSlug + 7 */				

// TYPEDEFS
// TYPEDEFS
// TYPEDEFS

typedef struct 
{
	int16 marcatorePax;
} marcatorepax, **handlemarcatorepax;

typedef struct 
{
	uint32 codiceBox;
} codicebox, **handlecodicebox;

typedef struct 
{
	uchar nomeImmagine[32];
} immagine, **handleimmagine;

typedef struct 
{
	uint32 codiceDocumento;
} codicedoc, **handlecodicedoc;

typedef struct 
{
	bool8 immagineCampione;
} immaginecampione, **handleimmaginecampione;

typedef struct 
{
	int16 colonna;
} codiceingombro, **handlecodiceingombro;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*! 
	@function			PrendiCodiceBox
	@abstract		gestione slug
	@discussion		Ritorna l'ultimo codice interno usato. Da usare sempre dopo che Š stato inserito
					lo slug

					14 Aprile 2005 - Fabrizio.

	@param			nessuno
	@result			nessuno
*/
uint32 XTAPI PrendiCodiceBox() throw();

/*! 
	@function			SlugDiCodice
	@abstract		gestione slug
	@discussion		imposta uno slug sul box di testo corrente per rieseguire l'impaginazione

					14 Aprile 2005 - Fabrizio.

	@param			nessuno
	@result			ritorna l'eventuale errore dell'xtension
*/
errorixtension XTAPI SlugDiCodice() throw();

/*! 
	@function			TrovaSlugDiCodice
	@abstract		gestione slug
	@discussion		Cerca il box di testo con un dato slug e lo imposta come box corrente

					14 Aprile 2005 - Fabrizio.

	@param			codicebox  codice dello slug da trovare
	@result			ritorna l'eventuale errore dell'xtension
*/
errorixtension XTAPI TrovaSlugDiCodice(uint32 codicebox) throw();

/*! 
	@function			IsBoxCorrenteMio
	@abstract		gestione slug
	@discussion		Per testare se il box corrente e' un mio box oppure un box di ingombro
					inserito dall'utente per limitare la pagina dove impaginare

					14 Aprile 2005 - Fabrizio.

	@param			nessuno
	@result			ritorna TRUE se e' un mio box
*/
bool8 XTAPI IsBoxCorrenteMio() throw();

/*! 
	@function			SlugDiImmagine
	@abstract		gestione slug
	@discussion		Imposta uno slug sul box di immagine corrente per l'importazione successiva
					dell'immagine

					14 Aprile 2005 - Fabrizio.

	@param			nomeimmagine nome completo dell'immagine da associare
	@result			ritorno l'eventuale errore dell'xtension
*/
errorixtension XTAPI  SlugDiImmagine(uchar* nomeimmagine) throw();

/*! 
	@function			PrendiSlugDiImmagine
	@abstract		gestione slug
	@discussion		Prende lo slug sul box di immagine corrente con le informazioni sull'immagine
					da importare successivamente

					14 Aprile 2005 - Fabrizio.

	@param			nomeimmagine puntatore alla stringa dove salvare il nome associato all'immagine
					corrente
	@result			ritorno l'eventuale errore dell'xtension
*/
errorixtension XTAPI PrendiSlugDiImmagine(uchar* nomeimmagine) throw();

/*! 
	@function			SlugDiAppWare
	@abstract		gestione slug
	@discussion		Imposta uno slug sul box di testo corrente con le informazioni di AppWare
					per permettere la stampa del file di AppWare

					14 Aprile 2005 - Fabrizio.

	@param			puntatore alla struttura di AppWare da associare al box corrente
	@result			ritorno l'eventuale errore dell'xtension
*/
errorixtension XTAPI  SlugDiAppWare(appware *ptrappware) throw();

/*! 
	@function			PrendiSlugDiAppWare
	@abstract		gestione slug
	@discussion		Prende lo slug sul box di testo corrente con le informazioni di AppWare
					per permettere la stampa del file di AppWare

					14 Aprile 2005 - Fabrizio.

	@param			puntatore alla struttura di AppWare per ritornare le informazioni
	@result			ritorno l'eventuale errore dell'xtension
*/
errorixtension XTAPI PrendiSlugDiAppWare(appware *ptrappware) throw();

/*! 
	@function			SlugDiDocumento
	@abstract		gestione slug
	@discussion		Imposta uno slug sul documento corrente per essere sicuro di effettuare il
					calcolo del posizionamento e il posizionamento sul documento in cui Š stata
					fatta l'importazione dei dati

					14 Aprile 2005 - Fabrizio.

	@param			codicedocumento codice da impostare al documento corrente
	@result			ritorno l'eventuale errore dell'xtension
*/
errorixtension XTAPI  SlugDiDocumento(uint32 codicedocumento) throw();

/*! 
	@function			PrendiCodiceDocumento
	@abstract		gestione slug
	@discussion		Prende il codice del documento corrente per confrontarlo con il codice
					delle informazioni sul posizionamento

					14 Aprile 2005 - Fabrizio.

	@param			nessuno
	@result			ritorna il codice del documento corrente
*/
uint32 XTAPI PrendiCodiceDocumento() throw();

/*! 
	@function			SlugDiImmagineCampione
	@abstract		gestione slug
	@discussion		Imposta uno slug sul box di immagine campione per l'importazione successiva

					14 Aprile 2005 - Fabrizio.

	@param			nessuno
	@result			ritorno l'eventuale errore dell'xtension
*/
errorixtension XTAPI SlugDiImmagineCampione() throw();

/*! 
	@function			SlugDiImmagineCampione
	@abstract		gestione slug
	@discussion		Cerca il box immagine campione e lo imposta come box corrente

					14 Aprile 2005 - Fabrizio.

	@param			nessuno
	@result			ritorno l'eventuale errore dell'xtension
*/
errorixtension XTAPI TrovaSlugDiImmagineCampione() throw();

/*! 
	@function			SlugDiIngombro
	@abstract		gestione slug
	@discussion		Imposta uno slug sul box di ingombro corrente

					14 Aprile 2005 - Fabrizio.

	@param			nessuno
	@result			ritorno l'eventuale errore dell'xtension
*/
errorixtension XTAPI SlugDiIngombro(int16 colonna) throw();

/*! 
	@function			TrovaSlugDiIngombro
	@abstract		gestione slug
	@discussion		Cerca il box di ingombro di una data colonna e lo imposta come box corrente

					14 Aprile 2005 - Fabrizio.

	@param			colonna colonna di cui trovare l'ingombro
	@result			ritorno l'eventuale errore dell'xtension
*/
errorixtension XTAPI TrovaSlugDiIngombro(int16 colonna) throw();

/*! 
	@function			SlugDiPax
	@abstract		gestione slug
	@discussion		Imposta uno slug sul box corrente per marcarlo come campione per il
					simbolo della Pax

					11 Maggio 2005 - Fabrizio.

	@param			nessuno
	@result			ritorno l'eventuale errore dell'xtension
*/
errorixtension XTAPI SlugDiPax() throw();

/*! 
	@function			TrovaSlugDiPax
	@abstract		gestione slug
	@discussion		Cerca il box con il marcatore di pax e lo rimuove

					11 Maggio 2005 - Fabrizio.

	@param			boxtrovato - in uscita, id del box trovato
	@result			ritorno l'eventuale errore dell'xtension
*/
errorixtension XTAPI TrovaSlugDiPax(boxid *boxtrovato) throw();

/*! 
	@function			RimuoviSlugDiPax
	@abstract		gestione slug
	@discussion		Rimuove il marcatore dal box passato come parametro

					11 Maggio 2005 - Fabrizio.

	@param			vecchiobox - il box dal quale cancellare il simbolo
	@result			ritorno l'eventuale errore dell'xtension
*/
void XTAPI RimuoviSlugDiPax(boxid vecchiobox) throw();

#endif // GestioneSlug_h