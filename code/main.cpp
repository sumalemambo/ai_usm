#include "classes/instance.cpp"

int main(int argc, char *argv[])
{
    OSAPInstance problemInstance;
    string path = "./instances/" + (string) argv[1] + ".txt";

    problemInstance.init(path);
    vector<int> greedySolution;
    vector<int> hillClimbingSolution;
    cout << '\n';
    greedySolution = problemInstance.Greedy(problemInstance.roomsVector);
    cout << '\n';
    return 0;
}