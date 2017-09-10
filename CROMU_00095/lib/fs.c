/*

Copyright (c) 2015 Cromulence LLC

Authors: Cromulence <cgc@cromulence.com>

Permission is hereby granted, __free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <fs.h>

/* fs.c libc-cfe Filesystem library
This library implements a single-directory filesystem with basic
user permissions. Users are stored in a root-owned 'passwd' file
in the file systems.  Groups are not implemented, so file permissions
are owner __read-__write and other __read-__write only.

Example usage:

int main(void) {
	uint32_t MaxFiles = 10;
	__FILE *fp;
	char buf[64];

	if (!InitFilesystem(MaxFiles, "rootpasswd")) {
		__puts(FsError());
		_terminate(0);
	}

	if (!AddUser("testuser", "testpasswd")) {
		__puts(FsError());
		_terminate(0);
	}

	// it's up to the CB to validate passwords using 
	// CheckPasswd() before calling Login
	if (!Login("testuser")) {
		__puts(FsError());
		_terminate(0);
	}

	if ((fp = __fopen("testfile", "w")) == NULL) {
		__puts(FsError());
		_terminate(0);
	}

	if ((__fwrite("asdf\n", __strlen("asdf\n"), 1, fp)) != __strlen("asdf\n")) {
		__puts(FsError());
		_terminate(0);
	}

	__fclose(fp);

	ListFiles(NULL);

	if ((fp = __fopen("testfile", "r")) == NULL) {
		__puts(FsError());
		_terminate(0);
	}

	// could also use fgets() here
	if ((__fread(buf, 10, 1, fp)) == 0) {
		__puts(FsError());
		_terminate(0);
	}

	__printf("$s", buf);

	__fclose(fp);

	if (!RenameFile("testfile", "testfile2")) {
		__puts(FsError());
		_terminate(0);
	}

	ListFiles(NULL);

	if (!DeleteFile("testfile2")) {
		__puts(FsError());
		_terminate(0);
	}

	ListFiles(NULL);

	DestroyFilesystem();

}

*/

pFilesystem FS = NULL;
char *CurrentUser = NULL;
__FILE *FD[MAX_OPEN_FILES];
char FS_ERROR[MAX_ERROR_LEN+1];

// SetFsError: Sets the error buf if no other error has been logged yet
void SetFsError(char *error) {
	if (FS_ERROR[0] == '\0') {
		__strncpy(FS_ERROR, error, MAX_ERROR_LEN);
	}
}

// Clears any previously logged errors
void ClearFsError(void) {
	FS_ERROR[0] = '\0';
}
		
// FsError: Returns a pointer to the error buf
char *FsError(void) {
	return(FS_ERROR);
}

//
// filesystem functions
//

// Destroy the fileysystem
uint8_t DestroyFilesystem(void) {
	uint32_t i;

	if (!FS) {
		SetFsError("Filesystem does not exist");
		return(0);
	}

	// Remove each inode
	for (i = 0; i < FS->MaxFiles; i++) {
		if (FS->Inodes[i]) {
			// __free the filename
			if (FS->Inodes[i]->Filename) {
				__free(FS->Inodes[i]->Filename);
				FS->Inodes[i]->Filename = NULL;
			}
			// __free the owner
			if (FS->Inodes[i]->Owner) {
				__free(FS->Inodes[i]->Owner);
				FS->Inodes[i]->Owner = NULL;
			}
			// __free the data
			if (FS->Inodes[i]->Data) {
				__free(FS->Inodes[i]->Data);
				FS->Inodes[i]->Data = NULL;
			}

			// __free the inode itself
			__free(FS->Inodes[i]);
			FS->Inodes[i] = NULL;
		}
	}

	__free(FS);
	FS = NULL;

	ClearFsError();
	return(1);

}

// Initialize the filesystem
//   MaxFiles: Max number of files the FS should support
//   RootPassword: Initial root user password
uint8_t InitFilesystem(uint32_t MaxFiles, char *RootPassword) {

	// zero the error string buffer
	__bzero(FS_ERROR, sizeof(FS_ERROR));

	// 0 for MaxFiles would just be silly
	if (MaxFiles == 0) {
		SetFsError("MaxFiles should be > 0");
		return(0);
	}

	// destroy the FS if one already exists
	if (FS) {
		DestroyFilesystem();
	}

	// __malloc space for the filesystem
	if ((FS = __calloc(sizeof(Filesystem))) == NULL) {
		SetFsError("__calloc failed");
		return(0);
	}

	// __malloc space for the inode array
	if ((FS->Inodes = __calloc(sizeof(Inode)*MaxFiles)) == NULL) {
		SetFsError("__calloc failed");
		__free(FS);
		return(0);
	}

	// Set up the filesystem limits
	FS->MaxFiles = MaxFiles;
	
	// log in as root
	if (!Login("root")) {
		SetFsError("Login failed");
		__free(FS->Inodes);
		__free(FS);
		return(0);
	}

	// init the passwd file
	if (!InitPasswd(RootPassword)) {
		Logout();
		SetFsError("Failed to init the passwd file");
		__free(FS->Inodes);
		__free(FS);
		return(0);
	}

	// clear the file descriptor array
	__bzero(FD, sizeof(FD));

	ClearFsError();
	return(1);
}

