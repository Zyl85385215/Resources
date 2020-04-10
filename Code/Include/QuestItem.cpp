#include "QuestItem.h"
#include "xnfile.H"


PLOADQUESTFUNC loadquestfromversion[QUEST_FILE_VERS_MAX-QUEST_FILE_VERS_ORI]=
{
	LOAD_ORIVERSION,
	LOAD_VER_1,
	LOAD_VER_2,
	LOAD_VER_3,
	LOAD_VER_4,
	LOAD_VER_5,
	LOAD_VER_6,
	LOAD_VER_7,
	LOAD_VER_8,
	LOAD_VER_9,
	LOAD_VER_10,
	LOAD_VER_11,
	LOAD_VER_12,
	LOAD_VER_13,
	LOAD_VER_14,
};

QuestConfig::QuestConfig() 
{
	memset(this,0,sizeof QuestConfig);
	m_bySex = 2;
}

QuestConfig & QuestConfig::operator =( QuestConfig & pItem ) 
{
	if (sNPC)	free(sNPC);sNPC=NULL;
	if (eNPC)	free(eNPC);eNPC=NULL;
	if (sNPCTalk)	free(sNPCTalk);sNPCTalk=NULL;
	if (eNPCTalk)	free(eNPCTalk);eNPCTalk=NULL;

	if (QuestTitle) free(QuestTitle);QuestTitle=NULL;
	if (Questlog) free(Questlog);Questlog=NULL;
	if (IncompleteTalk) free(IncompleteTalk);IncompleteTalk=NULL;
	if (szScriptOnGiveUp) free(szScriptOnGiveUp);szScriptOnGiveUp=NULL;	
	if (szScriptOnAcceptTask) free(szScriptOnAcceptTask);szScriptOnAcceptTask=NULL;	
	if (szScriptOnCompleteTask) free(szScriptOnCompleteTask);szScriptOnCompleteTask=NULL;	
	if (pGetFormNpc) free(pGetFormNpc); pGetFormNpc=NULL;
	memcpy(this,&pItem,sizeof QuestConfig);
	this->QuestTitle=strdup(pItem.QuestTitle);
	this->Questlog =strdup(pItem.Questlog);
	this->sNPC=strdup(pItem.sNPC);
	this->sNPCTalk=strdup(pItem.sNPCTalk);
	this->eNPC=strdup(pItem.eNPC);
	this->eNPCTalk=strdup(pItem.eNPCTalk);
	this->IncompleteTalk=strdup(pItem.IncompleteTalk);
	this->szSimpleDesc = strdup(pItem.szSimpleDesc);
	this->szScriptOnGiveUp = strdup(pItem.szScriptOnGiveUp);
	this->szScriptOnAcceptTask = strdup(pItem.szScriptOnAcceptTask);
	this->szScriptOnCompleteTask = strdup(pItem.szScriptOnCompleteTask);
	this->pGetFormNpc = strdup(pItem.pGetFormNpc);
	return *this;

}
QuestConfig::~QuestConfig() 
{
	if (sNPC)	free(sNPC);
	if (eNPC)	free(eNPC);
	if (sNPCTalk) free(sNPCTalk);
	if (eNPCTalk) free(eNPCTalk);

	if (QuestTitle) free(QuestTitle);
	if (Questlog) free(Questlog);
	if (IncompleteTalk) free(IncompleteTalk);
	if(szSimpleDesc) free(szSimpleDesc);
	if(szScriptOnGiveUp) free(szScriptOnGiveUp);
	if (pGetFormNpc) free(pGetFormNpc);
	if(szScriptOnAcceptTask)	free(szScriptOnAcceptTask);
	if(szScriptOnCompleteTask) free(szScriptOnCompleteTask);
}

