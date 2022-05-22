#include "include/solution.h"
#include <iostream>

using namespace std;
using namespace MiuiIsTheBest;

int main(int argc, char *argv[]) {
    solution case_solution;
    if (case_solution.GetSolution()) {
        case_solution.OutPut();
        return 0;
    } else {
        cout << "no solution2 for this case";
        return 1;
    }
}