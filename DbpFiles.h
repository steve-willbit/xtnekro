/*

	@header			DbpFiles.h
	@abstract 		DBP files Manager.
	@discussion 	Questo header file contiene tutte le definizioni comuni per la gestione
					dei files.
	
					Copyright © Sinedita S.r.l 2002. All Rights Reserved.

*/

#if !defined(dbpfiles_h)
#define dbpfiles_h

// DBP INCLUDES
// DBP INCLUDES
// DBP INCLUDES

#include "DbpInclude.h"

// NAMESPACE DBP
// NAMESPACE DBP
// NAMESPACE DBP

namespace DBP
{
		
	// FUNCTIONS
	// FUNCTIONS
	// FUNCTIONS
	
	/*!
		@function		IsFile
		@abstract 		esistenza di un file.
		@discussion		Lo scopo di questa funzione e' di controllare se un certo file e' presente
						nel File System. Il parametro deve essere valido e contiene il pathname completo
						del file. Ritorna TRUE se il file esiste, FALSE in tutti gli altri casi.
		
						17 giugno 2003 - Taretto Fabrizio.

		@param			iFilePath path name completo del file.
		@result  		TRUE o FALSE a seconda che il file sia o meno presente nel File System.
	*/
	bool8 XTAPI IsFile(const uchar* const iFilePath) throw();

	/*!
		@function		IsFile
		@abstract 		esistenza di un file.
		@discussion		Lo scopo di questa funzione e' di controllare se un certo file e' presente
						nel File System. Il parametro deve essere valido e contiene il pathname completo
						del file. Ritorna TRUE se il file esiste, FALSE in tutti gli altri casi.
		
						17 giugno 2003 - Taretto Fabrizio.

		@param			iFilePath path name completo del file.
		@result  		TRUE o FALSE a seconda che il file sia o meno presente nel File System.
	*/
	bool8 XTAPI IsFile(const std::string& iFilePath) throw();
	
	/*!
		@function		IsDir
		@abstract 		stabilire se è una cartella.
		@discussion		funzione che ritorna TRUE nel caso in cui il file sia una cartella.
		
						4 aprile 2003 - Taretto Fabrizio.

		@param			iFolder path della cartella.
		@result  		TRUE o FALSE a seconda che il file sia o meno una cartella.
	*/
	bool8 XTAPI IsDir(const uchar* const iFolder) throw();

	/*!
		@function		IsDir
		@abstract 		stabilire se è una cartella.
		@discussion		funzione che ritorna TRUE nel caso in cui il file sia una cartella.
		
						4 aprile 2003 - Taretto Fabrizio.

		@param			iFolder path della cartella.
		@result  		TRUE o FALSE a seconda che il file sia o meno una cartella.
	*/
	bool8 XTAPI IsDir(const std::string& iFolder) throw();
	
	/*!
		@function		FindFirstFolder
		@abstract 		trova la prima subdirectory.
		@discussion		Funzione che esegue la ricerca della prima sottocartella all'interno della cartella principale.
						Il path name di ricerca non contiene come ultimo carattere il separatore di cartelle.
		
						4 aprile 2003 - Taretto Fabrizio.

		@param			iFolderPath path della cartella di input nella quale eseguire la ricerca.
		@param			oSubFolder nome della sottocartella trovata. Non e' un path name completo.
		@param			oDirFound TRUE se la ricerca ha avuto esito positivo
		@param			oNumOfDir ritorna il numero di directory trovate.
		@param			iCharsToSkip eventuale elenco dei caratteri da skippare all'inizio del folder.
						Se non voglio skippare nulla metto "".
		@result  		nessuno.
	*/
	void XTAPI FindFirstFolder(const std::string& iInputFolder, std::string& oSubFolder,
		bool8& oDirFound, int32& oNumOfDir, const std::string& iCharsToSkip = std::string("")) throw();


