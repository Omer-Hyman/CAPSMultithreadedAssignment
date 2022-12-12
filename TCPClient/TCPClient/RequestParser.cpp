#include <regex>

#include "RequestParser.h"
#include <iostream>

// COPY TO SERVER BEFORE DELETING

Request::Request() : requestType(0), topicID(""), message(""), valid(true) {}
Request::Request(char requestType) : requestType(requestType), topicID(""), message(""), valid(true) {}
Request::Request(char requestType, string topicID, string message) : requestType(requestType), topicID(topicID), message(message), valid(true) {}
Request::~Request() {}

Request Request::parse(string request) {
	const auto atIndex = request.find("@");
	if (atIndex != std::string::npos) { // if @ can be found
		string requestType = request.substr(0, atIndex);
		if (requestType == "POST" || requestType == "READ" || requestType == "COUNT") {
			if (requestType[0] == 'C') { // COUNT
				Request newRequest(requestType[0], request.substr(6, request.size()), "");
				return newRequest;
			}
			else { // POST and READ
				const string topicID = request.substr(5, request.find("#") - 5);
				const string message = request.substr(6 + topicID.size(), request.size());
				Request newRequest(requestType[0], topicID, message);
				return newRequest;
			}
		}
	}
	else if (request.find("LIST") != std::string::npos) {
		string requestType = request.substr(0, 1);
		Request newRequest(requestType[0]);
		return newRequest;
	}
	Request invalid;
	invalid.setValidToFalse();
	return invalid;
}