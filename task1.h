#ifndef TASK1_H
#define TASK1_H

#include <string>

// Maximum number of parcels that can be stored in a queue.
extern const int MAX;

// Automatically assigned ID for the next new parcel.
extern int nextParcelID;

// Parcel record
class Parcel {
public:
  int parcelID;
  std::string recipientName;
  std::string recipientID;
  std::string deliveryCompany;
  std::string parcelType;
  std::string arrivalDateTime;
  std::string status;
};

// Circular queue for storing parcels
struct Queue {
  Parcel parcels[500];
  int front = -1;
  int rear = -1;
  int count = 0;
};

// Queue declarations
extern Queue normalParcels;
extern Queue toBeSpecialHandling;
extern Queue toBeDelivered;
extern Queue toBeCollectParcel;
extern Queue returnedParcel;
extern Queue onHoldInvestigaton;
extern Queue collectedParcel;

// Array containing pointers to all parcel queues
extern Queue *allQueue[7];

// Result array
extern Parcel result[500];

// Queue operations
bool isFull(Queue queue);
bool isEmpty(Queue queue);
void enqueue(Parcel parcel, Queue &queue);
void dequeue(Queue &queue);
bool removeParcel(Queue &queue, int parcelID);
Queue *findParcelQueue(int parcelID);
bool isParcelCollected(int parcelID);

// Question 1: Register and serve parcels
void registerNewParcel(std::string recipientName, std::string recipientID,
                       std::string deliveryCompany, std::string parcelType,
                       std::string arrivalDateTime,
                       std::string status = "Sorting");

void serveParcel(Queue queue);

// Display and search functions
void printParcel(Parcel parcel);
void displayAllParcel(Queue queue);
void searchAllMatchingParcel(std::string searchTerm, int field);
void viewParcelDetail(int parcelID);

// Question 3: Sort and process incoming parcels
void processParcel();

// Question 4: Search and update a specific parcel
Parcel *searchSpecificParcel(std::string parcelID);
void updateParcel(std::string parcelID);

void clearAllQueues();
bool saveParcelsToFile(const std::string &filename = "parcels.txt");
bool loadParcelsFromFile(const std::string &filename = "parcels.txt");

#endif // PARCEL_H
