#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <math.h>

#include "entity.cpp"
#include "constraint.cpp"
#include "constants.h"

using namespace std;

// Represents a whole instance of OSAP
class OSAPInstance {
    public:
        const vector<int> penaltyCosts = {20, 10, 10, 10, 10, 10, 50, 10, 10 ,10};

        static const int SOFT_CONSTRAINT = 0;
        static const int HARD_CONSTRAINT = 1;

        mt19937 gen{28112022};
        vector<Entity> entitiesVector;
        vector<Room> roomsVector;
        vector<Constraint> hardConstraints;
        vector<Constraint> softConstraints;
        uniform_int_distribution<> distribRooms;



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
        vector<int> HillClimbingBI(vector<int> solution) {
            vector<int> bestSolution;
            vector<int> bestNeighborSolution;
            vector<int> neightborSolution;
            float bestSolutionPenalty;
            float bestNeighborPenalty;
            float neightborPenalty;
            int randomInt;

            // Set random se
            uniform_int_distribution<> distrib(0, 2);

            bestSolution = solution;

            // Add restarts for search diversification.
            for (int i = 0; i < N_RESTARTS; i++) {

                // Set a maximum of generated neighbors
                for (int j = 0; j < MAX_ITERATIONS; j++) {

                    // Generate random number
                    randomInt = distrib(gen);

                    if (randomInt == 0) {

                    }
                    else if (randomInt == 1) {

                    }
                    else {

                    }

                }
            }

            return solution;
        }

        /* greedy() builds a solution for the instance starting from an empty partial solution
        * using a greedy algorithm.
        */ 
        vector<int> Greedy(vector<Room> roomsVector) {
            int root;
            float penalty;
            float minPenalty;
            int minPenaltyRoomIndex;
            vector<int> partialSolution;
            vector<Entity> unassignedEntities;
            vector<Entity> assignedEntities;
            vector<vector<int>> visitedRooms;


            partialSolution.resize(entitiesVector.size());
            unassignedEntities = sort(entitiesVector);
            reverse(unassignedEntities.begin(), unassignedEntities.end());

            // Root corresponds to the id where greedy starts solving
            root = unassignedEntities[unassignedEntities.size() - 1].id;

            assignedEntities.reserve(unassignedEntities.size());

            // Vector to keep track of already tried rooms
            visitedRooms.resize(unassignedEntities.size());

            // Fill partial solution vector as unassigned
            for (int i = 0; i < (int) partialSolution.size(); i++) {
                partialSolution[i] = UNASSIGNED_ENTITY;
            }


            do {
                // Get entity to assign
                Entity entity = unassignedEntities.back();

                minPenalty = -1;
                minPenaltyRoomIndex = -1;

                for (int i = 0; i < (int) roomsVector.size(); i++) {

                    // Check that rooms has not been previously tried
                    if (find(visitedRooms[entity.id].begin(), visitedRooms[entity.id].end(), roomsVector[i].id) == visitedRooms[entity.id].end()) {
                        visitedRooms[entity.id].push_back(roomsVector[i].id);
                        
                        // Assign room to partial solution
                        partialSolution[entity.id] = roomsVector[i].id;

                        // Check hard constraints
                        if (checkHardConstraints(partialSolution) == 1) {

                            // Calc penalty
                            penalty = greedyPenalty(entity, roomsVector[i], partialSolution);

                            // Check if current room penalty is lower than the best one
                            if (minPenalty > penalty || minPenaltyRoomIndex == -1) {
                                minPenalty = penalty;
                                minPenaltyRoomIndex = i;
                            }
                        }

                    }
                }

                // Check if there is factible room to be assigned to the entity
                if (minPenaltyRoomIndex != -1) {
                    partialSolution[entity.id] = roomsVector[minPenaltyRoomIndex].id;

                    roomsVector[minPenaltyRoomIndex].capacity -= entity.size;

                    unassignedEntities.pop_back();
                    assignedEntities.push_back(entity);
                }

                // Otherwise, try to assign another room to the previous entity
                else {
                    // Mark entity as UNASSIGNED
                    partialSolution[entity.id] = UNASSIGNED_ENTITY;

                    // Delete previously marked as visited rooms, need to check again with new assign
                    visitedRooms[entity.id].clear();

                    if (assignedEntities.size() > 0) {
                        Entity prevEntity = assignedEntities.back();

                        // Erase previous entity as assigned
                        assignedEntities.pop_back();

                        roomsVector[partialSolution[prevEntity.id]].capacity += prevEntity.size;
                        partialSolution[prevEntity.id] = UNASSIGNED_ENTITY;
                        unassignedEntities.push_back(prevEntity);
                    }
                }

            } while((partialSolution[root] != UNASSIGNED_ENTITY && visitedRooms[root].size() != roomsVector.size()) || (assignedEntities.size() != partialSolution.size()));
            cout << "COST = " <<objFunction(partialSolution) << '\n';
            return partialSolution;
        }

