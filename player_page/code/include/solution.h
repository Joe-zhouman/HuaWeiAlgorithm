//
// Created by joe on 22-5-21.
//
#pragma once

#include "machine.h"
#include "window.h"
#include "flow_line.h"
#include <stack>

#ifndef EMBEDDED_2022_SOLUTION_H
#define EMBEDDED_2022_SOLUTION_H

namespace MiuiIsTheBest {
    class solution {
        std::vector<window> windows;//所有窗口的集合
        std::vector<std::vector<short>> factories;//所有工厂的集合
        std::vector<flow_line> flow_lines;//所有生产线的集合
        std::vector<energy_type> region_energy_types;//所有区域对应的能源类型
        std::vector<machine> machines;//所有机器的集合

        short num_flow_line = 0;
        short num_core_flow_line = 0;//流水线核心产线边数
        //其他变量
        short index_current_flow_line = 0;
        short num_machine = 0;
        short num_factories = 0;
        short num_windows = 0;
        short num_regions = 0;
        short num_cycle_windows = 0;
        std::stack<std::vector<short>> op_cycled_windows;//每次操作使用回环的窗口记录栈
        void MachineInit();

        bool IsSuccessor(short first_window, short second_window, std::vector<short> cycled_windows);

        static void
        GetEnergyOrder(const int *machine_energy_cost, short &first_energy, short &second_energy, short i, short j);

        void RegainCycleTimes(std::vector<short> &cycled_windows);

        machine *GetCurrentMachine(short index_flow_line);

        machine *GetPreviousMachine(short index_flow_line);

        short GetCurrentWindow(short index_flow_line);

        short GetPreviousWindow(short index_flow_line);

    public:
        bool GetSolution();

        void OutPut();

        solution();
    };

} // MiuiIsTheBest

#endif //EMBEDDED_2022_SOLUTION_H