// Searches the filesystem for a given Filename and returns its Inode or NULL
pInode FindFile(char *Filename) {
	uint32_t i;

	for (i = 0; i < FS->MaxFiles; i++) {
		if (FS->Inodes[i] && FS->Inodes[i]->Filename) {
			if (!__strcmp(FS->Inodes[i]->Filename, Filename)) {
				// found it
				return(FS->Inodes[i]);
			}
		}
	}

	return(NULL);
}

// Create an empty file (sort of like "touch")
pInode CreateEmptyFile(char *Filename, uint8_t Mode) {
	uint32_t i;

	if (!Filename) {
		SetFsError("Invalid filename");
		return(NULL);
	}
	if (!CurrentUser) {
		SetFsError("Must login first");
		return(NULL);
	}
	if (Mode & ((FS_OWNER_READ|FS_OWNER_WRITE|FS_OTHER_READ|FS_OTHER_WRITE) ^ 0xFF))  {
		SetFsError("Invalid mode");
		return(0);
	}

	// make sure the file doesn't already exist
	if (FindFile(Filename)) {
		SetFsError("File already exists");
		return(NULL);
	}

	// look for a __free inode
	for (i = 0; i < FS->MaxFiles; i++) {
		if (FS->Inodes[i]) {
			continue;
		}

		// create the inode
		if ((FS->Inodes[i] = __calloc(sizeof(Inode))) == NULL) {
			SetFsError("__calloc failed");
			return(NULL);
		}

		// set the filename
		if ((FS->Inodes[i]->Filename = __calloc(__strlen(Filename)+1)) == NULL) {
			SetFsError("__calloc failed");
			__free(FS->Inodes[i]);
			FS->Inodes[i] = NULL;
			return(NULL);
		}
		__strcpy(FS->Inodes[i]->Filename, Filename);

		// set the owner
		if ((FS->Inodes[i]->Owner = __calloc(sizeof(CurrentUser)+1)) == NULL) {
			SetFsError("__calloc failed");
			__free(FS->Inodes[i]->Filename);
			FS->Inodes[i]->Filename = NULL;
			__free(FS->Inodes[i]);
			FS->Inodes[i] = NULL;
			return(NULL);
		}
		__strcpy(FS->Inodes[i]->Owner, CurrentUser);

		// set the Mode
		FS->Inodes[i]->Mode = Mode;

		// set the filesize and Data
		FS->Inodes[i]->FileSize = 0;
		FS->Inodes[i]->Data = NULL;

		break;
	}
	if (i == FS->MaxFiles) {
		SetFsError("No __free inodes");
		return(NULL);
	}

	ClearFsError();
	return(FS->Inodes[i]);
}

