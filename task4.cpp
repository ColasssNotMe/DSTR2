// Task 4 - Parcel Activity and Record Management Module
// Stack with linkedlist implementation

#include "task4.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

// pointers
Activity* stackTop = nullptr; // Top of the stack (most recent undoable activity)
Activity* logHead = nullptr; // Oldest activity in the log
Activity* logTail = nullptr; // Newest activity in the log
int logCount = 0;
int nextActivityID = 1;

// Task 1 function
// check if parcel exist
bool ParcelExists(int parcelID) {
    return findParcelQueue(parcelID) != nullptr;
}

// get parcel current status
string GetParcelStatus(int parcelID) {
    if (!ParcelExists(parcelID)) {
        return "";
    }
    Parcel* p = searchSpecificParcel(to_string(parcelID));
    if (p == nullptr) {
        return "";
    }
    return p->status;
}

// find specific queue based on status
static Queue* getQueueByStatus(const string& status) {
    if (status == "Sorting" || status == "Unprocessed" || status == "Received" || status == "-" || status == "None") {
        return &normalParcels;
    }
    if (status == "Awaiting Collection" || status == "Collection") {
        return &toBeCollectParcel;
    }
    if (status == "Out for Delivery" || status == "Delivery") {
        return &toBeDelivered;
    }
    if (status == "Special Handling") {
        return &toBeSpecialHandling;
    }
    if (status == "On Hold") {
        return &onHoldInvestigaton;
    }
    if (status == "Return to Sender" || status == "Return") {
        return &returnedParcel;
    }
    if (status == "Collected") {
        return &collectedParcel;
    }
    return nullptr;
}

// set parcel status
bool SetParcelStatus(int parcelID, const string& targetStatus) {
    Queue* currentQueue = findParcelQueue(parcelID);
    if (currentQueue == nullptr) {
        return false;
    }

    Parcel* p = searchSpecificParcel(to_string(parcelID));
    if (p == nullptr) {
        return false;
    }

    Parcel parcelCopy = *p;
    parcelCopy.status = targetStatus;

    Queue* destQueue = getQueueByStatus(targetStatus);
    if (destQueue == nullptr) {
        destQueue = currentQueue;
    }
    if (destQueue != currentQueue) {
        if (removeParcel(*currentQueue, parcelID)) {
            enqueue(parcelCopy, *destQueue);
            return true;
        }
        return false;
    } else {
        p->status = targetStatus;
        return true;
    }
}

// helper functions
// get current timestamp in YYYY-MM-DD HH:MM:SS format
string currentTimestamp() {
    time_t now = time(nullptr);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buffer);
}

// for input validation
bool readInt(const string& prompt, int& value) {
    cout << prompt;
    string line;
    getline(cin, line);
    if (line.empty() || line.length() > 9) return false;
    for (size_t i = 0; i < line.length(); i++) {
        if (line[i] < '0' || line[i] > '9') return false;
    }
    try {
        value = stoi(line);
        return true;
    } catch (...) {
        return false;
    }
}

bool confirm(const string& question) {
    cout << question << " (y/n): ";
    string answer;
    getline(cin, answer);
    if (answer == "y" || answer == "Y") {
        return true;
    }
    return false;
}

// Stack operations (custom linked list)
// check if stack empty
bool isStackEmpty() { 
    return stackTop == nullptr; 
}

// push activity onto stack
void pushStack(Activity* node) { // O(1)
    node->stackNext = stackTop;
    stackTop = node;
}

// pop activity from stack
Activity* popStack() { // O(1)
    if (isStackEmpty()) return nullptr;
    Activity* node = stackTop;
    stackTop = stackTop->stackNext;
    node->stackNext = nullptr;
    return node;
}

// check the resource still hold the parcel
static bool resourceHeldByParcel(const string& resourceID, int parcelID) {
    Resource* r = findResource(resourceID);
    return r != nullptr && r->status == "Occupied" && r->parcelID == parcelID;
}

// undo back the resource 
static bool releaseResourceForUndo(const string& resourceID, int parcelID) {
    if (!resourceHeldByParcel(resourceID, parcelID)) {
        return false;
    }
    Resource* r = findResource(resourceID);
    r->status = "Available";
    r->parcelID = 0;
    r->receiptID = 0;
    saveResources();
    return true;
}

