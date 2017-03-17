#pragma once
#include "ThreadPool.h"

namespace LAN
{
///////////////////////////////////////////////////////////////////////////////////
//Model for IP address
///////////////////////////////////////////////////////////////////////////////////
	class IpAddressesModel
	{
	public:
		std::string ipAddress, hostName, macAddress;
		unsigned long subnetMask;
		short subnetMaskBitsLength;
		bool valid;
	};

///////////////////////////////////////////////////////////////////////////////////
//Base class for Searching through LAN
///////////////////////////////////////////////////////////////////////////////////
	class Traverse
	{
	public:

		virtual std::vector<LAN::IpAddressesModel> Search(Traverse* traverse)
		{
			return addresses;
		}

	protected:
		//Member functions

		void LAN::Traverse::SendICMP(IpAddressesModel* ipAddressModel);

		LAN::IpAddressesModel LAN::Traverse::GetAdaptersAddress();

		std::string LAN::Traverse::ResolveHostname(const std::string& ipAddress);

		std::string LAN::Traverse::SendArp(const std::string& str_ip);

		//Members

		std::vector<LAN::IpAddressesModel> addresses;
	};

	//Function for getting object of Trverse derived class. (Use it in Traverse* as polymorphism)
	LAN::Traverse* GetTraverseObject(int choice);
}