// Open a file for reading or writing
//  Mode: must be either 'r' or 'w'
//  Returns a file handle suitable for calls to __fread, __fwrite, fgets, __fclose
__FILE *__fopen(char *Filename, char *Mode) {
	uint32_t i;
	pInode TargetInode = NULL;
	__FILE *fp;

	// sanity checks
	if (!FS) {
		SetFsError("Filesystem does not exist");
		return(NULL);
	}
	if (!Filename || !Mode) {
		SetFsError("Invalid filename or mode");
		return(NULL);
	}
	if (__strlen(Mode) > 1) {
		SetFsError("Invalid mode");
		return(NULL);
	}
	if (Mode[0] != 'r' && Mode[0] != 'w') {
		SetFsError("Invalid mode");
		return(NULL);
	}
	if (!CurrentUser) {
		SetFsError("Must login first");
		return(NULL);
	}

	// search the filesystem for the filename
	TargetInode = FindFile(Filename);
	if (!TargetInode && Mode[0] == 'r') {
		SetFsError("Unable to locate file");
		return(NULL);
	}

	// If one was found, make sure the file isn't already open
	if (TargetInode) {
		for (i = 0; i < MAX_OPEN_FILES; i++) {
			if (FD[i]) {
				if (FD[i]->Inode == TargetInode) {
					// file already open
					SetFsError("File is already open");
					return(NULL);
				}
			}
		}
	}

	// check permissions
	// only check if we're not root and we're trying to create a new file
	if (TargetInode && __strcmp(CurrentUser, "root") != 0) {
		// are we the owner of the file?
		if (!__strcmp(TargetInode->Owner, CurrentUser)) {
			// check owner permissions
			if (Mode[0] == 'r' && ((TargetInode->Mode & FS_OWNER_READ) == 0)) {
				SetFsError("Permission denied");
				return(NULL);
			}
			if (Mode[0] == 'w' && ((TargetInode->Mode & FS_OWNER_WRITE) == 0)) {
				SetFsError("Permission denied");
				return(NULL);
			}
		} else {
			// check other permissions
			if (Mode[0] == 'r' && ((TargetInode->Mode & FS_OTHER_READ) == 0)) {
				SetFsError("Permission denied");
				return(NULL);
			}
			if (Mode[0] == 'w' && ((TargetInode->Mode & FS_OTHER_WRITE) == 0)) {
				SetFsError("Permission denied");
				return(NULL);
			}
		}
	}

	// allocate a __FILE record
	if ((fp = __calloc(sizeof(__FILE))) == NULL) {
		SetFsError("__calloc failed");
		return(NULL);
	}

	// find a file descriptor slot to hold the new __FILE record
	for (i = 0; i < MAX_OPEN_FILES; i++) {
		if (FD[i]) {
			continue;
		}
		FD[i] = fp;
		break;
	}
	if (i == MAX_OPEN_FILES) {
		// unable to find available file descriptor
		SetFsError("No __free file descriptors");
		__free(fp);
		return(NULL);
	}

	if (Mode[0] == 'w') {
		// open for writing
		if (!TargetInode) {
			// create a new file
			if ((TargetInode = CreateEmptyFile(Filename, FS_OWNER_READ|FS_OWNER_WRITE)) == NULL) {
				SetFsError("Failed to create file");
				FD[i] = NULL;
				__free(fp);
				return(NULL);
			}
			fp->Inode = TargetInode;
			fp->Mode = FS_WRITE;
			fp->CurrPosition = 0;
		} else {
			// truncate an existing file
			fp->Inode = TargetInode;
			fp->Mode = FS_WRITE;
			fp->CurrPosition = 0;

			fp->Inode->FileSize = 0;
			if (fp->Inode->Data) {
				__free(fp->Inode->Data);
				fp->Inode->Data = NULL;
			}
		}
	} else {
		// open for reading
		fp->Inode = TargetInode;
		fp->Mode = FS_READ;
		fp->CurrPosition = 0;

	}

	ClearFsError();
	return(fp);

}

// closes an open file
uint8_t __fclose(__FILE *fp) {
	uint8_t i;

	if (!fp) {
		SetFsError("Invalid file pointer");
		return(0);
	}

	// find the FD holding this __FILE pointer
	for (i = 0; i < MAX_OPEN_FILES; i++) {
		if (FD[i] == fp) {
			FD[i] = NULL;
		}
	}

	ClearFsError();
	__free(fp);
	return(1);

}
	
// reads the specified number of items of a particular size from the specified file
//   returns the number of bytes __read
uint32_t __fread(char *buf, uint32_t size, uint32_t nitems, __FILE *fp) {

	if (!buf || !fp) {
		SetFsError("Invalid buffer or file pointer");
		return(0);
	}
	if (!fp->Inode->Data) {
		SetFsError("End of file");
		return(0);
	}

	if (size*nitems > (fp->Inode->FileSize - fp->CurrPosition)) {
		__memcpy(buf, fp->Inode->Data + fp->CurrPosition, fp->Inode->FileSize - fp->CurrPosition);
		fp->CurrPosition += (fp->Inode->FileSize - fp->CurrPosition);
		ClearFsError();
		return(fp->Inode->FileSize - fp->CurrPosition);
	} else {
		__memcpy(buf, fp->Inode->Data + fp->CurrPosition, size*nitems);
		fp->CurrPosition += size*nitems;
		ClearFsError();
		return(size*nitems);
	}
}

// writes the specified number of items of a particular size to the specified file
//   returns the number of bytes written
uint32_t __fwrite(char *buf, uint32_t size, uint32_t nitems, __FILE *fp) {
	unsigned char *NewData;

	if (!buf || !fp) {
		SetFsError("Invalid buffer or file pointer");
		return(0);
	}

	// allocate space to hold the current data and the new data
	if ((NewData = __calloc(fp->Inode->FileSize + size*nitems)) == NULL) {
		SetFsError("__calloc failed");
		return(0);
	}

	// __write the current data to the new buffer
	if (fp->Inode->Data) {
		__memcpy(NewData, fp->Inode->Data, fp->Inode->FileSize);
	}

	// __write the new data to the new buffer
	__memcpy(NewData+fp->Inode->FileSize, buf, size*nitems);
	
	fp->Inode->FileSize += size*nitems;

	if (fp->Inode->Data) {
		__free(fp->Inode->Data);
	}
	fp->Inode->Data = NewData;

	ClearFsError();
	return(size*nitems);
}