// function for record new activity
void recordActivity(int parcelID, const string& type, const string& prevStatus,
    const string& newStatus, const string& requestID,
    const string& resourceID, bool reversible,
    const string& previousRequestStatus, const string& newRequestStatus) {
    Activity* node = new Activity;
    node->activityID = nextActivityID++;
    node->parcelID = parcelID;
    node->activityType = type;
    node->previousStatus = prevStatus;
    node->newStatus = newStatus;
    node->requestID = requestID;
    node->previousRequestStatus = previousRequestStatus;
    node->newRequestStatus = newRequestStatus;
    node->resourceID = resourceID;
    node->timestamp = currentTimestamp();
    node->reversible = reversible;
    node->reversed = false;
    node->nextActivity = nullptr;
    node->stackNext = nullptr;

    // Append to chronological log (tail) in O(1)
    if (logHead == nullptr) {
        logHead = logTail = node;
    } else {
        logTail->nextActivity = node;
        logTail = node;
    }
    logCount++;

    // Push to stack
    if (reversible) {
        pushStack(node);
    }
}

// print the table column 
void printActivityHeader() {
    cout << left << setw(6) << "ID" << setw(9) << "Parcel" << setw(20) << "Activity"
        << setw(32) << "Previous" << setw(32) << "New" << setw(22) << "Time"
        << setw(10) << "Request" << setw(10) << "Resource" << "State" << endl;
    cout << string(147, '-') << endl;
}

// print activity as one row
void printActivity(const Activity* a) {
    string state;
    if (a->reversed) {
        state = "Reversed";
    } else if (a->reversible) {
        state = "Reversible";
    } else {
        state = "Non-Reversible";
    }

    string reqID = a->requestID;
    if (reqID.empty()) {
        reqID = "-";
    }

    string resID = a->resourceID;
    if (resID.empty()) {
        resID = "-";
    }

    cout << left << setw(6) << a->activityID << setw(9) << a->parcelID
        << setw(20) << a->activityType << setw(32) << a->previousStatus
        << setw(32) << a->newStatus << setw(22) << a->timestamp << setw(10)
        << reqID << setw(10)
        << resID << state << endl;
}

// display recent activities from old to new
void displayRecentActivities(int n) {
    if (logHead == nullptr) {
        cout << "No activity history recorded yet." << endl;
        return;
    }
    if (n <= 0) {
        cout << "Number of activities must be at least 1." << endl;
        return;
    }

    int skip = 0;
    if (logCount > n) {
        skip = logCount - n;
    }

    Activity* current = logHead;
    for (int i = 0; i < skip; i++) {
        current = current->nextActivity;
    }

    int countToShow = logCount;
    if (countToShow > n) {
        countToShow = n;
    }

    cout << endl << "--- Recent Activities (Last " << countToShow << ") ---" << endl;
    printActivityHeader();
    while (current != nullptr) {
        printActivity(current);
        current = current->nextActivity;
    }
}

// check if parcel has any activity in the log
bool parcelHasHistory(int parcelID) {
    for (Activity* c = logHead; c != nullptr; c = c->nextActivity) {
        if (c->parcelID == parcelID) {
            return true;
        }
    }
    return false;
}

// Activity history of one parcel
void displayParcelHistory(int parcelID) {
    if (!parcelHasHistory(parcelID)) {
        if (ParcelExists(parcelID)) {
            cout << "No activity history found for Parcel " << parcelID << "." << endl;
        } else {
            cout << "Invalid Parcel ID: Parcel " << parcelID << " does not exist." << endl;
        }
        return;
    }
    bool found = false;
    for (Activity* c = logHead; c != nullptr; c = c->nextActivity) {
        if (c->parcelID == parcelID) {
            if (!found) {
                cout << endl << "--- Activity History for Parcel " << parcelID << " ---" << endl;
                printActivityHeader();
            }
            printActivity(c);
            found = true;
        }
    }
}

