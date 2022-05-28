//
// Created by joe on 22-5-21.
//
#include "vector"
#include "BoolType.h"
#include "enum.h"
#include "Position.h"
#include "Window.h"
#include "Start.h"
#ifndef EMBEDDED_2022_MACHINE_H
#define EMBEDDED_2022_MACHINE_H

namespace MiuiIsTheBest {

    class Machine {
    public:
        int current_position = 0;//当前所在的许用位置
        int depth = 0;
        unsigned int cost[5];//使用不同能源的费用
        std::vector<Position> positions;//所有的容许位置
        std::vector<int> *parents;//父节点
        std::vector<Start> *children;//子节点
        std::vector<bool> *connect_types;//同父节点的连接状态
        std::vector<int> *cycled_windows;//从每个父节点过来所用的回环窗口
        V_STATUS status = V_STATUS::UNDISCOVERED;//节点状态，拓扑排序时使用
        int type = 0;//仪器类型
        bool is_core = false;//是否位于核心生产线
        int NumParents() { return static_cast<int>(parents->size()); }

        void RegainCycleTimes(std::vector<Window> &windows) {
            for (int &index: *cycled_windows) {
                if (index != -1) {
                    windows.at(index).rest_cycle_times++;
                    index = -1;
                }
            }
        }

        int CurrentWindow() { return positions.at(current_position).window; }

        int CurrentRegion() { return positions.at(current_position).region; }

        bool OutOfPosition() { return current_position >= positions.size(); }//是否超出所在位置

        ~Machine() {
            delete parents;
            delete children;
            delete connect_types;
            delete cycled_windows;
        }

        Machine(const Machine &m) {
            depth = m.depth;
            current_position = m.current_position;
            status = m.status;
            type = m.type;
            is_core = m.is_core;
            positions = m.positions;
            for (int i = 0; i < 5; ++i) {
                cost[i] = m.cost[i];
            }
            parents = new std::vector<int>;
            children = new std::vector<Start>;
            connect_types = new std::vector<bool>;
            cycled_windows = new std::vector<int>;
        }

        explicit Machine(int machine_type_number)
                : type(machine_type_number) {
            parents = new std::vector<int>;
            children = new std::vector<Start>;
            connect_types = new std::vector<bool>;
            cycled_windows = new std::vector<int>;
            for (unsigned int &i: cost) {
                i = 0;
            }
        }
//        Machine() {
//            positions = new std::vector<Position>;
//            parents = new std::vector<int>;
//            children = new std::vector<Start>;
//            connect_types = new std::vector<bool>;
//            cycled_windows = new std::vector<int>;
//            for (int i = 0; i < 5; ++i) {
//                cost[i] = 0;
//            }
//        }
    };

} // MiuiIsTheBest

#endif //EMBEDDED_2022_MACHINE_H
