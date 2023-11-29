class Ride {
private:
    int rideNumber;
    int rideCost;
    int tripDuration;
    int heapIndex;
    int color;
    Ride* parent;
    Ride* left;
    Ride* right;

public:
    // Default constructor
    Ride() : rideNumber(0), rideCost(0), tripDuration(0), heapIndex(-1), parent(nullptr), left(nullptr), right(nullptr) {}

    // Parameterized constructor
    Ride(int rideNumber, int rideCost, int tripDuration)
        : rideNumber(rideNumber), rideCost(rideCost), tripDuration(tripDuration), heapIndex(-1), parent(nullptr), left(nullptr), right(nullptr) {}

    // Setter methods
    void setIndex(int heapIndex) {
        this->heapIndex = heapIndex;
    }

    void setRideCost(int rideCost) {
        this->rideCost = rideCost;
    }

    void setTripDuration(int tripDuration) {
        this->tripDuration = tripDuration;
    }
};
