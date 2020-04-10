#include "SlaveAttribConfig.h"
#include "xnExtstr.h"
#include <string>

CreateSingleton(CSlaveAttribConfig);

void CSlaveAttribConfig::Load( const char * filename )
{
	xnExtstr * pIni = xnExtstr::Import((char*)filename);
	if(pIni == NULL) return;
	char szText[128];
	pIni->SetSection("zizhi");
	for (int i=0;i<10;i++)
	{
		sprintf(szText,"%d",i);
		m_nZizhi[i] = atoi(pIni->ReadString(szText,"0"));
	}
	for (int i=0;i<5;i++)
	{
		sprintf(szText,"k%d",i+1);
		m_dK[i] = atof(pIni->ReadString(szText,"0"));
		sprintf(szText,"u%d",i+1);
		m_dU[i] = atof(pIni->ReadString(szText,"0"));
	}

	pIni->SetSection("jinhua");
	for (int i=0;i<10;i++)
	{
		sprintf(szText,"%d",i);
		m_nJinhua[i] = atoi(pIni->ReadString(szText,"0"));
	}
	for (int i=0;i<5;i++)
	{
		sprintf(szText,"t%d",i+1);
		m_dT[i] = atof(pIni->ReadString(szText,"0"));
	}
	m_dJinhuaCanshu = atof(pIni->ReadString("canshu","0"));
}