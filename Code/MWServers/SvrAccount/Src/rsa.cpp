#include "stdafx.h"
#include "bigint.h"
#include "rsa.h"
//#include "rsa_if.h"
#include"stdlib.h"
#include"stdio.h"
#include <string.h>


#define     absc_trace(logType,fmt)
#define     absc_trace1(logType,fmt, p1)
#define     absc_trace2(logType,fmt, p1, p2)

#define MAC_KEYLEN         64
int dividend[MAC_KEYLEN+1];
int quotient[MAC_KEYLEN+1];
int remnant[MAC_KEYLEN+1];


static void absc_BigDigit_To_HexDigit(char*BigDigit,char*OutHexDigit);
static void  absc_HexStr_To_RSAStruct(int rsa_bits,unsigned char *rsa_exponent,unsigned char *rsa_modulus,R_RSA_PUBLIC_KEY* OutStructKey)
;

static  void absc_GetKey(const char* szPrivateKey,const char *szModKey,R_RSA_PUBLIC_KEY *pRsaInitKey)
{
   unsigned char InitKey[MAC_KEYLEN+1] = {0};
   unsigned char InitModKey[MAC_KEYLEN+1] = {0};

   absc_BigDigit_To_HexDigit((char*)szPrivateKey,(char*)InitKey);
   absc_BigDigit_To_HexDigit((char*)szModKey,(char*)InitModKey);

   memset(pRsaInitKey,0,sizeof(R_RSA_PUBLIC_KEY));
   absc_HexStr_To_RSAStruct(128,InitKey,InitModKey,pRsaInitKey);
}

int absc_htoi(char *hexStr)
{
	int hexdigit = 0;
	int i = 0;
	int n = 0;

	for(i=0; i<2; ++i)
	{
		if(hexStr[i] >= '0' && hexStr[i] <= '9')
		{
			hexdigit = hexStr[i] - '0';
		}
		else if(hexStr[i] >= 'a' && hexStr[i] <= 'f')
		{
			hexdigit = hexStr[i] - 'a' + 10;
		}
		else if(hexStr[i] >= 'A' && hexStr[i] <= 'F')
		{
			hexdigit = hexStr[i] - 'A' + 10;
		}
		else
		{
		   return -1;
		}

		n = n * 16 + hexdigit;

	}

	return n;

}


void hex2byte(char *inBuf,int inLen,char *sOut) 
{
	int i=0,j=0;
	int flag=0;
	char *p=NULL;
	
	p=(char *)malloc(inLen+2);
	memset(p,0,inLen+2);
	if(inLen%2!=0)
	{
		*p='0';
		strcat(p,inBuf);
		inLen++;
	}
	else
	{
		strcpy(p,inBuf);
	}
	
	for(; i<inLen; i++)
	{
		if(p[i]>='0' && p[i]<='9')
			p[i]=p[i]-'0';
		else if(p[i]>='a' && p[i]<='f')
			p[i]=p[i]-'a'+10;
		else
		{
			sOut[j++]= p[i];
			continue;
		}
		
		flag++;
		if(flag%2==0)
		{
			sOut[j++]=p[i-1]*16+p[i];
		}
	}
	sOut[j]=0;
	
	free(p);
} 


void absc_change(char *str)
{
 
    int i = 0;
    int len = strlen(str);

    dividend[0] = len;
    for(i=1;i<= len;i++)
    {
        if(str[i-1] >= '0' && str[i-1] <= '9')
        {
            dividend[i] = str[i-1] - '0';
        }
		else if(str[i-1] >= 'a' && str[i-1] <= 'z')
		{
		 	dividend[i] = str[i-1] - 'a'+10;
		}
		else if(str[i-1] >= 'A' && str[i-1] <= 'Z')
		{
		 	dividend[i] = str[i-1] - 'Z'+10;
		}
		else
		{
		  return;
		}
    }
}

void absc_solve(int oldBase,int newBase)
{
    int y = 0,i = 0,j = 0;

    memset(remnant,0,sizeof(remnant));

    while(dividend[0] >= 1)
    {
        y=0;
        i=1;
        quotient[0]=dividend[0];

        while(i <= dividend[0])
        {
            y = y * oldBase + dividend[i];
            quotient[i++] = y/newBase;
            y %= newBase;
        }
        remnant[++remnant[0]] = y;
        i = 1;
        while((i<=quotient[0]) && (quotient[i]==0)) i++;
        memset(dividend,0,sizeof(dividend));
        for(j = i;j <= quotient[0];j++)
            dividend[++dividend[0]] = quotient[j];
        memset(quotient,0,sizeof(quotient));
    }
}

