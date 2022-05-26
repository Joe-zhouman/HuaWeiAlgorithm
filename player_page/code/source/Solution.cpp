//
// Created by joe on 22-5-21.
//
#include "iostream"
#include "Solution.h"

//#define DEBUG
#define FULL_OUTPUT
#define MAX_ITER_TIMES 100000000
namespace MiuiIsTheBest {
    void Solution::RegainCycleTimes() {
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
        if (result) {
            op_cycled_windows.push(cycled_windows);
            return true;
        }
        return false;
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
            std::vector<short> index_correct_parent;
            if (current_machine->parents == nullptr && current_machine->OutOfPosition()) {
                return false;
            }
            while (!current_machine->OutOfPosition()) {
                bool all_parent_ok = true;
                index_wrong_parent.clear();
                index_correct_parent.clear();
                if (current_machine->parents != nullptr) {
                    for (short index_parent: *current_machine->parents) {
                        succesor = IsSuccessor(current_machine->CurrentWindow(),
                                               machines[index_parent].CurrentWindow(),
                                               machines[index_parent].GetChildConnectType(index_current_machine));

                        if (!succesor) {
                            index_wrong_parent.emplace_back(index_parent);
                            all_parent_ok = false;
                        } else {
                            index_correct_parent.emplace_back(index_parent);
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
                    for (int i = 0; i < index_correct_parent.size(); ++i) {
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
                short back_tracking_position = 0;
                for (short num_matched_wrong_parents = 0;
                     num_matched_wrong_parents < index_wrong_parent.size(); index_current_step++) {
#ifdef DEBUG
                    std::cout<<"Reset machine "<<machine_index[index_current_step]<<std::endl;
#endif
                    index_current_machine = machine_index[index_current_step];
                    for (short index: index_wrong_parent) {
                        if (index_current_machine == index) {
                            num_matched_wrong_parents++;
                            back_tracking_position = machines[index].current_position;
                        }
                    }
                    for (short index: index_correct_parent) {
                        if (index_current_machine == index) {
                            RegainCycleTimes();
                        }
                    }
                    machines[index_current_machine].current_position = 0;
                }
                machines[index_current_machine].current_position = back_tracking_position++;
                index_current_step--;
            }
        }
        return index_current_step < num_machine;
    }

    Solution::Solution() {
        std::cin.ignore(100, '\n');
        std::cin.ignore(100, '\n');
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
        short window_cost[num_windows];
        for (short i = 0; i < num_windows; ++i) {
            Window temp_window;
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
            machines.emplace_back(Machine(machine_type1, first_energy, second_energy));
        }
        std::cin >> num_flow_line;

        for (short i = 0; i < num_flow_line; ++i) {
            FlowLine temp_flow_line;
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
                    current_machine->children_types->at(u) = E_TYPE::TREE;
                    if (!TSort(index_current_child, clock, S)) return false;
                    break;
                case V_STATUS::DISCOVERED:
                    current_machine->children_types->at(u) = E_TYPE::BACKWARD;
                    return false;
                default:
                    current_machine->children_types->at(u) = (current_machine->depth < current_child->depth)
                                                             ? E_TYPE::FORWARD : E_TYPE::CROSS;
                    break;
            }
        }
        current_machine->status = V_STATUS::VISITED;
        S->emplace_back(index_machine);
        return true;
    }

    void Solution::GetEnergyOrder(const int *machine_energy_cost, short &first_energy, short &second_energy, short i,
                                  short j) {
        if (machine_energy_cost[i] <= machine_energy_cost[j]) {
            first_energy = i;
            second_energy = j;
        } else {
            first_energy = j;
            second_energy = i;
        }
    }

    void Solution::MachineGraphInit() {
        for (FlowLine current_flow_line: flow_lines) {
            if (machines[current_flow_line.current_machine].is_core) {
                machines[current_flow_line.previous_machine].children->emplace_back(current_flow_line.current_machine);
                machines[current_flow_line.previous_machine].connect_types->emplace_back(current_flow_line.type);
                machines[current_flow_line.previous_machine].children_types->emplace_back(E_TYPE::UNDETERMINED);
            } else {
                machines[current_flow_line.previous_machine].children->insert(
                        machines[current_flow_line.previous_machine].children->begin(),
                        current_flow_line.current_machine);
                machines[current_flow_line.previous_machine].connect_types->insert(
                        machines[current_flow_line.previous_machine].connect_types->begin(), current_flow_line.type);
                machines[current_flow_line.previous_machine].children_types->insert(
                        machines[current_flow_line.previous_machine].children_types->begin(), E_TYPE::UNDETERMINED);
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
                if (!current_machine.is_core ||
                    (current_machine.is_core && current_window->init_type[current_machine.type.ToInt()])) {
                    for (short index_region = factories[current_window->factory].front();
                         index_region <= factories[current_window->factory].back(); index_region++) {
                        if (region_energy_types[index_region] == current_machine.first_energy_type) {
                            current_machine.allowed_windows->emplace_back(index_window);
                            current_machine.allowed_regions->emplace_back(index_region);
                        }
                        if (region_energy_types[index_region] == current_machine.second_energy_type) {
                            current_machine.allowed_windows->emplace_back(index_window);
                            current_machine.allowed_regions->emplace_back(index_region);
                        }
                    }
                }
            }
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