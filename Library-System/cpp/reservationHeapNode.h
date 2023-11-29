#include <chrono>
using namespace std;

// Define a class representing a node in the reservation heap
class ReservationHeapNode {
private:
    // Unique identifier for the patron
    int patronID;

    // Priority number of the reservation
    int priorityNumber;

    // Timestamp indicating the time of reservation
    chrono::time_point<chrono::high_resolution_clock> timeOfReservation;

public:
    // Constructor to initialize the node with patron ID, priority number, and reservation timestamp
    ReservationHeapNode(int _patronID, int _priorityNumber, chrono::time_point<chrono::high_resolution_clock> _timeOfReservation) {
        patronID = _patronID;
        priorityNumber = _priorityNumber;
        timeOfReservation = _timeOfReservation;
    }

    // Getter method to retrieve the patron ID
    int getPatronID() const {
        return patronID;
    }

    // Setter method to modify the patron ID
    void setPatronID(int id) {
        patronID = id;
    }

    // Getter method to retrieve the priority number
    int getPriorityNumber() const {
        return priorityNumber;
    }

    // Setter method to modify the priority number
    void setPriorityNumber(int priority) {
        priorityNumber = priority;
    }

    // Getter method to retrieve the reservation timestamp
    chrono::time_point<chrono::high_resolution_clock> getTimeOfReservation() const {
        return timeOfReservation;
    }

    // Setter method to modify the reservation timestamp
    void setTimeOfReservation(chrono::time_point<chrono::high_resolution_clock> time) {
        timeOfReservation = time;
    }

    // Method to compare the current node with another node based on priority and reservation time
    bool isLessThan(ReservationHeapNode other) {
        if (priorityNumber == other.getPriorityNumber()) {
            // If priorities are equal, compare reservation timestamps
            return timeOfReservation.time_since_epoch().count() < other.getTimeOfReservation().time_since_epoch().count();
        } else {
            // If priorities differ, compare priority numbers
            return priorityNumber < other.getPriorityNumber();
        }
    }

    // Method to check if the current node has a higher priority than another node
    bool isGreaterThan(ReservationHeapNode other) {
        if (priorityNumber == other.getPriorityNumber()) {
            // If priorities are equal, compare reservation timestamps
            return timeOfReservation.time_since_epoch().count() > other.getTimeOfReservation().time_since_epoch().count();
        } else {
            // If priorities differ, compare priority numbers
            return priorityNumber > other.getPriorityNumber();
        }
    }
};