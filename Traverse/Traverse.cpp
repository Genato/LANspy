#include "..\LANspy\stdafx.h"


VOID NTAPI ReplyCame(PVOID context, PIO_STATUS_BLOCK pio, DWORD reserved)
{
	CString str("Hellou from CALLBACK!!");
	MessageBox(NULL, str, str, NULL);

	return;
}

///////////////////////////////////////////////////////////////////////////////////
//Traverse Member implemetation
///////////////////////////////////////////////////////////////////////////////////

void LAN::Traverse::SendICMP(IpAddressesModel* ipAddressModel)
{
	HANDLE hIcmpFile;
	unsigned long ipaddr = INADDR_NONE;
	char * data = "hi all";
	char * ReplyBuffer = (char*)malloc(sizeof(ICMP_ECHO_REPLY) * 1000 + 8);
	DWORD ReplySize = 0;
	std::stringstream ss;

	ipaddr = inet_addr(ipAddressModel->ipAddress.c_str());
	if (ipaddr == INADDR_NONE)
	{
		return;
	}

	hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	DWORD dwRetVal = IcmpSendEcho2(hIcmpFile, 0, ReplyCame, 0, ipaddr,
		data, strlen(data), NULL, ReplyBuffer, (sizeof(ICMP_ECHO_REPLY) * 1000) + 8, 1000);

	//if (dwRetVal == ERROR_IO_PENDING)
	//{
	//	return;
	//}

	ipAddressModel->hostName = ResolveHostname(ipAddressModel->ipAddress);
	ipAddressModel->macAddress = SendArp(ipAddressModel->ipAddress);

	addresses.push_back(*ipAddressModel);
}

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
			return ipAddressModel;
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
				// MAC address
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


				// IP address and subnet mask
				std::string ip_str;
				PULONG mask = &outBufLen;
				unsigned long net_mask;
				pUnicast = pCurrAddresses->FirstUnicastAddress;
				short mask_bits_length = pUnicast->OnLinkPrefixLength;
				if (pUnicast != NULL)
				{
					for (int i = 0; pUnicast != NULL; i++)
					{
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

						struct sockaddr_in  *sockaddr_ipv4;
						sockaddr_ipv4 = (struct sockaddr_in *)pUnicast->Address.lpSockaddr;
						ip_str = inet_ntoa(sockaddr_ipv4->sin_addr);

						pUnicast = pUnicast->Next;
					}
				}

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
		if (dwRetVal == ERROR_NO_DATA)
		{
			ipAddressModel.valid = false;
			return ipAddressModel;
		}
		else {

			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				// Default language
				(LPTSTR)& lpMsgBuf, 0, NULL))
			{
				LocalFree(lpMsgBuf);
				if (pAddresses)
					FREE(pAddresses);

				ipAddressModel.valid = false;
				return ipAddressModel;
			}
		}
	}

	if (pAddresses)
	{
		FREE(pAddresses);
	}

	return ipAddressModel;
}

std::string LAN::Traverse::ResolveHostname(const std::string& ipAddress)
{
	// Declare and initialize variables
	WSADATA wsaData = { 0 };
	int iResult = 0;

	DWORD dwRetval;

	struct sockaddr_in saGNI;
	char hostname[NI_MAXHOST];
	char servInfo[NI_MAXSERV];
	u_short port = 27015;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0)
	{
		std::cout << "WSAStartup failed: %d\n" << iResult;
	}


	// Set up sockaddr_in structure which is passed
	// to the getnameinfo function
	saGNI.sin_family = AF_INET;
	saGNI.sin_addr.s_addr = inet_addr(ipAddress.c_str());
	saGNI.sin_port = htons(port);

	// Call getnameinfo
	dwRetval = getnameinfo((struct sockaddr *) &saGNI, sizeof(struct sockaddr), hostname, NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);

	if (dwRetval != 0)
		return "Couldn't resolve hostname.";

	return hostname;
}

std::string LAN::Traverse::SendArp(const std::string& str_ip)
{
	DWORD dwRetVal;
	IPAddr DestIp = 0;
	ULONG MacAddr[2];       /* for 6-byte hardware addresses */
	ULONG PhysAddrLen = 6;  /* default to length of six bytes */
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
			ss << "Warning: SendArp completed successfully, but returned length=0";
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
	}

	return ss.str();
}


///////////////////////////////////////////////////////////////////////////////////
//Class for getting this pc LAN info
///////////////////////////////////////////////////////////////////////////////////
class ThisPcInfo : public LAN::Traverse
{
public:

	virtual std::vector<LAN::IpAddressesModel> Search(LAN::Traverse* traverse) override
	{
		LAN::IpAddressesModel ipAddressModel;

		ipAddressModel = LAN::Traverse::GetAdaptersAddress();

		LAN::Traverse::SendICMP(&ipAddressModel);

		return addresses;
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
