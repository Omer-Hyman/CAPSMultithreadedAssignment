#ifndef __REQUESTPARSER_H
#define __REQUESTPARSER_H

#include <string>

using namespace std;

// TODO: use polymorphism
class Request {
public:
	Request();
	Request(string requestType, string topicID, string message, bool valid);
	~Request();

	static Request parse(const string request);
	const bool isValid() const { return this->valid; };
	const string getTopicID() const { return this->topicID; }
	const string getMessage() const { return this->message; }
	const string getRequestType() const { return this->requestType; }

private:
	const string requestType;
	const string topicID;
	const string message;
	bool valid = false;
};

class PostRequest : public Request {
public:
	PostRequest(string topicID, string message, bool valid);
	~PostRequest();

private:
	const string topicID;
	const string message;
	bool valid;
};

class ReadRequest : public Request {
public:
	ReadRequest(string topicID, string message, bool valid);
	~ReadRequest();

private:
	const string topicID;
	const string message;
	bool valid;
};

//class PostRequest {
//public:
//	PostRequest(string topicID, string message);
//	~PostRequest();
//	string getTopicId();
//	string getMessage();
//	virtual string toString();
//	bool isRequestValid() { return valid; }
//
//private:
//	string topicId;
//	string message;
//	bool valid;
//};
//
//class ReadRequest
//{
//public:
//	ReadRequest();
//	~ReadRequest();
//	static ReadRequest parse(string request);
//	string getTopicId();
//	int getPostId();
//	string toString();
//	bool isRequestValid() { return valid; }
//
//private:
//	string topicId;
//	int postId;
//	bool valid;
//};
//
//class CountRequest
//{
//public:
//	CountRequest();
//	~CountRequest();
//	static CountRequest parse(string request);
//	string getTopicId();
//	string toString();
//	bool isRequestValid() { return valid; }
//
//private:
//	string topicId;
//	bool valid;
//};
//
//class ListRequest
//{
//public:
//	ListRequest();
//	~ListRequest();
//	static ListRequest parse(string request);
//	string toString();
//	bool isRequestValid() { return valid; }
//
//private:
//	bool valid;
//};
//
//class ExitRequest
//{
//public:
//	ExitRequest();
//	~ExitRequest();
//	static ExitRequest parse(string request);
//	string toString();
//	bool isRequestValid() { return valid; }
//
//private:
//	bool valid;
//};

#endif //__REQUESTPARSER_H

