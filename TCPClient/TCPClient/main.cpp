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

void threadClient(bool isPoster);
LONGLONG startTimer();
LONGLONG measureTime();

int numberOfRequests, secondsOfRequests, numberOfPosterClients, numberOfReaderClients;
bool isProgramThrottled = false;
string topics[10] = { "heel", "kick", "hard", "dragon", "shuv", "varial", "fakie", "switch", "primo", "nollie" };
mutex m;
std::map<string, vector<string>> readRequestsMap;
std::map<string, vector<string>> postRequestsMap;
LARGE_INTEGER startTime, endTime, frequency;
barrier* bar = new barrier(1);

int main(int argc, char *argv[]) {
  srand((int)time(NULL));
  vector<thread> clientThreads;
  if (argc != 5) {
    cout << "Usage: \n1: Seconds of requests.\n2: Number of poster clients.\n3: Number of reader clients.\n3: Throttled\n";
    return 1;
  }
  /*secondsOfRequests = 3;
  numberOfPosterClients = 3;
  numberOfReaderClients = 3;
  isProgramThrottled = false;*/
  
  secondsOfRequests = stoi(argv[1]);
  numberOfPosterClients = stoi(argv[2]);
  numberOfReaderClients = stoi(argv[3]);
  if (*argv[4] == '1') {
    isProgramThrottled = true;
  }
  else {
    isProgramThrottled = false;
  }
  cout << "Program settings:\nSeconds: " << secondsOfRequests << "\nPosters: " << numberOfPosterClients << "\nReaders: " << numberOfReaderClients << "\nThrottled: " << isProgramThrottled << "\n\n";
  bar = new barrier(numberOfPosterClients + numberOfReaderClients);
  for (int i = 0; i < numberOfPosterClients; i++) {
    clientThreads.emplace_back(threadClient, true);
  }
  for (int i = 0; i < numberOfReaderClients; i++) {
    clientThreads.emplace_back(threadClient, false);
  }
  for (thread& thread : clientThreads)
    thread.join();

  TCPClient tempClient("localhost", PORT);
  tempClient.OpenConnection();
  tempClient.send("exit", isProgramThrottled);
  tempClient.CloseConnection();

  int totalPosterRequests = 0, totalReaderRequests = 0, averagePoster = 0, averageReader = 0;
  cout << "POSTS:\n";
  for (auto post : postRequestsMap) {
    cout << post.first << ": " << post.second.size() << endl;
    
    totalPosterRequests = totalPosterRequests + post.second.size();
  }
  averagePoster = totalPosterRequests / numberOfPosterClients;
  cout << "\nREAD:\n";
  for (auto read : readRequestsMap) {
    cout << read.first << ": " << read.second.size() << endl;
    totalReaderRequests = totalReaderRequests + read.second.size();
  }
  averageReader = totalReaderRequests / numberOfReaderClients;

  string fileName = to_string(numberOfPosterClients) + "Posters" + to_string(numberOfReaderClients) + "Readers.txt";
  ofstream file;
  file.open(fileName, ios::app);
  if (!file.fail()) {
    file << "Total poster requests: " << totalPosterRequests << endl;
    file << "Total reader requests: " << totalReaderRequests << endl;
    file << "Average requests per poster thread: " << averagePoster << endl;
    file << "Average requests per reader thread: " << averageReader << endl;
    file << "Total requests: " << totalPosterRequests + totalReaderRequests << endl;
    file << "Average requests per thread: " << (totalPosterRequests + totalReaderRequests) / (numberOfPosterClients + numberOfReaderClients) << endl;
    file << "Average requests per thread per second: " << ((totalPosterRequests + totalReaderRequests) / (numberOfPosterClients + numberOfReaderClients)) / secondsOfRequests << endl;
    file.close();
  }
  cout << "Total poster requests: " << totalPosterRequests << endl;
  cout << "Total reader requests: " << totalReaderRequests << endl;
  cout << "Average requests per poster thread: " << averagePoster << endl;
  cout << "Average requests per reader thread: " << averageReader << endl;
  cout << "Total requests: " << totalPosterRequests + totalReaderRequests << endl;
  cout << "Average requests per thread: " << (totalPosterRequests + totalReaderRequests) / (numberOfPosterClients + numberOfReaderClients) << endl;
  cout << "Average requests per thread per second: " << ((totalPosterRequests + totalReaderRequests) / (numberOfPosterClients + numberOfReaderClients)) / secondsOfRequests << endl;
  //int keepAppOpen = _getch();
  return 0;
}

void threadClient(bool isPoster) {
  string send, reply;
  bool quit = false;
  LARGE_INTEGER temp;
  TCPClient client("localhost", PORT);
  client.OpenConnection();
  bar->count_down_and_wait();
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
      m.lock();
      readRequestsMap[topicID].push_back("1");
      m.unlock();
      /*if (reply != "") {
        m.lock();
        readRequestsMap[topicID].push_back("1");
        m.unlock();
      }*/
    }
    if (measureTime() >= oneSecond) {
      client.resetNumberOfRequestsSent();
      QueryPerformanceCounter(&temp);
      oneSecond = temp.QuadPart + frequency.QuadPart;
    }
    if (measureTime() >= timeToReach)
      quit = true;
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