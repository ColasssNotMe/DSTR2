#include "iostream"
#include "task4.h"
#include <exception>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

extern const int MAX = 500;

int nextParcelID = 1001;

class Parcel {
public:
  int parcelID;
  string recipientName;
  string recipientID;
  string deliveryCompany;
  string parcelType;
  string arrivalDateTime;
  string status;
};

struct Queue {
  Parcel parcels[MAX];
  int front = -1;
  int rear = -1;
  int count = 0;
};

Queue normalParcels;

Queue toBeSpecialHandling;
Queue toBeDelivered;
Queue toBeCollectParcel;
Queue returnedParcel;
Queue onHoldInvestigaton;
Queue collectedParcel;

Queue *allQueue[] = {&normalParcels,      &toBeCollectParcel,
                     &toBeDelivered,      &toBeSpecialHandling,
                     &onHoldInvestigaton, &returnedParcel,
                     &collectedParcel};

Parcel result[MAX];

bool isFull(Queue queue) { return queue.count == MAX; }
bool isEmpty(Queue queue) { return queue.count == 0; }

void enqueue(Parcel parcel, Queue &queue) {
  if (isFull(queue)) {
    cout << "Max number of parcel reached. (500 parcels)" << endl;
    return;
  }
  if (isEmpty(queue)) {
    queue.front = 0;
  }
  queue.rear = (queue.rear + 1) % MAX;
  queue.parcels[queue.rear] = parcel;
  queue.count++;
  cout << "Added parcel into queue" << endl;
}

static void appendParcelToQueue(Parcel parcel, Queue &queue) {
  if (isFull(queue)) {
    return;
  }
  if (isEmpty(queue)) {
    queue.front = 0;
  }
  queue.rear = (queue.rear + 1) % MAX;
  queue.parcels[queue.rear] = parcel;
  queue.count++;
}

void clearAllQueues() {
  for (int i = 0; i < 7; i++) {
    allQueue[i]->front = -1;
    allQueue[i]->rear = -1;
    allQueue[i]->count = 0;
  }
}

void dequeue(Queue &queue) {
  if (isEmpty(queue)) {
    cout << "Queue is Empty! No customers to serve." << endl;
    return;
  }
  cout << "Parcel " << queue.parcels[queue.front].parcelID
       << " is being served." << endl;
  queue.count--;
  if (queue.front == queue.rear) {
    queue.front = queue.rear = -1;
  } else {
    queue.front = (queue.front + 1) % MAX;
  }
}

bool removeParcel(Queue &queue, int parcelID) {
  if (isEmpty(queue)) {
    return false;
  }

  int current = queue.front;
  for (int i = 0; i < queue.count; i++) {
    if (queue.parcels[current].parcelID == parcelID) {
      int next = (current + 1) % MAX;
      while (current != queue.rear) {
        queue.parcels[current] = queue.parcels[next];
        current = next;
        next = (next + 1) % MAX;
      }
      queue.parcels[queue.rear] = Parcel{};
      queue.count--;

      if (queue.count == 0) {
        queue.front = -1;
        queue.rear = -1;
      } else {
        queue.rear = (queue.rear - 1 + MAX) % MAX;
      }
      return true;
    }
    current = (current + 1) % MAX;
  }
  return false;
}

Queue *findParcelQueue(int parcelID) {
  for (int i = 0; i < 7; i++) {
    Queue *currentQueue = allQueue[i];
    int j = currentQueue->front;
    for (int count = 0; count < currentQueue->count; count++) {
      if (currentQueue->parcels[j].parcelID == parcelID) {
        return currentQueue;
      }
      j = (j + 1) % MAX;
    }
  }
  return nullptr;
}

bool isParcelCollected(int parcelID) {
  Queue *currentQueue = findParcelQueue(parcelID);
  if (currentQueue == nullptr) {
    return false;
  }
  if (currentQueue == &collectedParcel) {
    return true;
  }
  int current = currentQueue->front;
  for (int i = 0; i < currentQueue->count; i++) {
    if (currentQueue->parcels[current].parcelID == parcelID) {
      return currentQueue->parcels[current].status == "Collected";
    }
    current = (current + 1) % MAX;
  }
  return false;
}

