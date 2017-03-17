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
		if (IPaddress->m_IPaddress != listCtrlView.GetItemText(i, 0))
		{
			if (IPaddress->CanAppend())
			{
				IPaddress->AddNew();
				IPaddress->m_IPaddress = listCtrlView.GetItemText(i, 0);
				IPaddress->m_Hostname = listCtrlView.GetItemText(i, 1);
				IPaddress->m_MACaddress = listCtrlView.GetItemText(i, 2);
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

		IPaddress->MoveNext();
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
		std::vector<LAN::IpAddressesModel> tmpVec;

		tmpVec = future.get();
		futureResults = true;
		listCtrlView.DeleteAllItems();

		for (int i = 0; tmpVec.size() > 0; ++i)
		{
			listCtrlView.InsertItem(i, ToCString(tmpVec.back().ipAddress));
			listCtrlView.SetItemText(i, 1, ToCString(tmpVec.back().hostName));
			listCtrlView.SetItemText(i, 2, ToCString(tmpVec.back().macAddress));
			tmpVec.pop_back();
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
