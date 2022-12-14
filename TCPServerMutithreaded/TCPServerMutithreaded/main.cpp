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
#include <shared_mutex>
#include <fstream>

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
typedef std::map<string, vector<string>> RequestsMap; // TODO: change to vectors of just the message - not the whole request
RequestsMap requests;
std::shared_mutex sharedMutex;
mutex m;

int main() {
	TCPServer server(DEFAULT_PORT);
	ReceivedSocketData receivedData;
	vector<thread> serverThreads;

	cout << "Starting server. Send \"exit\" (without quotes) to terminate." << endl;

	while (!terminateServer) {
		receivedData = server.accept();

		if (!terminateServer) {
			serverThreads.emplace_back(serverThreadFunction, &server, receivedData);
		}
	}

	for (thread& thread : serverThreads)
		thread.join();

	int count = 0;
	for (auto request : requests) {
		count = count + request.second.size();
	}
	cout << "Total number of post requests stored: " << count << endl;

	cout << "Server terminated." << endl;
	//int keepAppOpen = _getch();
	return 0;
}

//										rvalue reference / move semantics
void serverThreadFunction(TCPServer* server, ReceivedSocketData&& data) {
	unsigned int socketIndex = (unsigned int)data.ClientSocket;

	do {
		server->receiveData(data, 0); // updates request field
		if (data.request != "" && data.request != "exit" && data.request != "EXIT") {
			data.reply = protocolVerifier(data.request);
			server->sendReply(data);
		} else if (data.request == "exit" || data.request == "EXIT") {
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

	std::unique_lock<std::shared_mutex> uniqueLock(sharedMutex, std::defer_lock);
	std::shared_lock<std::shared_mutex> sharedLock(sharedMutex, std::defer_lock);
	
	Request newRequest = Request::parse(requestReceived);
	if (newRequest.isValid()) {
		const char requestTypeFirstChar = newRequest.getRequestType()[0];
		switch (requestTypeFirstChar) {
		case 'P': {
			uniqueLock.lock();
			auto mapIterator = requests.find(newRequest.getTopicID());
			if (mapIterator == requests.end()) { // if new request
				vector<string> newVector = { newRequest.getMessage() };
				requests.insert(pair<string, vector<string>>(newRequest.getTopicID(), newVector));
				uniqueLock.unlock();
				return "0";
			}
			else {
				mapIterator->second.push_back(newRequest.getMessage());
				int count = mapIterator->second.size();
				const string ret = std::to_string(--count);
				uniqueLock.unlock();
				return ret;
			}
			break;
		}
		case 'R': {
			sharedLock.lock();
			auto mapIterator = requests.find(newRequest.getTopicID());
			if (mapIterator == requests.end()) {
				sharedLock.unlock();
				return "";
			}
			else {
				const int countToFind = stoi(newRequest.getMessage());
				if (mapIterator->second.size() <= countToFind) {
					sharedLock.unlock();
					return "";
				}
				else {
					sharedLock.unlock();
					return mapIterator->second.at(countToFind);
				}
			}
			break;
		}
		case 'L': {
			int count = 0;
			sharedLock.lock();
			if (requests.size() == 0) {
				sharedLock.unlock();
				return "";
			}
			else {
				string ret = "";
				for (auto iter = requests.begin(); iter != requests.end(); iter++) {
					ret = ret.append("@" + iter->first);
					if (std::next(iter) != requests.end()) {
						ret = ret.append("#");
					}
				}
				sharedLock.unlock();
				return ret;
			}
			break;
		}
		case 'C': {
			sharedLock.lock();
			auto mapIterator = requests.find(newRequest.getTopicID());
			if (mapIterator == requests.end()) {
				sharedLock.unlock();
				return "0";
			}
			else {
				const string ret = std::to_string(mapIterator->second.size());
				sharedLock.unlock();
				return ret;
			}
			break;
		}
		default:
			return "";
			break;
		}
	}
	else {
		return "";
	}
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
