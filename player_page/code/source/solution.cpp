//
// Created by joe on 22-5-21.
//
#include
"iostream"
#include
"solution.h"

//#define DEBUG
#define
FULL_OUTPUT
#define
MAX_ITER_TIMES 100000000
namespace MiuiIsTheBest {
void solution::RegainCycleTimes(const std::vector<short>* cycled_windows) {
for (short cycled_window: *cycled_windows) {
windows[cycled_window].rest_cycle_times++;
}
}

bool solution::IsSuccessor(short current_window, short previous_window, bool flow_line_type) {
bool result= false;
std::vector<short> cycled_windows;
if (previous_window < current_window) result =  true;
if (previous_window > current_window && previous_window < num_cycle_windows &&
windows[num_cycle_windows - 1].rest_cycle_times > 0) {
windows[num_cycle_windows - 1].rest_cycle_times--;
cycled_windows.emplace_back(num_cycle_windows - 1);
result = true;
}
if (previous_window==current_window&&flow_line_type) result = true;
if (previous_window == current_window&&!flow_line_type && windows[current_window].rest_cycle_times > 0) {
windows[current_window].rest_cycle_times--;
cycled_windows.emplace_back(current_window);
result = true;
}
if (result){
op_cycled_windows.push(cycled_windows);
return true;
}
return false;
}

machine *solution::GetCurrentMachine(short index_flow_line) {
return &machines[flow_lines[index_flow_line].current_machine];
}

machine *solution::GetPreviousMachine(short index_flow_line) {
return &machines[flow_lines[index_flow_line].previous_machine];
}

short solution::GetCurrentWindow(short index_flow_line) {
return GetCurrentMachine(index_flow_line)->CurrentWindow();
}

short solution::GetPreviousWindow(short index_flow_line) {
return GetPreviousMachine(index_flow_line)->CurrentWindow();
}

bool solution::GetSolution() {

short total_iter_times = 0;
while (index_current_flow_line < num_flow_line && index_current_flow_line >= 0) {
#ifdef
DEBUG
std::cout<<"find region for flow line "<<index_current_flow_line
<<" and machine "<<flow_lines[index_current_flow_line].current_machine<<std::endl;
#endif
machine *current_machine = GetCurrentMachine(index_current_flow_line);
short index_current_windows = GetCurrentWindow(index_current_flow_line);
short index_previous_windows = GetPreviousWindow(index_current_flow_line);
bool successor=IsSuccessor(index_current_windows, index_previous_windows, flow_lines[index_current_flow_line].type);
if (current_machine->first_flow_line==index_current_flow_line){
while (current_machine->HasNext()&&!successor) {
current_machine->current_position++;
index_current_windows = current_machine->CurrentWindow();
#ifdef
DEBUG
std::cout<<"match "<<current_machine->current_position<<"/"<<
current_machine->allowed_windows.size()-1<<" region for  machine "
<<flow_lines[index_current_flow_line].current_machine<<std::endl;
#endif
successor=IsSuccessor(index_current_windows, index_previous_windows, flow_lines[index_current_flow_line].type);
}
}
if (successor) {
#ifdef
DEBUG
std::cout<<"match region "<<current_machine->CurrentRegion()
<<" and windows "<<current_machine->CurrentWindow()<<"  for  machine "
<<flow_lines[index_current_flow_line].current_machine<<std::endl;
#endif
index_current_flow_line++;
continue;
}
machine* back_tracking_machine;
short first_occur_flow_line;
if (current_machine->first_flow_line!=index_current_flow_line) {
back_tracking_machine = current_machine;
first_occur_flow_line = back_tracking_machine->first_flow_line;
while (index_current_flow_line > first_occur_flow_line) {
index_current_flow_line--;
current_machine = GetCurrentMachine(index_current_flow_line);
if (current_machine != back_tracking_machine) {
if (current_machine->current_position>0)
current_machine->current_position--;
if (op_cycled_windows.empty()) {
#ifdef
DEBUG
std::cout<<"op_cycled_windows is empty"<<std::endl;
#endif
return false;
}
RegainCycleTimes(&op_cycled_windows.top());
op_cycled_windows.pop();
}
}
}
current_machine->current_position++;
while (current_machine->OutOfPosition()){
back_tracking_machine = GetPreviousMachine(index_current_flow_line);
first_occur_flow_line = back_tracking_machine->first_flow_line;
while (index_current_flow_line > first_occur_flow_line) {
current_machine = GetCurrentMachine(index_current_flow_line);
if (current_machine != back_tracking_machine) {
if (current_machine->current_position>0){
current_machine->current_position--;
current_machine->current_position--;
}
if (op_cycled_windows.empty()) {
#ifdef
DEBUG
std::cout<<"op_cycled_windows is empty";
#endif
return false;
}
RegainCycleTimes(&op_cycled_windows.top());
op_cycled_windows.pop();
}
index_current_flow_line--;
}
current_machine->current_position++;
}
//            if(total_iter_times>MAX_ITER_TIMES) return false;
}
return index_current_flow_line >= 0;
}

solution::solution() {

std::cin.ignore(100, '\n');
std::cin.ignore(100, '\n');
std::cin >> num_factories;
for (int i = 0; i < num_factories; ++i) {
            std::vector<short> temp_factory;
            factories.push_back(temp_factory);
        }
        std::cin >> num_regions;
        short factory, energy;
        for (short i = 0; i < num_regions; ++i) {
            std::cin >> factory >> energy;
            factories[factory].push_back(i);
            region_energy_types.emplace_back(energy_type(energy));
        }
        short max_cycle_times;
        std::cin >> max_cycle_times;
        max_cycle_times++;
        std::cin >> num_cycle_windows;
        std::cin >> num_windows;
        bool self_cycle;
        short factory_index;
        short window_cost[num_windows];
        for (short i = 0; i < num_windows; ++i) {
            window temp_window;
            std::cin >> self_cycle >> factory_index >> window_cost[i] >> temp_window.init_type[0]
                     >> temp_window.init_type[1] >> temp_window.init_type[2];

            temp_window.factory = factory_index;
            if (i < num_cycle_windows - 1) {
                temp_window.cnext = -1;
            } else if (i < num_cycle_windows) {
                temp_window.cnext = 0;
                temp_window.rest_cycle_times = max_cycle_times;
            } else {
                temp_window.cnext = self_cycle ? i : -1;
                temp_window.rest_cycle_times = self_cycle ? max_cycle_times : 0;
            }
            windows.push_back(temp_window);
        }
        for (short i = 0; i < num_windows - 1; ++i) {
            if (windows[i].cnext == -1) {
                windows[i].first_order_next = false;
            } else {
                if (window_cost[i + 1] >= window_cost[windows[i].cnext]) {
                    windows[i].first_order_next = true;
                } else {
                    windows[i].first_order_next = false;
                }
            }
        }
        windows[num_windows - 1].first_order_next = true;
        std::cin >> num_machine;
        short machine_type1;
        int machine_energy_cost[5];
        short first_energy;
        short second_energy;
        for (short i = 0; i < num_machine; ++i) {
            std::cin >> machine_type1 >> machine_energy_cost[0] >> machine_energy_cost[1] >> machine_energy_cost[2]
                     >> machine_energy_cost[3] >> machine_energy_cost[4];
            if (machine_type1 == 0) {
                GetEnergyOrder(machine_energy_cost, first_energy, second_energy, 0, 1);
            } else if (machine_type1 == 1) {
                GetEnergyOrder(machine_energy_cost, first_energy, second_energy, 0, 2);
            } else {
                GetEnergyOrder(machine_energy_cost, first_energy, second_energy, 3, 4);
            }
            machines.emplace_back(machine(machine_type1, first_energy, second_energy));
        }
        std::cin >> num_flow_line;

        for (short i = 0; i < num_flow_line; ++i) {
            flow_line temp_flow_line;
            std::cin >> temp_flow_line.type >> temp_flow_line.previous_machine >> temp_flow_line.current_machine;
            flow_lines.push_back(temp_flow_line);
            if (machines[flow_lines[i].current_machine].first_flow_line == -1) {
                machines[flow_lines[i].current_machine].first_flow_line = i;
            }
        }
        std::cin >> num_core_flow_line;
        short index_core_flow_line;
        std::cin >> index_core_flow_line;
        flow_lines[index_core_flow_line].is_core = true;
        machines[flow_lines[index_core_flow_line].current_machine].is_core = true;
        machines[flow_lines[index_core_flow_line].previous_machine].is_core = true;

        for (short i = 1; i < num_core_flow_line; ++i) {
            std::cin >> index_core_flow_line;
            flow_lines[index_core_flow_line].is_core = true;
            machines[flow_lines[index_core_flow_line].current_machine].is_core = true;
        }
        MachineInit();
    }

