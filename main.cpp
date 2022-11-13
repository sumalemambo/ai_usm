#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

using namespace std;

class Entity {
    public:
        int id;
        float size;

        // Class constructor
        Entity(int id, float size) {
            this->id = id;
            this->size = size;
        }
};

class Room {
    public:
        int id;
        int floor;
        float capacity;
        vector<int> adjList;
        shared_ptr<vector<vector<int>>> roomsInFloor;

        // Class constructor
        Room(int id, int floor, float capacity, vector<int> adjList) {
            this->id = id;
            this->floor = floor;
            this->capacity = capacity;
            this->adjList = adjList;
        }

        void setRoomsInFloor(shared_ptr<vector<vector<int>>> roomsInFloor) {
            this->roomsInFloor = roomsInFloor;
        }
};

int getFromHeader(istream& file)
{
    string dispose;
    string line;
    int n;

    getline(file, line);
    istringstream lineStream(line);
    lineStream >> dispose >> n;
    return n;
}

vector<Entity> readEntities(istream& file, int nEntities)
{
    int id;
    float size;
    string line;
    string dispose;
    vector<Entity> entityVector;

    entityVector.reserve(nEntities);

    getline(file, line);
    getline(file, line);

    for (int i = 0; i < nEntities; i++) {
        getline(file, line);
        istringstream lineStream(line);

        lineStream >> id >> dispose >> size;
        entityVector.push_back(Entity(id, size));
    }
    return entityVector;
}

vector<Room> readRooms(istream& file, int nRooms, int nFloors) {
    int id;
    int floor;
    float capacity;
    int nAdj;
    int idAdj;
    string line;
    vector<Room> roomsVector;
    vector<int> adjList;
    shared_ptr<vector<vector<int>>> roomsInFloor = make_shared<vector<vector<int>>>();

    roomsVector.reserve(nRooms);
    (*roomsInFloor).resize(nFloors);

    getline(file, line);
    getline(file, line);

    for (int i = 0; i < nRooms; i++) {
        getline(file, line);
        istringstream lineStream(line);

        lineStream >> id >> floor >> capacity >> nAdj;
        adjList.reserve(nAdj);
        for (int j = 0; j < nAdj; j++) {
            lineStream >> idAdj;
            adjList.push_back(idAdj);
        }
        roomsVector.push_back(Room(id, floor, capacity, adjList));
        adjList.clear();
        (*roomsInFloor)[floor].push_back(id);
    }
    for (int i = 0; i < roomsVector.size(); i++) {
        roomsVector[i].setRoomsInFloor(roomsInFloor);
    }
    return roomsVector;
}


int main(int argc, char *argv[])
{
    ifstream file;
    string line;
    string dispose;
    int nEntities;
    int nRooms;
    int nFloors;
    int nConstraints;
    int nHardConstraints;
    int nSoftConstraints;
    vector<int> entities;
    vector<Entity> entitiesVector;
    vector<Room> roomsVector;

    vector<vector<int>> adjacencyVector;

    string path = "./instances/" + (string) argv[1] + ".txt";
    file.open(path);

    // Get number of entities
    nEntities = getFromHeader(file);

    // Get number of rooms
    nRooms = getFromHeader(file);

    // Get number of floors
    nFloors = getFromHeader(file);

    // Get number of constraints
    nConstraints = getFromHeader(file);

    // Get number of hard constraints
    nHardConstraints = getFromHeader(file);

    // Get number of soft constraints
    nSoftConstraints = getFromHeader(file);

    entitiesVector = readEntities(file, nEntities);

    roomsVector = readRooms(file, nRooms, nFloors);
    
    /*
    Check if adjList are created correctly

    for (int i = 0; i < roomsVector[0].adjList.size(); i++) {
        cout << roomsVector[0].adjList[i] << '\n';
    }

    for (int i = 0; i < roomsVector[nRooms - 1].adjList.size(); i++) {
        cout << roomsVector[nRooms - 1].adjList[i] << '\n';
    }
    */
    /*
    printf("%p\n", (void *) &(*(roomsVector[0].roomsInFloor)));
    printf("%p\n", (void *) &(*(roomsVector[nRooms - 1].roomsInFloor)));
    for (int i = 0; i < (*(roomsVector[nRooms - 1]).roomsInFloor).size(); i++) {
        cout << "FLOOR " << i << '\n';
        for (int j = 0; j < (*(roomsVector[nRooms - 1]).roomsInFloor)[0].size(); j++) {
            cout << ((*(roomsVector[nRooms - 1]).roomsInFloor)[0])[j] << '\n';
        }
    }
    */

    file.close();
    return 0;
}