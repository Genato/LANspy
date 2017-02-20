#pragma once


class ViewModelLogic
{
public:
	CLANspyDlg* cLANspyDlg;
	CIPaddresses* IPaddress;

	ViewModelLogic();
	~ViewModelLogic();

	void Load(CListCtrl& listCtrlView);

};

