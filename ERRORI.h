/*!
	@header			Errori.h
	@abstract		errori dell'XT
	@discussion 		File che contiene le definizioni per la gestione degli errori.
	
					5 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.1  2005/04/20 07:14:19  taretto
					importazione files
					
*/
#ifndef Errori_h
#define Errori_h

#ifndef XTNecro_h
// #include "XTNecro.h"
#endif // XTNecro_h

// ENUMS
// ENUMS
// ENUMS

/*!
	@enum 			errorixtension
	@abstract		messaggi dell'XT.
	@discussion		identificatori degli errori dell'Xtension usati sia per caricare le stringhe 
					dal file delle risorse e per visualizzare la dialog ad essi associata
*/
typedef enum {
	kErroreGenerico = 1,
	kFileRisorseRovinato = 2,
	kErroreMemoria = 3,
	kErroreLetturaFile = 4,
	kErroreGestioneProcessi = 5,
	kErroreDiSistema = 6,
	kErrorePresenzaCartellaEditata = 7,
	kErrorePresenzaCartellaPreferenze = 8,
	kErroreNomeImmagineLungo = 9,
	kErroreEstensione = 10,
	kErroreVecchiePreferenze = 11,
	kErroreInserimentoPagine = 12,
	kImpossibileCompensare = 13,
	kErroreAssociaMastro = 14,
	kErroreBloccante = 15,
	
	kErroreNonBloccante = kErroreBloccante + 1,
	kErroreControlloPreferenze = kErroreBloccante + 2,
	kErroreComandi = kErroreBloccante + 3,
	kErroreArgomenti = kErroreBloccante + 4,
	kErroreTipoImpaginazione = kErroreBloccante + 5,
	kErroreAperturaDocumento = kErroreBloccante + 6,
	kErroreSalvaDocumentoCorrente = kErroreBloccante + 7,
	kErrorePresenzaCartella = kErroreBloccante + 8,
	kErroreChiusuraTuttiDocumenti = kErroreBloccante + 9,
	kErroreRiimpaginazioneSuDocumentoSbagliato = kErroreBloccante + 10,
	kErroreMancaImmagine = kErroreBloccante + 11,
	kErroreDocumentoPagineAffiancate = kErroreBloccante + 12,
	kErroreTroppaDifferenza = kErroreBloccante + 13,
	kErroreStileNonDefinito = kErroreBloccante + 14,
	kErroreEsecuzioneAppWare = kErroreBloccante + 15,
	kErroreCancellaFile = kErroreBloccante + 16,
	kErroreBoxConSbordo = kErroreBloccante + 17,
	kErroreDefinitivoBoxConSbordo = kErroreBloccante + 18,
	kErrorePresenzaFile = kErroreBloccante + 19,
	kErroreSulTipoFile = kErroreBloccante + 20,
	kErroreSulTipoApplicazione = kErroreBloccante + 21,
	kErroreMancaDocumentoDaImpaginare = kErroreBloccante + 22,
	kErroreNecrologiNonOmogenei = kErroreBloccante + 23,
	kNessunFileElencoImmagini = kErroreBloccante + 24,
	kFileElencoImmaginiNonUsabile = kErroreBloccante + 25,
	kErroreSceltaColonnaOttimale = kErroreBloccante + 26,
	kErroreNumeroNecrologiSullaColonna = kErroreBloccante + 27,
	kImpossibileDecrementareColonne = kErroreBloccante + 28,
	kImpossibileIncrementareColonne = kErroreBloccante + 29,
	kAllargamentoForzato = kErroreBloccante + 30,
	kRestringimentoForzato = kErroreBloccante + 31,
	kErrorePosizioneComandi = kErroreBloccante + 32,
	kErroreDocumentoGiaUsato = kErroreBloccante + 33,
	kErrorePosizionamentoSuDocumentoSbagliato = kErroreBloccante + 34,
	kErroreCreazioneDati = kErroreBloccante + 35,
	kErroreRigoreAnniversari = kErroreBloccante + 36,
	kErroreRigoreRingraziamenti = kErroreBloccante + 37,
	kErroreRigoreAnnunci = kErroreBloccante + 38,
	kErroreTroppiRigoriAnniversari = kErroreBloccante + 39,
	kErroreTroppiRigoriRingraziamenti = kErroreBloccante + 40,
	kErroreTroppiRigoriAnnunci = kErroreBloccante + 41,
	kErroreTroppiAnnunciLunghi = kErroreBloccante + 42,
	kErroreLunghezzaImmagine = kErroreBloccante + 43,
	kNessunSlug = kErroreBloccante + 44,
	kErroreColonnaStretta = kErroreBloccante + 45,
	kErroreColonnaLarga = kErroreBloccante + 46,
	kErroreModuliNecessari = kErroreBloccante + 47,
	kErrorePuntiNecessari = kErroreBloccante + 48,
	kErroreNecrologiFuoriPagina = kErroreBloccante + 49,
	kErroreRicercaIngombro = kErroreBloccante + 50,
	kErroreDeviDarePiuSpazio = kErroreBloccante + 51,
	kErroreDeviDareMenoSpazio = kErroreBloccante + 52,
	kErroreSlugSimboloPaxPresente = kErroreBloccante + 53,
	kUltimoErrore = kErroreBloccante + 54,
	kErroreDelCodice = 9998,
	kNessunErrore = 9999
} errorixtension;

