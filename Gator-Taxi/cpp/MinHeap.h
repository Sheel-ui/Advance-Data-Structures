#include <iostream>
#include <vector>
#include "Ride.h"

class MinHeap {
private:
    std::vector<Ride> list;

    int getParent(int i) {
        if (i % 2 == 1) return i / 2;
        return (i - 1) / 2;
    }

    int getLeft(int i) { return 2 * i + 1; }

    int getRight(int i) { return 2 * i + 2; }

    void swap(int i, int parent) {
        Ride a = list[parent];
        a.index = i;

        Ride b = list[i];
        b.index = parent;

        list[parent] = b;
        list[i] = a;
    }

    void heapify(int i) {
        int left = getLeft(i);
        int right = getRight(i);
        int smallest = -1;

        if (left <= list.size() - 1 && list[left].rideCost < list[i].rideCost)
            smallest = left;
        else
            smallest = i;

        if (right <= list.size() - 1 && list[right].rideCost < list[smallest].rideCost)
            smallest = right;

        if (smallest != i) {
            swap(i, smallest);
            heapify(smallest);
        }
    }

public:
    MinHeap() {}

    int size() {
        return list.size();
    }

    void insert(Ride r) {
        r.index = list.size();
        list.push_back(r);
        int i = list.size() - 1;
        int parent = getParent(i);

        while (parent != i && list[i].rideCost <= list[parent].rideCost) {
            bool flag = true;
            if (list[i].rideCost == list[parent].rideCost) {
                if (list[i].tripDuration > list[parent].tripDuration) {
                    flag = false;
                    break;
                }
            }
            if (flag == true) {
                swap(i, parent);
                i = parent;
                parent = getParent(i);
            }
        }
    }

    void decreaseDuration(int rideIndex, int tripDuration) {
        list[rideIndex].tripDuration = tripDuration;
        int parent = getParent(rideIndex);

        while (rideIndex != 0 && list[rideIndex].rideCost <= list[parent].rideCost) {
            bool flag = true;
            if (list[rideIndex].rideCost == list[parent].rideCost) {
                if (list[rideIndex].tripDuration > list[parent].tripDuration) {
                    flag = false;
                    break;
                }
            }
            if (flag == true) {
                swap(rideIndex, parent);
                rideIndex = parent;
                parent = getParent(rideIndex);
            }
        }
    }

    Ride removeMin() {
        if (list.size() == 0) {
            throw std::runtime_error("MinHeap is EMPTY");
        } else if (list.size() == 1) {
            Ride min = list[0];
            list.pop_back();
            return min;
        }

        Ride min = list[0];
        Ride lastItem = list.back();
        list.pop_back();
        lastItem.index = 0;
        list[0] = lastItem;

        heapify(0);

        return min;
    }

    void remove(int heapIndex) {
        int minCost = list[0].rideCost;
        list[heapIndex].rideCost = minCost - 1;

        int parent = getParent(heapIndex);
        while (list[heapIndex].rideCost < list[parent].rideCost) {
            swap(heapIndex, parent);
            heapIndex = parent;
            parent = getParent(parent);
        }

        removeMin();
    }
};