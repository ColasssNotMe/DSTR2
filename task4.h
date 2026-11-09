#ifndef TASK4_H
#define TASK4_H

#include <string>

// Activity structure for Task 4
// Uses two custom pointer links
struct Activity {
    int activityID;
    int parcelID;
    std::string activityType;      // REGISTER, PROCESS, UPDATE_STATUS, SERVICE_REQUEST, UNDO
    std::string previousStatus;    // Status before the activity occurred
    std::string newStatus;         // Status after the activity occurred

    std::string requestID;         // for task 2 service request
    std::string previousRequestStatus;
    std::string newRequestStatus;

    std::string resourceID;        // for task 3 resource

    std::string timestamp;         // YYYY-MM-DD HH:MM:SS
    bool reversible;               // flag activity is eligible for undo
    bool reversed;                 // flag activity has already been reversed
    Activity* nextActivity;        // Pointer to newer activity for log
    Activity* stackNext;           // Pointer to older activity below on LIFO undo stack
};

// Core Task 4 operations
void recordActivity(int parcelID, const std::string& type, const std::string& prevStatus,
                    const std::string& newStatus, const std::string& requestID = "",
                    const std::string& resourceID = "", bool reversible = true,
                    const std::string& previousRequestStatus = "",const std::string& newRequestStatus = "");

void task4Menu();
std::string GetParcelStatus(int parcelID);
void displayRecentActivities(int n);
void displayParcelHistory(int parcelID);
void reviewParcelActivities(int parcelID);
bool undoLastActivity();
bool undoActivityByID(int activityID);
bool saveActivitiesToFile(const std::string& filename = "activities.txt");
bool loadActivitiesFromFile(const std::string& filename = "activities.txt");

#endif // TASK4_H