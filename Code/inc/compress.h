#ifndef __COMPRESS_H__
#define __COMPRESS_H__

int LzariCompress(void *dst, void *src, int datasize);
int LzariDecompress(void *dst, void *src, int datasize);

int LzssCompress(void *dst, void *src, int datasize);
int LzssDecompress(void *dst, void *src, int datasize);

int LzoCompress(void *dst, void *src, int datasize);
int LzoDecompress(void *dst, void *src, int datasize);

#endif
