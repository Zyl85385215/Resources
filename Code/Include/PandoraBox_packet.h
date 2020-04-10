#ifndef PANDORABOX_PACKET_H
#define PANDORABOX_PACKET_H

#pragma pack (push)
#pragma pack (1)

enum _PANDORABOX
{	
	PANDORABOX_OPEN,
	PANDORABOX_START,   // NPC�����b��	
	PANDORABOX_TIMEUP,  // ������Ҫ�r�g��
	PANDORABOX_RESULT,  // �Д�Y��
	PANDORABOX_END,     // �P�]����
	PANDORABOX_CANCEL,  // ȡ������
};

enum
{
	pandorabox_rlt_success,    // �ɹ�
	pandorabox_rlt_failed,     // ʧ��

	pandorabox_rlt_checkpass,  // �z��ͨ�^
	pandorabox_rlt_checkfailed // ��Ʒ����
};  

enum
{
	PANDORABOX_STATUS_NONE,
	PANDORABOX_STATUS_DOING,
	PANDORABOX_STATUS_END
};


struct PANDORABOX_PANDORABOX_START_Data 
{
	DWORD	dwIndex;				//�b��
};


struct PANDORABOX_PANDORABOX_RESULT_Data 
{		
	BYTE	bRlt;
};

#pragma pack (pop)
#endif