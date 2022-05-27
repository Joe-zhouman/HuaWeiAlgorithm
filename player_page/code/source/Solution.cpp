//
// Created by joe on 22-5-21.
//
#include "iostream"
#include "Solution.h"
#include "algorithm"
#include "assert.h"

#define DEBUG
#define FULL_OUTPUT
namespace MiuiIsTheBest {
    void Solution::RegainCycleTimes() {
        assert(!op_cycled_windows.empty());
        std::vector<short> *cycled_windows = &op_cycled_windows.top();
        for (short cycled_window: *cycled_windows) {
            windows[cycled_window].rest_cycle_times++;
        }
        op_cycled_windows.pop();
    }

    bool Solution::IsSuccessor(short current_window, short previous_window, bool flow_line_type) {
        bool result = false;
        std::vector<short> cycled_windows;
        if (previous_window < current_window) result = true;
        if (previous_window > current_window && previous_window < num_cycle_windows &&
            windows[num_cycle_windows - 1].rest_cycle_times > 0) {
            windows[num_cycle_windows - 1].rest_cycle_times--;
            cycled_windows.emplace_back(num_cycle_windows - 1);
            result = true;
        }
        if (previous_window == current_window && flow_line_type) result = true;
        if (previous_window == current_window && !flow_line_type && windows[current_window].rest_cycle_times > 0) {
            windows[current_window].rest_cycle_times--;
            cycled_windows.emplace_back(current_window);
            result = true;
        }
        op_cycled_windows.push(cycled_windows);
        return result;

    }

    Machine *Solution::GetCurrentMachine(short index_flow_line) {
        return &machines[flow_lines[index_flow_line].current_machine];
    }

    Machine *Solution::GetPreviousMachine(short index_flow_line) {
        return &machines[flow_lines[index_flow_line].previous_machine];
    }

    short Solution::GetCurrentWindow(short index_flow_line) {
        return GetCurrentMachine(index_flow_line)->CurrentWindow();
    }

    short Solution::GetPreviousWindow(short index_flow_line) {
        return GetPreviousMachine(index_flow_line)->CurrentWindow();
    }

    bool Solution::GetSolution() {
        std::vector<short> machine_index;
        short clock = 0;
        if (!TSort(0, clock, &machine_index)) {
            return false;
        }
        index_current_step = machine_index.size() - 2;
        short index_current_machine;
        Machine *current_machine;
        bool succesor = false;
        while (index_current_step >= 0 && index_current_step < machine_index.size()) {
#ifdef DEBUG
            std::cout<<"Find Region for machine "<<machine_index[index_current_step]<<std::endl;
#endif
            index_current_machine = machine_index[index_current_step];
            current_machine = &machines[index_current_machine];
            std::vector<short> index_wrong_parent;
            if (current_machine->parents == nullptr && current_machine->OutOfPosition()) {
                return false;
            }
            while (!current_machine->OutOfPosition()) {
                bool all_parent_ok = true;
                index_wrong_parent.clear();
                if (current_machine->parents != nullptr) {
                    for (short index_parent: *current_machine->parents) {
                        succesor = IsSuccessor(current_machine->CurrentWindow(),
                                               machines[index_parent].CurrentWindow(),
                                               machines[index_parent].GetChildConnectType(index_current_machine));

                        if (!succesor) {
                            index_wrong_parent.emplace_back(index_parent);
                            all_parent_ok = false;
                        }
                    }
                    if (all_parent_ok) {
#ifdef DEBUG
                        std::cout<<"Set window "<<current_machine->CurrentWindow()
                        <<" and region "<<current_machine->CurrentRegion()<<" for machine "
                        <<machine_index[index_current_step]<<std::endl;
#endif
                        break;
                    }
                    for (int i = 0; i < current_machine->parents->size(); ++i) {
                        RegainCycleTimes();
                    }
                    current_machine->current_position++;
                    continue;
                }
            }
            if (!current_machine->OutOfPosition()) {
                index_current_step--;
                continue;
            } else {
                if (current_machine->parents == nullptr) return false;
                for (int i = 0; i < current_machine->parents->size(); i++) {
                    RegainCycleTimes();
                }
                index_current_step++;
                short back_tracking_position = 0;
                for (short num_matched_wrong_parents = 0;; index_current_step++) {
#ifdef DEBUG
                    std::cout << "Reset machine " << machine_index[index_current_step] << std::endl;
#endif
                    index_current_machine = machine_index[index_current_step];
                    current_machine = &machines[index_current_machine];
                    for (short index: index_wrong_parent) {
                        if (index_current_machine == index) {
                            num_matched_wrong_parents++;
                        }
                    }
                    if (num_matched_wrong_parents == index_wrong_parent.size())break;
                    for (int i = 0; i < current_machine->parents->size(); ++i) {
                        RegainCycleTimes();
                    }
                    current_machine->current_position = 0;
                }
                current_machine->current_position++;
                while (current_machine->OutOfPosition()) {
                    index_wrong_parent.clear();
                    if (current_machine->parents == nullptr) return false;
                    for (short index: *current_machine->parents) {
                        index_wrong_parent.emplace_back(index);
                    }
                    for (short num_matched_wrong_parents = 0;; index_current_step++) {
#ifdef DEBUG
                        std::cout << "Reset machine " << machine_index[index_current_step]
                                  << " for out of position" << std::endl;
#endif
                        index_current_machine = machine_index[index_current_step];
                        current_machine = &machines[index_current_machine];

                        for (short index: index_wrong_parent) {
                            if (index_current_machine == index) {
                                num_matched_wrong_parents++;
                            }
                        }
                        if (num_matched_wrong_parents == index_wrong_parent.size())break;
                        for (int i = 0; i < current_machine->parents->size(); ++i) {
                            RegainCycleTimes();
                        }
                        current_machine->current_position = 0;
                    }
                    current_machine->current_position++;
                }
            }
        }

        return index_current_step < num_machine;
    }

