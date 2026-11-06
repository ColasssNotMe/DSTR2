#include "iostream"
#include <algorithm>
#include <exception>
#include <iomanip>
#include <string>
using namespace std;

const int MAX = 500;

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
  int front;
  int rear;
  int count;
};

Queue normalParcels;

Queue toBeSpecialHandling;
Queue toBeDelivered;
Queue toBeCollectParcel;
Queue returnedParcel;
Queue onHoldInvestigaton;

Queue *allQueue[] = {&normalParcels, &toBeCollectParcel, &toBeDelivered,
                     &onHoldInvestigaton, &returnedParcel};

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
  parcel.parcelID = queue.rear;
  queue.count++;
  cout << "Added parcel into queue" << endl;
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

void removeParcel(Queue &queue, string parcelID) {
  if (isEmpty(queue)) {
    cout << "Queue is Empty! Unable to remove parcel." << endl;
    return;
  }

  try {
    stoi(parcelID);
  } catch (exception e) {
    cerr << "Error converting parcel ID to integer" << endl;
    return;
  }

  bool foundIndex = false;
  for (int i = queue.front;; i = (i + 1) % MAX) {
    if (!foundIndex) {
      if (queue.parcels[i].parcelID == stoi(parcelID)) {
        queue.parcels[i] = Parcel{};
        queue.count--;
        foundIndex = true;

        // Shift forward
        int current = i;
        while (current != queue.rear) {
          int next = (current + 1) % MAX;
          queue.parcels[current] = queue.parcels[next];
          current = next;
        }
        queue.parcels[queue.rear] = Parcel{};

        if (queue.front == queue.rear) {
          queue.front = queue.rear = -1;
        } else {
          queue.front = (queue.front + 1) % MAX;
        }

        queue.count--;
        break;
      }
    }
    if (i == queue.rear) {
      break;
    }
  }
  if (!foundIndex) {
    cout << "Parcel not found!" << endl;
  }
}

// question 1
void registerNewParcel(string recipientName, string recipientID,
                       string deliveryCompany, string parcelType,
                       string arrivalDateTime, string status) {
  // Q5 validation
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

  Parcel parcelToBeAdd;
  parcelToBeAdd.parcelID = nextParcelID++;
  parcelToBeAdd.recipientName = recipientName;
  parcelToBeAdd.recipientID = recipientID;
  parcelToBeAdd.deliveryCompany = deliveryCompany;
  parcelToBeAdd.parcelType = parcelType;
  enqueue(parcelToBeAdd, normalParcels);
  cout << "Parcel added with Parcel ID: " << parcelToBeAdd.parcelID << endl;
  // FIXME: impossible for id duplicate
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
  int count = 0;
  switch (field) {
  case 1:
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < allQueue[i]->count; j++) {
        Parcel currentLoopParcel = allQueue[i]->parcels[j];
        int parcelID;
        try {
          parcelID = stoi(searchTerm);
        } catch (exception e) {
          cerr << "Error while attempting to convert search term to parcel ID"
               << endl;
          return;
        }
        if (currentLoopParcel.parcelID == parcelID) {
          result[count] = currentLoopParcel;
          i++;
        }
      }
    }
    break;
  case 2:
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < allQueue[i]->count; j++) {
        Parcel currentLoopParcel = allQueue[i]->parcels[j];
        if (currentLoopParcel.recipientID == searchTerm) {
          result[count] = currentLoopParcel;
          i++;
        }
      }
    }
    break;
  case 3:
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < allQueue[i]->count; j++) {
        Parcel currentLoopParcel = allQueue[i]->parcels[j];
        if (currentLoopParcel.recipientName == searchTerm) {
          result[count] = currentLoopParcel;
          i++;
        }
      }
    }
    break;
  case 4:
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < allQueue[i]->count; j++) {
        Parcel currentLoopParcel = allQueue[i]->parcels[j];
        if (currentLoopParcel.deliveryCompany == searchTerm) {
          result[count] = currentLoopParcel;
          i++;
        }
      }
    }
    break;

  case 5:
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < allQueue[i]->count; j++) {
        Parcel currentLoopParcel = allQueue[i]->parcels[j];
        if (currentLoopParcel.parcelType == searchTerm) {
          result[count] = currentLoopParcel;
          i++;
        }
      }
    }
    break;
  case 6:
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < allQueue[i]->count; j++) {
        Parcel currentLoopParcel = allQueue[i]->parcels[j];
        if (currentLoopParcel.status == searchTerm) {
          result[count] = currentLoopParcel;
          i++;
        }
      }
    }
    break;

  default:
    cout << "Invalid search term" << endl;
  }

  // NOTE: the return result is in result[]
}

