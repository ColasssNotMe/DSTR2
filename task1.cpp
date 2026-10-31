#include "iostream"
#include <iomanip>
using namespace std;

const int MAX = 500;

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

bool isFull(Queue queue) { return queue.rear == MAX - 1; }
bool isEmpty(Queue queue) {
  return queue.front == -1 || queue.front > queue.rear;
}

void enqueue(Parcel parcel, Queue queue) {
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

void dequeue(Queue queue) {
  if (isEmpty(queue)) {
    cout << "Queue is Empty! No customers to serve.\n";
    return;
  }
  cout << "Parcel " << queue.parcels[queue.front].parcelID
       << " is being served.\n";
  queue.count--;
  if (queue.front == queue.rear) {
    queue.front = queue.rear = -1;
  } else {
    queue.front = (queue.front + 1) % MAX;
  }
}

// question 1
void registerNewParcel(string recipientName, string recipientID,
                       string deliveryCompany, string parcelType,
                       string arrivalDateTime, string status) {
  Parcel parcelToBeAdd;
  parcelToBeAdd.recipientName = recipientName;
  parcelToBeAdd.recipientID = recipientID;
  parcelToBeAdd.deliveryCompany = deliveryCompany;
  parcelToBeAdd.parcelType = parcelType;
  enqueue(parcelToBeAdd, normalParcels);
  // FIXME: need to check for duplicate, but how
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

  for (int i = queue.front; i <= queue.rear; i++) {
    printParcel(queue.parcels[i]);
  }
}

void searchForParcel(string searchTerm, int field) {
  Parcel result[MAX];
  int count = 0;
  switch (field) {
  case 1:
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < allQueue[i]->count; j++) {
        Parcel currentLoopParcel = allQueue[i]->parcels[j];
        if (currentLoopParcel.recipientID == searchTerm) {
          result[count] = currentLoopParcel;
          i++;
        }
      }
    }
  case 2:
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < allQueue[i]->count; j++) {
        Parcel currentLoopParcel = allQueue[i]->parcels[j];
        if (currentLoopParcel.recipientName == searchTerm) {
          result[count] = currentLoopParcel;
          i++;
        }
      }
    }
  case 3:
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < allQueue[i]->count; j++) {
        Parcel currentLoopParcel = allQueue[i]->parcels[j];
        if (currentLoopParcel.deliveryCompany == searchTerm) {
          result[count] = currentLoopParcel;
          i++;
        }
      }
    }
  case 4:
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < allQueue[i]->count; j++) {
        Parcel currentLoopParcel = allQueue[i]->parcels[j];
        if (currentLoopParcel.parcelType == searchTerm) {
          result[count] = currentLoopParcel;
          i++;
        }
      }
    }
  case 5:
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < allQueue[i]->count; j++) {
        Parcel currentLoopParcel = allQueue[i]->parcels[j];
        if (currentLoopParcel.status == searchTerm) {
          result[count] = currentLoopParcel;
          i++;
        }
      }
    }

  default:
    cout << "Invalid search term" << endl;
  }
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
