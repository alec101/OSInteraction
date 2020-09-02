#include "osinteraction.h"
#include "util/typeShortcuts.h"
#include "util/fileOp.h"


/*
void fileSkipWhitespace(FILE *f, bool *out_newLineSkipped) {
  uint8 c;
  int64 pos;
  while(!feof(f)) {
    pos= ftell(f);
    fread(&c, 1, 1, f);

  }
}


void fileReadWordOrWordsInQuotes(FILE *f, str8 *out_string) {
}
*/


bool readLine8(FILE *f, str8 *out_str) {
  if(out_str== null) return false;
  if(f== null) { out_str->delData(); return false; }

  int64 start= osi.ftell64(f);

  uint8 c;
  int64 size= 0;

  /// checkout how big the line is
  while(1) {
    if(!fread(&c, 1, 1, f)) break;
    size++;
    if((c== '\n') || (c== 0)) break;
  }

  /// nothing read? exit
  if(!size) { out_str->delData(); return false; }
 
  // read the text
  uint8 *p= new uint8[size+ 1];
  osi.fseek64(f, start, SEEK_SET);
  fread(p, 1, size, f);
  p[size]= 0;                           /// str terminator

  /// returned string
  out_str->delData();
  out_str->d= (char *)p;
  out_str->updateLen();

  return true;
}


bool readLine16(FILE *f, str16 *out_str) {
  if((f== null) || (out_str== null)) return false;

  int64 start= osi.ftell64(f);

  uint16 c;
  int64 size= 0;

  /// checkout how big the line is
  while(1) {
    if(!fread(&c, 2, 1, f)) break;
    size++;
    if((c== '\n') || (c== 0)) break;
  }

  /// nothing read? exit
  if(!size) { out_str->delData(); return false; }
 
  // read the text
  uint16 *p= new uint16[size+ 1];
  osi.fseek64(f, start, SEEK_SET);
  fread(p, 2, size, f);
  p[size]= 0;                           /// str terminator

  /// returned string
  out_str->delData();
  out_str->d= (char16 *)p;
  out_str->updateLen();

  return true;
}


bool readLine32(FILE *f, str32 *out_str) {
  if((f== null) || (out_str== null)) return false;

  int64 start= osi.ftell64(f);

  uint32 c;
  int64 size= 0;

  /// checkout how big the line is
  while(1) {
    if(!fread(&c, 4, 1, f)) break;
    size++;
    if((c== '\n') || (c== 0)) break;
  }

  /// nothing read? exit
  if(!size) { out_str->delData(); return false; }
 
  // read the text
  uint32 *p= new uint32[size+ 1];
  osi.fseek64(f, start, SEEK_SET);
  fread(p, 4, size, f);
  p[size]= 0;                           /// str terminator

  /// returned string
  out_str->delData();
  out_str->d= (char32 *)p;
  out_str->updateLen();
  return true;
}


bool readFile(FILE *f, uint8_t **out_buf) {
  if(!f) return false;
  int64 size= fileRemain(f);
  if(!size) { *out_buf= null; return false; }

  *out_buf= new uint8[size+ 7];
  if(fread(*out_buf, 1, size, f)!= size) {
    delete []*out_buf;
    fclose(f);
    return false;
  }

  for(int a= 6; a>= 0; a--)
    (*out_buf)[size+ a]= 0;

  return true;
}


bool secureRead8(const char *name, str8 *out_str) {
  if(!out_str) return false;
  FILE *f= fopen(name, "rb");
  if(!f) { error.console(str8().f("secureRead8: cannot open \"%s\"", (name!= null? name: "null"))); return false; }

  uint8 *buffer;
  if(readFile(f, &buffer)) {
    out_str->secureUTF8((cchar *)buffer);
    delete []buffer;
  } else
    return false;

  fclose(f);
  return true;
}


bool secureRead16(const char *name, str16 *out_str) {
  if(!out_str) return false;
  FILE *f= fopen(name, "rb");
  if(!f) { error.console(str8().f("secureRead16: cannot open \"%s\"", (name!= null? name: "null"))); return false; }

  uint8 *buffer;
  readFile(f, &buffer);
  out_str->secureUTF16((cchar16 *)buffer);
  delete []buffer;

  fclose(f);
  return true;
}


bool secureRead32(const char *name, str32 *out_str) {
  if(!out_str) return false;
  FILE *f= fopen(name, "rb");
  if(!f) { error.console(str8().f("secureRead32: cannot open \"%s\"", (name!= null? name: "null"))); return false; }

  uint8 *buffer;
  readFile(f, &buffer);
  out_str->secureUTF32((cchar32 *)buffer);
  delete []buffer;

  fclose(f);
  return true;
}


int64_t fileSize(FILE *f) {
  int64 pos= osi.ftell64(f);
  osi.fseek64(f, 0, SEEK_END);
  int64 size= osi.ftell64(f);
  osi.fseek64(f, pos, SEEK_SET);
  return size;
}


int64_t fileRemain(FILE *f) {
  int64 pos= osi.ftell64(f);
  osi.fseek64(f, 0, SEEK_END);
  int64 size= osi.ftell64(f)- pos;
  osi.fseek64(f, pos, SEEK_SET);
  return size;
}


void getFileExt(const char *in_file, str8 *out_ext) {
  *out_ext= pointFileExt(in_file);
}


// returns the start of the extension, pointing to the same string - therefore the file name length can be found with a simple substraction
inline const char *pointFileExt(const char *in_fn) {
  if(in_fn== null) return null;
  
  // go to end of string
  int8 *p= (int8 *)in_fn;       /// will hold the end of string
  while(*p)
    p++;

  // walk backwards, until '.' is found
  for(int8 *r= p- 1; r> (int8 *)in_fn; r--)
    if(*r== '.') return (cchar *)(r+ 1);

  // reached this point, then the file has no extension - return the end of the string
  return (cchar *)p;
}

// returns the file name length in bytes, stops when '.' or str teminator is found 
inline int getFileNameLen(const char *in_fn) {
  if(in_fn== null) return 0;
  int8 *p= (int8 *)pointFileExt(in_fn);
  if(p> (int8 *)in_fn)
    if(*(p- 1)== '.') p--;

  return (int)(p- (int8 *)in_fn);
}

// return in [out_name] only the name of the file, without the extension
void getFileName(const char *in_f, str8 *out_name) {
  out_name->delData();
  out_name->insertStr(in_f, getFileNameLen(in_f));
}


/*
void getFileExt(const char *in_file, str8 *out_ext) {
  *out_ext= "";
  
  int n= Str::strlen8(in_file)- 1;/// number of bytes in text, handy var
  if(n< 1) return;

  cint8 *p2= (cint8 *)in_file;    /// will walk the fileName
  int len= 0;                     /// will hold the number of bytes the extension has
  bool dotFound= false;           /// a '.' must be found in order to have a valid extension

  while(n> 0) {
    if(p2[n]== '.') { dotFound= true; break; }
    len++;
    n--;
  }
  if(!dotFound) return;

  out_ext->delData();
  out_ext->d= (char *)new int8[len+ 1];

  int8 *p1= (int8 *)out_ext->d;   /// will walk the output extension string
  p2+= n;
  
  while(len>= 0)                  /// 0 inclusive for terminator
    *p1++= *p2++, len--;

  out_ext->updateLen();
}
*/













