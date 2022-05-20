//
// Created by joe on 22-5-2.
//
#pragma once
#ifndef HUAWEIALGORITHM_MACHINE_TYPE_H
#define HUAWEIALGORITHM_MACHINE_TYPE_H
namespace MiuiIsTheBest {
    struct machine_type {
        //范围[0,2]用两个bool维护
        bool first_bit;
        bool second_bit;

        bool operator==(const machine_type &p) const {
            return (first_bit == p.first_bit && second_bit == p.second_bit);
        }

        int to_int() {
            return static_cast<int>(first_bit) + (static_cast<int>(second_bit) << 1);
        }

        explicit machine_type(short n) {
            first_bit = n & 0b1;//00^01=00;01^01=01;10^01=00;11^01=01;
            second_bit = (n & 0b10) >> 1;//00^10=00;01^10=00;10^10=11;11^10=10;
        }
    };
}
#endif //HUAWEIALGORITHM_MACHINE_TYPE_H
