//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Adam White
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Lab 5-2 Binary Search Tree
//============================================================================

#include <iostream>
#include <time.h>
#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() : amount(0.0) {}
};

// Internal structure for tree node
struct Node {
    Bid bid;
    Node* left;
    Node* right;

    // default constructor
    Node() : left(nullptr), right(nullptr) {}

    // initialize with a bid
    Node(Bid aBid) : Node() {
        bid = aBid;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Bid bid) {
        if (node == nullptr) {
            node = new Node(bid);
            return;
        }
        if (bid.bidId < node->bid.bidId) {
            if (node->left == nullptr) {
                node->left = new Node(bid);
            }
            else {
                addNode(node->left, bid);
            }
        }
        else {
            if (node->right == nullptr) {
                node->right = new Node(bid);
            }
            else {
                addNode(node->right, bid);
            }
        }
    }

    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left);
            displayBid(node->bid);
            inOrder(node->right);
        }
    }

    void preOrder(Node* node) {
        if (node != nullptr) {
            displayBid(node->bid);
            preOrder(node->left);
            preOrder(node->right);
        }
    }

    void postOrder(Node* node) {
        if (node != nullptr) {
            postOrder(node->left);
            postOrder(node->right);
            displayBid(node->bid);
        }
    }

    Node* removeNode(Node* node, string bidId) {
        if (node == nullptr) return nullptr;

        if (bidId < node->bid.bidId) {
            node->left = removeNode(node->left, bidId);
        }
        else if (bidId > node->bid.bidId) {
            node->right = removeNode(node->right, bidId);
        }
        else {
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            Node* temp = findMin(node->right);
            node->bid = temp->bid;
            node->right = removeNode(node->right, temp->bid.bidId);
        }
        return node;
    }

    Node* findMin(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

public:
    BinarySearchTree() : root(nullptr) {}
    virtual ~BinarySearchTree() { destroyTree(root); }

    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    void Insert(Bid bid) {
        if (root == nullptr) {
            root = new Node(bid);
        }
        else {
            addNode(root, bid);
        }
    }

    void Remove(string bidId) {
        root = removeNode(root, bidId);
    }

    Bid Search(string bidId) {
        Node* current = root;
        while (current != nullptr) {
            if (current->bid.bidId == bidId) {
                return current->bid;
            }
            else if (bidId < current->bid.bidId) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        throw runtime_error("Bid ID not found");
    }

    void InOrder() { inOrder(root); }
    void PreOrder() { preOrder(root); }
    void PostOrder() { postOrder(root); }
};

void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | " << bid.fund << endl;
}

void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;
    csv::Parser file = csv::Parser(csvPath);

    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << endl;

    try {
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');
            bst->Insert(bid);
        }
    }
    catch (csv::Error& e) {
        cerr << "CSV parsing error: " << e.what() << endl;
    }
}

double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

int main(int argc, char* argv[]) {
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98223";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98223";
    }

    clock_t ticks;
    BinarySearchTree* bst = new BinarySearchTree();
    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            ticks = clock();
            loadBids(csvPath, bst);
            ticks = clock() - ticks;
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;
        case 2:
            bst->InOrder();
            break;
        case 3:
            ticks = clock();
            try {
                Bid foundBid = bst->Search(bidKey);
                displayBid(foundBid);
            }
            catch (const runtime_error& e) {
                cout << e.what() << endl;
            }
            ticks = clock() - ticks;
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;
        case 4:
            bst->Remove(bidKey);
            break;
        }
    }
    cout << "Good bye." << endl;
    delete bst;
    return 0;
}