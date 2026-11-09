
#include "task3.h"
#include <iostream>

using namespace std;

const int MAX_RESOURCE = 10;

int nextCounterNumber = 1;
int nextDeliveryNumber = 1;

// Resource queues
ResourceQueue counterData;
ResourceQueue deliveryData;

ResourceQueue* counterQueue = &counterData;
ResourceQueue* deliveryQueue = &deliveryData;

ResourceQueue* allResourceQueue[2] = {
    &counterData,
    &deliveryData
};


// check 
bool isResourceQueueFull(ResourceQueue*& queue) {
    return queue->count == MAX_RESOURCE;
}

bool isResourceQueueEmpty(ResourceQueue*& queue) {
    return queue->count == 0;
}


// add and remove resourvce
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


// find use rid
Resource* findResource(string resourceID) {

    for (int i = 0; i < 2; i++) {

        ResourceQueue* queue = allResourceQueue[i];

        for (int j = 0; j < queue->count; j++) {

            int position = (queue->front + j) % MAX_RESOURCE;
            Resource* resource = queue->resources[position];

            if (resource != nullptr &&
                resource->resourceID == resourceID) {

                return resource;
            }
        }
    }

    return nullptr;
}


// find use pid
Resource* getResourceOfParcel(int parcelID) {

    for (int i = 0; i < 2; i++) {

        ResourceQueue* queue = allResourceQueue[i];

        for (int j = 0; j < queue->count; j++) {

            int position = (queue->front + j) % MAX_RESOURCE;
            Resource* resource = queue->resources[position];

            if (resource != nullptr &&
                resource->parcelID == parcelID) {

                return resource;
            }
        }
    }

    return nullptr;
}


// q1
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
    newResource->receiptID = 0;


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

    cout << "Receipt ID  : ";

    if (resource->receiptID == 0) {
        cout << "None" << endl;
    }
    else {
        cout << resource->receiptID << endl;
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
string assignParcelToResource(int parcelID, string resourceID) {

    Resource* resource = findResource(resourceID);

    if (resource == nullptr) {
        return "Resource ID not found.";
    }

    if (resource->status != "Available") {
        return "Resource is not available.";
    }

    resource->status = "Occupied";
    resource->parcelID = parcelID;
    resource->receiptID = 0;

    return "Parcel assigned successfully to " + resourceID;
}


// Assign Receipt to exact Resource ID
string assignReceiptToResource(int receiptID, string resourceID) {

    Resource* resource = findResource(resourceID);

    if (resource == nullptr) {
        return "Resource ID not found.";
    }

    if (resource->status != "Available") {
        return "Resource is not available.";
    }

    resource->status = "Occupied";
    resource->parcelID = 0;
    resource->receiptID = receiptID;

    return "Receipt assigned successfully to " + resourceID;
}


// Assign Parcel Menu
void assignParcelMenu() {

    int parcelID;
    string resourceID;

    cout << "\n========== ASSIGN PARCEL ==========\n";

    cout << "Enter Parcel ID: ";
    cin >> parcelID;

    cout << "Enter Resource ID (C01/D01): ";
    cin >> resourceID;

    string result = assignParcelToResource(parcelID, resourceID);

    cout << result << endl;
}


// Assign Receipt Menu
void assignReceiptMenu() {

    int receiptID;
    string resourceID;

    cout << "\n========== ASSIGN RECEIPT ==========\n";

    cout << "Enter Receipt ID: ";
    cin >> receiptID;

    cout << "Enter Resource ID (C01/D01): ";
    cin >> resourceID;

    string result = assignReceiptToResource(receiptID, resourceID);

    cout << result << endl;
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
    resource->receiptID = 0;

    cout << "Resource released successfully." << endl;

    return parcelID;
}

// Set Resource Occupied
bool setResourceOccupied(string resourceID) {

    Resource* resource = findResource(resourceID);

    if (resource == nullptr) {
        cout << "Resource not found." << endl;
        return false;
    }

    if (resource->status == "Occupied") {
        cout << "Resource is already occupied." << endl;
        return false;
    }

    if (resource->status == "Unavailable") {
        cout << "Cannot set an unavailable resource as occupied." << endl;
        return false;
    }

    resource->status = "Occupied";

    cout << "Resource set to occupied." << endl;

    return true;
}

// Set Resource Unavailable
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


// Set Resource Available
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


// // Add initial resources
// void seedResources() {

//     if (counterQueue->count == 0 &&
//         deliveryQueue->count == 0) {

//         registerNewResource("Counter");
//         registerNewResource("Counter");
//         registerNewResource("Delivery");
//         registerNewResource("Delivery");

//         cout << "\nInitial resources loaded.\n";
//     }
// }
