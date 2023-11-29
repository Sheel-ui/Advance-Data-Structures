#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


class GatorTaxi {
public:
    std::string insert(int rideNumber, int rideCost, int tripDuration, MinHeap& minHeap, RbTree& rbt) {
        Ride r = rbt.searchTree(rideNumber);
        if (r.rideNumber == 0) {
            Ride ride(rideNumber, rideCost, tripDuration);
            minHeap.insert(ride);
            rbt.insert(ride);
            return "";
        } else {
            return "Duplicate RideNumber";
        }
    }

    std::string print(int rideNumber, RbTree& rbt) {
        Ride ride = rbt.searchTree(rideNumber);
        return "(" + std::to_string(ride.rideNumber) + "," + std::to_string(ride.rideCost) + "," + std::to_string(ride.tripDuration) + ")";
    }

    std::string print(int rideNumber1, int rideNumber2, RbTree& rbt) {
        return rbt.getRides(rideNumber1, rideNumber2);
    }

    std::string getNextRide(MinHeap& minHeap, RbTree& rbt) {
        if (minHeap.size() == 0) {
            return "No active ride requests";
        } else {
            Ride r = minHeap.removeMin();
            rbt.deleteRide(r.rideNumber);
            return "(" + std::to_string(r.rideNumber) + "," + std::to_string(r.rideCost) + "," + std::to_string(r.tripDuration) + ")";
        }
    }

    void updateTrip(int rideNumber, int tripDuration, MinHeap& minHeap, RbTree& rbt) {
        Ride r = rbt.searchTree(rideNumber);
        if (r.rideNumber != 0) {
            int cost = r.rideCost;
            if (r.heapIndex != -1) {
                int current = r.tripDuration;
                int rideIndex = r.heapIndex;
                int updated = tripDuration;

                if (updated <= current) {
                    minHeap.decreaseDuration(rideIndex, updated);
                } else if (current < updated && updated <= 2 * current) {
                    rbt.deleteRide(r.rideNumber);
                    minHeap.remove(r.heapIndex);
                    insert(r.rideNumber, cost + 10, updated, minHeap, rbt);
                } else if (2 * current < updated) {
                    rbt.deleteRide(r.rideNumber);
                    minHeap.remove(r.heapIndex);
                }
            }
        }
    }

    void cancelRide(int rideNumber, MinHeap& minHeap, RbTree& rbt) {
        Ride r = rbt.searchTree(rideNumber);
        if (r.heapIndex != -1) {
            rbt.deleteRide(r.rideNumber);
            minHeap.remove(r.heapIndex);
        }
    }
};

int main(int argc, char* argv[]) {
    std::vector<std::string> list;

    try {
        // File reader and writer
        std::ifstream inputFile(argv[1]);
        std::ofstream outputFile("output_file.txt");

        std::string line;
        while (std::getline(inputFile, line)) {
            list.push_back(line);
        }

        // Initialize GatorTaxi, MinHeap, and RBT
        GatorTaxi gatorTaxi;
        MinHeap minHeap;
        RbTree rbt;
        std::string s;

        for (int i = 0; i < list.size(); i++) {
            std::string inp = list[i];
            std::istringstream iss(inp);
            std::string operation, arguments;
            iss >> operation;
            std::getline(iss >> std::ws, arguments);

            bool flag = false;

            // Switch cases for operations
            if (operation == "Insert") {
                s = gatorTaxi.insert(/*parse arguments*/, minHeap, rbt);
                if (s.length() > 0) {
                    outputFile << s << "\n";
                    if (s == "Duplicate RideNumber") {
                        // Break when Duplicate Ride Found
                        flag = true;
                    }
                }
            } else if (operation == "Print") {
                if (arguments.find(',') == std::string::npos) {
                    s = gatorTaxi.print(std::stoi(arguments), rbt);
                    outputFile << s << "\n";
                } else {
                    // Extract two integers from arguments and call print accordingly
                    std::istringstream argStream(arguments);
                    int rideNumber1, rideNumber2;
                    char comma;
                    argStream >> rideNumber1 >> comma >> rideNumber2;
                    s = gatorTaxi.print(rideNumber1, rideNumber2, rbt);
                    outputFile << s << "\n";
                }
            } else if (operation == "GetNextRide") {
                s = gatorTaxi.getNextRide(minHeap, rbt);
                outputFile << s << "\n";
            } else if (operation == "CancelRide") {
                // Extract an integer from arguments and call cancelRide accordingly
                int rideNumber = std::stoi(arguments);
                gatorTaxi.cancelRide(rideNumber, minHeap, rbt);
            } else if (operation == "UpdateTrip") {
                // Extract two integers from arguments and call updateTrip accordingly
                std::istringstream argStream(arguments);
                int rideNumber, tripDuration;
                char comma;
                argStream >> rideNumber >> comma >> tripDuration;
                gatorTaxi.updateTrip(rideNumber, tripDuration, minHeap, rbt);
            }

            if (flag) {
                break;
            }
        }

        outputFile.close();
        inputFile.close();
    } catch (const std::exception& e) {
        std::cerr << "An error occurred." << std::endl;
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
