//
// Created by joe on 22-5-21.
//
#pragma once

#include "Machine.h"
#include "Window.h"
#include "FlowLine.h"
#include <stack>
#include "queue"
#include "Start.h"
#include "list"
#ifndef EMBEDDED_2022_SOLUTION_H
#define EMBEDDED_2022_SOLUTION_H

namespace MiuiIsTheBest {
    class Solution {
        int K;//生产次数
        int manu_time[5];//能源加工时间
        std::vector<Window> windows;//所有窗口的集合
        std::vector<std::vector<int>> factories;//所有工厂的集合
        std::vector<FlowLine> flow_lines;//所有生产线的集合
        std::vector<int> region_energy_types;//所有区域对应的能源类型
        std::vector<Machine> machines;//所有机器的集合
        std::vector<Start> starts;
        std::vector<int> machine_index;
        std::list<int> core_line_machines;
        std::vector<Position> CorePosition[3];
        std::vector<Position> NonCorePosition[3];
        int num_flow_line = 0;
        int num_core_flow_line = 0;//流水线核心产线边数
        //其他变量
        int index_current_step = 0;
        int num_machine = 0;
        int num_factories = 0;
        int num_windows = 0;
        int num_regions = 0;
        int num_cycle_windows = 0;
        unsigned int total_cost[6];


        bool IsSuccessor(Machine *current_machine, int parent_index);

        bool TSort(int index_machine, std::vector<int> *S);

        void Reset() {
            for (Machine &machine: machines) {
                machine.status = V_STATUS::UNDISCOVERED;
            }
        }

    public:
        bool GetSolution();

        void MachinePositionInitWithoutSort();

        bool TSortBfs();

        bool MachineGraphInit();

        void OutPut();

        Solution();
    };

} // MiuiIsTheBest

#endif //EMBEDDED_2022_SOLUTION_H
