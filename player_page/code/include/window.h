//
// Created by joe on 22-5-2.
//

#ifndef HUAWEIALGORITHM_WINDOW_H
#define HUAWEIALGORITHM_WINDOW_H

namespace MiuiIsTheBest {

    struct window {
        char factory;//窗口的车间
        char cnext;//环路上的下一个窗口,-1表示不在环路上
        bool first_order_next;//下一个窗口优先级
        char rest_cycle_times;//剩余的环回次数
        bool[3] init_type;//预处理支持类型
    };

} // MiuiIsTheBest

#endif //HUAWEIALGORITHM_WINDOW_H
