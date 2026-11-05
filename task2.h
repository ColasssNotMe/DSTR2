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

// 2. Priority Queue Structure (Must be declared before using in functions!)
struct RequestPriorityQueue {
    ServiceRequest requests[MAX];
    int count;
};

// 3. Global Variables
extern RequestPriorityQueue activeRequests;
extern int nextRequestID;

// 4. Helper & Requirement Prototypes
bool isRequestQueueFull(const RequestPriorityQueue& q);
bool isRequestQueueEmpty(const RequestPriorityQueue& q);

void createServiceRequest(int parcelID, string serviceType, int priorityLevel, string requestDateTime);

#endif