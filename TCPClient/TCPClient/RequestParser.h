#ifndef __REQUESTPARSER_H
#define __REQUESTPARSER_H

#include <string>

using namespace std;

class PostRequest
{
public:
	PostRequest(const string topicID, const string message);
	~PostRequest();
	static PostRequest parse(string request);
	string getTopicId() const { return this->topicId; }
	void setTopicId(string topicID) { this->topicId = topicID; }
	string getMessage() const { return this->message; }
	void setMessage(string message) { this->message = message;  }
	string toString();

private:
	string topicId;
	string message;
	bool valid;
};

class ReadRequest
{
public:
	ReadRequest();
	~ReadRequest();
	static ReadRequest parse(string request);
	string getTopicId();
	int getPostId();
	string toString();

	string topicId;
	int postId;
	bool valid;
};

class CountRequest
{
public:
	CountRequest();
	~CountRequest();
	static CountRequest parse(string request);
	string getTopicId();
	string toString();

	string topicId;
	bool valid;
};

class ListRequest
{
public:
	ListRequest();
	~ListRequest();
	static ListRequest parse(string request);
	string toString();
	bool valid;
};

class ExitRequest
{
public:
	ExitRequest();
	~ExitRequest();
	static ExitRequest parse(string request);
	string toString();
	bool valid;
};

#endif //__REQUESTPARSER_H