// Retrieve and Review Previous Activities
void reviewParcelActivities(int parcelID) {
    if (!parcelHasHistory(parcelID)) {
        if (ParcelExists(parcelID)) {
            cout << "No activity history found for Parcel " << parcelID << "." << endl;
        } else {
            cout << "Invalid Parcel ID: Parcel " << parcelID << " does not exist." << endl;
        }
        return;
    }
    int steps = 0;
    Activity* latest = nullptr;
    for (Activity* c = logHead; c != nullptr; c = c->nextActivity) {
        if (c->parcelID == parcelID) {
            steps++;
            latest = c;
        }
    }

    string currentStatus = GetParcelStatus(parcelID);
    if (currentStatus.empty()) {
        currentStatus = latest->newStatus; // Shows latest status even if cancelled (e.g. "Cancelled")
    }

    cout << endl << string(55, '=') << endl;
    cout << "  Detailed Review: Parcel " << parcelID << endl;
    cout << string(55, '=') << endl;
    cout << "Current Status       : " << currentStatus << endl;
    cout << "Total Steps Taken    : " << steps << endl;
    cout << "\nProcessing Sequence:" << endl;
    int step = 1;
    for (Activity* c = logHead; c != nullptr; c = c->nextActivity) {
        if (c->parcelID == parcelID) {
            string reversedTag = "";
            if (c->reversed) {
                reversedTag = "  [Reversed]";
            }

            // CHANGE 1: resource steps do not change the parcel status, so show the resource instead
            cout << "  Step " << step++ << ": [" << c->activityType << "] ";
            if (c->previousStatus == c->newStatus && !c->resourceID.empty()) {
                cout << "Resource " << c->resourceID;
            } else {
                cout << c->previousStatus << " -> " << c->newStatus;
            }
            cout << " (at " << c->timestamp << ")" << reversedTag << endl;
        }
    }
    cout << "\nMost Recent Activity : #" << latest->activityID << " (" << latest->activityType << ")" << endl;
    cout << "Timestamp            : " << latest->timestamp << endl;
    cout << "Previous Status      : " << latest->previousStatus << endl;
    cout << "New Status           : " << latest->newStatus << endl;

    string reqID = latest->requestID;
    if (reqID.empty()) {
        reqID = "-";
    }
    string resID = latest->resourceID;
    if (resID.empty()) {
        resID = "-";
    }

    cout << "Request ID (latest)  : " << reqID << endl;
    cout << "Resource ID (latest) : " << resID << endl;

    cout << string(55, '=') << endl;
}

// Find activity by ID
Activity* findActivityByID(int activityID) {
    for (Activity* c = logHead; c != nullptr; c = c->nextActivity) {
        if (c->activityID == activityID) return c;
    }
    return nullptr;
}

