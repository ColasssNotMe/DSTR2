#ifndef TASK1_H
#define TASK1_H

#include <string>
using namespace std;

class Parcel {
public:
  string parcelID;
  string recipientName;
  string recipientID;
  string deliveryCompany;
  string parcelType;
  string arrivalDateTime;
  string status;
};

void registerNewParcel(string recipientName, string recipientID,
                       string deliveryCompany, string parcelType,
                       string arrivalDateTime, string status);

void displayAllParcel();

void searchForParcel(string searchTerm, int field);
void viewParcelDetail(int parcelID);
#endif
