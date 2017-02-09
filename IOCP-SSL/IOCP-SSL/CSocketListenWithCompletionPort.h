#pragma once
#include "CSocketGet.h"
#include "CSocketInfoQueue.h"
#define DATA_BUFSIZE 8192
namespace NSCSocket{
	//���ڴ��͵ݰ󶨵�Socket
	typedef struct LThreadParameter{
		SOCKET bindSocket;
	}LThreadParameter;
	//���ڱ�����ͻ��˽���ͨ�ŵ��׽���
	typedef struct {
		SOCKET Socket;
		string Addr;
		DWORD Flags;
		DWORD RecvBytes;
		int type;//�����,�ͻ���
	}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;
	//���ڱ���I/O����������ָ���ͺͽ������ݣ����������
	typedef struct {
		OVERLAPPED Overlapped;
		WSABUF DataBuf;
		CHAR Buffer[DATA_BUFSIZE];
		DWORD ByteSend;
		DWORD ByteRecv;
	}PER_IO_DATA, *LPPER_IO_DATA;

	class CSocketListenWithCompletionPort :public CSocketGet{
	public:
		CSocketListenWithCompletionPort(){}
		static HANDLE hCompletionPort;
		DWORD iCompletionPort(SOCKET s);
		static char* getAddrIn(sockaddr_in* _in);
		static DWORD WINAPI ListenThread(LPVOID);
		static DWORD WINAPI ServerWorkerThread(LPVOID);
		static int waitTime();
		static void initPerIoData(LPPER_IO_DATA);
		static void initPerHandleData(LPPER_HANDLE_DATA mPerHandleData,string _addr, SOCKET socket);
		static void bindCompletionPort(SOCKET&,string);
	};
}