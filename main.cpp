#include <iostream>
#include <iomanip>
#include <string>
#include "task1.h"
#include "task2.h"

using namespace std;

void option1Menu() {
  cout << string(50, '-') << endl;
  cout << "Unprocessed Parcel" << endl;
  cout << string(50, '-') << endl;
  displayAllParcel(normalParcels);

  cout << string(50, '-') << endl;
  cout << "To Be Collect Parcel" << endl;
  cout << string(50, '-') << endl;
  displayAllParcel(toBeCollectParcel);

  cout << string(50, '-') << endl;
  cout << "To Be Deliver Parcel" << endl;
  cout << string(50, '-') << endl;
  displayAllParcel(toBeDelivered);

  cout << string(50, '-') << endl;
  cout << "To Be Special Handling Parcel" << endl;
  cout << string(50, '-') << endl;
  displayAllParcel(toBeSpecialHandling);

  cout << string(50, '-') << endl;
  cout << "On Hold For Investigation Parcel" << endl;
  cout << string(50, '-') << endl;
  displayAllParcel(onHoldInvestigaton);

  cout << string(50, '-') << endl;
  cout << "Waiting For Return Parcel" << endl;
  cout << string(50, '-') << endl;
  displayAllParcel(returnedParcel);
}

void option2Menu() {
  int searchSelection;

  while (true) {
    cout << "Search By:" << endl;
    cout << "1. Parcel ID" << endl;
    cout << "2. Recipient ID" << endl;
    cout << "3. Recipient Name" << endl;
    cout << "4. Delivery Company" << endl;
    cout << "5. Parcel Type" << endl;
    cout << "6. Status" << endl;
    cout << "0. Back to menu" << endl;

    cout << "Enter selection: ";
    cin >> searchSelection;

    if (searchSelection >= 0 && searchSelection <= 6) {
      break;
    }

    cout << "Invalid selection. Please try again.\n";
  }

  if (searchSelection == 0) {
    return;
  }

  string searchTerm;
  cout << "Enter search term to be searched: ";
  cin >> searchTerm;

  searchAllMatchingParcel(searchTerm, searchSelection);
}

void option3Menu() {
  cout << "Enter parcel ID: ";
  int parcelID;
  cin >> parcelID;
  viewParcelDetail(parcelID);
}

void option5Menu() {
  string parcelID;
  cout << "Enter Parcel ID to update: " << endl;
  cin >> parcelID;
  updateParcel(parcelID);
}

void option6Menu() {
  string parcelID;
  cout << "Enter Parcel ID to delete: " << endl;
  cin >> parcelID;
  removeParcel(parcelID);
}

void option7Menu() {
  cout << string(50, '-') << endl;
  cout << "Create Service Request" << endl;
  cout << string(50, '-') << endl;

  int parcelID, priorityLevel;
  string serviceType, requestDateTime;

  cout << "Enter Target Parcel ID: ";
  cin >> parcelID;
  cin.ignore(); 

  cout << "Select Service Type:\n";
  cout << "1. Campus Delivery\n";
  cout << "2. Special Handling\n";
  cout << "3. Recipient Collection\n";
  cout << "4. Return to Sender\n";
  cout << "Enter choice (1-4): ";
  int choice;
  cin >> choice;
  cin.ignore();

  switch (choice) {
    case 1: serviceType = "Campus Delivery"; priorityLevel = 2; break;
    case 2: serviceType = "Special Handling"; priorityLevel = 1; break;
    case 3: serviceType = "Recipient Collection"; priorityLevel = 3; break;
    case 4: serviceType = "Return to Sender"; priorityLevel = 1; break;
    default:
      cout << "Invalid choice. Setting default: Campus Delivery.\n";
      serviceType = "Campus Delivery";
      priorityLevel = 2;
      break;
  }

  cout << "Enter Request Date/Time (e.g. 2026-11-05 10:00): ";
  getline(cin, requestDateTime);

  createServiceRequest(parcelID, serviceType, priorityLevel, requestDateTime);
}

void seedInitialData() {
  registerNewParcel("Alice Tan", "TP012345", "DHL", "Box", "2026-11-05 08:00", "Received");
  registerNewParcel("Bob Lee", "TP067890", "FedEx", "Document", "2026-11-05 08:30", "Received");
  cout << "[SYSTEM] Preloaded 2 initial parcels (ID 1001 & 1002).\n\n";
}

void menu() {
  seedInitialData();

  while (true) {
    cout << string(35, '=') << endl;
    cout << setw(12) << " " << "Main Menu" << endl;
    cout << string(35, '=') << endl;

    //question 1 
    cout << "1. Display All Parcels" << endl;
    //question 2
    cout << "2. Search Parcel" << endl;
    cout << "3. View Parcel Detail" << endl;
    cout << "4. Sort & Process Incoming Parcel" << endl;
    // TODO: havent do 5
    cout << "5. Update Parcel Record" << endl;
    cout << "6. Delete Parcel Record" << endl;
    cout << "7. [Task 2] Create Service Request" << endl;
    cout << "0. Exit" << endl;

    int selection;
    cout << "Enter selection: ";
    cin >> selection;

    switch (selection) {
      case 1:
        option1Menu();
        break;
      case 2:
        option2Menu();
        break;
      case 3:
        option3Menu();
        break;
      case 4:
        processParcel();
        break;
      case 5:
        option5Menu();
        break;
      case 6:
        option6Menu();
        break;
      case 7:
        option7Menu();
        break;
      case 0:
        return;
      default:
        cout << "Invalid selection. Please try again.\n";
        break;
    }
  }
}

int main() { menu();}