    void solution::GetEnergyOrder(const int *machine_energy_cost, short &first_energy, short &second_energy, short i,
                                  short j) {
        if (machine_energy_cost[i] <= machine_energy_cost[j]) {
            first_energy = i;
            second_energy = j;
        } else {
            first_energy = j;
second_energy = i;
}
}

void solution::MachineInit() {
for (machine &current_machine: machines) {
for (short index_window = 0; index_window < num_windows; index_window++) {
                window *current_window = &windows[index_window];
                if (!current_machine.is_core ||
                    (current_machine.is_core && current_window->init_type[current_machine.type.to_int()])) {
                    for (short index_region = factories[current_window->factory].front();
                         index_region <= factories[current_window->factory].back(); index_region++) {
                        if (region_energy_types[index_region] == current_machine.first_energy_type) {
                            current_machine.allowed_windows.emplace_back(index_window);
                            current_machine.allowed_regions.emplace_back(index_region);
                        }
                        if (region_energy_types[index_region] == current_machine.second_energy_type) {
                            current_machine.allowed_windows.emplace_back(index_window);
                            current_machine.allowed_regions.emplace_back(index_region);
}
}
}
}
}
}

void solution::OutPut() {
std::cout << num_machine << '\n';
for (machine current_machine: machines) {
std::cout << current_machine.CurrentRegion() << ' ';
}
std::cout << '\n';
std::cout << num_core_flow_line + 1 << '\n';
short i = 0;
for (; i < num_flow_line; ++i) {
if (flow_lines[i].is_core) {
machine *previous_machine = GetPreviousMachine(i);
machine *current_machine = GetCurrentMachine(i);
std::cout << previous_machine->CurrentWindow() << ' '
<< current_machine->CurrentWindow() << ' ';
break;
}
}
for (++i; i < num_flow_line; ++i) {
if (flow_lines[i].is_core) {
machine *current_machine = GetCurrentMachine(i);
std::cout << current_machine->CurrentWindow() << ' ';
}
}
std::cout << '\n';
#ifdef
FULL_OUTPUT
for (machine current_machine:machines) {
std::cout << current_machine.CurrentWindow() << ' ';
}
std::cout << '\n';
#endif
    }
} // MiuiIsTheBest