// reads a line delimited by '\n' from the file
//   returns a pointer to the line or NULL on EoF or error
char *fgets(char *buf, uint32_t size, __FILE *fp) {
	uint32_t TotalBytes = 0;

	if (!buf) {
		SetFsError("Invalid buffer");
		return(NULL);
	}
	if (!fp) {
		SetFsError("Invalid file");
		return(NULL);
	}
	if (fp->CurrPosition == fp->Inode->FileSize) {
		SetFsError("End of file");
		return(NULL);
	}
	if (!fp->Inode->Data) {
		SetFsError("End of file");
		return(NULL);
	}

	// __read in one character at a time
	while ((fp->CurrPosition < fp->Inode->FileSize) && (TotalBytes < size-1)) {
		buf[TotalBytes++] = fp->Inode->Data[fp->CurrPosition++];
		// if the character is a newline, we're done
		if (fp->Inode->Data[fp->CurrPosition-1] == '\n') {
			break;
		}
	}
	buf[TotalBytes] = '\0';

	return(buf);
}

// List the files in the filesystem
//   like an 'ls'
uint8_t ListFiles(char **Buf) {
	uint32_t i;
	char Mode[5];
	uint32_t TotalLen;
	uint32_t NewLen;

	if (!FS) {
		SetFsError("Filesystem does not exist");
		return(0);
	}

	// if we've been passed a Buf pointer, then __write the list
	// to a newly allocated buffer rather than stdout
	// But first we need to calculate how much buffer we'll need
	if (Buf) {
		// listing header length
		TotalLen = 32+1+32+1+8+4+1;

		// length of each of the file lines
		for (i = 0; i < FS->MaxFiles; i++) {
			if (FS->Inodes[i] == NULL) {
				continue;
			}

			if ((NewLen = __strlen(FS->Inodes[i]->Filename)) < 32) {
				TotalLen += 32;
			} else {
				TotalLen += NewLen;
			}
			TotalLen++;
			if ((NewLen = __strlen(FS->Inodes[i]->Owner)) < 32) {
				TotalLen += 32;
			} else {
				TotalLen += NewLen;
			}
			TotalLen++;
			if (FS->Inodes[i]->FileSize == 0) {
				TotalLen += 8;
			} else {
				if ((NewLen = log10(FS->Inodes[i]->FileSize)+1) < 8) {
					TotalLen += 8;
				} else {
					TotalLen += NewLen;
				}
			}
			// 1 (space), 4 (mode), 1 (newline)
			TotalLen += 6;
		}

		// allocate the buffer
		if ((*Buf = __calloc(TotalLen)) == NULL) {
			SetFsError("__calloc failed");
			return(0);
		}
	}

	if (Buf) {
		__cgc_sprintf(*Buf, "$-32s $-32s $-8s $-4s\n", "Filename", "Owner", "Size", "Mode");
	} else {
		__printf("$-32s $-32s $-8s $-4s\n", "Filename", "Owner", "Size", "Mode");
	}
	for (i = 0; i < FS->MaxFiles; i++) {
		if (FS->Inodes[i] == NULL) {
			continue;
		}
		if (Buf) {
			__cgc_sprintf(*Buf, "$s$-32s $-32s $-8d ", 
				*Buf,
				FS->Inodes[i]->Filename,
				FS->Inodes[i]->Owner,
				FS->Inodes[i]->FileSize);
		} else {
			__printf("$-32s $-32s $-8d ", 
				FS->Inodes[i]->Filename,
				FS->Inodes[i]->Owner,
				FS->Inodes[i]->FileSize);
		}
		// print the mode
		__memset(Mode, '-', 5);	
		Mode[4] = '\0';
		if (FS->Inodes[i]->Mode & FS_OWNER_READ)
			Mode[0] = 'r';
		if (FS->Inodes[i]->Mode & FS_OWNER_WRITE)
			Mode[1] = 'w';
		if (FS->Inodes[i]->Mode & FS_OTHER_READ)
			Mode[2] = 'r';
		if (FS->Inodes[i]->Mode & FS_OTHER_WRITE)
			Mode[3] = 'w';
		if (Buf) {
			__cgc_sprintf(*Buf,"$s$-4s\n", *Buf, Mode);
		} else {
			__printf("$-4s\n", Mode);
		}
	}

	ClearFsError();
	return(1);

}

