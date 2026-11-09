#include "task2.h"
#include "task1.h"
#include "task4.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

void saveRequestsToFile(const string& filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "[ERROR] Could not open file '" << filename << "' for writing.\n";
        return;
    }


    outFile << left << setw(10) << "Req ID"
            << setw(12) << "Parcel ID"
            << setw(20) << "Recipient Name"
            << setw(22) << "Service Type"
            << setw(12) << "Priority"
            << setw(20) << "Date/Time"
            << setw(14) << "Status" << "\n";
    outFile << string(110, '-') << "\n";


    for (int i = 0; i < activeRequests.count; i++) {
        ServiceRequest req = activeRequests.requests[i];
        outFile << left << setw(10) << req.requestID
                << setw(12) << req.parcelID
                << setw(20) << req.recipientName
                << setw(22) << req.serviceType
                << setw(12) << req.priorityLevel
                << setw(20) << req.requestDateTime
                << setw(14) << req.status << "\n";
    }

    outFile.close();
    cout << "[SUCCESS] Saved " << activeRequests.count << " request(s) to '" << filename << "'.\n";
}


RequestPriorityQueue activeRequests = { {}, 0 };
int nextRequestID = 2001;

bool isRequestQueueFull(const RequestPriorityQueue& q) {
    return q.count >= MAX;
}

bool isRequestQueueEmpty(const RequestPriorityQueue& q) {
    return q.count == 0;
}

ServiceRequest* findServiceRequest(int requestID) {
    for (int i = 0; i < activeRequests.count; i++) {
        if (activeRequests.requests[i].requestID == requestID) {
            return &activeRequests.requests[i];
        }
    }
    return nullptr;
}

/**
 * Requirement 1: Create a Service Request
 * Validates parcel, avoids duplicate pending/in-progress requests,
 * assigns ID, and inserts in ascending order of priority level (1 = highest).
 */
void createServiceRequest(int parcelID, string serviceType, int priorityLevel, string requestDateTime) {
    if (isRequestQueueFull(activeRequests)) {
        cout << "[ERROR] Cannot create request. Active Request Queue is FULL.\n";
        return;
    }

    Parcel* targetParcel = searchSpecificParcel(to_string(parcelID));
    if (targetParcel == nullptr) {
        cout << "[ERROR] Parcel ID " << parcelID << " does not exist in the system.\n";
        return;
    }

    for (int i = 0; i < activeRequests.count; i++) {
        if (activeRequests.requests[i].parcelID == parcelID && 
           (activeRequests.requests[i].status == "Pending" || activeRequests.requests[i].status == "In Progress")) {
            cout << "[ERROR] An active service request (Request ID: " 
                 << activeRequests.requests[i].requestID 
                 << ") already exists for Parcel ID " << parcelID << ".\n";
            return;
        }
    }

    ServiceRequest req;
    req.requestID = nextRequestID++;
    req.parcelID = targetParcel->parcelID;
    req.recipientName = targetParcel->recipientName;
    req.recipientID = targetParcel->recipientID;
    req.serviceType = serviceType;
    req.priorityLevel = priorityLevel;
    req.requestDateTime = requestDateTime;
    req.status = "Pending";

    // Insertion sort to place higher priority (lower priorityLevel value) toward front
    int i = activeRequests.count - 1;
    while (i >= 0 && activeRequests.requests[i].priorityLevel > req.priorityLevel) {
        activeRequests.requests[i + 1] = activeRequests.requests[i];
        i--;
    }
    activeRequests.requests[i + 1] = req;
    activeRequests.count++;

    string prevStatus = targetParcel->status;
    targetParcel->status = "Requested: " + serviceType;

   
    recordActivity(targetParcel->parcelID, "SERVICE_REQUEST", prevStatus, targetParcel->status, to_string(req.requestID), "", true);

    cout << "[SUCCESS] Service Request ID " << req.requestID 
         << " created for Parcel ID " << parcelID 
         << " (" << serviceType << " | Priority: " << priorityLevel << ").\n";
}

/**
 * Requirement 2: Display Active Service Requests
 * Prints all currently pending or in-progress requests in order.
 */
void displayActiveRequests() {
    if (isRequestQueueEmpty(activeRequests)) {
        cout << "\n[INFO] No active service requests in the queue.\n";
        return;
    }

    cout << "\n" << string(110, '=') << endl;
    cout << left << setw(10) << "Req ID" 
         << setw(12) << "Parcel ID" 
         << setw(20) << "Recipient Name" 
         << setw(22) << "Service Type" 
         << setw(12) << "Priority" 
         << setw(20) << "Date/Time" 
         << setw(14) << "Status" << endl;
    cout << string(110, '-') << endl;

    for (int i = 0; i < activeRequests.count; i++) {
        ServiceRequest req = activeRequests.requests[i];
        cout << left << setw(10) << req.requestID 
             << setw(12) << req.parcelID 
             << setw(20) << req.recipientName 
             << setw(22) << req.serviceType 
             << setw(12) << req.priorityLevel 
             << setw(20) << req.requestDateTime 
             << setw(14) << req.status << endl;
    }
    cout << string(110, '=') << endl;
}

/**
 * Requirement 3: Determine and View Next Request to Handle
 * Returns a pointer to the highest priority request at the front of the queue.
 */
ServiceRequest* getNextRequest() {
    if (isRequestQueueEmpty(activeRequests)) {
        cout << "[INFO] Priority Queue is empty. No request to handle.\n";
        return nullptr;
    }
    return &activeRequests.requests[0];
}

/**
 * Requirement 4: Process Service Request
 * Updates request status, updates parcel status in Task 1, and removes from active queue if completed/cancelled.
 */
void processServiceRequest(int requestID, string newStatus) {
    ServiceRequest* req = findServiceRequest(requestID);
    
    if (req == nullptr) {
        cout << "[ERROR] Service Request ID " << requestID << " not found.\n";
        return;
    }

    if (req->status == "Completed" || req->status == "Cancelled") {
        cout << "[ERROR] Request ID " << requestID << " is already " << req->status << " and cannot be re-processed.\n";
        return;
    }

    req->status = newStatus;

    Parcel* targetParcel = searchSpecificParcel(to_string(req->parcelID));
    if (targetParcel != nullptr) {
        string prevStatus = targetParcel->status;
        targetParcel->status = "Request " + newStatus;
        recordActivity(req->parcelID, "PROCESS_REQUEST", prevStatus, targetParcel->status, to_string(req->requestID), "", true);
    }

    if (newStatus == "Completed" || newStatus == "Cancelled") {
        int targetIdx = -1;
        for (int i = 0; i < activeRequests.count; i++) {
            if (activeRequests.requests[i].requestID == requestID) {
                targetIdx = i;
                break;
            }
        }
        if (targetIdx != -1) {
            for (int i = targetIdx; i < activeRequests.count - 1; i++) {
                activeRequests.requests[i] = activeRequests.requests[i + 1];
            }
            activeRequests.count--;
        }
    }

    cout << "[SUCCESS] Service Request ID " << requestID << " successfully updated to: " << newStatus << endl;
}

/**
 * Requirement 5: Cancel Service Request
 * Cancels a pending request by ID and frees it from the priority queue.
 */
void cancelServiceRequest(int requestID) {
    ServiceRequest* req = findServiceRequest(requestID);
    if (req == nullptr) {
        cout << "[ERROR] Cannot cancel. Request ID " << requestID << " does not exist.\n";
        return;
    }
    processServiceRequest(requestID, "Cancelled");
}