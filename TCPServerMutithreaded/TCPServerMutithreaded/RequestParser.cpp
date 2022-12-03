#include <regex>

#include "RequestParser.h"
#include <iostream>

Request::Request() : requestType(""), topicID(""), message(""), valid(true) {}
Request::Request(string requestType) : requestType(requestType), topicID(""), message(""), valid(true) {}
Request::Request(string requestType, string topicID, string message) : requestType(requestType), topicID(topicID), message(message), valid(true) {}
Request::~Request() {}

Request Request::parse(string request) {
	const auto atIndex = request.find("@");
	if (atIndex != std::string::npos) { // if @ can be found
		string requestType = request.substr(0, atIndex);
		if (requestType == "POST" || requestType == "READ" || requestType == "COUNT") {
			requestType = requestType.substr(0, 1); // shorten type down to one letter
			if (requestType == "C") { // COUNT
				Request newRequest(requestType, request.substr(6, request.size()), "");
				return newRequest;
			} else { // POST and READ
				const string topicID = request.substr(5, request.find("#") - 5);
				const string message = request.substr(6 + topicID.size(), request.size());
				Request newRequest(requestType, topicID, message);
				return newRequest;
			}
		}
	} else if (request.find("LIST") != std::string::npos) {
		string requestType = request.substr(0, 1);
		Request newRequest(requestType);
		return newRequest;
	}
	Request invalid;
	invalid.setValidToFalse();
	return invalid;
}