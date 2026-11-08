#ifndef TASK4_H
#define TASK4_H

#include <string>

// Activity structure for Task 4
// Uses two custom pointer links
// - nextActivity: Singly Linked List (points forward in time to newer activity)
// - stackNext: LIFO Stack (points downward on the undo stack to older activity)
struct Activity {
    int activityID;
    int parcelID;
    std::string activityType;      // e.g. REGISTER, PROCESS, UPDATE_STATUS, SERVICE_REQUEST, UNDO
    std::string previousStatus;    // Status before the activity occurred
    std::string newStatus;         // Status after the activity occurred
    std::string requestID;         // Task 2 service request ID ("" if none)
    std::string resourceID;        // Task 3 resource/counter ID ("" if none)
    std::string timestamp;         // Formatted YYYY-MM-DD HH:MM:SS
    bool reversible;               // Whether this activity is eligible for undo
    bool reversed;                 // Whether this activity has already been reversed
    Activity* nextActivity;        // Pointer to newer activity for log
    Activity* stackNext;           // Pointer to older activity below on LIFO undo stack
};

// Core Task 4 operations
void recordActivity(int parcelID, const std::string& type, const std::string& prevStatus,
                    const std::string& newStatus, const std::string& requestID = "",
                    const std::string& resourceID = "", bool reversible = true);

void task4Menu();
void displayRecentActivities(int n);
void displayParcelHistory(int parcelID);
void reviewParcelActivities(int parcelID);
bool undoLastActivity();
bool undoActivityByID(int activityID);

#endif // TASK4_H