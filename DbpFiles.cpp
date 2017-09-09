/* ------------------------------------------------------------------------ *

	DbpFiles.cpp
	
	Copyright © Sinedita S.r.l 2003. All Rights Reserved.

	$Log: not supported by cvs2svn $
	Revision 1.45  2007/05/23 09:43:42  taretto
	modificata DBP::CopyFiles
	
	Revision 1.44  2007/05/21 16:19:54  taretto
	copyfile funzionante
	
	Revision 1.43  2007/05/18 15:32:39  taretto
	compilazione QXP6/7
	
	Revision 1.42  2006/10/11 13:01:11  taretto
	 corretto salvataggio preferenze su QXP60
	
	Revision 1.41  2006/10/06 14:06:25  taretto
	aggiunte funzioni varie
	
	Revision 1.40  2004/10/06 15:40:43  marchese
	corretto un conflitto su un commento
	
	Revision 1.39  2004/05/13 07:09:24  taretto
	eliminata la direttiva di precompilazione #error
	
	Revision 1.38  2004/03/23 15:12:32  marchese
	inserito il cast (const uchar*) per evitare una ricorsione infinita
	
	Revision 1.37  2004/03/23 08:56:30  marchese
	tolti dei #if WINOS inutili
	
	Revision 1.36  2004/03/23 08:01:36  taretto
	prima compilazione sclass su win con codice unico
	
	Revision 1.35  2004/03/22 16:43:57  taretto
	porting sclass macos
	
	Revision 1.34  2003/10/23 10:12:31  marchese
	spostati degli #if QXP40 e QXP50
	
	Revision 1.33  2003/10/13 10:29:53  marchese
	tolto qualche spazio di troppo
	
	Revision 1.32  2003/07/24 15:57:10  marchese
	aggiunte le assert per controllare la lunghezza dei file
	
	Revision 1.31  2003/07/18 07:40:52  marchese
	aggiunto codice WINOS nelle funzioone FindFirstFolder() e FindFirstFile()
	
	Revision 1.30  2003/07/15 08:22:46  marchese
	prova di riscrittura delle funzione FindFirst* usando il codice nativo WINOS
	
	Revision 1.29  2003/07/11 12:35:40  taretto
	compilazione XTRPDAds
	
	Revision 1.28  2003/07/01 16:05:25  marchese
	nella funzione FindFirstFile() ho aggiunto un parametro per specificare l'indice del file da leggere
	
	Revision 1.27  2003/07/01 12:14:44  marchese
	aggiunto un assert nella funzione FindFirstFile() per il controllo della lunghezza del campo con l'estensione
	
	Revision 1.26  2003/07/01 08:14:33  marchese
	aggiunta la macro STRCONST al posto del cast (uchar*)
	
	Revision 1.25  2003/06/20 09:53:08  taretto
	correnti conflitti
	
	Revision 1.24  2003/06/20 09:41:21  taretto
	aggiunte funzioni per ricavare il path completo
	
	Revision 1.23  2003/06/20 08:53:52  marchese
	cambiato il ritorno delle funzioni GetFullPathName, da void a bool8
	
	Revision 1.22  2003/06/19 11:25:58  marchese
	aggiunto il prototipo della funzione GetFullPathName()
	
	Revision 1.21  2003/06/19 09:39:38  marchese
	- cambiato il nome alle funzione di ricerca dei file e dei folder
	- adesso la FindFirstFolder() ritorna la prima cartella valida trovata
	
	Revision 1.20  2003/06/19 08:25:37  taretto
	aggiunta fuinzione per verificare la presenza di un file
	
	Revision 1.19  2003/06/17 16:35:43  marchese
	la FindFirst() e la FindFisrtSubFolder() ricevono parametri di tipo std::string
	
	Revision 1.18  2003/06/17 14:32:49  marchese
	aggiunto un return(TRUE)
	
	Revision 1.17  2003/06/17 14:28:24  marchese
	uniformato il risultato di ritorno delle funzioni sui file: sempre TRUE/FALSE
	
	Revision 1.16  2003/06/17 14:13:04  marchese
	creato il prototipo della funzione IsFile()
	
	Revision 1.15  2003/06/16 16:49:02  marchese
	copia di alcune funzioni con parametri std::string
	
	Revision 1.14  2003/06/06 10:16:37  marchese
	modificata l'implementazione delle funzione CreateHtmlFile
	
	Revision 1.13  2003/06/06 09:45:16  taretto
	aggiunta funzione per creazione file html
	
	Revision 1.12  2003/06/06 07:53:15  marchese
	aggiunte le assert nella funzione CreateHtmlFile
	
	Revision 1.11  2003/06/06 07:47:37  marchese
	aggiunta la dichiarazione della funzione CreateHtmlFile
	
	Revision 1.10  2003/06/04 13:10:33  taretto
	modificata funzione FindFirstFolder
	
	Revision 1.9  2003/05/07 07:16:22  marchese
	spostati gli standard includes nei file .h
	
	Revision 1.8  2003/04/18 12:04:15  taretto
	tolto commento inutile
	
	Revision 1.7  2003/04/17 14:39:25  taretto
	cambiati alcuni commenti
	
	Revision 1.6  2003/04/17 12:56:49  taretto
	apportate alcune modifiche, aggiunte assert
	
	Revision 1.5  2003/04/16 12:11:45  taretto
	archiviazione provvisoria
	
	Revision 1.4  2003/04/08 08:08:34  taretto
	da verificare la funzione copy
	
	Revision 1.3  2003/04/04 09:11:53  taretto
	da aggiungere assert
	
	Revision 1.2  2003/03/31 10:03:38  marchese
	piccole correzioni sui file
	
	Revision 1.1  2003/03/31 09:48:03  taretto
	HdrsLibs per le operazioni sui file
	
* ------------------------------------------------------------------------ */

