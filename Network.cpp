#include "Network.hpp"

#include <cstdio>

Network::Network() {
	obj=Network_singleton::GetInstance();
#ifdef _DEBUG
	printf("%d\n",Network_singleton::count);
#endif
}

Network::~Network() {
	if( --Network_singleton::count == 0 )
		delete obj;
#ifdef _DEBUG
	printf("%d\n",Network_singleton::count);
#endif
}

const char* Network::getip(const char* adr) {
	thisHost = gethostbyname(adr);
	if (thisHost==NULL) {
		std::cerr << "gethostbyname failed: " << psock_errno() << std::endl;
		//return EXIT_FAILURE;
		throw std::logic_error("gethostbyname failed");
	}
	/*for (struct in_addr **adr = (struct in_addr **)thisHost->h_addr_list; *adr; adr++,i++) {
		cout << "h_addr_list " << i << " = " << inet_ntoa(**adr) << endl;
	}*/
	return inet_ntoa(*(struct in_addr*)thisHost->h_addr);
}

int Network::createsocket() {
	my_socket = ::psock_socket( PSOCK_AF_INET, PSOCK_SOCK_STREAM, IPPROTO_TCP );
	return my_socket;
	
}

int Network::connect(const char* ip,short unsigned int port) {
	my_service.sin_family = AF_INET;
	my_service.sin_addr.s_addr = inet_addr(ip);
	my_service.sin_port = (short unsigned int) htons( (short unsigned int) port);
	con = ::psock_connect(my_socket, (struct sockaddr*) &my_service, sizeof(my_service) );
	if(con==PSOCK_SOCKET_ERROR) {
		std::cerr << "connect failed: " << psock_errno() << std::endl;
		//return EXIT_FAILURE;
		throw std::logic_error("connect failed");
	}
	return con;
}

/*int Network::psock_errno() {
#if defined(_WIN32)||defined(__CYGWIN__)||defined(__MINGW32__)
	return WSAGetLastError();
#else
	return (errno);
#endif
}*/

int Network::close() {
	return ::psock_close(my_socket);
}

int Network::send(const void* buf,int len) {
	int res=(int)::psock_send(my_socket,buf,len,0);
	if(res==-1) {
		std::cerr << "send failed : " << psock_errno() << std::endl;
		throw std::logic_error("send failed");
	}
	else if(res!=len) {
		std::cerr << "send incomplete" << std::endl;
	}
	return res;
}

int Network::send(const std::string &s) {
	return send(s.c_str(),(int)s.size());
}

int Network::recv(void *buf, int len) {
	return (int)::psock_recv(my_socket,buf,len,0);
}
