#include "task1.h"
// #include "task2.h"
#include "task3.h"
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

void option1Menu() {
  cout << string(50, '=') << endl;
  cout << "Unprocessed Parcel" << endl;
  cout << string(50, '=') << endl;
  displayAllParcel(normalParcels);
  cout << endl << endl;

  cout << string(50, '=') << endl;
  cout << "To Be Collect Parcel" << endl;
  cout << string(50, '=') << endl;
  displayAllParcel(toBeCollectParcel);
  cout << endl << endl;

  cout << string(50, '=') << endl;
  cout << "To Be Deliver Parcel" << endl;
  cout << string(50, '=') << endl;
  displayAllParcel(toBeDelivered);
  cout << endl << endl;

  cout << string(50, '=') << endl;
  cout << "To Be Special Handling Parcel" << endl;
  cout << string(50, '=') << endl;
  displayAllParcel(toBeSpecialHandling);
  cout << endl << endl;

  cout << string(50, '=') << endl;
  cout << "On Hold For Investigation Parcel" << endl;
  cout << string(50, '=') << endl;
  displayAllParcel(onHoldInvestigaton);
  cout << endl << endl;

  cout << string(50, '=') << endl;
  cout << "Waiting For Return Parcel" << endl;
  cout << string(50, '=') << endl;
  displayAllParcel(returnedParcel);
  cout << endl << endl;
}

