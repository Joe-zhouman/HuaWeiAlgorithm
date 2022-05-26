#include "include/Solution.h"
#include <iostream>

using namespace std;
using namespace MiuiIsTheBest;

int main(int argc, char *argv[]) {
    Solution case_solution;
    if (case_solution.GetSolution()) {
        case_solution.OutPut();
        return 0;
    } else {
        cout << "no Solution for this case";
        return 1;
    }
}