void QuestConfig::SaveToFile( FILE * pFile ) 
{
	WriteData(type);	
	WriteData(minlevel);//the lowest level to accept quest,
	WriteData(jobflag);//the job require
	WriteData(stateflag);//must in the state to accept quest,
	WriteData(lpContry);//contry require
	WriteData(dwCountDown);

	WriteData(lpOptionQ);	//must complete one of 
	WriteData(lpMustQ);		//must complete all 
	WriteData(QID);
	Writestring(sNPC);
	Writestring(eNPC);
	Writestring(QuestTitle);
	Writestring(Questlog);
	WriteData(lpItemRequire);
	WriteData(expAdd);
	WriteData(moneyAdd);
	WriteData(lpItemOpt);	//the items player could only select one
	WriteData(lpItemMust);	//the items will all give player 

	WriteData(NextQID);
	WriteData(bIsRandomID);
	WriteData(RandomID);
	Writestring(sNPCTalk);
	Writestring(eNPCTalk);
	WriteData(lpMonsterRequire);
	WriteData(pGuardInfo);
	WriteData(pChgDrop);
	Writestring(IncompleteTalk);
	WriteData(lpQuestItems);
	WriteData(bCanRepeat);
	WriteData(nKind);
	Writestring(szSimpleDesc);
	WriteData(maxlevel);//the highest level to accept quest,
	WriteData(m_bySex);
	WriteData(m_nBagNeed);
	WriteData(bsNPCNotShow);
	WriteData(beNPCNotShow);
	WriteData(jobexpAdd);
	Writestring(szScriptOnGiveUp);
	WriteData(completelevel);
	WriteData(wGutType);
	WriteData(nAcceptNeedGold);  
	WriteData(sNPCID);
	WriteData(eNPCID);
	//WriteData(QuestEnemy);

	//WriteData(honourAdd);
	//WriteData(moneyBangAdd);
	//WriteData(lpMustFrd);
	Writestring(pGetFormNpc);
}
int QuestConfig::LoadFromFile( BYTE * Data ) 
{
	int readpos=0;
	LoadData(type);

	LoadData(minlevel);//the lowest level to accept quest,

	LoadData(jobflag);//the job require
	LoadData(stateflag);//must in the state to accept quest,
	LoadData(lpContry);//contry require
	LoadData(dwCountDown);

	LoadData(lpOptionQ);	//must complete one of 
	LoadData(lpMustQ);		//must complete all 
	LoadData(QID);
	Loadstring(sNPC);
	Loadstring(eNPC);
	Loadstring(QuestTitle);
	Loadstring(Questlog);
	LoadData(lpItemRequire);
	LoadData(expAdd);
	LoadData(moneyAdd);
	LoadData(lpItemOpt);	//the items player could only select one
	LoadData(lpItemMust);	//the items will all give player 
	//Loadstring(pGetFormNpc);
	return readpos;
}
int QuestConfig::LoadAllQuestItem( char * filename,LPQUESTITEM*& pResult ) 
{
	int size=0;
	BYTE* pData=(BYTE *)xnFileRead(filename,&size);
	if (!pData) return 0;
	DWORD fileversion=0;
	memcpy(&fileversion,pData,4);
	int rt=0;
	if (fileversion>QUEST_FILE_VERS_ORI && fileversion<QUEST_FILE_VERS_MAX)
		rt=loadquestfromversion[fileversion-QUEST_FILE_VERS_ORI](pData,size,pResult);
	else
		rt=loadquestfromversion[0](pData,size,pResult);
	free(pData);
	return rt;
}
BOOL QuestConfig::SaveAllQuest( char * curDir,LPQUESTITEM*& pResult,int count) 
{
	if (!curDir)
	{
		MessageBox(NULL,"����Ŀ䛿�","����",MB_OK);
	}

	char filename[MAX_PATH] = {0};
	sprintf(filename,"%s\\Questlist.txt",curDir);
	FILE* pFile = fopen(filename,"w+b");
	if(pFile == NULL)
	{
		char message[100];
		sprintf(message, "�����ļ�%sʧ��", filename);
		MessageBox(NULL,message,"����",MB_OK);
		return false;
	}
	DWORD v=QUEST_FILE_VERS;
	WriteData(v);
	WriteData(count);
	for (int i =0;i<count;i++)
	{
		pResult[i]->SaveToFile(pFile);
	}
	fclose(pFile);
	return true;
}

BOOL QuestConfig::SaveAllQuest_T(char * curDir,LPQUESTITEM*& pResult,int count) //ת��Ϊ�����ٱ���
{
	if (!curDir)
	{
		MessageBox(NULL,"����Ŀ䛿�","����",MB_OK);
	}

	char filename[MAX_PATH] = {0};
	sprintf(filename,"%s\\Questlist.txt",curDir);
	FILE* pFile = fopen(filename,"w+b");
	if(pFile == NULL)
	{
		char message[100];
		sprintf(message, "�����ļ�%sʧ��", filename);
		MessageBox(NULL,message,"����",MB_OK);
		return false;
	}
	DWORD v=QUEST_FILE_VERS;
	WriteData(v);
	WriteData(count);
	for (int i =0;i<count;i++)
	{
		QuestConfig* temp = pResult[i]->DeepCopy();
		temp->TransToT();
		if (!temp)
		{			
			continue;
		}
		temp->SaveToFile(pFile);
		delete temp;
	}
	fclose(pFile);
	return true;
}

