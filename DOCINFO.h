/*!
	@header			DocInfo.h
	@abstract		informazioni documento
	@discussion 		Contiene le descrizioni per le procedure per prelevare informazioni sul
					documento corrente.
	
					11 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
*/

#ifndef DocInfo_h
#define DocInfo_h

#ifndef Errori_h
#include "Errori.h"
#endif // Errori_h

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			PrendiDocInfo
	@abstract 		gestione errori.
	@discussion		Legge le informazioni del documento correntemente aperto. Non pu• essere
					chiamata senza documento aperto

					11 Aprile 2005 - Fabrizio.

	@param 			errore codice dell'errore da visualizzare
	@result  			nessuno
*/
errorixtension XTAPI PrendiDocInfo() throw();

/*!
	@function			PrendiMargineAlto
	@abstract 		info documento.
	@discussion		Legge il margine alto del documento corrente controllando che sia comunque
					aperto un documento

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			ritorna il margine alto se c'Š un documento aperto altrimenti 0
*/
Fixed XTAPI PrendiMargineAlto() throw();

/*!
	@function			PrendiMargineBasso
	@abstract 		info documento.
	@discussion		Calcola il bottom del documento corrente

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			ritorna il bottom del documento aperto altrimenti 0
*/
Fixed XTAPI PrendiMargineBasso() throw();

/*!
	@function			PrendiMargineSinistro
	@abstract 		info documento.
	@discussion		Legge il margine sinistro del documento

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			ritorna il margine sinistro se c'e' un documento aperto altrimenti 0
*/
Fixed XTAPI PrendiMargineSinistro() throw();

/*!
	@function			PrendiMargineDestro
	@abstract 		info documento.
	@discussion		Legge il margine sinistro del documento

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			ritorna il margine destro se c'e' un documento aperto altrimenti 0
*/
Fixed XTAPI PrendiMargineDestro() throw();

/*!
	@function			PrendiLarghezzaColonna
	@abstract 		info documento.
	@discussion		Calcola la larghezza colonna del documento

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			ritorna la larghezza della colonna se c'e' un documento aperto altrimenti 0
*/
Fixed XTAPI PrendiLarghezzaColonna() throw();

/*!
	@function			PrendiAltezzaColonna
	@abstract 		info documento.
	@discussion		Calcola l'altezza colonna del documento

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			ritorna l'altezza della colonna se c'e' un documento aperto altrimenti 0
*/
Fixed XTAPI PrendiAltezzaColonna() throw();

/*!
	@function			PrendiNumeroColonne
	@abstract 		info documento.
	@discussion		Restituisce il numero delle colonne del documento corrente

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			ritorna il numero delle colonne se c'e' un documento aperto altrimenti 0
*/
int16 XTAPI PrendiNumeroColonne() throw();

/*!
	@function			PrendiDistanzaColonne
	@abstract 		info documento.
	@discussion		Restituisce la distanza tra le colonne

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			ritorna la distanza tra le colonne se c'e' un documento aperto altrimenti 0
*/
Fixed XTAPI PrendiDistanzaColonne() throw();

/*!
	@function			IsPagineAffiancate
	@abstract 		info documento.
	@discussion		Per sapere se il documento corrente e' a pagine affiancate

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			ritorna TRUE se il documento e' a pagine affiacate
*/
bool8 XTAPI IsPagineAffiancate() throw();

/*!
	@function			ImpostaScalaDocumento
	@abstract 		info documento.
	@discussion		Imposta la scala di visualizzazione del documento

					12 Aprile 2005 - Fabrizio.

	@param 			percetuale puntatore alla la percentuale da usare
	 				in output ritorna la percetuale di visualizzazione precedente
	@return			nessuno 				
*/
void XTAPI ImpostaScalaDocumento(Fixed *percetuale) throw();

/*!
	@function			ImpostaFinestraDocumento
	@abstract 		info documento.
	@discussion		Imposta la dimensione della finestra del documento.
					ATTENZIONE: in restringimento funziona bene, in allargamento fa un po'
					di casini nel visualizzare nuovamente le scroll bar.
					Sono comunque solo casini nella visualizzazione.

					12 Aprile 2005 - Fabrizio.

	@param 			altezza puntatore alla nuova altezza, in output ritorna la vecchia altezza 
	@param			 larghezza puntatore alla nuova larghezza, in output ritorna la vecchia larghezza		
	@return			nessuno 				
*/
void XTAPI ImpostaFinestraDocumento(int16 *larghezza, int16 *altezza) throw();

#endif // #ifndef DocInfo_h