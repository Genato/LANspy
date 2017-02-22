#pragma once


class ViewModelLogic
{
public:
	CIPaddresses* IPaddress;

	ViewModelLogic();
	~ViewModelLogic();

	void Load(CListCtrl& listCtrlView);
	void Save();
	void Search(CString option);
	void Search(CString option, DWORD startDwAddress, DWORD endDwAddress);
};

