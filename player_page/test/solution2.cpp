//
// Created by joe on 22-5-2.
//
#define DEBUG

#include "solution2.h"
#include "iostream"

namespace MiuiIsTheBest {
    void solution2::GetRegion(short current_machine, short &index_current_region, short &index_current_window) {
        index_current_region = machine_regions[current_machine];
        index_current_window = machine_windows[current_machine];
    }

    void solution2::SetRegion(short current_machine, short index_current_region, short index_current_window) {
        machine_regions[current_machine] = index_current_region;
        machine_windows[current_machine] = index_current_window;
    }

    bool solution2::MatchInit(short index_machine, short index_window) {
        return windows[index_window].init_type[machine_types[index_machine].to_int()];
    }

    bool solution2::MatchEnergy(short index_machine, short index_region) {
        return region_energy_types[index_region] == machine_first_energy_type[index_machine] ||
               region_energy_types[index_region] == machine_second_energy_type[index_machine];
    }

    //TODO:有点问题
    void solution2::NextWindow(short &index_current_window, std::vector<short> &cycled_windows) {
        window *current_window = &windows[index_current_window];
        if (current_window->rest_cycle_times != 0 && current_window->first_order_next &&
            current_window->cnext != -1) {
            cycled_windows.push_back(index_current_window);
            current_window->rest_cycle_times--;
            index_current_window = current_window->cnext;
//            if(current_window->rest_cycle_times==0)
//                !current_window->first_order_next;
            return;
        }
        if (index_current_window < num_windows - 1) {
            index_current_window++;
        } else {
            index_current_window = current_window->cnext;
        }
    }

    //TODO:有点问题。。。
    void
    solution2::NextRegion(short &index_current_region, short &index_current_window,
                          std::vector<short> &cycled_windows) {
        if (GetFactory(index_current_window).back() > index_current_region) {
            index_current_region++;
            return;
        }
        NextWindow(index_current_window, cycled_windows);
        if (index_current_window >= num_windows) {
            index_current_region = -1;
            return;
        }
        index_current_region = GetFactory(index_current_window).front();
    }

    //TODO:有点问题。。。
    void solution2::MatchRegion(short current_machine, short &start_region, short &start_window, bool is_core_line,
                                std::vector<short> &cycled_windows) {
        while (start_region != -1 && start_window < num_windows) {
#ifdef DEBUG
            std::cout << "match machine " << current_machine << " with region " << start_region << '\n';
#endif

            bool is_match = MatchEnergy(current_machine, start_region);
            if (is_match && is_core_line) {
                is_match = MatchInit(current_machine, start_window);
            }
            if (is_match) return;
            NextRegion(start_region, start_window, cycled_windows);
        }
        start_region = -1;
    }

//TODO 需要增加逻辑以修正错误：如果当前机器已被安排，首先判断当前流水线是否成立，否则，需退回至该机器第一次出现的流水线，从其下一个位置开始匹配
//TODO 需要增加逻辑以简化：如果退回到初始窗口的下一个窗口优先级需进行修改
    bool solution2::GetSolution() {
        short current_region = 0;
        short current_window = 0;
        short temp_region;
        short temp_window;
        std::vector<short> cycled_windows;
        MatchRegion(flow_lines[0].previous_machine, current_region, current_window, flow_lines[0].is_core_line,
                    cycled_windows);
        SetRegion(0, current_region, current_window);
        if (!flow_lines[0].type) {
            NextWindow(current_window, cycled_windows);
        }
        current_region = GetFactory(current_window).front();
#ifdef DEBUG
        std::cout << "reset region for flow_line " << index_current_flow_line - 1 << " with machine 0" << '\n';
#endif
        op_cycled_windows.push(cycled_windows);
        cycled_windows.clear();
        while (index_current_flow_line < num_flow_line) {
            GetRegion(flow_lines[index_current_flow_line].current_machine, temp_region, temp_window);
            if (temp_region != -1) {
                short previous_region, previous_window;
                GetRegion(flow_lines[index_current_flow_line].previous_machine, previous_region, previous_window);
                if (IsSuccessor(previous_window, temp_window, cycled_windows)) {
                    index_current_flow_line++;
                    op_cycled_windows.push(cycled_windows);
                    cycled_windows.clear();
                    continue;
                } else {
#ifdef DEBUG
                    std::cout << "windows  " << temp_window << " is not the successor of " << previous_window << '\n';
#endif
                    short first_occur_flow_line = machine_first_flow_line[flow_lines[index_current_flow_line].current_machine];
                    short temp_machine = flow_lines[index_current_flow_line].current_machine;
                    index_current_flow_line--;
                    while (index_current_flow_line > first_occur_flow_line) {
                        if (flow_lines[index_current_flow_line].current_machine != temp_machine) {
                            SetRegion(flow_lines[index_current_flow_line].current_machine, -1, -1);
                        }
#ifdef DEBUG
                        std::cout << "reset region for flow_line " << index_current_flow_line << " with machine "
                                  << flow_lines[index_current_flow_line].current_machine << '\n';
#endif
                        index_current_flow_line--;
                        cycled_windows = op_cycled_windows.top();
                        op_cycled_windows.pop();
                        RegainCycleTimes(cycled_windows);
                        cycled_windows.clear();
                    }
                    GetRegion(flow_lines[index_current_flow_line].current_machine, current_region, current_window);
                    if (current_window == num_windows - 1) return false;
                    current_window++;
                    current_region = GetFactory(current_window).front();
                }
            }
            MatchRegion(flow_lines[index_current_flow_line].current_machine, current_region, current_window,
                        flow_lines[index_current_flow_line].is_core_line, cycled_windows);
            while (current_region < 0) {
#ifdef DEBUG
                std::cout << "reset region for flow_line " << index_current_flow_line - 1 << " with machine "
                          << flow_lines[index_current_flow_line].current_machine << '\n';
#endif
                short target_machine = flow_lines[index_current_flow_line].previous_machine;
                while (flow_lines[index_current_flow_line].current_machine != target_machine) {
                    if (index_current_flow_line == 0) return false;
                    SetRegion(flow_lines[index_current_flow_line].current_machine, -1, -1);
#ifdef DEBUG
                    std::cout << "reset region for flow_line " << index_current_flow_line - 1 << " with machine "
                              << flow_lines[index_current_flow_line].current_machine << '\n';
#endif
                    index_current_flow_line--;
                    cycled_windows = op_cycled_windows.top();
                    op_cycled_windows.pop();
                    RegainCycleTimes(cycled_windows);
                    cycled_windows.clear();
                }
                cycled_windows.clear();
                GetRegion(flow_lines[index_current_flow_line].current_machine, current_region, current_window);
                if (current_region < GetFactory(current_window).back()) {
                    current_region++;
                } else {
                    if (windows[current_window].first_order_next || windows[current_window].rest_cycle_times == 0) {
                        current_window++;
                    } else {
                        current_window = windows[current_window].cnext;
                        windows[current_window].rest_cycle_times--;
                        cycled_windows.emplace_back(current_window);
                    }
                    current_region = GetFactory(current_window).front();
                }
                MatchRegion(flow_lines[index_current_flow_line].current_machine, current_region, current_window,
                            flow_lines[index_current_flow_line].is_core_line, cycled_windows);
            }

            op_cycled_windows.push(cycled_windows);
            SetRegion(flow_lines[index_current_flow_line].current_machine, current_region, current_window);
#ifdef DEBUG
            std::cout << "set region for flow_line " << index_current_flow_line << " with machine "
                      << flow_lines[index_current_flow_line].current_machine << '\n';
#endif
            index_current_flow_line++;
            cycled_windows.clear();
            GetRegion(flow_lines[index_current_flow_line].previous_machine, current_region, current_window);
            if (!flow_lines[index_current_flow_line].type) {
                NextWindow(current_window, cycled_windows);
            }
            current_region = GetFactory(current_window).front();
        }
        return true;
    }