// CONFIG INCLUDES
// CONFIG INCLUDES
// CONFIG INCLUDES

// always the first include, this is include must be present in all the xtensions

#include "XTConfig.h"
#include "QXPConfig.h"

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpAssert.h"
#include "DbpPascalString.h"
#include "DbpFiles.h"

#if QXP40
// macro che contiene la definizione delle variabili globali di XPress
EXTERN_XT_GLOBALS
#endif // QXP40

// DEFINES
// DEFINES
// DEFINES

#if WINOS	
	#define INTSIZE int32
#endif // WINOS	

#if MACOS 
	#define INTSIZE int16
#endif //MACOS	

// FUNCTIONS
// FUNCTIONS
// FUNCTIONS

#if QXP60 || QCD35

/* ------------------------------------------------------------------------ *

	DBP::IsFile

* ------------------------------------------------------------------------- */
bool8 XTAPI DBP::IsFile(const uchar* iFilePath) throw()
{
	assert (NULL != iFilePath);
	assert (0 != iFilePath[0]);
	
	bool8 ret = FALSE;
	
	// apertura del documento
	uchar p[MAXPATHNAME] = "";
	PSTRCPY(p, iFilePath);
	FSSpec f;
	int16 lFileReference = 0;
	
	OSErr makeFSSpec = FSMakeFSSpec(0, 0, p, &f);
	
	if (!makeFSSpec)
	{
		OSErr openError = FSpOpenDF(&f, fsRdWrPerm, &lFileReference);
		
		if (openError==noErr || openError==opWrErr || openError==permErr)
			ret = TRUE;
		
		if (openError == noErr)
			FSClose(lFileReference);
	}
	else
	{
		ret = FALSE;
	}
	return(ret);
	
} // DBP::IsFile

#endif // QXP60 || QCD35

#if QXP40 || QXP50
/* ------------------------------------------------------------------------ *

	DBP::IsFile
	
* ------------------------------------------------------------------------ */
bool8 XTAPI DBP::IsFile(const uchar* const iFilePath) throw()
{
	assert(NULL != iFilePath);
	assert(0 != iFilePath[0]);
	assert(STRLEN(iFilePath) < MAXPATHNAME);

	// risultato della funzione	
	bool8 isFile = FALSE;

	// provo ad aprire il file
	int16 refNum;
	OSErr error =
		FSOpenPerm
		(
			iFilePath,
			0, // vRefNum
			&refNum,
			1 // (1=read, 2=write, etc)
		);
	
	// se non si e' verificato alcun errore ritorno TRUE e chiudo immediatamente il file,
	// in ogni altro caso FALSE
	if (error != fnfErr)
	{
		FSClose(refNum);
		isFile = TRUE;
	}

	return(isFile);	
} // DBP::IsFile
#endif // QXP40 || QXP50

#if QXP40 || QXP50
/* ------------------------------------------------------------------------ *

	DBP::IsFile
	
* ------------------------------------------------------------------------ */
bool8 XTAPI DBP::IsFile(const std::string& iFilePath) throw()
{
	assert(!iFilePath.empty());
	assert(iFilePath.length() < MAXPATHNAME);

	return(DBP::IsFile((const uchar*) iFilePath.c_str()));
} // DBP::IsFile
#endif // QXP40 || QXP50

#if QXP40 || QXP50	
/* ------------------------------------------------------------------------ *

	DBP::IsDir
	
* ------------------------------------------------------------------------ */
bool8 XTAPI DBP::IsDir(const uchar* const iFolder) throw()
{
	assert(NULL != iFolder);
	assert(0 != iFolder[0]);
	assert(STRLEN(iFolder) < MAXPATHNAME);

	// risultato della funzione	
	bool8 isDir = FALSE; 

#if MACOS
	FSSpec fsspecFolder = {0, 0, NULL};
	FSMakeFSSpec(0, 0, iFolder, &fsspecFolder);
	
	int16			n;
	CInfoPBRec		pb;
	n = STRLEN(iFolder);
	if (n > 0)
	{
		pb.dirInfo.ioNamePtr = (uchar*)iFolder;
		pb.dirInfo.ioVRefNum = 0;
		pb.dirInfo.ioFDirIndex = 0;
		
		if (PBGetCatInfo(&pb, FALSE))
		{
			isDir = FALSE;
		}
		else if (!(pb.dirInfo.ioFlAttrib & 16))
		{
			isDir = FALSE;
		}
		else
		{
			isDir = TRUE;
		}
	}
	else
	{
		// non esiste comunque	
		isDir = FALSE;
	}	
#endif // MACOS


#if WINOS
	// cerco di aprire la cartella
	int16 refNum;
	OSErr error = FSOpenPerm(iFolder, 0, &refNum, 1);
	if (error == bdNamErr)
	{
		isDir = TRUE;
	}
	else
	{
		error = FSClose(refNum);
	}
#endif // WINOS
	
	return(isDir);
} // DBP::IsDir
#endif // QXP40 || QXP50 

