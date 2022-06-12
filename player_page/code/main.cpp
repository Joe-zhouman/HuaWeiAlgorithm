#include "include/Solution.h"
#include <iostream>
#include "assert.h"
using namespace std;
using namespace MiuiIsTheBest;

int main(int argc, char *argv[]) {
    Solution case_solution;
    case_solution.MachinePositionInit();
    if (case_solution.MachineGraphInit() && case_solution.GetSolution()) {
        case_solution.OutPut();
        return 0;
    } else {
        cout << "no Solution for this case";
        return 1;
    }
}