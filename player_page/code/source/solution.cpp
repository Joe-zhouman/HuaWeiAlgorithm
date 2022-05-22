//
// Created by joe on 22-5-21.
//
#include "iostream"
#include "solution.h"

namespace MiuiIsTheBest {
    void solution::RegainCycleTimes(std::vector<short> &cycled_windows) {
        for (short cycled_window: cycled_windows) {
            windows[cycled_window].rest_cycle_times++;
        }
    }

    bool solution::IsSuccessor(short first_window, short second_window, std::vector<short> cycled_windows) {
        if (second_window > first_window)return true;
        if (second_window < first_window && first_window < num_cycle_windows &&
            windows[num_cycle_windows - 1].rest_cycle_times > 0) {
            windows[num_cycle_windows - 1].rest_cycle_times--;
            cycled_windows.emplace_back(num_cycle_windows - 1);
            return true;
        }
        if (second_window == first_window && windows[first_window].rest_cycle_times > 0) {
            windows[first_window].rest_cycle_times--;
            cycled_windows.emplace_back(num_cycle_windows - 1);
            return true;
        }
        return false;
    }

    machine *solution::GetCurrentMachine(short index_flow_line) {
        return &machines[flow_lines[index_flow_line].current_machine];
    }

    machine *solution::GetPreviousMachine(short index_flow_line) {
        return &machines[flow_lines[index_flow_line].previous_machine];
    }

    short solution::GetCurrentWindow(short index_flow_line) {
        machine *current_machine = GetCurrentMachine(index_flow_line);
        return current_machine->allowed_windows[current_machine->current_position];
    }

    short solution::GetPreviousWindow(short index_flow_line) {
        machine *current_machine = GetPreviousMachine(index_flow_line);
        return current_machine->allowed_windows[current_machine->current_position];
    }

    bool solution::GetSolution() {
        machines[0].current_position = 0;
        std::vector<short> cycled_windows;
        while (index_current_flow_line < num_flow_line && index_current_flow_line >= 0) {
            machine *current_machine = GetCurrentMachine(index_current_flow_line);
            short index_current_windows = GetCurrentWindow(index_current_flow_line);
            short index_previous_windows = GetPreviousWindow(index_current_flow_line);
            while (current_machine->HasNext()
                   || (!flow_lines[index_current_flow_line].type &&
                       !IsSuccessor(index_current_windows, index_previous_windows, cycled_windows))
                   || (flow_lines[index_current_flow_line].type && index_current_windows == index_previous_windows)) {
                current_machine->current_position++;
                op_cycled_windows.push(cycled_windows);
                cycled_windows.clear();
            }
            if (current_machine->HasNext()) {
                index_current_flow_line++;
                continue;
            }
            while (index_current_flow_line > current_machine->first_flow_line) {
                if (op_cycled_windows.empty()) return false;
                cycled_windows = op_cycled_windows.top();
                RegainCycleTimes(cycled_windows);
                cycled_windows.clear();
                index_current_flow_line--;
            }
            current_machine = GetCurrentMachine(index_current_flow_line);
            current_machine->current_position++;
        }
        return index_current_flow_line >= 0;
    }
    solution::solution() {

        std::cin.ignore(100, '\n');
        std::cin.ignore(100, '\n');
        std::cin >> num_factories;
        for (int i = 0; i < num_factories; ++i) {
            std::vector<short> temp_factory;
            factories.push_back(temp_factory);
        }
        std::cin >> num_regions;
        short factory, energy;
        for (short i = 0; i < num_regions; ++i) {
            std::cin >> factory >> energy;
            factories[factory].push_back(i);
            region_energy_types.emplace_back(energy_type(energy));
        }
        short max_cycle_times;
        std::cin >> max_cycle_times;
        max_cycle_times++;
        std::cin >> num_cycle_windows;
        std::cin >> num_windows;
        bool self_cycle;
        short factory_index;
        short window_cost[num_windows];
        for (short i = 0; i < num_windows; ++i) {
            window temp_window;
            std::cin >> self_cycle >> factory_index >> window_cost[i] >> temp_window.init_type[0]
                     >> temp_window.init_type[1] >> temp_window.init_type[2];

            temp_window.factory = factory_index;
            if (i < num_cycle_windows - 1) {
                temp_window.cnext = -1;
            } else if (i < num_cycle_windows) {
                temp_window.cnext = 0;
                temp_window.rest_cycle_times = max_cycle_times;
            } else {
                temp_window.cnext = self_cycle ? i : -1;
                temp_window.rest_cycle_times = self_cycle ? max_cycle_times : 0;
            }
            windows.push_back(temp_window);
        }
        for (short i = 0; i < num_windows - 1; ++i) {
            if (windows[i].cnext == -1) {
                windows[i].first_order_next = false;
            } else {
                if (window_cost[i + 1] >= window_cost[windows[i].cnext]) {
                    windows[i].first_order_next = true;
                } else {
                    windows[i].first_order_next = false;
                }
            }
        }
        windows[num_windows - 1].first_order_next = true;
        std::cin >> num_machine;
        short machine_type1;
        int machine_energy_cost[5];
        short first_energy;
        short second_energy;
        for (short i = 0; i < num_machine; ++i) {
            std::cin >> machine_type1 >> machine_energy_cost[0] >> machine_energy_cost[1] >> machine_energy_cost[2]
                     >> machine_energy_cost[3] >> machine_energy_cost[4];
            if (machine_type1 == 0) {
                GetEnergyOrder(machine_energy_cost, first_energy, second_energy, 0, 1);
            } else if (machine_type1 == 1) {
                GetEnergyOrder(machine_energy_cost, first_energy, second_energy, 0, 2);
            } else {
                GetEnergyOrder(machine_energy_cost, first_energy, second_energy, 3, 4);
            }
            machines.emplace_back(machine(machine_type1, first_energy, second_energy));
        }
        std::cin >> num_flow_line;

        for (short i = 0; i < num_flow_line; ++i) {
            flow_line temp_flow_line;
            std::cin >> temp_flow_line.type >> temp_flow_line.previous_machine >> temp_flow_line.current_machine;
            flow_lines.push_back(temp_flow_line);
            if (machines[flow_lines[i].current_machine].first_flow_line == -1) {
                machines[flow_lines[i].current_machine].first_flow_line = i;
            }
        }
        std::cin >> num_core_flow_line;
        short index_core_flow_line;
        std::cin >> index_core_flow_line;
        flow_lines[index_core_flow_line].is_core = true;
        machines[flow_lines[index_core_flow_line].current_machine].is_core = true;
        machines[flow_lines[index_core_flow_line].previous_machine].is_core = true;

        for (short i = 1; i < num_core_flow_line; ++i) {
            std::cin >> index_core_flow_line;
            flow_lines[index_core_flow_line].is_core = true;
            machines[flow_lines[index_core_flow_line].current_machine].is_core = true;
        }
        MachineInit();
    }

