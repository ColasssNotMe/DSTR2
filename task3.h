
#ifndef TASK3_H
#define TASK3_H

#include <string>

extern const int MAX_RESOURCE;

extern int nextCounterNumber;
extern int nextDeliveryNumber;

class Resource {
public:
    std::string resourceID;
    std::string type;
    std::string status;

    int parcelID = 0;
    int requestID = 0;
};

struct ResourceQueue {
    Resource* resources[10];

    int front = -1;
    int rear = -1;
    int count = 0;
};

extern ResourceQueue* counterQueue;
extern ResourceQueue* deliveryQueue;
extern ResourceQueue* allResourceQueue[2];

// Queue functions
bool isResourceQueueFull(ResourceQueue*& queue);
bool isResourceQueueEmpty(ResourceQueue*& queue);

bool enqueueResource(Resource* resource, ResourceQueue*& queue);
bool dequeueResource(ResourceQueue*& queue, Resource*& resource);

Resource* findResource(std::string resourceID);
Resource* getResourceOfParcel(int parcelID);

bool registerNewResource(std::string type);
void seedResources();

void printResource(Resource* resource);
void displayAllResources();

std::string assignParcelToResource(
    int parcelID,
    int requestID,
    std::string type
);

int releaseParcelResource(std::string resourceID);

bool setResourceUnavailable(std::string resourceID);
bool setResourceAvailable(std::string resourceID);

#endif
