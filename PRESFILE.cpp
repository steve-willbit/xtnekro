/* ------------------------------------------------------------------------ *

	PresenzaFile.cpp

	8 Aprile 2005							Fabrizio

	Descrizione:
	Questo file contiene le procedure per verificare se all'interno della
	directory scelta durante la configurazione del sistema delle Necrologie
	e' presente il documento di testo esportato da 4D da usare per
	l'impaginazione.
	Quindi e' necessario verificare se all'interno di una directory e' presente
	almeno un file. E' necessario sapere se questo file e' effettivamente un
	file da impaginare. Se si tratta di un file da impaginare, ovviamente si
	passa alla sua impaginazione ed alla sua cancellazione. Se, al contrario,
	non e' un file da impaginare si procede a spostare questo file in un'altra
	cartella.
	
	Copyright © Sinedita S.r.l 2005. All Rights Reserved.
	
	$Log: not supported by cvs2svn $
	Revision 1.4  2005/05/09 09:47:53  taretto
	gestione importazione tutte immagini ok
	
	Revision 1.3  2005/05/04 14:31:20  taretto
	ingombro - immagine campione
	
	Revision 1.2  2005/04/20 13:48:52  taretto
	impaginazione ok
	
	Revision 1.1  2005/04/20 07:14:21  taretto
	importazione files
	

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

#include <cassert>
#include <string>

#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"
#include "DbpResource.h"
#include "DbpFiles.h"

// PROJECT INCLUDES
// PROJECT INCLUDES
// PROJECT INCLUDES

/*
#include "Include.h"
#include "Define.h"
#include "Debug.h"
*/
#include "Memoria.h"
#include "Errori.h"
#include "Pref.h"
#include "ReadFile.h"
#include "ResStr.h"
#include "StMes.h"
#include "XTNecro.h"

#include "PresFile.h"

// CONSTS
// CONSTS
// CONSTS

// cartella finta per la scrivania
#define kNomeScrivania "c:\\Windows"

// STATIC GLOBALS
// STATIC GLOBALS
// STATIC GLOBALS

// per ritornare l'estensione del file da impaginare
static uchar gEstensione[4] = "";

// per ritornare l'estensione del file da impaginare
static uchar gNomeFile[MAXPATHNAME] = "";

// per ritornare il percoso completo
static uchar gPercorsoCompleto[kDimensioneStringhe] = "";

// fnsplit example
static uchar gDrive[kDimensioneStringhe];
static uchar gDir[kDimensioneStringhe];
static int16 gFlags;

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			Estensione
	@abstract 		file
	@discussion		Preleva l'estensione del file da impaginare

					8 Aprile 2005			Fabrizio.

	@param 			nomefile puntatore alla stringa C che contiene il path da cui estrarre l'estensione.
	@result  			torna l'estenzione se esiste NULL altrimenti.
*/
static XTAPI uchar* Estensione(uchar* nomefile) throw();

/*!
	@function			NomeFile
	@abstract 		file
	@discussion		Preleva solo il nome del file.

					8 Aprile 2005			Fabrizio.

	@param 			iFullPath puntatore alla stringa C che contiene il nome del file
	@result  			torna l'estenzione se esiste NULL altrimenti.
*/
static XTAPI uchar *NomeFile(uchar* iFullPath) throw();

/*!
	@function			IsCartella
	@abstract 		cartella
	@discussion		Verifica che il nomecartella  passato sia di una cartella.

					8 Aprile 2005			Fabrizio.

	@param 			nomecartella puntatore al nome della cartella da verificare
	@result  			ritorna il nome del file.
*/
static XTAPI bool8 IsCartella(uchar* nomecartella) throw();

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

/* ------------------------------------------------------------------------ *

	Estensione
	
* ------------------------------------------------------------------------ */
static XTAPI uchar *Estensione(uchar* nomefile) throw()
{
	assert(nomefile);
	
	// pulisco la globale
	STRCPY(gEstensione, "");
	
	// copio in una variabile di tipo string
	std::string s = (char*) nomefile;
	
	// cerco posizione del punto
	int16 pos = s.find('.');
	
	if (pos > 0)
	{
		// carico valore estensione	
		STRCPY(gEstensione, s.substr(pos+1, s.length()).c_str());
	}

	return((uchar*)strupr((char*)gEstensione));
} // Estensione

