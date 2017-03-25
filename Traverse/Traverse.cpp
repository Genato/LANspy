#include "..\LANspy\stdafx.h"
#include "Traverse.h"

//Callback method for async call of SendICMP() --> IcmpSendEcho2()
VOID NTAPI ReplyCame(PVOID context, PIO_STATUS_BLOCK pio, DWORD reserved)
{
	LAN::GlobalVariableHolder* pointerForCallbackFun = (LAN::GlobalVariableHolder*)context;

	pointerForCallbackFun->traverse->IncrementCallbackReplys();

	if (pio->Status == 0)
	{
		pointerForCallbackFun->traverse->InsertAddressModelIntoMap(pointerForCallbackFun->ipAddress);
	}

	CString str(pointerForCallbackFun->ipAddress.c_str());
	MessageBox(NULL, str, str, NULL);

	return;
}

///////////////////////////////////////////////////////////////////////////////////
//Traverse Member implemetation
///////////////////////////////////////////////////////////////////////////////////

LAN::IpAddressesModel LAN::Traverse::GetAdaptersAddress()
{
	IpAddressesModel ipAddressModel;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;
	ULONG flags = GAA_FLAG_INCLUDE_GATEWAYS;
	ULONG family = AF_INET;
	LPVOID lpMsgBuf = NULL;
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG outBufLen = 0;
	ULONG Iterations = 0;
	PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
	PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
	outBufLen = WORKING_BUFFER_SIZE;

	do {
		pAddresses = (IP_ADAPTER_ADDRESSES *)MALLOC(outBufLen);
		if (pAddresses == NULL)
		{
			ipAddressModel.valid = false;
			throw "GetAdaptersAddresses failed with error: Memory allocation failed for IP_ADAPTER_ADDRESSES struct";
		}

		dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

		if (dwRetVal == ERROR_BUFFER_OVERFLOW)
		{
			FREE(pAddresses);
			pAddresses = NULL;
		}
		else {
			break;
		}

		Iterations++;

	} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

	if (dwRetVal == NO_ERROR)
	{
		pCurrAddresses = pAddresses;
		while (pCurrAddresses)
		{
			if (pCurrAddresses->OperStatus == IF_OPER_STATUS::IfOperStatusUp)
			{
				//Parsing MAC address
				std::string mac_str = ParseMacAddrFromPIP_ADAPTER_ADDRESSES(pCurrAddresses);

				//Parsing Subnet info and Ip address
				std::string ip_str;
				unsigned long net_mask;
				pUnicast = pCurrAddresses->FirstUnicastAddress;
				short mask_bits_length = pUnicast->OnLinkPrefixLength;
				if (pUnicast != NULL)
				{
					for (int i = 0; pUnicast != NULL; i++)
					{
						net_mask = ParseSubnetInfoFromPIP_ADAPTER_UNICAST_ADDRESS(pUnicast);
						ip_str = ParseIpAddrFromPIP_ADAPTER_UNICAST_ADDRESS(pUnicast);
						pUnicast = pUnicast->Next;
					}
				}

				//Fill IpAddressesModel with this pc info 
				if (pCurrAddresses->FirstDnsServerAddress && pCurrAddresses->FirstGatewayAddress)
				{
					ipAddressModel.macAddress = mac_str;
					ipAddressModel.ipAddress = ip_str;
					ipAddressModel.subnetMaskBitsLength = mask_bits_length;
					ipAddressModel.subnetMask = net_mask;
					ipAddressModel.valid = true;
				}
			}
			pCurrAddresses = pCurrAddresses->Next;
		}
	}
	else
	{
		if (pAddresses)
			FREE(pAddresses);

		ipAddressModel.valid = false;
		throw "Call to GetAdaptersAddresses failed with error: No addresses were found for this machine";
	}

	if (pAddresses)
	{
		FREE(pAddresses);
	}

	return ipAddressModel;
}

void LAN::Traverse::SendICMP(GlobalVariableHolder* pointerForCallbackMethod, std::string ipAddress)
{
	HANDLE hIcmpFile;
	unsigned long ipaddr = INADDR_NONE;
	char * data = "hi all";
	char * ReplyBuffer = (char*)malloc(sizeof(ICMP_ECHO_REPLY) * 1000 + 8);
	DWORD ReplySize = 0;
	std::stringstream ss;

	if (ReplyBuffer == NULL)
		throw "SendICMP: Unable to allocate memory for reply buffer.";

	ipaddr = inet_addr(ipAddress.c_str());
	if (ipaddr == INADDR_NONE)
		throw "SendICMP: Invalind IP address.";

	hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE)
		throw "SendICMP: Unable to open handle.";

	DWORD dwRetVal = IcmpSendEcho2(hIcmpFile, 0, ReplyCame, pointerForCallbackMethod, ipaddr,
		data, strlen(data), NULL, ReplyBuffer, (sizeof(ICMP_ECHO_REPLY) * 1000) + 8, 1000);

	free(ReplyBuffer);
}