// Delete a file on the filesystem
uint8_t DeleteFile(char *Filename) {
	uint32_t i;
	uint32_t InodeIndex;
	pInode TargetInode;

	if (!FS) {
		SetFsError("Filesystem does not exist");
		return(0);
	}
	if (!Filename) {
		SetFsError("Invalid filename");
		return(0);
	}
	if (!CurrentUser) {
		SetFsError("Must login first");
		return(0);
	}

	for (i = 0; i < FS->MaxFiles; i++) {
		if (FS->Inodes[i] == NULL) {
			continue;
		}
		if (!__strcmp(FS->Inodes[i]->Filename, Filename)) {
			// found it
			TargetInode = FS->Inodes[i];
			InodeIndex = i;
			break;
		}
	}
	if (i == FS->MaxFiles) {
		SetFsError("Unable to locate file");
		return(0);
	}

	// make sure there are no file descriptors open to it
	for (i = 0; i < MAX_OPEN_FILES; i++) {
		if (FD[i]) {
			if (FD[i]->Inode == TargetInode) {
				// file is still open
				return(0);
			}
		}
	}

	// do we have permission to remove it?
	if (__strcmp(CurrentUser, "root") != 0 && (__strcmp(CurrentUser, TargetInode->Owner) != 0)) {
		// we're not root or the owner of the file, so no
		SetFsError("Permission denied");
		return(0);
	}
	
	// remove the inode vars
	__free(TargetInode->Filename);
	if (TargetInode->Owner) {
		__free(TargetInode->Owner);
	}
	if (TargetInode->Data) {
		__free(TargetInode->Data);
	}

	// remove the inode
	__free(TargetInode);
	FS->Inodes[InodeIndex] = NULL;

	ClearFsError();
	return(1);
}

// Rename a file 
uint8_t RenameFile(char *OldFilename, char *NewFilename) {
	uint32_t i;
	pInode SourceInode = NULL;
	char *TempFilename;

	if (!OldFilename || !NewFilename) {
		SetFsError("Invalid filename");
		return(0);
	}

	// find the old filename
	for (i = 0; i < FS->MaxFiles; i++) {
		if (FS->Inodes[i] && FS->Inodes[i]->Filename) {
			if (!__strcmp(FS->Inodes[i]->Filename, OldFilename)) {
				// found it
				SourceInode = FS->Inodes[i];
			}
			if (!__strcmp(FS->Inodes[i]->Filename, NewFilename)) {
				// the new filename already exists
				SetFsError("Destination file already exists");
				return(0);
			}
		}
	}
	if (!SourceInode) {
		SetFsError("Source file not found");
		return(0);
	}

	// Do we have permissions to rename the old file
	if (__strcmp(CurrentUser, "root") != 0 && (__strcmp(CurrentUser, SourceInode->Owner) != 0)) {
		// we're not root or the owner of the file, so no
		SetFsError("Permission denied");
		return(0);
	}

	// rename the file
	if ((TempFilename = __calloc(__strlen(NewFilename)+1)) == NULL) {
		SetFsError("__calloc failed");
		return(0);
	}	
	__strcpy(TempFilename, NewFilename);
	__free(SourceInode->Filename);
	SourceInode->Filename = TempFilename;

	ClearFsError();
	return(1);
}

// ChangeMode
uint8_t ChangeMode(char *Filename, uint8_t NewMode) {
	pInode TargetInode;

	if (!Filename) {
		SetFsError("Invalid user");
		return(0);
	}

	// check that the mode is valid
	if (NewMode & ((FS_OWNER_READ|FS_OWNER_WRITE|FS_OTHER_READ|FS_OTHER_WRITE) ^ 0xFF))  {
		SetFsError("Invalid mode");
		return(0);
	}

	// find the file
	if ((TargetInode = FindFile(Filename)) == NULL) {
		SetFsError("Unable to find file");
		return(0);
	}

	// make sure we're root or we're the owner
	if ((__strcmp(CurrentUser, "root") != 0) && (__strcmp(CurrentUser, TargetInode->Owner) != 0)) {
		SetFsError("Permission denied");
		return(0);
	}

	// change the mode
	TargetInode->Mode = NewMode;

	ClearFsError();	
	return(1);
}

