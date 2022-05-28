//
// Created by joe on 22-5-2.
//
#pragma once
#ifndef HUAWEIALGORITHM_WINDOW_H
#define HUAWEIALGORITHM_WINDOW_H

namespace MiuiIsTheBest {

    struct Window {
        int factory;//窗口的车间
        int rest_cycle_times;//剩余的环回次数
        int cost_coeff;//窗口费用系数
        bool init_type[3];//预处理支持类型
    };
} // MiuiIsTheBest

#endif //HUAWEIALGORITHM_WINDOW_H