void LAN::Traverse::ResolveHostname(const std::string& ipAddress)
{
	WSADATA wsaData = { 0 };
	int iResult = 0;

	DWORD dwRetval;

	struct sockaddr_in saGNI;
	char hostname[NI_MAXHOST];
	char servInfo[NI_MAXSERV];
	u_short port = 27015;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0)
		throw "ResolveHostname: WSAStartup failed to initialize.";

	saGNI.sin_family = AF_INET;
	saGNI.sin_addr.s_addr = inet_addr(ipAddress.c_str());
	saGNI.sin_port = htons(port);

	dwRetval = getnameinfo((struct sockaddr *) &saGNI, sizeof(struct sockaddr), hostname, NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);

	if (dwRetval != 0)
		throw "ResolveHostname: ResolveHostname failed.";

	addressess[ipAddress].hostName = hostname;
}

void LAN::Traverse::SendArp(const std::string& str_ip)
{
	DWORD dwRetVal;
	IPAddr DestIp = 0;
	ULONG MacAddr[2];   
	ULONG PhysAddrLen = 6; 
	std::stringstream ss;

	BYTE *bPhysAddr;

	DestIp = inet_addr(str_ip.c_str());

	memset(&MacAddr, 0xff, sizeof(MacAddr));

	dwRetVal = SendARP(DestIp, NULL, &MacAddr, &PhysAddrLen);

	if (dwRetVal == NO_ERROR)
	{
		bPhysAddr = (BYTE *)& MacAddr;

		if (PhysAddrLen)
		{
			for (int i = 0; i < (int)PhysAddrLen; i++) {
				if (i == (PhysAddrLen - 1))
				{
					ss << std::setfill('0') << std::setw(2) << std::hex << (int)bPhysAddr[i];
				}
				else
				{
					ss << std::setfill('0') << std::setw(2) << std::hex << (int)bPhysAddr[i];
					ss << "-";
				}
			}
		}
		else
		{
			ss << "Warning: SendArp completed successfully, but returned length=0";
			throw ss.str().c_str();
		}
	}
	else
	{
		ss << "Arp failed: " << dwRetVal;

		switch (dwRetVal)
		{
		case ERROR_GEN_FAILURE:
			ss << " (ERROR_GEN_FAILURE)";
			break;
		case ERROR_INVALID_PARAMETER:
			ss << " (ERROR_INVALID_PARAMETER)";
			break;
		case ERROR_INVALID_USER_BUFFER:
			ss << " (ERROR_INVALID_USER_BUFFER)";
			break;
		case ERROR_BAD_NET_NAME:
			ss << " (ERROR_GEN_FAILURE)";
			break;
		case ERROR_BUFFER_OVERFLOW:
			ss << " (ERROR_BUFFER_OVERFLOW)";
			break;
		case ERROR_NOT_FOUND:
			ss << " (ERROR_NOT_FOUND)";
			break;
		default:
			break;
		}

		throw ss.str().c_str();
	}

	addressess[str_ip].macAddress = ss.str();
}

void LAN::Traverse::IncrementCallbackReplys()
{
	std::lock_guard<std::mutex> lock(mtx);

	lock;
	++callbackReplys;
}

void LAN::Traverse::InsertAddressModelIntoMap(std::string ipAddress)
{
	std::lock_guard<std::mutex> lock(mtx);

	lock;
	LAN::IpAddressesModel tmpIpAddreModel;
	addressess.insert(std::pair<std::string, LAN::IpAddressesModel>(ipAddress, tmpIpAddreModel));
}

std::string LAN::Traverse::ParseMacAddrFromPIP_ADAPTER_ADDRESSES(PIP_ADAPTER_ADDRESSES pCurrAddresses)
{
	std::string mac_str;
	std::stringstream ss;
	if (pCurrAddresses->PhysicalAddressLength != 0)
	{
		for (int i = 0; i < (int)pCurrAddresses->PhysicalAddressLength; i++)
		{
			if (i == (pCurrAddresses->PhysicalAddressLength - 1))
			{
				ss << std::setfill('0') << std::setw(2) << std::hex << (int)pCurrAddresses->PhysicalAddress[i];
			}
			else
			{
				ss << std::setfill('0') << std::setw(2) << std::hex << (int)pCurrAddresses->PhysicalAddress[i];
				ss << "-";
			}
		}
		mac_str = ss.str();
	}
	
	return mac_str;
}

