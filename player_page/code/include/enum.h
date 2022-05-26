//
// Created by joe on 22-5-25.
//
#pragma once
#ifndef EMBEDDED_2022_ENUM_H
#define EMBEDDED_2022_ENUM_H
namespace MiuiIsTheBest {
    enum class V_STATUS {
        UNDISCOVERED, DISCOVERED, VISITED
    };//顶点状态
    enum class E_TYPE {
        UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD
    };//边在遍历树中所属的类型
    //using V_STATUS = enum {  };
    //using E_TYPE = enum { };
}
#endif //EMBEDDED_2022_ENUM_H
