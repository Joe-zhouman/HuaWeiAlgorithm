//
// Created by joe on 22-5-21.
//
#pragma once

#include "Machine.h"
#include "Window.h"
#include "FlowLine.h"
#include <stack>
#include "queue"

#ifndef EMBEDDED_2022_SOLUTION_H
#define EMBEDDED_2022_SOLUTION_H

namespace MiuiIsTheBest {
    class Solution {
        int K;//生产次数
        short manu_time[5];//能源加工时间
        std::vector<Window> windows;//所有窗口的集合
        std::vector<std::vector<short>> factories;//所有工厂的集合
        std::vector<FlowLine> flow_lines;//所有生产线的集合
        std::vector<BoolType> region_energy_types;//所有区域对应的能源类型
        std::vector<Machine> machines;//所有机器的集合

        short num_flow_line = 0;
        short num_core_flow_line = 0;//流水线核心产线边数
        //其他变量
        short index_current_step = 0;
        short num_machine = 0;
        short num_factories = 0;
        short num_windows = 0;
        short num_regions = 0;
        short num_cycle_windows = 0;
        std::stack<std::vector<short>> op_cycled_windows;//每次操作使用回环的窗口记录栈
        void MachinePositionInit();

        void MachineGraphInit();

        bool IsSuccessor(short current_window, short previous_window, bool flow_line_type);

        void RegainCycleTimes();

        Machine *GetCurrentMachine(short index_flow_line);

        Machine *GetPreviousMachine(short index_flow_line);

        short GetCurrentWindow(short index_flow_line);

        short GetPreviousWindow(short index_flow_line);

        bool TSort(short index_current_machine, short &clock, std::vector<short> *S);
    public:
        bool GetSolution();

        void OutPut();

        Solution();
    };

} // MiuiIsTheBest

#endif //EMBEDDED_2022_SOLUTION_H
