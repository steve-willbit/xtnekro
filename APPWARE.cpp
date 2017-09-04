/* ------------------------------------------------------------------------ *

	AppWare.cpp

	15 Aprile 2005						Fabrizio

	Descrizione:
	Contiene le procedure usate per la generazione del file per AppWare

	$Log: not supported by cvs2svn $
	Revision 1.1  2005/04/20 07:14:18  taretto
	importazione files
	

* ------------------------------------------------------------------------ */

/* Required Includes ********************************************************/
#include PROJECT_HEADERS
#if WINOS
#pragma hdrstop		// force Visual C++ precompiled header
#endif

#include "Include.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "Errori.h"
#include "Pref.h"
#include "ReadFile.h"
#include "DocInfo.h"
#include "GesSlug.h"
#include "SpaceCol.h"
#include "Paletta.h"
#include "PresFile.h"
#include "CalcPos.h"

#include "AppWare.h"

// STATICS GLOBALS
// STATICS GLOBALS
// STATICS GLOBALS

// contatore dei necrologi
static int gContatoreNecrologi = 0;

// identificatore del file per AppWare
static short gRiferimentoFilePerAppWare = 0;

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	InizializzaContatoreNecrologi
	
* ------------------------------------------------------------------------ */
void XTAPI  InizializzaContatoreNecrologi() throw()
{
	gContatoreNecrologi = 0;
} // InizializzaContatoreNecrologi

/* ------------------------------------------------------------------------ *

	InizializzaAppWare

* ------------------------------------------------------------------------ */
void XTAPI InizializzaAppWare(appware *ptrappware) throw()
{
	(*ptrappware).contatore = 0;
	(*ptrappware).codiceDefunto[0] = '\0';
	(*ptrappware).codiceNecrologio[0] = '\0';
	(*ptrappware).tipoAppWare = kNessunAppWare;
	(*ptrappware).tipoRigore = kNessunRigore;
	(*ptrappware).tipoGruppo = kNessunGruppo;
	(*ptrappware).posizioneNelGruppo = kNessunaPosizione;
	(*ptrappware).altezzaBox = 0;
	(*ptrappware).posizionePagina = 0;
	(*ptrappware).posizioneColonna = 0;
	(*ptrappware).spazioTraParagrafi = 0;
	(*ptrappware).numeroParagrafiDiversi = 0;
	(*ptrappware).numeroParagrafi = 0;
	(*ptrappware).numeroLinee = 0;
	(*ptrappware).restringimento = 0;
	(*ptrappware).allargamento = 0;
	(*ptrappware).collegato = FALSE;
	(*ptrappware).codiceBox = 0;
} // InizializzaAppWare

/* ------------------------------------------------------------------------ *

	ImpostaContatoreNecrologi

* ------------------------------------------------------------------------ */
void XTAPI ImpostaContatoreNecrologi(appware *ptrappware) throw()
{
	gContatoreNecrologi++;
	(*ptrappware).contatore = gContatoreNecrologi;
} // ImpostaContatoreNecrologi

/* ------------------------------------------------------------------------ *

	ImpostaCodiceNecrologio

* ------------------------------------------------------------------------ */
void XTAPI ImpostaCodiceDefunto(appware *ptrappware, uchar *codicedefunto) throw()
{	
	STRCPY((*ptrappware).codiceDefunto, codicedefunto);
} // ImpostaCodiceDefunto

/* ------------------------------------------------------------------------ *

	ImpostaCodiceNecrologio

* ------------------------------------------------------------------------ */
void XTAPI ImpostaCodiceNecrologio(appware *ptrappware, uchar *codicenecrologio) throw()
{
	STRCPY((*ptrappware).codiceNecrologio, codicenecrologio);
} // ImpostaCodiceNecrologio

/* ------------------------------------------------------------------------ *

	ImpostaTipoAppWare

* ------------------------------------------------------------------------ */
void XTAPI ImpostaTipoAppWare(appware *ptrappware, tipoappware appware) throw()
{
	(*ptrappware).tipoAppWare = appware;
} // ImpostaTipoAppWare

/* ------------------------------------------------------------------------ *

	ImpostaTipoRigore

* ------------------------------------------------------------------------ */
void XTAPI ImpostaTipoRigore(appware *ptrappware, tiporigore rigore) throw()
{
	(*ptrappware).tipoRigore = rigore;
} // ImpostaTipoRigore

/* ------------------------------------------------------------------------ *

	ImpostaTipoGruppo

* ------------------------------------------------------------------------ */
void XTAPI ImpostaTipoGruppo(appware *ptrappware, uchar tipogruppo) throw()
{
	(*ptrappware).tipoGruppo = tipogruppo;
} // ImpostaTipoGruppo

/* ------------------------------------------------------------------------ *

	ImpostaPosizioneGruppo

* ------------------------------------------------------------------------ */
void XTAPI ImpostaPosizioneNelGruppo(appware *ptrappware, uchar posizionenelgruppo) throw()
{
	(*ptrappware).posizioneNelGruppo = posizionenelgruppo;
} // ImpostaPosizioneGruppo

/* ------------------------------------------------------------------------ *

	ImpostaAltezzaBox

* ------------------------------------------------------------------------ */
void XTAPI ImpostaAltezzaBox(appware *ptrappware, Fixed altezzabox) throw()
{
	(*ptrappware).altezzaBox = altezzabox;
} // ImpostaAltezzaBox

/* ------------------------------------------------------------------------ *

	ImpostaPosizionePagina

* ------------------------------------------------------------------------ */
void XTAPI ImpostaPosizionePagina(appware *ptrappware, int16 posizionepagina) throw()
{
	(*ptrappware).posizionePagina = posizionepagina;
} // ImpostaPosizionePagina

/* ------------------------------------------------------------------------ *

	ImpostaPosizioneColonna

* ------------------------------------------------------------------------ */
void XTAPI ImpostaPosizioneColonna(appware *ptrappware, int16 posizionecolonna) throw()
{
	(*ptrappware).posizioneColonna = posizionecolonna;
} // ImpostaPosizioneColonna

/* ------------------------------------------------------------------------ *

	ImpostaRestringimento

* ------------------------------------------------------------------------ */
void XTAPI ImpostaRestringimento(appware *ptrappware, Fixed restringimento) throw()
{
	(*ptrappware).restringimento = restringimento;
} // ImpostaRestringimento

