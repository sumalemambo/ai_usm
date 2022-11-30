#include "classes/instance.cpp"

int main(int argc, char *argv[])
{
    OSAPInstance problemInstance;
    string path = "./instances/" + (string) argv[1] + ".txt";

    problemInstance.init(path);
    vector<int> greedySolution;
    greedySolution = problemInstance.Greedy(problemInstance.roomsVector);
    problemInstance.HillClimbingBI(greedySolution);
    for (int i = 0; i < (int) problemInstance.roomsVector.size(); i++) {
        cout << problemInstance.roomsVector[i].capacity << '\n';
    }
    return 0;
}