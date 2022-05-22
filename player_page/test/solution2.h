//
// Created by joe on 22-5-2.
//
#pragma once
#ifndef HUAWEIALGORITHM_SOLUTION_H
#define HUAWEIALGORITHM_SOLUTION_H

#include "flow_line.h"
#include "energy_type.h"
#include "window.h"
#include "machine_type.h"
#include <vector>
#include <stack>

namespace MiuiIsTheBest {

    class solution2 {
    private:
        //静态变量组
        std::vector<window> windows;//所有窗口的集合
        std::vector<std::vector<short>> factories;//所有工厂的集合
        std::vector<flow_line> flow_lines;//所有生产线的集合
        std::vector<energy_type> region_energy_types;//所有区域对应的能源类型
        std::vector<machine_type> machine_types;//所有机器的类型
        std::vector<energy_type> machine_first_energy_type;//机器的第一顺位能源
        std::vector<energy_type> machine_second_energy_type;//机器的第二顺位能源

        short num_flow_line;//流水线核心产线边数
        short num_core_flow_line;
        //其他变量
        short index_current_flow_line;
        short num_factories;
        short num_windows;
        short num_regions;
        short num_cycle_windows;
        std::stack<std::vector<short>> op_cycled_windows;//每次操作使用回环的窗口记录栈
        std::vector<short> machine_regions;//每个机器匹配到的区域
        std::vector<short> machine_windows;//每个机器匹配到的窗口
        std::vector<short> machine_first_flow_line;//每个机器第一次出现的流水线
        std::vector<short> first_match_regions;
        std::vector<short> first_match_windows;

        //私有方法
        bool MatchEnergy(short index_machine, short index_region);

        bool MatchInit(short index_machine, short index_window);

        void NextWindow(short &index_current_window, std::vector<short> &cycled_windows);

        void NextRegion(short &index_current_region, short &index_current_window, std::vector<short> &cycled_windows);

        void MatchRegion(short current_machine, short &start_region, short &start_window, bool is_core_line,
                         std::vector<short> &cycled_windows);

        void RegainCycleTimes(std::vector<short> &cycled_windows);

        inline void GetRegion(short current_machine, short &index_current_region, short &index_current_window);

        inline void SetRegion(short current_machine, short index_current_region, short index_current_window);

        inline std::vector<short> &GetFactory(short current_window);

        bool IsSuccessor(short first_window, short second_window, std::vector<short> cycled_windows);

    public:
        //结果变量
        //TODO
        short num_machine;

        bool GetSolution();

        void OutPut();

        solution2();
    };

} // MiuiIsTheBest

#endif //HUAWEIALGORITHM_SOLUTION_H
