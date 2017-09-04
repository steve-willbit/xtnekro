/*!
	@header			Immagini.h
	@abstract		impaginazione annunci
	@discussion 		Contiene la descrizione delle procedure usate per aprire, salvare e
					chiudere i documenti Xpress
	
					19 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.3  2005/05/09 09:47:53  taretto
					gestione importazione tutte immagini ok
					
					Revision 1.2  2005/05/04 14:31:19  taretto
					ingombro - immagine campione
					
					Revision 1.1  2005/04/20 07:14:19  taretto
					importazione files
					
*/
#ifndef Immagini_h
#define Immagini_h


// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			ImpostaImmagineCampione
	@abstract 		gestione immagini.
	@discussion		Cerca su documento corrente l'immagine campione e salva le sue caratteristiche
					nella struttura globale

					19 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI ImpostaImmagineCampione() throw();

/*!
	@function			PrendiImmagineCampione
	@abstract 		gestione immagini.
	@discussion		Prende il puntatore alle caratteristiche dell'immagine campione

					19 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
xtboxptr XTAPI PrendiImmagineCampione() throw();

/*!
	@function			ImportaImmagine
	@abstract 		gestione immagini.
	@discussion		Importa immagine all'interno del documento

					19 Aprile 2005 - Fabrizio.

	@param 			nomeimmagine puntatore alla stringa che contiene il nome dell'immagine
					da importare. L'immagine viene cercata nella cartella immagini impostata
					nelle preferenze.
	@result  			nessuno
*/
void XTAPI ImportaImmagine(uchar *nomeimmagine) throw();

/*!
	@function			PresenzaImmagine
	@abstract 		gestione immagini.
	@discussion		Verifica la presenza dell'immagine e crea il FSSpec dell'immagine da importare
					nella variabile globale gFSSpecImmagine

					19 Aprile 2005 - Fabrizio.

	@param 			nomeimmagine puntatore alla stringa che contiene il nome dell'immagine
					da importare. L'immagine viene cercata nella cartella immagini impostata
					nelle preferenze. Non viene fatta la gestione degli errori
	@result  			ritorna l'eventuale errore di sistema
*/
OSErr XTAPI PresenzaImmagine(uchar *nomeimmagine) throw();

/*!
	@function			PresenzaImmagine
	@abstract 		gestione immagini.
	@discussion		Verifica la presenza dell'immagine e crea il FSSpec dell'immagine da importare
					nella variabile globale gFSSpecImmagine gestendo sulla paletta l'eventuale errore

					19 Aprile 2005 - Fabrizio.

	@param 			nomeimmagine puntatore alla stringa che contiene il nome dell'immagine
					a importare. L'immagine viene cercata nella cartella immagini impostata
					nelle preferenze. La gestione degli errori viene fatta all'interno di questa
					procedura
	@result  			ritorna l'eventuale errore di sistema
*/
OSErr XTAPI CreaInfoImmagine(uchar *nomeimmagine) throw();

/*!
	@function			CreaImmagineCampione
	@abstract 		gestione immagini.
	@discussion		Crea l'immagine campione sulla prima pagina del documento corrente. Se questa
					immagine esiste gia' la seleziona

					4 Maggio 2005 - Fabrizio.

	@param 			nessuno
	@result  			ritorna l'eventuale errore dell'xtension
*/
errorixtension XTAPI CreaImmagineCampione() throw();

/*!
	@function			ControllaImmagini
	@abstract 		gestione immagini.
	@discussion		Controlla la presenza delle immagini nella caretella delle immagini,
					in base al file che ne contiene l'elenco

					9 Maggio 2005 - Fabrizio.

	@param 			nessuno
	@result  			ritorna l'eventuale errore dell'xtension
*/
errorixtension XTAPI ControllaImmagini() throw();

/*!
	@function			ImpaginaImmagini
	@abstract 		gestione immagini.
	@discussion		Cerca di importare le immagini associate ai box di immagini presenti sul 
					documento correntemente aperto

					9 Maggio 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI ImpaginaImmagini() throw();

/*!
	@function			ImpostaImmagineCampionePax
	@abstract 		gestione immagini.
	@discussion		Cerca su documento corrente l' immagine campione del box della Pax e salva
					il suo ID

					11 Maggio 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
errorixtension XTAPI ImpostaImmagineCampionePax() throw();

/*!
	@function			PrendiImmagineCampionePax
	@abstract 		gestione immagini.
	@discussion		Ritorna l'ID del box campione del simbolo di Pax

					11 Maggio 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
boxid XTAPI PrendiImmagineCampionePax() throw();


#endif //  Immagini_h