bool hasRecipientIDWithDifferentName(string recipientID, string recipientName) {
  for (int i = 0; i < 7; i++) {
    Queue *currentQueue = allQueue[i];
    if (isEmpty(*currentQueue)) {
      continue;
    }
    int current = currentQueue->front;
    for (int j = 0; j < currentQueue->count; j++) {
      Parcel currentParcel = currentQueue->parcels[current];
      if (currentParcel.recipientID == recipientID &&
          currentParcel.recipientName != recipientName) {
        return true;
      }
      current = (current + 1) % MAX;
    }
  }
  return false;
}

// question 1
void registerNewParcel(string recipientName, string recipientID,
                       string deliveryCompany, string parcelType,
                       string arrivalDateTime, string status = "Sorting") {

  if (status.empty()) {
    status = "Sorting";
  }

  // TODO: verify the data if exist or smtg instead of just empty value
  if (recipientName == "") {
    cout << "Invalid Recipient Name" << endl;
    return;
  }
  if (deliveryCompany == "") {
    cout << "Invalid Delivery Company" << endl;
    return;
  }
  if (recipientID == "") {
    cout << "Invalid Recipient ID" << endl;
    return;
  }
  if (parcelType == "") {
    cout << "Invalid Parcel Type" << endl;
    return;
  }
  if (arrivalDateTime == "") {
    cout << "Invalid Arrival Date Time" << endl;
    return;
  }
  if (status == "") {
    cout << "Invalid Parcel Status" << endl;
    return;
  }

  if (hasRecipientIDWithDifferentName(recipientID, recipientName)) {
    cout << "Recipient ID " << recipientID
         << " already exists with a different recipient name." << endl;
    return;
  }

  Parcel parcelToBeAdd;
  parcelToBeAdd.parcelID = nextParcelID++;
  parcelToBeAdd.recipientName = recipientName;
  parcelToBeAdd.recipientID = recipientID;
  parcelToBeAdd.deliveryCompany = deliveryCompany;
  parcelToBeAdd.parcelType = parcelType;
  parcelToBeAdd.arrivalDateTime = arrivalDateTime;
  parcelToBeAdd.status = status;
  enqueue(parcelToBeAdd, normalParcels);
  cout << "Parcel added with Parcel ID: " << parcelToBeAdd.parcelID << endl;
  // FIXME: impossible for id duplicate

  // Task 4: Record parcel registration (initial record is non-reversible)
  recordActivity(parcelToBeAdd.parcelID, "REGISTER", "-", parcelToBeAdd.status,
                 "", "", false);
}

void serveParcel(Queue queue) { dequeue(queue); }

void printParcel(Parcel parcel) {
  cout << left << setw(10) << parcel.parcelID << setw(25)
       << parcel.recipientName << setw(15) << parcel.recipientID << setw(20)
       << parcel.deliveryCompany << setw(15) << parcel.parcelType << setw(25)
       << parcel.arrivalDateTime << setw(15) << parcel.status << endl;
}

void displayAllParcel(Queue queue) {
  if (isEmpty(queue)) {
    cout << "No parcels in the queue." << endl;
    return;
  }

  cout << left << setw(10) << "ID" << setw(25) << "Recipient Name" << setw(15)
       << "Recipient ID" << setw(20) << "Delivery Company" << setw(15)
       << "Parcel Type" << setw(25) << "Arrival Date/Time" << setw(15)
       << "Status" << endl;

  cout << string(125, '-') << endl;

  int i = queue.front;

  for (int count = 0; count < queue.count; count++) {
    printParcel(queue.parcels[i]);

    i = (i + 1) % MAX;
  }
}