    void solution::GetEnergyOrder(const int *machine_energy_cost, short &first_energy, short &second_energy, short i,
                                  short j) {
        if (machine_energy_cost[i] <= machine_energy_cost[j]) {
            first_energy = i;
            second_energy = j;
        } else {
            first_energy = j;
            second_energy = i;
        }
    }

    void solution::MachineInit() {
        for (machine current_machine: machines) {
            for (short index_window = 0; index_window < num_windows; index_window++) {
                window *current_window = &windows[index_window];
                if (!current_machine.is_core ||
                    (current_machine.is_core && current_window->init_type[current_machine.type.to_int()])) {
                    for (short index_region = factories[current_window->factory].front();
                         index_region <= factories[current_window->factory].back(); index_region++) {
                        if (region_energy_types[index_region] == current_machine.first_energy_type) {
                            current_machine.allowed_windows.emplace_back(index_window);
                            current_machine.allowed_regions.emplace_back(index_region);
                        }
                    }
                }
            }
            for (short index_window = 0; index_window < num_windows; index_window++) {
                window *current_window = &windows[index_window];
                if (!current_machine.is_core ||
                    (current_machine.is_core && current_window->init_type[current_machine.type.to_int()])) {
                    for (short index_region = factories[current_window->factory].front();
                         index_region <= factories[current_window->factory].back(); index_region++) {
                        if (region_energy_types[index_region] == current_machine.second_energy_type) {
                            current_machine.allowed_windows.emplace_back(index_window);
                            current_machine.allowed_regions.emplace_back(index_region);
                        }
                    }
                }
            }
        }
    }

    void solution::OutPut() {
        std::cout << num_machine << '\n';
        for (machine current_machine: machines) {
            std::cout << current_machine.allowed_regions[current_machine.current_position] << ' ';
        }
        std::cout << '\n';
        std::cout << num_core_flow_line + 1 << '\n';
        short i = 0;
        for (; i < num_flow_line; ++i) {
            if (flow_lines[i].is_core) {
                machine *previous_machine = GetPreviousMachine(i);
                machine *current_machine = GetCurrentMachine(i);
                std::cout << previous_machine->allowed_windows[previous_machine->current_position] << ' '
                          << current_machine->allowed_windows[current_machine->current_position] << ' ';
                break;
            }
        }
        for (++i; i < num_flow_line; ++i) {
            if (flow_lines[i].is_core) {
                machine *current_machine = GetCurrentMachine(i);
                std::cout << current_machine->allowed_windows[current_machine->current_position] << ' ';
            }
        }
        std::cout << '\n';
#ifdef DEBUG
        for (short i = 0; i < num_machine; ++i) {
            std::cout << machine_windows[i] << ' ';
        }
        std::cout << '\n';
#endif
    }
} // MiuiIsTheBest