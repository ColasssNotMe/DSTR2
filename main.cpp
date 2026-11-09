#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

void optionAddParcelMenu() {
  string recipientName;
  string recipientID;
  string deliveryCompany;
  string parcelType;
  string arrivalDateTime;

  cout << endl << endl;
  cin.ignore();

  cout << "Enter recipient name: ";
  getline(cin, recipientName);

  cout << "Enter recipient ID: ";
  getline(cin, recipientID);

  cout << "Enter delivery company: ";
  getline(cin, deliveryCompany);

  cout << "Enter parcel type: ";
  getline(cin, parcelType);

  cout << "Enter arrival date/time (e.g. 2026-11-09 10:00): ";
  getline(cin, arrivalDateTime);

  registerNewParcel(recipientName, recipientID, deliveryCompany, parcelType,
                    arrivalDateTime);
}

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

// ================= TASK 2 SUBMENU =================

void option7Menu_Create() {
  cout << "\n" << string(40, '-') << endl;
  cout << "  Create Service Request" << endl;
  cout << string(40, '-') << endl;

  int parcelID, priorityLevel;
  string serviceType, requestDateTime;

  cout << "Enter Target Parcel ID: ";
  cin >> parcelID;
  cin.ignore();

  cout << "\nSelect Service Type:\n";
  cout << "1. Special Handling (Priority 1 - High)\n";
  cout << "2. Return to Sender (Priority 1 - High)\n";
  cout << "3. Campus Delivery (Priority 2 - Medium)\n";
  cout << "4. Recipient Collection (Priority 3 - Low)\n";
  cout << "Enter choice (1-4): ";
  int choice;
  cin >> choice;
  cin.ignore();

  switch (choice) {
  case 1:
    serviceType = "Special Handling";
    priorityLevel = 1;
    break;
  case 2:
    serviceType = "Return to Sender";
    priorityLevel = 1;
    break;
  case 3:
    serviceType = "Campus Delivery";
    priorityLevel = 2;
    break;
  case 4:
    serviceType = "Recipient Collection";
    priorityLevel = 3;
    break;
  default:
    cout << "Invalid choice. Setting default: Campus Delivery.\n";
    serviceType = "Campus Delivery";
    priorityLevel = 2;
    break;
  }

  cout << "Enter Request Date/Time (e.g. 2026-11-09 10:00): ";
  getline(cin, requestDateTime);

  createServiceRequest(parcelID, serviceType, priorityLevel, requestDateTime);
}

void option7Menu() {
  int selection;
  while (true) {
    cout << "\n" << string(45, '=') << endl;
    cout << "  Task 2: Service Request Management" << endl;
    cout << string(45, '=') << endl;
    cout << "1. Create Service Request" << endl;
    cout << "2. Display Active Service Requests" << endl;
    cout << "3. Determine & View Next Request to Handle" << endl;
    cout << "4. Process Service Request (Complete)" << endl;
    cout << "5. Cancel Service Request" << endl;
    cout << "6. Save Active Service Requests to File" << endl;
    cout << "0. Back to Main Menu" << endl;

    cout << "Enter selection: ";
    cin >> selection;

    switch (selection) {
    case 1:
      option7Menu_Create();
      break;
    case 2:
      displayActiveRequests();
      break;
    case 3: {
      ServiceRequest* nextReq = getNextRequest();
      if (nextReq != nullptr) {
        cout << "\n--- NEXT REQUEST TO PROCESS (Highest Priority) ---\n";
        cout << "Request ID : " << nextReq->requestID << endl;
        cout << "Parcel ID  : " << nextReq->parcelID << endl;
        cout << "Recipient  : " << nextReq->recipientName << endl;
        cout << "Service    : " << nextReq->serviceType << endl;
        cout << "Priority   : " << nextReq->priorityLevel << endl;
        cout << "--------------------------------------------------\n";
      }
      break;
    }
    case 4: {
      int reqID;
      cout << "\nEnter Request ID to Complete: ";
      cin >> reqID;
      processServiceRequest(reqID, "Completed");
      break;
    }
    case 5: {
      int reqID;
      cout << "\nEnter Request ID to Cancel: ";
      cin >> reqID;
      cancelServiceRequest(reqID);
      break;
    }
    case 6: {
    saveRequestsToFile("service_requests.txt");
    break;
}
    case 0:
      return;
    default:
      cout << "Invalid selection. Please try again.\n";
      break;
    }
  }
}