void viewParcelDetail(int parcelID) {
  Parcel selectedParcel;
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < allQueue[i]->count; j++) {
      Parcel currentLoopParcel = allQueue[i]->parcels[j];
      if (currentLoopParcel.parcelID == parcelID) {
        selectedParcel = currentLoopParcel;
        i++;
      }
    }
  }

  cout << string("-", 50) << endl;
  cout << "Parcel Detail" << endl;
  cout << "ID: " << selectedParcel.parcelID << endl;
  cout << "Type: " << selectedParcel.parcelType << endl;
  cout << "Arrival date time: " << selectedParcel.arrivalDateTime << endl;
  cout << "Delivery Company: " << selectedParcel.deliveryCompany << endl;
  cout << "Recipient ID:" << selectedParcel.recipientID << endl;
  cout << "Recipient Name:" << selectedParcel.recipientName << endl;
  cout << "Status: " << selectedParcel.status << endl;
  cout << string("-", 50) << endl;
}

void processParcel() {
  if (isEmpty(normalParcels)) {
    cout << "No parcel waiting to be process" << endl;
    return;
  }

  Parcel currentParcel = normalParcels.parcels[normalParcels.front];

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
    break;

  case 2:
    currentParcel.status = "Out for Delivery";
    enqueue(currentParcel, toBeDelivered);
    cout << "Successfully added " << currentParcel.parcelID << " to "
         << currentParcel.status << endl;
    break;

  case 3:
    currentParcel.status = "Special Handling";
    enqueue(currentParcel, toBeSpecialHandling);
    cout << "Successfully added " << currentParcel.parcelID << " to "
         << currentParcel.status << endl;
    break;

  case 4:
    currentParcel.status = "Return to Sender";
    enqueue(currentParcel, returnedParcel);
    cout << "Successfully added " << currentParcel.parcelID << " to "
         << currentParcel.status << endl;
    break;

  case 5:
    currentParcel.status = "On Hold";
    enqueue(currentParcel, onHoldInvestigaton);
    cout << "Successfully added " << currentParcel.parcelID << " to "
         << currentParcel.status << endl;
    break;

  default:
    cout << "Invalid selection." << endl;
    return;
  }
}

// Question 4
// TODO: verify the pointer thing
Parcel *searchSpecificParcel(string parcelID) {
  Parcel *found;
  int parcelIDint;
  try {
    parcelIDint = stoi(parcelID);
  } catch (exception e) {
    cerr << "Error while attempting to convert search term to parcel ID"
         << endl;
    return found;
  }

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < allQueue[i]->count; j++) {
      Parcel *currentLoopParcel = &allQueue[i]->parcels[j];
      if (currentLoopParcel->parcelID == parcelIDint) {
        return currentLoopParcel;
      }
    }
  }

  cout << "No parcel with parcel ID: " << parcelID << " found" << endl;
  return found;
}

void updateParcel(string parcelID) {
  cout << "Select parcel status to update: " << endl;
  cout << "1. Collection" << endl;
  cout << "2. Delivery" << endl;
  cout << "3. Special Handling" << endl;
  cout << "4. On Hold" << endl;
  cout << "5. Return" << endl;
  cout << "6. Cancellation" << endl;
  cout << "0. Return to main menu" << endl;

  int selection;
  cin >> selection;

  Parcel *tobeModify;
  switch (selection) {
  case 1:
    tobeModify = searchSpecificParcel(parcelID);
    tobeModify->status = "Collection";
    cout << "Successfully update status of " << parcelID << endl;
    break;

  case 2:
    tobeModify = searchSpecificParcel(parcelID);
    tobeModify->status = "Delivery";
    cout << "Successfully update status of " << parcelID << endl;
    break;

  case 3:
    tobeModify = searchSpecificParcel(parcelID);
    tobeModify->status = "Return";
    cout << "Successfully update status of " << parcelID << endl;
    break;
  case 4:
    tobeModify = searchSpecificParcel(parcelID);
    tobeModify->status = "Cancellation";
    cout << "Successfully update status of " << parcelID << endl;
    break;

  default:
    cout << "Invalid selection, returning to main menu" << endl;
    return;
  }
}

// TODO:
void removeParcel(string parcelID) {
  Parcel *parcelToBeRemoved;

  parcelToBeRemoved = searchSpecificParcel(parcelID);
}

// Question 5
//  Integrate within the whole task1 so no need implementation
