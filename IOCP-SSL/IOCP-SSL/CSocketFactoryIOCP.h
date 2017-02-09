#pragma once
#include "ConvertClass.h"
#include "CSocketListenWithCompletionPort.h"
#include "CSocketSend.h"
#include "CSocketInit.h"
#include "CSocketSSL.h"

namespace NSCSocket{
	using namespace NSCSocketInfoQueue;
	using namespace ConvertClass;
	using namespace std;

	typedef struct{
		int sslEnable;	//�Ự�Ƿ����
		SSL_DATA mSSL_DATA;	//ÿ���Ự��ssl��Ϣ
		PER_HANDLE_DATA mPerHandleData;	//Socket��Ϣ
	}Iocp_Data;

	class CSocketFactoryIOCP{
	private:
		static CSocketSSL mSocketSSL;
		static HANDLE mWorkThread;
	public:
		static map<string, Iocp_Data> RecvHandle;	//���յı���ÿһ������
		static map<string, Iocp_Data> SendHandle;	//���͵ı���ÿһ������
		static CSocketSend mSocketSend;
		static CSocketListenWithCompletionPort mIOCP;
		CSocketInit mInit = CSocketInit((u_short)10000,(u_short)10001);

		string localAddress;

		CSocketFactoryIOCP();

		int WorkListen();
		void StartInfo();
		static DWORD WINAPI GetInfo(LPVOID);
	};
}
