//
// Created by joe on 22-5-21.
//
#include "iostream"
#include "Solution.h"
#include "algorithm"
//#include "assert.h"

//#define DEBUG
//#define FULL_OUTPUT
namespace MiuiIsTheBest {

    bool Solution::IsSuccessor(Machine *current_machine, short parent_index) {
//        assert(current_machine->parents != nullptr);
        short index_parent = machines[current_machine->parents->at(parent_index)].CurrentWindow();
        short index_current = current_machine->CurrentWindow();
        if (index_parent < index_current) return true;
        if (index_parent > index_current && index_parent < num_cycle_windows &&
            windows[num_cycle_windows - 1].rest_cycle_times > 0) {
            windows[num_cycle_windows - 1].rest_cycle_times--;
            current_machine->cycled_windows->at(parent_index) = num_cycle_windows - 1;
            return true;
        }
        if (index_parent == index_current && current_machine->connect_types->at(parent_index)) return true;
        if (index_parent == index_current && !current_machine->connect_types->at(parent_index) &&
            windows[index_parent].rest_cycle_times > 0) {
            windows[index_current].rest_cycle_times--;
            current_machine->cycled_windows->at(parent_index) = index_current;
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
    bool Solution::GetSolution() {
        for (Start &start: starts) {
            if (!TSort(start.index, &machine_index)) {
                return false;
            }
            start.depth = machine_index.size();
            machine_index.clear();
            Reset();
        }
        std::sort(starts.begin(), starts.end());
        for (Start start: starts) {
            if (!TSort(start.index, &machine_index)) {
                return false;
            }
        }
        if (machine_index.size() < 1)return false;
        index_current_step = machine_index.size() - 1;
        short index_current_machine;
        Machine *current_machine;

        while (index_current_step >= 0 && index_current_step < num_machine) {
//#ifdef DEBUG
//            std::cout << "Find Region for machine " << machine_index[index_current_step] << std::endl;
//#endif
            index_current_machine = machine_index[index_current_step];
            current_machine = &machines[index_current_machine];
//            if (current_machine->parents == nullptr) {
//                current_machine->current_position++;
//                if (current_machine->OutOfPosition()) {
//                    return false;
//                }
//                index_current_step--;
//                index_current_machine = machine_index[index_current_step];
//                current_machine = &machines[index_current_machine];
//            }
            while (!current_machine->OutOfPosition()) {
                if (current_machine->parents != nullptr) {
                    bool succesor = false;
                    for (short i = 0; i < current_machine->NumParents(); i++) {
                        succesor = IsSuccessor(current_machine, i);
                        if (!succesor) {
                            current_machine->RegainCycleTimes(windows);
                            current_machine->current_position++;
                            break;
                        }
                    }
                    if (succesor) {
//#ifdef DEBUG
//                        std::cout << "Set window " << current_machine->CurrentWindow()
//                                  << " and region " << current_machine->CurrentRegion() << " for machine "
//                                  << machine_index[index_current_step] << std::endl;
//#endif
                        break;
                    }
                } else {
                    break;
                }
            }
            if (!current_machine->OutOfPosition()) {
                index_current_step--;
            } else {
                while (current_machine->OutOfPosition()) {
                    if (current_machine->parents == nullptr) return false;
//#ifdef DEBUG
//                    std::cout << "Machine " << machine_index[index_current_step] << " is out of position" << std::endl;
//#endif
//                    short back_tracking_position = 0;
                    std::vector<short> *back_tracking_machines = current_machine->parents;
                    short num_matched_wrong_parents = 0;
                    while (num_matched_wrong_parents < back_tracking_machines->size()) {
                        current_machine->RegainCycleTimes(windows);
                        current_machine->current_position = 0;
                        index_current_step++;
                        index_current_machine = machine_index[index_current_step];
                        current_machine = &machines[index_current_machine];
//#ifdef DEBUG
//                        std::cout << "Reset machine " << machine_index[index_current_step] << std::endl;
//#endif
                        for (short index: *back_tracking_machines) {
                            if (index_current_machine == index) {
                                num_matched_wrong_parents++;
//                                back_tracking_position = current_machine->current_position;
                            }
                        }
                    }
                    current_machine->current_position++;
                    current_machine->RegainCycleTimes(windows);
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
    }

    bool Solution::TSort(short index_machine, std::vector<short> *S) {
        Machine *current_machine = &machines[index_machine];
        current_machine->status = V_STATUS::DISCOVERED;
        if (current_machine->children != nullptr) {


            for (short u = 0; u < current_machine->children->size(); u++) {
                short index_current_child = current_machine->children->at(u).index;
                Machine *current_child = &machines[index_current_child];
                switch (current_child->status) {
                    case V_STATUS::UNDISCOVERED:
                        if (!TSort(index_current_child, S)) return false;
                        break;
                    case V_STATUS::DISCOVERED:
                        return false;
                    default:
                        break;
                }
        }
        }
        current_machine->status = V_STATUS::VISITED;
        S->emplace_back(index_machine);
        return true;
    }

    bool Solution::MachineGraphInit() {
        for (FlowLine current_flow_line: flow_lines) {
            machines[current_flow_line.previous_machine].children->emplace_back(current_flow_line.current_machine);
            machines[current_flow_line.current_machine].connect_types->emplace_back(current_flow_line.type);
            machines[current_flow_line.current_machine].parents->emplace_back(current_flow_line.previous_machine);
            machines[current_flow_line.current_machine].cycled_windows->emplace_back(-1);
        }
        for (short i = 0; i < num_machine; i++) {
            if (machines[i].parents->size() == 0) {
                machines[i].parents = nullptr;
                starts.emplace_back(Start(i));
            }
            if (machines[i].children->size() == 0)machines[i].children = nullptr;
        }
        for (Machine &machine: machines) {
            if (machine.children == nullptr)continue;
            if (machine.children->size() > 1) {
                for (Start &start: *machine.children) {
                    if (!TSort(start.index, &machine_index)) {
                        return false;
                    }
                    start.depth = machine_index.size();
                    std::sort(machine.children->begin(), machine.children->end());
                    machine_index.clear();
                    Reset();
                }
            }
        }
        return true;
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
//        for (Machine &machine: machines) {
//            for (int i = 0; i < machine.positions->size(); ++i) {
//                unsigned int cost = 0;
//                cost += machine.cost[region_energy_types[machine.positions->at(i).region].ToInt()];
//                if (machine.is_core) {
//                    cost += manu_time[machine.type.ToInt()] * (K + windows[machine.positions->at(i).window].cost_coeff);
//                }
//                machine.positions->at(i).cost = cost;
//            }
//            std::sort(machine.positions->begin(), machine.positions->end());
//        }
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
//#ifdef FULL_OUTPUT
//        for (Machine &current_machine: machines) {
//            std::cout << current_machine.CurrentWindow() << ' ';
//        }
//        std::cout << '\n';
//#endif
    }
} // MiuiIsTheBest