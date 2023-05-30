#include "NodeUtils.h"
#include "Navarro.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>

using namespace std;

// Creates node with overlap
// returns new Node
Node NodeUtils::createNode(const string& line, int overlap) {
    vector<string> arr;
    string token;
    stringstream ss(line);
    while (getline(ss, token, '\t')) {
        arr.push_back(token);
    }

    int id = stoi(arr[1]);
    string sequence = arr[2];
    return Node(id * 2, false, sequence.substr(0, sequence.length() - overlap));
}

// Get reverse complement of sequence: A->T, C->G, G->C, A->T
Node NodeUtils::createReverseComplementNode(const string& line, int overlap) {
    vector<string> arr;
    string token;
    stringstream ss(line);
    while (getline(ss, token, '\t')) {
        arr.push_back(token);
    }

    int id = stoi(arr[1]);
    string sequence = arr[2];
    return Node(id * 2 + 1, true, getReverseComplement(sequence).substr(0, sequence.length() - overlap));
}

// Creates reverse complement node with overlap
// returns new Node
string NodeUtils::getReverseComplement(const string& sequence) {
    string reverse_complement;
    for (int i = sequence.length() - 1; i >= 0; i--) {
        char c = sequence[i];
        switch (c) {
            case 'A':
                reverse_complement += 'T';
                break;
            case 'C':
                reverse_complement += 'G';
                break;
            case 'G':
                reverse_complement += 'C';
                break;
            case 'T':
                reverse_complement += 'A';
                break;
            default:
                throw runtime_error("Unexpected value: " + to_string(c));
        }
    }
    return reverse_complement;
}

// returns copy of neighbors
vector<vector<int>> NodeUtils::copyNeighbors(const vector<vector<int>>& neighbors) {
    vector<vector<int>> newList;
    for (const vector<int>& nodeNeighbors : neighbors) {
        newList.push_back(nodeNeighbors);
    }
    return newList;
}
