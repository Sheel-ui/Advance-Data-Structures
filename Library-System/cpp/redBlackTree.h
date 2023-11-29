// Header containing RedBlackTree Class
#include <iostream>
#include <unordered_map>
#include "redBlackTreeNode.h"
using namespace std;


class RedBlackTree {
public:
    redBlackTreeNode* root;
    redBlackTreeNode* externalNode;
    // Map to store old tree information
    unordered_map<redBlackTreeNode*, bool> oldTreeState;
    // Map to store new tree information
    unordered_map<redBlackTreeNode*, bool> newTreeState;
    int flipCount;
    bool debug;

    // Print book with given bookId
    void printBook(int bookId) {
        redBlackTreeNode* node = search(bookId);
        if (node!=nullptr) {
            node->book.printDetails();
        }
        else {
            cout << "Book " << bookId << " not found in the library" << endl;
        }
        cout << endl;
    }

    // Print books with bookId between bookId1 and bookId2
    void printBooks(int bookId1, int bookId2) {
        vector<redBlackTreeNode*> validNodes = inorderSearch(bookId1, bookId2);
        for(auto & validNode : validNodes) {
            validNode->book.printDetails();
        }
        if (validNodes.empty()) {
            cout << "BookID not found in the library" << endl;
        }
    }

    // Perform borrow book for the given patronId, bookId and patronPriority
    void borrowBook(int patronId, int bookId, int patronPriority) {
        redBlackTreeNode* node = search(bookId);
        if (node!=nullptr) {
            node->book.borrowBook(patronId, patronPriority);
        }
        else {
            cout << "Book " << bookId << " not found in the library" << endl;
        }
    }

    // Perform return book for given patronId and bookId
    void returnBook(int patronId, int bookId) {
        redBlackTreeNode* node = search(bookId);
        if (node!=nullptr) {
            node->book.returnBook(patronId, bookId);
        }
        else {
            cout << "Book " << bookId << " not found in the library" << endl;
        }
    }

    // Find and return the books closest to given bookId
    void findClosestBook(int bookId) {
        // Get greatest bookId smaller than given bookId
        redBlackTreeNode* closestOnLeft = getGreatestOnLeft(bookId);
        // Get smallest bookId larger than given bookId
        redBlackTreeNode* closestOnRight = getSmallestOnRight(bookId);

        // If both exist then we need to pick the closest one
        if (closestOnLeft && closestOnRight) {
            if(closestOnLeft == closestOnRight) {
                closestOnLeft->book.printDetails();
            }
            else {
                int leftDiff = bookId - closestOnLeft->getKey();
                int rightDiff = closestOnRight->getKey() - bookId;
                // If difference is same then both are the closest books
                if (leftDiff == rightDiff) {
                    closestOnLeft->book.printDetails();
                    closestOnRight->book.printDetails();
                }
                // Otherwise the one with the smaller difference is the closest book
                else if(leftDiff < rightDiff) {
                    closestOnLeft->book.printDetails();
                }
                else {
                    closestOnRight->book.printDetails();
                }
            }
        }
        // If only one or none exist then pick the one which exists
        else {
            if (closestOnLeft) {
                closestOnLeft->book.printDetails();
            }
            if (closestOnRight) {
                closestOnRight->book.printDetails();
            }
        }
    }

    // Get the greatest bookId smaller than the given bookId
    redBlackTreeNode* getGreatestOnLeft(int bookId) {
        redBlackTreeNode* result = NULL;
        getGreatestOnLeftHelper(root, bookId, result);
        return result;
    }

    // Recursively search the greatest bookId smaller than the given bookId
    void getGreatestOnLeftHelper(redBlackTreeNode* root, int bookId, redBlackTreeNode* &result) {
        if(root != externalNode) {
            if (root->getKey() == bookId) {
                result = root;
            }
            else if (root->getKey() > bookId) {
                getGreatestOnLeftHelper(root->left, bookId, result);
            }
            else {
                result = root;
                getGreatestOnLeftHelper(root->right, bookId, result);
            }
        }
    }