unsigned long LAN::Traverse::ParseSubnetInfoFromPIP_ADAPTER_UNICAST_ADDRESS(PIP_ADAPTER_UNICAST_ADDRESS pUnicast)
{
	ULONG outBufLen = WORKING_BUFFER_SIZE;
	PULONG mask = &outBufLen;
	std::string ip_str;
	unsigned long net_mask;
	short mask_bits_length = pUnicast->OnLinkPrefixLength;

	ConvertLengthToIpv4Mask(pUnicast->OnLinkPrefixLength, mask);

	if (mask_bits_length >= 24 && mask_bits_length <= 32)
	{
		std::string binary = std::bitset<32>(*mask).to_string();
		net_mask = std::bitset<8>(binary.substr(0, 8)).to_ulong();
	}
	else if (mask_bits_length >= 16 && mask_bits_length <= 23)
	{
		std::string binary = std::bitset<24>(*mask).to_string();
		net_mask = std::bitset<8>(binary.substr(0, 8)).to_ulong();
	}
	else if (mask_bits_length >= 8 && mask_bits_length <= 15)
	{
		std::string binary = std::bitset<16>(*mask).to_string();
		net_mask = std::bitset<8>(binary.substr(0, 8)).to_ulong();
	}

	return net_mask;
}

std::string LAN::Traverse::ParseIpAddrFromPIP_ADAPTER_UNICAST_ADDRESS(PIP_ADAPTER_UNICAST_ADDRESS pUnicast)
{
	std::string ip_str;
	struct sockaddr_in  *sockaddr_ipv4;
	sockaddr_ipv4 = (struct sockaddr_in *)pUnicast->Address.lpSockaddr;
	ip_str = inet_ntoa(sockaddr_ipv4->sin_addr);

	return ip_str;
}

///////////////////////////////////////////////////////////////////////////////////
//Class for getting this pc LAN info
///////////////////////////////////////////////////////////////////////////////////
class ThisPcInfo : public LAN::Traverse
{
public:

	virtual std::map<std::string, LAN::IpAddressesModel> Search(LAN::Traverse* traverse) override
	{
		LAN::GlobalVariableHolder pointerForCallbackMethod;
		LAN::IpAddressesModel ipAddressModel;

		try
		{
			ipAddressModel = LAN::Traverse::GetAdaptersAddress();

			pointerForCallbackMethod.ipAddress = ipAddressModel.ipAddress;
			pointerForCallbackMethod.traverse = this;

			LAN::Traverse::SendICMP(&pointerForCallbackMethod, ipAddressModel.ipAddress);
			SleepEx(1100, true);

			LAN::Traverse::ResolveHostname(ipAddressModel.ipAddress);
			LAN::Traverse::SendArp(ipAddressModel.ipAddress);
			addressess[ipAddressModel.ipAddress].ipAddress = ipAddressModel.ipAddress;
		}
		catch (const char* e)
		{
			MessageBoxA(NULL, (LPCSTR)e, (LPCSTR)e, MB_OK);
		}


		return addressess;
	}
};

///////////////////////////////////////////////////////////////////////////////////
//Class for getting this pc subnet info
///////////////////////////////////////////////////////////////////////////////////
class ThisPcSubnet : public LAN::Traverse
{
public:
};

///////////////////////////////////////////////////////////////////////////////////
//Class for getting range of addresses
///////////////////////////////////////////////////////////////////////////////////
class RangeOfAddresses : public LAN::Traverse
{
public:
};

///////////////////////////////////////////////////////////////////////////////////
LAN::Traverse* LAN::GetTraverseObject(int choice)
{
	switch (choice)
	{
	case IDC_THISPCINFO:
		return new ThisPcInfo();
	case IDC_THISPCSUBNET:
		return new ThisPcSubnet();
	case IDC_RANGEOFIPADDR:
		return new RangeOfAddresses();
	default:
		break;
	}

	return new ThisPcInfo();
}








//ThreadPool thPool(1);
//thPool.enqueue( [=] { LAN::Traverse::SendICMP("192.168.5.35"); });
//std::this_thread::sleep_for(std::chrono::milliseconds(5000));
//for(int i = 0; i < addresses.size(); ++i)
//{
//	thPool.enqueue( [=] { ResolveHostname(addresses.at(i).ipAddress); });
//}
//thPool.~ThreadPool();

/*		std::string ttt1("192.168.5.35");
		pointerForCallbackMethod[1].ipAddress = ttt1;
		LAN::Traverse::SendICMP(&pointerForCallbackMethod[1], ttt1);
		pointerForCallbackMethod[0].traverse = traverse;

		int cntSendICMP = 0;
		while(callbackReplys < cntSendICMP)
		SleepEx(1100, true);

*/