char * __fastcall Gb2Big(char * sGb, int nLen) 
{
	char* pszGbt = NULL;		//Gb����ķ����ַ�
	char* pszGbs = NULL;		//Gb����ļ����ַ�
	wchar_t* wszUnicode = NULL; //Unicode������ַ�
	char* pszBig5 = NULL;		//Big5������ַ�
	int iLen = 0;				//��Ҫת�����ַ���
	pszGbs = sGb;				//������Ҫת�����ַ�����

	char * szResult = (char*)malloc(nLen);	//���ص��ַ���
	memset(szResult, 0, nLen);
	int nSumLen = 0;
	while(nSumLen < nLen)
	{
		//����ת�����ַ��� 
		iLen = MultiByteToWideChar (936, 0, pszGbs, -1, NULL,0) ; 
		//��pszGbt�����ڴ� 
		pszGbt = new char[iLen*2+1]; 
		//ת��Gb����嵽Gb�뷱�壬ʹ��API����LCMapString 
		LCMapString(0x0804, LCMAP_TRADITIONAL_CHINESE, pszGbs, -1, pszGbt, iLen*2);
		//��wszUnicode�����ڴ� 
		wszUnicode = new wchar_t[iLen+1]; 
		//ת��Gb�뵽Unicode�룬ʹ����API����MultiByteToWideChar 
		MultiByteToWideChar (936, 0, pszGbt, -1, wszUnicode,iLen); 
		//����ת�����ַ��� 
		iLen = WideCharToMultiByte (936, 0, (PWSTR) wszUnicode, -1, NULL,0, NULL, NULL);	// 950?
		//��pszBig5�����ڴ� 
		pszBig5 = new char[iLen+1]; 
		//ת��Unicode�뵽Big5�룬ʹ��API����WideCharToMultiByte 
		WideCharToMultiByte (936, 0, (PWSTR) wszUnicode, -1, pszBig5,iLen, NULL, NULL);
		//����Big5���ַ�
		memcpy(szResult+nSumLen, pszBig5, iLen);

		//�ͷ��ڴ� 
		delete [] wszUnicode; 
		delete [] pszGbt; 
		delete [] pszBig5;

		nSumLen += iLen;
		pszGbs += iLen;
	}
	return szResult;
}

QuestConfig* QuestConfig::DeepCopy()
{
	if (!this)
	{
		return NULL;
	}
	QuestConfig* pReturn = new QuestConfig;
	memcpy(pReturn,this,sizeof(QuestConfig));

	pReturn->eNPC = strdup(eNPC);
	pReturn->eNPCTalk = strdup(eNPCTalk);
	pReturn->IncompleteTalk = strdup(IncompleteTalk);
	pReturn->Questlog = strdup(Questlog);
	pReturn->QuestTitle = strdup(QuestTitle);
	pReturn->sNPC = strdup(sNPC);
	pReturn->sNPCTalk = strdup(sNPCTalk);
	pReturn->szSimpleDesc = strdup(szSimpleDesc);
	pReturn->szScriptOnGiveUp = strdup(szScriptOnGiveUp);
	pReturn->szScriptOnAcceptTask = strdup(szScriptOnAcceptTask);
	pReturn->szScriptOnCompleteTask = strdup(szScriptOnCompleteTask);
	return pReturn;
}

bool QuestConfig::TransToT()
{
	if (!this)
	{
		return false;
	}

	char* neweNPC = Gb2Big(eNPC,strlen(eNPC)+1);
	char* neweNPCTalk = Gb2Big(eNPCTalk,strlen(eNPCTalk)+1);
	char* newIncompleteTalk = Gb2Big(IncompleteTalk,strlen(IncompleteTalk)+1);
	char* newQuestlog = Gb2Big(Questlog,strlen(Questlog)+1);
	char* newQuestTitle = Gb2Big(QuestTitle,strlen(QuestTitle)+1);
	char* newsNPC = Gb2Big(sNPC,strlen(sNPC)+1);
	char* newsNPCTalk = Gb2Big(sNPCTalk,strlen(sNPCTalk)+1);
	char* newszSimpleDesc = Gb2Big(szSimpleDesc,strlen(szSimpleDesc)+1);

	strcpy(eNPC,neweNPC);
	strcpy(eNPCTalk,neweNPCTalk);
	strcpy(IncompleteTalk,newIncompleteTalk);
	strcpy(Questlog,newQuestlog);
	strcpy(QuestTitle,newQuestTitle);
	strcpy(sNPC,newsNPC);
	strcpy(sNPCTalk,newsNPCTalk);
	strcpy(szSimpleDesc,newszSimpleDesc);

	free(neweNPC);
	free(neweNPCTalk);
	free(newIncompleteTalk);
	free(newQuestlog);
	free(newQuestTitle);
	free(newsNPC);
	free(newsNPCTalk);
	free(newszSimpleDesc);

	return true;
}