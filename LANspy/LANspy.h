// LANspy.h : main header file for the PROJECT_NAME application

#pragma once

// CLANspyApp:
// See LANspy.cpp for the implementation of this class
//

class CLANspyApp : public CWinApp
{
public:
	CLANspyApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CLANspyApp theApp;