/* ------------------------------------------------------------------------ *

	ImpostaSpazioTraParagrafi

* ------------------------------------------------------------------------ */
void XTAPI ImpostaSpazioTraParagrafi(appware *ptrappware, Fixed spaziotraparagrafi) throw()
{
	(*ptrappware).spazioTraParagrafi = spaziotraparagrafi;
} // ImpostaSpazioTraParagrafi

/* ------------------------------------------------------------------------ *

	ImpostaNumeroParagrafiDiversi

* ------------------------------------------------------------------------ */
void XTAPI ImpostaNumeroParagrafiDiversi(appware *ptrappware, int32 numeroparagrafi) throw()
{
	(*ptrappware).numeroParagrafiDiversi = numeroparagrafi;
} // ImpostaNumeroParagrafiDiversi

/* ------------------------------------------------------------------------ *

	ImpostaSpazioTraParagrafi

* ------------------------------------------------------------------------ */
void XTAPI ImpostaNumeroParagrafi(appware *ptrappware, int32 numeroparagrafi) throw()
{
	(*ptrappware).numeroParagrafi = numeroparagrafi;
} // ImpostaNumeroParagrafi

/* ------------------------------------------------------------------------ *

	ImpostaNumeroLinee

* ------------------------------------------------------------------------ */
void XTAPI ImpostaNumeroLinee(appware *ptrappware, int32 numerolinee) throw()
{
	(*ptrappware).numeroLinee = numerolinee;
} // ImpostaNumeroLinee

/* ------------------------------------------------------------------------ *

	ImpostaAllargamento

* ------------------------------------------------------------------------ */
void XTAPI ImpostaAllargamento(appware *ptrappware, Fixed allargamento) throw()
{
	(*ptrappware).allargamento = allargamento;
} // ImpostaAllargamento

/* ------------------------------------------------------------------------ *

	ImpostaCollegato

/* ------------------------------------------------------------------------ */
void XTAPI ImpostaCollegato(appware *ptrappware, Boolean collegato) throw()
{
	(*ptrappware).collegato = collegato;
} // ImpostaCollegato

/* ------------------------------------------------------------------------ *

	ImpostaIdentificatoreBox

* ------------------------------------------------------------------------ */
void XTAPI ImpostaCodiceBox(appware *ptrappware, uint32 codicebox) throw()
{
	(*ptrappware).codiceBox = codicebox;
} // ImpostaCodiceBox

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // APPWARE

/* Costanti */
/* Costanti */
/* Costanti */

#define kPrecisione 10
/* costante di moltiplicazione dei valori per appware */

#define kTipoFilePerAppWare TEXT
/* tipo del file di debug */

#define kCreatoreFilePerAppWare TEXT /*'QED1' */
/* creatore del file di debug */

#define kPosizionePagina 9
#define kPosizioneCodiceBox 16

#define kNumeroTabulatoriPagina kPosizionePagina - 1
/* numero dei tabulatori da saltare per leggere il codice box */

#define kNumeroTabulatoriCodiceBox kPosizioneCodiceBox - (kPosizionePagina + 2)
/* numero dei tabulatori da saltare per leggere il codice box */

static void StampaAppWare(char *stringa);
static void StampaAppWare(char *stringa)
/* ------------------------------------------------------------------------ *

	StampaAppWare

	7 Novembre 1995						Stefano

	Descrizione:
	Stampa su file AppWare 

	Parametri:
	stringa - puntatore alla stringa da stampare

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */
{
	long	lLung = 0L;
	
	if (stringa != NULL) {
			
		/* posizionamento in fondo per la scrittura */
		GetEOF(gRiferimentoFilePerAppWare, &lLung);
		SetFPos(gRiferimentoFilePerAppWare, 1, lLung);
	
		/* scrive la stringa */
		lLung =  (long) strlen(stringa);
		FSWrite(gRiferimentoFilePerAppWare, &lLung, (Ptr) stringa);
	}
}

static void StampaCampiAppWare(appware *ptrappware, Fixed altezzacolonna);
static void StampaCampiAppWare(appware *ptrappware, Fixed altezzacolonna)
/* ------------------------------------------------------------------------ *

	StampaCampiAppWare

	7 Novembre 1995						Stefano

	Descrizione:
	Stampa una linea sul file di AppWare con il valore corrente dei campi.
	Ogni campo e separato da un  tabulatore 

	Parametri:
	ptrappware - puntatore alla struttura da stampare
	altezzacolonna - altezza della colonna corrente

	Ritorno:
	nessuno

* ------------------------------------------------------------------------ */

{
	char lRiga[1024] ="";
	Fixed lAltezzaBox = 0;
	Fixed lRestringimento = 0;
	Fixed lAllargamento = 0;
	Fixed lAltezzaColonna = 0;
	
	/* moltiplico l'altezza per 10 */
	lAltezzaBox = (*ptrappware).altezzaBox;
	if (kCheckModificaSpazioTraNecrologi == TRUE)
		lAltezzaBox += kFixedMinimoSpazioTraNecrologi;
	else lAltezzaBox += kFixedSpazioTraNecrologi;
	lAltezzaBox *= kPrecisione;
	lRestringimento = (*ptrappware).restringimento * kPrecisione;
	lAllargamento = (*ptrappware).allargamento * kPrecisione;
	lAltezzaColonna = altezzacolonna * kPrecisione;
	
	sprintf(lRiga,
			"%d\t%s\t%s\t%c\t%c\t%c\t%c\t%ld\t%d\t%d\t%ld\t%ld\t%ld\t%ld\t%d\t%ld\r\n",
			(*ptrappware).contatore,
			(*ptrappware).codiceDefunto,
			(*ptrappware).codiceNecrologio,
			(*ptrappware).tipoAppWare,
			(*ptrappware).tipoGruppo,
			(*ptrappware).posizioneNelGruppo,
			(*ptrappware).tipoRigore,
			mLong(lAltezzaBox),
			(*ptrappware).posizionePagina,
			(*ptrappware).posizioneColonna,
			mLong(lRestringimento),
			mLong(lAllargamento),
			mLong(lAltezzaColonna),
			0L,
			(*ptrappware).collegato,
			(*ptrappware).codiceBox); /* */
			
	StampaAppWare(lRiga);
	
}

