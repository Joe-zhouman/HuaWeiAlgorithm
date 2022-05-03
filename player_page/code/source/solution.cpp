//
// Created by joe on 22-5-2.
//

#include "../include/solution.h"

namespace MiuiIsTheBest {
    bool solution::MatchInit(short index_machine, char index_window) {
        return windows[index_window].init_type[machine_types[index_machine].to_int()];
    }

    bool solution::MatchEnergy(short index_machine, short index_region) {
        return region_energy_types[index_region] == machine_first_energy_type[index_machine] ||
               region_energy_types[index_region] == machine_second_energy_type[index_machine];
    }

    //TODO:有点问题
    void solution::NextWindow(char &index_current_window) {
        window *current_window = windows + index_current_window;
        if (current_window->rest_cycle_times != 0 && current_window->first_order_next == 1 &&
            current_window->cnext != -1) {
            cycled_windows.push_back(index_current_window);
            current_window->rest_cycle_times--;
            index_current_window = current_window->cnext;
            return;
        }
        index_current_window++;
        return;
    }

    void solution::NextRegion(short &index_current_region, char &index_current_window) {
        if (factories[windows[index_current_window].factory].back() > index_current_window) {
            index_current_region++;
            return;
        }
        NextWindow(index_current_window);
        if (index_current_window >= windows.size()) {
            index_current_region = -1;
            return;
        }
        index_current_region = factories[windows[index_current_window].factory].front();
        return;
    }

    void solution::MatchRegion(short current_machine, short &start_region, char &start_window, bool is_core_line) {
        while (start_region != -1) {
            bool is_match = MatchEnergy(current_machine, start_region);
            if (is_match && is_core_line) {
                is_match = MatchInit(current_machine, start_window);
            }
            if (is_match) return;
            NextRegion(start_region, start_window);
        }
        start_region = -1;
    }

    bool solution::GetSolution() {
        
    }
} // MiuiIsTheBest