    // Get the smallest bookId larger than the given bookId
    redBlackTreeNode* getSmallestOnRight(int bookId) {
        redBlackTreeNode* result = NULL;
        getSmallestOnRightHelper(root, bookId, result);
        return result;
    }

    // Recursively search the smalest bookId larger than the given bookId
    void getSmallestOnRightHelper(redBlackTreeNode* root, int bookId, redBlackTreeNode* &result) {
        if(root != externalNode) {
            if (root->getKey() == bookId) {
                result = root;
            }
            else if (root->getKey() > bookId) {
                result = root;
                getSmallestOnRightHelper(root->left, bookId, result);
            }
            else {
                getSmallestOnRightHelper(root->right, bookId, result);
            }
        }
    }

    // Return flip count
    int getFlipCount() const {
        return flipCount;
    }

    // Print flip count
    void printFlipCount() {
        cout << "Color Flip Count: "<< getFlipCount() << endl;
        cout << endl;
    }

    // Function to print tree map for debugging
    static void printTreeState(unordered_map<redBlackTreeNode*, bool> treeState) {
        unordered_map<redBlackTreeNode*, bool>::iterator itr;

        for(itr = treeState.begin(); itr!=treeState.end();itr++) {
            string color;
            if (itr->second) {
                color = "BLACK";
            }
            else {
                color = "RED";
            }
            cout<< itr->first->getKey() << ":" << color << endl;
        }
    }
    
    // Create a new node and set book fields
    redBlackTreeNode* createNewNode(int bookId, string bookName, string authorName, bool availabilityStatus) {
        redBlackTreeNode* newNode = new redBlackTreeNode;
        newNode->setParent(NULL);
        newNode->setKey(bookId);
        newNode->setBookDetails(bookId, bookName, authorName, availabilityStatus);
        newNode->left = (externalNode);
        newNode->right = (externalNode);
        newNode->black = false;
        newTreeState[newNode] = false;
        return newNode;
    }

    // Function to update flipcounts 
    void updateFlipCount() {
        if (debug) {
            cout<<"\nNodes that flipped:\n";
        }
        unordered_map<redBlackTreeNode*, bool>::iterator itr;
        // For each bookId that changed color after current operation
        for(itr = newTreeState.begin(); itr != newTreeState.end();itr++){
            // Check if that bookId's color is changed when compared to the old state of the tree
            if (oldTreeState.find(itr->first) != oldTreeState.end()) {
                if (oldTreeState[itr->first] != newTreeState[itr->first]) {
                    if (debug) {
                        cout<<itr->first->getKey()<<endl;
                    }
                    // Update flip count and tree state
                    flipCount++;
                    oldTreeState[itr->first] = newTreeState[itr->first];
                }
            }
            else {
                // Update tree state
                oldTreeState[itr->first] = newTreeState[itr->first];
            }
        }
        // Clear new tree state for next operation
        newTreeState.clear();
    }

    // Function to insert a new book into the Red-Black Tree
    void insertBook(int bookId, string bookName, string authorName, bool availabilityStatus) {
        // Creating a new node for the book
        redBlackTreeNode* node = createNewNode(bookId, bookName, authorName, availabilityStatus);
        redBlackTreeNode* tempNode = NULL;
        redBlackTreeNode* currentNode = this->root;

        // Finding the appropriate position for the new node in the tree
        while (currentNode != externalNode) {
            tempNode = currentNode;
            if (node->key < currentNode->key) {
                currentNode = currentNode->left;
            }
            else {
                currentNode = currentNode->right;
            }
        }
        
        // Setting the parent of the new node
        node->setParent(tempNode);

        // Attaching the new node to its appropriate position in the tree
        if (tempNode == NULL) {
            root = node;
        }
        else if (node->key < tempNode->key) {
            tempNode->left = (node);
        }
        else {
            tempNode->right = (node);
        }

        // Ensuring the Red-Black Tree properties are maintained after insertion
        if (node->parent == NULL) {
            node->black = true;
            newTreeState[node] = true;
        }
        else if (node->parent->parent == NULL) {
            // Do nothing
        }
        else {
            maintainInsert(node);
        }
        // Printing tree state for debugging purposes
        if (debug) {
            cout << "Inserting " << bookId << endl;
            // printTree();
            cout << "\nOld Tree State:\n";
            printTreeState(oldTreeState);
            cout << "\nNew Tree State:\n";
            printTreeState(newTreeState);
        }
        // Updating flip count and printing for debugging
        updateFlipCount();
        if (debug) {
            cout<< "FlipCount:" << flipCount << endl;
            cout<< endl << "--------------------------------" << endl;
        }
    }

