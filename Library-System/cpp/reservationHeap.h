#include <vector>
#include <iostream>
#include "reservationHeapNode.h"
#include <algorithm>
using namespace std;

// Class representing a Reservation Heap
class ReservationHeap {
    public:
    vector<ReservationHeapNode> nodeList; // Vector to store ReservationHeapNodes
    int capacity = 20; // Default capacity for the heap

    // Method to initialize the capacity of the heap
    void initializeCapacity(int _capacity) {
        capacity = _capacity;
    }

    // Method to add a ReservationHeapNode to the heap's nodeList
    void addToNodeList(ReservationHeapNode node) {
        nodeList.push_back(node);
    }

    // Getter for the capacity of the heap
    int getCapacity() const {
        return capacity;
    }

    // Getter for the size of the heap's nodeList
    int getSize() const {
        return nodeList.size();
    }

    // Method to display the nodeList contents
    void displayList() {
        // Displaying contents of the nodeList
        // Format: [element1, element2, ..., elementN]
        // Empty list is displayed as []
        if (nodeList.empty()) {
            cout << "[]" << endl;
            return;
        }
        cout << "[";
        for (int i=0;i<nodeList.size();i++) {
            // Last Iteration
            if (i==nodeList.size()-1) {
                cout << nodeList[i].getPatronID() << "]" << endl;
            }   
            else {
                cout << nodeList[i].getPatronID() << ", ";
            }
            // cout << "PatronID: " <<node.getPatronID() <<"|PriorityNumber: " << node.getPriorityNumber() << "|TimeOfReservation: " << node.getTimeOfReservation().time_since_epoch().count() << endl;
        }
    }
    
    // Returns the index of the parent node
    static int getParentIndex(int index) {
        return (index-1)/2;
    }

    // Returns the index of the left child node
    static int getLeftChildIndex(int index) {
        return 2*index + 1;
    }

    // Returns the index of the right child node
    static int getRightChildIndex(int index) {
        return 2*index + 2;
    }

    // Returns true if the heap is empty, otherwise false
    bool isEmpty() const {
        return nodeList.size() == 0;
    }

    // Pushes a node into the heap and maintains the heap property
    void push(ReservationHeapNode node) {
        if (getSize() < getCapacity()) {
            int childIndex = getSize();
            nodeList.push_back(node);
            while (childIndex!=0) {
                int parentIndex = getParentIndex(childIndex);
                if (nodeList[childIndex].isLessThan(nodeList[parentIndex])) {
                    swap(nodeList[childIndex], nodeList[parentIndex]);
                    childIndex = parentIndex;
                }
                else {
                    break;
                }
            }
        }
        else {
            cout << "Push not allowed, heap at capacity!" << endl;
        }
    }

    // Removes the top element from the heap and reorganizes the heap structure
    void pop() {
        if (getSize()>0) {
            int lastIndex = getSize() - 1;
            swap(nodeList[lastIndex], nodeList[0]);
            nodeList.pop_back();
            lastIndex -= 1;
            int currentIndex = 0;
            heapify(currentIndex, lastIndex);
        }
        else {
            cout << "Pop not allowed, heap at capacity!" << endl;
        }
    }

    // Reorganizes the heap structure from root index to maintain the heap property
    void heapify(int currentIndex, int lastIndex) {
        if (currentIndex == lastIndex) {
            return;
        }
        int leftChildIndex = getLeftChildIndex(currentIndex);
        int rightChildIndex = getRightChildIndex(currentIndex);

        if (leftChildIndex <= lastIndex && rightChildIndex <= lastIndex) {
            //Left child is smaller than right child
            if (nodeList[leftChildIndex].isLessThan(nodeList[rightChildIndex])) {
                if (nodeList[currentIndex].isGreaterThan(nodeList[leftChildIndex])) {
                    swap(nodeList[currentIndex], nodeList[leftChildIndex]);
                    heapify(leftChildIndex, lastIndex);
                }
            }
            // If right child is smaller than left child
            else {
                if (nodeList[currentIndex].isGreaterThan(nodeList[rightChildIndex])) {
                    swap(nodeList[currentIndex], nodeList[rightChildIndex]);
                    heapify(rightChildIndex, lastIndex);
                }
            }
        }
        else if (leftChildIndex <= lastIndex) {
            if (nodeList[currentIndex].isGreaterThan(nodeList[leftChildIndex])) {
                swap(nodeList[currentIndex], nodeList[leftChildIndex]);
                heapify(leftChildIndex, lastIndex);
            }
        }
        else if (rightChildIndex <= lastIndex) {
            if (nodeList[currentIndex].isGreaterThan(nodeList[rightChildIndex])) {
                swap(nodeList[currentIndex], nodeList[rightChildIndex]);
                heapify(rightChildIndex, lastIndex);
            }
        }
    }

    // Method to retrieve the top element of the heap
    ReservationHeapNode top() {
        return nodeList[0];
    }

    // Method to retrieve keys (patron IDs) of elements in the heap
    vector<int> getHeapKeys() {
        vector<int> heapKeys;
        for (auto i : nodeList) {
            heapKeys.push_back(i.getPatronID());
        }

        return heapKeys;
    }
};