static errorixtension StampaNecrologiPerAppWare(void)
/* ------------------------------------------------------------------------ *

	StampaNecrologiPerAppWare

	11 Marzo 1996						Stefano

	Descrizione:
	Stampa i necrologi su file per AppWare

	Parametri:
	nessuno

	Ritorno:
	ritorna l'eventuale errore dell'xtension

* ------------------------------------------------------------------------ */
{
	int lPagina = 0;
	/* per scorrere le pagine */
	int lColonna = 0;
	/* per scorrere le colonne */
	spaziocolonna *lPtrSpazioColonna = NULL;
	/* per prendere le informazioni sulle colonne */
	Fixed lAltezzaColonna = 0;
	/* altezza della colonna */
	necrologio *lPtrNecrologio = NULL;
	/*per prendere le informazioni sui necrologi */
	appware lAppWare;
	/* per prendere le info da stampare */
	defunto *lPtrDefunto = NULL;
	/* per prendere le informazioni sui defunti */
	
	InizializzaContatoreNecrologi();
	
	/* stampo i necrologi in pagina */
	for (lPagina = 1; lPagina <= PrendiNumeroPagine(); lPagina++) {
		for (lColonna = 1; lColonna <= PrendiNumeroColonne(); lColonna++) {
			
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina,lColonna);
			
			/* prendo l'altezza della colonna */
			lAltezzaColonna = (*lPtrSpazioColonna).spazioDisponibile;
			
			/* imposto il puntatore sul primo necrologio della colonna */
			lPtrNecrologio = (*lPtrSpazioColonna).testaNecrologi;
			
			while (1) {
			
//				SystemTask();

				if (lPtrNecrologio == NULL) break;
				
				/* cerco il box con il codice specificato */
				gErroreXtension = TrovaSlugDiCodice((*lPtrNecrologio).codiceBox);
				if (gErroreXtension != kNessunErrore) {
					/* errore sulla ricerca del codice */
					StampaDebug("TrovaSlugDiCodice", (long) gErroreXtension);
					return(gErroreXtension);
				}
				
				/* inizializzo il record per appware */
				InizializzaAppWare(&lAppWare);
				
				/* prendo lo slug di AppWare */
				gErroreXtension = PrendiSlugDiAppWare(&lAppWare);
				if (gErroreXtension != kNessunErrore) {
					/* errore sulla ricerca del codice */
					StampaDebug("PrendiSlugDiAppWare", (long) gErroreXtension);
					return(gErroreXtension);
				}
				
				/* finisco di riempire le ultime info di appware */
				ImpostaContatoreNecrologi(&lAppWare);
				ImpostaPosizionePagina(&lAppWare, lPagina);
				ImpostaPosizioneColonna(&lAppWare, lColonna);
				
#if kOLD
				ImpostaCodiceBox(&lAppWare, (*lPtrNecrologio).codiceBox);
				ImpostaTipoGruppo(&lAppWare, (*lPtrNecrologio).tipoGruppo);
				ImpostaAltezzaBox(&lAppWare, (*lPtrNecrologio).altezzaBox);
				ImpostaCollegato(&lAppWare, (*lPtrNecrologio).collegato);
#endif /* kOLD */
				
				/* stampo le info di AppWare */
				StampaCampiAppWare(&lAppWare, lAltezzaColonna);

				/* passo la necrologio successivo */
				lPtrNecrologio = (*lPtrNecrologio).successivo;
			}

			/* imposto il puntatore al primo defunto della colonna */
			lPtrDefunto = (*lPtrSpazioColonna).testaDefunti;

			while (1) {

//				SystemTask();

				if (lPtrDefunto == NULL) break;

				/* imposto il puntatore sul primo necrologio codice A */
				lPtrNecrologio = (*lPtrDefunto).elencoNecrologiA;

				while (1) {

//					SystemTask();

					if (lPtrNecrologio == NULL) break;

					/* cerco il box con il codice specificato */
					gErroreXtension = TrovaSlugDiCodice((*lPtrNecrologio).codiceBox);
					if (gErroreXtension != kNessunErrore) {
						/* errore sulla ricerca del codice */
						StampaDebug("TrovaSlugDiCodice", (long) gErroreXtension);
						return(gErroreXtension);
					}

					/* inizializzo il record per appware */
					InizializzaAppWare(&lAppWare);

					/* prendo lo slug di AppWare */
					gErroreXtension = PrendiSlugDiAppWare(&lAppWare);
					if (gErroreXtension != kNessunErrore) {
						/* errore sulla ricerca del codice */
						StampaDebug("PrendiSlugDiAppWare", (long) gErroreXtension);
						return(gErroreXtension);
					}

					/* finisco di riempire le ultime info di appware */
					ImpostaContatoreNecrologi(&lAppWare);
					ImpostaPosizionePagina(&lAppWare, lPagina);
					ImpostaPosizioneColonna(&lAppWare, lColonna);

#if kOLD
					ImpostaCodiceBox(&lAppWare, (*lPtrNecrologio).codiceBox);
					ImpostaTipoGruppo(&lAppWare, (*lPtrNecrologio).tipoGruppo);
					ImpostaAltezzaBox(&lAppWare, (*lPtrNecrologio).altezzaBox);
					ImpostaCollegato(&lAppWare, (*lPtrNecrologio).collegato);
#endif /* kOLD */

					/* stampo le info di AppWare */
					StampaCampiAppWare(&lAppWare, lAltezzaColonna);

					/* passo la necrologio successivo */
					lPtrNecrologio = (*lPtrNecrologio).successivo;
				}

				/* imposto il puntatore sul primo necrologio codice B */
				lPtrNecrologio = (*lPtrDefunto).elencoNecrologiB;

				while (1) {

//					SystemTask();

					if (lPtrNecrologio == NULL) break;

					/* cerco il box con il codice specificato */
					gErroreXtension = TrovaSlugDiCodice((*lPtrNecrologio).codiceBox);
					if (gErroreXtension != kNessunErrore) {
						/* errore sulla ricerca del codice */
						StampaDebug("TrovaSlugDiCodice", (long) gErroreXtension);
						return(gErroreXtension);
					}

					/* inizializzo il record per appware */
					InizializzaAppWare(&lAppWare);

					/* prendo lo slug di AppWare */
					gErroreXtension = PrendiSlugDiAppWare(&lAppWare);
					if (gErroreXtension != kNessunErrore) {
						/* errore sulla ricerca del codice */
						StampaDebug("PrendiSlugDiAppWare", (long) gErroreXtension);
						return(gErroreXtension);
					}

					/* finisco di riempire le ultime info di appware */
					ImpostaContatoreNecrologi(&lAppWare);
					ImpostaPosizionePagina(&lAppWare, lPagina);
					ImpostaPosizioneColonna(&lAppWare, lColonna);

#if kOLD
					ImpostaCodiceBox(&lAppWare, (*lPtrNecrologio).codiceBox);
					ImpostaTipoGruppo(&lAppWare, (*lPtrNecrologio).tipoGruppo);
					ImpostaAltezzaBox(&lAppWare, (*lPtrNecrologio).altezzaBox);
					ImpostaCollegato(&lAppWare, (*lPtrNecrologio).collegato);
#endif /* kOLD */

					/* stampo le info di AppWare */
					StampaCampiAppWare(&lAppWare, lAltezzaColonna);

					/* passo la necrologio successivo */
					lPtrNecrologio = (*lPtrNecrologio).successivo;
				}

				/* imposto il puntatore sul primo necrologio codice C */
				lPtrNecrologio = (*lPtrDefunto).elencoNecrologiC;

				while (1) {

//					SystemTask();

					if (lPtrNecrologio == NULL) break;

					/* cerco il box con il codice specificato */
					gErroreXtension = TrovaSlugDiCodice((*lPtrNecrologio).codiceBox);
					if (gErroreXtension != kNessunErrore) {
						/* errore sulla ricerca del codice */
						StampaDebug("TrovaSlugDiCodice", (long) gErroreXtension);
						return(gErroreXtension);
					}
					
					/* inizializzo il record per appware */
					InizializzaAppWare(&lAppWare);

					/* prendo lo slug di AppWare */
					gErroreXtension = PrendiSlugDiAppWare(&lAppWare);
					if (gErroreXtension != kNessunErrore) {
						/* errore sulla ricerca del codice */
						StampaDebug("PrendiSlugDiAppWare", (long) gErroreXtension);
						return(gErroreXtension);
					}

					/* finisco di riempire le ultime info di appware */
					ImpostaContatoreNecrologi(&lAppWare);
					ImpostaPosizionePagina(&lAppWare, lPagina);
					ImpostaPosizioneColonna(&lAppWare, lColonna);
					
#if kOLD
					ImpostaCodiceBox(&lAppWare, (*lPtrNecrologio).codiceBox);
					ImpostaTipoGruppo(&lAppWare, (*lPtrNecrologio).tipoGruppo);
					ImpostaAltezzaBox(&lAppWare, (*lPtrNecrologio).altezzaBox);
					ImpostaCollegato(&lAppWare, (*lPtrNecrologio).collegato);
#endif /* kOLD */

					/* stampo le info di AppWare */
					StampaCampiAppWare(&lAppWare, lAltezzaColonna);
					
					/* passo la necrologio successivo */
					lPtrNecrologio = (*lPtrNecrologio).successivo;
				}

				/* passo al defunto successivo */
				lPtrDefunto = (*lPtrDefunto).successivo;
			}

			
			/* imposto il puntatore al primo defunto in fondo alla colonna */
			lPtrDefunto = (*lPtrSpazioColonna).testaFondoDefunti;
			
			while (1) {
			
//				SystemTask();

				if (lPtrDefunto == NULL) break;
				
				/* imposto il puntatore sul primo necrologio codice A */
				lPtrNecrologio = (*lPtrDefunto).elencoNecrologiA;

				while (1) {

//					SystemTask();

					if (lPtrNecrologio == NULL) break;

					/* cerco il box con il codice specificato */
					gErroreXtension = TrovaSlugDiCodice((*lPtrNecrologio).codiceBox);
					if (gErroreXtension != kNessunErrore) {
						/* errore sulla ricerca del codice */
						StampaDebug("TrovaSlugDiCodice", (long) gErroreXtension);
						return(gErroreXtension);
					}

					/* inizializzo il record per appware */
					InizializzaAppWare(&lAppWare);

					/* prendo lo slug di AppWare */
					gErroreXtension = PrendiSlugDiAppWare(&lAppWare);
					if (gErroreXtension != kNessunErrore) {
						/* errore sulla ricerca del codice */
						StampaDebug("PrendiSlugDiAppWare", (long) gErroreXtension);
						return(gErroreXtension);
					}

					/* finisco di riempire le ultime info di appware */
					ImpostaContatoreNecrologi(&lAppWare);
					ImpostaPosizionePagina(&lAppWare, lPagina);
					ImpostaPosizioneColonna(&lAppWare, lColonna);

#if kOLD
					ImpostaCodiceBox(&lAppWare, (*lPtrNecrologio).codiceBox);
					ImpostaTipoGruppo(&lAppWare, (*lPtrNecrologio).tipoGruppo);
					ImpostaAltezzaBox(&lAppWare, (*lPtrNecrologio).altezzaBox);
					ImpostaCollegato(&lAppWare, (*lPtrNecrologio).collegato);
#endif /* kOLD */

					/* stampo le info di AppWare */
					StampaCampiAppWare(&lAppWare, lAltezzaColonna);

					/* passo la necrologio successivo */
					lPtrNecrologio = (*lPtrNecrologio).successivo;
				}

				/* imposto il puntatore sul primo necrologio codice B */
				lPtrNecrologio = (*lPtrDefunto).elencoNecrologiB;

				while (1) {

//					SystemTask();

					if (lPtrNecrologio == NULL) break;

					/* cerco il box con il codice specificato */
					gErroreXtension = TrovaSlugDiCodice((*lPtrNecrologio).codiceBox);
					if (gErroreXtension != kNessunErrore) {
						/* errore sulla ricerca del codice */
						StampaDebug("TrovaSlugDiCodice", (long) gErroreXtension);
						return(gErroreXtension);
					}

					/* inizializzo il record per appware */
					InizializzaAppWare(&lAppWare);

					/* prendo lo slug di AppWare */
					gErroreXtension = PrendiSlugDiAppWare(&lAppWare);
					if (gErroreXtension != kNessunErrore) {
						/* errore sulla ricerca del codice */
						StampaDebug("PrendiSlugDiAppWare", (long) gErroreXtension);
						return(gErroreXtension);
					}

					/* finisco di riempire le ultime info di appware */
					ImpostaContatoreNecrologi(&lAppWare);
					ImpostaPosizionePagina(&lAppWare, lPagina);
					ImpostaPosizioneColonna(&lAppWare, lColonna);

#if kOLD
					ImpostaCodiceBox(&lAppWare, (*lPtrNecrologio).codiceBox);
					ImpostaTipoGruppo(&lAppWare, (*lPtrNecrologio).tipoGruppo);
					ImpostaAltezzaBox(&lAppWare, (*lPtrNecrologio).altezzaBox);
					ImpostaCollegato(&lAppWare, (*lPtrNecrologio).collegato);
#endif /* kOLD */

					/* stampo le info di AppWare */
					StampaCampiAppWare(&lAppWare, lAltezzaColonna);

					/* passo la necrologio successivo */
					lPtrNecrologio = (*lPtrNecrologio).successivo;
				}

				/* imposto il puntatore sul primo necrologio codice C */
				lPtrNecrologio = (*lPtrDefunto).elencoNecrologiC;

				while (1) {

//					SystemTask();

					if (lPtrNecrologio == NULL) break;

					/* cerco il box con il codice specificato */
					gErroreXtension = TrovaSlugDiCodice((*lPtrNecrologio).codiceBox);
					if (gErroreXtension != kNessunErrore) {
						/* errore sulla ricerca del codice */
						StampaDebug("TrovaSlugDiCodice", (long) gErroreXtension);
						return(gErroreXtension);
					}
					
					/* inizializzo il record per appware */
					InizializzaAppWare(&lAppWare);

					/* prendo lo slug di AppWare */
					gErroreXtension = PrendiSlugDiAppWare(&lAppWare);
					if (gErroreXtension != kNessunErrore) {
						/* errore sulla ricerca del codice */
						StampaDebug("PrendiSlugDiAppWare", (long) gErroreXtension);
						return(gErroreXtension);
					}

					/* finisco di riempire le ultime info di appware */
					ImpostaContatoreNecrologi(&lAppWare);
					ImpostaPosizionePagina(&lAppWare, lPagina);
					ImpostaPosizioneColonna(&lAppWare, lColonna);
					
#if kOLD
					ImpostaCodiceBox(&lAppWare, (*lPtrNecrologio).codiceBox);
					ImpostaTipoGruppo(&lAppWare, (*lPtrNecrologio).tipoGruppo);
					ImpostaAltezzaBox(&lAppWare, (*lPtrNecrologio).altezzaBox);
					ImpostaCollegato(&lAppWare, (*lPtrNecrologio).collegato);
#endif /* kOLD */

					/* stampo le info di AppWare */
					StampaCampiAppWare(&lAppWare, lAltezzaColonna);
					
					/* passo la necrologio successivo */
					lPtrNecrologio = (*lPtrNecrologio).successivo;
				}

				/* passo al defunto successivo */
				lPtrDefunto = (*lPtrDefunto).successivo;
			}
		}
	}
	
	/* stampo i necrologi fuori pagina */			
	lPtrSpazioColonna = PrendiSpazioColonnaFuori();
	
	/* fisso l'altezza della colonna */
	lAltezzaColonna = 0;
	lColonna = 0;
	lPagina = 0;

	/* imposto il puntatore sul primo necrologio della colonna */
	lPtrNecrologio = (*lPtrSpazioColonna).testaNecrologi;
	
	while (1) {
	
//		SystemTask();
		
		if (lPtrNecrologio == NULL) break;
		
		/* cerco il box con il codice specificato */
		gErroreXtension = TrovaSlugDiCodice((*lPtrNecrologio).codiceBox);
		if (gErroreXtension != kNessunErrore) {
			/* errore sulla ricerca del codice */
			StampaDebug("TrovaSlugDiCodice", (long) gErroreXtension);
			return(gErroreXtension);
		}
		
		/* inizializzo il record per appware */
		InizializzaAppWare(&lAppWare);
		
		/* prendo lo slug di AppWare */
		gErroreXtension = PrendiSlugDiAppWare(&lAppWare);
		if (gErroreXtension != kNessunErrore) {
			/* errore sulla ricerca del codice */
			StampaDebug("PrendiSlugDiAppWare", (long) gErroreXtension);
			return(gErroreXtension);
		}
		
		/* finisco di riempire le ultime info di appware */
		ImpostaContatoreNecrologi(&lAppWare);
		ImpostaPosizionePagina(&lAppWare, 0);
		ImpostaPosizioneColonna(&lAppWare, 0);
		
#if kOLD
		ImpostaCodiceBox(&lAppWare, (*lPtrNecrologio).codiceBox);
		ImpostaTipoGruppo(&lAppWare, (*lPtrNecrologio).tipoGruppo);
		ImpostaAltezzaBox(&lAppWare, (*lPtrNecrologio).altezzaBox);
		ImpostaCollegato(&lAppWare, (*lPtrNecrologio).collegato);
#endif /* kOLD */

		/* stampo le info di AppWare */
		StampaCampiAppWare(&lAppWare, lAltezzaColonna);

		/* passo la necrologio successivo */
		lPtrNecrologio = (*lPtrNecrologio).successivo;
	}

	/* imposto il puntatore al primo defunto fuori della colonna */
	lPtrDefunto = (*lPtrSpazioColonna).testaDefunti;

	while (1) {

//		SystemTask();

		if (lPtrDefunto == NULL) break;

		/* imposto il puntatore sul primo necrologio codice A */
		lPtrNecrologio = (*lPtrDefunto).elencoNecrologiA;

		while (1) {

//			SystemTask();

			if (lPtrNecrologio == NULL) break;

			/* cerco il box con il codice specificato */
			gErroreXtension = TrovaSlugDiCodice((*lPtrNecrologio).codiceBox);
			if (gErroreXtension != kNessunErrore) {
				/* errore sulla ricerca del codice */
				StampaDebug("TrovaSlugDiCodice", (long) gErroreXtension);
				return(gErroreXtension);
			}

			/* inizializzo il record per appware */
			InizializzaAppWare(&lAppWare);

			/* prendo lo slug di AppWare */
			gErroreXtension = PrendiSlugDiAppWare(&lAppWare);
			if (gErroreXtension != kNessunErrore) {
				/* errore sulla ricerca del codice */
				StampaDebug("PrendiSlugDiAppWare", (long) gErroreXtension);
				return(gErroreXtension);
			}

			/* finisco di riempire le ultime info di appware */
			ImpostaContatoreNecrologi(&lAppWare);
			ImpostaPosizionePagina(&lAppWare, lPagina);
			ImpostaPosizioneColonna(&lAppWare, lColonna);

#if kOLD
			ImpostaCodiceBox(&lAppWare, (*lPtrNecrologio).codiceBox);
			ImpostaTipoGruppo(&lAppWare, (*lPtrNecrologio).tipoGruppo);
			ImpostaAltezzaBox(&lAppWare, (*lPtrNecrologio).altezzaBox);
			ImpostaCollegato(&lAppWare, (*lPtrNecrologio).collegato);
#endif /* kOLD */

			/* stampo le info di AppWare */
			StampaCampiAppWare(&lAppWare, lAltezzaColonna);

			/* passo la necrologio successivo */
			lPtrNecrologio = (*lPtrNecrologio).successivo;
		}

		/* imposto il puntatore sul primo necrologio codice B */
		lPtrNecrologio = (*lPtrDefunto).elencoNecrologiB;

		while (1) {

//			SystemTask();

			if (lPtrNecrologio == NULL) break;

			/* cerco il box con il codice specificato */
			gErroreXtension = TrovaSlugDiCodice((*lPtrNecrologio).codiceBox);
			if (gErroreXtension != kNessunErrore) {
				/* errore sulla ricerca del codice */
				StampaDebug("TrovaSlugDiCodice", (long) gErroreXtension);
				return(gErroreXtension);
			}

			/* inizializzo il record per appware */
			InizializzaAppWare(&lAppWare);

			/* prendo lo slug di AppWare */
			gErroreXtension = PrendiSlugDiAppWare(&lAppWare);
			if (gErroreXtension != kNessunErrore) {
				/* errore sulla ricerca del codice */
				StampaDebug("PrendiSlugDiAppWare", (long) gErroreXtension);
				return(gErroreXtension);
			}

			/* finisco di riempire le ultime info di appware */
			ImpostaContatoreNecrologi(&lAppWare);
			ImpostaPosizionePagina(&lAppWare, lPagina);
			ImpostaPosizioneColonna(&lAppWare, lColonna);

#if kOLD
			ImpostaCodiceBox(&lAppWare, (*lPtrNecrologio).codiceBox);
			ImpostaTipoGruppo(&lAppWare, (*lPtrNecrologio).tipoGruppo);
			ImpostaAltezzaBox(&lAppWare, (*lPtrNecrologio).altezzaBox);
			ImpostaCollegato(&lAppWare, (*lPtrNecrologio).collegato);
#endif /* kOLD */

			/* stampo le info di AppWare */
			StampaCampiAppWare(&lAppWare, lAltezzaColonna);

			/* passo la necrologio successivo */
			lPtrNecrologio = (*lPtrNecrologio).successivo;
		}

		/* imposto il puntatore sul primo necrologio codice C */
		lPtrNecrologio = (*lPtrDefunto).elencoNecrologiC;

		while (1) {

//			SystemTask();

			if (lPtrNecrologio == NULL) break;

			/* cerco il box con il codice specificato */
			gErroreXtension = TrovaSlugDiCodice((*lPtrNecrologio).codiceBox);
			if (gErroreXtension != kNessunErrore) {
				/* errore sulla ricerca del codice */
				StampaDebug("TrovaSlugDiCodice", (long) gErroreXtension);
				return(gErroreXtension);
			}

			/* inizializzo il record per appware */
			InizializzaAppWare(&lAppWare);

			/* prendo lo slug di AppWare */
			gErroreXtension = PrendiSlugDiAppWare(&lAppWare);
			if (gErroreXtension != kNessunErrore) {
				/* errore sulla ricerca del codice */
				StampaDebug("PrendiSlugDiAppWare", (long) gErroreXtension);
				return(gErroreXtension);
			}

			/* finisco di riempire le ultime info di appware */
			ImpostaContatoreNecrologi(&lAppWare);
			ImpostaPosizionePagina(&lAppWare, lPagina);
			ImpostaPosizioneColonna(&lAppWare, lColonna);

#if kOLD
			ImpostaCodiceBox(&lAppWare, (*lPtrNecrologio).codiceBox);
			ImpostaTipoGruppo(&lAppWare, (*lPtrNecrologio).tipoGruppo);
			ImpostaAltezzaBox(&lAppWare, (*lPtrNecrologio).altezzaBox);
			ImpostaCollegato(&lAppWare, (*lPtrNecrologio).collegato);
#endif /* kOLD */

			/* stampo le info di AppWare */
			StampaCampiAppWare(&lAppWare, lAltezzaColonna);

			/* passo la necrologio successivo */
			lPtrNecrologio = (*lPtrNecrologio).successivo;
		}

		/* passo al defunto successivo */
		lPtrDefunto = (*lPtrDefunto).successivo;
	}

	/* imposto il puntatore al primo defunto fuori in fondo alla colonna */
	lPtrDefunto = (*lPtrSpazioColonna).testaFondoDefunti;

	while (1) {

//		SystemTask();

		if (lPtrDefunto == NULL) break;

		/* imposto il puntatore sul primo necrologio codice A */
		lPtrNecrologio = (*lPtrDefunto).elencoNecrologiA;

		while (1) {

//			SystemTask();

			if (lPtrNecrologio == NULL) break;

			/* cerco il box con il codice specificato */
			gErroreXtension = TrovaSlugDiCodice((*lPtrNecrologio).codiceBox);
			if (gErroreXtension != kNessunErrore) {
				/* errore sulla ricerca del codice */
				StampaDebug("TrovaSlugDiCodice", (long) gErroreXtension);
				return(gErroreXtension);
			}

			/* inizializzo il record per appware */
			InizializzaAppWare(&lAppWare);

			/* prendo lo slug di AppWare */
			gErroreXtension = PrendiSlugDiAppWare(&lAppWare);
			if (gErroreXtension != kNessunErrore) {
				/* errore sulla ricerca del codice */
				StampaDebug("PrendiSlugDiAppWare", (long) gErroreXtension);
				return(gErroreXtension);
			}

			/* finisco di riempire le ultime info di appware */
			ImpostaContatoreNecrologi(&lAppWare);
			ImpostaPosizionePagina(&lAppWare, lPagina);
			ImpostaPosizioneColonna(&lAppWare, lColonna);

#if kOLD
			ImpostaCodiceBox(&lAppWare, (*lPtrNecrologio).codiceBox);
			ImpostaTipoGruppo(&lAppWare, (*lPtrNecrologio).tipoGruppo);
			ImpostaAltezzaBox(&lAppWare, (*lPtrNecrologio).altezzaBox);
			ImpostaCollegato(&lAppWare, (*lPtrNecrologio).collegato);
#endif /* kOLD */

			/* stampo le info di AppWare */
			StampaCampiAppWare(&lAppWare, lAltezzaColonna);

			/* passo la necrologio successivo */
			lPtrNecrologio = (*lPtrNecrologio).successivo;
		}

		/* imposto il puntatore sul primo necrologio codice B */
		lPtrNecrologio = (*lPtrDefunto).elencoNecrologiB;

		while (1) {

//			SystemTask();

			if (lPtrNecrologio == NULL) break;

			/* cerco il box con il codice specificato */
			gErroreXtension = TrovaSlugDiCodice((*lPtrNecrologio).codiceBox);
			if (gErroreXtension != kNessunErrore) {
				/* errore sulla ricerca del codice */
				StampaDebug("TrovaSlugDiCodice", (long) gErroreXtension);
				return(gErroreXtension);
			}

			/* inizializzo il record per appware */
			InizializzaAppWare(&lAppWare);

			/* prendo lo slug di AppWare */
			gErroreXtension = PrendiSlugDiAppWare(&lAppWare);
			if (gErroreXtension != kNessunErrore) {
				/* errore sulla ricerca del codice */
				StampaDebug("PrendiSlugDiAppWare", (long) gErroreXtension);
				return(gErroreXtension);
			}

			/* finisco di riempire le ultime info di appware */
			ImpostaContatoreNecrologi(&lAppWare);
			ImpostaPosizionePagina(&lAppWare, lPagina);
			ImpostaPosizioneColonna(&lAppWare, lColonna);

#if kOLD
			ImpostaCodiceBox(&lAppWare, (*lPtrNecrologio).codiceBox);
			ImpostaTipoGruppo(&lAppWare, (*lPtrNecrologio).tipoGruppo);
			ImpostaAltezzaBox(&lAppWare, (*lPtrNecrologio).altezzaBox);
			ImpostaCollegato(&lAppWare, (*lPtrNecrologio).collegato);
#endif /* kOLD */

			/* stampo le info di AppWare */
			StampaCampiAppWare(&lAppWare, lAltezzaColonna);

			/* passo la necrologio successivo */
			lPtrNecrologio = (*lPtrNecrologio).successivo;
		}

		/* imposto il puntatore sul primo necrologio codice C */
		lPtrNecrologio = (*lPtrDefunto).elencoNecrologiC;

		while (1) {

//			SystemTask();

			if (lPtrNecrologio == NULL) break;

			/* cerco il box con il codice specificato */
			gErroreXtension = TrovaSlugDiCodice((*lPtrNecrologio).codiceBox);
			if (gErroreXtension != kNessunErrore) {
				/* errore sulla ricerca del codice */
				StampaDebug("TrovaSlugDiCodice", (long) gErroreXtension);
				return(gErroreXtension);
			}

			/* inizializzo il record per appware */
			InizializzaAppWare(&lAppWare);

			/* prendo lo slug di AppWare */
			gErroreXtension = PrendiSlugDiAppWare(&lAppWare);
			if (gErroreXtension != kNessunErrore) {
				/* errore sulla ricerca del codice */
				StampaDebug("PrendiSlugDiAppWare", (long) gErroreXtension);
				return(gErroreXtension);
			}

			/* finisco di riempire le ultime info di appware */
			ImpostaContatoreNecrologi(&lAppWare);
			ImpostaPosizionePagina(&lAppWare, lPagina);
			ImpostaPosizioneColonna(&lAppWare, lColonna);

#if kOLD
			ImpostaCodiceBox(&lAppWare, (*lPtrNecrologio).codiceBox);
			ImpostaTipoGruppo(&lAppWare, (*lPtrNecrologio).tipoGruppo);
			ImpostaAltezzaBox(&lAppWare, (*lPtrNecrologio).altezzaBox);
			ImpostaCollegato(&lAppWare, (*lPtrNecrologio).collegato);
#endif /* kOLD */

			/* stampo le info di AppWare */
			StampaCampiAppWare(&lAppWare, lAltezzaColonna);

			/* passo la necrologio successivo */
			lPtrNecrologio = (*lPtrNecrologio).successivo;
		}

		/* passo al defunto successivo */
		lPtrDefunto = (*lPtrDefunto).successivo;
	}

	setcurbox(NULL);

	return(kNessunErrore);
}

