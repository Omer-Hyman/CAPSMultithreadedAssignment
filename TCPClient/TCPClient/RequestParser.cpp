#include <regex>

#include "RequestParser.h"
#include <iostream>

PostRequest::PostRequest(const string topicID, const string message) : topicId(topicID), message(message), valid(0)
{ }

PostRequest::~PostRequest()
{ }

PostRequest PostRequest::parse(string request) {
	regex postRegex("^POST(@[^@#]*)#(.+)$");
	smatch postMatch;
	PostRequest post("", "");

	if (!regex_match(request, postMatch, postRegex, regex_constants::match_default)) {
		post.valid = 0;
		return post;
	}

	post.topicId = postMatch[1];
	post.message = postMatch[2];
	post.valid = 1;

	return post;
}

string PostRequest::toString() {
	return string("POST@") + topicId + string("#") + message;
}

ReadRequest::ReadRequest() : valid(0){ }

ReadRequest::~ReadRequest(){ }

ReadRequest ReadRequest::parse(string request) {
	regex readRegex("^READ(@[^@#]*)#([0-9]+)$");
	smatch readMatch; // results of the regex?
	ReadRequest read;

	if (!regex_match(request, readMatch, readRegex, regex_constants::match_default)) {
		read.valid = 0;
		return read;
	}

	read.topicId = readMatch[1];
	read.postId = stoi(readMatch[2]); // stoi = string to int
	read.valid = 1;

	return read;
}

string ReadRequest::getTopicId() {
	return topicId;
}

int ReadRequest::getPostId() {
	return postId;
}

string ReadRequest::toString() {
	return string("READ") + topicId + string("#") + to_string(postId);
}

CountRequest::CountRequest() : valid(0) { }

CountRequest::~CountRequest() { }

CountRequest CountRequest::parse(string request) {
	regex countRegex("^COUNT(@[^@#]*)$");
	smatch countMatch;
	CountRequest count;

	if (!regex_match(request, countMatch, countRegex, regex_constants::match_default)) {
		count.valid = 0;
		return count;
	}

	count.topicId = countMatch[1];
	count.valid = 1;

	return count;
}

string CountRequest::getTopicId() {
	return topicId;
}

string CountRequest::toString() {
	return string("COUNT") + topicId;
}

ListRequest::ListRequest() : valid(0) { }

ListRequest::~ListRequest() { }

ListRequest ListRequest::parse(string request) {
	regex listRegex("^LIST$");
	smatch listMatch;
	ListRequest list;

	if (!regex_match(request, listMatch, listRegex, regex_constants::match_default)) {
		list.valid = 0;
		return list;
	}

	list.valid = 1;

	return list;
}

string ListRequest::toString() {
	return string("LIST");
}

ExitRequest::ExitRequest() : valid(0) { }

ExitRequest::~ExitRequest() { }

ExitRequest ExitRequest::parse(string request) {
	regex exitRegex("^EXIT$");
	smatch exitMatch;
	ExitRequest exitReq;

	if (!regex_match(request, exitMatch, exitRegex, regex_constants::match_default)) {
		exitReq.valid = 0;
		return exitReq;
	}

	exitReq.valid = 1;

	return exitReq;
}

string ExitRequest::toString() {
	return string("EXIT");
}
