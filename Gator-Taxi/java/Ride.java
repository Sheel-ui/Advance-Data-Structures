class Ride {

    // Ride is our object represent taxi object

    // heap index is used as a link between RBT and min Heap
    // element found in RBT in O(log(n)) time can be accessed in O(1) time using heapIndex

    int rideNumber;
    int rideCost;
    int tripDuration;
    int heapIndex;
    int color;
    Ride parent;
    Ride left;
    Ride right;

    // default constructor

    public Ride(){
        this.rideNumber = 0;
        this.rideCost = 0;
        this.tripDuration = 0;
        this.heapIndex = -1;
    }

    // parameterized constructor

    public Ride(int rideNumber, int rideCost, int tripDuration){
        this.rideNumber = rideNumber;
        this.rideCost = rideCost;
        this.tripDuration = tripDuration;
        this.heapIndex = -1;
    }

    // setter methods

    public void setIndex(int heapIndex) {
        this.heapIndex = heapIndex;
    }

    public void setRideCost(int rideCost) {
        this.rideCost = rideCost;
    }

    public void setTripDuration(int tripDuration) {
        this.tripDuration = tripDuration;
    }
}