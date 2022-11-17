#include <vector>
#include <memory>

using namespace std;

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