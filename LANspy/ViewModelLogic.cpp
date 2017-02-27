#include "stdafx.h"


ViewModelLogic::ViewModelLogic() : IPaddress(new CIPaddresses()){}

ViewModelLogic::~ViewModelLogic() 
{
	delete IPaddress;
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

void ViewModelLogic::Search(CListCtrl&, CString option)
{

}

void ViewModelLogic::Search(CListCtrl&, CString option, DWORD startDwAddress, DWORD endDwAddress)
{

}