// ================= TASK 3 =================

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

void optionResource2() {
    cout << "\n========== Resource Availability ==========\n";
    displayAllResources();
}

void optionResource3() {
    int choice;

    cout << "\n========== ASSIGN RESOURCE ==========\n";
    cout << "1. Assign Parcel" << endl;
    cout << "2. Assign Receipt" << endl;
    cout << "0. Back" << endl;

    cout << "Enter selection: ";
    cin >> choice;

    switch (choice) {
    case 1:
        assignParcelMenu();
        break;
    case 2:
        assignReceiptMenu();
        break;
    case 0:
        return;
    default:
        cout << "Invalid selection." << endl;
        break;
    }
}

void optionResource4() {
    string resourceID;

    cout << "\nEnter Resource ID to release: ";
    cin >> resourceID;

    int parcelID = releaseParcelResource(resourceID);

    if (parcelID != 0) {
        cout << "Parcel " << parcelID << " released.\n";
    }
}

void optionResource5() {
    string resourceID;

    cout << "\nEnter Resource ID: ";
    cin >> resourceID;

    setResourceUnavailable(resourceID);
}

void optionResource6() {
    string resourceID;

    cout << "\nEnter Resource ID: ";
    cin >> resourceID;

    setResourceAvailable(resourceID);
}

void optionResource7() {
    string resourceID;

    cout << "\nEnter Resource ID: ";
    cin >> resourceID;

    setResourceOccupied(resourceID);
}

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
        cout << "7. Set Resource Occupied" << endl;
        cout << "0. Back to Main Menu" << endl;

        cout << "Enter selection: ";
        cin >> selection;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input.\n";
            continue;
        }

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
        case 7:
            optionResource7();
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
  registerNewParcel("Alice Tan", "TP012345", "DHL", "Box", "2026-11-05 08:00", "");
  registerNewParcel("Bob Lee", "TP067890", "FedEx", "Document", "2026-11-05 08:30", "");
  cout << "[SYSTEM] Preloaded 2 initial parcels (ID 1001 & 1002).\n\n";
}

void menu() {
  if (!loadParcelsFromFile()) {
    seedInitialData();
  }

  loadResources();
  if (isResourceQueueEmpty(counterQueue) && isResourceQueueEmpty(deliveryQueue)) {
      seedResources();  
  }

  while (true) {
    cout << endl << endl;
    cout << string(35, '=') << endl;
    cout << setw(12) << " " << "Main Menu" << endl;
    cout << string(35, '=') << endl;

    cout << "1. Add A Parcel" << endl;
    cout << "2. Display All Parcels" << endl;
    cout << "3. Search Parcel" << endl;
    cout << "4. View Parcel Detail" << endl;
    cout << "5. Sort & Process Incoming Parcel" << endl;
    cout << "6. Update Parcel Record" << endl;
    cout << "7. Delete Parcel Record" << endl;
    cout << "8. [Task 2] Service Request Management" << endl;
    cout << "9. [Task 3] Resource Management Module" << endl;
    cout << "10. [Task 4] Parcel Activity & Record Management" << endl;
    cout << "11. Save Parcels to File" << endl;
    cout << "0. Exit" << endl;

    int selection;
    cout << "Enter selection: ";
    cin >> selection;

    switch (selection) {
    case 1:
      optionAddParcelMenu();
      break;
    case 2:
      option1Menu();
      break;
    case 3:
      option2Menu();
      break;
    case 4:
      option3Menu();
      break;
    case 5:
      processParcel();
      break;
    case 6:
      option5Menu();
      break;
    case 7:
      option6Menu();
      break;
    case 8:
      option7Menu();
      break;
    case 9:
      option8Menu();
      break;
    case 10:
      task4Menu();
      break;
    case 11:
      saveParcelsToFile();
      break;
    case 0:
      return;
    default:
      cout << "Invalid selection. Please try again.\n";
      break;
    }
  }
}

int main() { menu(); }