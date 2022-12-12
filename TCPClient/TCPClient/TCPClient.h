#ifndef __TCPCLIENT_H
#define __TCPCLIENT_H

#include <winsock2.h>
#include <string>

class TCPClient
{
public:
	TCPClient(std::string server, unsigned short int port);
	~TCPClient();

	std::string send(std::string request, bool throttled);
	void OpenConnection();
	void CloseConnection();

	void resetNumberOfRequestsSent() { this->amountOfRequestsSent = 0; }
	int getConnectSocket() const { return this->ConnectSocket; }

private:
	std::string server;
	unsigned short int port;
	std::string portString;
	int amountOfRequestsSent;

	SOCKET ConnectSocket;
};

#endif // __TCPCLIENT_H
