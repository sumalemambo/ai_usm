#include <vector>
#include <memory>

using namespace std;

// Represents the rooms to which the entities will be assigned
class Room {
    public:
        int id;
        int floor;
        float capacity;
        vector<int> adjList;

        // Class constructor
        Room(int id, int floor, float capacity, vector<int> adjList) {
            this->id = id;
            this->floor = floor;
            this->capacity = capacity;
            this->adjList = adjList;
        }
};