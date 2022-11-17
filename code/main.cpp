#include "classes/instance.cpp"

int main(int argc, char *argv[])
{
    OSAPInstance problemInstance;
    string path = "./instances/" + (string) argv[1] + ".txt";

    problemInstance.init(path);
    problemInstance.solveGreedy();
    return 0;
}