    // Function to maintain Red-Black Tree properties after insertion
    void maintainInsert(redBlackTreeNode* node) {
        // The other child of grandparent not in the path where node was inserted
        redBlackTreeNode* d;
        // We need to take steps only if parent of inserted node is red
        while (node->parent->isRed()) {
            // Checking if pp(parent of inserted node) is right child of gp(grand parent of inserted node) 
            if (node->parent == node->parent->parent->right) {
                d = node->parent->parent->left;
                if (d->isRed()) {
                    // Case 1: Uncle is red
                    // Perform color flips and rotations to maintain properties
                    // Update tree state for debugging
                    // Move up the tree to check further violations
                    d->black = true;
                    newTreeState[d] = true;
                    node->parent->black = true;
                    newTreeState[node->parent] = true;
                    node->parent->parent->black = false;
                    newTreeState[node->parent->parent] = false;
                    node = node->parent->parent;
                }
                else {
                    // Case 2: Uncle is black
                    // Perform rotations and color changes to maintain properties
                    // Update tree state for debugging
                    // Move up the tree to check further violations
                    if (node == node->parent->left) {
                        node = node->parent;
                        rotateRight(node);
                    }
                    node->parent->black = true;
                    newTreeState[node->parent] = true;
                    node->parent->parent->black = false;
                    newTreeState[node->parent->parent] = false;
                    rotateLeft(node->parent->parent);
                }
            }
            // Symmetric cases for left child
            // Handle similar cases as above with proper rotations and color changes
            else {
                d = node->parent->parent->right;
                if (d->isRed()) {
                    d->black = true;
                    newTreeState[d] = true;
                    node->parent->black = true;
                    newTreeState[node->parent] = true;
                    node->parent->parent->black = false;
                    newTreeState[node->parent->parent] = false;
                    node = node->parent->parent;
                }
                else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        rotateLeft(node);
                    }
                    node->parent->black = true;
                    newTreeState[node->parent] = true;
                    node->parent->parent->black = false;
                    newTreeState[node->parent->parent] = false;
                    rotateRight(node->parent->parent);
                }
            }
            if (node == root) {
                break;
            }
        }
        // Ensure root is always black to maintain properties
        root->black = true;
        newTreeState[root] = true;
    }

    // Function to delete a book from the Red-Black Tree
    void deleteBook(int data) {
        // Finding and removing the node associated with the book to be deleted
        redBlackTreeNode* nodeToDelete = removeNode(this->root, data);
        if (nodeToDelete == nullptr) {
            return;
        }

        // Printing tree state for debugging purposes
        if (debug) {
            // Print tree state before deletion
            cout << "Removing " << data << endl;
            // printTree();
            cout << "\nOld Tree State:\n";
            printTreeState(oldTreeState);
            cout << "\nNew Tree State:\n";
            printTreeState(newTreeState);
        }

        // Update flip count and display for debugging
        updateFlipCount();
        if (debug) {
            cout<< "FlipCount:" << flipCount << endl;
            cout<< endl << "--------------------------------" << endl;
        }

        // Handling book reservation and informing patrons about cancellation
        vector<int> patronList = nodeToDelete->book.reservationHeap.getHeapKeys();
        if (patronList.empty()) {
            cout << "Book "<< data << " is no longer available" << endl;
        }
        else {
            cout << "Book "<< data << " is no longer available. Reservation made by Patrons ";
            for (int i=0;i<patronList.size();i++) {
                // Displaying patrons whose reservations got cancelled
                // Displaying comma-separated list except for the last item
                // Informing about cancellation
                if (i == patronList.size() - 1) {
                    cout<<patronList[i];
                }
                else {
                    cout << patronList[i] << ",";
                }
            }
            cout << " have been cancelled!\n";
        }

        // Cleanup: Erasing node references and freeing memory
        newTreeState.erase(nodeToDelete);
        oldTreeState.erase(nodeToDelete);
        delete nodeToDelete;
        cout << endl;
    }

    // Function to remove a node from the Red-Black Tree
    redBlackTreeNode* removeNode(redBlackTreeNode* root, int key) {
        redBlackTreeNode* nodeToDelete = externalNode;
        redBlackTreeNode* successor;
        redBlackTreeNode* replacementNode;

        // Finding the node to be deleted
        while (root != externalNode) {
            if (root->key == key) {
                nodeToDelete = root;
            }

            if (root->key <= key) {
                root = root->right;
            } else {
                root = root->left;
            }
        }

        // Handling case where the key is not found in the tree
        if (nodeToDelete == externalNode) {
            cout << "Key not found in the tree" << endl;
            return nullptr;
        }

        // Handling different cases for node deletion
        replacementNode = nodeToDelete;
        bool replacementNodeIsBlack = replacementNode->black;

        // Case 1: Node to be deleted has one child or no child
        if (nodeToDelete->left == externalNode) {
            successor = nodeToDelete->right;
            replaceTree(nodeToDelete, nodeToDelete->right);
        }
        else if (nodeToDelete->right == externalNode) {
            successor = nodeToDelete->left;
            replaceTree(nodeToDelete, nodeToDelete->left);
        }
        // Case 2: Node to be deleted has two children
        else {
            replacementNode = getMaximum(nodeToDelete->left);
            replacementNodeIsBlack = replacementNode->black;
            successor = replacementNode->left;

            if (replacementNode->parent == nodeToDelete) {
                successor->setParent(replacementNode);
            } else {
                replaceTree(replacementNode, replacementNode->left);
                replacementNode->left = (nodeToDelete->left);
                replacementNode->left->setParent(replacementNode);
            }

            replaceTree(nodeToDelete, replacementNode);
            replacementNode->right = (nodeToDelete->right);
            replacementNode->right->setParent(replacementNode);

            // Handling color changes for balancing the tree after replacement
            if (nodeToDelete->black) {
                replacementNode->black = true;
                newTreeState[replacementNode] = true;
            } else {
                replacementNode->black = false;
                newTreeState[replacementNode] = false;
            }
        }

        // Checking if the replacement node is black and balancing the tree
        if (replacementNodeIsBlack) {
            maintainDelete(successor);
        }

        return nodeToDelete;
    }


    void maintainDelete(redBlackTreeNode* nodeToBeDeleted) {
        // This function maintains the tree's properties after a deletion.
        // It checks for violations of the red-black tree properties and corrects them
        // if necessary.

        // The sibling node of the node to be deleted
        redBlackTreeNode* siblingNode;
        // Loop until the node to be deleted is the root or it is black
        while (nodeToBeDeleted != root && nodeToBeDeleted->black) {
            // Determine the sibling node based on whether the node to be deleted
            // is the left or right child of its parent
            if (nodeToBeDeleted == nodeToBeDeleted->parent->left) {
                siblingNode = nodeToBeDeleted->parent->right;
                // If the sibling node is red, perform a color flip and rotate
                // to make the sibling node black and its parent red
                if (siblingNode->isRed()) {
                    siblingNode->black = true;
                    newTreeState[siblingNode] = true;
                    nodeToBeDeleted->parent->black = false;
                    newTreeState[nodeToBeDeleted->parent] = false;
                    rotateLeft(nodeToBeDeleted->parent);
                    siblingNode = nodeToBeDeleted->parent->right;
                }
                // If the sibling node's children are both black, simply make the
                // sibling node black and move up the tree
                if (siblingNode->left->black && siblingNode->right->black) {
                    siblingNode->black = false;
                    newTreeState[siblingNode] = false;
                    nodeToBeDeleted = nodeToBeDeleted->parent;
                }
                // Otherwise, perform a rotation and color flip to maintain the
                // black height property of the tree
                else {
                    if (siblingNode->right->isBlack()) {
                        siblingNode->left->black = true;
                        newTreeState[siblingNode->left] = true;
                        siblingNode->black = false;
                        newTreeState[siblingNode] = false;
                        rotateRight(siblingNode);
                        siblingNode = nodeToBeDeleted->parent->right;
                    }
                    // Update the colors of the sibling node, its parent, and its
                    // right child to maintain the properties of the tree
                    if (nodeToBeDeleted->parent->black) {
                        siblingNode->black = true;
                        newTreeState[siblingNode] = true;
                    }
                    else {
                        siblingNode->black = false;
                        newTreeState[siblingNode] = false;
                    }
                    nodeToBeDeleted->parent->black = true;
                    newTreeState[nodeToBeDeleted->parent] = true;
                    siblingNode->right->black = true;
                    newTreeState[siblingNode->right] = true;
                    rotateLeft(nodeToBeDeleted->parent);
                    nodeToBeDeleted = root;
                }
            }
            // Perform similar operations for the case where the node to be deleted
            // is the right child of its parent
            else {
                siblingNode = nodeToBeDeleted->parent->left;
                // If the sibling node is red, perform a color flip and rotate
                // to make the sibling node black and its parent red
                if (siblingNode->isRed()) {
                    siblingNode->black = true;
                    newTreeState[siblingNode] = true;
                    nodeToBeDeleted->parent->black = false;
                    newTreeState[nodeToBeDeleted->parent] = false;
                    rotateRight(nodeToBeDeleted->parent);
                    siblingNode = nodeToBeDeleted->parent->left;
                }

                // If the sibling node's children are both black, simply make the
                // sibling node black and move up the tree
                if (siblingNode->right->black && siblingNode->left->black) {
                    siblingNode->black = false;
                    newTreeState[siblingNode] = false;
                    nodeToBeDeleted = nodeToBeDeleted->parent;
                }
                // Otherwise, perform a rotation and color flip to maintain the
                // black height property of the tree
                else {
                    if (siblingNode->left->black) {
                        siblingNode->right->black = true;
                        newTreeState[siblingNode->right] = true;
                        siblingNode->black = false;
                        newTreeState[siblingNode] = false;
                        rotateLeft(siblingNode);
                        siblingNode = nodeToBeDeleted->parent->left;
                    }
                    if (nodeToBeDeleted->parent->black) {
                        siblingNode->black = true;
                        newTreeState[siblingNode] = true;
                    }
                    else {
                        siblingNode->black = false;
                        newTreeState[siblingNode] = false;
                    }
                    nodeToBeDeleted->parent->black = true;
                    newTreeState[nodeToBeDeleted->parent] = true;
                    siblingNode->left->black = true;
                    newTreeState[siblingNode->left] = true;
                    rotateRight(nodeToBeDeleted->parent);
                    nodeToBeDeleted = root;
                }
            }
        }
        nodeToBeDeleted->black = true;
        newTreeState[nodeToBeDeleted] = true;
    }

    // Function to search for a book in the Red-Black Tree by book ID
    redBlackTreeNode* search(int bookId) {
    // Starting the search from the root of the tree
        redBlackTreeNode* node = searchTree(this->root, bookId);
        // Checking if the node is the external node (indicating not found) and returning accordingly
        if (node == externalNode) {
            return nullptr; // Book not found
        }
        else {
            return node; // Returning the node containing the book information
        }
    }

    // Recursive function to search for a book node in the Red-Black Tree
    redBlackTreeNode* searchTree(redBlackTreeNode* node, int key) {
        // Base cases: If node is external or if the key matches the node's key, return the node
        if (node == externalNode || key == node->key) {
            return node;
        }

        // If the key is smaller than the current node's key, search in the left subtree
        if (key < node->key) {
            return searchTree(node->left, key);
        }
        
        // Otherwise, search in the right subtree
        return searchTree(node->right, key);
    }


    // Function to perform an inorder search within a given range of book IDs in the Red-Black Tree
    vector<redBlackTreeNode*> inorderSearch(int bookIdLeft, int bookIdRight) {
    // Vector to store nodes within the specified range
        vector<redBlackTreeNode*> validNodes;

        // Initiates the inorder search process
        inorderSearchTree(this->root, bookIdLeft, bookIdRight, validNodes);

        // Returns the vector containing valid nodes within the specified range
        return validNodes;
    }

    // Recursive function to perform an inorder search within a given range of book IDs
    void inorderSearchTree(redBlackTreeNode* node, int leftRange, int rightRange, vector<redBlackTreeNode*> &validNodes) {
        // Checking if the node is not an external node
        if (node != externalNode) {
            // Traverses the left subtree
            inorderSearchTree(node->left, leftRange, rightRange, validNodes);

            // Checks if the current node's key falls within the specified range
            if (node->getKey() <= rightRange && node->getKey() >= leftRange) {
                // If within the range, adds the node to the vector of valid nodes
                validNodes.push_back(node);
            }

            // Traverses the right subtree
            inorderSearchTree(node->right, leftRange, rightRange, validNodes);
        }
    }


    // Function to perform a ranged search within a given range of book IDs in the Red-Black Tree
    vector<redBlackTreeNode*> rangedSearch(int bookIdLeft, int bookIdRight) {
        // Vector to store nodes within the specified range
        vector<redBlackTreeNode*> validNodes;

        // Initiates the ranged search process
        rangedSearchTree(this->root, bookIdLeft, bookIdRight, validNodes);

        // Returns the vector containing valid nodes within the specified range
        return validNodes;
    }

    // Recursive function to perform a ranged search within a given range of book IDs
    void rangedSearchTree(redBlackTreeNode* node, int leftRange, int rightRange, vector<redBlackTreeNode*> &validNodes) {
        // Checking if the node is an external node, indicating the end of the tree branch
        if (node == externalNode) {
            return;
        }
        else {
            int currentKey = node->key;
            
            // Checking if the current node's key is less than the left range
            if (currentKey < leftRange) {
                rangedSearchTree(node->right, leftRange, rightRange, validNodes);
            }
            // Checking if the current node's key is greater than the right range
            else if (currentKey > rightRange) {
                rangedSearchTree(node->left, leftRange, rightRange, validNodes);
            }
            // If the current node's key is within the specified range
            else {
                // Adding the node to the vector of valid nodes
                validNodes.push_back(node);
                
                // Traversing both left and right subtrees to continue the search
                rangedSearchTree(node->left, leftRange, rightRange, validNodes);
                rangedSearchTree(node->right, leftRange, rightRange, validNodes);
            }
        }
    }


    // Function to perform a left rotation on a node in the Red-Black Tree
    void rotateLeft(redBlackTreeNode* node) {
        // Storing the right child node of the given node
        redBlackTreeNode* rightChildNode = node->right;
        
        // Reassigning pointers to perform the left rotation
        node->right = rightChildNode->left;
        if (rightChildNode->left != externalNode) {
            rightChildNode->left->setParent(node);
        }
        rightChildNode->setParent(node->parent);
        
        // Updating the root if the current node is the root, otherwise adjusting parent pointers
        if (node->parent == NULL) {
            this->root = rightChildNode;
        }
        else if (node == node->parent->left) {
            node->parent->left = rightChildNode;
        }
        else {
            node->parent->right = rightChildNode;
        }
        
        // Completing the left rotation by adjusting pointers
        rightChildNode->left = node;
        node->setParent(rightChildNode);
    }

    // Function to perform a right rotation on a node in the Red-Black Tree
    void rotateRight(redBlackTreeNode* node) {
        // Storing the left child node of the given node
        redBlackTreeNode* leftChildNode = node->left;
        
        // Reassigning pointers to perform the right rotation
        node->left = leftChildNode->right;
        if (leftChildNode->right != externalNode) {
            leftChildNode->right->setParent(node);
        }
        leftChildNode->setParent(node->parent);
        
        // Updating the root if the current node is the root, otherwise adjusting parent pointers
        if (node->parent == NULL) {
            this->root = leftChildNode;
        }
        else if (node == node->parent->right) {
            node->parent->right = leftChildNode;
        }
        else {
            node->parent->left = leftChildNode;
        }
        
        // Completing the right rotation by adjusting pointers
        leftChildNode->right = node;
        node->setParent(leftChildNode);
    }


    // Function to replace a node in the Red-Black Tree with another node
    void replaceTree(redBlackTreeNode* oldNode, redBlackTreeNode* newNode) {
        // If the node being replaced is the root, set the new node as the root
        if (oldNode->parent == NULL) {
            root = newNode;
        }
        // If the node being replaced is the left child of its parent, update the left child with the new node
        else if (oldNode == oldNode->parent->left) {
            oldNode->parent->left = newNode;
        }
        // If the node being replaced is the right child of its parent, update the right child with the new node
        else {
            oldNode->parent->right = newNode;
        }

        // Set the parent of the new node to the parent of the old node
        newNode->setParent(oldNode->parent);
    }