/* ------------------------------------------------------------------------ *

	NomeFile
	
* ------------------------------------------------------------------------ */
static XTAPI uchar *NomeFile(uchar* iFullPath) throw()
{
	assert(iFullPath);
	
	// pulisco la globale
	STRCPY(gNomeFile, iFullPath);
	
	// prendo lunghezza file
	int16 len = STRLEN(gNomeFile);
	
	// estraggo nome file
	ExtractFileName(gNomeFile);
	STRNCPY(gNomeFile, gNomeFile, (len - 4));

	return((uchar*)strupr((char*)gNomeFile));
} // Estensione

/* ------------------------------------------------------------------------ *

	IsCartella

* ------------------------------------------------------------------------ */
static XTAPI bool8 IsCartella(uchar* nomecartella) throw()
{
	
	assert(NULL != nomecartella);
	assert(0 != nomecartella[0]);
	assert(STRLEN(nomecartella) < MAXPATHNAME);

	// creco di aprire la cartella
	// risultato della funzione	
	bool8 isDir = FALSE; 
	
	int16 refNum = 0;
	
	OSErr error = FSOpen(nomecartella, 0, &refNum);
		
	if (error == bdNamErr)
	{
		isDir = TRUE;
	}
	else
	{
		error = FSClose(refNum);
	}
	
	return(isDir);
} // IsCartella


/* ------------------------------------------------------------------------ *

	PresenzaFile

* ------------------------------------------------------------------------ */
OSErr XTAPI PresenzaFile(uchar *nomefile) throw()
{
	assert(NULL != nomefile);
	assert(0 != nomefile[0]);
	assert(STRLEN(nomefile) < MAXPATHNAME);
	
	// provo ad aprire il file
	int16 refNum;
	OSErr error = FSOpen(nomefile,0, &refNum);
	
	// se non si e' verificato alcun errore ritorno TRUE e chiudo immediatamente il file, in ogni altro caso FALSE
	if (error != fnfErr)
		FSClose(refNum);

	return(error);	
} // PresenzaFile


/* ------------------------------------------------------------------------ *

	PresenzaCartella

* ------------------------------------------------------------------------ */
int32 XTAPI  PresenzaCartella(uchar *nomecartella) throw()
{
	Boolean lRisultato = FALSE;
	
	// per sapere se e' una cartella 
	uchar lNomeCartella[MAXPATHNAME] = "";
	
	STRCPY(lNomeCartella, nomecartella);

	lRisultato = IsCartella(lNomeCartella);
	
	if (lRisultato == FALSE) 
		return(-1);
		
	return(noErr);
} // PresenzaCartella

/* ------------------------------------------------------------------------ *

	SpostaFileSuScrivania

* ------------------------------------------------------------------------ */
errorixtension XTAPI SpostaFileSuScrivania(uchar *nomefile, bool8 *risultato) throw()
{
	assert(nomefile != NULL);
	
	// per creare il nome completo sulla scrivania
	uchar lNuovoNomeFile[MAXPATHNAME] = "";

	(*risultato) = FALSE;

	if (nomefile[0] == 0) {
		// impossibile spostare il file
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
		return(kErroreDelCodice);
	}

	// creo il nome del file sulla scrivania
	STRCPY(lNuovoNomeFile, kNomeScrivania);
	STRCPY(lNuovoNomeFile, "\\");

	// estraggo il nome + estensione del file
	uchar tmpStrFile[MAXPATHNAME] = "";
	STRCPY(tmpStrFile, nomefile);
	ExtractFileName(tmpStrFile);

	// costruisco path
	STRCAT(lNuovoNomeFile, tmpStrFile);
	
	// CancellaFile(lNuovoNomeFile);
	DBP::DeleteFile(lNuovoNomeFile);
	gErrore  = rename((char*)nomefile, (char*)lNuovoNomeFile );
	if (gErrore != noErr) {
		return(kNessunErrore);
	}

	(*risultato) = TRUE;
	return(kNessunErrore);
} // SpostaFileSuScrivania

