// This code manages a library system using commands read from a file and a Red-Black Tree data structure.
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "redBlackTree.h"
#include <algorithm>

using namespace std;

// Function to extract parameters from a line
vector<string> extractParameters(string line) {
    // Initialize variables
    string newLine = ""; // Holds the modified line without extra spaces
    bool doubleQuoteInProcess = false; // Tracks if processing a string within double quotes

    // Iterate through each character in the line
    for (char &s: line) {
        if (s == '"') {
            // Toggle doubleQuoteInProcess flag when encountering double quotes
            doubleQuoteInProcess = !doubleQuoteInProcess;
        }

        if (doubleQuoteInProcess) {
            // Inside a quoted string: handle ',' as a placeholder for later substitution
            if (s == ',') {
                newLine += '~'; // Substitute ',' with '~' for later restoration
            } else {
                newLine += s; // Append characters normally inside the quoted string
            }
        } else {
            // Outside quoted string: remove unnecessary characters except for the ones specified
            if (s == '(' || s == ')' || s == ' ' || s == ';') {
                continue; // Skip unnecessary characters
            } else {
                newLine += s; // Append relevant characters to the newLine string
            }
        }
    }

    // Extracted and processed parameters will be stored in the 'params' vector
    vector<string> params;
    string temp = ""; // Temporary string to store each parameter

    // Parse the modified line to extract individual parameters
    for (char &s: newLine) {
        if (s == ',') {
            // Found a parameter delimiter (','), add the extracted parameter to the 'params' vector
            params.push_back(temp);
            temp.clear(); // Clear the temporary string for the next parameter
        } else {
            if (s == '~') {
                temp += ','; // Restore original ',' in parameters from '~'
            } else {
                temp += s; // Append characters to form the parameter
            }
        }
    }

    // Add the last extracted parameter to the 'params' vector
    params.push_back(temp);

    // Return the vector containing the extracted parameters
    return params;
}


// Function to generate an output file name based on the input file name
string getOutputFileName(string input) {
    string inputFileName = ""; // Stores the name of the input file without its extension
    string extension = ""; // Stores the extension of the input file
    bool dotFound = false; // Indicates whether a dot (.) representing the extension is found

    // Iterate through each character in the input string
    for (char &s: input) {
        if (s == '.') {
            dotFound = true; // Flag that a dot (.) representing the extension is found
            continue; // Move to the next character without including the dot (.)
        }

        if (dotFound) {
            extension += s; // If dotFound is true, store characters after the dot as the extension
        } else {
            inputFileName += s; // If dotFound is false, store characters before the dot as the input file name
        }
    }

    // Concatenate the modified input file name, an underscore (_), the fixed string "output_file," and its extension
    return inputFileName + "_" + "output_file." + extension; 
}


int main(int argc, char *argv[]) {
    // Declare red black tree
    RedBlackTree rbTree;

    // If arguments are less than 2 then it means file parameter is missing
    if (argc < 2) {
        cout << " File parameter missing" << endl;
        return 1;
    }

    // Open input file
    string inputFilename = argv[1];
    ifstream inputFile(inputFilename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << inputFilename << endl;
        return 1;
    }

    // Generate output file name and set cout target to the output file
    string outputFilename = getOutputFileName(inputFilename);
    freopen(outputFilename.c_str(),"w",stdout);

    string line;
    // Read each line from the input file
    while (getline(inputFile, line)) {
        size_t pos = line.find('(');
        string operation = line.substr(0,pos);
        string params = line.substr(pos, line.size() - 1);
        // Extract parameters
        vector<string> parameters = extractParameters(params);

        // Insert book
        if (operation == "InsertBook") {
            if (parameters.size() == 4) {
                int bookId = stoi(parameters[0]);
                string bookName = parameters[1];
                string authorName = parameters[2];
                bool availability = (parameters[3] == "\"Yes\"") ? true : false;
                rbTree.insertBook(bookId, bookName, authorName, availability);
            }
        }
        // Print book
        else if (operation == "PrintBook") {
            if (parameters.size() == 1) {
                int bookId = stoi(parameters[0]);
                rbTree.printBook(bookId);
            }
            else {
                cout << operation << " has invalid number of parameters." << endl;
            }
        }
        // Print books between ids
        else if (operation == "PrintBooks") {
            if (parameters.size() == 2) {
                int bookId1 = stoi(parameters[0]);
                int bookId2 = stoi(parameters[1]);
                rbTree.printBooks(bookId1, bookId2);
            }
            else {
                cout << operation << " has invalid number of parameters." << endl;
            }
        }
        // Borrow book
        else if (operation == "BorrowBook") {
            if (parameters.size() == 3) {
                int patronId = stoi(parameters[0]);
                int bookId = stoi(parameters[1]);
                int patronPriority = stoi(parameters[2]);
                rbTree.borrowBook(patronId, bookId, patronPriority);
            }
            else {
                cout << operation << " has invalid number of parameters." << endl;
            }
        }
        // Return book
        else if (operation == "ReturnBook") {
            if (parameters.size() == 2) {
                int patronId = stoi(parameters[0]);
                int bookId = stoi(parameters[1]);
                rbTree.returnBook(patronId, bookId);
            }
            else {
                cout << operation << " has invalid number of parameters." << endl;
            }
        }
        // Delete book
        else if (operation == "DeleteBook") {
            if (parameters.size() == 1) {
                int bookId = stoi(parameters[0]);
                rbTree.deleteBook(bookId);
            }
            else {
                cout << operation << " has invalid number of parameters." << endl;
            }
        }
        // Find closest book
        else if (operation == "FindClosestBook") {
            if (parameters.size() == 1) {
                int bookId = stoi(parameters[0]);
                rbTree.findClosestBook(bookId);
            }
            else {
                cout << operation << " has invalid number of parameters." << endl;
            }
        }
        // Get color flip count
        else if (operation == "ColorFlipCount") {
            rbTree.printFlipCount();
        }
        // Terminate program
        else if (operation == "Quit") {
            cout << "Program Terminated!!" << endl;
            inputFile.close();
            return 0;
        }
        else {
            cout << "Invalid operation" << endl;
        }
    }

    inputFile.close();
    return 0;
}
