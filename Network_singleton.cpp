#include "Network_singleton.hpp"

Network_singleton* Network_singleton::obj=NULL;
int Network_singleton::count=0;

Network_singleton::Network_singleton() {
#if defined(_WIN32)||defined(__CYGWIN__)||defined(__MINGW32__)
	WORD    wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);

	if (WSAStartup(wVersionRequested, &wsaData)!=0)
	{
		/* FIXME: Tell the user that we could not find a usable WinSock DLL. */
		throw std::runtime_error("no usable winsock found");
	}
#endif
}

Network_singleton::~Network_singleton() {
#if defined(_WIN32)||defined(__CYGWIN__)||defined(__MINGW32__)
	WSACleanup();
#endif
}

Network_singleton* Network_singleton::GetInstance() {
	if(obj==NULL) {
		obj=new Network_singleton();
		count++;
	}
	return obj;
}