/* ------------------------------------------------------------------------ *

	SpostaFileNellaCartella
	
* ------------------------------------------------------------------------ */
errorixtension XTAPI SpostaFileNellaCartella(uchar *nomefile, uchar *nomecartella, bool8 *risultato) throw()
{
	assert(nomefile != NULL);
	assert(nomecartella != NULL);
	
	// per creare il nome completo nella nuova posizione
	uchar lNuovoNomeFile[MAXPATHNAME] = "";

	(*risultato) = FALSE;

	if (nomefile[0] == 0) {
		// impossibile spostare il file
		SysBeep(1);
		SysBeep(1);
		SysBeep(1);
		return(kErroreDelCodice);
	}

	// creo il nome del file nella nuova posizione
	STRCPY(lNuovoNomeFile, nomecartella);
	STRCAT(lNuovoNomeFile, "\\");

	// estraggo il nome + estensione del file
	uchar tmpStrFile[MAXPATHNAME] = "";
	STRCPY(tmpStrFile, nomefile);
	ExtractFileName(tmpStrFile);
	
	// costruisco fullpath
	STRCAT(lNuovoNomeFile, tmpStrFile);

	CancellaFile(lNuovoNomeFile);
	gErrore  = rename((char*)nomefile, (char*)lNuovoNomeFile);
	if (gErrore != noErr) 
	{
		return(kNessunErrore);
	}

	(*risultato) = TRUE;
	return(kNessunErrore);
} // SpostaFileNellaCartella

/* ------------------------------------------------------------------------ *

	GenNomeFilenewExt

* ------------------------------------------------------------------------ */
errorixtension XTAPI GenNomeFilenewExt(uchar *name, uchar *ext) throw()
{
	assert(name != NULL);
	
	// variabile d'utilita'
	uchar tmpPath[MAXPATHNAME] = "";
	
	// lunghezza path
	int16 len = STRLEN(name);
	
	// copio il fullpath senza l'estensione
	STRNCPY(tmpPath, name, (len-4));
	
	// concateno nuova estensione
	STRCAT(tmpPath, ext);

	// copio nella variabile d'i/o il risultato
	STRCPY(name, tmpPath);
	
	return(kNessunErrore);
} // GenNomeFilenewExt

