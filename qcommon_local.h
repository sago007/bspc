#pragma once

#ifdef __cplusplus
//extern "C" {
#endif

#ifndef va
char* va(const char *format, ...) __attribute__ ((format (printf, 1, 2)));
#endif
void Com_DPrintf( const char *fmt, ... ) __attribute__ ((format (printf, 1, 2)));
void Com_Error( int code, const char *fmt, ... ) __attribute__ ((noreturn, format(printf, 2, 3)));
void QDECL Com_Printf( const char *msg, ... ) __attribute__ ((format (printf, 1, 2)));

#ifdef __cplusplus
//}
#endif
