#ifndef TASK3_H
#define TASK3_H

#include <string>

extern const int MAX_RESOURCE;

extern int nextCounterNumber;
extern int nextDeliveryNumber;

// Resource information
class Resource {
public:
    std::string resourceID;
    std::string type;
    std::string status;

    int parcelID = 0;
    int receiptID = 0;
};

// Resource Queue
struct ResourceQueue {
    Resource* resources[10];

    int front = -1;
    int rear = -1;
    int count = 0;
};

// Global resource queues
extern ResourceQueue* counterQueue;
extern ResourceQueue* deliveryQueue;
extern ResourceQueue* allResourceQueue[2];

// Queue functions
bool isResourceQueueFull(ResourceQueue*& queue);
bool isResourceQueueEmpty(ResourceQueue*& queue);

bool enqueueResource(Resource* resource, ResourceQueue*& queue);
bool dequeueResource(ResourceQueue*& queue, Resource*& resource);

// Find resource
Resource* findResource(std::string resourceID);
Resource* getResourceOfParcel(int parcelID);

// Q1
bool registerNewResource(std::string type);
void seedResources();

// Q2
void printResource(Resource* resource);
void displayAllResources();

// Q3
std::string assignParcelToResource(int parcelID, std::string resourceID);
std::string assignReceiptToResource(int receiptID, std::string resourceID);

void assignParcelMenu();
void assignReceiptMenu();
void optionResource3();

// Q4
int releaseParcelResource(std::string resourceID);
bool setResourceOccupied(std::string resourceID);
bool setResourceUnavailable(std::string resourceID);
bool setResourceAvailable(std::string resourceID);

#endif