/* ------------------------------------------------------------------------ *

	PresenzaFileDaQuattroD

* ------------------------------------------------------------------------ */
errorixtension XTAPI PresenzaFileDaQuattroD(uchar  *name, bool8 *risultato) throw()
{
	// per sapere quale tipo di impaginazione privilegiare
	static tipoimpaginazione lProssimaImpaginazione = kImpaginazioneBozza;
	
	// per sapere quale tipo di impaginazione ha il file corrente
	tipoimpaginazione lFileCorrente  = kNessunaImpaginazione;
	
	// per sapere quale tipo di impaginazione ha il file scelto
	tipoimpaginazione lFileDaImpaginare = kNessunaImpaginazione;
	
	// per sapere se ci sono file nella cartella dei file da impaginare
	int16 lFileTrovati = 0;

	// indice del file da analizzare
	int16 lIniziaDa = 1;
	
	// lunghezza del file aperto
	int32 lLunghezza = 0;
	
	// per caricare l'estensione del file da impaginare
	uchar lEstensione[4] = "";
	
	// per sapere se e' stato spsotato correttamente il file
	bool8 lFileSpostato = TRUE;
	
	uchar lNomeCartella[MAXPATHNAME] = "";
	uchar lNomeFileDaImpaginare[MAXPATHNAME];
	// struct ffblk	myffblk; <- struttura borland obsoleta
	
	int16 lIdentificatoreFile;

	// per verificare se è una cartella
	Boolean lBool = TRUE;

	(*risultato) = FALSE;

	// per prelevare FSSpec della cartella dei file da impaginare
	lBool = IsCartella(gXtPreferences.datiPref.folderDati); // ex kStrCartellaDati
	if (lBool != TRUE) 
	{
		// la cartella dei file da impaginare non e' presente
		ConcatenaAllaStringaErrore(gXtPreferences.datiPref.folderDati);
		return(kErrorePresenzaCartella);
	}

	// ciclo di scansione dei file presenti nella cartella
	// alla fine in lFSSpecFileDaImpaginare c'e'  il file da impaginare
	STRCPY( lNomeCartella, gXtPreferences.datiPref.folderDati );
	
	// gErrore = findfirst(lNomeCartella,&myffblk,0); <- vecchia chiamata borland, ora obsoleta
	std::string inputFolderString = (char*)lNomeCartella;
	std::string fileTypeString = "*";
	std::string fileFoundName;
	bool8 found;
	int32 numFileFound;
	// controllo se trovo files con una delle estensioni passate in input da preferenze
	fileTypeString = (char*)(gXtPreferences.datiPref.estensione1);
	DBP::FindFirstFile(inputFolderString, fileTypeString, fileFoundName, found, numFileFound);
	if (!found)
	{
		fileTypeString = (char*)(gXtPreferences.datiPref.estensione2);
		DBP::FindFirstFile(inputFolderString, fileTypeString, fileFoundName, found, numFileFound);
		if (!found)
		{
			fileTypeString = (char*)(gXtPreferences.datiPref.estensione3);
			DBP::FindFirstFile(inputFolderString, fileTypeString, fileFoundName, found, numFileFound);
		}
	}
		
	// concateno separatore al nome cartella
	STRCAT( lNomeCartella, "\\*.*");

	if (gErrore != noErr)
	{
		(*risultato) = FALSE;
		return(kNessunErrore);
	}
/*	
	if (!found) // aggiunta by Fabry 19/04/2005
	{
		// non c'e' nessun documento nella cartella dei file da impaginare
		(*risultato) = FALSE;
		return(kNessunErrore);
	}
*/
//	while (1)
//	{
		lFileCorrente  = kNessunaImpaginazione;
		
		// leggo l'estenzione del file da impaginare
		STRCPY(lEstensione, Estensione((uchar*)fileFoundName.c_str()));
		
		// controllo se e' un file con l'estensioni ammesse
		if (gXtPreferences.datiPref.impaginaDocEstensione1 == TRUE &&
			STRCMP(lEstensione, gXtPreferences.datiPref.estensione1) == 0)
		{
			lFileCorrente = kImpaginazioneBozza;
		} 
		else if (gXtPreferences.datiPref.impaginaDocEstensione2 == TRUE &&
			STRCMP(lEstensione, gXtPreferences.datiPref.estensione2) == 0) 
		{
			lFileCorrente = kImpaginazioneIngombro;
		} 
		else if (gXtPreferences.datiPref.impaginaDocEstensione3 == TRUE &&
			STRCMP(lEstensione, gXtPreferences.datiPref.estensione3) == 0) 
		{
			lFileCorrente = kImpaginazioneDefinitiva;
		}

		if (lFileCorrente != kNessunaImpaginazione) 
		{
			// e' un file impaginabile
			if (lFileDaImpaginare == kNessunaImpaginazione) 
			{
				// non ho ancora scelto alcun file da impaginare
				// quindi prendo il corrente
				wsprintf((char*)lNomeFileDaImpaginare, "%s\\%s", gXtPreferences.datiPref.folderDati, fileFoundName.c_str());
				lFileDaImpaginare = lFileCorrente;
			} 
			else 
			{
				// ho gi… svelto un file da impaginare
				// controllo se questo tipo di impaginazione e l'impaginazione che
				// mi aspetto
				if (lProssimaImpaginazione == lFileCorrente) 
				{
					wsprintf((char*)lNomeFileDaImpaginare, "%s\\%s", gXtPreferences.datiPref.folderDati, fileFoundName.c_str());
					lFileDaImpaginare = lFileCorrente;
				}
			}
		}
		else
		{
			// non ho trovato nessun file impaginabile
			found = FALSE;
		}
		
		// gErrore = findnext(&myffblk); <- obsoleta
//		DBP::FindFirstFile(inputFolderString, fileTypeString, fileFoundName, found, numFileFound);
		
		// se non trovo piu' alcun file esco dal ciclo
//		if (!found)
//			break;
//	}
	
	
	if (!found) 
	{
		// non c'e' nessun documento nella cartella dei file da impaginare
		(*risultato) = FALSE;
		return(kNessunErrore);
	}

	// verifico che il file non sia gi… aperto in scrittura
	// OSErr err = _lopen((char*)lNomeFileDaImpaginare, OF_SHARE_EXCLUSIVE);
	OSErr err = FSOpen(lNomeFileDaImpaginare, 0, &lIdentificatoreFile);
	if (lIdentificatoreFile == HFILE_ERROR || err != noErr) 
	{
		// significa che non e' possibile ancora leggere il file
		(*risultato) = FALSE;
		return(kNessunErrore);
	}

	// leggo la lunghezza del file
	err = GetEOF(lIdentificatoreFile, &lLunghezza); // _filelength(lIdentificatoreFile);

	if (err != noErr) // lLunghezza == -1
	{
		// significa che non e' possibile ancora leggere il file
		(*risultato) = FALSE;
		return(kNessunErrore);
	}

	if (lLunghezza == 0)  
	{
		// significa che non e' possibile ancora leggere il file 
		(*risultato) = FALSE;
		return(kNessunErrore);
	}

	// lIdentificatoreFile = _lclose(lIdentificatoreFile);
	err = FSClose(lIdentificatoreFile);
	if ( err !=  noErr) 
	{
		// significa che non Š possibile ancora leggere il file
		(*risultato) = FALSE;
		return(kNessunErrore);
	}
// 	FlushVol(NULL, 0);

#if 0 // OLDESTVERSION

	// ho trovato un file devo verificare se e' di testo
	gErrore = FSpGetFInfo(&lFSSpecFileDaImpaginare, &lFileInfo);
	if (gErrore != noErr) {
		// significa che non e' possibile ancora leggere il file
		StampaDebug("FSpGetFInfo", (long) gErrore);
		(*risultato) = FALSE;
		return(kNessunErrore);
	}

	if (lFileInfo.ftype != TEXT) {
		/* non e' un file di testo */
		/* sposto il documento da questa directory in modo */
		/* da non averlo pi— tra i  piedi */

#if kNEW

		// sposto il file nella cartella dei temporanei
		gErroreXtension = SpostaFileNellaCartella(lFSSpecFileDaImpaginare,
																kStrCartellaTmp,
																&lFileSpostato);
		if (gErroreXtension != kNessunErrore) {
			// errore nello spostare il file
			StampaDebug("SpostaFileNellaCartella", (long) gErroreXtension);
			return(gErroreXtension);
		}

		if (lFileSpostato ==  FALSE) {
			// nel caso in cui non si riesca a spostare il file lo cancello
			gErroreXtension = CancellaFile(lFSSpecFileDaImpaginare);
			if (gErroreXtension != kNessunErrore) {
				// errore nel cancellare il file
				StampaDebug("CancellaFile", (long) gErroreXtension);
				return(gErroreXtension);
			}
		}

#else

		// sposto il file sulla scrivania 
		gErroreXtension = SpostaFileSuScrivania(lFSSpecFileDaImpaginare, &lFileSpostato);
		if (gErroreXtension != kNessunErrore) {
			// errore nello spostare il file
			StampaDebug("SpostaFileSuScrivania", (long) gErroreXtension);
			return(gErroreXtension);
		}

		if (lFileSpostato ==  FALSE) {
			// nel caso in cui non si riesca a spostare il file lo cancello
			gErroreXtension = CancellaFile(lFSSpecFileDaImpaginare);
			if (gErroreXtension != kNessunErrore) {
				// errore nel cancellare il file
				StampaDebug("CancellaFile", (long) gErroreXtension);
				return(gErroreXtension);
			}
		}

#endif /* kNEW */

		(*risultato) = FALSE;
		return(kNessunErrore);
	}

#endif // 0 OLDESTVERSION

	// il file trovato e' un file di testo
	if (STRLEN(lNomeFileDaImpaginare)) 
	{
		// verificare se bisogna comporre tutto il path del file da impagimare
		STRCPY( name, lNomeFileDaImpaginare );
		(*risultato) = TRUE;

		// aggiorno la variabile con la prossima impaginazione
		if (lProssimaImpaginazione == kImpaginazioneBozza) 
			lProssimaImpaginazione = kImpaginazioneIngombro;
		else if (lProssimaImpaginazione == kImpaginazioneIngombro) 
			lProssimaImpaginazione = kImpaginazioneDefinitiva;
		else 
		lProssimaImpaginazione = kImpaginazioneBozza;

		return(kNessunErrore);

	} else 
	{
		SysBeep(1);
		return(kErroreDelCodice);
	}
} // PresenzaFileDaQuattroD

