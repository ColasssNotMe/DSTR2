#include "task3.h"
#include <iostream>
using namespace std;

const int MAX_RESOURCE = 10;
int nextCounterNumber = 1;
int nextDeliveryNumber = 1;

ResourceQueue counterData;
ResourceQueue deliveryData;
ResourceQueue* counterQueue = &counterData;
ResourceQueue* deliveryQueue = &deliveryData;
ResourceQueue* allResourceQueue[2] = {
    &counterData,
    &deliveryData
};

// check full or empty
bool isResourceQueueFull(ResourceQueue*& queue) {
    return queue->count == MAX_RESOURCE;
}
bool isResourceQueueEmpty(ResourceQueue*& queue) {
    return queue->count == 0;
}

// add and remove
bool enqueueResource(Resource* resource, ResourceQueue*& queue) {
    if (isResourceQueueFull(queue)) {
        return false;
    }
    if (isResourceQueueEmpty(queue)) {
        queue->front = 0;
    }
    queue->rear = (queue->rear + 1) % MAX_RESOURCE;
    queue->resources[queue->rear] = resource;
    queue->count++;
    return true;
}
bool dequeueResource(ResourceQueue*& queue, Resource*& resource) {
    if (isResourceQueueEmpty(queue)) {
        return false;
    }
    resource = queue->resources[queue->front];
    queue->resources[queue->front] = nullptr;
    queue->front = (queue->front + 1) % MAX_RESOURCE;
    queue->count--;
    if (queue->count == 0) {
        queue->front = -1;
        queue->rear = -1;
    }
    return true;
}

// find using resid
Resource* findResource(string resourceID) {
    for (int i = 0; i < 2; i++) {
        ResourceQueue* queue = allResourceQueue[i];
        for (int j = 0; j < queue->count; j++) {
            int position = (queue->front + j) % MAX_RESOURCE;
            Resource* resource = queue->resources[position];
            if (resource->resourceID == resourceID) {
                return resource;
            }
        }
    }
    return nullptr;
}

// find using parid
Resource* getResourceOfParcel(int parcelID) {
    for (int i = 0; i < 2; i++) {
        ResourceQueue* queue = allResourceQueue[i];
        for (int j = 0; j < queue->count; j++) {
            int position = (queue->front + j) % MAX_RESOURCE;
            Resource* resource = queue->resources[position];
            if (resource->parcelID == parcelID) {
                return resource;
            }
        }
    }
    return nullptr;
}

//q1
bool registerNewResource(string type) {
    ResourceQueue* queue = nullptr;
    string resourceID;
    if (type == "Counter") {
        queue = counterQueue;
        if (isResourceQueueFull(queue)) {
            cout << "Counter queue is full." << endl;
            return false;
        }
        if (nextCounterNumber < 10) {
            resourceID = "C0" + to_string(nextCounterNumber);
        }
        else {
            resourceID = "C" + to_string(nextCounterNumber);
        }
    }
    else if (type == "Delivery") {
        queue = deliveryQueue;
        if (isResourceQueueFull(queue)) {
            cout << "Delivery queue is full." << endl;
            return false;
        }
        if (nextDeliveryNumber < 10) {
            resourceID = "D0" + to_string(nextDeliveryNumber);
        }
        else {
            resourceID = "D" + to_string(nextDeliveryNumber);
        }
    }
    else {

        cout << "Invalid resource type." << endl;
        return false;
    }
    Resource* newResource = new Resource;
    newResource->resourceID = resourceID;
    newResource->type = type;
    newResource->status = "Available";
    newResource->parcelID = 0;
    newResource->requestID = 0;
    if (enqueueResource(newResource, queue)) {

        if (type == "Counter") {
            nextCounterNumber++;
        }
        else {
            nextDeliveryNumber++;
        }
        cout << "Resource registered successfully." << endl;
        cout << "Resource ID: " << resourceID << endl;
        return true;
    }
    delete newResource;
    return false;
}
// q2
void printResource(Resource* resource) {
    if (resource == nullptr) {
        return;
    }
    cout << "Resource ID : " << resource->resourceID << endl;
    cout << "Type        : " << resource->type << endl;
    cout << "Status      : " << resource->status << endl;
    cout << "Parcel ID   : ";

    if (resource->parcelID == 0) {
        cout << "None" << endl;
    }
    else {
        cout << resource->parcelID << endl;
    }

    cout << "Request ID  : ";

    if (resource->requestID == 0) {
        cout << "None" << endl;
    }
    else {
        cout << resource->requestID << endl;
    }

    cout << "-----------------------------------" << endl;
}

