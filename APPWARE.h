/*!
	@header			AppWare.h
	@abstract		AppWare
	@discussion 		Contiene la descrizione delle procedure usate per la generazione del file
					per AppWare.
	
					12 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.
					
					$Log: not supported by cvs2svn $

*/	
#ifndef AppWare_h
#define AppWare_h

#ifndef LeggiFile_h
#include "ReadFile.h"
#endif // LeggiFile_h

#ifndef XTNecro_h
#include "XTNecro.h"
#endif // XTNecro_h

// ENUMS
// ENUMS
// ENUMS

typedef enum 
{
	kNessunAppWare = '*',
	kAnnuncioAppWare = 'n',
	kAnniversarioAppWare = 'a',
	kRingraziamentoAppWare = 'r',
	kSpazioBianco = 's'
} tipoappware;

// STRUCT
// STRUCT
// STRUCT
typedef struct 
{
	int16 contatore;
	uchar codiceDefunto[kLunghezzaCodice];
	uchar codiceNecrologio[kLunghezzaCodice];
	tipoappware tipoAppWare;
	tiporigore tipoRigore;
	uchar tipoGruppo;
	uchar posizioneNelGruppo;
	Fixed altezzaBox;
	int16 posizionePagina;
	int16 posizioneColonna;
	Fixed spazioTraParagrafi;
	int32 numeroParagrafiDiversi;
	int32 numeroParagrafi;
	int32 numeroLinee;
	Fixed restringimento;
	Fixed allargamento;
	Boolean collegato;
	uint32 codiceBox;
} appware, **handleappware;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			InizializzaContatoreNecrologi
	@abstract 		AppWare
	@discussion		Inizializza il contatore dei necrologi
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			nessuno
	@result			nessuno
*/
void XTAPI  InizializzaContatoreNecrologi() throw();

/*!
	@function			InizializzaContatoreNecrologi
	@abstract 		AppWare
	@discussion		Inizializza il puntatore alla struttura AppWare passato. Devo chiamare questa
					procedura ogni volta che inizio ad impaginare un nuovo box
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura AppWare da inizializzare
	@result			nessuno
*/
void XTAPI InizializzaAppWare(appware *ptrappware) throw();

/*!
	@function			InizializzaContatoreNecrologi
	@abstract 		AppWare
	@discussion		Incrementa il contatore dei necrologi e assegna il nuovo valore al campo
					della struttura passata
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura AppWare da inizializzare
	@result			nessuno
*/
void XTAPI ImpostaContatoreNecrologi(appware *ptrappware) throw();

/*!
	@function			ImpostaCodiceDefunto
	@abstract 		AppWare
	@discussion		Per impostare il codice defunto 
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura AppWare da inizializzare
	@param			odicedefunto contiene il codice del defunto
	@result			nessuno
*/
void XTAPI ImpostaCodiceDefunto(appware *ptrappware, uchar *codicedefunto) throw();

/*!
	@function			ImpostaCodiceNecrologio
	@abstract 		AppWare
	@discussion		Per impostare il codice necrologio 
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura AppWare da inizializzare
	@param			codicenecrologio contiene il codice del necrologio
	@result			nessuno
*/
void XTAPI ImpostaCodiceNecrologio(appware *ptrappware, uchar *codicenecrologio) throw();

/*!
	@function			ImpostaTipoAppWare
	@abstract 		AppWare
	@discussion		Per impostare il codice necrologio 
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura AppWare da inizializzare
	@param			appware - contiene il tipo del campo da inserire nella struttura
	@result			nessuno
*/
void XTAPI ImpostaTipoAppWare(appware *ptrappware, tipoappware appware) throw();

/*!
	@function			ImpostaTipoRigore
	@abstract 		AppWare
	@discussion		Per impostare il tipo di rigore del necrologio 
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura AppWare da inizializzare
	@param			rigore contiene il tipo di rigore da inserire nella struttura
	@result			nessuno
*/
void XTAPI ImpostaTipoRigore(appware *ptrappware, tiporigore rigore) throw();

/*!
	@function			ImpostaTipoGruppo
	@abstract 		AppWare
	@discussion		Per impostare il tipo del gruppo
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura AppWare da inizializzare
	@param			tipogruppo carattere del tipo del gruppo
	@result			nessuno
*/
void XTAPI ImpostaTipoGruppo(appware *ptrappware, uchar tipogruppo) throw();