/* ------------------------------------------------------------------------ *

	PrendiFileElencoImmagini

* ------------------------------------------------------------------------ */
errorixtension XTAPI PrendiFileElencoImmagini(uchar *name) throw()
{
	// identificatore del file da leggere
	HFILE lIdentificatoreFile = 0;
	// lunghezza del file aperto
	int32 lLunghezza = 0;
	// per caricare l'estensione del file da impaginare
	uchar lEstensione[4] = "";
	
	uchar lNomeCartella[kDimensioneStringhe] = "";
	uchar lNomeFileDaControllare[kDimensioneStringhe];


	// per verificare se è una cartella
	Boolean lBool = TRUE;
	
	// per prelevare FSSpec della cartella dei file da impaginare
	lBool = IsCartella(gXtPreferences.datiPref.folderDati);
	if (lBool != TRUE) 
	{
		// la cartella dei file da impaginare non e' presente
		ConcatenaAllaStringaErrore(gXtPreferences.datiPref.folderDati);
		return(kErrorePresenzaCartella);
	}

	// ciclo di scansione dei file presenti nella cartella
	// alla fine in lFSSpecFileDaImpaginare c'e' il file da impaginare

	STRCPY( lNomeCartella, gXtPreferences.datiPref.folderDati);
	STRCAT( lNomeCartella, "\\*.*");

	// gErrore = findfirst(lNomeCartella,&myffblk,0);
	std::string inputFolderString = (char*)lNomeCartella;
	std::string fileTypeString = "*";
	std::string fileFoundName;
	bool8 found;
	int32 numFileFound;
	DBP::FindFirstFile(inputFolderString, fileTypeString, fileFoundName, found, numFileFound);
	
	if (gErrore != noErr)
	{
		return(kNessunErrore);
	}

//	while (!gErrore)
	{
		// leggo l'estenzione del file da impaginare
		STRCPY(lEstensione, Estensione((uchar*)fileFoundName.c_str()));
		
		// controllo se e' un file con l'estensione ammessa 
		if (STRCMP(&lEstensione[1], gXtPreferences.immaginiPref.estensioneElenco) == 0) 
		{
			wsprintf((char*)lNomeFileDaControllare,"%s\\%s", gXtPreferences.datiPref.folderDati, fileFoundName.c_str() );
		}
		// gErrore = findnext(&myffblk);
	}
	
	if (lNomeFileDaControllare[0] == 0) 
	{
		// non c'e' nessun documento nella cartella dei file da impaginare
		return(kNessunFileElencoImmagini);
	}

	// verifico che il file non sia gi… aperto in scrittura
	lIdentificatoreFile = _lopen((char*)lNomeFileDaControllare, OF_SHARE_EXCLUSIVE);
	if (lIdentificatoreFile == HFILE_ERROR) 
	{
		// significa che non e' possibile ancora leggere il file
		return(kFileElencoImmaginiNonUsabile);
	}

	// leggo la lunghezza del file
	lLunghezza = filelength(lIdentificatoreFile);
	if (lLunghezza == -1) 
	{
		// significa che non e' possibile ancora leggere il file 
		return(kNessunErrore);
	}


	if (lLunghezza == 0)  
	{
		// significa che non e' possibile ancora leggere il file
		return(kNessunErrore);
	}

	lIdentificatoreFile = _lclose(lIdentificatoreFile);
	if (lIdentificatoreFile == HFILE_ERROR) 
	{
		// significa che non e' possibile ancora leggere il file
		return(kFileElencoImmaginiNonUsabile);
	}
	FlushVol(NULL, 0);


	if (STRLEN(lNomeFileDaControllare)) 
	{
		STRCPY( name, lNomeFileDaControllare );
		return(kNessunErrore);
	} 
	else 
	{
		return(kErroreDelCodice);
	}
} // PrendiFileElencoImmagini