void absc_output(char *OutHexDigit)
{
    int i = 0;
	int j = 0;

    for(i = remnant[0];i >= 1;--i,j++)
    {
		sprintf(OutHexDigit+j,"%x",remnant[i]);
    }
}

void absc_BigDigit_To_HexDigit(char*BigDigit,char*OutHexDigit)
{
    absc_trace1(ABSC_LOG_INFO,"[absc_BigDigit_To_HexDigit] BigDigit = %s",BigDigit);
    absc_change(BigDigit);
	absc_solve(10,16);
	absc_output(OutHexDigit);

    absc_trace1(ABSC_LOG_INFO,"[absc_BigDigit_To_HexDigit] OutHexDigit = %s",OutHexDigit);
}

void absc_HexDigit_To_BigDigit(char*HexDigit,char*OutBigDigit)
{
    absc_trace1(ABSC_LOG_INFO,"[absc_HexDigit_To_BigDigit] HexDigit = %s",HexDigit);
    absc_change(HexDigit);
	absc_solve(16,10);
	absc_output(OutBigDigit);

    absc_trace1(ABSC_LOG_INFO,"[absc_HexDigit_To_BigDigit] OutBigDigit = %s",OutBigDigit);

}

void  absc_HexStr_To_RSAStruct(int rsa_bits,unsigned char *rsa_exponent,unsigned char *rsa_modulus,R_RSA_PUBLIC_KEY* OutStructKey)
{
  int len = 0;
  int i = 127,j = 0;
  char temp[3]= {0};

  memset(OutStructKey, 0, sizeof(OutStructKey));
  OutStructKey->bits = rsa_bits;
 // absc_trace1(ABSC_LOG_INFO,"[absc_HexStr_To_RSAStruct] OutStructKey->bits = %d", OutStructKey->bits);

  len = strlen((const char*)rsa_exponent);
  if(len%2 !=0)
  {
    len++;
  }
  for(i=128-(len/2),j=0;i<=127;i++)
  {
     if(strlen((const char*)rsa_exponent)%2 != 0 && j == 0)
     {
       strncpy(temp, "0",1);
       strncat(temp, (const char*)rsa_exponent,1);
	   j++;
	 }
	 else
	 {
     	strncpy(temp, (const char*)rsa_exponent+j,2);
		j+=2;
	 }

     OutStructKey->exponent[i] =absc_htoi(temp);
	 memset(temp, 0, sizeof(temp));

	// absc_trace2(ABSC_LOG_INFO,"[absc_HexStr_To_RSAStruct] OutStructKey->exponent[%d] = %x",i,OutStructKey->exponent[i]);
  }


  len = strlen((const char*)rsa_modulus);
  if(len%2 !=0)
  {
    len++;
  }
  for(i=128-(len/2),j=0;i<=127;i++)
  {
  	 if(strlen((const char*)rsa_modulus)%2 != 0 && j == 0)
     {
       strncpy(temp, "0",1);
       strncat(temp, (const char*)rsa_modulus,1);
	   j++;
	 }
	 else
	 {
     	strncpy(temp, (const char*)rsa_modulus+j,2);
		j+=2;
	 }
     OutStructKey->modulus[i] =absc_htoi(temp);
	 memset(temp, 0, sizeof(temp));
  }
}

void absc_RSAStruct_To_HexStr(unsigned char *rsa_str,char* OutStr)
{
  int i = 0;
  int iValue = 0;
  int j = 0;
  char TempBuf[MAC_KEYLEN+1] ={0};

  while(rsa_str[i] == 0)
  {
    i++;
  }

  iValue = i;

  for(i=iValue;i<=127;i++,j+=2)
  {
    sprintf(TempBuf+j,"%02x",rsa_str[i]);
  }


  absc_HexDigit_To_BigDigit(TempBuf,OutStr);
}

