#include "book.h"

// Class defining a node in the Red-Black Tree
class redBlackTreeNode {
public:
    int key; // Key value stored in the node
    redBlackTreeNode* parent; // Pointer to the parent node
    redBlackTreeNode* left; // Pointer to the left child node
    redBlackTreeNode* right; // Pointer to the right child node
    bool black; // Boolean indicating the color of the node (Red or Black)
    Book book; // Instance of the Book class representing book details

public:
    // Getter and Setter for 'key'
    int getKey() const {
        return key;
    }

    void setKey(int _key) {
        key = _key;
    }

    // Methods to handle Book details associated with the node
    Book getBook() {
        return book;
    }

    void setBookDetails(int bookId, string bookName, string authorName, bool availabilityStatus) {
        book.addDetails(bookId, bookName, authorName, availabilityStatus);
    }

    // Getter and Setter for 'parent'
    redBlackTreeNode* getParent() const {
        return parent;
    }

    void setParent(redBlackTreeNode* _parent) {
        parent = _parent;
    }

    // Getter and Setter for 'left'
    redBlackTreeNode* getLeft() const {
        return left;
    }

    void setLeft(redBlackTreeNode* _left) {
        left = _left;
    }

    // Getter and Setter for 'right'
    redBlackTreeNode* getRight() const {
        return right;
    }

    void setRight(redBlackTreeNode* _right) {
        right = _right;
    }

    // Methods to handle node color (Black or Red)
    bool isBlack() const {
        return black;
    }

    void setBlack() {
        black = true;
    }

    bool isRed() const {
        return !black;
    }

    void setRed() {
        black = false;
    }
};