public:
    // Constructor for the Red-Black Tree
    RedBlackTree() {
        // Initializing the external node used for leaves in the Red-Black Tree
        externalNode = new redBlackTreeNode();
        externalNode->black = true;
        externalNode->left = NULL;
        externalNode->right = NULL;

        // Initializing the root as the external node and other properties
        root = externalNode;
        flipCount = 0;
        debug = false;
    }

    // Function to find the node with the minimum key value in the subtree rooted at the given node
    redBlackTreeNode* getMinimum(redBlackTreeNode* node) const {
        // Traverses the left child nodes until it reaches the leftmost node
        while (node->left != externalNode) {
            node = node->left;
        }
        return node;
    }

    // Function to find the node with the maximum key value in the subtree rooted at the given node
    redBlackTreeNode* getMaximum(redBlackTreeNode* node) const {
        // Traverses the right child nodes until it reaches the rightmost node
        while (node->right != externalNode) {
            node = node->right;
        }
        return node;
    }


    // Function to find the next node in the Red-Black Tree
    redBlackTreeNode* getNextNode(redBlackTreeNode* node) {
        // If the node has a right child, find the minimum node in its right subtree
        if (node->right != externalNode) {
            // Case 1: Find the minimum node in the right subtree
            return getMinimum(node->right);
        }
        // If the node does not have a right child, traverse up to find the next node
        redBlackTreeNode* parentNode = node->parent;
        while (parentNode != externalNode && node == parentNode->right) {
            node = parentNode;
            parentNode = parentNode->parent;
        }
        return parentNode;
    }

    // Function to find the previous node in the Red-Black Tree
    redBlackTreeNode* getPreviousNode(redBlackTreeNode* node) {
        // If the node has a left child, find the maximum node in its left subtree
        if (node->left != externalNode) {
            return getMaximum(node->left);
        }
        // If the node does not have a left child, traverse up to find the previous node
        redBlackTreeNode* parentNode = node->parent;
        while (parentNode != externalNode && node == parentNode->left) {
            node = parentNode;
            parentNode = parentNode->parent;
        }
        return parentNode;
    }

    // Function to retrieve the root of the Red-Black Tree
    redBlackTreeNode* getRoot() {
        return this->root;
    }

};