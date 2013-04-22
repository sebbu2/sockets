#ifndef NETWORK_H
#define NETWORK_H

#if defined(_WIN32)||defined(__CYGWIN__)||defined(__MINGW32__)
/* windows */
#include <winsock2.h>
#include <windows.h>
#else
/* posix */
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif
/*
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
*/
#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <exception>
#include <stdexcept>

#include "Network_base.h"
#include "Network_singleton.hpp"

using namespace std;

class Network {
public:
	hostent* thisHost;
	PSOCK_SOCKET my_socket;
	sockaddr_in my_service;
	int con;
	char my_buf[513];
private:
	Network_singleton* obj;
#if 0
/* psock functions */
extern int psock_ctl(int sd, int mode);    /* a real function in compat/psock.c */
extern int psock_ctl_off(int sd,int mode); /* a real function in compat/psock.c */
#elif 0
/* psock functions */
extern int psock_ctl(int sd, int mode);    /* a real function in compat/psock.c */
extern int psock_ctl_off(int sd,int mode); /* a real function in compat/psock.c */
#endif
	/*int psock_errno();
	int psock_socket(int domain, int type, int protocol);
	int psock_connect(int sockfd, struct sockaddr *serv_addr, socklen_t addrlen );*/
public:
	Network();
	virtual ~Network();
	const char* getip(const char* adr);
	virtual int createsocket();
	virtual int connect(const char* ip,short unsigned int port);
	int close();
	int send(const void* buf,int len);
	int send(const std::string &s);
	int recv(void *buf, int len);
};

#endif
