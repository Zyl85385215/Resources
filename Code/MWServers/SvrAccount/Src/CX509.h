#pragma once

#define NULL					0

typedef int						BOOL;
#define TRUE					1
#define FALSE					0

#define	DEFAULT_KEY_SIZE		1024

#define KT_PUBLIC				0x00000000		//π´‘ø
#define KT_PRIVATE				0x00000002		//ÀΩ‘ø

#define EX_ENCRYPTION			0x00000000		//º”√‹
#define	EX_DECRYPTION			0x00000001		//Ω‚√‹

#define EX_PUBLICKEY_EN			(KT_PUBLIC + EX_ENCRYPTION)		//π´‘øº”√‹
#define EX_PUBLICKEY_DE			(KT_PUBLIC + EX_DECRYPTION)		//π´‘øΩ‚√‹
#define EX_PRIVATEKEY_EN		(KT_PRIVATE + EX_ENCRYPTION)	//ÀΩ‘øº”√‹
#define EX_PRIVATEKEY_DE		(KT_PRIVATE + EX_DECRYPTION)	//ÀΩ‘øΩ‚√‹


class CX509
{
private:
	unsigned int m_bits;

	unsigned int m_public_key_size;
	unsigned char *m_public_key;

	unsigned int m_private_key_size;
	unsigned char *m_private_key;

	typedef int (CX509::*LPFUNEX)(const unsigned char *key,int key_size,const unsigned char *from,int from_size,unsigned char *to,int to_size);

	struct tagOperator
	{
		LPFUNEX lpDoEnde;
		unsigned char **key;
		unsigned int *key_size;
	};

	tagOperator m_exOpr[4];

	int DoPublicKeyEncryption(const unsigned char *key,int key_size,const unsigned char *from,int from_size,unsigned char *to,int to_size);
	int DoPublicKeyDecryption(const unsigned char *key,int key_size,const unsigned char *from,int from_size,unsigned char *to,int to_size);
	int DoPrivateKeyEncryption(const unsigned char *key,int key_size,const unsigned char *from,int from_size,unsigned char *to,int to_size);
	int DoPrivateKeyDecryption(const unsigned char *key,int key_size,const unsigned char *from,int from_size,unsigned char *to,int to_size);

public:
	CX509(void);
	CX509(unsigned int key_size);
	~CX509(void);

	BOOL InitRsa();

	void SetPublicKey(const unsigned char *key,unsigned int key_size);
	void SetPrivateKey(const unsigned char *key,unsigned int key_size);

	unsigned int GetPublicKey(unsigned char **key);
	unsigned int GetPrivateKey(unsigned char **key);

	int DoEnde(const unsigned char *from,int from_size,unsigned char *to,int to_size,int nFlag);
	int DoEnde(const unsigned char *key,int key_size,const unsigned char *from,int from_size,unsigned char *to,int to_size,int nFlag);
};
