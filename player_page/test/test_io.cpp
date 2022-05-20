//
// Created by joe on 22-5-13.
//
#include<iostream>
#include "string"

using namespace std;

int main(int argc, char *argv[]) {
    short n = 1;
    bool a = n & 0b1;
    bool b = (n & 0b10) >> 1;
    bool c = (n & 0b100) >> 2;
    cout << a << ' ' << b << ' ' << c << '\n';
}