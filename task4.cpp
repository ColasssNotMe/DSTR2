// Task 4 - Parcel Activity and Record Management Module
// Stack with linkedlist implementation

#include "task4.h"
#include "task1.h"
#include <ctime>
#include <iomanip>
#include <iostream>
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
    return &normalParcels;
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

// function for record new activity
void recordActivity(int parcelID, const string& type, const string& prevStatus,
    const string& newStatus, const string& requestID,
    const string& resourceID, bool reversible) {
    Activity* node = new Activity;
    node->activityID = nextActivityID++;
    node->parcelID = parcelID;
    node->activityType = type;
    node->previousStatus = prevStatus;
    node->newStatus = newStatus;
    node->requestID = requestID;
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
    cout << left << setw(6) << "ID" << setw(9) << "Parcel" << setw(18) << "Activity"
        << setw(20) << "Previous" << setw(38) << "New" << setw(22) << "Time"
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
        << setw(18) << a->activityType << setw(20) << a->previousStatus
        << setw(38) << a->newStatus << setw(22) << a->timestamp << setw(10)
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
            cout << "  Step " << step++ << ": [" << c->activityType << "] "
                << c->previousStatus << " -> " << c->newStatus
                << " (at " << c->timestamp << ")"
                << reversedTag << endl;
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
    cout << "Request ID           : " << reqID << endl;
    cout << "Resource ID          : " << resID << endl;
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

    popStack();
    target->reversed = true;

    // Log the undo action itself as an audit record
    recordActivity(target->parcelID, "UNDO", target->newStatus,
        target->previousStatus, target->requestID, target->resourceID, false);

    cout << "Activity #" << target->activityID << " reversed. Parcel "
        << target->parcelID << " restored to '" << target->previousStatus << "'." << endl;
    return true;
}

// undo newest activity
bool undoLastActivity() {
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