void searchAllMatchingParcel(string searchTerm, int field) {
  int searchID = -1;
  if (field == 1) {
    try {
      searchID = stoi(searchTerm);
    } catch (const exception &e) {
      cout << "Invalid Parcel ID." << endl;
      return;
    }
  }

  int count = 0;

  cout << endl;
  cout << left << setw(10) << "ID" << setw(25) << "Recipient Name" << setw(15)
       << "Recipient ID" << setw(20) << "Delivery Company" << setw(15)
       << "Parcel Type" << setw(25) << "Arrival Date/Time" << setw(15)
       << "Status" << endl;

  cout << string(125, '-') << endl;
  for (int i = 0; i < 7; i++) {
    Queue *currentQueue = allQueue[i];
    if (isEmpty(*currentQueue)) {
      continue;
    }
    int current = currentQueue->front;
    for (int j = 0; j < currentQueue->count; j++) {
      Parcel currentParcel = currentQueue->parcels[current];
      bool matched = false;
      switch (field) {
      case 1:
        matched = (currentParcel.parcelID == searchID);
        break;

      case 2:
        matched = (currentParcel.recipientID == searchTerm);
        break;

      case 3:
        matched = (currentParcel.recipientName == searchTerm);
        break;

      case 4:
        matched = (currentParcel.deliveryCompany == searchTerm);
        break;

      case 5:
        matched = (currentParcel.parcelType == searchTerm);
        break;

      case 6:
        matched = (currentParcel.status == searchTerm);
        break;

      default:
        cout << "Invalid search field." << endl;
        return;
      }

      if (matched) {
        printParcel(currentParcel);
        count++;
      }
      current = (current + 1) % MAX;
    }
  }

  if (count == 0) {
    cout << "No parcel matches the search value." << endl;
  } else {
    cout << "Total matching parcels: " << count << endl;
  }
}

void viewParcelDetail(int parcelID) {
  Parcel *selectedParcel = nullptr;
  for (int i = 0; i < 7; i++) {
    Queue *currentQueue = allQueue[i];
    if (isEmpty(*currentQueue)) {
      continue;
    }
    int current = currentQueue->front;
    for (int j = 0; j < currentQueue->count; j++) {
      if (currentQueue->parcels[current].parcelID == parcelID) {
        selectedParcel = &currentQueue->parcels[current];
        break;
      }
      current = (current + 1) % MAX;
    }
    if (selectedParcel != nullptr) {
      break;
    }
  }

  if (selectedParcel == nullptr) {
    cout << "Parcel with ID " << parcelID << " not found." << endl;
    return;
  }

  cout << endl;
  cout << string(125, '=') << endl;

  cout << left << setw(10) << "ID" << setw(25) << "Recipient Name" << setw(15)
       << "Recipient ID" << setw(20) << "Delivery Company" << setw(15)
       << "Parcel Type" << setw(25) << "Arrival Date/Time" << setw(15)
       << "Status" << endl;

  cout << string(125, '-') << endl;
  printParcel(*selectedParcel);
  cout << string(125, '=') << endl;
}

void processParcel() {
  cout << endl << endl;
  if (isEmpty(normalParcels)) {
    cout << "No parcel waiting to be process" << endl;
    return;
  }

  Parcel currentParcel = normalParcels.parcels[normalParcels.front];
  string prevStatus = currentParcel.status;

  if (prevStatus == "Collected") {
    cout << "Parcel " << currentParcel.parcelID
         << " has already been collected and cannot be processed." << endl;
    return;
  }

  cout << "Current parcel waiting to be process: " << currentParcel.parcelID
       << endl;

  cout << "1. Recipient Collection" << endl;
  cout << "2. Campus Delivery" << endl;
  cout << "3. Special Handling" << endl;
  cout << "4. Return to Sender" << endl;
  cout << "5. Place on Hold" << endl;
  cout << "Enter your choice: " << endl;

  int choice;
  cin >> choice;

  switch (choice) {
  case 1:
    currentParcel.status = "Awaiting Collection";
    enqueue(currentParcel, toBeCollectParcel);
    cout << "Successfully added " << currentParcel.parcelID << " to "
         << currentParcel.status << endl;
    dequeue(normalParcels);
    recordActivity(currentParcel.parcelID, "PROCESS", prevStatus,
                   currentParcel.status, "", "", true);
    break;

  case 2:
    currentParcel.status = "Out for Delivery";
    enqueue(currentParcel, toBeDelivered);
    cout << "Successfully added " << currentParcel.parcelID << " to "
         << currentParcel.status << endl;
    dequeue(normalParcels);
    recordActivity(currentParcel.parcelID, "PROCESS", prevStatus,
                   currentParcel.status, "", "", true);
    break;

  case 3:
    currentParcel.status = "Special Handling";
    enqueue(currentParcel, toBeSpecialHandling);
    cout << "Successfully added " << currentParcel.parcelID << " to "
         << currentParcel.status << endl;
    dequeue(normalParcels);
    recordActivity(currentParcel.parcelID, "PROCESS", prevStatus,
                   currentParcel.status, "", "", true);
    break;

  case 4:
    currentParcel.status = "Return to Sender";
    enqueue(currentParcel, returnedParcel);
    cout << "Successfully added " << currentParcel.parcelID << " to "
         << currentParcel.status << endl;
    dequeue(normalParcels);
    recordActivity(currentParcel.parcelID, "PROCESS", prevStatus,
                   currentParcel.status, "", "", true);
    break;

  case 5:
    currentParcel.status = "On Hold";
    enqueue(currentParcel, onHoldInvestigaton);
    cout << "Successfully added " << currentParcel.parcelID << " to "
         << currentParcel.status << endl;
    dequeue(normalParcels);
    recordActivity(currentParcel.parcelID, "PROCESS", prevStatus,
                   currentParcel.status, "", "", true);
    break;

  default:
    cout << "Invalid selection." << endl;
    return;
  }
}

