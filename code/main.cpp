#include "classes/instance.cpp"

int main(int argc, char *argv[])
{
    OSAPInstance problemInstance;
    string path = "./instances/" + (string) argv[1] + ".txt";

    problemInstance.init(path);
    vector<int> greedySolution;
    greedySolution = problemInstance.Greedy(problemInstance.roomsVector);
    problemInstance.writeSolutionToFile(problemInstance.HillClimbingBI(greedySolution));

    return 0;
}