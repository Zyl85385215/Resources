#ifndef __LZARI_H__
#define __LZARI_H__

int LzariCompress(void *src, unsigned src_len, void *dst);
int LzariDecompress(void *src, unsigned src_len, void *dst);

#endif
