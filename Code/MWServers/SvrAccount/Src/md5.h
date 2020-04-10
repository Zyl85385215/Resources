/*
 *  absc_md5.h
 *  testC
 *
 *  Created by  lyong on 11-7-4.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

typedef unsigned int uint32;

typedef struct MD5Context {
	uint32 buf[4];
	uint32 bits[2];
	unsigned char in[64];
}MD5_CTX;

void absc_MD5_Hash
(
 unsigned char *pucData,
 unsigned long iDataLen,
 unsigned char *pDigest
 );

void absc_MD5Init(MD5_CTX *);
void absc_MD5Update(MD5_CTX *, unsigned char *, unsigned int);
void absc_MD5Final(unsigned char *, MD5_CTX *);
void gIAppMD5(char *pInput,int len,char *szOutput);