#ifndef NETWORK_SINGLETON_H
#define NETWORK_SINGLETON_H

#include <cerrno>
#include <exception>
#include <stdexcept>
#include <cstdio>

#include "Network_base.h"

class Network_singleton {
private:
	Network_singleton();
	static Network_singleton* obj;
public:
	static int count;
	~Network_singleton();
	static Network_singleton* GetInstance();
};

#endif /*NETWORK_SINGLETON_H*/