// check if activity can be undone and perform action
bool undoActivity(Activity* target) {
    // Check if the activity already reversed
    if (target->reversed) {
        cout << "Activity " << target->activityID << " has already been reversed." << endl;
        return false;
    }

    // Check if the activity is reversable
    if (!target->reversible) {
        cout << "Activity " << target->activityID << " (" << target->activityType
            << ") cannot be reversed (marked non-reversible)." << endl;
        return false;
    }

    // Check if the activity is the recent activity on stack
    if (stackTop == nullptr || target != stackTop) {
        cout << "Only the most recent undoable activity can be undone (LIFO)." << endl;
        if (stackTop != nullptr) {
            cout << "Most recent undoable activity is #" << stackTop->activityID
                << " (" << stackTop->activityType << ")." << endl;
        }
        return false;
    }

    // Check if parcel still exists 
    if (!ParcelExists(target->parcelID)) {
        cout << "Parcel " << target->parcelID << " no longer exists in the system." << endl;
        return false;
    }

    // Check status consistency 
    string currentStatus = GetParcelStatus(target->parcelID);
    if (currentStatus != target->newStatus) {
        cout << "Cannot undo: Parcel " << target->parcelID << " status is currently '" 
            << currentStatus << "', but this activity produced '" << target->newStatus << "'." << endl;
        cout << "Undoing this would result in an invalid or corrupted parcel status." << endl;
        return false;
    }

    // Check: resource must still hold this parcel
    if (target->activityType == "ASSIGN_RESOURCE" &&
        !resourceHeldByParcel(target->resourceID, target->parcelID)) {
        cout << "Cannot undo: resource " << target->resourceID
             << " no longer holds parcel " << target->parcelID << "." << endl;
        return false;
    }

    // Check whether the related service request can be found
    ServiceRequest* request = nullptr;

    if (target->activityType == "SERVICE_REQUEST" ||
        target->activityType == "PROCESS_REQUEST") {

        if (target->requestID.empty()) {
            cout << "Service Request ID is missing.\n";
            return false;
        }

        request = findServiceRequest(stoi(target->requestID));

        if (request == nullptr) {
            cout << "Related service request cannot be found.\n";
            return false;
        }

        // Make sure the request is still in the expected state
        if (target->activityType == "PROCESS_REQUEST" &&
            request->status != target->newRequestStatus) {
            cout << "Service request status has changed. Undo is not allowed.\n";
            return false;
        }
    }

    // Confirmation
    cout << "\nAbout to reverse activity:" << endl;
    printActivityHeader();
    printActivity(target);
    if (!confirm("Confirm undo and restore parcel status?")) {
        cout << "Undo cancelled by user." << endl;
        return false;
    }

    // Restore parcel status and queue in Task 1
    if (!SetParcelStatus(target->parcelID, target->previousStatus)) {
        cout << "Failed to restore parcel status in Task 1. Undo aborted." << endl;
        return false;
    }

    // Restore related service request information
    if (target->activityType == "SERVICE_REQUEST") {

        // Remove the request that was created by this activity
        if (!removeServiceRequestForUndo(stoi(target->requestID))) {
            // Restore the original parcel status if removal fails
            SetParcelStatus(target->parcelID, currentStatus);
            cout << "Could not remove the service request.\n";
            return false;
        }

        saveRequestsToFile();

        cout << "Service request removed.\n";
    }
    else if (target->activityType == "PROCESS_REQUEST") {

        // Restore the request to its previous status
        request->status = target->previousRequestStatus;

        saveRequestsToFile();

        cout << "Service request status restored to: "
            << request->status << "\n";
    }
    // Restore the related resource
    if (target->activityType == "ASSIGN_RESOURCE") {

        if (!releaseResourceForUndo(target->resourceID, target->parcelID)) {

            // Restore the original parcel status if resource release fails
            SetParcelStatus(target->parcelID, currentStatus);

            cout << "Failed to release the assigned resource. Undo aborted.\n";
            return false;
        }

        cout << "Resource " << target->resourceID << " released." << endl;
    }

    popStack();
    target->reversed = true;

    // Log the undo action itself as an audit record
    recordActivity(target->parcelID, "UNDO", target->newStatus,
        target->previousStatus, target->requestID, target->resourceID, false);

    cout << "Activity #" << target->activityID << " reversed. Parcel "
        << target->parcelID << " restored to '" << target->previousStatus << "'." << endl;
    return true;
}

static bool isStale(Activity* a) {
    if (!ParcelExists(a->parcelID)) return true;
    if (GetParcelStatus(a->parcelID) != a->newStatus) return true;
    if (a->activityType == "ASSIGN_RESOURCE" &&
        !resourceHeldByParcel(a->resourceID, a->parcelID)) return true;
    return false;
}

// undo newest activity
bool undoLastActivity() {
    while (!isStackEmpty() && isStale(stackTop)) {
        cout << "Skipping activity #" << stackTop->activityID
             << ": it can no longer be reversed." << endl;
        stackTop->reversible = false;
        popStack();
    }
    if (isStackEmpty()) {
        cout << "No activity history available to undo." << endl;
        return false;
    }
    return undoActivity(stackTop);
}

// undo activity selected by ID
bool undoActivityByID(int activityID) {
    if (logHead == nullptr) {
        cout << "No activity history available to undo." << endl;
        return false;
    }
    Activity* target = findActivityByID(activityID);
    if (target == nullptr) {
        cout << "Invalid activity selection: No activity found with ID " << activityID << "." << endl;
        return false;
    }
    return undoActivity(target);
}