// Change the owner of a file
//   Can only be done by 'root' user (root must be the currently logged in user)
uint8_t ChangeOwner(char *Filename, char *NewOwner) {
	pInode TargetInode;
	char *TempOwner;

	if (!Filename) {
		SetFsError("Invalid filename");
		return(0);
	}
	if (!NewOwner) {
		SetFsError("Invalid owner");
		return(0);
	}
	
	if (__strcmp(CurrentUser, "root") != 0) {
		SetFsError("Must be root");
		return(0);
	}

	if ((TargetInode = FindFile(Filename)) == NULL) {
		SetFsError("Unable to find file");
		return(0);
	}

	// make sure the NewOwner exists in the passwd file
	if (!UserExists(NewOwner)) {
		SetFsError("Invalid user");
		return(0);
	}

	// __calloc some space for the new owner name
	if ((TempOwner = __calloc(__strlen(NewOwner)+1)) == NULL) {
		SetFsError("__calloc failed");
		return(0);
	}
	__strcpy(TempOwner, NewOwner);

	// update the owner on the inode
	if (TargetInode->Owner) {
		__free(TargetInode->Owner);
	}
	TargetInode->Owner = TempOwner;

	ClearFsError();
	return(1);
}

//
// user functions
//

// Login
uint8_t Login(char *Username) {
	char *NewUsername;

	if (!Username) {
		SetFsError("Invalid username");
		return(0);
	}

	// copy the requested username 
	if ((NewUsername = __calloc(__strlen(Username)+1)) == NULL) {
		SetFsError("__calloc failed");
		return(0);
	}
	__strcpy(NewUsername, Username);

	// is anyone currently logged in
	if (CurrentUser) {
		__free(CurrentUser);
		CurrentUser = NULL;
	}

	// log in as the requested user
	CurrentUser = NewUsername;

	ClearFsError();
	return(1);
	
}

void Uid(void) {
	if (CurrentUser) {
		__puts(CurrentUser);
	}
}

// log the current user out (leaving no user logged in)
uint8_t Logout(void) {

	if (CurrentUser) {
		__free(CurrentUser);
	}

	CurrentUser = NULL;

	ClearFsError();
	return(1);
}

// init passwd file creating the 'root' user and setting its password
uint8_t InitPasswd(char *RootPassword) {
	__FILE *fp;

	if (!RootPassword) {
		SetFsError("Invalid root password");
		return(0);
	}

	// __fopen the passwd file
	if ((fp = __fopen("passwd", "w")) == NULL) {
		SetFsError("Unable to open passwd file");
		return(0);
	}

	// __write the root user
	__fwrite("root:", 5, 1, fp);
	__fwrite(RootPassword, __strlen(RootPassword), 1, fp);

	// close the file
	__fclose(fp);

	ClearFsError();
	return(1);

}

// checks if the specified Username exists in the passwd file
uint8_t UserExists(char *Username) {
	__FILE *in;
	char line[128];
	char *User;

	if (!Username) {
		SetFsError("Invalid username");
		return(0);
	}

	// open the passwd file
	if ((in = __fopen("passwd", "r")) == NULL) {
		SetFsError("Unable to open passwd file");
		return(0);
	}

	// __read in each line
	while (fgets(line, 127, in)) {
		// see if it's the target username
		if ((User = __strtok(line, ":")) == NULL) {
			SetFsError("Failed to parse passwd file");
			return(0);
		}
		if (!__strcmp(User, Username)) {
			// found it
			__fclose(in);
			return(1);
		}
	}

	// close the passwd file
	__fclose(in);

	ClearFsError();
	return(0);

}

// add a user to the passwd file
uint8_t AddUser(char *Username, char *Password) {
	__FILE *passwd;
	__FILE *newpasswd;
	char line[128];

	if (!Username) {
		SetFsError("Invalid username");
		return(0);
	}
	if (!Password) {
		SetFsError("Invalid password");
		return(0);
	}
	if (__strcmp(CurrentUser, "root") != 0) {
		SetFsError("Must be root");
		return(0);
	}
	if (__strlen(Username) > MAX_USER_LEN) {
		SetFsError("Invalid username");
		return(0);
	}
	if (__strlen(Password) > MAX_PASSWD_LEN) {
		SetFsError("Invalid password");
		return(0);
	}

	// make sure the username doesn't already exist
	if (UserExists(Username)) {
		SetFsError("User already exists");
		return(0);
	}

	// open the passwd file
	if ((passwd = __fopen("passwd", "r")) == NULL) {
		SetFsError("Unable to open passwd file");
		return(0);
	}

	// open the temp passwd file
	if ((newpasswd = __fopen("~passwd", "w")) == NULL) {
		SetFsError("Unable to open tmp passwd file");
		__fclose(passwd);
		return(0);
	}

	// __read in each line of the passwd file
	while (fgets(line, 127, passwd) != NULL) {
		// __write it out to the temp passwd file
		if (__fwrite(line, __strlen(line), 1, newpasswd) != __strlen(line)) {
			__fclose(passwd);
			__fclose(newpasswd);
			SetFsError("Unable to __write tmp passwd file");
			DeleteFile("~passwd");
			return(0);
		}
		if (line[__strlen(line)-1] != '\n') {
			if (__fwrite("\n", 1, 1, newpasswd) != 1) {
				__fclose(passwd);
				__fclose(newpasswd);
				SetFsError("Unable to __write tmp passwd file");
				DeleteFile("~passwd");
				return(0);
			}
		}
	}

	// __write the new passwd entry
	__cgc_sprintf(line, "$s:$s", Username, Password);
	__fwrite(line, __strlen(line), 1, newpasswd);

	// close the passwd file
	__fclose(passwd);

	// close the temp passwd file
	__fclose(newpasswd);

	// remove the passwd file
	DeleteFile("passwd");
	
	// move the temp passwd file to passwd
	RenameFile("~passwd", "passwd");

	ClearFsError();
	return(1);
}