	/*!
		@function		FindFirstFile
		@abstract 		trova il primo file di in una cartella.
		@discussion		Funzione che esegue la ricerca del primo file di Xpress in una cartella.
						Il path name di ricerca non contiene come ultimo carattere il separatore di cartelle.
						Ritorna il nome del file, un booleano per indicare se la ricerca ha dato risultato positivo e in intero
						che indica il numero totale di file nella cartella.
		
						30 marzo 2003 - Taretto Fabrizio.

		@param			iInputFolder path della cartella di input nella quale eseguire la ricerca.
		@param			iFileType tipo di file che voglio cercare. Inserire l'estensione.
		@param			oFileFoundName nome del file trovato. Non e' un poath name completo.
		@param			oFileFound True se la ricerca ha avuto esito positivo.
		@param			oNumFileFound variabile che ritorna il numero di files trovati.
		@param			iFileIndex variabile per indicare l'indice del file da ritornare. Si parte da zero per
						per ritornare il primo file.
		@result  		nessuno.
	*/
	void XTAPI FindFirstFile(const std::string& iInputFolder, const std::string& iFileType, std::string& oFileFoundName,
		bool8& oFileFound, int32& oNumFileFound, const int32& iFileIndex = 0) throw();

	/*!
		@function		FindFiles
		@abstract 		trova il primo file di in una cartella.
		@discussion		Funzione che esegue la ricerca del primo file di Xpress in una cartella.
						Il path name di ricerca non contiene come ultimo carattere il separatore di cartelle.
						Ritorna il nome del file, un booleano per indicare se la ricerca ha dato risultato positivo e in intero
						che indica il numero totale di file nella cartella.
		
						3 gennaio 2005 - Taretto Fabrizio.

		@param			iInputFolder path della cartella di input nella quale eseguire la ricerca.
		@param			iFileType tipo di file che voglio cercare. Inserire l'estensione.
		@param			oFileFound True se la ricerca ha avuto esito positivo.
		@param			iFileIndex variabile per indicare l'indice del file da ritornare. Si parte da zero per
						per ritornare il primo file.
		@result  		vettore con nome file trovato, no fullpath.
	*/
	std::vector<std::string> XTAPI FindFiles(const uchar* iInputFolder, const uint32 iFileType, const int32 iFileIndex, bool8& oFileFound) throw();

	/*!
		@function		CopyFile
		@abstract 		duplicazione un file da un folder all'altro.
		@discussion		duplicazione un file da un folder all'altro.
		
						30 marzo 2003 - Taretto Fabrizio.

		@param			iFilePathIn path del file nella cartella di input.
		@param			iFilePathOut path del file nella cartella output.
		@result  		TRUE se la copia ha successo, FALSE in tutti gli altri casi.
	*/
	bool8 XTAPI CopyFile(const uchar* const iFilePathIn, const uchar* const iFilePathOut) throw();

	/*!
		@function		CopyFile
		@abstract 		duplicazione un file da un folder all'altro.
		@discussion		duplicazione un file da un folder all'altro.
		
						30 marzo 2003 - Taretto Fabrizio.

		@param			iFilePathIn path del file nella cartella di input.
		@param			iFilePathOut path del file nella cartella output.
		@result  		TRUE se la copia ha successo, FALSE in tutti gli altri casi.
	*/
	bool8 XTAPI CopyFile(const std::string& iFilePathIn, const std::string& iFilePathOut) throw();

	/*!
		@function		DeleteFile
		@abstract 		delimina file.
		@discussion		esegue l'eliminazione del file di cui passo il pathname.
		
						30 marzo 2003 - Taretto Fabrizio.

		@param			iFilePath path del file nella cartella di input.
		@result  		TRUE se la cancellazione ha successo, FALSE in tutti gli altri casi.
	*/
	bool8 XTAPI DeleteFile(const uchar* const iFilePath) throw();

	/*!
		@function		DeleteFile
		@abstract 		delimina file.
		@discussion		esegue l'eliminazione del file di cui passo il pathname.
		
						30 marzo 2003 - Taretto Fabrizio.

		@param			iFilePath path del file nella cartella di input.
		@result  		TRUE se la cancellazione ha successo, FALSE in tutti gli altri casi.
	*/
	bool8 XTAPI DeleteFile(const std::string& iFilePath) throw();
	
	/*!
		@function		CreateDir
		@abstract 		crea directory.
		@discussion		esegue l'eliminazione della directory di cui passo il pathname.
		
						17 aprile 2003 - Taretto Fabrizio.

		@param			iDirPath path dellacartella da creare.
		@result  		TRUE se la directory viene creata.
	*/
	bool8 XTAPI CreateDir(const uchar* const iDirPath) throw();

	/*!
		@function		CreateDir
		@abstract 		crea directory.
		@discussion		esegue l'eliminazione della directory di cui passo il pathname.
		
						17 aprile 2003 - Taretto Fabrizio.

		@param			iDirPath path dellacartella da creare.
		@result  		TRUE se la directory viene creata.
	*/
	bool8 XTAPI CreateDir(const std::string& iDirPath) throw();
	
