
// VC剪切板.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CVC剪切板App: 
// 有关此类的实现，请参阅 VC剪切板.cpp
//

class CVC剪切板App : public CWinApp
{
public:
	CVC剪切板App();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CVC剪切板App theApp;