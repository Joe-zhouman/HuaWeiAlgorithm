//
// Created by joe on 22-5-21.
//
#include "vector"
#include "BoolType.h"
#include "enum.h"

#ifndef EMBEDDED_2022_MACHINE_H
#define EMBEDDED_2022_MACHINE_H

namespace MiuiIsTheBest {

    class Machine {
    private:
        void NewPointer() {
            allowed_windows = new std::vector<short>;
            allowed_regions = new std::vector<short>;
            parents = new std::vector<short>;
            children = new std::vector<short>;
            connect_types = new std::vector<bool>;
            children_types = new std::vector<E_TYPE>;
        }

    public:
        short current_position = 0;
        short depth = 0;
        std::vector<short> *allowed_windows;
        std::vector<short> *allowed_regions;
        std::vector<short> *parents;
        std::vector<short> *children;
        std::vector<bool> *connect_types;
        std::vector<E_TYPE> *children_types;
        V_STATUS status = V_STATUS::UNDISCOVERED;
        BoolType type;
        BoolType first_energy_type;
        BoolType second_energy_type;
        bool is_core = false;
        short first_flow_line = -1;

        short CurrentWindow() { return allowed_windows->at(current_position); }

        short CurrentRegion() { return allowed_regions->at(current_position); }

        bool HasNext() { return current_position < allowed_windows->size() - 1; }

        bool OutOfPosition() { return current_position >= allowed_windows->size(); }

        short GetChildPosition(short index_child) {
            if (children == nullptr) return -1;
            for (int i = 0; i < children->size(); ++i) {
                if (children->at(i) == index_child) return i;
            }
            return -1;
        }

        bool GetChildConnectType(short index_child) {
            return connect_types->at(GetChildPosition(index_child));
        }

        ~Machine() {
            delete allowed_windows;
            delete allowed_regions;
            delete parents;
            delete children;
            delete connect_types;
            delete children_types;
        }

        Machine(const Machine &m) {
            current_position = m.current_position;
            depth = m.depth;
            status = m.status;
            type = m.type;
            first_energy_type = m.first_energy_type;
            second_energy_type = m.second_energy_type;
            is_core = m.is_core;
            first_flow_line = m.first_flow_line;
            NewPointer();
        }

        Machine(short machine_type_number, short first_energy_type_number, short second_energy_type_number)
                : type(BoolType(machine_type_number)),
                  first_energy_type(BoolType(first_energy_type_number)),
                  second_energy_type(BoolType(second_energy_type_number)) {
            NewPointer();
        }

    };

} // MiuiIsTheBest

#endif //EMBEDDED_2022_MACHINE_H