#if QXP40 || QXP50	
/* ------------------------------------------------------------------------ *

	DBP::IsDir
	
* ------------------------------------------------------------------------ */
bool8 XTAPI DBP::IsDir(const std::string& iFolder) throw()
{
	assert(!iFolder.empty());
	assert(iFolder.length() < MAXPATHNAME);

	return(DBP::IsDir((const uchar*) iFolder.c_str()));
} // DBP::IsDir
#endif // QXP40 || QXP50

/* ------------------------------------------------------------------------ *

	DBP::FindFirstFolder
	
* ------------------------------------------------------------------------ */
void XTAPI DBP::FindFirstFolder(const std::string& iInputFolder, std::string& oSubFolder,
	bool8& oDirFound, int32& oNumOfDir, const std::string& iCharsToSkip) throw()
{
	assert(!iInputFolder.empty());
	assert(iInputFolder.length() < MAXPATHNAME);

#if MACOS
	// versione MACOS ancora da realizzare
	assert(0);
#endif // MACOS
	
#if WINOS
#if 1
	// risultato della ricerca
	oSubFolder.clear();
	oDirFound = FALSE;
	oNumOfDir = 0;
	
	// costruisco il nome della cartella di ricerca
	std::string inputFolder(iInputFolder);
	inputFolder.append("\\*");

	// avvio la ricerca
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
	hFind = FindFirstFile(inputFolder.c_str(), &findFileData);
	if (INVALID_HANDLE_VALUE != hFind) 
	{
		// ai caratteri da skippare aggiungo anche il punto
		std::string charsToSkip(iCharsToSkip);
		charsToSkip.append(".");
		
		// ciclo per contare i folder contenuti nel folder
		// e per prenderne il primo trovato
		while (1)
		{
			// controllo che il file caricato sia una cartella
			// e che non contenga i caratteri da skippare
			std::string folderName(findFileData.cFileName);
			if ((FILE_ATTRIBUTE_DIRECTORY & findFileData.dwFileAttributes) // e' una cartella
				&& std::string::npos == charsToSkip.find(folderName[0])) // non contiene i caratteri da skippare
			{
				if (!oDirFound)
				{
					// memorizzo i dati della prima cartella ammissibile trovata
					oSubFolder = folderName;
					oDirFound = TRUE;
				}
				
				// conto le cartelle
				++oNumOfDir;
			}

			// prendo il file successivo
			bool8 ret = FindNextFile(hFind, &findFileData);
			if (!ret)
			{
				break;
			}
			
		}
	}
	
    FindClose(hFind);

#else // 1
	
	// handle alla directory in cui effettuo la ricerca
	Handle dirPath = NewHandle(sizeof(INTSIZE) + iInputFolder.length() + 1);
	assert (NULL != dirPath);
	if (dirPath) 
	{
		HLock(dirPath);		
		**(INTSIZE **) dirPath = 1;  // number of search path 
		uchar* tmpPtr = (uchar *) (*(INTSIZE **) dirPath + 1);    
		STRCPY(tmpPtr, (const uchar*) iInputFolder.c_str()); 
		HUnlock(dirPath);
	}
	
	// allocate handle which on exit will fill with the found file
	Handle extNameHndl = NULL;
	Handle filePathHandle = NewHandle(sizeof(int32) + (sizeof(ftyperec) * 1));
	assert (NULL != filePathHandle);
	if (filePathHandle) 
	{
		HLock(filePathHandle);
		**(int32 **) filePathHandle = 0;
		uchar* tmpPtr = (uchar *)(*(int32 **) filePathHandle + 1);
		
		// allocate handle which on exit will fill with the found file initilize to size of 'int32'   
		extNameHndl = NewHandle(sizeof(int32) + (sizeof(ftyperec) * 1));
		assert (NULL != extNameHndl);
		if (extNameHndl) 
		{
			**(int32 **)extNameHndl = 0;
			(**(int32 **)filePathHandle)++;  // increment the number of type to search 
			
			// search for doc in input folder 	
			STRCPY(((ftypeptr) tmpPtr)->ftype, "*");
			((ftypeptr) tmpPtr)->lang = -1;       
			((ftypeptr) tmpPtr)->a.skipappf = 1;       
			((ftypeptr) tmpPtr)->a.appendfinfo = 0;
			((ftypeptr) tmpPtr)->a.attachpath = 0;	  // gives us full pathname  
			((ftypeptr) tmpPtr)->a.minpath = 1;  	  // search from search path 1      
			((ftypeptr) tmpPtr)->a.maxpath = 1;  	  // to search path 1        
			((ftypeptr) tmpPtr)->a.findfirst = 0;
			((ftypeptr) tmpPtr)->hndl = extNameHndl;   // where to put the found file 
			
		}
		
		// prendo i files all'interno della cartella di ricerca
		getfilesbytype(filePathHandle, dirPath);
		
		// libero la memoria
		HUnlock(filePathHandle);
		DisposeHandle(filePathHandle);
		filePathHandle = NULL;	
	}
	
	if (dirPath) 
		DisposeHandle(dirPath);	
		
	if (extNameHndl) 
	{    
		// process the extNameHndl    
		HLock(extNameHndl);

		// caratteri da skippare
		iCharToSkip.append(".");

		// ciclo sui file ritornati
		oNumOfDir = 0;
		int32 numFile = **(int32 **) extNameHndl;   
		uchar* tmpPtr = (uchar *)(*(int32 **) extNameHndl + sizeof(int32)); 
		std::string subDirTmpStr("");
		for (int16 i = 0; i < numFile; i++) 
		{        							
			std::string s;
			s = tmpPtr[0];
			if (std::string::npos == iCharToSkip.find(s))
			{
				subDirTmpStr = iInputFolder;      
				subDirTmpStr += "\\";
				subDirTmpStr += (char*) tmpPtr;
				
				// controllo se ho trovato una cartella 
				bool8 dirFound = IsDir(subDirTmpStr);
				if (TRUE == dirFound)
				{
					// contatore del numero di directory
					oNumOfDir++;
					
					// per ritornare solo il nome della cartella trovata
					oSubFolder = (char*) tmpPtr;
					oDirFound = TRUE;
					
					// blocco il ciclo cosi' ritorno il primo folder trovato
					break;        
		 		}    
			}
			tmpPtr += STRLEN(tmpPtr) + 1;
		}
		
		// libero la memoria	
		HUnlock(extNameHndl);
		DisposeHandle(extNameHndl);
	}
	
#endif // 1
#endif // WINOS
	
}// DBP::FindFirstFolder

