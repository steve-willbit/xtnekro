/* ------------------------------------------------------------------------ *

	Immagini.cpp

	19 Aprile 2005						Fabrizio

	Descrizione:
	Contiene le procedure usate per importare le immagini all'interno del box
	immagine corrente
	
	Log: Immagini.cpp $
		
* ------------------------------------------------------------------------ */

// CONFIG INCLUDES
// CONFIG INCLUDES
// CONFIG INCLUDES

// always the first
#include "XTConfig.h"
#include "QXPConfig.h"

// STANDARD INCLUDES
// STANDARD INCLUDES
// STANDARD INCLUDES

#if QXP60
#if defined(__MWERKS__) && defined(__MACH__)
	#define TARGET_API_MAC_OSX 1
	#include <MSL MacHeadersMach-O.h>
#endif // defined(__MWERKS__) && defined(__MACH__)
#endif // QXP60

#include <stdio.h>
#include <string.h>

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

#include "Errori.h"
#include "Pref.h"
#include "DocInfo.h"
#include "Coord.h"
#include "Box.h"
#include "ResStr.h"
#include "StMes.h"
#include "ReadFile.h"
#include "AppWare.h"
#include "PresFile.h"
#include "Memoria.h"
#include "GesSlug.h"
#include "Tempo.h"
#include "Memoria.h"

#include "Immagini.h"

// STATICS GLOBALS
// STATICS GLOBALS
// STATICS GLOBALS

// per memorizzare l'immagine campione
static xtboxptr gPtrImmagineCampione = NULL;

// FSSpec dell'immagine da importare
static uchar  gFSSpecImmagine[kDimensioneStringhe];

// nome completo dell'immagine
static char gNomeCompletoImmagine[kDimensioneStringhe] = "";

// globale contenente puntatore all'immagine
uchar* gPtrImm = NULL;

extern Handle gHndlFile;

// per memorizzare l'i0mmagine campione del simbolo di pax
static xtboxptr gPtrImmagineCampionePax = NULL;

// il box campione che contiene il simbolo di Pax preso prima di impaginare
static boxid gBoxCampioneDiPax = 0L;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			ControllaPresenzaImmagini
	@abstract 		gestione immagini.
	@discussion		Legge il nome delle immagini dall'handle in input e ne verifica la presenza
					nella relativa cartella

					9 Maggio 2005 - Fabrizio.

	@param 			handle - puntatore all'handle che contiene il documento da impaginare
	@result  			ritorna l'eventuale errore dell'xtension
*/
static errorixtension XTAPI ControllaPresenzaImmagini(uchar** handle) throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	ImpostaImmagineCampione

* ------------------------------------------------------------------------ */
void XTAPI ImpostaImmagineCampione() throw()
{
	if (gPtrImmagineCampione != NULL) 
	{
		// libero le info precedenti
		disposextbox(gPtrImmagineCampione,TRUE);
		gPtrImmagineCampione = NULL;
	}

	// cerco il box immagine campione
	gErroreXtension = TrovaSlugDiImmagineCampione();
	if (gErroreXtension != kNessunErrore) 
	{
		// non ho trovato il box immagine campione

	} else 
	{
		// prendo le caratteristiche del box immagine campione
		gPtrImmagineCampione = getxtbox(NULL, NULL, FALSE);
	}

	xtset_curbox(NULL);
} // ImpostaImmagineCampione

/* ------------------------------------------------------------------------ *

	PrendiImmagineCampione

* ------------------------------------------------------------------------ */
xtboxptr XTAPI PrendiImmagineCampione() throw()
{
	return(gPtrImmagineCampione);
} // PrendiImmagineCampione


