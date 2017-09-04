/* ------------------------------------------------------------------------ *

	InfoNecrologi.h

	6 Febbraio 1996						Stefano

	Descrizione:
	Contiene le descrizioni delle procedure per la costruzione delle informazioni
	sui necrologi

* ------------------------------------------------------------------------ */

#ifndef InfoNecrologi_h
#define InfoNecrologi_h

#ifndef LeggiFile_h
#include "ReadFile.h"
#endif // LeggiFile_h

#ifndef XTNecro_h
#include "XTNecro.h"
#endif // XTNecro_h

#ifndef AppWare_h
#include "AppWare.h"
#endif // AppWare_h

// STRUCT
// STRUCT
// STRUCT

typedef struct nec 
{
	unsigned long codiceBox;
	uchar tipoGruppo;
	Fixed altezzaBox;
	Fixed spazioTraParagrafi;
	long numeroParagrafiDiversi;
	long numeroParagrafi;
	long numeroLinee;
	Boolean collegato;
	Boolean cambioColonna;
	struct nec *successivo;
} necrologio, *NecrologioPtr, **NecrologioHandle;

typedef struct def 
{
	uchar codiceDefunto[kLunghezzaCodice];
	tiporigore tipoRigore;
	DoubleFixed altezzaTotaleDoubleFixed;
	int16 numeroNecrologi;
	int16 numeroColonne;
	necrologio *elencoNecrologiA;
	necrologio *ultimoNecrologioA;
	necrologio *elencoNecrologiB;
	necrologio *ultimoNecrologioB;
	necrologio *elencoNecrologiC;
	necrologio *ultimoNecrologioC;
	struct def *successivo;
} defunto, *defuntoptr, **defuntohndl;

// EXTERN GLOBALS
// EXTERN GLOBALS
// EXTERN GLOBALS

// contatore degli annunci da posizionare
extern int16 gNumeroAnnunci;
// contatore degli anniversari da posizionare
extern int16 gNumeroAnniversari;
// contatore dei ringraziamenti da posizionare
extern int16 gNumeroRingraziamenti;
// array che contine tutti gli annunci da posizionare
extern defuntoptr gPtrAnnunci;
// array che contine tutti gli anniversari da posizionare
extern defuntoptr gPtrAnniversari;
// array che contine tutti i ringraziamenti da posizionare
extern defuntoptr gPtrRingraziamenti;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			LiberaListaNecrologi
	@abstract 		gestione annunci necrologi.
	@discussion		Libera la memoria della lista dei necrologi

					12 Aprile 2005 - Fabrizio.

	@param 			ptrnecrologio puntatore alla lista da liberare
	@result  			nessuno
*/
void XTAPI LiberaListaNecrologi(necrologio **ptrnecrologio) throw();

/*!
	@function			LiberaInfoDefuntiNecrologi
	@abstract 		gestione annunci necrologi.
	@discussion		Socrre l'array dei defunti per liberare la memoria dei necrologi

					12 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto puntatore alla lista che contiene i defunti da liberare
	@result  			nessuno
*/
void XTAPI LiberaInfoDefuntiNecrologi(defunto *ptrdefunto, int16 numerodefunti) throw();

/*!
	@function			LiberaListaDefunti
	@abstract 		gestione annunci necrologi.
	@discussion		Socrre l'array dei defunti per liberare la memoria dei necrologi

					12 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto puntatore alla lista che contiene i defunti da liberare
	@result  			nessuno
*/
void XTAPI LiberaListaDefunti(defunto **ptrdefunto) throw();

/*!
	@function			LiberaInfoDefunti
	@abstract 		gestione annunci necrologi.
	@discussion		Libera la memoria usata per memorizzare le informazioni sui necrologi
					ma non fa la dispose dei necrologi.

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI LiberaInfoDefunti() throw();

/*!
	@function			LiberaInfoDefunti
	@abstract 		gestione annunci necrologi.
	@discussion		Costruisce le singole dei necrologi da posizionare

					12 Aprile 2005 - Fabrizio.

	@param 			
	@result  			ritorna l'evetuale errore dell'xtension
*/
errorixtension XTAPI CostruisciNecrologi(defunto **ptrdefunto, int16 numerodefunti) throw();

/*!
	@function			CreaInfoNecrologi
	@abstract 		gestione annunci necrologi.
	@discussion		Crea le info sui necrologi da posizionare

					12 Aprile 2005 - Fabrizio.

	@param 			ptrdocumento puntatore al file da impaginare per contare i necrologi e,
					in base al conteggioo, dimensionare le strutture necessarie
	@result  			ritorna l'evetuale errore dell'xtension
*/
errorixtension XTAPI CreaInfoNecrologi(uchar*ptrdocumento) throw();