    Solution::Solution() {
        std::cin >> K;
        std::cin >> manu_time[0] >> manu_time[1] >> manu_time[2] >> manu_time[3] >> manu_time[4];
        std::cin >> num_factories;
        for (int i = 0; i < num_factories; ++i) {
            std::vector<short> temp_factory;
            factories.emplace_back(temp_factory);
        }
        std::cin >> num_regions;
        short factory, energy;
        for (short i = 0; i < num_regions; ++i) {
            std::cin >> factory >> energy;
            factories[factory].emplace_back(i);
            region_energy_types.emplace_back(BoolType(energy));
        }
        short max_cycle_times;
        std::cin >> max_cycle_times;
        max_cycle_times++;
        std::cin >> num_cycle_windows;
        std::cin >> num_windows;
        bool self_cycle;
        short factory_index;
        for (short i = 0; i < num_windows; ++i) {
            Window temp_window;
            std::cin >> self_cycle >> factory_index >> temp_window.cost_coeff >> temp_window.init_type[0]
                     >> temp_window.init_type[1] >> temp_window.init_type[2];

            temp_window.factory = factory_index;
            if (i == num_cycle_windows - 1) {
                temp_window.rest_cycle_times = max_cycle_times;
            } else {
                temp_window.rest_cycle_times = self_cycle ? max_cycle_times : 0;
            }
            windows.push_back(temp_window);
        }

        std::cin >> num_machine;
        for (short i = 0; i < num_machine; ++i) {
            short machine_type;
            std::cin >> machine_type;
            machines.emplace_back(Machine(machine_type));
            for (int j = 0; j < 5; j++) {
                std::cin >> machines.back().cost[j];
            }

        }
        std::cin >> num_flow_line;

        for (short i = 0; i < num_flow_line; ++i) {
            FlowLine temp_flow_line;
            std::cin >> temp_flow_line.type >> temp_flow_line.previous_machine >> temp_flow_line.current_machine;
            flow_lines.push_back(temp_flow_line);
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
        MachinePositionInit();
        MachineGraphInit();
    }

    bool Solution::TSort(short index_machine, short &clock, std::vector<short> *S) {
        Machine *current_machine = &machines[index_machine];
        current_machine->depth = ++clock;
        current_machine->status = V_STATUS::DISCOVERED;
        for (short u = 0; current_machine->children != nullptr && u < current_machine->children->size(); u++) {
            short index_current_child = current_machine->children->at(u);
            Machine *current_child = &machines[index_current_child];
            switch (current_child->status) {
                case V_STATUS::UNDISCOVERED:
                    if (!TSort(index_current_child, clock, S)) return false;
                    break;
                case V_STATUS::DISCOVERED:
                    return false;
                default:
                    break;
            }
        }
        current_machine->status = V_STATUS::VISITED;
        S->emplace_back(index_machine);
        return true;
    }

    void Solution::MachineGraphInit() {
        for (FlowLine current_flow_line: flow_lines) {
            if (machines[current_flow_line.current_machine].is_core) {
                machines[current_flow_line.previous_machine].children->emplace_back(current_flow_line.current_machine);
                machines[current_flow_line.previous_machine].connect_types->emplace_back(current_flow_line.type);
            } else {
                machines[current_flow_line.previous_machine].children->insert(
                        machines[current_flow_line.previous_machine].children->begin(),
                        current_flow_line.current_machine);
                machines[current_flow_line.previous_machine].connect_types->insert(
                        machines[current_flow_line.previous_machine].connect_types->begin(), current_flow_line.type);
            }
            machines[current_flow_line.current_machine].parents->emplace_back(current_flow_line.previous_machine);
        }
        for (Machine &machine: machines) {
            if (machine.parents->size() == 0)machine.parents = nullptr;
            if (machine.children->size() == 0)machine.children = nullptr;
        }
    }

    void Solution::MachinePositionInit() {
        for (Machine &current_machine: machines) {
            for (short index_window = 0; index_window < num_windows; index_window++) {
                Window *current_window = &windows[index_window];
                int current_machine_type = current_machine.type.ToInt();
                if (!current_machine.is_core ||
                    (current_machine.is_core && current_window->init_type[current_machine_type])) {
                    for (short index_region: factories[current_window->factory]) {
                        if (current_machine_type == 0) {
                            if (region_energy_types[index_region] == BoolType(0) ||
                                region_energy_types[index_region] == BoolType(1)) {
                                current_machine.positions->emplace_back(Position(index_window, index_region, 0));
                            }
                        } else if (current_machine_type == 1) {
                            if (region_energy_types[index_region] == BoolType(0) ||
                                region_energy_types[index_region] == BoolType(2)) {
                                current_machine.positions->emplace_back(Position(index_window, index_region, 0));
                            }
                        } else if (current_machine_type == 2) {
                            if (region_energy_types[index_region] == BoolType(3) ||
                                region_energy_types[index_region] == BoolType(4)) {
                                current_machine.positions->emplace_back(Position(index_window, index_region, 0));
                            }
                        }
                    }
                }
            }
        }
        for (Machine &machine: machines) {
            for (int i = 0; i < machine.positions->size(); ++i) {
                unsigned int cost = 0;
                cost += machine.cost[region_energy_types[machine.positions->at(i).region].ToInt()];
                if (machine.is_core) {
                    cost += manu_time[machine.type.ToInt()] * (K + windows[machine.positions->at(i).window].cost_coeff);
                }
                machine.positions->at(i).cost = cost;
            }
            std::sort(machine.positions->begin(), machine.positions->end());
        }
    }

    void Solution::OutPut() {
        std::cout << num_machine << '\n';
        for (Machine &current_machine: machines) {
            std::cout << current_machine.CurrentRegion() << ' ';
        }
        std::cout << '\n';
        std::cout << num_core_flow_line + 1 << '\n';
        short i = 0;
        for (; i < num_flow_line; ++i) {
            if (flow_lines[i].is_core) {
                Machine *previous_machine = GetPreviousMachine(i);
                Machine *current_machine = GetCurrentMachine(i);
                std::cout << previous_machine->CurrentWindow() << ' '
                          << current_machine->CurrentWindow() << ' ';
                break;
            }
        }
        for (++i; i < num_flow_line; ++i) {
            if (flow_lines[i].is_core) {
                std::cout << GetCurrentMachine(i)->CurrentWindow() << ' ';
            }
        }
        std::cout << '\n';
#ifdef FULL_OUTPUT
        for (Machine &current_machine: machines) {
            std::cout << current_machine.CurrentWindow() << ' ';
        }
        std::cout << '\n';
#endif
    }
} // MiuiIsTheBest