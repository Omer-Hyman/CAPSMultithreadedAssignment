#ifndef __REQUESTPARSER_H
#define __REQUESTPARSER_H

#include <string>

using namespace std;

class Request {
public:
	Request();
	Request(char requestType);
	Request(char requestType, string topicID, string message);
	~Request();

	static Request parse(const string request);
	const bool isValid() const { return this->valid; };
	const string getTopicID() const { return this->topicID; }
	const string getMessage() const { return this->message; }
	const char getRequestType() const { return this->requestType; }
	void setValidToFalse() { this->valid = false; }
	const string toString() { };

private:
	const char requestType;
	const string topicID;
	const string message;
	bool valid = false;
};

#endif //__REQUESTPARSER_H