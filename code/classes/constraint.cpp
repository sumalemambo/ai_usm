#include <vector>
#include "room.cpp"
#include "constants.h"

using namespace std;

// Represents constraints of the optimization problem
class Constraint {
    public:
        static const int ALLOCATION_CONSTRAINT = 0;
        static const int NONALLOCATION_CONSTRAINT = 1;
        static const int ONEOF_CONSTRAINT = 2;
        static const int CAPACITY_CONSTRAINT = 3;
        static const int SAMEROOM_CONSTRAINT = 4;
        static const int NOTSAMEROOM_CONSTRAINT = 5;
        static const int NOTSHARING_CONSTRAINT = 6;
        static const int ADJACENCY_CONSTRAINT = 7;
        static const int NEARBY_CONSTRAINT = 8;
        static const int AWAYFROM_CONSTRAINT = 9;

        int id;
        int constraintType;
        vector<int> parametersIds;

        // Class constructor
        Constraint(int id, int constraintType, vector<int> parametersIds) {
            this->id = id;
            this->constraintType = constraintType;
            this->parametersIds = parametersIds;
        }

        // 
        int checkConstraint(vector<int> solution, vector<Room> &roomsVector) {

            // Handle the different types of contraints
            if (constraintType == ALLOCATION_CONSTRAINT) {

                // Check if entity is assigned in the solution
                if (solution[parametersIds[0]] == -1) {

                    // Entity is not assigned in the current solution
                    return -1;
                }
                else if (solution[parametersIds[0]] == parametersIds[1]) {
                    return 1;
                }
                else {
                    return 0;
                }
            }
            else if (constraintType == NONALLOCATION_CONSTRAINT) {
                if (solution[parametersIds[0] == -1]) {
                    return -1;
                }
                else if (solution[parametersIds[0]] != parametersIds[1]) {
                    return 1;
                }
                else {
                    return 0;
                }
            }
            else if (constraintType == CAPACITY_CONSTRAINT) {
                for (auto room : roomsVector) {
                    if (room.id == parametersIds[0]) {

                        // If room is not overused the constraint is satisfied
                        if (room.capacity > 0) {
                            return 1;
                        }
                        else {
                            return 0;
                        }
                    }
                }
            }
            else if (constraintType == SAMEROOM_CONSTRAINT) {

                // Check if both entities are assigned to rooms
                if (solution[parametersIds[0]] == -1 || solution[parametersIds[1]] == -1) {
                    return -1;
                }

                // If both entities are allocated in the same room the constraint is satisfied
                else if (solution[parametersIds[0]] == solution[parametersIds[1]]) {
                    return 1;
                }
                else{
                    return 0;
                }
            }
            else if (constraintType == NOTSAMEROOM_CONSTRAINT) {

                // Check if both entities are assigned to a room in the solution
                if (solution[parametersIds[0]] == -1 || solution[parametersIds[1]] == -1) {
                    return -1;
                }

                // If both entities are allocated in differents room the constraint is satisfied
                else if (solution[parametersIds[0]] != solution[parametersIds[1]]) {
                    return 1;
                }
                else {
                    return 0;
                }
            }
            else if (constraintType == NOTSHARING_CONSTRAINT) {

                // Check if entity is assigned to a room in the current solution
                if (solution[parametersIds[0]] == -1) {
                    return -1;
                }
                else {
                    for (int i = 0; i < (int) solution.size(); i++) {

                        // Dont compare the entity with itself
                        if (i != parametersIds[0]) {

                            // Check that entity with i id is not assigned to the same room
                            if (solution[i] == solution[parametersIds[0]]) {
                                return 0;
                            }
                        }
                    }
                    return 1;
                }
            }
            else if (constraintType == ADJACENCY_CONSTRAINT) {

                // Check that both entities are assigned in the current solution
                if (solution[parametersIds[0]] == -1 || solution[parametersIds[1] == -1]) {
                    return -1;
                }
                else {
                    for (int i = 0; i < (int) roomsVector.size(); i++) {
                        if (roomsVector[i].id == solution[parametersIds[0]]) {

                            // Get the room associated with entity 1
                            Room roomEntity = roomsVector[i];

                            // Iterate over the adjacency list of the entity 1 room
                            // to find out if the entity 2 room is adjacent
                            for (int j = 0; j < roomEntity.adjList.size(); j++) {

                                // Check if entity 2 room is adjacent to entity 1 room
                                if (solution[parametersIds[1]] == roomEntity.adjList[j]) {
                                    return 1;
                                }
                            }
                            i = roomsVector.size();
                        }
                    }
                    return 0;
                }
            }
            else if (constraintType == NEARBY_CONSTRAINT) {

                // Check if both entities are assigned to a room in the current solution
                if (solution[parametersIds[0]] == -1 || solution[parametersIds[1]] == -1) {
                    return -1;
                }
                else {
                    // Get the rooms associated with each entity
                    for (int i = 0; i < (int) roomsVector.size(); i++) {
                        if (roomsVector[i].id == solution[parametersIds[0]]) {
                            i = (int) roomsVector.size();

                            for (int j = 0; j < (int) roomsVector.size(); j++) {
                                if (roomsVector[j].id == solution[parametersIds[1]]) {
                                    j = i;

                                    // Two rooms are said to be nearby if they are on the same floor
                                    if (roomsVector[i].floor == roomsVector[j].floor) {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                    return 0;
                }
            }
            else {
                // Check if both entities are assigned to a room in the current solution
                if (solution[parametersIds[0]] == -1 || solution[parametersIds[1]] == -1) {
                    return -1;
                }
                else {
                    // Get the rooms associated with each entity
                    for (int i = 0; i < (int) roomsVector.size(); i++) {
                        if (roomsVector[i].id == solution[parametersIds[0]]) {
                            i = (int) roomsVector.size();

                            for (int j = 0; j < (int) roomsVector.size(); j++) {
                                if (roomsVector[j].id == solution[parametersIds[1]]) {
                                    j = i;

                                    // Two rooms are said to be away if they are on the distinct floors
                                    if (roomsVector[i].floor != roomsVector[j].floor) {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                    return 0;
                }
            }
        }
};