/*!
	@function			ImpostaNuovoDefunto
	@abstract 		gestione annunci necrologi.
	@discussion		Imposta la varibile usata per sapere che sto trattando le info di un
					nuovo defunto

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI ImpostaNuovoDefunto() throw();

/*!
	@function			InserisciInfoNecrologio
	@abstract 		gestione annunci necrologi.
	@discussion		Inserisce nelle strutture dati gia' create le info del necrologio

					12 Aprile 2005 - Fabrizio.

	@param 			ptrappware puntatore alle info da inserire
	@result  			nessuno
*/
void XTAPI InserisciInfoNecrologio(appware *ptrappware) throw();

/*!
	@function			OrdinaInfoNecrologi
	@abstract 		gestione annunci necrologi.
	@discussion		Ordina le strutture sui necrologi. Gli annunci vengono ordinati sempre in modo
					decrescente. L'ordinamento dei ringraziamenti e degli anniversari dipende dal fatto
					se questi sono usati come riempitivi alla fine di tutte le colonne o se devono essere
					messi in fondo allo spazio dedicato alle necrologie.
					Nel caso siano usati come riempitivi si ordina in modo decrescente.
					Se vengono posizionati alla fine dello spazio dedicato alle necrologie si ordina in modo
					crescente. In questo modo i box do testo vengono iseriti partendo dal fondo

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI OrdinaInfoNecrologi() throw();

/*!
	@function			CreaListeInfoNecrologi
	@abstract 		gestione annunci necrologi.
	@discussion		Trasforma gli array creati in precedenza in liste da usare all'interno dell'
					algoritmo di posizionamento.

					12 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI CreaListeInfoNecrologi() throw();

/*!
	@function			EstraiNecrologio
	@abstract 		gestione annunci necrologi.
	@discussion		Estrae dalla lista passata un necrologio. E' una estrazione: il necrologio scelto
					non e' piu' reperibile scorrendo i puntatori della lista

					13 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto puntatore all'indirizzo che e' la testa della lista sia in entrata che in uscita
	@result  			puntatore al necrologio da estrarre; NULL se non e' rimasto nulla
*/
necrologio* XTAPI EstraiNecrologio(defunto **ptrdefunto) throw();

/*!
	@function			EstraiSoloNecrologio
	@abstract 		gestione annunci necrologi.
	@discussion		Estrae dalla lista passata un necrologio. E' una estrazione: il necrologio scelto
					non e' piu' reperibile scorrendo i puntatori della lista. A differenza della precedente
					non prosegue sulla lista dei defunti. Se il defunto che si sta scorrendo attualmente
					non ha piu' annunci si ferma.

					13 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto puntatore all'indirizzo che e' la testa della lista sia in entrata che in uscita
	@result  			puntatore al necrologio da estrarre; NULL se non e' rimasto nulla
*/
necrologio* XTAPI EstraiSoloNecrologio(defunto *ptrdefunto) throw();

/*!
	@function			EstraiSoloNecrologioA
	@abstract 		gestione annunci necrologi.
	@discussion		Estrae dalla lista passata un necrologio. E' una estrazione: il necrologio scelto
					non e' piu' reperibile scorrendo i puntatori della lista. A differenza della precedente
					non prosegue sulla lista dei defunti. Se il defunto che si sta scorrendo attualmente
					non ha piu' annunci si ferma. Scelgo solo i necrologi di codice A

					13 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto puntatore all'indirizzo che e' la testa della lista sia in entrata che in uscita
	@result  			puntatore al necrologio da estrarre; NULL se non e' rimasto nulla
*/
necrologio* XTAPI EstraiSoloNecrologioA(defunto *ptrdefunto) throw();

/*!
	@function			EstraiSoloNecrologioBC
	@abstract 		gestione annunci necrologi.
	@discussion		Estrae dalla lista passata un necrologio. E' una estrazione: il necrologio scelto
					non e' piu' reperibile scorrendo i puntatori della lista. A differenza della precedente
					non prosegue sulla lista dei defunti. Se il defunto che si sta scorrendo attualmente
					non ha piu' annunci si ferma. Lavora solo con i necrologi codice B e di codice C

					13 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto puntatore all'indirizzo che e' la testa della lista sia in entrata che in uscita
	@result  			puntatore al necrologio da estrarre; NULL se non e' rimasto nulla
*/
necrologio* XTAPI EstraiSoloNecrologioBC(defunto *ptrdefunto) throw();

