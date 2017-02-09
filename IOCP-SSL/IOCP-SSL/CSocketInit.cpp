#pragma once
#include "CSocketInit.h"
#include "ConvertClass.h"
#pragma comment(lib,"WS2_32.lib") 
#pragma comment(lib,"User32.lib")
#pragma comment(lib,"Iphlpapi.lib")
namespace NSCSocket{
	using namespace ConvertClass;

	CSocketInit::CSocketInit(u_short uport, u_short bport){
		this->setUPort(uport);
		this->setBPort(uport);
	}

	DWORD CSocketInit::iWSA(){
		//����WSAStartup������ȷ�����̼���socketӦ�ó���������Ļ����Ϳ��ļ�
		mRequested = MAKEWORD(2, 2);
		int err = WSAStartup(mRequested, &wsaData);
		if (err){
			int errcode = WSAGetLastError();
			LPCTSTR _code = (LPCTSTR)errcode;
			return -1;
		}
		return 0;
	}
	DWORD CSocketInit::iBind(){
		struct addrinfo uhints, bhints, *uresult = NULL, *bresult = NULL;
		
		//�󶨷��͵�socket
		SecureZeroMemory(&uhints, sizeof(uhints));

		uhints.ai_family = AF_INET;
		uhints.ai_socktype = SOCK_STREAM;
		uhints.ai_protocol = IPPROTO_TCP;
		uhints.ai_flags = AI_PASSIVE;

		string struPort = CConvert::toString(localUnicastPort);
		int uaddrre = getaddrinfo(localAddress.c_str(), (PCSTR)(struPort.c_str()), &uhints, &uresult);
		if (uaddrre != 0){
			this->showError(L"unicast getaddrinfo failed");
		}

		this->uSocket = WSASocket(uresult->ai_family, uresult->ai_socktype, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (uSocket == INVALID_SOCKET){
			this->showError(L"lUnicast init failed");
		}

		int ure = bind(uSocket, uresult->ai_addr, (int)uresult->ai_addrlen);
		if (ure == SOCKET_ERROR){
			this->showWSAError(L"lUnicast Binding Server failed");
		}

		//�󶨼�����socket
		SecureZeroMemory(&bhints, sizeof(bhints));

		bhints.ai_family = AF_INET;
		bhints.ai_socktype = SOCK_DGRAM;
		bhints.ai_protocol = IPPROTO_UDP;
		bhints.ai_flags = AI_PASSIVE;

		string strbPort = CConvert::toString(localBroadcastPort);

		int baddrre = getaddrinfo(localAddress.c_str(), (PCSTR)(strbPort.c_str()), &bhints, &bresult);

		if (baddrre != 0){
			this->showError(L"broadcast getaddrinfo failed");
		}

		this->bSocket = socket(bresult->ai_family, bresult->ai_socktype, bresult->ai_protocol);

		if (bSocket == INVALID_SOCKET){
			this->showError(L"lBroadcast init failed");
		}

		int bre = bind(bSocket, bresult->ai_addr, (int)bresult->ai_addrlen);

		if (bre == SOCKET_ERROR){
			this->showError(L"lBroadcast Binding Server failed");
		}

		return 0;
	}
	DWORD CSocketInit::iAddr(){
		/**************ͨ��������ȡ***************/
		PIP_ADAPTER_INFO pAdapterInfo;
		DWORD AdapterInfoSize;
		TCHAR szMac[32] = { 0 };
		DWORD Err;
		AdapterInfoSize = 0;
		Err = GetAdaptersInfo(NULL, &AdapterInfoSize);
		if ((Err != 0) && (Err != ERROR_BUFFER_OVERFLOW)){
			return   FALSE;
		}
		//   ����������Ϣ�ڴ�  
		pAdapterInfo = (PIP_ADAPTER_INFO)GlobalAlloc(GPTR, AdapterInfoSize);
		if (pAdapterInfo == NULL){
			return   FALSE;
		}
		if (GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize) != 0){
			GlobalFree(pAdapterInfo);
			return   FALSE;
		}
		PIP_ADAPTER_INFO pAdapter = NULL;
		pAdapter = pAdapterInfo;

		int ipListi = 0;
		while (pAdapter)
		{
			//pAdapter->IpAddressList.IpAddress.String �������Ǹ������ӵ�IP��ַ,
			//�������ӵĿ���κŵ�IP��ַ���ڣ��Լ�����һ��
			//�������Ͳο�pAdapter->Type��ֵ(PCI����,��������,PPPOE,VPN)������ô�ֲ����
			/*
			*23  ����
			*71��111 ����
			*6   ��������
			*/
			if (pAdapter->Type == 6){
				string _tmp = pAdapter->IpAddressList.IpAddress.String;
				wstring tmp = CConvert::toWstring(_tmp);
				this->setAddress(_tmp);
			}

			pAdapter = pAdapter->Next;
		}
		GlobalFree(pAdapterInfo);
	}

	SOCKET CSocketInit::getUSocket(){
		return this->uSocket;
	}
	SOCKET CSocketInit::getBSocket(){
		return this->bSocket;
	}
}