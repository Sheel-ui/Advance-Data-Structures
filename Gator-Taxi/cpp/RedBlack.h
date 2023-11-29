#include <iostream>
#include "Ride.h"

class RbTree {
private:
    Ride* NULLNODE;
    Ride* root;

    Ride* searchHelper(Ride* ride, int key) {
        if (ride == NULLNODE || key == ride->rideNumber) return ride;
        if (key < ride->rideNumber) return searchHelper(ride->left, key);
        return searchHelper(ride->right, key);
    }

void deleteFix(Ride* x) {
    Ride* s;
    while (x != root && x->color == 0) {
        if (x == x->parent->left) {
            s = x->parent->right;
            if (s->color == 1) {
                // case 3.1
                s->color = 0;
                x->parent->color = 1;
                rotateLeft(x->parent);
                s = x->parent->right;
            }

            if (s->left->color == 0 && s->right->color == 0) {
                // case 3.2
                s->color = 1;
                x = x->parent;
            } else {
                if (s->right->color == 0) {
                    // case 3.3
                    s->left->color = 0;
                    s->color = 1;
                    rotateRight(s);
                    s = x->parent->right;
                }

                // case 3.4
                s->color = x->parent->color;
                x->parent->color = 0;
                s->right->color = 0;
                rotateLeft(x->parent);
                x = root;
            }
        } else {
            s = x->parent->left;
            if (s->color == 1) {
                // case 3.1
                s->color = 0;
                x->parent->color = 1;
                rotateRight(x->parent);
                s = x->parent->left;
            }

            if (s->right->color == 0 && s->right->color == 0) {
                // case 3.2
                s->color = 1;
                x = x->parent;
            } else {
                if (s->left->color == 0) {
                    // case 3.3
                    s->right->color = 0;
                    s->color = 1;
                    rotateLeft(s);
                    s = x->parent->left;
                }

                // case 3.4
                s->color = x->parent->color;
                x->parent->color = 0;
                s->left->color = 0;
                rotateRight(x->parent);
                x = root;
            }
        }
    }
    x->color = 0;
}


