#pragma once

#include "GlobalDefine.h"

class BuffObject
{
public:
	BuffObject();
	~BuffObject();


	//基础效果对象
	int		m_nBuffMult;			//效果倍率(百分之),200为2倍
	//DWORD	m_dwWorkingFrame;		//下次作用的倒计时
};