#if WINOS	
/* ------------------------------------------------------------------------ *

	DBP::FindFirstFile
	
* ------------------------------------------------------------------------ */
void XTAPI DBP::FindFirstFile(const std::string& iInputFolder, const std::string& iFileType,
	std::string& oFileFoundName, bool8& oFileFound, int32& oNumFileFound, const int32& iFileIndex) throw()
{
	assert(!iInputFolder.empty());
	assert(iInputFolder.length() < MAXPATHNAME);

#if 1
	// risultato della ricerca
	oFileFoundName.clear();
	oFileFound = FALSE;
	oNumFileFound = 0;

	// costruisco il nome della cartella di ricerca
	std::string inputFolder(iInputFolder);
	inputFolder.append("\\*.");
	inputFolder.append(iFileType);

	// avvio la ricerca
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
	hFind = FindFirstFile(inputFolder.c_str(), &findFileData);
	if (INVALID_HANDLE_VALUE != hFind) 
	{
		// ciclo per contare i file contenuti nel folder
		// e per prenderne quello corrispondente all'indice passato
		while (1)
		{
			// controllo che sia effettivamente un file
			std::string fileName(findFileData.cFileName);
			if (!(FILE_ATTRIBUTE_DIRECTORY & findFileData.dwFileAttributes))		
			{
				if (!oFileFound && iFileIndex == oNumFileFound)
				{
					// memorizzo i dati del file scelto
					oFileFoundName = fileName;
					oFileFound = TRUE;
				}
				
				// conto i file
				++oNumFileFound;
			}

			// prendo il file successivo
			bool8 ret = FindNextFile(hFind, &findFileData);
			if (!ret)
			{
				break;
			}
			
		}
	}
	
    FindClose(hFind);

#else // 1

	// handle alla directory in cui effettuo la ricerca
	Handle dirPath = NewHandle(sizeof(INTSIZE) + iFolderPath.length() + 1);
	assert(NULL != dirPath);
	if (dirPath) 
	{
		HLock(dirPath);		
		**(INTSIZE **)dirPath = 1;  // number of search path 
		uchar* tmpPtr = (uchar *)(*(INTSIZE **)dirPath+1);    
		STRCPY(tmpPtr, (const uchar*) iFolderPath.c_str()); 
		HUnlock(dirPath);
	}
	
	// allocate handle which on exit will fill with the found file
	Handle extNameHndl = NULL;
	Handle filePathHandle = NewHandle(sizeof(int32) + (sizeof(ftyperec) * 1));
	assert(NULL != filePathHandle);
	if (filePathHandle) 
	{
		HLock(filePathHandle);
		**(int32 **)filePathHandle = 0;
		uchar* tmpPtr = (uchar *)(*(int32 **)filePathHandle+1);
		
		// allocate handle which on exit will fill with the found file initilize to size of 'int32'   
		extNameHndl = NewHandle(sizeof(int32)+(sizeof(ftyperec)*1));
		assert(NULL != extNameHndl);
		if (extNameHndl) 
		{
			**(int32 **)extNameHndl = 0;
			(**(int32 **)filePathHandle)++;  // increment the number of type to search 
			
			// search for doc in input folder 	
			STRCPY(((ftypeptr)tmpPtr)->ftype, iType);
			((ftypeptr)tmpPtr)->lang = -1;       
			((ftypeptr)tmpPtr)->a.skipappf = 1;       
			((ftypeptr)tmpPtr)->a.appendfinfo = 0;
			((ftypeptr)tmpPtr)->a.attachpath = 0;	  // gives us full pathname  
			((ftypeptr)tmpPtr)->a.minpath = 1;  	  // search from search path 1      
			((ftypeptr)tmpPtr)->a.maxpath = 1;  	  // to search path 1        
			((ftypeptr)tmpPtr)->a.findfirst = 0;
			((ftypeptr)tmpPtr)->hndl = extNameHndl;   // where to put the found file 	
		}
		
		// ricerca di tutti i files all'interno della cartella
		getfilesbytype(filePathHandle,dirPath);
		
		// libero la memoria
		HUnlock(filePathHandle);
		DisposeHandle(filePathHandle);
		filePathHandle = NULL;	
	}
	
	if (dirPath) 
		DisposeHandle(dirPath);	
		
	if (extNameHndl) 
	{    
		// process the extNameHndl    
		HLock(extNameHndl); 
		
		// flag per sapere se ho trovato un file
		oNumFileFound = **(int32 **) extNameHndl;   
		uchar* tmpPtr = (uchar *)(*(int32 **) extNameHndl + sizeof(int32)); 
		int32 counter = -1;
		for (int16 i = 0; i < oNumFileFound; i++) 
		{
			// devo controllare che non inizino con il punto
			if (tmpPtr[0] != '.')
			{
				// controllo il valore dell'indice del file da ritornare
				if (++counter == iFileIndex)
				{
					// ritorno solo il nome del file
					oFileFoundName = (char*) tmpPtr;
					oFileFound = TRUE;
					
					// blocco il ciclo cosi' ritorno il rpimo file trovato
					break;
				}
			}
			tmpPtr += STRLEN(tmpPtr) + 1;
		}
		
		// libero la memoria
		HUnlock(extNameHndl);
		DisposeHandle(extNameHndl);
	}
#endif // 1
} // DBP::FindFirstFile
#endif // WINOS		

