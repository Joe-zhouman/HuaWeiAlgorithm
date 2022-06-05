//
// Created by joe on 22-5-21.
//
#include "iostream"
#include "Solution.h"
#include "algorithm"
#include "ordered_list.h"
//#include "assert.h"

//#define DEBUG
//#define FULL_OUTPUT
namespace MiuiIsTheBest {

    bool Solution::IsSuccessor(Machine *current_machine, int parent_index) {
//        assert(current_machine->parents != nullptr);
        int index_parent = machines[current_machine->parents->at(parent_index)].CurrentWindow();
        int index_current = current_machine->CurrentWindow();
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

    bool Solution::TSortBfs() {
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
        return true;
    }

    bool Solution::GetSolution() {
        for (int i = 0; i < machine_index.size(); ++i) {
            machines[machine_index[i]].depth = i;
        }
        if (machine_index.size() < 1)return false;
        index_current_step = machine_index.size() - 1;
        int index_current_machine;
        Machine *current_machine;

        while (index_current_step >= 0 && index_current_step < num_machine) {
//#ifdef DEBUG
//            std::cout << "Find Region for machine " << machine_index[index_current_step] << std::endl;
//#endif
            index_current_machine = machine_index[index_current_step];
            current_machine = &machines[index_current_machine];
            std::vector<int> back_tracking;
            std::vector<int> *back_tracking_machines = &back_tracking;
            while (!current_machine->OutOfPosition()) {
                if (current_machine->parents != nullptr) {
                    bool succesor = false;
                    bool all_ok = true;
                    back_tracking_machines->clear();
                    for (int i = 0; i < current_machine->NumParents(); i++) {
                        succesor = IsSuccessor(current_machine, i);
                        if (!succesor) {
                            back_tracking_machines->emplace_back(current_machine->parents->at(i));
                            all_ok = false;
                        }
                    }
                    if (!all_ok) {
                        current_machine->RegainCycleTimes(windows);
                        current_machine->current_position++;
                        continue;
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
                    if (current_machine->parents == nullptr) {
                        return false;
                    }
//#ifdef DEBUG
//                    std::cout << "Machine " << machine_index[index_current_step] << " is out of position" << std::endl;
//#endif
                    int back_tracking_step = 0;

                    for (int &index: *back_tracking_machines) {
                        int position = machines[index].depth;
                        if (position > back_tracking_step) back_tracking_step = position;
                    }
                    while (index_current_step < back_tracking_step) {
                        current_machine->RegainCycleTimes(windows);
                        current_machine->current_position = 0;
                        index_current_step++;
                        index_current_machine = machine_index[index_current_step];
                        current_machine = &machines[index_current_machine];
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
            std::vector<int> temp_factory;
            factories.emplace_back(temp_factory);
        }
        std::cin >> num_regions;
        int factory, energy;
        for (int i = 0; i < num_regions; ++i) {
            std::cin >> factory >> energy;
            factories[factory].emplace_back(i);
            region_energy_types.emplace_back(energy);
        }
        int max_cycle_times;
        std::cin >> max_cycle_times;
        max_cycle_times++;
        std::cin >> num_cycle_windows;
        std::cin >> num_windows;
        bool self_cycle;
        for (int i = 0; i < num_windows; ++i) {
            windows.emplace_back(Window());
            std::cin >> self_cycle >> windows.back().factory >> windows.back().cost_coeff >> windows.back().init_type[0]
                     >> windows.back().init_type[1] >> windows.back().init_type[2];
            if (i == num_cycle_windows - 1) {
                windows.back().rest_cycle_times = max_cycle_times;
            } else {
                windows.back().rest_cycle_times = self_cycle ? max_cycle_times : 0;
            }
        }
        std::cin >> num_machine;
        for (int i = 0; i < num_machine; ++i) {
            int machine_type;
            std::cin >> machine_type;
            machines.emplace_back(Machine(machine_type));
            for (int j = 0; j < 5; j++) {
                std::cin >> machines.back().cost[j];
            }
        }
        std::cin >> num_flow_line;
        for (int i = 0; i < num_flow_line; ++i) {
            FlowLine temp_flow_line;
            std::cin >> temp_flow_line.type >> temp_flow_line.previous_machine >> temp_flow_line.current_machine;
            flow_lines.push_back(temp_flow_line);
        }
        std::cin >> num_core_flow_line;
        int core_index;
        for (int i = 0; i < num_core_flow_line; ++i) {
            std::cin >> core_index;
            int previous_machine = flow_lines[core_index].previous_machine;
            int current_machine = flow_lines[core_index].current_machine;
            machines[flow_lines[core_index].current_machine].is_core = true;
            machines[flow_lines[core_index].previous_machine].is_core = true;
            if (core_line_machines.empty()) {
                core_line_machines.emplace_front(current_machine);
                core_line_machines.emplace_front(previous_machine);
            } else if (current_machine == core_line_machines.front()) {
                core_line_machines.emplace_front(previous_machine);
            } else {
                core_line_machines.emplace_back(current_machine);
            }
        }
    }

    bool Solution::TSort(int index_machine, std::vector<int> *S) {
        Machine *current_machine = &machines[index_machine];
        current_machine->status = V_STATUS::DISCOVERED;
        if (current_machine->children != nullptr) {
            for (int u = 0; u < current_machine->children->size(); u++) {
                int index_current_child = current_machine->children->at(u).index;
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
        for (int i = 0; i < num_machine; i++) {
            if (machines[i].parents->size() == 0) {
                machines[i].parents = nullptr;
                starts.emplace_back(Start(i));
            }
            if (machines[i].children->size() == 0)machines[i].children = nullptr;
        }
//        for (Machine &machine: machines) {
//            if (machine.children == nullptr)continue;
//            if (machine.children->size() > 1) {
//                for (Start &start: *machine.children) {
//                    if (!TSort(start.index, &machine_index)) {
//                        return false;
//                    }
//                    start.depth = machine_index.size();
//                    std::sort(machine.children->begin(), machine.children->end());
//                    machine_index.clear();
//                    Reset();
//                }
//            }
//        }
        return true;
    }

    void Solution::MachinePositionInit() {
        for (int i = 0; i < 6; ++i) {
            total_cost[i] = 0;
        }
        for (Machine machine: machines) {
            if (machine.type == 0) {
                total_cost[0] += machine.cost[0];
                total_cost[1] += machine.cost[1];
            } else if (machine.type == 1) {
                total_cost[2] += machine.cost[0];
                total_cost[3] += machine.cost[2];
            } else {
                total_cost[4] += machine.cost[3];
                total_cost[5] += machine.cost[4];
            }
        }
        int temp[6];
        if (total_cost[0] > total_cost[1]) {
            temp[0] = 1;
            temp[1] = 0;
        } else {
            temp[0] = 0;
            temp[1] = 1;
        }
        if (total_cost[2] > total_cost[3]) {
            temp[2] = 2;
            temp[3] = 0;
        } else {
            temp[2] = 0;
            temp[3] = 2;
        }
        if (total_cost[4] > total_cost[5]) {
            temp[4] = 4;
            temp[5] = 3;
        } else {
            temp[4] = 3;
            temp[5] = 4;
        }
        ordered_list<Window> temp_window_index;
        for (Window &window: windows) {
            window.cost = (K + window.cost_coeff);
            temp_window_index.insert(window);
        }
        for (int index_window = 0; index_window < num_windows; index_window++) {
            for (int index_region: factories[windows[index_window].factory]) {
                if (region_energy_types[index_region] == temp[0] || region_energy_types[index_region] == temp[1]) {
                    NonCorePosition[0].emplace_back(Position(index_window, index_region, 0));
                    if (windows[index_window].init_type[0]) {
                        CorePosition[0].emplace_back(Position(index_window, index_region, 0));
                    }
                }
                if (region_energy_types[index_region] == temp[2] || region_energy_types[index_region] == temp[3]) {
                    NonCorePosition[1].emplace_back(Position(index_window, index_region, 0));
                    if (windows[index_window].init_type[1]) {
                        CorePosition[1].emplace_back(Position(index_window, index_region, 0));
                    }
                }
                if (region_energy_types[index_region] == temp[4] || region_energy_types[index_region] == temp[5]) {
                    NonCorePosition[2].emplace_back(Position(index_window, index_region, 0));
                    if (windows[index_window].init_type[2]) {
                        CorePosition[2].emplace_back(Position(index_window, index_region, 0));
                    }
                }
            }
        }
//        for (int i = 0; i < 3; ++i) {
//            for (Position &position: CorePosition[i]) {
//                position.cost = manu_time[i] * (K + windows[position.window].cost_coeff);
//            }
//            std::sort(CorePosition[i].begin(), CorePosition[i].end());
//        }
        for (Machine &machine: machines) {
            int type = machine.type;
            if (machine.is_core) {
                machine.positions = CorePosition[type];
//                for (Position &position: CorePosition[type]) {
//                    position.cost = 0;
//                    position.cost += machine.cost[region_energy_types[position.region]] +
//                                     manu_time[type] * (K + windows[position.window].cost_coeff);
//                    if (machine.positions.empty()) {
//                        machine.positions.emplace_back(position);
//                    } else {
//                        machine.positions.emplace(
//                                std::lower_bound(machine.positions.begin(), machine.positions.end(), position),
//                                position);
////                        if (cost < machine.positions.front().cost) {
////                            machine.positions.emplace(machine.positions.begin(), position);
////                        } else {
////                            machine.positions.emplace_back(position);
////                        }
//                    }
//                }
            } else {
                machine.positions = NonCorePosition[type];
//                for (Position position: CorePosition[type]) {
//                    unsigned int cost = 0;
//                    cost += machine.cost[region_energy_types[position.region]];
//                    if (machine.positions.empty()) {
//                        machine.positions.emplace_back(position);
//                    } else {
//                        if (cost < machine.positions.front().cost) {
//                            machine.positions.emplace(machine.positions.begin(), position);
//                        } else {
//                            machine.positions.emplace_back(position);
//                        }
//                    }
//                }
            }
        }
//        for (Machine &current_machine: machines) {
//            if (current_machine.is_core) {
//                current_machine.positions = CorePosition[current_machine.type];
//            } else {
//                current_machine.positions = NonCorePosition[current_machine.type];
//            }
//        }
//        for (Machine &machine: machines) {
//            if (!machine.is_core)continue;
//            for (int i = 0; i < machine.positions.size(); ++i) {
//                unsigned int cost = 0;
//                cost += machine.cost[region_energy_types[machine.positions[i].region]];
//                cost += manu_time[machine.type] * (K + windows[machine.positions.at(i).window].cost_coeff);
//                machine.positions.at(i).cost = cost;
//            }
//            std::sort(machine.positions.begin(), machine.positions.end());
//        }
    }

    void Solution::OutPut() {
        std::cout << num_machine << '\n';
        for (Machine &current_machine: machines) {
            std::cout << current_machine.CurrentRegion() << ' ';
        }
        std::cout << '\n';
        std::cout << num_core_flow_line + 1 << '\n';
        for (int index: core_line_machines) {
            std::cout << machines[index].CurrentWindow() << " ";
        }

//#ifdef FULL_OUTPUT
//        std::cout<<std::endl;
//        for (Machine &machine: machines) {
//            std::cout << machine.CurrentWindow() << ' ';
//        }
//        std::cout << '\n';
//#endif
    }
} // MiuiIsTheBest