// Question 4
Parcel *searchSpecificParcel(string parcelID) {
  int id;
  try {
    id = stoi(parcelID);
  } catch (const exception &e) {
    cout << "Invalid Parcel ID." << endl;
    return nullptr;
  }
  for (int i = 0; i < 7; i++) {
    Queue *currentQueue = allQueue[i];
    if (isEmpty(*currentQueue)) {
      continue;
    }
    int current = currentQueue->front;
    for (int j = 0; j < currentQueue->count; j++) {
      if (currentQueue->parcels[current].parcelID == id) {
        return &currentQueue->parcels[current];
      }
      current = (current + 1) % MAX;
    }
  }
  cout << "No parcel with Parcel ID: " << parcelID << " found." << endl;
  return nullptr;
}

void updateParcel(string parcelID) {
  int id;

  try {
    id = stoi(parcelID);
  } catch (const exception &e) {
    cout << "Invalid Parcel ID" << endl;
    return;
  }

  if (isParcelCollected(id)) {
    cout << "Parcel " << id << " has already been collected and cannot be updated."
         << endl;
    return;
  }

  Queue *originalQueue = findParcelQueue(id);
  if (originalQueue == nullptr) {
    cout << "Parcel not found!" << endl;
    return;
  }
  Parcel updatedParcel;
  int current = originalQueue->front;
  for (int i = 0; i < originalQueue->count; i++) {
    if (originalQueue->parcels[current].parcelID == id) {
      updatedParcel = originalQueue->parcels[current];
      break;
    }
    current = (current + 1) % MAX;
  }

  string prevStatus = updatedParcel.status;

  cout << endl << endl;
  cout << "Select parcel status to update: " << endl;
  cout << "1. Collection" << endl;
  cout << "2. Delivery" << endl;
  cout << "3. Special Handling" << endl;
  cout << "4. On Hold" << endl;
  cout << "5. Return" << endl;
  cout << "6. Cancellation" << endl;
  cout << "7. Collected" << endl;
  cout << "0. Return to main menu" << endl;

  cout << "Enter selection: ";

  int selection;
  cin >> selection;

  Queue *destinationQueue = nullptr;

  switch (selection) {
  case 1:
    updatedParcel.status = "Awaiting Collection";
    destinationQueue = &toBeCollectParcel;
    break;

  case 2:
    updatedParcel.status = "Out for Delivery";
    destinationQueue = &toBeDelivered;
    break;

  case 3:
    updatedParcel.status = "Special Handling";
    destinationQueue = &toBeSpecialHandling;
    break;

  case 4:
    updatedParcel.status = "On Hold";
    destinationQueue = &onHoldInvestigaton;
    break;

  case 5:
    updatedParcel.status = "Return to Sender";
    destinationQueue = &returnedParcel;
    break;

  case 6:
    if (removeParcel(*originalQueue, id)) {
      cout << "Parcel " << id << " successfully cancelled." << endl;
      recordActivity(id, "CANCEL", prevStatus, "Cancelled", "", "", false);
    }
    return;

  case 7:
    updatedParcel.status = "Collected";
    destinationQueue = &collectedParcel;
    break;

  case 0:
    return;

  default:
    cout << "Invalid selection." << endl;
    return;
  }

  if (isFull(*destinationQueue)) {
    cout << "Destination queue is full. Unable to update parcel." << endl;
    return;
  }

  if (removeParcel(*originalQueue, id)) {
    enqueue(updatedParcel, *destinationQueue);
    cout << "Successfully updated parcel " << id << " to "
         << updatedParcel.status << endl;
    recordActivity(id, "UPDATE_STATUS", prevStatus, updatedParcel.status, "",
                   "", updatedParcel.status != "Collected");
  }
}

