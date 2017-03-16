//Traverse.h

#pragma once

#undef UNICODE
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Ipexport.h>

typedef struct _IO_STATUS_BLOCK {
	union {
		NTSTATUS Status;
		PVOID    Pointer;
	};
	ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef
VOID (NTAPI *PIO_APC_ROUTINE) (
	IN PVOID ApcContext,
	IN PIO_STATUS_BLOCK IoStatusBlock,
	IN ULONG Reserved
	);

#define PIO_APC_ROUTINE_DEFINED
#include <Iphlpapi.h>
#include <icmpapi.h>
#include <Iptypes.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <bitset>
//#include <mutex> 
//#include <thread>
//#include <future>
//#include <chrono>
//#include "ThreadPool.h"
#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#if defined(_MSC_VER)
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")	
#endif


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