#pragma mark -
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

/* Costanti */
/* Costanti */
/* Costanti */

/* Variabili */
/* Variabili */
/* Variabili */


/*
	per utilizzi futuri
		 printf("Command processor info:\n");
	 if(flags & DRIVE)
		 printf("\tdrive: %s\n",drive);
	 if(flags & DIRECTORY)
		 printf("\tdirectory: %s\n",dir);
	 if(flags & FILENAME)
		 printf("\tfile: %s\n",file);
*/

/* ------------------------------------------------------------------------ *

	PercorsoCompleto

* ------------------------------------------------------------------------ */
uchar* XTAPI PercorsoCompleto(uchar* percorsoparziale) throw()
{
	// percorso completo della cartella di Xpress
	Str255 lStrVolume = "";
	// numero volume precedente
	int16 lVolumeVecchio = 0;
	// numero volume
	int16 lVolume = 0;
	int16 iddrive;

	// inizializzo il percorso completo
	STRCPY(gPercorsoCompleto, "");
	
	// cerco il nome completo della cartella di del disco di default
	gFlags=fnsplit(percorsoparziale,gDrive,gDir,gFile,gExt);
	if(gFlags & DRIVE )  
		return(percorsoparziale);
	else
	{
		iddrive = getdisk();
		gPercorsoCompleto[0] = 'A'+iddrive;
		gPercorsoCompleto[1] = '\0';
		STRCAT(gPercorsoCompleto, gDir);
		STRCAT(gPercorsoCompleto, gFile );
		STRCAT(gPercorsoCompleto, gExt );
	}
	return(gPercorsoCompleto);
} // PercorsoCompleto

