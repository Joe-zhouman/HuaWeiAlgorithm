//
// Created by joe on 22-5-2.
//
#pragma once
#ifndef HUAWEIALGORITHM_WINDOW_H
#define HUAWEIALGORITHM_WINDOW_H

namespace MiuiIsTheBest {

    struct window {
        short factory;//窗口的车间
        short cnext;//环路上的下一个窗口,-1表示不在环路上
        bool first_order_next;//下一个窗口优先级
        short rest_cycle_times;//剩余的环回次数
        bool init_type[3];//预处理支持类型
    };


} // MiuiIsTheBest

#endif //HUAWEIALGORITHM_WINDOW_H