/*!
	@function			ImpostaPosizioneNelGruppo
	@abstract 		AppWare
	@discussion		Per impostare il tipo del gruppo
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura AppWare da inizializzare
	@param			posizionenelgruppo carattere che contiene la posizione all'interno del gruppo
	@result			nessuno
*/
void XTAPI ImpostaPosizioneNelGruppo(appware *ptrappware, uchar posizionenelgruppo) throw();

/*!
	@function			ImpostaAltezzaBox
	@abstract 		AppWare
	@discussion		Per impostare l'altezza del box
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura AppWare da inizializzare
	@param			altezzabox contiene l'altezza in fixed del box appena impaginato
	@result			nessuno
*/
void XTAPI ImpostaAltezzaBox(appware *ptrappware, Fixed altezzabox) throw();

/*!
	@function			ImpostaPosizionePagina
	@abstract 		AppWare
	@discussion		Per impostare la posizione della pagina 
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura AppWare da inizializzare
	@param			posizionepagina contiene la posizione della pagina
	@result			nessuno
*/
void XTAPI ImpostaPosizionePagina(appware *ptrappware, int16 posizionepagina) throw();

/*!
	@function			ImpostaPosizioneColonna
	@abstract 		AppWare
	@discussion		Per impostare la posizione della colonna 
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura AppWare da inizializzare
	@param			posizionecolonna contiene la posizione della colonna
	@result			nessuno
*/
void XTAPI ImpostaPosizioneColonna(appware *ptrappware, int16 posizionecolonna) throw();

/*!
	@function			ImpostaRestringimento
	@abstract 		AppWare
	@discussion		Per impostare il restringimento 
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura AppWare da inizializzare
	@param			restringimento contiene il valore di restringimento del box
	@result			nessuno
*/
void XTAPI ImpostaRestringimento(appware *ptrappware, Fixed restringimento) throw();

/*!
	@function			ImpostaSpazioTraParagrafi
	@abstract 		AppWare
	@discussion		Per impostare lo spazio tra i paragrafi 
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura AppWare da inizializzare
	@param			rspaziotraparagrafi contiene lo spazio tra i paragrafi
	@result			nessuno
*/
void XTAPI  ImpostaSpazioTraParagrafi(appware *ptrappware, Fixed spaziotraparagrafi) throw();

/*!
	@function			ImpostaNumeroParagrafiDiversi
	@abstract 		AppWare
	@discussion		Per impostare il numero di paragrafi con spazio sopra e/o sotto diverso da zero
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura da impostare
	@param			numeroparagrafi contiene il numero di paragrafi
	@result			nessuno
*/
void XTAPI ImpostaNumeroParagrafiDiversi(appware *ptrappware, int32 numeroparagrafi) throw();

/*!
	@function			ImpostaNumeroParagrafi
	@abstract 		AppWare
	@discussion		Per impostare il numero di paragrafi 
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura da impostare
	@param			numeroparagrafi contiene il numero di paragrafi
	@result			nessuno
*/
void XTAPI ImpostaNumeroParagrafi(appware *ptrappware, int32 numeroparagrafi) throw();

/*!
	@function			ImpostaNumeroLinee
	@abstract 		AppWare
	@discussion		Per impostare il numero di linee 
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura da impostare
	@param			numerolinee contiene il numero di linee
	@result			nessuno
*/
void XTAPI ImpostaNumeroLinee(appware *ptrappware, int32 numerolinee) throw();

/*!
	@function			ImpostaAllargamento
	@abstract 		AppWare
	@discussion		Per impostare l'allargamento 
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura da impostare
	@param			allargamento contiene il valore di allargamento
	@result			nessuno
*/
void XTAPI ImpostaAllargamento(appware *ptrappware, Fixed allargamento) throw();

/*!
	@function			ImpostaCollegato
	@abstract 		AppWare
	@discussion		Per impostare se un blocchetto collegato
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura da impostare
	@param			collegato contiene il booleano per sapere se e' un box collegato
	@result			nessuno
*/
void XTAPI ImpostaCollegato(appware *ptrappware, Boolean collegato) throw();

/*!
	@function			ImpostaCodiceBox
	@abstract 		AppWare
	@discussion		Per impostare il codice interno del box dato dal sistema
					
					18 Aprile 2005 - Taretto Fabrizio.

	@param			ptrappware puntatore alla struttura da impostare
	@param			codicebox codice del box
	@result			nessuno
*/
void XTAPI ImpostaCodiceBox(appware *ptrappware, uint32 codicebox) throw();

#pragma mark -
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1


errorixtension StampaFilePerAppWare(void);
errorixtension InserisciNecrologiNelleColonne(char *ptrdocumento);

#endif // // NECRO v1
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#endif /* AppWare_h */