#pragma once
#include "..\Traverse\Traverse.h"
#include "IPaddresses.h"

class ViewModelLogic
{
public:
	CIPaddresses* IPaddress;
	LAN::Traverse* traverse;
	std::future<std::map<std::string, LAN::IpAddressesModel>> future;

	ViewModelLogic();
	~ViewModelLogic();

	//Function for loading LAN information from database
	void Load(CListCtrl& listCtrlView);

	//Function for saving LAN information to database
	void Save(CListCtrl& listCtrlView);

	//Overloaded function for searching through LAN (this function gets ThisPcInfo and ThisPcSubnet objects)
	void Search(int choice);

	//Overloaded function for searching through LAN (this function gets RangeOfAddresses object)
	void Search(DWORD startDwAddress, DWORD endDwAddress, int choice);

	//Function that checks for future results from Traverse, if result is back it writes it to list control
	bool GetTraverseResult(CListCtrl& listCtrlView);

	std::string ToStdString(CString str);
	CString ToCString(std::string str);
};

