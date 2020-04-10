#ifdef USE_ANDROID
#include "com_mejoygame_rpgwar_JniCall.h"
#include "cocos2d.h"
#include "JniHelper.h"
#include "MainMenu.h"
#include "GlobalFunction.h"
using namespace cocos2d;
void Java_com_mejoygame_rpgwar_JniCall_OpenForm( JNIEnv*  env, jobject thiz, jstring szForm,bool bOpen )
{
	if (bOpen)
	{
		pMainInterface->GetDForm(const_cast<char*>(JniHelper::jstring2string(szForm).c_str()))->Open();
	}else{
		pMainInterface->GetDForm(const_cast<char*>(JniHelper::jstring2string(szForm).c_str()))->Close();
	}
}

void Java_com_mejoygame_rpgwar_JniCall_SetLoginStep( JNIEnv* env, jobject thiz,int nStep )
{
	g_pTPObj->nLoginStep = nStep;
}

void Java_com_mejoygame_rpgwar_JniCall_SetLoginMID( JNIEnv* env, jobject thiz,int nStep )
{
	g_pTPObj->nLoginMID = nStep;

}

void Java_com_mejoygame_rpgwar_JniCall_SetToken( JNIEnv* env, jobject thiz,jstring szForm )
{
	//if (g_pTPObj->szToken)
	//{
	//	free(g_pTPObj->szToken);
	//}
	g_pTPObj->szToken = strdup(JniHelper::jstring2string(szForm).c_str());
}

#endif
