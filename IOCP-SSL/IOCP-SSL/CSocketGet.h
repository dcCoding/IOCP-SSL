#pragma once
#include "CSocket.h"
#include <map>
namespace NSCSocket{
	class CSocketGet :virtual public CSocket{
	public:
		static HANDLE hGetInformationEvent;
		static HANDLE hGetAddressEvent;
		static HANDLE hUpdateEvent;
		static HANDLE hCompletionPort;
		static string SInformation;
		static string SAddress;

		//���ӵĿͻ�������
		string* addresslist;
		int addressCount;

		map<string, wstring> infoRecord;

		CSocketGet();
		HANDLE getInformationEvent();
		HANDLE getAddressInfoEvent();
		HANDLE getUpdateEvent();

		void setInformationEvent();
		void setAddressInforEvent();

		string gInformation();
		string gAddress();
		BOOL ifGetAddress(char *);
	};
}