// Question 5
//  Integrate within the whole task1 so no need implementation

bool saveParcelsToFile(const string &filename) {
  ofstream outFile(filename);

  if (!outFile.is_open()) {
    cout << "Unable to open '" << filename << "' for writing." << endl;
    return false;
  }

  outFile << nextParcelID << endl;

  int totalParcels = 0;

  for (int queueIndex = 0; queueIndex < 7; queueIndex++) {
    Queue *queue = allQueue[queueIndex];
    int current = queue->front;

    for (int i = 0; i < queue->count; i++) {
      Parcel parcel = queue->parcels[current];

      outFile << queueIndex << '|' << parcel.parcelID << '|'
              << parcel.recipientName << '|' << parcel.recipientID << '|'
              << parcel.deliveryCompany << '|' << parcel.parcelType << '|'
              << parcel.arrivalDateTime << '|' << parcel.status << endl;

      current = (current + 1) % MAX;
      totalParcels++;
    }
  }

  outFile.close();

  if (outFile.fail()) {
    cout << "Error occurred while saving parcels to '" << filename << "'."
         << endl;
    return false;
  }

  cout << "Saved " << totalParcels << " parcel(s) to '" << filename << "'."
       << endl;
  return true;
}

bool loadParcelsFromFile(const string &filename) {
  ifstream inFile(filename);

  if (!inFile.is_open()) {
    return false;
  }

  string line;

  if (!getline(inFile, line)) {
    inFile.close();
    return false;
  }

  int savedNextParcelID;
  try {
    savedNextParcelID = stoi(line);
  } catch (const exception &e) {
    savedNextParcelID = 1001;
  }

  clearAllQueues();

  int loadedCount = 0;
  int highestParcelID = 0;

  while (getline(inFile, line)) {
    stringstream recordStream(line);
    string queueField;
    string parcelIDField;

    if (!getline(recordStream, queueField, '|') ||
        !getline(recordStream, parcelIDField, '|')) {
      continue;
    }

    int queueIndex;
    int parcelID;
    try {
      queueIndex = stoi(queueField);
      parcelID = stoi(parcelIDField);
    } catch (const exception &e) {
      continue;
    }

    if (queueIndex < 0 || queueIndex >= 7) {
      continue;
    }

    Parcel parcel;
    parcel.parcelID = parcelID;
    if (!getline(recordStream, parcel.recipientName, '|') ||
        !getline(recordStream, parcel.recipientID, '|') ||
        !getline(recordStream, parcel.deliveryCompany, '|') ||
        !getline(recordStream, parcel.parcelType, '|') ||
        !getline(recordStream, parcel.arrivalDateTime, '|') ||
        !getline(recordStream, parcel.status)) {
      continue;
    }

    if (!parcel.status.empty() && parcel.status.back() == '\r') {
      parcel.status.pop_back();
    }

    appendParcelToQueue(parcel, *allQueue[queueIndex]);

    loadedCount++;
    if (parcelID > highestParcelID) {
      highestParcelID = parcelID;
    }
  }

  inFile.close();

  nextParcelID = savedNextParcelID > highestParcelID ? savedNextParcelID
                                                     : highestParcelID + 1;

  cout << "Loaded " << loadedCount << " parcel(s) from '" << filename << "'."
       << endl;
  return true;
}
