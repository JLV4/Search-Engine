
/**------------------------------------------
    Program 2: Search Engine
    Description: 
    This program creates a search engine by ultilzing maps and sets to store websites and text
    Year: Sophomore
    Course: CS 251, Fall 2023.
    System: VS Code on MacOS 13.51 Ventura
    Author: Justin Vaughn
 ---------------------------------------------**/

#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <map>
using namespace std;

//This function accepts a string parameter and filters it by removing all leading punctuations as well as making sure every char in the string is lowercase
string cleanToken(string s) {
    string newString;
    bool containsChr = false;
    for (auto &c : s) {
        if (isalpha(c)) {
            containsChr = true;
            break;
        }
    }
    if (!containsChr) {
        return "";
    }
    for (unsigned int i = 0; i < s.length(); i++) {
        if (!ispunct(s.at(i))) {
            newString = s.substr(i);
            break;
        }
    }
    for (int j = (newString.size() - 1); j >= 0; j--) {
        if (!ispunct(newString.at(j))) {
            newString = newString.substr(0, j + 1);
            break;
        }
    }
    for (auto &c : newString) {
        c = tolower(c);
    }
    return newString;
}

//This function gather the tokens from a string of text using stringstream to parse the string parameter for space seperated strings, it also calls clean token to filter each word in the parameter string
set<string> gatherTokens(string text) {
    string newString;
    stringstream str(text);
    set<string> tokens;
    while (str >> newString) {
        if (cleanToken(newString) != "") {
            tokens.insert(cleanToken(newString));
        }
    }
    return tokens;
}

//This function loads the map by opening the filename parameter and reading in both the URLS and the texts form the URLS, it then calls the above functions in order to parse the text and then inserts each word into the map with its associated set of URLS
int buildIndex(string filename, map<string, set<string>>& index) {
    ifstream myFile{filename};
    string URL;
    string text;
    int count = 0;
    if (myFile.is_open()) {
        while (getline(myFile, URL) && getline(myFile, text)) {
            for (auto &word : gatherTokens(text)) {
                index[word].insert(URL);
            }
            count++;
        }
    }
    else {
        return count;
    }
    myFile.close();
    return count;
}

//this function returns the union of the two set parameters
set<string> setUnion(set<string> &s1, set<string> &s2) {
    set<string> s3;
    for (auto &e : s1) {
        s3.insert(e);
    }
    for (auto &e : s2) {
        s3.insert(e);
    }
    return s3;
}

//this function returns the difference of the two set parameters
set<string> setDifference(set<string> &s1, set<string> &s2) {
    set<string> s3;
    for (auto &e : s1) {
        if (!s2.count(e)) {
            s3.insert(e);
        }
    }
    return s3;
}

//this function returns the intersection of the two set parameters
set<string> setIntersect(set<string> &s1, set<string> &s2) {
    set<string> s3;
    for (auto &e : s1) {
        if (s2.count(e)) {
            s3.insert(e);
        }
    }
    return s3;
}

//This function returns the set of URLs matching what the user wants to seatch for. It utilizes set function in order to get the differences, intersections, or union bewteen sets related to multiple words from input
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
    set<string> returnSet;
    set<string> holdSet;
    string word;
    string oneWord;
    stringstream str(sentence);
    str >> word;
    if ((index.count(word)) == 0) {
        return returnSet;
    }
    returnSet = setUnion(returnSet, index.at(word));

    //this block of code uses stringstream to read in words, clean them, and then return the set based a union, intersect, or difference
    while (str >> word) {
        switch (word.at(0)) {
            case '-':
                word = cleanToken(word);
                returnSet = setDifference(returnSet, index.at(word));
                break;
            case '+':
                word = cleanToken(word);
                returnSet = setIntersect(returnSet, index.at(word));
                break;
            default:
                word = cleanToken(word);
                returnSet = setUnion(returnSet, index.at(word));
                break;
        }
    }
    return returnSet;
}

//This function calls all of the above functions either directly or indirectly to create a full-fledged search engine based on the filename parameter passind in by the user
void searchEngine(string filename) {
    string query;
    int wrdCount = 0;
    map<string, set<string>> InvIdx;
    set<string> matchSet;
    cout << "Stand by while building index...\n";
    cout << "Indexed " << buildIndex(filename, InvIdx);
    for (auto &key : InvIdx) {
        wrdCount++;
    }
    cout << " pages containing " << wrdCount << " unique terms" << endl;
    cout << "\nEnter query sentence (press enter to quit): ";
    getline(cin, query);
    while (query != "") {
        matchSet = findQueryMatches(InvIdx, query);
        cout << "Found " << matchSet.size() << " matching pages\n";
        for (auto &e : matchSet) {
            cout << e << "\n";
        }
        cout << "\nEnter query sentence (press enter to quit): ";
        getline(cin, query);
    }
    cout << "\nThank you for searching!" << endl;
}
