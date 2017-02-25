#pragma once


class ViewModelLogic
{
public:
	CIPaddresses* IPaddress;

	ViewModelLogic();
	~ViewModelLogic();

	void Load(CListCtrl& listCtrlView);
	void Save(CListCtrl& listCtrlView);
	void Search(CListCtrl&, CString option);
	void Search(CListCtrl&, CString option, DWORD startDwAddress, DWORD endDwAddress);
};

