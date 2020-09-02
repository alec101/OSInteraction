#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif

bool readLine8(FILE *f, str8 *out_str);
bool readLine16(FILE *f, str16 *out_str);
bool readLine32(FILE *f, str32 *out_str);
bool readFile(FILE *f, uint8_t **out_buf);     // reads the file, from the current location. adds 7 terminators, in case it should be a utf32 file, for failsafe

bool secureRead8(const char *name, str8 *out_str);
bool secureRead16(const char *name, str16 *out_str);
bool secureRead32(const char *name, str32 *out_str);

int64_t fileSize(FILE *f);
int64_t fileRemain(FILE *f);

void getFileExt(const char *in_fn, str8 *out_ext);   // out_ext will contain the file extension
const char *pointFileExt(const char *in_fn);              // returns the start of the extension, pointing to the same string
int getFileNameLen(const char *in_fn);               // returns the file name length in bytes, stops when '.' or str teminator is found 
void getFileName(const char *in_f, str8 *out_name);  // return in [out_name] only the name of the file, without the extension