        vector<int> solveGreedy()
        {
            vector<int> partialSolution;
            vector<vector<int>> visitedRooms;

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

        vector<int> entityRelocate(vector<int> solution) {

            // Set random seed and distribution
            mt19937 gen{28112022};
            uniform_int_distribution<> distribRoom(0, (int) roomsVector.size() - 1);


        }

        /* objFunction() calculates the value of the objective function for
        * the current solution.
        */
        float objFunction(vector<int> solution) {
            float capacity;
            float usagePenalty;
            float sum;

            usagePenalty = 0;
            for (int i = 0; i < (int) roomsVector.size(); i++) {
                capacity = roomsVector[i].capacity;
                sum = 0;
                for (int j = 0; j < (int) solution.size(); j++) {
                    if (solution[j] == roomsVector[i].id) {
                        for (int k = 0; k < (int) entitiesVector.size(); k++) {
                            if (j == entitiesVector[k].id) {
                                sum += entitiesVector[k].size;
                            }
                        }
                    }
                }
                usagePenalty += max(capacity - sum, 2 * (sum - capacity));
            }
            return usagePenalty + calcSoftPenalty(solution);
        }


        /* calcSoftPenalty() calculates the penalty associated to the violation of soft
        * constraints in the given solution.
        */
        float calcSoftPenalty(vector<int> solution) {
            float penalty;

            penalty = 0;
            for (auto constraint : softConstraints) {
                if (constraint.checkConstraint(solution, roomsVector) == 0) {
                    penalty += penaltyCosts[constraint.constraintType];
                }
            }
            return penalty;
        }

        float objectiveFunction(Entity e, Room r) {
            return max(r.capacity - e.size, 2 * (e.size - r.capacity));
        }

        float roomUsePenalty(Entity e, Room r) {
            return max(r.capacity - e.size, 2 * (e.size - r.capacity));
        }

        float greedyPenalty(Entity e, Room r, vector<int> solution){
            return roomUsePenalty(e, r) + calcSoftPenalty(solution);
        }

        /* 
        * checkHardConstraint() verifies if the current solution satisfies all
        * the evaluable hard constraints. Returns 0 if a hard constraint is 
        * violated and 1 otherwise.
        */
        int checkHardConstraints(vector<int> solution) {
            for (auto constraint : hardConstraints) {
                if (constraint.checkConstraint(solution, roomsVector) == 0) {
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
        vector<Entity> findLargeEntities(vector<Entity>& entitiesVector) {
            vector<Entity> largeEntities;
            float average;
            float standardDeviation;
            float upperLimit;
            float maxSize;
            int maxIndex;

            average = calculateAverage(entitiesVector);
            standardDeviation = calculateStandardDeviation(entitiesVector, average);

            // We define an entity as large if space(entity) > average + 2 * sd
            upperLimit = average + 2 * standardDeviation;

            do {
                maxSize = 0;
                for (int i = 0; i < (int) entitiesVector.size(); i++) {

                    // Check that entity is large
                    if (entitiesVector[i].size > upperLimit) {
                        if (entitiesVector[i].size > maxSize) {
                            maxSize = entitiesVector[i].size;
                            maxIndex = i;
                        }
                    }
                }
                if (maxSize != 0) {
                    largeEntities.push_back(entitiesVector[maxIndex]);
                    entitiesVector.erase(entitiesVector.begin() + maxIndex);
                }
            } while (maxSize != 0);

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
        
        /* sortByConstraints() sort and returns the entitiesVector according to hard constraints
        * then soft constraints.
        */
        vector<Entity> sortByConstraints(vector<Entity>& entitiesVector) {
            int maxConstraints;
            int index;
            int nConstraints;
            vector<Entity> sortedEntities;
            
            sortedEntities.reserve(entitiesVector.size());

            // Find the entity with the most number of hard constraints in entitiesVector
            do {
                maxConstraints = 0;
                for (int i = 0; i < (int) entitiesVector.size(); i++) {
                    nConstraints = getNumberOfConstraints(entitiesVector[i], HARD_CONSTRAINT);
                    if (nConstraints > maxConstraints) {
                        maxConstraints = nConstraints;
                        index = i;
                    }
                }

                // Check that entity is at least connected to a restriction
                if (maxConstraints > 0) {
                    sortedEntities.push_back(entitiesVector[index]);
                    entitiesVector.erase(entitiesVector.begin() + index);
                }
            } while (maxConstraints != 0);

            // Find the entity with the most number of soft constraints in entitiesVector
            do {
                maxConstraints = 0;
                for (int i = 0; i < (int) entitiesVector.size(); i++) {
                    nConstraints = getNumberOfConstraints(entitiesVector[i], SOFT_CONSTRAINT);
                    if (nConstraints > maxConstraints) {
                        maxConstraints = nConstraints;
                        index = i;
                    }
                }

                // Check that entity is at least connected to a restriction
                if (maxConstraints > 0) {
                    sortedEntities.push_back(entitiesVector[index]);
                    entitiesVector.erase(entitiesVector.begin() + index);
                }
            } while (maxConstraints != 0);

            // Add unrestricted entities
            while (entitiesVector.size() > 0) {
                sortedEntities.push_back(entitiesVector.back());
                entitiesVector.pop_back();
            }
            return sortedEntities;
        }

        /* sort() sorts and returns the vector of entities entityVector according to three criteria:
        * large entities, most connected entities to hard constraints and most connected entities to
        * soft constraints.
        */
        vector<Entity> sort(vector<Entity> entityVector) {
            vector<Entity> sortedEntities;
            vector<Entity> entitiesByConstraint;

            sortedEntities = findLargeEntities(entityVector);
            entitiesByConstraint = sortByConstraints(entityVector);
            
            sortedEntities.insert(sortedEntities.end(), entitiesByConstraint.begin(), entitiesByConstraint.end());

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