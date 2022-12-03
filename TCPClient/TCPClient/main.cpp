#include <iostream>
#include "TCPClient.h"
#include <thread>
#include <conio.h>
#include <vector>
#include "RequestParser.h"
#include <windows.h>

#define PORT 12345

using namespace std;

int howManyRequests();
int requestsForHowLong();
int howManyClients();
void threadClient(TCPClient* client);

int numberOfRequests, secondsOfRequests, numberOfPosterClients, numberOfReaderClients;
string topics[11] = { "heel", "kick", "hard", "dragon", "forward", "shuv", "varial", "fakie", "switch", "primo", "nollie" };
string request, reply;
int requestCounter;


int main(int c, char** v) { // not sure about these parameters - maybe command line?
  srand((int)time(NULL));
  TCPClient client(v[1], PORT);
  vector<PostRequest> postRequests; // TODO: switch to heap memory
  vector<thread> clientThreads;

  if (c != 2) {
    printf("usage: %s server-name|IP-address\n", v[0]);
    return 1;
  }
  
  //numberOfRequests = howManyRequests();
  secondsOfRequests = requestsForHowLong();
  cout << "Posters...\n";
  numberOfPosterClients = howManyClients();
  cout << "Readers...\n";
  numberOfReaderClients = howManyClients();

  for (int i = 0; i < (numberOfPosterClients + numberOfReaderClients); i++) {
    clientThreads.emplace_back(threadClient, &client);
  }

  // TODO: barrier 
  for (thread& thread : clientThreads)
    thread.join();

  cout << requestCounter;
  int keepAppOpen = _getch();

  return 0;
}

void threadClient(TCPClient *client) {
  LARGE_INTEGER start, end;
  __int64 CounterStart;
  double PCFrequency = 0, timeTaken = 0;

  client->OpenConnection();

  /*if (!QueryPerformanceFrequency(&start))
      cout << "QueryPerformanceFrequency failed!\n";
  PCFrequency = double(start.QuadPart) / 1000.0;
  QueryPerformanceCounter(&start);
  QueryPerformanceCounter(&end);
  timeTaken = double(end.QuadPart - start.QuadPart) / PCFrequency;
  cout << "time: " << timeTaken / 1000 << "s" << endl << endl;
  */

  LARGE_INTEGER qpcFrequency;
  QueryPerformanceFrequency(&qpcFrequency);

  LARGE_INTEGER startTime;
  QueryPerformanceCounter(&startTime);

  LARGE_INTEGER timeToReach;
  timeToReach.QuadPart = startTime.QuadPart + qpcFrequency.QuadPart * secondsOfRequests;
  bool quit = false;
  while (!quit)
  {
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    string topicID = topics[0]; // do counter 0 - 11
    string message(1, (rand() % 25) + 97);
    PostRequest post(topicID, message);
    request = post.toString();
    reply = client->send(request);
    requestCounter++;
    // cout << "Sent: " << request << "\n";
    // cout << "Returned: " << reply << "\n";

    if (currentTime.QuadPart >= timeToReach.QuadPart)
      quit = true;
  }

  //while (timeTaken < secondsOfRequests) {
  //  string topicID = topics[0]; // do counter 0 - 11
  //  string message(1, (rand() % 25) + 97);
  //  PostRequest post(topicID, message);
  //  request = post.toString();
  //  reply = client->send(request);
  //  cout << "Sent: " << request << "\n";
  //  cout << "Returned: " << reply << "\n";

  //}
  client->send("exit");
  client->CloseConnection();

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
  cout << "How many seconds do you want to run requests for?\n";
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
