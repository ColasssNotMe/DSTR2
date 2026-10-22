#include "iostream"
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
int front = -1;
int rear = -1;
int size = 0;

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
  ++rear;
  parcels[rear] = parcel;
  parcel.parcelID = rear;
  cout << "Added parcel into queue" << endl;
}

void dequeue() {
  if (isEmpty()) {
    cout << "Queue is Empty! No customers to serve.\n";
    return;
  }
  cout << "Parcel " << parcels[front].parcelID << " is being served.\n";
  front++;
}

void registerNewParcel(string recipientName, string recipientID,
                       string deliveryCompany, string parcelType,
                       string arrivalDateTime, string status) {
  Parcel parcelToBeAdd;
  parcelToBeAdd.recipientName = recipientName;
  parcelToBeAdd.recipientID = recipientID;
  parcelToBeAdd.deliveryCompany = deliveryCompany;
  parcelToBeAdd.parcelType = parcelType;
}