void GetNomeFile(char *nomefile, char *newname);
void GetNomeFile(char *nomefile, char *newname)
/* ------------------------------------------------------------------------ *

	Estensione

	18 Ottobre 1995						Stefano

	Descrizione:
	Preleva l'estensione del file da impaginare

	Parametri:
	nomefile - puntatore alla stringa C che contiene il nome della cartella

	Ritorno:
	ritorna l'estenzione se esiste NULL altrimenti

* ------------------------------------------------------------------------ */
{

	strcpy(newname, "");
	gFlags=fnsplit(nomefile,gDrive,gDir,gFile,gExt);
	strcpy(newname, gFile);
	strcat(newname, gExt);
}


errorixtension CancellaTutto(char *nomecartella)
/* ------------------------------------------------------------------------ *

	CancellaTutto

	23 Novembre 1995						Stefano

	Descrizione:
	Serve a cancellare tutti i file presenti nella cartella indicata come
	parametro

	Parametri:
	nomecartella - puntatore alla stringa C che contiene la cartella in cui
	cancellare

	Ritorno:
	ritorna l'eventuale errore dell'xtension

* ------------------------------------------------------------------------ */

{
//	long lIdentificatoreCartella = 0;
	/* identificatore della cartella */
//	Boolean lIsCartella = TRUE;
	/* serve per sapere se la cartella  Š veramente una cartella */
//	FSSpec lFSSpecCartella = {0, 0, "\p"};
	/* per leggere le info della cartella dei file da impaginare */
	char lNomeCartella[kDimensioneStringhe] = "";
	struct ffblk	myffblk;

	strcpy( lNomeCartella, nomecartella );
	strcat( lNomeCartella, "\\*.*");

	gErrore = findfirst(lNomeCartella,&myffblk,0);
	if (gErrore != noErr)
	{
		return(kNessunErrore);
	}
	while (!gErrore)
	{
		sprintf( lNomeCartella, "%s\\%s", nomecartella, myffblk.ff_name );
		if( remove( lNomeCartella ))
		{
			StampaDebug("DeleteDirectoryContents", (long) gErrore);
			ConcatenaAllaStringaErrore(nomecartella);
			return(kErroreCancellaFile);
		}
		gErrore = findnext(&myffblk);
	}
	return(kNessunErrore);
}

errorixtension PresenzaFileDaAppWare(char *name, Boolean *risultato)
/* ------------------------------------------------------------------------ *

	PresenzaFileDaAppWare

	11 Marzo 1996						Stefano

	Descrizione:
	Cerca di verificare se nella cartella dei dati da AppWare
	c'Š un file. Se non c'Š nessun file ritorna FALSE.
	Se Š un file impaginabile ritorna TRUE impostando il puntatore passato
	per permetterne l'apertura e poi la cancellazione.

	Parametri:
	fsspecfile - puntatore al FSSpec del file da impaginare
	risultato - ritorna TRUE se Š stato trovato un file da impaginare.
	Ritorna FALSE in tutti gli altri casi

	Ritorno:
	ritorna l'eventuale errore dell'xtension

* ------------------------------------------------------------------------ */

