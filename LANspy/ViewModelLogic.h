#pragma once


class ViewModelLogic
{
public:
	CIPaddresses* IPaddress;

	ViewModelLogic();
	~ViewModelLogic();

	void Load(CListCtrl& listCtrlView);
	void Save();
};

