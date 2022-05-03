//
// Created by joe on 22-5-2.
//

#ifndef HUAWEIALGORITHM_SOLUTION_H
#define HUAWEIALGORITHM_SOLUTION_H

#include "flow_line.h"
#include "energy_type.h"
#include "window.h"
#include "machine_type.h"
#include <vector>
#include <stack>

namespace MiuiIsTheBest {

    class solution {
    private:
        //静态变量组
        static std::vector <window> windows;//所有窗口的集合
        static std::vector <std::vector<char>> factories;//所有工厂的集合
        static std::vector <flow_line> flow_line;//所有生产线的集合
        static std::vector <energy_type> region_energy_types;//所有区域对应的能源类型
        static std::vector <machine_type> machine_types;//所有机器的类型
        static std::vector <energy_type> machine_first_energy_type;//机器的第一顺位能源
        static std::vector <energy_type> machine_second_energy_type;//机器的第二顺位能源

        static short num_flow_line;//流水线核心产线边数
        //其他变量
        short index_current_flow_line;
        std::stack <std::vector<char>> op_cycled_windows;//每次操作使用回环的窗口记录栈
        short []
        machine_regions;//每个机器匹配到的区域
        short []
        machine_windows;//每个机器匹配到的窗口
        std::vector<char> cycled_windows;//每次操作使用回环的窗口记录
        //私有方法
        bool MatchEnergy(short index_machine, short index_region);

        bool MatchInit(short index_machine, char index_window);

        void NextWindow(char &index_current_window);

        void NextRegion(short &index_current_region, char &index_current_window);

        void MatchRegion(short current_machine, short &start_region, char &start_window, bool is_core_line);

        void RegainCycleTimes();

    public:
        //结果变量
        //TODO
        static short num_machine;
        std::vector<short> machine_factories;

        bool GetSolution();

        bool solution(string filename);
    };

} // MiuiIsTheBest

#endif //HUAWEIALGORITHM_SOLUTION_H
