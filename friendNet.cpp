// Hanna Brender and Reid Whitson
// CPSC 450: 01
// FriendNET: Deliverable 1

// Best friend chain algorithm: Prim's Algorithm
// Killer Features:
//      1. Fake friend feature: Remove friends who rate you 4 points less than you rate them
//      2. Delete person who is the least popular among other users

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

void prompt(map<string, int>, map<int, string>, vector<vector<int>>, vector<vector<int>>);

int main(){
    ifstream dataFile ("data.txt");
    
    vector<vector<int>> adjList;
    vector<vector<int>> valuesBetween; // a parallel list to the adjList
    map<string, int> nameToIndex; // map to convert from user's name and their index
    map<int, string> indexToName; // map to convert from user's index to their name
    
    string aLine;
    
    //variable to keep track of values in the map
    int x = 0;
    int indexFirstName;
    int indexSecondName;
    
    while (not dataFile.eof()) {
        for (int i = 0; i < 2; i++){
            getline(dataFile, aLine, ' ');
            
            if(i == 0){
                //this is the first name in the line of the file
                if(nameToIndex.find(aLine) == nameToIndex.end()){
                    //it isnt in the map yet
                    nameToIndex[aLine] = x;
                    indexToName[x] = aLine;
                    indexFirstName = x;
                    x++;
                    adjList.resize(x);
                    valuesBetween.resize(x);
                } else{
                    //found the name in the list
                    indexFirstName = nameToIndex[aLine];
                }
            } else{
                //this is the second name in the line of the file
                if(nameToIndex.find(aLine) == nameToIndex.end()){
                    //it isnt in the map yet
                    nameToIndex[aLine] = x;
                    indexToName[x] = aLine;
                    indexSecondName = x;
                    x++;
                    adjList.resize(x);
                    valuesBetween.resize(x);
                } else{
                    indexSecondName = nameToIndex[aLine];
                }
            }
        }
        int intVal;
        //read it into an int value
        dataFile >> intVal;
        //eat up the newline
        getline(dataFile,aLine);
        
        //add value to adjacency list
        adjList[indexFirstName].push_back(indexSecondName);
        
        //now intVal holds the distance between them
        valuesBetween[indexFirstName].push_back(intVal);
    }
    
    // Names to index
    cout << "\n\nName to Index Map" << endl << "------------------------------" << endl;
    for(auto it: nameToIndex){
        cout << "name: " << it.first << "\tindex->" <<  it.second << endl;
    }
    
    // Names to index
    cout << "\n\nIndex to Name Map" << endl << "------------------------------" << endl;
    for(auto it: indexToName){
        cout << "name: " << it.first << "\tindex->" <<  it.second << endl;
    }
    
    //current adjListaency list
    cout << "\n\nCurrent adjacency list " << endl << "----------------------------" << endl;
    for (int i = 0; i < adjList.size(); i++){
        cout << "index " << i << " -> ";
        for (int j = 0; j < adjList[i].size(); j++){
            cout << adjList[i][j] << " ";
        }
        cout << endl;
    }
    
    //current distance values -> parallel to aboce
    cout << "\n\nCurrent values between the individuals" << endl << "-------------------------" << endl;
    for (int i = 0; i < valuesBetween.size(); i++){
        cout << "index " << i << " -> ";
        for (int j = 0; j < valuesBetween[i].size(); j++){
            cout << valuesBetween[i][j] << " " ;
        }
        cout << endl;
    }
    prompt(nameToIndex, indexToName, adjList, valuesBetween);
    
    return 0;
}

void prompt(map<string, int> nameToIndex, map<int, string> indexToName, vector<vector<int>> adjList, vector<vector<int>> valuesBetween) {
    int option;
    cout << "\nWhat do you want to do?\n" <<
            "1) Check if user exists\n" <<
            "2) Check connection between users\n"
            "3) Find fake friends\n"
            "4) Quit\n";
    cin >> option;
    
    if (option == 1) {
        string user;
        cout << "What user? ";
        cin >> user;
        
        if (nameToIndex.find(user) != nameToIndex.end()) {
            cout << user << " does exist\n";
        } else {
            cout << user << " does not exist\n";
        }
        prompt(nameToIndex, indexToName, adjList, valuesBetween);
    } else if (option == 2) {
        string users;
        cout << "What users (seperated by spaces)? ";
        cin.ignore();
        getline(cin, users);
        
        // split the string of two user names
        string name;
        string first;
        string second;
        for (auto n: users) {
            if (n == ' ') {
                first = name;
                name = "";
            } else {
                name = name + n;
            }
        }
        second = name;
        
        int firstIndex;
        int secondIndex;
        // check if users exist
        if (nameToIndex.find(first) == nameToIndex.end()) {
            cout << "Error: " << first << " does not exist\n";
            prompt(nameToIndex, indexToName, adjList, valuesBetween);
        } else if (nameToIndex.find(second) == nameToIndex.end()) {
            cout << "Error: " << second << " does not exist\n";
            prompt(nameToIndex, indexToName, adjList, valuesBetween);
        } else {
            // get user index from map
            firstIndex = nameToIndex[first];
            secondIndex = nameToIndex[second];
            
            int length = adjList[firstIndex].size(); // total number of connection for first user
            // go through each of first's connections and check if second is one of their connections
            for (int i = 0; i < length; i++) {
                // second is connected to first
                if (adjList[firstIndex][i] == secondIndex) {
                    // value between first and second
                    int weight = valuesBetween[firstIndex][i];
                    cout << "The connection from " << first << " to " << second << " has weight " << weight << "\n";
                    prompt(nameToIndex, indexToName, adjList, valuesBetween);
                }
            }
            // no connection found
            cout << "No connection between " << first << " and " << second << "\n";
            prompt(nameToIndex, indexToName, adjList, valuesBetween);
        }
    } else if (option == 3) {
        string user_name;
        cout << "What is your name? ";
        cin >> user_name;
        
        // check if user exist
        if (nameToIndex.find(user_name) == nameToIndex.end()) {
            cout << "Error: " << user_name << " does not exist\n";
            prompt(nameToIndex, indexToName, adjList, valuesBetween);
        } else {
            // get user index from map
            int user_index = nameToIndex[user_name];
            
            int count = 0;
            // check if each of user's friends is a fake friend or not
            for (int i = 0; i < adjList[user_index].size(); i++) {
                int friend_index = adjList[user_index][i];
                int user_rating = valuesBetween[user_index][i];
                // check friend's rating of user
                for (int j = 0; j < adjList[friend_index].size(); j++) {
                    if (adjList[friend_index][j] == user_index) {
                        int friend_rating = valuesBetween[friend_index][j];
                        string friend_name = indexToName[friend_index];
                        // display fake friend
                        if ((user_rating - friend_rating) >= 4) {
                            count++;
                            cout << friend_name << " is a fake friend\n";
                        }
                    }
                }
            }
            if (count == 0) {
                cout << "No fake friends\n";
            }
            prompt(nameToIndex, indexToName, adjList, valuesBetween);
        }
    } else if (option == 4) {
        exit(0);
    } else {
        cout << "\nError: Incorrect option\n";
        prompt(nameToIndex, indexToName, adjList, valuesBetween);
    }
}