#if MACOS && (QXP60 || QCD35)	
/* ------------------------------------------------------------------------ *

	DBP::FindFirstFile
	
* ------------------------------------------------------------------------ */
std::vector<std::string> XTAPI DBP::FindFiles(const uchar* iInputFolder, const uint32 iFileType, const int32 iFileIndex, bool8& oFileFound) throw()
{	
	// vettore per ritornare il risultato
	std::vector<std::string> fileFoundVector;

	// imposto di default il flag a false
	oFileFound = FALSE;

	// handle alla directory in cui effettuo la ricerca
	Handle dirPath = NewHandle(sizeof(int16) + STRLEN(iInputFolder) + 1);
	assert(NULL != dirPath);
	if (dirPath) 
	{
		HLock(dirPath);		
		**(int16 **)dirPath = 1;  // number of search path 
		uchar* tmpPtr = (uchar *)(*(int16 **)dirPath+1);    
		STRCPY(tmpPtr, (const uchar*) iInputFolder); 
		HUnlock(dirPath);
	}
	
	// allocate handle which on exit will fill with the found file
	Handle extNameHndl = NULL;
	Handle filePathHandle = NewHandle(sizeof(int32) + (sizeof(ftyperec) * 1));
	assert(NULL != filePathHandle);
	if (filePathHandle) 
	{
		HLock(filePathHandle);
		**(int32 **)filePathHandle = 0;
		uchar* tmpPtr = (uchar *)(*(int32 **)filePathHandle+1);
		
		// allocate handle which on exit will fill with the found file initilize to size of 'int32'   
		extNameHndl = NewHandle(sizeof(int32));
		assert(NULL != extNameHndl);
		if (extNameHndl) 
		{
			**(int32 **)extNameHndl = 0;
			(**(int32 **)filePathHandle)++;  // increment the number of type to search 
			
			// search for doc in input folder 	
			((ftypeptr)tmpPtr)->creator = NULL;
			
			if (iFileType==0)
				((ftypeptr)tmpPtr)->ftype = NULL;				
			else
				((ftypeptr)tmpPtr)->ftype = iFileType;
			
			((ftypeptr)tmpPtr)->lang = -1;       
			((ftypeptr)tmpPtr)->a.skipappf = 1;       
			((ftypeptr)tmpPtr)->a.appendfinfo = 0;
			((ftypeptr)tmpPtr)->a.attachpath = 0;	  // gives us full pathname  
			((ftypeptr)tmpPtr)->a.minpath = 1;  	  // search from search path 1      
			((ftypeptr)tmpPtr)->a.maxpath = 1;  	  // to search path 1        
			((ftypeptr)tmpPtr)->a.findfirst = 0;
			((ftypeptr)tmpPtr)->hndl = extNameHndl;   // where to put the found file 	
		}
		
		// ricerca di tutti i files all'interno della cartella
		getfilesbytype(filePathHandle,dirPath);
		
		// libero la memoria
		HUnlock(filePathHandle);
		DisposeHandle(filePathHandle);
		filePathHandle = NULL;	
	}
	
	if (dirPath) DisposeHandle(dirPath);

	// examine the files found 
	if (extNameHndl) 
	{
	    // process the extnamehndl 
	    HLock(extNameHndl);
	    int32 numFileFound = **(int32 **)extNameHndl;
	    uchar* tmpPtr = (uchar *)(*(int32 **)extNameHndl+1);
	    for (int16 i = 0; i < numFileFound; i++) 
	    {
	        // estraggo nome file
	        uchar file[256] = "";
	        STRCPY(file,tmpPtr);
	        
	        // inserisco nome in vettore....
	        P2CSTR(file);
	        
	        // ... ma solo se non inizia per '.'
	        if (file[0]!='.')
	        {
		        std::string tmpCStr((char*) file);
		        fileFoundVector.push_back(tmpCStr);
		        
		        // flag a true
		        oFileFound = TRUE;
	        }
	        
	        tmpPtr += *tmpPtr + 1;		        
	    }
	    HUnlock(extNameHndl);
	    DisposeHandle(extNameHndl);
	}
	
	return(fileFoundVector);	
} // FindFiles
#endif // MACOS && (QXP60 || QCD35)		

