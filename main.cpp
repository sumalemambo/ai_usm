#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

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

vector<int> readEntities(istream& file, int n)
{
    vector<int> entitiesSize;
    string line;
    string dispose;
    int capacity;

    // Assign space to vector array
    entitiesSize.resize(n);
    for (int i = 0; i < n; i++) {
        getline(file, line);
        istringstream lineStream(line);
        lineStream >> dispose >> dispose >> capacity;
        entitiesSize[i] = capacity;
    }
    return entitiesSize;
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
    vector<int> entitiesSize;

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

    entitiesSize = readEntities(file, nEntities);

    file.close();
    return 0;
}