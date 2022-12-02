#define MINE
#ifdef MINE

#include <iostream>
#include <algorithm>
#include <string>
#include <thread>
#include <vector>
#include <conio.h>
#include <mutex>
#include <map>

#include "config.h"
#include "TCPServer.h"
#include "TCPClient.h"
#include "RequestParser.h"

using namespace std;

#define DEFAULT_PORT 12345

void serverThreadFunction(TCPServer* server, ReceivedSocketData&& data);
string protocolVerifier(string request);

bool terminateServer = false;
typedef std::vector<Request> VectorOfRequests;
typedef std::map<string, VectorOfRequests> RequestsMap;
RequestsMap requests;
mutex m;

int main() {
	TCPServer server(DEFAULT_PORT);
	ReceivedSocketData receivedData;
	vector<thread> serverThreads;

	cout << "Starting server. Send \"exit\" (without quotes) to terminate." << endl;

	while (!terminateServer) {
		receivedData = server.accept();

		// TODO: implement a barrier at the start and end. Do timer in between the two barriers.
		if (!terminateServer) {
			serverThreads.emplace_back(serverThreadFunction, &server, receivedData);
		}
	}

	cout << serverThreads.size() << "\n";
	for (thread& thread : serverThreads)
		thread.join();

	cout << "Server terminated." << endl;
	int keepAppOpen = _getch();
	return 0;
}

//										rvalue reference / move semantics
void serverThreadFunction(TCPServer* server, ReceivedSocketData&& data) {
	unsigned int socketIndex = (unsigned int)data.ClientSocket;

	do {
		server->receiveData(data, 0); // updates request field
		if (data.request != "" && data.request != "exit" && data.request != "EXIT") {
			//cout << "[" << socketIndex << "]" << "Data received: " << data.request << endl;
			data.reply = protocolVerifier(data.request);
			server->sendReply(data);
		}
		else if (data.request == "exit" || data.request == "EXIT") {
			cout << "[" << socketIndex << "] Data received: " << data.request << endl;
			cout << "[" << socketIndex << "] Exiting... Bye bye!" << endl;

			data.reply = "";
			server->sendReply(data);
		}

	} while (data.request != "exit" && data.request != "EXIT" && !terminateServer);

	if (!terminateServer && (data.request == "exit" || data.request == "EXIT")) {
		terminateServer = true;

		TCPClient tempClient(string("127.0.0.1"), DEFAULT_PORT);
		tempClient.OpenConnection();
		tempClient.CloseConnection();
	}
	server->closeClientSocket(data);
}

string protocolVerifier(string requestReceived) {
	// TODO: refactor to get rid of nested if loops maybe?

	int count = 0;

	Request newRequest = Request::parse(requestReceived);
	if (newRequest.isValid()) {
			m.lock();
			auto mapIterator = requests.find(newRequest.getTopicID());
			if (mapIterator == requests.end()) { // if new request
				if (newRequest.getRequestType() == "POST") {
					VectorOfRequests newVector;
					newVector.push_back(newRequest);
					requests.insert(pair<string, VectorOfRequests>(newRequest.getTopicID(), newVector));
					m.unlock();
					return "0";
				}
				else if (newRequest.getRequestType() == "READ") {
					m.unlock();
					return "";
				}
			}
			else { // existing request
				if (newRequest.getRequestType() == "POST") {
					const string ret = std::to_string(mapIterator->second.size());
					m.unlock();
					return ret;
				}
				else if (newRequest.getRequestType() == "READ") {
					const int index = stoi(newRequest.getMessage());
					string ret = "";
					if (mapIterator->second.size() > index) {
						ret = mapIterator->second.at(index).getMessage();
					}
					m.unlock();
					return ret;
				}
			}
			m.unlock();
	}
	else {
		cout << "Request " << requestReceived << "was not added.\n";
		return "";
	}
	
	//else if (ReadRequest::parse(request).isRequestValid()) { // return the first message (if read message is 0 after #) with the same topic id.
	//	int counter = 0;
	//	m.lock();
	//	for (PostRequest existingPost : postRequests) {
	//		if (existingPost.getTopicId() == newRead.getTopicId()) {
	//			if (counter == newRead.getPostId()) {
	//				return existingPost.getMessage();
	//			}
	//			counter++;
	//		}
	//	}
	//	m.unlock();
	//}
	//else if (ExitRequest::parse(request).isRequestValid()) {
	//	return "";
	//}

	//cout << "Can't parse message " << request << endl;
	//return "";
}

#else

#include <iostream>
#include <algorithm>
#include <string>
#include <thread>
#include <vector>

#include "config.h"
#include "TCPServer.h"
#include "TCPClient.h"

#define DEFAULT_PORT 12345

bool terminateServer = false;
int xyz = 0;

void serverThreadFunction(TCPServer* server, ReceivedSocketData&& data);

int main()
{
	TCPServer server(DEFAULT_PORT);

	ReceivedSocketData receivedData;

	std::vector<std::thread> serverThreads;

	std::cout << "Starting server. Send \"exit\" (without quotes) to terminate." << std::endl;

	while (!terminateServer)
	{
		receivedData = server.accept();

		if (!terminateServer)
		{
			serverThreads.emplace_back(serverThreadFunction, &server, receivedData); // stops at 3 threads
		}
	}

	for (auto& th : serverThreads)
		th.join();

	std::cout << "Server terminated." << std::endl;

	return 0;
}

void serverThreadFunction(TCPServer* server, ReceivedSocketData&& data)
{
	unsigned int socketIndex = (unsigned int)data.ClientSocket;
	cout << ++xyz << endl;

	do {
		server->receiveData(data, 0);

		if (data.request != "" && data.request != "exit" && data.request != "EXIT")
		{
			std::cout << "[" << socketIndex << "] Bytes received: " << data.request.size() << std::endl;
			std::cout << "[" << socketIndex << "] Data received: " << data.request << std::endl;

			data.reply = data.request;
			std::reverse(data.reply.begin(), data.reply.end());

			server->sendReply(data);
		}
		else if (data.request == "exit" || data.request == "EXIT")
		{
			std::cout << "[" << socketIndex << "] Data received: " << data.request << std::endl;
			std::cout << "[" << socketIndex << "] Exiting... Bye bye!" << std::endl;

			data.reply = data.request;
			server->sendReply(data);
		}
	} while (data.request != "exit" && data.request != "EXIT" && !terminateServer);

	if (!terminateServer && (data.request == "exit" || data.request == "EXIT"))
	{
		terminateServer = true;

		TCPClient tempClient(std::string("127.0.0.1"), DEFAULT_PORT);
		tempClient.OpenConnection();
		tempClient.CloseConnection();
	}

	server->closeClientSocket(data);
}
#endif
