
#ifndef _GL_PACKET_H_
#define _GL_PACKET_H_

#include "sys_packet.h"
#include "chat_packet.h"
#include "groupCmd.h"

#pragma pack (push)
#pragma pack (1)


#define PACKET_MARK             0x1234
#define	ENCRYPT_PACKET_MARK		0x3412
#define DEFAULT_SOCKET_KEY      0x4321
//#define NEW_ENEMY	
#ifdef NEW_ENEMY
#define ENEMY_H "enemynew.h"
#else
#define ENEMY_H "enemy.h"
#endif

#define NEW_SLAVE
#ifdef NEW_SLAVE
#define SLAVE_H "SlaveNew.h"
#else
#define SLAVE_H "Slave.h"
#endif


struct Packet
{
    WORD	wMark;          // dwMark = PACKET_MARK
	WORD	wSize;          // dwSize = sizeof the data
	WORD	wCRC;			// 16Bits crc
	BYTE	bCmdGroup;		// 类型一
    BYTE    bCmd;			// 类型二
	BYTE	data[2];
//	...
//	WORD	wKey;
};

#define	AllocPacketSize(packet, cmdGroup, cmd, dataSize)        \
	packet = (Packet *)malloc(sizeof(Packet) + (dataSize));     \
    if (packet != NULL)                                         \
    {                                                           \
    	packet->wMark = PACKET_MARK;							\
        packet->wSize = (dataSize);                             \
        packet->bCmdGroup = cmdGroup;                           \
        packet->bCmd = cmd;                                     \
    }

#define	AllocPacket(packet, cmdGroup, cmd, dataStruct)	            \
    AllocPacketSize(packet, cmdGroup, cmd, sizeof(dataStruct))

#define	BuildPacketSize(packet, cmdGroup, cmd, buffer, dataSize)	\
    packet = (Packet *)buffer;                                      \
    packet->wMark = PACKET_MARK;                                    \
    packet->wSize = (dataSize);                                     \
    packet->bCmdGroup = cmdGroup;                                   \
    packet->bCmd = cmd

