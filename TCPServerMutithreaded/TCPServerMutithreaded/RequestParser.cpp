#include <regex>

#include "RequestParser.h"
#include <iostream>

Request::Request() {}
Request::Request(string requestType, string topicID, string message, bool valid) : topicID(topicID), message(message), valid(true), requestType(requestType) {}
Request::~Request() {}

PostRequest::PostRequest(string topicID, string message, bool valid) : topicID(topicID), message(message), valid(false) { }

PostRequest::~PostRequest() { }

Request Request::parse(string request) {
	if (request.find("@") != std::string::npos) {
		const string typeOfRequest = request.substr(0, request.find("@"));
		const string topicID = request.substr(typeOfRequest.size() + 1, request.find("#") - typeOfRequest.size() - 1);
		const string message = request.substr(typeOfRequest.size() + topicID.size() + 2, request.size());
	// TODO: test invalid requests

		if (typeOfRequest == "") {
			cout << "invalid request: " << request <<"\n";
			Request invalid;
			return invalid;
		} // TODO: potentially if request type doesn't equal all the valid types branch
		else {
			Request newRequest(typeOfRequest, topicID, message, true);
			return newRequest;
		}
	}
	if (request.find("LIST") != std::string::npos) {
		Request newRequest("LIST", "", "", true);
	}
}

//string PostRequest::getTopicId() {
//	return topicId;
//}
//
//string PostRequest::getMessage() {
//	return message;
//}
//
//string PostRequest::toString() {
//	return string("POST") + topicId + string("#") + message;
//}
//
//ReadRequest::ReadRequest() : valid(0){ }
//
//ReadRequest::~ReadRequest(){ }
//
//ReadRequest ReadRequest::parse(string request) {
//	regex readRegex("^READ(@[^@#]*)#([0-9]+)$");
//	smatch readMatch; // results of the regex?
//	ReadRequest read;
//
//	if (!regex_match(request, readMatch, readRegex, regex_constants::match_default)) {
//		read.valid = 0;
//		return read;
//	}
//
//	read.topicId = readMatch[1];
//	read.postId = stoi(readMatch[2]); // stoi = string to int
//	read.valid = 1;
//
//	return read;
//}
//
//string ReadRequest::getTopicId() {
//	return topicId;
//}
//
//int ReadRequest::getPostId() {
//	return postId;
//}
//
//string ReadRequest::toString() {
//	return string("READ") + topicId + string("#") + to_string(postId);
//}
//
//CountRequest::CountRequest() : valid(0) { }
//
//CountRequest::~CountRequest() { }
//
//CountRequest CountRequest::parse(string request) {
//	regex countRegex("^COUNT(@[^@#]*)$");
//	smatch countMatch;
//	CountRequest count;
//
//	if (!regex_match(request, countMatch, countRegex, regex_constants::match_default)) {
//		count.valid = 0;
//		return count;
//	}
//
//	count.topicId = countMatch[1];
//	count.valid = 1;
//
//	return count;
//}
//
//string CountRequest::getTopicId() {
//	return topicId;
//}
//
//string CountRequest::toString() {
//	return string("COUNT") + topicId;
//}
//
//ListRequest::ListRequest() : valid(0) { }
//
//ListRequest::~ListRequest() { }
//
//ListRequest ListRequest::parse(string request) {
//	regex listRegex("^LIST$");
//	smatch listMatch;
//	ListRequest list;
//
//	if (!regex_match(request, listMatch, listRegex, regex_constants::match_default)) {
//		list.valid = 0;
//		return list;
//	}
//
//	list.valid = 1;
//
//	return list;
//}
//
//string ListRequest::toString() {
//	return string("LIST");
//}
//
//ExitRequest::ExitRequest() : valid(0) { }
//
//ExitRequest::~ExitRequest() { }
//
//ExitRequest ExitRequest::parse(string request) {
//	regex exitRegex("^EXIT$");
//	smatch exitMatch;
//	ExitRequest exitReq;
//
//	if (!regex_match(request, exitMatch, exitRegex, regex_constants::match_default)) {
//		exitReq.valid = 0;
//		return exitReq;
//	}
//
//	exitReq.valid = 1;
//
//	return exitReq;
//}
//
//string ExitRequest::toString() {
//	return string("EXIT");
//}
