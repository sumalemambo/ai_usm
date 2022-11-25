#include "classes/instance.cpp"

int main(int argc, char *argv[])
{
    OSAPInstance problemInstance;
    string path = "./instances/" + (string) argv[1] + ".txt";

    problemInstance.init(path);
    vector<int> greedySolution = problemInstance.solveGreedy();
    for (int i = 0; i < greedySolution.size(); i++) {
        cout << greedySolution[i] << '\n';
    }
    problemInstance.Greedy();
    return 0;
}