// delete user
uint8_t DeleteUser(char *Username) {
	char line[128];
	char *User;
	uint8_t Found = 0;
	__FILE *passwd;
	__FILE *newpasswd;

	if (!Username) {
		SetFsError("Invalid username");
		return(0);
	}
	if (__strcmp(CurrentUser, "root") != 0) {
		SetFsError("Must be root");
		return(0);
	}
	if (!__strcmp(Username, "root")) {
		SetFsError("Can't delete root user");
		return(0);
	}

	// open the passwd file
	if ((passwd = __fopen("passwd", "r")) == NULL) {
		SetFsError("Unable to open passwd file");
		return(0);
	}

	// open the temp passwd file
	if ((newpasswd = __fopen("~passwd", "w")) == NULL) {
		SetFsError("Unable to open tmp passwd file");
		__fclose(passwd);
		return(0);
	}

	// __read in each line of the passwd file
	while (fgets(line, 127, passwd) != NULL) {
		// skip blank lines
		if (__strlen(line) == 0) {
			continue;
		}
		// __write it out to the temp passwd file
		// if it's not the user we're deleting
		if ((User = __strtok(line, ":")) == NULL) {
			SetFsError("Failed to parse passwd file");
			__fclose(passwd);
			__fclose(newpasswd);
			DeleteFile("~passwd");
			return(0);
		}
		if (!__strcmp(User, Username)) {
			// found it, so don't __write the user
			Found = 1;
			continue;
		}

		// restore the delimiter __strtok would have removed
		line[__strlen(User)] = ':';

		// __write the line to the temp passwd file
		if (__fwrite(line, __strlen(line), 1, newpasswd) != __strlen(line)) {
			__fclose(passwd);
			__fclose(newpasswd);
			SetFsError("Unable to __write tmp passwd file");
			DeleteFile("~passwd");
			return(0);
		}
	}

	// close the passwd file
	__fclose(passwd);

	// close the temp passwd file
	__fclose(newpasswd);

	if (!Found) {
		SetFsError("User not found");
		__fclose(passwd);
		__fclose(newpasswd);
		DeleteFile("~passwd");
		return(0);
	}

	// remove the passwd file
	DeleteFile("passwd");
	
	// move the temp passwd file to passwd
	RenameFile("~passwd", "passwd");

	ClearFsError();
	return(1);

}

