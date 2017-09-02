/*!
	@header			PresenzaFile.h
	@abstract		file
	@discussion 		File che contiene le dichiarazioni delle procedure per verificare
					l'esistenza del file da impaginare.
	
					8 Aprile 2005	Fabrizio
	
					Copyright © Sinedita S.r.l 2005. All Rights Reserved.

					$Log: not supported by cvs2svn $
					Revision 1.1  2005/04/20 07:14:21  taretto
					importazione files
					
*/
#ifndef PresenzaFile_h
#define PresenzaFile_h

// PROTOTYPES
// PROTOTYPES
// PROTOTYPES

/*!
	@function			PresenzaFile
	@abstract 		file
	@discussion		Verifica la presenza del file passato. Se non Š presente ritorna l'errore
					di sistema operativo	

					8 Aprile 2005			Fabrizio.

	@param 			omefile - puntatore alla stringa che contiene il path completo del file
					di cui fare la verificare la presenza.
	@result  			ritorna l'eventuale errore di sistema operativo.
*/
OSErr XTAPI  PresenzaFile(uchar* nomefile) throw();

/*!
	@function			PresenzaCartella
	@abstract 		cartelle
	@discussion		Verifica la presenza della cartella passato. Se non e' presente ritorna l'errore
					di sistema operativo. Il nome della cartella deve arrivare senza i due punti
					finali.

					8 Aprile 2005			Fabrizio.

	@param 			nomecartella puntatore alla stringa che contiene il path completo del file
					di cui fare la verificare la presenza.

	@result  			-1 se la cartella non e' presente, noErr altrimenti.
*/
int32 XTAPI PresenzaCartella(uchar *nomecartella) throw();

/*!
	@function			SpostaFileSuScrivania
	@abstract 		cartelle
	@discussion		Permette di spostare il file passato, dalla attuale alla scrivania.

					8 Aprile 2005			Fabrizio.

	@param 			nomefile contiene il nome completo dela file da spostare.
	@param			risultato TRUE se è stato possibile spostare il file passato

	@result  			ritorna l'eventuale errore dell'xtension
*/
errorixtension XTAPI SpostaFileSuScrivania(uchar* nomefile, bool8* risultato) throw();

/*!
	@function			SpostaFileNellaCartella
	@abstract 		gestione file e cartelle
	@discussion		Permette di spostare il file passato, dalla attuale posizione alla cartella
					passata come parametro.

					8 Aprile 2005			Fabrizio.

	@param 			nomefile contiene il nome completo dela file da spostare.
	@param			nomecartella contiene il nome della cartella dove spostare
	@param			risultato TRUE se è stato possibile spostare il file passato

	@result  			ritorna l'eventuale errore dell'xtension
*/
errorixtension XTAPI SpostaFileNellaCartella(uchar* nomefile, uchar* nomecartella, bool8* risultato) throw();

/*!
	@function			GenNomeFilenewExt
	@abstract 		gestione file e cartelle
	@discussion		Modifica estensione al file passato in input

					8 Aprile 2005			Fabrizio.

	@param 			parametro solo in entrata che indica il file di cui si vuole modificare l'estensione
	@param			nuova estensione

	@result  			ritorna l'eventuale errore dell'xtension
*/
errorixtension XTAPI GenNomeFilenewExt(uchar *name, uchar *ext) throw();

/*!
	@function			PresenzaFileDaQuattroD
	@abstract 		gestione file e cartelle
	@discussion		Cerca di verificare se nella cartella dei dati da impaginare
					c'e' un file. Se non c'e' nessun file ritorna FALSE.
					Tra i file presenti analizza solo quelli che hanno una determinata estensione.
					Se e' un file impaginabile ritorna TRUE impostando il puntatore passato
					per permetterne l'apertura e poi la cancellazione.

					8 Aprile 2005			Fabrizio.

	@param 			nomefile fullpath del file da impaginare
	@param			risultato  ritorna TRUE se e0' stato trovato un file da impaginare.

	@result  			ritorna l'eventuale errore dell'xtension
*/
errorixtension XTAPI PresenzaFileDaQuattroD(uchar *nomefile, bool8 *risultato) throw();

/*!
	@function			PrendiFileElencoImmagini
	@abstract 		gestione file e cartelle
	@discussion		Prende il file  dell'elenco delle immagini presente all'interno della cartella
					dati. Se non ci sono file con l'estensione giusta ritorna l'errore dell'xtension
					relativo.
					Se ci sono piu' file con l'estensione richiesta apre una dialog per permettere
					all'utente di scegliere il file giusto.

					8 Aprile 2005			Fabrizio.

	@param 			nomefile fullpath del file da impaginare
	
	@result  			ritorna l'eventuale errore dell'xtension
*/
errorixtension XTAPI PrendiFileElencoImmagini(uchar *name) throw();

#endif // PresenzaFile_h 

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if 0 // NECRO v1

/* Procedure */
/* Procedure */
/* Procedure */


//OSErr CreaFSSpecCartella(char *nomecartella, FSSpec *fsspeccartella);
//OSErr CreaFSSpecFile(char *nomefile, FSSpec *fsspecfile);
//OSErr CreaFInfoFile(char *nomefile, FInfo *finfofile);
/*!
	@function			PercorsoCompleto
	@abstract 		gestione file e cartelle
	@discussion		Ritorna il percoso completo.

					14 Aprile 2005			Fabrizio.

	@param 			percorsocompleto percorso completo del file o della cartella

	@result  			ritorna il percorso completo se riesce a costruirlo
*/
uchar* XTAPI PercorsoCompleto(uchar* percorsoparziale) throw();
errorixtension CancellaTutto(char *nomecartella);
errorixtension PrendiIdentificatoreCartella(char *nomecartella, long *idcartella);
errorixtension PrendiFileElencoImmagini(char *nomefile);
errorixtension PresenzaFileDaAppWare(char *nomefile, Boolean *risultato);

#endif // NECRO v1
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/