char* gEncRSA(int *outputLen, char *input, int inputLen, R_RSA_PUBLIC_KEY *publicKey)
{
	int tn=0,tn1=0,rlen;
	int i=0,j=0,mod=0;
	NN_DIGIT c[MAX_NN_DIGITS], e[MAX_NN_DIGITS], m[MAX_NN_DIGITS],
		n[MAX_NN_DIGITS];
	unsigned int eDigits, nDigits;
	char *pinput=NULL;
	char *output=NULL;
	char sTemp[MAX_RSA_MODULUS_LEN]="";

	tn=(publicKey->bits+7)/8;
	tn1=tn-1;

	pinput=(char *)malloc(inputLen+tn1+1);
	memset(pinput,0,inputLen+tn1+1);
	strcpy(pinput,input);
	if((mod=inputLen%tn1)!=0)
	{
		for(i=0;i<tn1-mod;i++)
			pinput[inputLen+i]=' ';
		inputLen+=tn1-mod;
	}
	rlen=inputLen/tn1;

	output=(char *)malloc(rlen*(tn+1)*2+1);
	memset(output,0,rlen*(tn+1)*2+1);

	/* decode the required RSA function input data */
	NN_Decode(n, MAX_NN_DIGITS, publicKey->modulus, MAX_RSA_MODULUS_LEN);
	NN_Decode(e, MAX_NN_DIGITS, publicKey->exponent, MAX_RSA_MODULUS_LEN);

	nDigits = NN_Digits(n, MAX_NN_DIGITS);
	eDigits = NN_Digits(e, MAX_NN_DIGITS);

	*outputLen=0;
	for(i=0;i<rlen;i++)
	{
		NN_Decode(m, MAX_NN_DIGITS, (unsigned char *)pinput+i*tn1, tn1);

		/* Compute c = m^e mod n.  To perform actual RSA calc.*/
		NN_ModExp (c, m, e, eDigits, n, nDigits);

		/* encode output to standard form */
		memset(sTemp,0,sizeof(sTemp));
		NN_Encode ((unsigned char *)sTemp, tn, c, nDigits);

		byte2hex(sTemp,tn,output+*outputLen);
		strcat(output," ");
		*outputLen+=tn*2+1;

		/* Clear sensitive information. */
		memset((POINTER)c, 0, sizeof(c));
		memset((POINTER)m, 0, sizeof(m));
	}

	free(pinput);

	return output;
}

char* gDecRSA(int *outputLen, char *input, int inputLen, R_RSA_PUBLIC_KEY *publicKey)
{
	int tn=0,tn1=0,rlen;
	int i=0;
	NN_DIGIT c[MAX_NN_DIGITS], d[MAX_NN_DIGITS], m[MAX_NN_DIGITS],
		n[MAX_NN_DIGITS];
	unsigned int dDigits, nDigits;
	char *p=NULL;
	char *q=NULL;
	char *pinput=NULL;
	char *output=NULL;
	char *temp=NULL;
	int nlen=0;

	tn=(publicKey->bits+7)/8;
	tn1=tn-1;

	/* decode the required RSA function input data */
	NN_Decode(n, MAX_NN_DIGITS, publicKey->modulus, MAX_RSA_MODULUS_LEN);
	NN_Decode(d, MAX_NN_DIGITS, publicKey->exponent, MAX_RSA_MODULUS_LEN);

	nDigits = NN_Digits(n, MAX_NN_DIGITS);
	dDigits = NN_Digits(d, MAX_NN_DIGITS);

	output=(char *)malloc(inputLen*2+1);
	memset(output,0,inputLen*2+1);

	pinput=(char *)malloc(inputLen+1);
	memset(pinput,0,inputLen+1);
	strcpy(pinput,input);

	temp=(char *)malloc(tn*2+1);

	q=pinput;
	while(*q)
	{
		p=strchr(q,' ');
		if(p!=NULL)
		{
			*p=0;
			nlen=p-q;
		}
		else
		{
			nlen=strlen(q);
		}
		memset(temp,0,tn*2+1);
		hex2byte(q,nlen,temp);
		if(nlen%2!=0)
			nlen++;
		
		NN_Decode(c, MAX_NN_DIGITS, (unsigned char *)temp, nlen/2);

		/* Compute m = c^d mod n.  To perform actual RSA calc.*/
		NN_ModExp (m, c, d, dDigits, n, nDigits);

		/* encode output to standard form */
		NN_Encode ((unsigned char *)output+i*tn1, tn1, m, nDigits);

		if(p==NULL)
			break;
		q=p+1;
		i++;

		/* Clear sensitive information. */
		memset((POINTER)c, 0, sizeof(c));
		memset((POINTER)m, 0, sizeof(m));
	}

	*outputLen=strlen(output);
	while(output[*outputLen-1]==' ')
	{
		output[*outputLen-1]=0;
		(*outputLen)--;
	}

	free(pinput);
	free(temp);

	return output;
}

char *gIAppDecRSA(int *outputLen, char *input, int inputLen, const char *szPrivatekey,const char *szModKey)
{
	R_RSA_PUBLIC_KEY absc_cpPrivateKey;
	absc_GetKey(szPrivatekey,szModKey,&absc_cpPrivateKey);
	return  gDecRSA(outputLen,input,inputLen,&absc_cpPrivateKey);

}
char *gIAppEncRSA(int *outputLen, char *input, int inputLen, const char *szPrivatekey,const char *szModKey)
{
	R_RSA_PUBLIC_KEY absc_cpPrivateKey;
	absc_GetKey(szPrivatekey,szModKey,&absc_cpPrivateKey);
	return gEncRSA(outputLen,input,inputLen,&absc_cpPrivateKey);
}



