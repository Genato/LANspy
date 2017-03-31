#include "stdafx.h"
#include "ViewModelLogic.h"


ViewModelLogic::ViewModelLogic() : IPaddress(new CIPaddresses()){}

ViewModelLogic::~ViewModelLogic() 
{
	delete IPaddress;
	delete traverse;
}

void ViewModelLogic::Load(CListCtrl& listCtrlView) 
{
	if(!IPaddress->IsOpen())
		IPaddress->Open(CRecordset::dynamic, NULL, CRecordset::readOnly);

	listCtrlView.DeleteAllItems();

	while(!IPaddress->IsEOF())
	{
		int n = listCtrlView.GetItemCount();

		listCtrlView.InsertItem(n, IPaddress->m_IPaddress);
		listCtrlView.SetItemText(n, 1, IPaddress->m_Hostname);
		listCtrlView.SetItemText(n, 2, IPaddress->m_MACaddress);
		listCtrlView.SetItemText(n, 3, IPaddress->m_SSID);

		IPaddress->MoveNext();
	}

	IPaddress->Close();
}

void ViewModelLogic::Save(CListCtrl& listCtrlView)
{
	if (!IPaddress->IsOpen())
		IPaddress->Open(CRecordset::dynamic, NULL, CRecordset::dynaset);
	
	for (int i = 0; i < listCtrlView.GetItemCount(); ++i)
	{
		int flag = 0;
		while (!IPaddress->IsEOF())
		{
			if (IPaddress->m_IPaddress == listCtrlView.GetItemText(i, 0))
				++flag;

			IPaddress->MoveNext();
		}

		IPaddress->MoveFirst();

		if (!flag)
		{
			if (IPaddress->CanAppend())
			{
				IPaddress->AddNew();
				IPaddress->m_IPaddress = listCtrlView.GetItemText(i, 0);
				IPaddress->m_Hostname = listCtrlView.GetItemText(i, 1);
				IPaddress->m_MACaddress = listCtrlView.GetItemText(i, 2);
				IPaddress->m_SSID = listCtrlView.GetItemText(i, 3);
				IPaddress->Update();
			}
			else
			{
				CString tmpStr1, tmpStr2;

				if (!tmpStr1.LoadString(MESSAGEBOXRECORDSETUPDATE))
					tmpStr1 = "";
				if (!tmpStr2.LoadString(IDC_SAVE))
					tmpStr2 = "";

				listCtrlView.MessageBox(tmpStr2, tmpStr1, MB_ICONASTERISK);
			}
		}
	}

	IPaddress->Close();
	listCtrlView.DeleteAllItems();
}

void ViewModelLogic::Search(int choice)
{
	traverse = LAN::GetTraverseObject(choice);

	auto fun = std::bind(&LAN::Traverse::Search, traverse, std::placeholders::_1);
	future = std::async(std::launch::async, fun, traverse);
}

void ViewModelLogic::Search(DWORD startDwAddress, DWORD endDwAddress, int choice)
{

}

bool ViewModelLogic::GetTraverseResult(CListCtrl& listCtrlView)
{
	bool futureResults = false;

	if (future._Is_ready() && future.valid())
	{
		std::map<std::string, LAN::IpAddressesModel> tmpMap;

		tmpMap = future.get();
		futureResults = true;
		listCtrlView.DeleteAllItems();

		for (std::map<std::string, LAN::IpAddressesModel>::iterator it = tmpMap.begin(); it != tmpMap.end(); ++it)
		{
			int n = listCtrlView.GetItemCount();

			listCtrlView.InsertItem(n, ToCString(it->first));
			listCtrlView.SetItemText(n, 1, ToCString(it->second.hostName));
			listCtrlView.SetItemText(n, 2, ToCString(it->second.macAddress));
		}
	}

	return futureResults;
}

//
//Helper methods
//

std::string ViewModelLogic::ToStdString(CString str)
{
	return std::string(CW2A(str.GetString()));
}

CString ViewModelLogic::ToCString(std::string str)
{
	return CString(str.c_str());
}