errorixtension StampaFilePerAppWare(void)
/* ------------------------------------------------------------------------ *

	StampaFilePerAppWare

	11 Marzo 1996						Stefano

	Descrizione:
	Per stampare il file per AppWare

	Parametri:
	nessuno

	Ritorno:
	ritorna l'eventuale errore dell'xtension

* ------------------------------------------------------------------------ */

{

	long lIdentificatoreCartella = 0;
	/* identificatore della cartella per AppWare */
	short lWDCartella = 0;
	/* working directory della cartella per AppWare */
//	FSSpec lFSSpecCartella = {0, 0, "\p"};
	/* FSSpec della cartella per AppWare */
	char lNomeCartella[80];


	strcpy(lNomeCartella,PercorsoCompleto(kStrCartellaPerAppWare));
	strcat( lNomeCartella, "\\");
	strcat( lNomeCartella, kStrDocumentoPerAppWare );
	/* costruisco il FSSpec del cartella del file dei dati dell'ultima impaginazione */
/*	gErrore = CreaFSSpecCartella(lNomeCartellaUltimaImpaginazione,
								 &lFSSpecCartellaUltimaImpaginazione);
*/


	/* cancello l'eventuale file vecchio */	
	FSDelete(lNomeCartella, 0);

	/* creo il file per AppWare */
	gErrore = Create(lNomeCartella,
			  0,
			  kCreatoreFilePerAppWare,
			  kTipoFilePerAppWare);
	if (gErrore != noErr) {
		/* errore sulla creazione del file per AppWare */
		StampaDebug("Create", gErrore);
		return(kErroreDiSistema);
	}	
	
	/* apertura del file per AppWare */
	gErrore = FSOpen(lNomeCartella,
					 0,
				  	 &gRiferimentoFilePerAppWare);
	if (gErrore != noErr) {
		/* errore sulla chiusura della cartella del documento definitivo */
		StampaDebug("FSOpen", gErrore);
		return(kErroreDiSistema);
	}


	/* stampo le info sui necrologi */
	gErroreXtension = StampaNecrologiPerAppWare();
	if (gErroreXtension != kNessunErrore) {
		/* errore nella scrittura del file per AppWare */
		StampaDebug("StampaNecrologiPerAppWare", gErroreXtension);
		return(gErroreXtension);
	}
	
	/* chiudo il file per AppWare */
	gErrore = FSClose(gRiferimentoFilePerAppWare);
	if (gErrore != noErr) {
		/* errore nella chiusura del file per AppWare */
		StampaDebug("FSClose", gErrore);
		return(kErroreDiSistema);
	}
	return(kNessunErrore);
}