{
//	FSSpec lFSSpecCartella = {0, 0, "\p"};
	/* per leggere le info della cartella dei file da AppWare */
//	FSSpec lFSSpecFile = {0, 0, "\p"};
	/* per leggere le info del file da impaginare */
//	short lFileTrovati = 0;
	/* per sapere se ci sono file nella cartella dei file da impaginare */
//	short lIniziaDa = 1;
	/* indice del file da analizzare */
	HFILE lIdentificatoreFile = 0;
	/* identificatore del file da leggere */
	long lLunghezza = 0;
	/* lunghezza del file aperto */
//	FInfo lFileInfo = {0,0, 0, {0,0}, 0};
	/* per leggere le info del file */
//	Boolean lFileSpostato = TRUE;
	/* per sapere se Š stato spsotato correttamente il file */
	char lNomeCartella[kDimensioneStringhe] = "";
	char lNomeFileDaAppWare[kDimensioneStringhe];
	struct ffblk	myffblk;
	int done;

	Boolean lBool = TRUE;
	/* per verificare se è una cartella */

	/* per prelevare FSSpec della cartella dei file da AppWare */
	lBool = IsCartella(PercorsoCompleto(kStrCartellaDaAppWare));
	if (lBool != TRUE) {
		/* la cartella dei file da AppWare non Š presente */
		StampaDebug("Non esiste la cartella 'da AppWare'", (long) gErrore);
		ConcatenaAllaStringaErrore(PercorsoCompleto(kStrCartellaDaAppWare));
		return(kErrorePresenzaCartella);
	}

	strcpy( lNomeCartella, kStrCartellaDaAppWare );
	strcat( lNomeCartella, "\\*.*");

	gErrore = findfirst(lNomeCartella,&myffblk,0);
	if (gErrore != noErr)
	{
		return(kNessunErrore);
	}

	if(myffblk.ff_name[0] == 0){
		/* non c'Š nessun documento nella cartella dei file da AppWare */
		(*risultato) = FALSE;
		return(kNessunErrore);
	}
	wsprintf( lNomeFileDaAppWare, "%s\\%s", kStrCartellaDaAppWare, myffblk.ff_name );
//	strcpy( lNomeFileDaAppWare, myffblk.ff_name );
	/* verifico che il file non sia gi… aperto in scrittura */
	lIdentificatoreFile = _lopen(lNomeFileDaAppWare, OF_SHARE_EXCLUSIVE);
	if (lIdentificatoreFile == HFILE_ERROR) {
		/* significa che non Š possibile ancora leggere il file */
		StampaDebug("_lopen", (long) lIdentificatoreFile);
		return(kNessunErrore);
	}

//	lIdentificatoreFile = fileno(check);

	/* leggo la lunghezza del file */
	lLunghezza = filelength(lIdentificatoreFile);
	if (lLunghezza == -1) {
		/* significa che non Š possibile ancora leggere il file */
		StampaDebug("filelength", (long)0);
		return(kNessunErrore);
	}


	if (lLunghezza == 0)  {
		/* significa che non Š possibile ancora leggere il file */
		StampaDebug("lLunghezza == 0", (long) gErrore);
		return(kNessunErrore);
	}

	lIdentificatoreFile = _lclose(lIdentificatoreFile);
	if (lIdentificatoreFile == HFILE_ERROR) {
		/* significa che non Š possibile ancora leggere il file */
		StampaDebug("_lclose", (long) lIdentificatoreFile);
		return(kNessunErrore);
	}
//	FlushVol(NULL, 0);


	/* il file trovato Š un file di testo */
	if (strlen(lNomeFileDaAppWare)) {
//		(*fsspecfile) = lFSSpecFileDaImpaginare;
		/* verificare se bisogna comporre tutto il path del file da impagimare */
		strcpy( name, lNomeFileDaAppWare );
		(*risultato) = TRUE;
		return(kNessunErrore);
	} else {
		StampaDebug("Errore parametri", 0);
		SysBeep(1);
		SysBeep(1);
		return(kErroreDelCodice);
	}
}

#endif // NECRO v1
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