#define	BuildPacket(packet, cmdGroup, cmd, buffer)		        \
    BuildPacketSize(packet, cmdGroup, cmd, buffer, sizeof(cmdGroup##_##cmd##_Data))

#define	BuildPacketEx(packet, cmdGroup, cmd, buffer, buffersize)		\
	Packet *	packet		= NULL;									\
	char buffer[buffersize] = {0};									\
	BuildPacket(packet, cmdGroup, cmd, buffer)

#define BuildCmdPacket(packet, cmdGroup, cmd, buffer)   \
    BuildPacketSize(packet, cmdGroup, cmd, buffer, 0)

#define BuildCmdPacketEx(packet, cmdGroup, cmd, buffer, buffersize)   \
	Packet *	packet		= NULL;									\
	char buffer[buffersize] = {0};									\
	BuildCmdPacket(packet, cmdGroup, cmd, buffer)

#define SetPacketSocketKey(pPacket, socketKey)                  \
    *((WORD*)&pPacket->data[pPacket->wSize]) = socketKey

#define SET_DATA(dataName, cmdGroup, cmd, packet)   \
	cmdGroup##_##cmd##_Data * dataName = (cmdGroup##_##cmd##_Data*)packet->data;


#define BuildPacket1(pack,cmdGroup,cmd,dataName)\
		Packet * pack=NULL;\
		char buffer_adfsadfasdf[1024]={0};\
    BuildPacketSize(pack, cmdGroup, cmd, buffer_adfsadfasdf, sizeof(cmdGroup##_##cmd##_Data));\
		SET_DATA(dataName, cmdGroup, cmd, pack)

template <class PTRTYPE,class KEYTYPE = int>
class MapIntToPtr
{
	typedef struct _Node
	{
		KEYTYPE key;
		PTRTYPE ptr;
		_Node * next,*pre;
		_Node()
		{
			memset(this,0,sizeof(*this));
		}
		~_Node()
		{

		}
	}*LPNODE;

protected:	
	LPNODE* m_pdata;
	int size;
	inline UINT GetHashKey(KEYTYPE nkey){return nkey%size;};
	inline int CompareKey(KEYTYPE nkey,KEYTYPE nkey1){return nkey-nkey1;};
public:
	int elecnt;
	MapIntToPtr(int sz=0xFF)
	{
		size=sz;elecnt=0;
		m_pdata=new LPNODE[sz];
		memset(m_pdata,0,sizeof(LPNODE)*sz);
	}
	void Clear()
	{
		for (int i=0;i<size;i++)
		{
			LPNODE pnd=m_pdata[i];
			while (pnd)
			{
				LPNODE pnn=pnd->next;
				delete pnd; pnd=pnn;
			}
		}
		for (i =0;i<size;i++)
		{
			m_pdata[i]=NULL;
		}
		elecnt=0;
	}
	~MapIntToPtr()
	{
		Clear();
		delete [] m_pdata;
	}

	void Add(KEYTYPE key,PTRTYPE ptr)
	{
		int indx=GetHashKey(key);
		if (indx<0) return;
		elecnt++;
		LPNODE newnode=new _Node();
		newnode->key=key;
		newnode->ptr=ptr;
		if(m_pdata[indx]==NULL)
		{
			m_pdata[indx]=newnode;
		}
		else 
		{
			LPNODE t=m_pdata[indx];
			while (t->next)
				t=t->next;
			t->next=newnode;
			newnode->pre=t;
		}
	}
	PTRTYPE Remove(KEYTYPE key)
	{
		PTRTYPE pData = NULL;
		int indx=GetHashKey(key);
		if (indx<0 || indx>size-1) return pData;
		LPNODE t=m_pdata[indx];
		while (t)
		{
			if (CompareKey(t->key,key)==0)
			{
				if (t->pre && t->next)
				{
					t->pre->next=t->next;
					t->next->pre=t->pre;
				}
				else if (t->pre)
				{
					t->pre->next=NULL;
				}
				else if (t->next)
				{
					m_pdata[indx]=t->next;
					m_pdata[indx]->pre=NULL;
				}
				else 
				{
					m_pdata[indx]=NULL;
				}
				pData = t->ptr;
				delete t;			
				elecnt--;
				break;
			}
			else
				t=t->next;
		}
		return pData;
	}
	PTRTYPE GetElement(KEYTYPE key)
	{
		int indx=GetHashKey(key);
		if (indx<0 || indx>size-1) return (PTRTYPE)(0);
		LPNODE t=m_pdata[indx];
		if (!t) return (PTRTYPE)(0);
		if (CompareKey(t->key,key)==0)
			return t->ptr;
		else 
		{
			t=t->next;
			while (t)
			{
				if (CompareKey(t->key,key)==0)
					return t->ptr;
				t=t->next;
			}
		}
		return (PTRTYPE)(0);
	}
	PTRTYPE operator [](KEYTYPE key)
	{
		return GetElement(key);
	}
	void Travel(void (*fun)(PTRTYPE ele,void * param),void * param=NULL)
	{
		for (int i=0;i<size;i++)
		{
			LPNODE pnd=m_pdata[i];
			while (pnd)
			{
				fun(pnd->ptr,param);
				pnd=pnd->next;
			}
		}
	}
};

template<> inline UINT MapIntToPtr<void*,char*>::GetHashKey(char* nkey)
{
	UINT nHash = 0;
	while (*nkey)
		nHash = (nHash<<5) + nHash + *nkey++;
	return nHash%size;
};

template<> inline int MapIntToPtr<void*,char*>::CompareKey(char * nkey,char* nkey1)
{
	return stricmp(nkey,nkey1);
};

template<> MapIntToPtr<void*,char*>::_Node::~_Node()
{
	if(key) free(key);
}

typedef MapIntToPtr<void*,char*> MapStrToPtr;

#pragma pack (pop)

#endif
