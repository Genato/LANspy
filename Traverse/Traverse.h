#pragma once
#include "ThreadPool.h"


namespace LAN
{
	class Traverse;

	class GlobalVariableHolder
	{
	public:
		LAN::Traverse* traverse;
		std::string ipAddress;
	};

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
		virtual std::map<std::string, LAN::IpAddressesModel> Search(Traverse* traverse)
		{
			return addressess;
		}

		void IncrementCallbackReplys();

		void InsertAddressModelIntoMap(std::string ipAddress);

	protected:
		//Member functions

		void LAN::Traverse::SendICMP(GlobalVariableHolder* pointerForCallbackMethod, std::string ipAddress);

		LAN::IpAddressesModel LAN::Traverse::GetAdaptersAddress();

		void LAN::Traverse::ResolveHostname(const std::string& ipAddress);

		void LAN::Traverse::SendArp(const std::string& str_ip);

		//Members

		std::vector<LAN::IpAddressesModel> addresses;
		std::map<std::string, LAN::IpAddressesModel> addressess;
		int callbackReplys = 0;
		std::mutex mtx;
	};

	//Function for getting object of Trverse derived class. (Use it in Traverse* as polymorphism)
	LAN::Traverse* GetTraverseObject(int choice);
}