/* ------------------------------------------------------------------------ *

	DBP::CopyFile
	
* ------------------------------------------------------------------------ */
bool8 XTAPI DBP::CopyFile(const uchar* const iFilePathIn, const uchar* const iFilePathOut) throw()
{
	assert(NULL != iFilePathIn);
	assert(0 != iFilePathIn[0]);
	assert(STRLEN(iFilePathIn) < MAXPATHNAME);
	assert(NULL != iFilePathOut);
	assert(0 != iFilePathOut[0]);
	assert(STRLEN(iFilePathOut) < MAXPATHNAME);

	OSErr copyError = noErr;

#if MACOS

	FSSpec fsSpecFolderIN;
	FSSpec fsSpecFolderOUT;
	
	FSMakeFSSpec(0, 0, iFilePathIn, &fsSpecFolderIN);
	FSMakeFSSpec(0, 0, iFilePathOut, &fsSpecFolderOUT);
	
// duplicazione file
#if QCD35
	XFileInfoRef srcFileInfoRef;
	XFileInfoRef destFileInfoRef;

	copyError = XTCreateEmptyXFileInfoRef(&srcFileInfoRef);
	if (copyError != ERR_SUCCESS) {
		copyError = memFullErr;
		goto _Exit;
	}

	copyError = XTCreateEmptyXFileInfoRef(&destFileInfoRef);
	if (copyError != ERR_SUCCESS) {
		copyError = memFullErr;
		goto _Exit;
	}

	copyError = XTSetFSSpecInXFileInfoRef(srcFileInfoRef, &fsSpecFolderIN);
	if (copyError != ERR_SUCCESS) {
		goto _CleanUp;
	}

	copyError = XTSetFSSpecInXFileInfoRef(destFileInfoRef, &fsSpecFolderOUT);
	if (copyError != ERR_SUCCESS) {
		goto _CleanUp;
	}

	copyError = XTDuplicateFile(srcFileInfoRef, destFileInfoRef, TRUE, TRUE, FALSE);
	if (copyError != ERR_SUCCESS) {
		goto _CleanUp;
	}

_CleanUp:
	
	XTDisposeXFileInfoRef(srcFileInfoRef);
	XTDisposeXFileInfoRef(destFileInfoRef);

#else
	// duplicazione file
	copyError = DuplicateFileFSp(&fsSpecFolderIN, &fsSpecFolderOUT, TRUE, TRUE, FALSE);
#endif // QCD35

#endif // MACOS
	
#if WINOS
	// setto l'ultimo flag a TRUE se voglio che mi venga chiesto di sovrascrivere

	// un file che esiste gia'	
	copyError = ::CopyFile((const char*)iFilePathIn, (const char*) iFilePathOut, FALSE);
#endif //WINOS

_Exit:

	return(copyError == noErr);

} // DBP::CopyFile

/* ------------------------------------------------------------------------ *

	DBP::CopyFile
	
* ------------------------------------------------------------------------ */
bool8 XTAPI DBP::CopyFile(const std::string& iFilePathIn, const std::string& iFilePathOut) throw()
{
	assert(!iFilePathIn.empty());
	assert(iFilePathIn.length() < MAXPATHNAME);
	assert(!iFilePathOut.empty());
	assert(iFilePathOut.length() < MAXPATHNAME);

	return(DBP::CopyFile((const uchar*) iFilePathIn.c_str(), (const uchar*) iFilePathOut.c_str()));
} // DBP::CopyFile

