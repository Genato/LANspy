#pragma once


class ViewModelLogic
{
public:
	CIPaddresses* IPaddress;
	LAN::Traverse* traverse;
	std::future<std::vector<LAN::IpAddressesModel>> future;

	ViewModelLogic();
	~ViewModelLogic();

	void Load(CListCtrl& listCtrlView);
	void Save(CListCtrl& listCtrlView);
	void SearchThisPcInfo();
	void SearchThisPcSubnet();
	void SearchRangeOfAddre(DWORD startDwAddress, DWORD endDwAddress);
	bool GetTraverseResult(CListCtrl& listCtrlView);

	std::string ToStdString(CString str);
	CString ToCString(std::string str);
};