/* ------------------------------------------------------------------------ *

	PresenzaImmagine
	
* ------------------------------------------------------------------------ */
OSErr XTAPI PresenzaImmagine(uchar *nomeimmagine) throw()
{
	// costruisco il nome completo dell'immagine da importare
	STRCPY(gNomeCompletoImmagine, gXtPreferences.immaginiPref.folderImg);
	STRCAT(gNomeCompletoImmagine, "\\");
	STRCAT(gNomeCompletoImmagine, nomeimmagine);
	
	// controllo se l'immagine e' presente su file system
	STRCPY( gFSSpecImmagine, gNomeCompletoImmagine);
	gErrore = PresenzaFile(gFSSpecImmagine);
	if (gErrore == noErr)
		goto _Uscita;
	
	// controllo se il nome immagine contiene gia' l'estensione
	if (STRLEN(gXtPreferences.immaginiPref.estensioneImg) > 0)
	{
		if (STRSTR(gNomeCompletoImmagine, gXtPreferences.immaginiPref.estensioneImg) == NULL)
		{
			// concateno l'estensione delle immagini
			STRCAT(gNomeCompletoImmagine, ".");
			STRCAT(gNomeCompletoImmagine, gXtPreferences.immaginiPref.estensioneImg);
		}	
	}
	
	// controllo la presenza del relativo file immagine EPS
	STRCPY(gFSSpecImmagine, gNomeCompletoImmagine);
	gErrore = PresenzaFile(gFSSpecImmagine);
	
_Uscita:	
	return(gErrore);
} // PresenzaImmagine

/* ------------------------------------------------------------------------ *

	CreaInfoImmagine
	
* ------------------------------------------------------------------------ */
OSErr XTAPI CreaInfoImmagine(uchar *nomeimmagine) throw()
{
	gErrore = PresenzaImmagine(nomeimmagine);
	if (gErrore != noErr) 
	{
		// errore presenza immagine
		ConcatenaAllaStringaErrore(nomeimmagine);
		DaiErrore(kErroreMancaImmagine);
	}
	
	return(gErrore);
} // CreaInfoImmagine


/* ------------------------------------------------------------------------ *

	ImportaImmagine
	
* ------------------------------------------------------------------------ */
static void XTAPI my_positionpict(boxid theboxid, int32 pos) throw();
static void XTAPI my_positionpict(boxid theboxid, int32 pos) throw()
{
	switch (pos) 
	{
		case 1:									// L'immagine viene posizionata nell'angolo in alto a sinistra 
			positionpict(theboxid, PPF_EXACT); 
			break;
		case 2:									// Scala l'immagine secondo la dimensione maggiore dell'immagine 
			positionpict(theboxid, PPF_KEEPSQUARE);
			break;
		case 3:
			positionpict(theboxid, PPF_KEEPSQUARE); 		// Scala l'immagine secondo la dimensione minore dell'immagine 
			break;
		case 4:									// Centra l'immagine nel box
			positionpict(theboxid, PPF_POSITION);
			break;
		default:									// Scala l'immagine secondo la dimensione maggiore dell'immagine
			positionpict(theboxid, PPF_KEEPSQUARE);
			break;
	}
/*
{
	frameinfo 	mioframe;
	Fixed 		mioframeAlt;
	Fixed 		mioframeLarg;
	Rect		picFrame;
	Fixed		picFrameAlt;
	Fixed		picFrameLarg;
	Fixed		scale1;
	Fixed		scale2;
	xtboxptr	pmybox;

	switch (pos) {
		case 1:									// L'immagine viene posizionata nell'angolo in alto a sinistra 
			break;
		case 2:									// Scala l'immagine secondo la dimensione maggiore dell'immagine 
			positionpict(FALSE, TRUE);
			break;
		case 3:									// Scala l'immagine secondo la dimensione minore dell'immagine 
			pmybox = getbox(NULL, theboxid, FALSE);
			mioframe = pmybox->t.frame;
			picFrame = pmybox->p.picFrame;
			picFrameAlt = mFixed(picFrame.bottom) - mFixed(picFrame.top);
			picFrameAlt = FixDiv(picFrameAlt, pmybox->p.auxyscale);			
			picFrameLarg = mFixed(picFrame.right) - mFixed(picFrame.left);
			picFrameLarg = FixDiv(picFrameLarg, pmybox->p.auxxscale);	
			mioframeAlt = mioframe.size.bottom - mioframe.size.top;
			mioframeLarg = mioframe.size.right - mioframe.size.left;
			scale1 = FixDiv(mioframeAlt, picFrameAlt);
			scale2 = FixDiv(mioframeLarg, picFrameLarg);
			if (scale1>scale2) {
				pmybox->p.xscale = scale1;
				pmybox->p.yscale = scale1;
			}
			else {
				pmybox->p.xscale = scale2;
				pmybox->p.yscale = scale2;
			}
			setbox(pmybox, theboxid, FALSE, TRUE);
			disposebox((Ptr)pmybox, TRUE);
			break;
		case 4:									// Centra l'immagine nel box
			positionpict(TRUE, TRUE);
			break;
		default:								// Scala l'immagine secondo la dimensione maggiore dell'immagine
			positionpict(FALSE, TRUE);
			break;
	}
}
*/	
	
} // my_positionpict


