#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

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

        // Class constructor
        Room(int id, int floor, float capacity, int size) {
            this->id = id;
            this->floor = floor;
            this->capacity = capacity;
            adjList.resize(size);
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

vector<Room> readRooms(istream& file, int nRooms) {
    int id;
    int floor;
    float capacity;
    string line;

    getline(file, line);
    getline(file, line);

    


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
    cout << entitiesVector[54].size << '\n';
    
    file.close();
    return 0;
}