//
// Created by joe on 22-5-21.
//
#include "vector"
#include "machine_type.h"
#include "energy_type.h"

#ifndef EMBEDDED_2022_MACHINE_H
#define EMBEDDED_2022_MACHINE_H

namespace MiuiIsTheBest {

    class machine {
    public:
        short current_position = -1;
        std::vector<short> allowed_windows;
        std::vector<short> allowed_regions;
        machine_type type;
        energy_type first_energy_type;
        energy_type second_energy_type;
        bool is_core = false;
        short first_flow_line = -1;

        short CurrentWindow() { return allowed_windows[current_position]; }

        short CurrentRegion() { return allowed_regions[current_position]; }

        bool HasNext() { return current_position < allowed_windows.size() - 1; }

        machine(short machine_type_number, short first_energy_type_number, short second_energy_type_number)
                : type(machine_type(machine_type_number)),
                  first_energy_type(energy_type(first_energy_type_number)),
                  second_energy_type(energy_type(second_energy_type_number)) {}
    };

} // MiuiIsTheBest

#endif //EMBEDDED_2022_MACHINE_H