    bool solution2::IsSuccessor(short first_window, short second_window, std::vector<short> cycled_windows) {
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

    void solution2::RegainCycleTimes(std::vector<short> &cycled_windows) {
        for (short cycled_window: cycled_windows) {
            windows[cycled_window].rest_cycle_times++;
//            !windows[cycled_window].first_order_next;
        }
    }

    inline std::vector<short> &solution2::GetFactory(short current_window) {
        return factories[windows[current_window].factory];
    }

    solution2::solution2() {
        num_flow_line = 0;
        num_core_flow_line = 0;
        num_windows = 0;
        num_machine = 0;
        num_regions = 0;
        num_factories = 0;
        num_cycle_windows = 0;
        index_current_flow_line = 0;
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
            machine_regions.emplace_back(-1);
            machine_windows.emplace_back(-1);
            std::cin >> machine_type1 >> machine_energy_cost[0] >> machine_energy_cost[1] >> machine_energy_cost[2]
                     >> machine_energy_cost[3] >> machine_energy_cost[4];
            machine_types.emplace_back(machine_type(machine_type1));
            short j = 0;
            for (first_energy = -1; first_energy == -1; ++j) {
                if (machine_energy_cost[j] != 0) {
                    first_energy = j;
                }
            }
            for (; j < 5; ++j) {
                if (machine_energy_cost[j] != 0) {
                    second_energy = j;
                }
            }
            if (first_energy <= second_energy) {
                machine_first_energy_type.emplace_back(energy_type(first_energy));
                machine_second_energy_type.emplace_back(energy_type(second_energy));
            } else {
                machine_first_energy_type.emplace_back(energy_type(second_energy));
                machine_second_energy_type.emplace_back(energy_type(first_energy));
            }
        }
        std::cin >> num_flow_line;
        machine_first_flow_line = std::vector<short>(num_machine, -1);
        machine_first_flow_line[0] = 0;
        for (short i = 0; i < num_flow_line; ++i) {
            flow_line temp_flow_line;
            std::cin >> temp_flow_line.type >> temp_flow_line.previous_machine >> temp_flow_line.current_machine;
            temp_flow_line.is_core_line = false;
            flow_lines.push_back(temp_flow_line);
            if (machine_first_flow_line[flow_lines[i].current_machine] == -1) {
                machine_first_flow_line[flow_lines[i].current_machine] = i;
            }
        }
        std::cin >> num_core_flow_line;
        short index_core_flow_line;
        for (int i = 0; i < num_core_flow_line; ++i) {
            std::cin >> index_core_flow_line;
            flow_lines[index_core_flow_line].is_core_line = true;
        }
    }

    void solution2::OutPut() {
        std::cout << num_machine << '\n';
        for (short i = 0; i < num_machine; ++i) {
            std::cout << machine_regions[i] << ' ';
        }
        std::cout << '\n';
        std::cout << num_core_flow_line + 1 << '\n';
        std::cout << machine_windows[0] << ' ';
        for (short i = 0; i < num_flow_line; ++i) {
            if (flow_lines[i].is_core_line) {
                std::cout << machine_windows[flow_lines[i].current_machine] << ' ';
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