#include <iostream>
#include "TCPClient.h"
#include <thread>
#include <conio.h>
#include <vector>
#include <windows.h>
#include <mutex>
#include <fstream>
#include "barrier.hpp"
#include <map>

#define PORT 12345

using namespace std;

int howManyRequests();
int requestsForHowLong();
int howManyClients();
void threadClient(bool isPoster);
LONGLONG startTimer();
LONGLONG measureTime();

int numberOfRequests, secondsOfRequests, numberOfPosterClients, numberOfReaderClients;
bool isProgramThrottled = false;
string topics[11] = { "heel", "kick", "hard", "dragon", "forward", "shuv", "varial", "fakie", "switch", "primo", "nollie" };
mutex m;
barrier bar(3); // make this work

vector<string> postRequests;
std::map<string, vector<string>> readRequestsMap;
std::map<string, vector<string>> postRequestsMap;
vector<string> readRequests;

LARGE_INTEGER startTime, endTime, elapsedMicroseconds, frequency;

int main() { // not sure about these parameters - maybe command line?
  srand((int)time(NULL));
  TCPClient client("localhost", PORT);
  vector<thread> clientThreads;
  string throttleString = "";

  //numberOfRequests = howManyRequests();
  secondsOfRequests = requestsForHowLong();
  cout << "Posters...\n";
  numberOfPosterClients = howManyClients();
  cout << "Readers...\n";
  numberOfReaderClients = howManyClients();
  cout << "Throttled? y/n\n";
  cin >> throttleString;
  if (throttleString == "y" || throttleString == "Y") {
    isProgramThrottled = true;
  }
  else {
    isProgramThrottled = false;
  }

  for (int i = 0; i < numberOfPosterClients; i++) {
    clientThreads.emplace_back(threadClient, true);
  }
  for (int i = 0; i < numberOfReaderClients; i++) {
    clientThreads.emplace_back(threadClient, false);
  }

  for (thread& thread : clientThreads)
    thread.join();

  cout << "POSTS:\n";
  for (auto post : postRequestsMap) {
    cout << post.first << ": " << post.second.size() << endl;
  }
  cout << "\nREAD:\n";
  for (auto read : readRequestsMap) {
    cout << read.first << ": " << read.second.size() << endl;
  }

  /*cout << "Number of post requests: " << postRequestsMap.size() << endl;
  cout << "Number of read requests: " << readRequests.size() << endl;
  cout << "Total number of requests sent: " << postRequests.size() + readRequests.size() << endl;*/

  TCPClient tempClient("localhost", PORT);
  tempClient.OpenConnection();
  tempClient.send("exit", isProgramThrottled);
  tempClient.CloseConnection();
  
  int keepAppOpen = _getch();
  return 0;
}

void threadClient(bool isPoster) {
  string send, reply;
  bool quit = false;
  TCPClient client("localhost", PORT);
  LARGE_INTEGER temp;
  client.OpenConnection();
  //cout << "Client " << client.getConnectSocket() << " started!\n";

  bar.count_down_and_wait(); // idk if this works???
  LONGLONG timeToReach = startTimer();
  QueryPerformanceCounter(&temp);
  LONGLONG oneSecond = temp.QuadPart + frequency.QuadPart;

  while (!quit)
  {
    //string topicID = topics[rand() % 11]; // point of improvement? removing random
    string topicID = to_string(client.getConnectSocket());
    string message(1, (rand() % 25) + 97);

    if (isPoster) {
      send = "POST@" + topicID + "#" + message;
      reply = client.send(send, isProgramThrottled);
      if (reply != "") {
        m.lock();
        postRequestsMap[topicID].push_back(message);
        m.unlock();
      }
    }
    else {
      send = "READ@" + topicID + "#" + "1";
      reply = client.send(send, isProgramThrottled);
      if (reply != "") {
        m.lock();
        readRequestsMap[topicID].push_back("1");
        m.unlock();
      }
    }
    if (measureTime() >= oneSecond) { // all good. Throttling works but it misses out two seconds. So for 5 seconds - 1 thread does 3000 requests
      QueryPerformanceCounter(&temp);
      oneSecond = temp.QuadPart + frequency.QuadPart;
      client.resetNumberOfRequestsSent();
    }
    if (measureTime() >= timeToReach)
    {
      quit = true;
    }
  }
  client.CloseConnection();
}

LONGLONG startTimer() {
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&startTime);
  return startTime.QuadPart + frequency.QuadPart * secondsOfRequests;
}

LONGLONG measureTime() {
  QueryPerformanceCounter(&endTime);
  return endTime.QuadPart;
}

int howManyRequests() {
  bool invalidInput;
  int numberOfRequests;
  do {
    invalidInput = false;
    cout << "How many requests do you want to send?\n0: Exit\n1: 100\n2: 1000\n3: 10000\n"; // moderate throughput is #3
    cin >> numberOfRequests;
    switch (numberOfRequests)
    {
    case 0:
      numberOfRequests = 0;
      break;
    case 1:
      numberOfRequests = 100;
      break;
    case 2:
      numberOfRequests = 1000;
      break;
    case 3:
      numberOfRequests = 10000;
      break;
    default:
      cout << "invalidInput! Try again.";
      invalidInput = true;
      break;
    }
  } while (invalidInput);

  return numberOfRequests;
}

int requestsForHowLong() {
  int seconds;
  cout << "How many seconds do you want to run requests for?  ";
  cin >> seconds;
  return seconds;
}

int howManyClients() {
  int quantity;
  cout << "How many clients?  ";
  cin >> quantity;
  if (quantity < 11) {
    return quantity;
  }
}

  //time for this client with reference server on 100 requests = 0.727208s
  //time for this client with my server on 100 requests = 0.924778s (0.582817s without protocol verifier)
  // 20479
