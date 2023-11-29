#include "reservationHeap.h"
#include <chrono>
using namespace std;
class Book {
public:
    int bookId;
    string bookName;
    string authorname;
    bool availabilityStatus;
    int borrowedBy;
    ReservationHeap reservationHeap;
  // Make the constructor and other members accessible

    void addDetails(int _bookId, string _bookName, string _authorname, bool _availabilityStatus, int _borrowedBy = -1) {
        bookId = _bookId;
        bookName = _bookName;
        authorname = _authorname;
        availabilityStatus = _availabilityStatus;
        borrowedBy = _borrowedBy;
    }

    // Getters for book parameters
    int getBookId() { return bookId; }

    string getBookName() { return bookName; }

    string getAuthorname() { return authorname; }

    bool isAvailabilable() { return availabilityStatus; }

    int getBorrowedBy() { return borrowedBy; }

    ReservationHeap getReservationHeap() { return reservationHeap; }

    void printDetails() {
        cout << "Book ID = " << bookId << endl;
        cout << "Title = " << bookName << endl;
        cout << "Author = " << authorname << endl;
        cout << "Availability = " << (availabilityStatus ? "YES" : "NO") << endl;
        cout << "BorrowedBy = " << (borrowedBy == -1 ? "None" : to_string(borrowedBy)) << endl;
        cout << "Reservations = ";
        reservationHeap.displayList();
        cout << endl;
    }

    void borrowBook(int _patronId, int _patronPriority) {
        // If book is available then alott it to the the patron
        if (availabilityStatus) {
            borrowedBy = _patronId;
            availabilityStatus = false;
            cout<< "Book " << bookId << " Borrowed by Patron " << _patronId << endl;
            cout<< endl;
        }
        // If not store patient in reservation heap for the book
        else {
            reservationHeap.push({_patronId, _patronPriority, chrono::high_resolution_clock::now()});
            cout<< "Book " << bookId << " Reserved by Patron " << _patronId << endl;
            cout<< endl;
        }
    }

    void returnBook(int _patronId, int _bookId) {
        borrowedBy = -1;
        availabilityStatus = true;
        cout<< "Book " << _bookId << " Returned by Patron " << _patronId << endl;
        cout<< endl;
        if (reservationHeap.isEmpty()) {
            return;
        }
        // If patron in reservation heap then alott book to the highest priority patron
        else {
            ReservationHeapNode top = reservationHeap.top();
            reservationHeap.pop();
            int currentPatronId = top.getPatronID();
            borrowedBy = currentPatronId;
            availabilityStatus = false;
            cout<< "Book " << _bookId << " Allotted to Patron " << currentPatronId << endl;
            cout<< endl;
        }
    }


};