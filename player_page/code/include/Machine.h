//
// Created by joe on 22-5-21.
//
#include "vector"
#include "BoolType.h"
#include "enum.h"
#include "Position.h"
#include "Window.h"
#ifndef EMBEDDED_2022_MACHINE_H
#define EMBEDDED_2022_MACHINE_H

namespace MiuiIsTheBest {

    class Machine {
    public:
        short current_position = 0;//当前所在的许用位置
        short depth = 0;
        unsigned int cost[5];//使用不同能源的费用
        std::vector<Position> *positions;//所有的容许位置
        std::vector<short> *parents;//父节点
        std::vector<short> *children;//子节点
        std::vector<bool> *connect_types;//同父节点的连接状态
        std::vector<short> *cycled_windows;//从每个父节点过来所用的回环窗口
        V_STATUS status = V_STATUS::UNDISCOVERED;//节点状态，拓扑排序时使用
        BoolType type;//仪器类型
        bool is_core = false;//是否位于核心生产线
        short NumParents() { return static_cast<short>(parents->size()); }

        void RegainCycleTimes(std::vector<Window> &windows) {
            for (short &index: *cycled_windows) {
                if (index != -1) {
                    windows.at(index).rest_cycle_times++;
                    index = -1;
                }
            }
        }

        short CurrentWindow() { return positions->at(current_position).window; }

        short CurrentRegion() { return positions->at(current_position).region; }

        bool OutOfPosition() { return current_position >= positions->size(); }//是否超出所在位置

        ~Machine() {
            delete positions;
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
            for (int i = 0; i < 5; ++i) {
                cost[i] = m.cost[i];
            }

            positions = new std::vector<Position>;
            parents = new std::vector<short>;
            children = new std::vector<short>;
            connect_types = new std::vector<bool>;
            cycled_windows = new std::vector<short>;
        }

        explicit Machine(short machine_type_number)
                : type(BoolType(machine_type_number)) {

            positions = new std::vector<Position>;
            parents = new std::vector<short>;
            children = new std::vector<short>;
            connect_types = new std::vector<bool>;
            cycled_windows = new std::vector<short>;
            for (unsigned int &i: cost) {
                i = 0;
            }
        }
        Machine() {
            positions = new std::vector<Position>;
            parents = new std::vector<short>;
            children = new std::vector<short>;
            connect_types = new std::vector<bool>;
            cycled_windows = new std::vector<short>;
            for (int i = 0; i < 5; ++i) {
                cost[i] = 0;
            }
        }
    };

} // MiuiIsTheBest

#endif //EMBEDDED_2022_MACHINE_H