	/*!
		@function		RemoveDir
		@abstract 		elimina directory.
		@discussion		esegue l'eliminazione della directory di cui passo il pathname.
		
						17 aprile 2003 - Taretto Fabrizio.

		@param			iDirPath path della cartella di input da eliminare.
		@result  		TRUE se la directory viene rimossa.
	*/
	bool8 XTAPI RemoveDir(const uchar* const iDirPath) throw();

	/*!
		@function		RemoveDir
		@abstract 		elimina directory.
		@discussion		esegue l'eliminazione della directory di cui passo il pathname.
		
						17 aprile 2003 - Taretto Fabrizio.

		@param			iDirPath path della cartella di input da eliminare.
		@result  		TRUE se la directory viene rimossa.
	*/
	bool8 XTAPI RemoveDir(const std::string& iDirPath) throw();

	/*!
		@function		CreateHtmlFile
		@abstract 		crea un file html.
		@discussion		Esegue la creazione di un file HTML utilizzando il contenuto del
						parametro iFileContent. In iFileContent c'e' la parte interna da inserire
						trai tag html - body. La funzione inserisce i tag html e body. Non crea le
						cartelle intermedie: in questo caso ritorna FALSE. 
		
						6 giugno 2003 - Taretto Fabrizio.

		@param			iDirPath path completo del file da creare. Il nome comprede anche
						l'estensione .HTML.
		@param			iFileContent contenuto da inserire nel file.
		@result  		TRUE se il file è creato. FALSE in tutti gli altri casi di errore.
	*/
	bool8 XTAPI CreateHtmlFile(const std::string& iFilePath, const std::string& iFileContent) throw();
	
	/*!
		@function		GetFullPathName
		@abstract 		creazione path name completo.
		@discussion		Lo scopo di questa funzione e' determinare il path name completo del
						file passato come parametro di input. 
		
						19 giugno 2003 - Taretto Fabrizio.

		@param			iFileSpec FSSpec del file di cui cercare il path name completo.
		@param			oFilePath ritorna il path name completo del file.
		@result  		TRUE se e' andato tutto bene, FALSE in tutti i casi di errore (l'errore
						piu' frequente che il path name completo e' stato troncato poiche' era troppo
						lungo).
	*/
	bool8 XTAPI GetFullPathName(FSSpec& iFileSpec, std::string& oFilePath) throw();

	/*! 
		@function		BuildPathOnFs
		@abstract		creazione folder su file system
		@discussion		Dato un file path, crea il percorso corretto se non esiste

						29 Giugno 2005 - Fabrizio Taretto.

		@param 			iFilePath file path tutta l'alberatura + (opzionalmente) nome file 
		
		@result			eventuale errore di sistema
	*/
	OSErr XTAPI BuildPathOnFs(const uchar* iFilePath) throw();
	
	/*! 
		@function		CreateOneFolderOnFs
		@abstract		creazione folder su file system
		@discussion		Dato un file path, crea una cartella al suo interno, ritornandone il path e l'FSSpec

						29 Giugno 2005 - Fabrizio Taretto.

		@param 			iPathFolderIntoCreate 
		@param			iNewFolderName
		@param			oNewFolderPath
		@param			oFolderSpec 
		
		@result			eventuale errore di sistema
	*/
	OSErr XTAPI CreateOneFolderOnFs(const uchar* iPathFolderIntoCreate, const uchar* iNewFolderName, uchar* oNewFolderPath, FSSpec& oFolderSpec) throw();

	/*! 
		@function		ExtractFolderPath
		@abstract		creazione folder su file system
		@discussion		dato il fullpath di un documento ne ritorna il path del folder

						29 Giugno 2005 - Fabrizio Taretto.

		@param 			ioFileFullPath fullpath del do in input, fullpath della dir in output
		
		@result			nessuno
	*/
	void XTAPI ExtractFolderPath(uchar* ioFileFullPath) throw();

	XTError XTAPI GetFullPathInLocalString(FSSpec* iFileSpec, uchar* oFileFullPath) throw();

	XTError XTAPI ExtractFileNameInLocalString(uchar* ioFile) throw();

} // namespace DBP


// USING NAMESPACE
// USING NAMESPACE
// USING NAMESPACE

using namespace DBP;

#endif // !defined(dbpfiles_h)