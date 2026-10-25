#include "iostream"
#include <iomanip>
using namespace std;

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

const int MAX = 500;
Parcel parcels[MAX];

Parcel toBeSpecialHandling[MAX];
Parcel toBeDelivered[MAX];
Parcel toBeCollectParcel[MAX];
Parcel returnedParcel[MAX];
Parcel onHoldInvestigaton[MAX];

int front = -1;
int rear = -1;
int parcelCount = 0;

bool isFull() { return rear == MAX - 1; }
bool isEmpty() { return front == -1 || front > rear; }

void enqueue(Parcel parcel) {
  if (isFull()) {
    cout << "Max number of parcel reached. (500 parcels)" << endl;
    return;
  }
  if (isEmpty()) {
    front = 0;
  }
  rear = (rear + 1) % MAX;
  parcels[rear] = parcel;
  parcel.parcelID = rear;
  parcelCount++;
  cout << "Added parcel into queue" << endl;
}

void dequeue() {
  if (isEmpty()) {
    cout << "Queue is Empty! No customers to serve.\n";
    return;
  }
  cout << "Parcel " << parcels[front].parcelID << " is being served.\n";
  parcelCount--;
  if (front == rear) {
    front = rear = -1;
  } else {
    front = (front + 1) % MAX;
  }
}

void registerNewParcel(string recipientName, string recipientID,
                       string deliveryCompany, string parcelType,
                       string arrivalDateTime, string status) {
  Parcel parcelToBeAdd;
  parcelToBeAdd.recipientName = recipientName;
  parcelToBeAdd.recipientID = recipientID;
  parcelToBeAdd.deliveryCompany = deliveryCompany;
  parcelToBeAdd.parcelType = parcelType;
  enqueue(parcelToBeAdd);
  // FIXME: need to check for duplicate, but how
}
void serveParcel() { dequeue(); }

void printParcel(Parcel parcel) {
  cout << left << setw(10) << parcel.parcelID << setw(25)
       << parcel.recipientName << setw(15) << parcel.recipientID << setw(20)
       << parcel.deliveryCompany << setw(15) << parcel.parcelType << setw(25)
       << parcel.arrivalDateTime << setw(15) << parcel.status << endl;
}

void displayAllParcel() {
  if (isEmpty()) {
    cout << "No parcels in the queue." << endl;
    return;
  }

  cout << left << setw(10) << "ID" << setw(25) << "Recipient Name" << setw(15)
       << "Recipient ID" << setw(20) << "Delivery Company" << setw(15)
       << "Parcel Type" << setw(25) << "Arrival Date/Time" << setw(15)
       << "Status" << endl;

  cout << string(125, '-') << endl;

  for (int i = front; i <= rear; i++) {
    printParcel(parcels[i]);
  }
}

void searchForParcel(string searchTerm, int field) {
  Parcel result[MAX];
  int count = 0;
  switch (field) {
  case 1:
    for (int i = 0; i < parcelCount; i++) {
      if (parcels[i].recipientID == searchTerm) {
        result[count] = parcels[i];
        i++;
      }
    }
  case 2:
    for (int i = 0; i < parcelCount; i++) {
      if (parcels[i].recipientName == searchTerm) {
        result[count] = parcels[i];
        i++;
      }
    }
  case 3:
    for (int i = 0; i < parcelCount; i++) {
      if (parcels[i].deliveryCompany == searchTerm) {
        result[count] = parcels[i];
        i++;
      }
    }
  case 4:
    for (int i = 0; i < parcelCount; i++) {
      if (parcels[i].parcelType == searchTerm) {
        result[count] = parcels[i];
        i++;
      }
    }
  case 5:
    for (int i = 0; i < parcelCount; i++) {
      if (parcels[i].status == searchTerm) {
        result[count] = parcels[i];
        i++;
      }
    }

  default:
    cout << "Invalid search term" << endl;
  }
}

void viewParcelDetail(int parcelID) {
  Parcel selectedParcel;
  for (int i = 0; i < parcelCount; i++) {
    if (parcels[i].parcelID == parcelID) {
      selectedParcel = parcels[i];
    }
  }
}
