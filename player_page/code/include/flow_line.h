//
// Created by joe on 22-5-2.
//
#pragma once
#ifndef HUAWEIALGORITHM_FLOW_LINE_H
#define HUAWEIALGORITHM_FLOW_LINE_H

namespace MiuiIsTheBest {

    struct flow_line {
        bool type;//0,1
        short previous_machine;
        short current_machine;
        bool is_core;
    };

} // MiuiIsTheBest

#endif //HUAWEIALGORITHM_FLOW_LINE_H