/* ------------------------------------------------------------------------ *

	DBP::DeleteFile

* ------------------------------------------------------------------------ */
bool8 XTAPI DBP::DeleteFile(const uchar* const iFilePath) throw()
{
	assert (NULL != iFilePath);
	assert(0 != iFilePath[0]);
	assert(STRLEN(iFilePath) < MAXPATHNAME);
	
	OSErr testError = noErr;
	
#if MACOS
	// versione MACOS ancora da realizzare
	assert(0);
#endif // MACOS

#if WINOS	
	uchar volName;
	int16 vRefNum;
	int32 dirID;
	HGetVol(&volName, &vRefNum, &dirID);
	
	testError = HDelete(vRefNum, 0, iFilePath);
#endif // WINOS	

	return(noErr == testError);
} // DBP::DeleteFile 

/* ------------------------------------------------------------------------ *

	DBP::DeleteFile

* ------------------------------------------------------------------------ */
bool8 XTAPI DBP::DeleteFile(const std::string& iFilePath) throw()
{
	assert(!iFilePath.empty());
	assert(iFilePath.length() < MAXPATHNAME);

	return(DBP::DeleteFile((const uchar*) iFilePath.c_str()));
} // DBP::DeleteFile 

/* ------------------------------------------------------------------------ *

	DBP::CreateDir

* ------------------------------------------------------------------------ */
bool8 XTAPI DBP::CreateDir(const uchar* const iDirPath) throw()
{
	assert(NULL != iDirPath);
	assert(0 != iDirPath[0]);
	assert(STRLEN(iDirPath) < MAXPATHNAME);
	
	bool8 newDirErr = FALSE;
	
#if MACOS
	// versione MACOS ancora da realizzare
	assert(0);
#endif // MACOS

#if WINOS
	newDirErr = CreateDirectory((const char*) iDirPath, NULL);	
#endif // WINOS
	
	return(newDirErr);		
} // DBP::CreateDir

/* ------------------------------------------------------------------------ *

	DBP::CreateDir

* ------------------------------------------------------------------------ */
bool8 XTAPI DBP::CreateDir(const std::string& iDirPath) throw()
{
	assert(!iDirPath.empty());
	assert(iDirPath.length() < MAXPATHNAME);

	return(DBP::CreateDir((const uchar*) iDirPath.c_str()));
} // DBP::CreateDir

/* ------------------------------------------------------------------------ *

	DBP::RemoveDir

* ------------------------------------------------------------------------ */
bool8 XTAPI DBP::RemoveDir(const uchar* const iDirPath) throw()
{
	assert(NULL != iDirPath);
	assert(0 != iDirPath[0]);
	assert(STRLEN(iDirPath) < MAXPATHNAME);

	bool8 delErr = FALSE;

#if MACOS
	// versione MACOS ancora da realizzare
	assert(0);
#endif // MACOS

#if WINOS	
	delErr = RemoveDirectory((const char*) iDirPath);
#endif // WINOS	
	
	return(delErr);
} // DBP::RemoveDir

/* ------------------------------------------------------------------------ *

	DBP::RemoveDir

* ------------------------------------------------------------------------ */
bool8 XTAPI DBP::RemoveDir(const std::string& iDirPath) throw()
{
	assert(!iDirPath.empty());
	assert(iDirPath.length() < MAXPATHNAME);

	return(DBP::RemoveDir((const uchar*) iDirPath.c_str()));
} // DBP::RemoveDir