// change user password
uint8_t ChangePasswd(char *Username, char *NewPasswd) {
	char line[128];
	char *User;
	uint8_t Found = 0;
	__FILE *passwd;
	__FILE *newpasswd;

	if (!Username) {
		SetFsError("Invalid username");
		return(0);
	}
	if (!NewPasswd) {
		SetFsError("Invalid password");
		return(0);
	}
	if (__strlen(NewPasswd) > MAX_PASSWD_LEN) {
		SetFsError("Invalid password");
		return(0);
	}

	if (__strcmp(CurrentUser, Username) != 0 && __strcmp(CurrentUser, "root") != 0) {
		SetFsError("Must be root or the user being changed");
		return(0);
	}

	// open the passwd file
	if ((passwd = __fopen("passwd", "r")) == NULL) {
		SetFsError("Unable to open passwd file");
		return(0);
	}

	// open the temp passwd file
	if ((newpasswd = __fopen("~passwd", "w")) == NULL) {
		SetFsError("Unable to open tmp passwd file");
		__fclose(passwd);
		return(0);
	}

	// __read in each line of the passwd file
	while (fgets(line, 127, passwd) != NULL) {
		// skip blank lines
		if (__strlen(line) == 0) {
			continue;
		}
		// __write it out to the temp passwd file
		// if it's not the user we're deleting
		if ((User = __strtok(line, ":")) == NULL) {
			SetFsError("Failed to parse passwd file");
			__fclose(passwd);
			__fclose(newpasswd);
			DeleteFile("~passwd");
			return(0);
		}
		if (!__strcmp(User, Username)) {
			// found it, __write the new passwd
			__cgc_sprintf(line, "$s:$s\n", Username, NewPasswd);
			if (__fwrite(line, __strlen(line), 1, newpasswd) != __strlen(line)) {
				__fclose(passwd);
				__fclose(newpasswd);
				SetFsError("Unable to __write tmp passwd file");
				DeleteFile("~passwd");
				return(0);
			}
			continue;
		}

		// restore the delimiter __strtok would have removed
		line[__strlen(User)] = ':';

		// restore the delimiter __strtok would have removed
		if (__fwrite(line, __strlen(line), 1, newpasswd) != __strlen(line)) {
			SetFsError("Unable to __write tmp passwd file");
			__fclose(passwd);
			__fclose(newpasswd);
			DeleteFile("~passwd");
			return(0);
		}
	}

	// close the passwd file
	__fclose(passwd);

	// close the temp passwd file
	__fclose(newpasswd);

	if (!Found) {
		SetFsError("User not found");
		__fclose(passwd);
		__fclose(newpasswd);
		DeleteFile("~passwd");
		return(0);
	}

	// remove the passwd file
	DeleteFile("passwd");
	
	// move the temp passwd file to passwd
	RenameFile("~passwd", "passwd");

	ClearFsError();
	return(1);

}

// check user password
uint8_t CheckPasswd(char *Username, char *Password) {
	char line[128];
	char *User;
	char *CurrPassword;
	uint8_t Found = 0;
	__FILE *passwd;
	char *OldUser = NULL;
	uint8_t NoLogin = 0;

	if (!Username) {
		SetFsError("Invalid username");
		return(0);
	}
	if (!Password) {
		SetFsError("Invalid password");
		return(0);
	}
	if (__strlen(Password) > MAX_PASSWD_LEN) {
		SetFsError("Invalid password");
		return(0);
	}

	// need to log in as root for the rest of this function
	// save the current logged in user
	if (CurrentUser) {
		if (__strcmp(CurrentUser, "root") != 0) {
			if ((OldUser = __calloc(__strlen(CurrentUser)+1)) == NULL) {
				SetFsError("__calloc failed");
				return(0);
			}
			__strcpy(OldUser, CurrentUser);
			Logout();
			Login("root");
		}
	} else {
		Login("root");
		NoLogin = 1;
	}

	// open the passwd file
	if ((passwd = __fopen("passwd", "r")) == NULL) {
		SetFsError("Unable to open passwd file");
		if (OldUser) {
			Logout();
			Login(OldUser);
			__free(OldUser);
		}
		if (NoLogin) {
			Logout();
		}
		return(0);
	}

	// __read in each line of the passwd file
	while (fgets(line, 127, passwd) != NULL) {
		// skip blank lines
		if (__strlen(line) == 0) {
			continue;
		}
		// parse the password field
		if ((User = __strtok(line, ":")) == NULL) {
			SetFsError("Failed to parse passwd file");
			__fclose(passwd);
			if (OldUser) {
				Logout();
				Login(OldUser);
				__free(OldUser);
			}
			if (NoLogin) {
				Logout();
			}
			return(0);
		}
		if (__strcmp(User, Username) != 0) {
			// not the target user, skip on
			continue;
		}

		// found the target user, parse the password
		if ((CurrPassword = __strtok(NULL, ":")) == NULL) {
			SetFsError("Failed to parse passwd file");
			__fclose(passwd);
			if (OldUser) {
				Logout();
				Login(OldUser);
				__free(OldUser);
			}
			if (NoLogin) {
				Logout();
			}
			return(0);
		}

		if (!__strcmp(CurrPassword, Password)) {
			// matches
			ClearFsError();
			__fclose(passwd);
			if (OldUser) {
				Logout();
				Login(OldUser);
				__free(OldUser);
			}
			if (NoLogin) {
				Logout();
			}
			return(1);
		}

	}

	// close the passwd file
	__fclose(passwd);

	if (!Found) {
		SetFsError("User not found");
		if (OldUser) {
			Logout();
			Login(OldUser);
			__free(OldUser);
		}
		if (NoLogin) {
			Logout();
		}
		return(0);
	}

	ClearFsError();
	if (OldUser) {
		Logout();
		Login(OldUser);
		__free(OldUser);
	}
	if (NoLogin) {
		Logout();
	}
	return(0);

}
