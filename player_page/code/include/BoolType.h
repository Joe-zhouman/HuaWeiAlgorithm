//
// Created by joe on 22-5-2.
//
#pragma once
#ifndef HUAWEIALGORITHM_MACHINE_TYPE_H
#define HUAWEIALGORITHM_MACHINE_TYPE_H
namespace MiuiIsTheBest {
    struct BoolType {
        //范围[0,4]用三个bool维护
        bool first_bit;
        bool second_bit;
        bool third_bit;

        bool operator==(const BoolType &p) const {
            return (first_bit == p.first_bit && second_bit == p.second_bit && third_bit == p.third_bit);
        }

        int ToInt() {
            return first_bit * 1 + second_bit * 2 + third_bit * 4;
        }

        BoolType() {
            first_bit = false;
            second_bit = false;
            third_bit = false;
        }

        explicit BoolType(short n) {
            first_bit = n & 0b1;
            second_bit = (n & 0b10) >> 1;
            third_bit = (n & 0b100) >> 2;
        }
    };
}
#endif //HUAWEIALGORITHM_MACHINE_TYPE_H