/*!
	@function			EstraiSoloNecrologioB
	@abstract 		gestione annunci necrologi.
	@discussion		Estrae dalla lista passata un necrologio. E' una estrazione: il necrologio scelto
					non e' piu' reperibile scorrendo i puntatori della lista. A differenza della precedente
					non prosegue sulla lista dei defunti. Se il defunto che si sta scorrendo attualmente
					non ha piu' annunci si ferma. Lavora solo con i necrologi codice B

					13 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto puntatore all'indirizzo che e' la testa della lista sia in entrata che in uscita
	@result  			puntatore al necrologio da estrarre; NULL se non e' rimasto nulla
*/
necrologio* XTAPI EstraiSoloNecrologioB(defunto *ptrdefunto) throw();

/*!
	@function			EstraiSoloNecrologioC
	@abstract 		gestione annunci necrologi.
	@discussion		Estrae dalla lista passata un necrologio. E' una estrazione: il necrologio scelto
					non e' piu' reperibile scorrendo i puntatori della lista. A differenza della precedente
					non prosegue sulla lista dei defunti. Se il defunto che si sta scorrendo attualmente
					non ha piu' annunci si ferma. Lavora solo con i necrologi codice B e C

					13 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto puntatore all'indirizzo che e' la testa della lista sia in entrata che in uscita
	@result  			puntatore al necrologio da estrarre; NULL se non e' rimasto nulla
*/
necrologio* XTAPI EstraiSoloNecrologioC(defunto *ptrdefunto) throw();

/*!
	@function			EstraiDefunto
	@abstract 		gestione annunci necrologi.
	@discussion		Estrae dalla lista passata un defunto. E' una estrazione: il defunto scelto
					non e' piu' reperibile scorrendo i puntatori della lista

					13 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto puntatore all'indirizzo che e' la testa della lista sia in entrata che in uscita
	@result  			puntatore al necrologio da estrarre; NULL se non e' rimasto nulla
*/
defunto* XTAPI EstraiDefunto(defunto **ptrdefunto) throw();

/*!
	@function			PrendiAltezzaDefunto
	@abstract 		gestione annunci necrologi.
	@discussion		Prende l'altezza del primo defunto della lista

					13 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto puntatore al defunto
	@result  			ritorna l'altezza se c'e' un defunto altrimenti ritorna 0
*/
DoubleFixed XTAPI PrendiAltezzaDefunto(defunto *ptrdefunto) throw();

/*!
	@function			PrendiAltezzaTotaleDefunti
	@abstract 		gestione annunci necrologi.
	@discussion		Prende l'altezza totale dell'array dei defunti passata

					13 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto puntatore all'array del defunto
	@param			numerodefunti numero dei defunti
	@result  			ritorna l'altezza totale
*/
uint32 PrendiAltezzaTotaleDefunti(defunto *ptrdefunto, int16 numerodefunti) throw();

/*!
	@function			PrendiAltezzaTotaleListaDefunti
	@abstract 		gestione annunci necrologi.
	@discussion		Prende l'altezza totale della lista dei defunti passata

					13 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto  puntatore alla lista dei defunti
	@result  			ritorna l'altezza totale
*/
uint32 XTAPI PrendiAltezzaTotaleListaDefunti(defunto *ptrdefunto) throw();

/*!
	@function			InserisciInfoLunghezzaPaginaNecrologi
	@abstract 		gestione annunci necrologi.
	@discussion		Inserisce l'informazione su quante colonne occupa il defunto

					13 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI InserisciInfoLunghezzaPaginaNecrologi() throw();

/*!
	@function			ControllaRigoreNecrologi
	@abstract 		gestione annunci necrologi.
	@discussion		Inserisce l'informazione su quante colonne occupa il defunto

					13 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			ritorna l'eventuale errore dell'xtension
*/
errorixtension XTAPI ControllaRigoreNecrologi() throw();

/*!
	@function			PrendiNumeroNecrologiDefunto
	@abstract 		gestione annunci necrologi.
	@discussion		Restituisce il numero dei necrologi del defunto

					13 Aprile 2005 - Fabrizio.

	@param 			ptrdefunto puntatore al defunto
	@result  			ritorna il numero dei necrologi se c'e' un defunto altrimenti ritorna 0
*/
int16 XTAPI  PrendiNumeroNecrologiDefunto(defunto *ptrdefunto) throw();

#endif /* InfoNecrologi_h */