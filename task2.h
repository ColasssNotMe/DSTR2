#ifndef TASK2_H
#define TASK2_H

#include <string>
#include "task1.h"

using namespace std;

// 1. Service Request Structure
struct ServiceRequest {
    int requestID;
    int parcelID;
    string recipientName;
    string recipientID;
    string serviceType;
    int priorityLevel;
    string requestDateTime;
    string status;
};

// 2. Priority Queue Structure
struct RequestPriorityQueue {
    ServiceRequest requests[500];
    int count;
};

// 3. Global Variables
extern RequestPriorityQueue activeRequests;
extern int nextRequestID;

// 4. Function Prototypes
bool isRequestQueueFull(const RequestPriorityQueue& q);
bool isRequestQueueEmpty(const RequestPriorityQueue& q);
ServiceRequest* findServiceRequest(int requestID);

void createServiceRequest(int parcelID, string serviceType, int priorityLevel, string requestDateTime);
void displayActiveRequests();
ServiceRequest* getNextRequest();
void processServiceRequest(int requestID, string newStatus);
void cancelServiceRequest(int requestID);


void saveRequestsToFile(const string& filename = "service_requests.txt");
#endif // TASK2_H