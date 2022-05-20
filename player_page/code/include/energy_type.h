//
// Created by joe on 22-5-2.
//
#pragma once
#ifndef HUAWEIALGORITHM_ENERGY_TYPE_H
#define HUAWEIALGORITHM_ENERGY_TYPE_H
namespace MiuiIsTheBest {
    struct energy_type {
        //范围[0,4]用三个bool维护
        bool first_bit;
        bool second_bit;
        bool third_bit;

        bool operator==(const energy_type &p) const {
            return (first_bit == p.first_bit && second_bit == p.second_bit && third_bit == p.third_bit);
        }

        explicit energy_type(short n) {
            first_bit = n & 0b1;//00^01=00;01^01=01;10^01=00;11^01=01;
            second_bit = (n & 0b10) >> 1;//00^10=00;01^10=00;10^10=11;11^10=10;
            third_bit = (n & 0b100) >> 2;
        }
    };
}
#endif //HUAWEIALGORITHM_ENERGY_TYPE_H
