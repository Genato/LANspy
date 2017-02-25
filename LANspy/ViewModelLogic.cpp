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

}

void ViewModelLogic::Search(CString option)
{

}

void ViewModelLogic::Search(CString option, DWORD startDwAddress, DWORD endDwAddress)
{

}