errorixtension InserisciNecrologiNelleColonne(char *ptrdocumento)
/* ------------------------------------------------------------------------ *

	InserisciNecrologiNelleColonne

	11 Marzo 1996						Stefano

	Descrizione:
	Leggendo le info che arrivano dal file per AppWare inserisco i necrologi
	nelle colonne

	Parametri:
	nessuno

	Ritorno:
	ritorna l'errore dell'xtension

* ------------------------------------------------------------------------ */
{
	Size lContatore = 0;
	/* per determinare la lunghezza del testo da leggere */	
	char *lPtrInizio = NULL;
	/* punta all'inizio del testo */
	int lPagina = 0;
	/* per scorrere le pagine */
	int lColonna = 0;
	/* per scorrere le colonne */
	int lContatoreTabulatori = 0;
	/* per contare i tabulatori da saltare */
	unsigned long lCodiceBox = 0;
	/* per leggere il codice box */
	appware lAppWare;
	/* per prendere le informazioni dello slug di AppWare */
	necrologio *lPtrNecrologio = NULL;
	/* per creare i necrologi da insarire nelle colonne */
	spaziocolonna *lPtrSpazioColonna = NULL;
	/* per prendere le info della colonna in cui fare l'inserimento */
	
	while (1) {
		if (*ptrdocumento == kFineFile) break;
		
		/* inizializzo il contatore dei tabulatori */
		lContatoreTabulatori = 0;
		while (1) {
			if (*ptrdocumento == kTabulatore) {
				/* incremento il contatore dei tabultori */
				lContatoreTabulatori++;
				if (lContatoreTabulatori == kNumeroTabulatoriPagina) break;
			}
			ptrdocumento++;
		}
		
		/* avanzo il puntatore */
		ptrdocumento++;
		
		/* leggo la pagina */
		lPtrInizio = ptrdocumento;
		lContatore = 0;
		while (1) {
			if (*ptrdocumento == kTabulatore) break;
			ptrdocumento++;
			lContatore++;
		}
		if (lContatore == 0) lPagina = 0;
		else {
			/* devo prelevare il valore della stringa puntato da lPtrInizio */
			BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
			gStringaC[lContatore] = kFineFile;
			sscanf(gStringaC,"%d", &lPagina);
		}
		
		/* avanzo il puntatore */
		ptrdocumento++;
		
		/* leggo la colonna */
		lPtrInizio = ptrdocumento;
		lContatore = 0;
		while (1) {
			if (*ptrdocumento == kTabulatore) break;
			ptrdocumento++;
			lContatore++;
		}
		if (lContatore == 0) lColonna = 0;
		else {
			/* devo prelevare il valore della stringa puntato da lPtrInizio */
			BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
			gStringaC[lContatore] = kFineFile;
			sscanf(gStringaC,"%d", &lColonna);
		}
		
		/* avanzo il puntatore */
		ptrdocumento++;
		
		/* inizializzo il contatore dei tabulatori */
		lContatoreTabulatori = 0;
		while (1) {
			if (*ptrdocumento == kTabulatore) {
				/* incremento il contatore dei tabultori */
				lContatoreTabulatori++;
				if (lContatoreTabulatori == kNumeroTabulatoriCodiceBox) break;
			}
			ptrdocumento++;
		}
		
		/* avanzo il puntatore */
		ptrdocumento++;
		
		/* leggo il codice del box */
		lPtrInizio = ptrdocumento;
		lContatore = 0;
		while (1) {
			if (*ptrdocumento == kRitornoACapo) break;
			ptrdocumento++;
			lContatore++;
		}
		if (lContatore == 0) lCodiceBox = 0;
		else {
			/* devo prelevare il valore della stringa puntato da lPtrInizio */
			BlockMove((Ptr) lPtrInizio, (Ptr) gStringaC, lContatore);
			gStringaC[lContatore] = kFineFile;
			sscanf(gStringaC,"%ld", &lCodiceBox);
		}
		
		/* avanzo il puntatore */
		ptrdocumento++;
		
		/* cerco il box con il codice specificato */
		gErroreXtension = TrovaSlugDiCodice(lCodiceBox);
		if (gErroreXtension != kNessunErrore) {
			/* errore sulla ricerca del codice */
			StampaDebug("TrovaSlugDiCodice", (long) gErroreXtension);
			return(gErroreXtension);
		}
		
		/* inizializzo il record per AppWare */
		InizializzaAppWare(&lAppWare);

		/* prendo lo slug di AppWare */
		gErroreXtension = PrendiSlugDiAppWare(&lAppWare);
		if (gErroreXtension != kNessunErrore) {
			/* errore sulla ricerca del codice */
			StampaDebug("PrendiSlugDiAppWare", (long) gErroreXtension);
			return(gErroreXtension);
		}
			
		/* costruisco il necrologio */
		lPtrNecrologio = (necrologio*) NewPtrClear(sizeof(necrologio));
		if (lPtrNecrologio == NULL) {
			/* errore di memoria */
			StampaDebug("NewPtrClear", 0);
			return(kErroreMemoria);
		}
		
		/* inserisco le info al necrologio */
		(*lPtrNecrologio).codiceBox = lCodiceBox;
		(*lPtrNecrologio).tipoGruppo = lAppWare.tipoGruppo;
		(*lPtrNecrologio).altezzaBox = lAppWare.altezzaBox;
		(*lPtrNecrologio).spazioTraParagrafi = lAppWare.spazioTraParagrafi;
		(*lPtrNecrologio).numeroParagrafiDiversi = lAppWare.numeroParagrafiDiversi;
		(*lPtrNecrologio).numeroParagrafi = lAppWare.numeroParagrafi;
		(*lPtrNecrologio).numeroLinee = lAppWare.numeroLinee;
		
		if (1 <= lPagina && lPagina <= PrendiNumeroPagine() &&
			1 <= lColonna && lColonna <= PrendiNumeroColonne()) {
			/* prendo le info della colonna in cui fare l'inserimento */
			lPtrSpazioColonna = PrendiSpazioColonna(lPagina, lColonna);
		} else {
			/* prendo le info della colonna in cui fare l'inserimento */
			lPtrSpazioColonna = PrendiSpazioColonnaFuori();
		}
		
		/* inserisco il necrologio nella pagina */
		InserisciNecrologioInCoda(lPtrSpazioColonna, lPtrNecrologio);

	}
	
	setcurbox(NULL);
	
	return(kNessunErrore);
}
#endif // APPWARE
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/