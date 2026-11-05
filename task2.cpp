#include <iostream>
#include "Task2.h"

using namespace std;

// Global instances definitions
RequestPriorityQueue activeRequests = { {}, 0 };
int nextRequestID = 2001; // Starting ID for service requests

bool isRequestQueueFull(const RequestPriorityQueue& q) {
    return q.count >= MAX;
}

bool isRequestQueueEmpty(const RequestPriorityQueue& q) {
    return q.count == 0;
}

/**
 * Functional Requirement 1: Create a Service Request
 * - Validates parcel existence in Task 1
 * - Prevents duplicate active requests
 * - Assigns unique Request ID
 * - Inserts request in Priority Order (Ascending order of priorityLevel)
 */
void createServiceRequest(int parcelID, std::string serviceType, int priorityLevel, std::string requestDateTime) {
    // 1. Check if the active requests queue is full
    if (isRequestQueueFull(activeRequests)) {
        cout << "[ERROR] Cannot create request. Priority Queue is FULL.\n";
        return;
    }

    // 2. Validate if the Parcel exists in Task 1 memory using Task 1's search function
    Parcel* targetParcel = searchSpecificParcel(std::to_string(parcelID));
    if (targetParcel == nullptr) {
        cout << "[ERROR] Parcel ID " << parcelID << " does not exist in the system.\n";
        return;
    }

    // 3. Prevent duplicate active requests for the exact same parcel
    for (int i = 0; i < activeRequests.count; i++) {
        if (activeRequests.requests[i].parcelID == parcelID && 
           (activeRequests.requests[i].status == "Pending" || activeRequests.requests[i].status == "In Progress")) {
            cout << "[ERROR] An active service request (Request ID: " 
                      << activeRequests.requests[i].requestID 
                      << ") already exists for Parcel ID " << parcelID << ".\n";
            return;
        }
    }

    // 4. Construct the new Service Request
    ServiceRequest req;
    req.requestID = nextRequestID++;
    req.parcelID = targetParcel->parcelID;
    req.recipientName = targetParcel->recipientName;
    req.recipientID = targetParcel->recipientID;
    req.serviceType = serviceType;
    req.priorityLevel = priorityLevel;
    req.requestDateTime = requestDateTime;
    req.status = "Pending";

    // 5. Insert into Priority Queue (Insertion Sort maintaining highest priority at front)
    int i = activeRequests.count - 1;
    while (i >= 0 && activeRequests.requests[i].priorityLevel > req.priorityLevel) {
        activeRequests.requests[i + 1] = activeRequests.requests[i];
        i--;
    }
    activeRequests.requests[i + 1] = req;
    activeRequests.count++;

    // 6. Sync back status with Task 1 Parcel
    targetParcel->status = "Service Requested - " + serviceType;

        cout << "[SUCCESS] Service Request ID " << req.requestID 
              << " created for Parcel ID " << parcelID 
              << " (" << serviceType << " | Priority: " << priorityLevel << ").\n";
}