void displayAllResources() {
    if (counterQueue->count == 0 &&
        deliveryQueue->count == 0) {
        cout << "No resources available." << endl;
        return;
    }
    cout << "\n========== COUNTER RESOURCES ==========\n";
    for (int i = 0; i < counterQueue->count; i++) {
        int position = (counterQueue->front + i) % MAX_RESOURCE;
        Resource* resource = counterQueue->resources[position];
        printResource(resource);
    }

    cout << "\n========== DELIVERY RESOURCES ==========\n";

    for (int i = 0; i < deliveryQueue->count; i++) {
        int position = (deliveryQueue->front + i) % MAX_RESOURCE;
        Resource* resource = deliveryQueue->resources[position];
        printResource(resource);
    }
}

// q3
string assignParcelToResource(int parcelID, int requestID, string type) {
    ResourceQueue* queue = nullptr;
    if (type == "Counter") {
        queue = counterQueue;
    }
    else if (type == "Delivery") {
        queue = deliveryQueue;
    }
    else {
        return "Invalid resource type.";
    }

    if (isResourceQueueEmpty(queue)) {
        return "No resources registered.";
    }

    for (int i = 0; i < queue->count; i++) {
        int position = (queue->front + i) % MAX_RESOURCE;
        Resource* resource = queue->resources[position];
        if (resource != nullptr &&
            resource->status == "Available") {
            resource->status = "Occupied";
            resource->parcelID = parcelID;
            resource->requestID = requestID;
            return "Parcel assigned successfully to "
                   + resource->resourceID;
        }
    }
    return "No available resource found.";
}


// q4

int releaseParcelResource(string resourceID) {
    Resource* resource = findResource(resourceID);
    if (resource == nullptr) {
        cout << "Resource not found." << endl;
        return 0;
    }
    if (resource->status != "Occupied") {
        cout << "Resource is not occupied." << endl;
        return 0;
    }
    int parcelID = resource->parcelID;
    resource->status = "Available";
    resource->parcelID = 0;
    resource->requestID = 0;
    cout << "Resource released successfully." << endl;
    return parcelID;
}


bool setResourceUnavailable(string resourceID) {
    Resource* resource = findResource(resourceID);
    if (resource == nullptr) {
        cout << "Resource not found." << endl;
        return false;
    }
    if (resource->status == "Occupied") {
        cout << "Cannot set an occupied resource as unavailable." << endl;
        return false;
    }
    if (resource->status == "Unavailable") {
        cout << "Resource is already unavailable." << endl;
        return false;
    }
    resource->status = "Unavailable";
    cout << "Resource set to unavailable." << endl;
    return true;
}

bool setResourceAvailable(string resourceID) {
    Resource* resource = findResource(resourceID);
    if (resource == nullptr) {
        cout << "Resource not found." << endl;
        return false;
    }
    if (resource->status == "Available") {
        cout << "Resource is already available." << endl;
        return false;
    }
    if (resource->status == "Occupied") {
        cout << "Cannot set an occupied resource as available." << endl;
        return false;
    }
    resource->status = "Available";
    cout << "Resource is now available." << endl;
    return true;
}

//FIXME: Not important, only testing
// Add initial resources
void seedResources() {
    if (counterQueue->count == 0 &&
        deliveryQueue->count == 0) {
        registerNewResource("Counter");
        registerNewResource("Counter");
        registerNewResource("Delivery");
        registerNewResource("Delivery");

        cout << "\nInitial resources loaded.\n";
    }
}
