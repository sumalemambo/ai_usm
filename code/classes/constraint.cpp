#include <vector>
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

        int checkConstraint(vector<int> solution) {

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
            else if (constraintType == SAMEROOM_CONSTRAINT) {

                // Check if both entities are asigned to rooms
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
            else {
                return 1;
            }
        }

};