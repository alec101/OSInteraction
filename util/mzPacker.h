#pragma once

// IMPORTANT:
// - ~5% of times, mainly if using small buffers, the (de)compiler will not write data in ouput, but will process input
//   this means, that a check should be done to see if mzPacker::results.outFilled, HAS a value (it can be 0)


#define MZ_BUFFER_SIZE 524288 // 0.5MB // 1048576 // 1MB buffer for decomp/ comp

enum mzTarget {
  INT_BUFFER= 0,      // uses internal .5mb buffer for source/ output
  USR_BUFFER,         // preallocate user buffer - it's size must be specified in [src/outLen]
  STDIO_FILE,         // preopen the file, go to desired file location and pass stdio [FILE *] pointer
  STDIO_FULL_FILE     // pass a filename - this file will be opened and it's full contents will be used for the operation
};


class mzPacker {
  void *_partDecomp, *_partComp;
  void *_file, *_file2;
  size_t _nrBytes;
  uchar *_inBuffer, *_outBuffer, *_inUsr, *_outUsr;
  uchar *_pIn, *_pOut;
  int8 _inType, _outType;
  bool _bMoreIn, _bMoreOut, _bAllIn;
  size_t _bytesProcessed;
  size_t _inSize, _outSize, _inUsrSize, _outUsrSize;

public:

  int8 compressionLevel;
  void setCompressionLevel(char); /// 1- 10 ?
  void setDefaultCompressionLevel() { setCompressionLevel(6); }

  // use these to know how big an archive can get, to prealloc a buffer
  uint64 compressBound(uint64 srcSize);   /// max size of the compressed data
  uint64 decompressBound(uint64 srcSize); /// max size of the decompressed data

  // ---=== compress & decompress ===---
  /// basic functions; source and output buffers MUST handle the whole comp/decomp process
  /// make shure output buffer is big enough to handle the whole decompressed source
  bool compress(const void *src, uint64 srcLen, void *out, uint64 outLen);
  bool decompress(const void *src, uint64 srcLen, void *out, uint64 outLen);


  // startAdv(De)Comp: call this before each (de)compress process, to (re)set all values
  // [nrBytes]: number of bytes, in total to (de)compress; this is ignored when using STDIO_FULL_FILE as input, as it will process the whole file
  //
  //  srcType / outType for startAdv(de)Comp:
  /// INT_BUFFER:      [src/out] ignored                                   - [src/outLen] ignored       (using internal .5mb buffers)
  /// USR_BUFFER:      [src/out] pointer to the buffer                     - [src/outLen] buffer's size (using user specified buffers)
  /// STDIO_FILE:      [src/out] stdio FILE * pointer                      - [src/outLen] ignored       (using internal .5mb buffers)
  /// STDIO_FULL_FILE: [src/out] null-terminated string of the file's name - [src/outLen] ignored       (using internal .5mb buffers)

  // doAdv(de)comp: keep calling this function until it returns null - signifieing the process is done
  //                check [mzPack::err] to see if any error ocurred during the last call
  //                [chunkSize]- the maximum chunk to process, in bytes; by default is 0, meaning that it should process all it can;
  //                             when using only files, it will process everything, without stopping; for buffers, it will stop to ask for more data
  //                [return]- it will return a pointer to the decompressed data
  //                        - or 1 to signal that there is still work to be done
  //                        - or 0 or NULL, to signal that the WORK IS DONE
  //                        when working with files, it will return a bool (null or 1), when working with output pointer, it will return a pointer
  //                [retLen]- optional - how much output was filled during the last function call

  // advanced compression funcs

  bool startAdvComp(size_t nrBytes, mzTarget srcType, cvoid *src, size_t srcLen, mzTarget outType, cvoid *out, size_t outLen);
  void *doAdvComp(size_t chunkSize= 0, size_t *retLen= null);

  // advanced decompression funcs

  bool startAdvDecomp(size_t nrBytes, mzTarget srcType, cvoid *src, size_t srcLen, mzTarget outType, cvoid *out, size_t outLen);
  void *doAdvDecomp(size_t chunkSize= 0, size_t *retLen= null);

  // these 2 funcs work when using USR_BUFFERS only - after a buffer is full/fully processed, it can be changed

  void setSrc(void *p, size_t s) { _inUsr= (uchar *)p; _inUsrSize= s; }
  void setOut(void *p, size_t s) { _outUsr= (uchar *)p; _outUsrSize= s; }
  



  // compress / decompress RESULTS - check these vars after each compress / decompress operation
  struct _mzPackerResults {
    uint64 srcProcessed;      /// how many bytes were compressed/ decompressed in src buffer
    uint64 srcTotalProcessed; /// how much data was compressed / decompressed in total, after multiple comp/decomp operations
    uint64 srcRemaining;      /// how many bytes remaining to compress / decompress in src buffer
    bool srcFullyProcessed;   /// src buffer FULLY processed (compress / decompress)

    uint64 outFilled;         /// how much of the out buffer was filled after a compress/decompress operation
    uint64 outTotalFilled;    /// how much data was compressed / decompressed in total, after multiple comp/decomp operations
    uint64 outRemaining;      /// how many bytes remaining in out buffer
    bool outIsFull;           /// out buffer is FULL

    _mzPackerResults();
    void delData();
    void delPartialData();
  } results;

  // util funcs

  ulong crc32(ulong, const uint8 *, size_t);

  // error handling - if funcs do not return true, check err number / getErr in text

  uint err;
  cchar *getErr();

  // constructor / destructor

  mzPacker();
  ~mzPacker();
  void delData();

};




/* ERROR values ([err] var)

  multiple errors can be flagged in [err] << SUBJECT TO CHANGE, this might be an encuberance!!!!!!!!!!!!!!!!!!!

0000 = OK, no errors
0001 = failed (unknown reason)
0002 = source buffer error
0004 = output buffer error
0008 = func parameter error
000F = adler32 mismatch
0010 = more input needed
0020 = have more output data
0040 = file read error
0080 = file write error
00F0 = file open error
0100 = processed data overflow
0200 = 
0400 = 
0800 = 
0F00 = 




*/