/* ------------------------------------------------------------------------ *

	DBP::CreateHtmlFile

* ------------------------------------------------------------------------ */
bool8 XTAPI DBP::CreateHtmlFile(const std::string& iFilePath, const std::string& iFileContent) throw()
{
	assert(!iFilePath.empty());
	assert(iFilePath.length() < MAXPATHNAME);
	
	std::ofstream htmlFile;
	htmlFile.open((char*) iFilePath.c_str());
	if (htmlFile.is_open())
	{
		
		htmlFile << "<html>" << std::endl << "<body>"
			<< iFileContent.c_str() 
			<< "</body>" << std::endl << "</html>" << std::endl;
		
		htmlFile.close();
		
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
} // DBP::CreateHtmlFile

/* ------------------------------------------------------------------------ *

	DBP::GetFullPathName

* ------------------------------------------------------------------------ */
bool8 XTAPI DBP::GetFullPathName(FSSpec& iFileSpec, std::string& oFilePath) throw()
{	
	// prendo il pathname completo del file 
	uchar filePath[MAXPATHNAME];
	GetFullPathFSp(filePath, &iFileSpec, FALSE);

	// ritorno il fullpathname	
	oFilePath = (char*) filePath;
	
	// controllo sel il path name e' stato troncato
	std::string fileName((char*) iFileSpec.name);
	std::string endFilePath(oFilePath.substr(oFilePath.length() - fileName.length()));

	return(fileName == endFilePath);
} // DBP::GetFullPathName

#if QXP40 || QXP50
/* ------------------------------------------------------------------------ *

	DBP::GetFullPathName

* ------------------------------------------------------------------------ */
bool8 XTAPI DBP::GetFullPathName(StandardFileReply& iFileSFR, std::string& oFilePath) throw()
{			
	return(DBP::GetFullPathName(iFileSFR.sfFile, oFilePath));	
	
} // DBP::GetFullPathName
#endif // QXP40 || QXP50

/* ------------------------------------------------------------------------ *

	DBP::BuildPathOnFs

* ------------------------------------------------------------------------ */
OSErr XTAPI DBP::BuildPathOnFs(const uchar* iPath) throw()
{	
	// variabile di ritorno
	OSErr retErr = noErr;

#if MACOS && QXP60	
	// estraggo il solo nome file
	uchar fileTmp[MAXPATHNAME] = "";
	uchar foldersToCreate[MAXPATHNAME] = "";
	STRCPY(fileTmp, iPath);
	ExtractFileName(fileTmp);
	
	// ricavo il path senza il nome file
	STRNCPY(foldersToCreate, iPath, (STRLEN(iPath) - STRLEN(fileTmp)));
	
	// copio path in stringa per facilitare la costruzione
	std::string pathTmp(DBP::CCC(foldersToCreate));
	
	// per contenere il path costruito
	uchar folderInPascalStr[256] = "";
	
	// conto il numer di separatori
	int16 foldersNum = std::count (pathTmp.begin(), pathTmp.end(), ':');
	
	// inizializzo gli indici per scorrere la stringa
	int16 idxStart = 0;
	int16 idxEnd = pathTmp.find(':', idxStart);
	
	for (int16 i = 0; i < foldersNum; i++)
	{
		// ricavo folder uno ad uno
		std::string folder = pathTmp.substr(idxStart, (idxEnd - idxStart + 1)).c_str();
		
		// copio il risultato in una stringa pascal
		STRCAT(folderInPascalStr, DBP::PPP(folder.c_str())); 
		
		// posiziono gli indici sul folder successivo
		// idxStart = idxEnd;
		idxStart = idxEnd + 1;
		idxEnd = pathTmp.find(':', idxStart);
		
		// controllo FSpec
		FSSpec foldersToCreateSpec;
		retErr = FSMakeFSSpec(0, 0, folderInPascalStr, &foldersToCreateSpec);
		
		if (retErr == fnfErr)
		{
			// devo creare il path del file
			int32 dirID;
			retErr = FSpDirCreate(&foldersToCreateSpec, smAllScripts, &dirID);
			
			if (retErr != noErr)
			{
				goto _Exit;
			}
			else
			{
				std::cout << "directory: " << DBP::CCC(folderInPascalStr) << " creata su file system";
			}
		}
	}
#endif // MACOS && QXP60
_Exit:	
	return(retErr);
} // DBP::BuildPathOnFs

/* ------------------------------------------------------------------------ *

	DBP::CreateOneFolderOnFs

* ------------------------------------------------------------------------ */
OSErr XTAPI DBP::CreateOneFolderOnFs(const uchar* iPathFolderIntoCreate, const uchar* iNewFolderName, uchar* oNewFolderPath, FSSpec& oFolderSpec) throw()
{
	assert(iPathFolderIntoCreate != NULL);
	assert(iNewFolderName != NULL);

	// per tornare errore
	OSErr err = noErr;

#if MACOS && QXP60
	// copio pathname folder
	uchar tmpFolder[MAXPATHNAME] = "";
	STRCPY(tmpFolder, iPathFolderIntoCreate);
	STRCAT(tmpFolder, iNewFolderName);

	// ricavo FSSpec
	FSSpec tmpSpec = {0, 0, NULL};
	FSMakeFSSpec(0, 0, tmpFolder, &tmpSpec);

	// creo directory
	int32 dirID;
	FSpDirCreate(&tmpSpec, smAllScripts, &dirID);
	if (err == noErr)
	{
		STRCPY(oNewFolderPath, tmpFolder);	
		oFolderSpec = tmpSpec;
	}
#endif // MACOS && QXP60
	
	return(err);
} // DBP::CreateOneFolderOnFs

/* ------------------------------------------------------------------------ *

	ExtractFolderPath
	
* ------------------------------------------------------------------------ */
void XTAPI DBP::ExtractFolderPath(uchar* ioFileFullPath) throw()
{
	assert(ioFileFullPath != NULL);

#if MACOS && QXP60	
	// ricavo nome (anche) parziale dei jpg da eliminare
	uchar nameFile[256] = "" ;
	STRCPY(nameFile, ioFileFullPath);

	ExtractFileName(nameFile);
	
	// ricavo folder in cui cercare i jpeg
	STRNCPY(ioFileFullPath, ioFileFullPath, (STRLEN(ioFileFullPath) - STRLEN(nameFile)));
#endif // MACOS && QXP60
	
} // DBP::ExtractFolderPath