//for saving activities to file
// save every activity in the log to a text file
bool saveActivitiesToFile(const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cout << "Unable to open '" << filename << "' for writing." << endl;
        return false;
    }

    out << nextActivityID << endl;
    for (Activity* c = logHead; c != nullptr; c = c->nextActivity) {
        out << c->activityID << '|' << c->parcelID << '|' << c->activityType << '|'
            << c->previousStatus << '|' << c->newStatus << '|' << c->requestID << '|'
            << c->previousRequestStatus << '|' << c->newRequestStatus << '|'
            << c->resourceID << '|' << c->timestamp << '|'
            << c->reversible << '|' << c->reversed << endl;
    }
    out.close();

    cout << "Saved " << logCount << " activity record(s) to '" << filename << "'." << endl;
    return true;
}

// load activities from the text file, then rebuild the log and the stack
bool loadActivitiesFromFile(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        return false;
    }

    string line;
    if (!getline(in, line)) {
        return false;
    }

    int savedNextActivityID = 1;

    try {
        savedNextActivityID = stoi(line);
    }
    catch (...) {
        savedNextActivityID = 1;
    }

    logHead = logTail = nullptr;
    stackTop = nullptr;
    logCount = 0;
    int highestID = 0;

    while (getline(in, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        stringstream ss(line);
        string f[12];
        bool ok = true;
        for (int i = 0; i < 12; i++) {
            if (!getline(ss, f[i], '|')) {
                ok = false;
                break;
            }
        }
        if (!ok) continue;

        Activity* node = new Activity;
        try {
            node->activityID = stoi(f[0]);
            node->parcelID = stoi(f[1]);
        } catch (...) {
            delete node;
            continue;
        }
        node->activityType = f[2];
        node->previousStatus = f[3];
        node->newStatus = f[4];
        node->requestID = f[5];
        node->previousRequestStatus = f[6];
        node->newRequestStatus = f[7];
        node->resourceID = f[8];
        node->timestamp = f[9];
        node->reversible = (f[10] == "1");
        node->reversed = (f[11] == "1");
        node->nextActivity = nullptr;
        node->stackNext = nullptr;

        if (logHead == nullptr) {
            logHead = logTail = node;
        } else {
            logTail->nextActivity = node;
            logTail = node;
        }
        logCount++;
        if (node->activityID > highestID) highestID = node->activityID;
    }
    in.close();

    nextActivityID = savedNextActivityID;

    if (nextActivityID <= highestID) {
        nextActivityID = highestID + 1;
    }

    // rebuild the undo stack: oldest to newest, so the newest ends on top
    for (Activity* c = logHead; c != nullptr; c = c->nextActivity) {
        if (c->reversible && !c->reversed) {
            pushStack(c);
        }
    }

    cout << "Loaded " << logCount << " activity record(s) from '" << filename << "'." << endl;
    return true;
}

// Main menu for task 4
void task4Menu() {
    cin.ignore(1000, '\n');
    while (true) {
        cout << endl << string(50, '=') << endl;
        cout << "  Task 4: Parcel Activity & Record Management" << endl;
        cout << string(50, '=') << endl;
        cout << "1. View Recent Activities" << endl;
        cout << "2. View Parcel Activity History" << endl;
        cout << "3. Review Parcel Activities (Detailed Sequence)" << endl;
        cout << "4. Undo Most Recent Reversible Activity" << endl;
        cout << "5. Undo Activity By ID" << endl;
        cout << "0. Back to Main Menu" << endl;

        int choice;
        if (!readInt("Enter selection: ", choice)) {
            cout << "Invalid input. Please enter a valid number." << endl;
            continue;
        }

        int value;
        switch (choice) {
        case 1:
            if (!readInt("How many recent activities to show: ", value)) {
                cout << "Invalid number." << endl;
            } else {
                displayRecentActivities(value);
            }
            break;
        case 2:
            if (!readInt("Enter Parcel ID: ", value)) {
                cout << "Invalid Parcel ID." << endl;
            } else {
                displayParcelHistory(value);
            }
            break;
        case 3:
            if (!readInt("Enter Parcel ID: ", value)) {
                cout << "Invalid Parcel ID." << endl;
            } else {
                reviewParcelActivities(value);
            }
            break;
        case 4:
            undoLastActivity();
            break;
        case 5:
            if (!readInt("Enter Activity ID to undo: ", value)) {
                cout << "Invalid activity selection." << endl;
            } else {
                undoActivityByID(value);
            }
            break;
        case 0:
            return;
        default:
            cout << "Invalid selection. Please try again." << endl;
            break;
        }
    }
}