/* ------------------------------------------------------------------------ *

	ImportaImmagine
	
* ------------------------------------------------------------------------ */
void XTAPI ImportaImmagine(uchar *nomeimmagine) throw()
{
	// handle per importare l'immagine 
	pathhandle lHndlNomeImmagine = NULL;
	// per costruire il nome dell'immagine
	uchar lNomeImmagine[kDimensioneStringhe] = "";
	
	// imposto comunque lo slug di immagine
	SlugDiImmagine(nomeimmagine);

	// crea il FSSpec dell'immagine 
	if (CreaInfoImmagine(nomeimmagine) != noErr) 
		return;

	// prepara il pathhandle per l'immagine da importare
	gErroreXtension = CreaHandle((Handle *) &lHndlNomeImmagine, (int32) (sizeof(pathrec) + STRLEN(gFSSpecImmagine) + 1));
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nell'allocazione di memoria
		MostraErrore(gErroreXtension);
		return;
	}
									
	STRCPY((*lHndlNomeImmagine)->longpath, gFSSpecImmagine);
	(*lHndlNomeImmagine)->type = 1;
	(*lHndlNomeImmagine)->volnum = 0L;
	(*lHndlNomeImmagine)->dirnum = 0L;

	// prelevamento dell'immagine
	gErrore = xtgettextpict(lHndlNomeImmagine, 0);
	if (gErrore != noErr) 
	{
		// errore di importazione: probabilmente manca l'immagine
		ConcatenaAllaStringaErrore(gFSSpecImmagine);
		DaiErrore(kErroreMancaImmagine);
		return;
	}

  	// modifica del 24 agosto su suggerimento dalla quark
  	// FSClose(fnum); <- penso non serva +

	// per posizionare l'immagine
//	positionpict(kCheckImmagineCentrata, kCheckImmagineProporzionata);
	boxid curbox;
	xtget_curbox(&curbox);
	my_positionpict(curbox, 1L);
	LiberaHandle((Handle *) &lHndlNomeImmagine);

	return;
} // ImportaImmagine

/* ------------------------------------------------------------------------ *

	CreaImmagineCampione

* ------------------------------------------------------------------------ */
errorixtension XTAPI CreaImmagineCampione() throw()
{
	boxid curbox;
	
	// cerco il box immagine campione
	gErroreXtension = TrovaSlugDiImmagineCampione();
	if (gErroreXtension == kNessunErrore) 
	{
		// ho trovato il box immagine campione
		xtget_curbox(&curbox);
		gotobox(curbox);
		return(kNessunErrore);
	}

	// inizializzo le coordinate per il box immagine campione
	ImpostaCoordinateBoxImmagineCampione();

	// creo il box immagine campione
	CreaBoxImmagine();

	// imposto lo slug del box immagine campione
	gErroreXtension = SlugDiImmagineCampione();
	if (gErroreXtension != kNessunErrore) 
	{
		// errore nel dare lo slug
		return(gErroreXtension);
	}

	// vado sul box selezionato
	xtget_curbox(&curbox);
	gotobox(curbox);

	// ritorno il risultato
	return(kNessunErrore);
}

/* ------------------------------------------------------------------------ *

	ControllaPresenzaImmagini
	
* ------------------------------------------------------------------------ */
static errorixtension XTAPI ControllaPresenzaImmagini(uchar** handle) throw()
{
 	// imposto il contatore
	int32 lPosizioneErrore = 0;
	// per prendere il nome dell'Immagine
	uchar lNomeImmagine[32] = "";
	// per determinare la lunghezza del testo da leggere
	Size lContatore = 0;
	// punta all'inizio del testo
	uchar* lPtrInizio = NULL;
	
	// inizializzazione del puntatore per leggere l'handle
	gPtrImm = (uchar*) (*handle);

	while (1) 
	{
		if (*gPtrImm == kAsterisco && *(gPtrImm + 1) == kSpazio) 
		{
			// devo catturare il nome dell'immagine fino al ritorno a capo
			gPtrImm += 2L;
			lPtrInizio = gPtrImm;
			lContatore = 0;
			while (1) 
			{
				if (*gPtrImm == kRitornoACapo) 
					break;
				gPtrImm++;
				lContatore++;
			}
			
			// incremento di uno il contatore
			lContatore++;
			
			// controllo la lunghezza dell'immagine
			if (lContatore > 31) 
			{
				// il nome dell'immagine e' troppo lungo
				return(kErroreLunghezzaImmagine);
			}
			// copio il nome dell'immagine
			BlockMove((Ptr) lPtrInizio,
					  (Ptr) lNomeImmagine,
					  lContatore - 1L);
			// inserisco il carattere '\0' alla fine del testo
			lNomeImmagine[lContatore - 1L] = '\0';

			// controllo se l'immagine e' presente
			gErrore = PresenzaImmagine(lNomeImmagine);
			if (gErrore != noErr) 
			{
				// errore presenza immagine
				ConcatenaAllaStringaErrore(gFSSpecImmagine);
				DaiErrore(kErroreMancaImmagine);
			}
			
		} 
		else if (*gPtrImm == kFineFile) 
			return(kNessunErrore);
		gPtrImm++;
		lPosizioneErrore++;
	}	

	LiberaTrueHandle((Handle)handle);

	return(kNessunErrore);
} // ControllaPresenzaImmagini

