//
// Created by joe on 22-5-21.
//
#include "vector"
#include "BoolType.h"
#include "enum.h"
#include "Position.h"

#ifndef EMBEDDED_2022_MACHINE_H
#define EMBEDDED_2022_MACHINE_H

namespace MiuiIsTheBest {

    class Machine {
    private:
        void NewPointer() {

            positions = new std::vector<Position>;
            parents = new std::vector<short>;
            children = new std::vector<short>;
            connect_types = new std::vector<bool>;
        }

    public:
        short current_position = 0;
        short depth = 0;
        unsigned int cost[5];
        std::vector<Position> *positions;
        std::vector<short> *parents;
        std::vector<short> *children;
        std::vector<bool> *connect_types;
        V_STATUS status = V_STATUS::UNDISCOVERED;
        BoolType type;
        bool is_core = false;

        short CurrentWindow() { return positions->at(current_position).window; }

        short CurrentRegion() { return positions->at(current_position).region; }

        bool OutOfPosition() { return current_position >= positions->size(); }

        short GetChildPosition(short index_child) {
            if (children == nullptr) return -1;
            for (short i = 0; i < children->size(); ++i) {
                if (children->at(i) == index_child) return i;
            }
            return -1;
        }

        bool GetChildConnectType(short index_child) {
            return connect_types->at(GetChildPosition(index_child));
        }

        ~Machine() {
            delete positions;
            delete parents;
            delete children;
            delete connect_types;
        }

        Machine(const Machine &m) {
            current_position = m.current_position;
            depth = m.depth;
            status = m.status;
            type = m.type;
            is_core = m.is_core;
            for (int i = 0; i < 5; ++i) {
                cost[i] = m.cost[i];
            }
            NewPointer();
        }

        explicit Machine(short machine_type_number)
                : type(BoolType(machine_type_number)) {
            NewPointer();
            for (int i = 0; i < 5; ++i) {
                cost[i] = 0;
            }
        }

        Machine() {
            NewPointer();
            for (int i = 0; i < 5; ++i) {
                cost[i] = 0;
            }
        }
    };

} // MiuiIsTheBest

#endif //EMBEDDED_2022_MACHINE_H
