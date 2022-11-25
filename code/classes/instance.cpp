#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>

#include "entity.cpp"
#include "constraint.cpp"
#include "constants.h"

using namespace std;

// Represents a whole instance of OSAP
class OSAPInstance {
    public:
        static const int SOFT_CONSTRAINT = 0;
        static const int HARD_CONSTRAINT = 1;

        vector<Entity> entitiesVector;
        vector<Room> roomsVector;
        vector<Constraint> hardConstraints;
        vector<Constraint> softConstraints;

        int init(string path) {
            int nEntities;
            int nRooms;
            int nFloors;
            int nConstraints;
            int nHardConstraints;
            int nSoftConstraints;
            ifstream file;
            string line;
            string dispose;
            vector<vector<Constraint>> constraintsVector;

            file.open(path);

            // Get number of entities
            nEntities = readFromHeader(file);

            // Get number of rooms
            nRooms = readFromHeader(file);

            // Get number of floors
            nFloors = readFromHeader(file);

            // Get number of constraints
            nConstraints = readFromHeader(file);

            // Get number of hard constraints
            nHardConstraints = readFromHeader(file);

            // Get number of soft constraints
            nSoftConstraints = readFromHeader(file);

            entitiesVector = readEntities(file, nEntities);

            roomsVector = readRooms(file, nRooms, nFloors);
            
            constraintsVector = readConstraints(file, nConstraints, nHardConstraints, nSoftConstraints);

            softConstraints.reserve(nSoftConstraints);
            softConstraints = constraintsVector[SOFT_CONSTRAINT];
            hardConstraints.reserve(nHardConstraints);
            hardConstraints = constraintsVector[HARD_CONSTRAINT];

            /*
            Check constraint vector
            constraintsVector = readConstraints(file, nConstraints, nHardConstraints, nSoftConstraints);
            cout << constraintsVector[0][8].constraintType << constraintsVector[0][8].parametersIds[0]<< constraintsVector[0][8].parametersIds[1] << '\n';
            */
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

        float calcObj() {
            return 0.01;
        }

        vector<int> solveGreedy()
        {
            vector<int> partialSolution;
            vector<vector<int>> visitedRooms;
            vector<Entity> unassignedEntities;

            unassignedEntities = findLargeEntities(entitiesVector);
            sortByConstraints(entitiesVector);

            partialSolution.reserve(entitiesVector.size());
            visitedRooms.resize(roomsVector.size());

            for (int i = 0; i < (int) entitiesVector.size(); i++) {
                partialSolution.push_back(UNASSIGNED_ENTITY);
            }
            int total = 0;
            int i = 0;
            while (true) {
                int optimalRoom = -1;
                float optimalRoomPenalization = -1;

                if ((i == partialSolution.size()) || (i == -1)) {
                    break;
                }
                for (int j = 0; j < (int) roomsVector.size(); j++) {
                    if (find(visitedRooms[i].begin(), visitedRooms[i].end(), j) == visitedRooms[i].end()) {
                        visitedRooms[i].push_back(j);
                        partialSolution[i] = roomsVector[j].id;
                        
                        if (checkHardConstraints(partialSolution) == 0) {
                            partialSolution[i] = -1;
                        }
                        else {
                            if (optimalRoomPenalization > objectiveFunction(entitiesVector[i], roomsVector[j])  || optimalRoom == -1) {
                                optimalRoom = j;
                                optimalRoomPenalization = objectiveFunction(entitiesVector[i], roomsVector[j]);
                            }
                        }
                    }
                }
                if (optimalRoom != -1) {
                    total += objectiveFunction(entitiesVector[i], roomsVector[optimalRoom]);
                    partialSolution[i] = roomsVector[optimalRoom].id;
                    roomsVector[optimalRoom].capacity -= entitiesVector[i].size;
                    i++;
                }
                else{
                    i--;
                    partialSolution[i] = -1;
                }
            }
            return partialSolution;
        }
    private:

        float objectiveFunction(Entity e, Room r) {
            return max(r.capacity - e.size, 2 * (e.size - r.capacity));
        }

        /* 
        * checkHardConstraint() verifies if the current solution satisfies all
        * the evaluable hard constraints. Returns 0 if a hard constraint is 
        * violated and 1 otherwise.
        */
        int checkHardConstraints(vector<int> solution) {
            for (auto constraint : hardConstraints) {
                if (constraint.checkConstraint(solution,roomsVector) == 0) {
                    return 0;
                }
            }
            return 1;
        }
        
        // calculateAverage() calculates and returns the average size of the entities.
        float calculateAverage(vector<Entity> entitiesVector) {
            float average = 0;
            for (int i = 0; i < (int) entitiesVector.size(); i++) {
                average += entitiesVector[i].size;
            }
            average = average / entitiesVector.size();
            return average;
        }

        /* calculateStandardDeviation() calculates and returns the standard deviation of the size
        * of the entities
        */
        float calculateStandardDeviation(vector<Entity> entitiesVector, float average) {
            float standardDeviation = 0;
            for (int i = 0; i < (int) entitiesVector.size(); i++) {
                standardDeviation += pow(entitiesVector[i].size - average, 2);
            }
            standardDeviation = standardDeviation / entitiesVector.size();
            return sqrt(standardDeviation);
        }

        /* findLargeEntities() identifies and returns a vector containing entities whose size
        * is abnormal(large).
        */
        vector<Entity> findLargeEntities(vector<Entity> entitiesVector) {
            vector<Entity> largeEntities;
            float average;
            float standardDeviation;
            float upperLimit;

            average = calculateAverage(entitiesVector);
            standardDeviation = calculateStandardDeviation(entitiesVector, average);

            // We define an entity as large if space(entity) > average + 2 * sd
            upperLimit = average + 2 * standardDeviation;

            for (int i = 0; i < (int) entitiesVector.size(); i++) {
                if (entitiesVector[i].size > upperLimit) {
                    largeEntities.push_back(entitiesVector[i]);
                }
            }
            return largeEntities;
        }

        /* getNumberOfConstraints() calculates and returns the numbers of constraints
        * with a specific hardness associated with the entity
        */
        int getNumberOfConstraints(Entity entity, int hardness) {
            int nConstraints = 0;
            vector<Constraint> constraints;
            
            // Search on specific set of constraints according to hardness
            if (hardness == SOFT_CONSTRAINT) {
                constraints = softConstraints;
            }
            else {
                constraints = hardConstraints;
            }
            for (int i = 0; i < (int) constraints.size(); i++) {

                // Entity is never associated with a capacity constraint
                if (constraints[i].constraintType != CAPACITY_CONSTRAINT) {
                    if (constraints[i].parametersIds[0] == entity.id) {
                        nConstraints += 1;
                    } 
                    else {

                        /* Allocation, non-allocation constraint second parameter id is a room
                        * not an entity
                        */ 
                        if (constraints[i].constraintType != ALLOCATION_CONSTRAINT &&
                        constraints[i].constraintType != NONALLOCATION_CONSTRAINT && 
                        constraints[i].constraintType != NOTSHARING_CONSTRAINT) {

                            // Check on the other parameter if entity is referenced
                            if (constraints[i].parametersIds[1] == entity.id) {
                                nConstraints += 1;
                            }
                        }
                    }
                }
            }
            return nConstraints;
        }

        vector<Entity> sortByConstraints(vector<Entity> entitiesVector) {
            int max;
            int index;
            int nConstraints;
            vector<Entity> sortedEntities;
            
            sortedEntities.reserve(entitiesVector.size());

            do {
                max = 0;
                for (int i = 0; i < (int) entitiesVector.size(); i++) {
                    nConstraints = getNumberOfConstraints(entitiesVector[i], HARD_CONSTRAINT);
                    if (nConstraints > max) {
                        max = nConstraints;
                        index = i;
                    }
                }
                max = 0;
            } while (max != 0);
            return sortedEntities;
        }

        // Extra utilities

        // readFromHeader() returns the int in the current line of the header in file.
        int readFromHeader(istream& file)
        {
            int n;
            string dispose;
            string line;

            getline(file, line);
            istringstream lineStream(line);
            lineStream >> dispose >> n;
            return n;
        }

        // readEntities() reads and returns a vector containing all the entities in file.
        vector<Entity> readEntities(istream& file, int nEntities)
        {
            int id;
            float size;
            string line;
            string dispose;
            vector<Entity> entityVector;

            entityVector.reserve(nEntities);

            // Ignore blank and redundant lines
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

        // readRooms() reads and returns a vector containing all the rooms in file.
        vector<Room> readRooms(istream& file, int nRooms, int nFloors) {
            int id;
            int floor;
            float capacity;
            int nAdj;
            int idAdj;
            string line;
            vector<Room> roomsVector;
            vector<int> adjList;

            // Ignore blank and redundant lines
            getline(file, line);
            getline(file, line);

            roomsVector.reserve(nRooms);
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
            }
            return roomsVector;
        }

        /* readConstraints() reads and returns the constraints as a 2-dimensional vector where the index 0
        * contains a vector of soft constraints and the index 1 contains a vector of hard constraints
        */
        vector<vector<Constraint>> readConstraints(istream& file, int nConstraints, int nHardConstraints,
         int nSoftConstraints)
        {
            int id;
            int constraintType;
            int constraintHardness;
            vector<int> parametersIds;
            vector<Constraint> hardConstraintsVector;
            vector<Constraint> softConstraintsVector;
            string line;
            int parameter;
            vector<vector<Constraint>> constraintsVector;

            constraintsVector.resize(2);
            hardConstraintsVector.reserve(nHardConstraints);
            softConstraintsVector.reserve(nSoftConstraints);

            parametersIds.reserve(2);

            // Ignore blank and redundant lines
            getline(file, line);
            getline(file, line);

            for (int i = 0; i < nConstraints; i++) {
                getline(file, line);
                istringstream lineStream(line);

                lineStream >> id >> constraintType >> constraintHardness;

                // Read and save constraint parameters
                lineStream >> parameter;
                parametersIds.push_back(parameter);
                lineStream >> parameter;
                parametersIds.push_back(parameter);

                // Handle the different types of constraints
                if (constraintHardness == SOFT_CONSTRAINT) {
                    softConstraintsVector.push_back(Constraint(id, constraintType, parametersIds));
                }
                else {
                    hardConstraintsVector.push_back(Constraint(id, constraintType, parametersIds));
                }

                parametersIds.clear();
            }

            constraintsVector[SOFT_CONSTRAINT] = softConstraintsVector;
            constraintsVector[HARD_CONSTRAINT] = hardConstraintsVector;

            return constraintsVector;
        }
};