/* ------------------------------------------------------------------------ *

	ControllaImmagini
	
* ------------------------------------------------------------------------ */
errorixtension XTAPI ControllaImmagini() throw()
{
	// per prendere gli errori dopo l'errore di impaginazione dell'Xtension
	errorixtension lErroreXtension1 = kNessunErrore;
	// per conoscere se le operazioni sono andate a buon fine
	Boolean lBool = FALSE;
	// per contenere il file dell'elenco delle immagini
	Handle lHndlFile = NULL;
	// nome nuovo per il file di Bertellino
	uchar lNuovoNome[kDimensioneStringhe] = "";
	// per salvare il vecchio nome del file
	uchar lVecchioNome[kDimensioneStringhe] = "";
	uchar lFSSpecFile[kDimensioneStringhe] = "";
	uchar onlynamefile[kDimensioneStringhe] = "";
	
	gErroreXtension = PrendiFileElencoImmagini(lFSSpecFile);
	if (gErroreXtension != kNessunErrore) 
	{
		// si e' verificato un errore nel prendere il file dell'elenco delle immagini
		return(gErroreXtension);
	}
	
	// copio il vecchio nome
	STRCPY(lVecchioNome, lFSSpecFile);
	STRCPY(lNuovoNome, lFSSpecFile);

	gErroreXtension = GenNomeFilenewExt(lNuovoNome, (uchar*)".rim");
	if (gErroreXtension != kNessunErrore) 
	{
		// si e' verificato un errore nel rinominare il file di 4D
		return(kErroreDiSistema);
	}

	gErrore = rename((char*)lFSSpecFile, (char*)lNuovoNome);
	if (gErrore != noErr) 
	{
		// si e' verificato un errore nel rinominare il file dell'elnco immagini
		return(kErroreDiSistema);
	}
	// copio il nuovo nome nella struttura FSSpec
	STRCPY((char *) lFSSpecFile, (char *) lNuovoNome);

	// bisogna eseguire il controllo delle immagini
	// allocca anche la memoria in lHndlFile
	gErroreXtension = LeggiContenutoFSSpecFile(lFSSpecFile, NULL);
	if (gErroreXtension != kNessunErrore) 
	{
		// si e' verificato un errore nel leggere il file
		return(gErroreXtension);
	}

	// il file da controllare e' in memoria 
	ConcatenaAllaStringaMessaggio(lVecchioNome);
	DaiMessaggio(kControllaElencoImmagini);

	// esecuzione del controllo e
	// libera anche la memoria alloccata in lHndlFile
	gErroreXtension = ControllaPresenzaImmagini((uchar**)(gHndlFile));
	if (gErroreXtension != kNessunErrore) 
	{
		// si e' verificato un errore nel controllo delle immagini

		// cerco di spostare il file nella cartella dei temporanei 
		lErroreXtension1 = SpostaFileNellaCartella(lFSSpecFile, gXtPreferences.cartellePref.cartellaTemporanei, &lBool);
		if (lErroreXtension1 != kNessunErrore) 
		{
			// si e' verificato un errore nel spostare il file da controllare
			return(lErroreXtension1);
		}
		
		if (lBool == FALSE) 
		{
			// non sono riuscito a spostare il file, allora lo cancello
			lErroreXtension1 = CancellaFile(lFSSpecFile);
			if (lErroreXtension1 != kNessunErrore) 
			{
				// si e' verificato un errore nel cancellare il file da controllare
				return(lErroreXtension1);
			}
		}

		return(gErroreXtension);
	}

	// cerco di spostare il file nella cartella dei temporanei
	gErroreXtension = SpostaFileNellaCartella(lFSSpecFile, gXtPreferences.cartellePref.cartellaTemporanei, &lBool);
	if (gErroreXtension != kNessunErrore) 
	{
		// si è verificato un errore nel spostare il file impaginato
		return(gErroreXtension);
	}
	if (lBool == FALSE) 
	{
		// non sono riuscito a spostare il file, allora lo cancello
		gErroreXtension = CancellaFile(lFSSpecFile);
		if (gErroreXtension != kNessunErrore) 
		{
			// si è verificato un errore nel cancellare il file da impaginare
			return(gErroreXtension);
		}
	}

	// fine controlla impaginazione 
	DaiMessaggio(kFineControllaElencoImmagini);

	return(kNessunErrore);
} // ControllaImmagini

