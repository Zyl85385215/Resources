#ifndef PAGETRANS_PACKET_H
#define PAGETRANS_PACKET_H

#pragma pack (push)
#pragma pack (1)

enum _PAGETRANS
{	
	PAGETRANS_OPEN,
	PAGETRANS_START,   // NPC�����b��	
	PAGETRANS_TIMEUP,  // ������Ҫ�r�g��
	PAGETRANS_RESULT,  // �Д�Y��
	PAGETRANS_END,     // �P�]����
	PAGETRANS_CANCEL,  // ȡ������
};

enum
{
	pagetrans_rlt_success,    // �ɹ�
	pagetrans_rlt_failed,     // ʧ��

	pagetrans_rlt_checkpass,  // �z��ͨ�^
	pagetrans_rlt_checkfailed // ��Ʒ����
};  

enum
{
	PAGETRANS_STATUS_NONE,
	PAGETRANS_STATUS_DOING,
	PAGETRANS_STATUS_END
};


struct PAGETRANS_PAGETRANS_START_Data 
{
	DWORD	dwIndex[3];				//����
};


struct PAGETRANS_PAGETRANS_RESULT_Data 
{		
	BYTE	bRlt;
};

#pragma pack (pop)
#endif