void option2Menu() {
  int searchSelection;

  cout << endl << endl;
  while (true) {
    cout << "Search By:" << endl;
    cout << "1. Parcel ID" << endl;
    cout << "2. Recipient ID" << endl;
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
  bool haveParcel = searchSpecificParcel(parcelID) != nullptr ? true : false;
  if (haveParcel) {
    updateParcel(parcelID);
  } else {
    cout << "Invalid Parcel ID" << endl << endl;
    return;
  }
}

void option6Menu() {
  string parcelIDInput;
  cout << "Enter Parcel ID to delete: " << endl;
  cin >> parcelIDInput;

  int parcelID;
  try {
    parcelID = stoi(parcelIDInput);
  } catch (exception e) {
    cout << "Invalid Parcel ID" << endl;
    return;
  }

  Queue *parcelQueue = findParcelQueue(parcelID);
  if (parcelQueue == nullptr) {
    cout << "Parcel with ID " << parcelID << " not found." << endl;
    return;
  }

  if (removeParcel(*parcelQueue, parcelID)) {
    cout << "Parcel " << parcelID << " successfully deleted." << endl;
  } else {
    cout << "Failed to delete parcel " << parcelID << "." << endl;
  }
}

// END OF TASK 1

// void option7Menu() {
//   cout << string(50, '-') << endl;
//   cout << "Create Service Request" << endl;
//   cout << string(50, '-') << endl;
//
//   int parcelID, priorityLevel;
//   string serviceType, requestDateTime;
//
//   cout << "Enter Target Parcel ID: ";
//   cin >> parcelID;
//   cin.ignore();
//
//   cout << "Select Service Type:\n";
//   cout << "1. Campus Delivery\n";
//   cout << "2. Special Handling\n";
//   cout << "3. Recipient Collection\n";
//   cout << "4. Return to Sender\n";
//   cout << "Enter choice (1-4): ";
//   int choice;
//   cin >> choice;
//   cin.ignore();
//
//   switch (choice) {
//   case 1:
//     serviceType = "Campus Delivery";
//     priorityLevel = 2;
//     break;
//   case 2:
//     serviceType = "Special Handling";
//     priorityLevel = 1;
//     break;
//   case 3:
//     serviceType = "Recipient Collection";
//     priorityLevel = 3;
//     break;
//   case 4:
//     serviceType = "Return to Sender";
//     priorityLevel = 1;
//     break;
//   default:
//     cout << "Invalid choice. Setting default: Campus Delivery.\n";
//     serviceType = "Campus Delivery";
//     priorityLevel = 2;
//     break;
//   }
//
//   cout << "Enter Request Date/Time (e.g. 2026-11-05 10:00): ";
//   getline(cin, requestDateTime);
//
//   createServiceRequest(parcelID, serviceType, priorityLevel,
//   requestDateTime);
// }


// ================= TASK 3 =================

// Q1: Register Resource
void optionResource1() {
    int choice;

    cout << "\n========== Register Resource ==========\n";
    cout << "1. Counter\n";
    cout << "2. Delivery\n";
    cout << "0. Back\n";

    cout << "Enter selection: ";
    cin >> choice;

    if (choice == 1) {
        registerNewResource("Counter");
    }
    else if (choice == 2) {
        registerNewResource("Delivery");
    }
    else if (choice == 0) {
        return;
    }
    else {
        cout << "Invalid selection.\n";
    }
}


// Q2: Display Resources
void optionResource2() {
    cout << "\n========== Resource Availability ==========\n";

    displayAllResources();
}


// Q3: Assign Parcel to Resource
void optionResource3() {
    int parcelID;
    int requestID;
    int choice;

    string type;

    cout << "\n========== Assign Parcel ==========\n";

    cout << "Enter Parcel ID: ";
    cin >> parcelID;

    cout << "Enter Request ID: ";
    cin >> requestID;

    cout << "\nSelect Resource Type:\n";
    cout << "1. Counter\n";
    cout << "2. Delivery\n";

    cout << "Enter selection: ";
    cin >> choice;

    if (choice == 1) {
        type = "Counter";
    }
    else if (choice == 2) {
        type = "Delivery";
    }
    else {
        cout << "Invalid selection.\n";
        return;
    }

    assignParcelToResource(parcelID, requestID, type);
}


// Q4: Release Resource
void optionResource4() {
    string resourceID;

    cout << "\nEnter Resource ID to release: ";
    cin >> resourceID;

    int parcelID = releaseParcelResource(resourceID);

    if (parcelID != 0) {
        cout << "Parcel " << parcelID << " released.\n";
    }
}


// Set Resource Unavailable
void optionResource5() {
    string resourceID;

    cout << "\nEnter Resource ID: ";
    cin >> resourceID;

    setResourceUnavailable(resourceID);
}


// Set Resource Available
void optionResource6() {
    string resourceID;

    cout << "\nEnter Resource ID: ";
    cin >> resourceID;

    setResourceAvailable(resourceID);
}


// Resource Management Menu
void option8Menu() {
    int selection;

    while (true) {
        cout << "\n" << string(40, '=') << endl;
        cout << "Resource Management" << endl;
        cout << string(40, '=') << endl;

        cout << "1. Register New Resource" << endl;
        cout << "2. Display All Resources" << endl;
        cout << "3. Assign Parcel to Resource" << endl;
        cout << "4. Release Resource" << endl;
        cout << "5. Set Resource Unavailable" << endl;
        cout << "6. Set Resource Available" << endl;
        cout << "0. Back to Main Menu" << endl;

        cout << "Enter selection: ";
        cin >> selection;

        switch (selection) {
        case 1:
            optionResource1();
            break;

        case 2:
            optionResource2();
            break;

        case 3:
            optionResource3();
            break;

        case 4:
            optionResource4();
            break;

        case 5:
            optionResource5();
            break;

        case 6:
            optionResource6();
            break;

        case 0:
            return;

        default:
            cout << "Invalid selection. Please try again.\n";
            break;
        }
    }
}

void seedInitialData() {
  registerNewParcel("Alice Tan", "TP012345", "DHL", "Box", "2026-11-05 08:00",
                    "");
  registerNewParcel("Bob Lee", "TP067890", "FedEx", "Document",
                    "2026-11-05 08:30", "");
  cout << "[SYSTEM] Preloaded 2 initial parcels (ID 1001 & 1002).\n\n";
  seedResources();
}

void menu() {
  seedInitialData();

  while (true) {
    cout << endl << endl;
    cout << string(35, '=') << endl;
    cout << setw(12) << " " << "Main Menu" << endl;
    cout << string(35, '=') << endl;

    // question 1
    cout << "1. Display All Parcels" << endl;
    // question 2
    cout << "2. Search Parcel" << endl;
    cout << "3. View Parcel Detail" << endl;
    cout << "4. Sort & Process Incoming Parcel" << endl;
    cout << "5. Update Parcel Record" << endl;
    cout << "6. Delete Parcel Record" << endl;
    cout << "7. [Task 2] Create Service Request" << endl;
    cout << "8. [Task 3] Resource Management Module" << endl;
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
      // option7Menu();
      break;
    case 8:
        option8Menu();
    case 0:
      return;
    default:
      cout << "Invalid selection. Please try again.\n";
      break;
    }
  }
}

int main() { menu(); }