// DEFINES
// DEFINES
// DEFINES

#define kPrimoErrore 1
#define kNumeroErrori kUltimoErrore - 1
#define kNumeroErroriBloccanti kErroreBloccante - 1

// EXTERN GLOBALS
// EXTERN GLOBALS
// EXTERN GLOBALS

// per prendere gli errori dell'xtension in tutta l'xtension
extern errorixtension gErroreXtension;

// per leggere gli errori del sistema operativo in tutta l'xtension
extern OSErr gErrore;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			InizializzaErrori
	@abstract 		gestione errori.
	@discussion		Allocca, usando le procedure interne di Xpress, gli errori dell'Xtension.

					5 Aprile 2005 - Fabrizio.

	@param 			nessuno
	@result  			nessuno
*/
void XTAPI InizializzaErrori() throw();

/*!
	@function			ConcatenaAllaStringaErrore
	@abstract 		gestione errori.
	@discussion		Concatena alla stringa gStringaErrore la stringa passata.

					5 Aprile 2005 - Fabrizio.

	@param 			stringaerrore messaggio da aggiungere alla variabile gStringaErrore
	@result  			nessuno
*/
void XTAPI ConcatenaAllaStringaErrore(uchar *stringaerrore) throw();

/*!
	@function			ConcatenaLongAllaStringaErrore
	@abstract 		gestione errori.
	@discussion		Concatena alla stringa gStringaErrore il long passato.

					5 Aprile 2005 - Fabrizio.

	@param 			svalore valore da concatenare alla stringa di errore
	@result  			nessuno
*/
void XTAPI ConcatenaLongAllaStringaErrore(int32 valore) throw();

/*!
	@function			VisualizzaErrore
	@abstract 		gestione errori.
	@discussion		Stampa l'errore passato sulla paletta dell'Xtension.

					5 Aprile 2005 - Fabrizio.

	@param 			errore codice dell'errore da stampare
	@result  			nessuno
*/
void XTAPI VisualizzaErrore(errorixtension errore) throw();

/*!
	@function			StampaErrore
	@abstract 		gestione errori.
	@discussion		Stampa sul file di rapporto il messaggio di errore corrispondente al
					codice passato.

					5 Aprile 2005 - Fabrizio.

	@param 			errore codice dell'errore da visualizzare
	@result  			nessuno
*/
void XTAPI StampaErrore(errorixtension errore) throw();

/*!
	@function			DaiErrore
	@abstract 		gestione errori.
	@discussion		Stampa l'errore passato sia sulla palette che sul file di report

					5 Aprile 2005 - Fabrizio.

	@param 			errore codice dell'errore da stampare
	@result  			nessuno
*/
void XTAPI DaiErrore(errorixtension errore) throw();

/*!
	@function			MostraErrore
	@abstract 		gestione errori.
	@discussion		Procedura che viene richiamata sia per gli errori bloccanti che per quelli
					non bloccanti.
					ERRORI BLOCCANTI:
					In base all'errore passato nel parametro visualizza la dialog opportuna.
					e poi esce da Xpress.
					ERRORI NON BLOCCANTI:
					Fa un beep di errore e stampa un messaggio sul file di rapporto e sulla
					palette dell'xtension

					5 Aprile 2005 - Fabrizio.

	@param 			errore codice dell'errore da visualizzare
	@result  			nessuno
*/
void XTAPI MostraErrore(errorixtension errore) throw();

#endif // Errori_h 
