/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
// cmdlib.h

#ifndef SIN
#define SIN
#endif //SIN

#ifndef __CMDLIB__
#define __CMDLIB__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>


#ifdef __cplusplus
#define EXTERNC extern "C"
#else 
#define EXTERNC 
#endif

#ifndef __BYTEBOOL__
#define __BYTEBOOL__
typedef enum {qfalse, qtrue} qboolean;
typedef unsigned char byte;
#endif


// the dec offsetof macro doesnt work very well...
#define myoffsetof(type,identifier) ((size_t)&((type *)0)->identifier)

#ifndef __GNUC__
#ifndef __attribute__
#define __attribute__(x)
#endif
#endif

// set these before calling CheckParm
extern int myargc;
extern char **myargv;

int Q_strncasecmp (const char *s1, const char *s2, int n);
int Q_strcasecmp (const char *s1, const char *s2);
void Q_strncpyz( char *dest, const char *src, int destsize );
void Q_getwd (char *out, size_t size);

int Q_filelength (FILE *f);

extern	char		qdir[1024];
extern	char		gamedir[1024];
void SetQdirFromPath (char *path);
char *ExpandArg (char *path);	// from cmd line
char *ExpandPath (char *path);	// from scripts
char *ExpandPathAndArchive (char *path);


double I_FloatTime (void);

EXTERNC void Error(const char *error, ...) __attribute__ ((format (printf, 1, 2))) __attribute__ ((noreturn));
void Warning(const char *warning, ...) __attribute__ ((format (printf, 1, 2)));

int		CheckParm (char *check);

FILE	*SafeOpenWrite (const char *filename);
FILE	*SafeOpenRead (const char *filename);
void	SafeRead (FILE *f, void *buffer, int count);
void	SafeWrite (FILE *f, void *buffer, int count);

int LoadFile (char *filename, void **bufferptr, int offset, int length);
int TryLoadFile (char *filename, void **bufferptr);
void SaveFile (char *filename, void *buffer, int count);
qboolean	FileExists (char *filename);

void 	DefaultExtension (char *path, char *extension);
void 	DefaultPath (char *path, char *basepath);
void 	StripFilename (char *path);
void 	StripExtension (char *path);

void 	ExtractFileBase (char *path, char *dest);
void	ExtractFileExtension (const char *path, char *dest);

int 	ParseNum (char *str);

short	BigShort (short l);
short	LittleShort (short l);
int		BigLong (int l);
int		LittleLong (int l);
float	BigFloat (float l);
float	LittleFloat (float l);

#ifdef SIN
unsigned short	BigUnsignedShort (unsigned short l);
unsigned short	LittleUnsignedShort (unsigned short l);
unsigned	      BigUnsigned (unsigned l);
unsigned	      LittleUnsigned (unsigned l);
#endif


char *COM_Parse (char *data);

extern	char		com_token[1024];
extern	qboolean	com_eof;

char *copystring(const char *s);


void CRC_Init(unsigned short *crcvalue);
void CRC_ProcessByte(unsigned short *crcvalue, byte data);
unsigned short CRC_Value(unsigned short crcvalue);

void	CreatePath (char *path);
void	QCopyFile (char *from, char *to);

extern	qboolean		archive;
extern	char			archivedir[1024];


extern	qboolean verbose;
void qprintf (const char *format, ...) __attribute__ ((format (printf, 1, 2)));

void ExpandWildcards (int *argc, char ***argv);


// for compression routines
typedef struct
{
	byte	*data;
	int		count;
} cblock_t;


#endif