    void Transform(Ride* u, Ride* v) {
        if (u->parent == nullptr) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        if (v != NULLNODE) v->parent = u->parent;
    }

void deleteRideHelper(Ride* ride, int key) {
    Ride* z = NULLNODE;
    Ride* x, * y;
    while (ride != NULLNODE) {
        if (ride->rideNumber == key) z = ride;
        if (key <= ride->rideNumber) ride = ride->right;
        else ride = ride->left;
    }

    if (z == NULLNODE) return;

    y = z;
    int yOriginalColor = y->color;
    if (z->left == NULLNODE) {
        x = z->right;
        Transform(z, z->right);
    } else if (z->right == NULLNODE) {
        x = z->left;
        Transform(z, z->left);
    } else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            Transform(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        Transform(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (yOriginalColor == 0) deleteFix(x);
}

void insertFix(Ride* k) {
    Ride* u;
    while (k->parent != nullptr && k->parent->color == 1) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left; // uncle
            if (u->color == 1) {
                // case 3.1
                u->color = 0;
                k->parent->color = 0;
                k->parent->parent->color = 1;
                k = k->parent->parent;
            } else {
                // left of parent - right rotate
                if (k == k->parent->left) {
                    k = k->parent;
                    rotateRight(k);
                }
                k->parent->color = 0;
                k->parent->parent->color = 1;
                rotateLeft(k->parent->parent);
            }
        } else {
            // parent sibling
            u = k->parent->parent->right;

            if (u->color == 1) {
                u->color = 0;
                k->parent->color = 0;
                k->parent->parent->color = 1;
                k = k->parent->parent;
            } else {
                // right of parent - left rotate
                if (k == k->parent->right) {
                    k = k->parent;
                    rotateLeft(k);
                }
                k->parent->color = 0;
                k->parent->parent->color = 1;
                rotateRight(k->parent->parent);
            }
        }
        if (k == root) break;
    }
    root->color = 0;
}


std::string printBetweenRides(int rideNumber1, int rideNumber2, Ride* ride) {
    std::string leftS, rightS;
    
    // go left till null
    if (ride->left != NULLNODE)
        leftS = printBetweenRides(rideNumber1, rideNumber2, ride->left);
    else
        leftS = "";

    // go right till null
    if (ride->right != NULLNODE)
        rightS = printBetweenRides(rideNumber1, rideNumber2, ride->right);
    else
        rightS = "";

    // if value between rideNumber1 and rideNumber2 add to solution
    if (rideNumber1 <= ride->rideNumber && rideNumber2 >= ride->rideNumber)
        return leftS + "(" + std::to_string(ride->rideNumber) + "," +
               std::to_string(ride->rideCost) + "," +
               std::to_string(ride->tripDuration) + ")," + rightS;
    else
        return leftS + rightS;
}


void rotateLeft(Ride* x) {
    Ride* y = x->right;
    x->right = y->left;
    if (y->left != NULLNODE)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}


void rotateRight(Ride* x) {
    Ride* y = x->left;
    x->left = y->right;
    if (y->right != NULLNODE)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

public:
    RbTree() {
        NULLNODE = new Ride();
        NULLNODE->color = 0;
        NULLNODE->left = nullptr;
        NULLNODE->right = nullptr;
        root = NULLNODE;
    }

    std::string getRides(int rideNumber1, int rideNumber2) {
        std::string s = printBetweenRides(rideNumber1, rideNumber2, root);
        if (s.length() > 0) return s.substr(0, s.length() - 1);
        else return "(0,0,0)";
    }

    Ride* searchTree(int k) {
        return searchHelper(root, k);
    }

    Ride* minimum(Ride* ride) {
        while (ride->left != NULLNODE) ride = ride->left;
        return ride;
    }

    Ride* maximum(Ride* ride) {
        while (ride->right != NULLNODE) ride = ride->right;
        return ride;
    }

    void insert(Ride* ride) {
        ride->parent = nullptr;
        ride->left = NULLNODE;
        ride->right = NULLNODE;
        ride->color = 1;

        Ride* y = nullptr;
        Ride* x = root;

        while (x != NULLNODE) {
            y = x;
            if (ride->rideNumber < x->rideNumber) x = x->left;
            else x = x->right;
        }

        ride->parent = y;
        if (y == NULLNODE) root = ride;
        else if (ride->rideNumber < y->rideNumber) y->left = ride;
        else y->right = ride;

        if (ride->parent == nullptr) {
            ride->color = 0;
            return;
        }

        if (ride->parent->parent == nullptr) return;

        insertFix(ride);
    }

    Ride* getRoot() {
        return root;
    }

    void deleteRide(int data) {
        deleteRideHelper(root, data);
    }
};

#include <iostream>

int main() {
    RbTree rbTree;

    // Example rides
    Ride ride1, ride2, ride3, ride4, ride5;
    ride1.rideNumber = 10;
    ride1.rideCost = 20;
    ride1.tripDuration = 30;

    ride2.rideNumber = 5;
    ride2.rideCost = 15;
    ride2.tripDuration = 25;

    ride3.rideNumber = 15;
    ride3.rideCost = 25;
    ride3.tripDuration = 35;

    ride4.rideNumber = 3;
    ride4.rideCost = 13;
    ride4.tripDuration = 23;

    ride5.rideNumber = 7;
    ride5.rideCost = 17;
    ride5.tripDuration = 27;

    // Insert rides into the Red-Black Tree
    rbTree.insert(&ride1);
    rbTree.insert(&ride2);
    rbTree.insert(&ride3);
    rbTree.insert(&ride4);
    rbTree.insert(&ride5);

    // Search for a ride
    int searchKey = 15;
    Ride* foundRide = rbTree.searchTree(searchKey);

    if (foundRide != nullptr) {
        std::cout << "Found Ride: (" << foundRide->rideNumber << ", "
                  << foundRide->rideCost << ", " << foundRide->tripDuration << ")\n";
    } else {
        std::cout << "Ride with key " << searchKey << " not found.\n";
    }

    // Print rides between ride numbers 5 and 15
    std::string result = rbTree.getRides(5, 15);
    std::cout << "Rides between 5 and 15: " << result << std::endl;

    return 0;
}