/* ------------------------------------------------------------------------ *

	ImpaginaImmagini
	
* ------------------------------------------------------------------------ */
void XTAPI ImpaginaImmagini() throw()
{
	// indice per il for
	int16 lPagina = 0;
	// per prendere il numero delle pagini del dcoumento corrente
	int16 lNumeroPagine = 0;
	// nome dell'immagine da importare
	uchar lNomeImmagine[32] = "";
	// identificatore del box su cui eventualmente importare l'immagine
	boxid lIdentificatoreBox = 0;
	// percetuale di scala del documento
	Fixed lPercetuale = 0;
	// lunghezza del nome immagine
	Size lLunghezza = 0;
	
	// prendo il numero delle pagine del documento corrente
	lNumeroPagine = numpages();
	
	// imposto la visualizzazione di impaginazione
	lPercetuale = kVistaControlloImmagini;
	ImpostaScalaDocumento(&lPercetuale);

	for (lPagina = 1; lPagina <= lNumeroPagine; lPagina++) 
	{
		// ciclo sulle pagine del documento

		gotopage(lPagina);
		lIdentificatoreBox = firstbox(lPagina, FALSE);
		// scorro tutti i box della pagina 
		while (1) 
		{
			if (lIdentificatoreBox == NULL) 
				break;
			xtset_curbox(lIdentificatoreBox);

			if (isapicture(lIdentificatoreBox) == TRUE) 
			{
				// cerco di importare l'immagine associata a questo box
				gErroreXtension = PrendiSlugDiImmagine(lNomeImmagine);
				if (gErroreXtension == kNessunErrore) 
				{
					lLunghezza = STRLEN(lNomeImmagine);
					if (lLunghezza > 0) 
						ImportaImmagine(lNomeImmagine);
				}
			}

			lIdentificatoreBox = nextbox(lIdentificatoreBox, FALSE);
		}

	}
	xtset_curbox(NULL);

	// torno alla prima pagina
	gotopage(kPrimaPagina);

	// imposto la scala precedente
	ImpostaScalaDocumento(&lPercetuale);
	
} // ImpaginaImmagini

/* ------------------------------------------------------------------------ *

	ImpostaImmagineCampionePax
	
* ------------------------------------------------------------------------ */
errorixtension XTAPI ImpostaImmagineCampionePax() throw()
{
	boxid ilBoxDiPax = 0L;

	gBoxCampioneDiPax = 0L;

	// cerco il box immagine campione
	gErroreXtension = TrovaSlugDiPax(&ilBoxDiPax);
	if (gErroreXtension == kNessunErrore)
	{
		// non ho trovato il box di pax
		DaiMessaggio(kSimboloPaxNonPresente);		

		return (kNessunErrore);		
	}
	else if (gErroreXtension != kErroreSlugSimboloPaxPresente)
	{
		return ( gErroreXtension );
	}

	// ho trovato il box di pax
	gBoxCampioneDiPax = ilBoxDiPax;
	
	return (kNessunErrore);
} // ImpostaImmagineCampionePax

/* ------------------------------------------------------------------------ *

	PrendiImmagineCampionePax
	
* ------------------------------------------------------------------------ */
boxid XTAPI PrendiImmagineCampionePax() throw()
{
	return(gBoxCampioneDiPax);
} // PrendiImmagineCampionePax


