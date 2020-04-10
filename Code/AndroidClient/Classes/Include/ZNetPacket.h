#pragma once

#include "ios_groupCmd.h"

#pragma pack (push)
#pragma pack (1)

#define ZPACKET_MARK	0x3412

struct ZPacket
{
	WORD	wMark;
	WORD	wSize;
	DWORD	dwKey;			//服务器之间是唯一识别, 客户端与网关作为加密字段
	BYTE	bCmdGroup;		// 类型一
	BYTE    bCmd;			// 类型二
	BYTE	data[1];		// 数据
};

//多线程处理包必须先new
//ZPakcet* PacketDup(ZPakcet * pPacket)
//{
//	if(pPacket == NULL) return NULL;
//	int memSize = pPacket->wSize + sizeof(ZPakcet)-1;
//	void * rp = malloc(memSize);
//	if (rp != NULL)
//		memcpy(rp, pPacket, memSize);
//	return (ZPakcet*)rp;
//}

#define	BuildPacketSize(packet, cmdGroup, cmd, buffer, dataSize)	\
	packet = (ZPacket *)buffer;                                      \
	packet->wSize = (dataSize);                                     \
	packet->wMark = ZPACKET_MARK;                                     \
	packet->bCmdGroup = cmdGroup;                                   \
	packet->bCmd = cmd

#define	BuildPacket(packet, cmdGroup, cmd, buffer)		        \
	BuildPacketSize(packet, cmdGroup, cmd, buffer, sizeof(cmdGroup##_##cmd##_Data))

#define	BuildPacketEx(packet, cmdGroup, cmd, buffer, bsize)		\
	ZPacket *	packet		= NULL;									\
	char buffer[bsize] = {0};									\
	BuildPacket(packet, cmdGroup, cmd, buffer)

#define SET_DATA(dataName, cmdGroup, cmd, packet)   \
	cmdGroup##_##cmd##_Data * dataName = (cmdGroup##_##cmd##_Data*)packet->data;


#pragma pack (pop)