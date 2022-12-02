#ifndef __RECEIVEDSOCKETDATA_H
#define __RECEIVEDSOCKETDATA_H

#include <winsock2.h>
#include <string>

using namespace std;

class ReceivedSocketData {
public:
	SOCKET ClientSocket;
	string request;
	string reply;

	ReceivedSocketData() {
		ClientSocket = INVALID_SOCKET;
	};

	//		what does rhs stand for?? right hand side?
	ReceivedSocketData(const ReceivedSocketData &rhs) {
		this->ClientSocket = rhs.ClientSocket;
		this->reply = rhs.reply;
		this->request = rhs.request;
	}

	~ReceivedSocketData() { };

	ReceivedSocketData& operator = (const ReceivedSocketData &rhs) {
		this->ClientSocket = rhs.ClientSocket;
		this->reply = rhs.reply;
		this->request = rhs.request;

		return *this;
	}
};

#endif //__RECEIVEDSOCKETDATA_H
