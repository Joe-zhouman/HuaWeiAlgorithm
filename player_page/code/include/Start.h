//
// Created by joe on 22-5-27.
//

#ifndef EMBEDDED_2022_START_H
#define EMBEDDED_2022_START_H


class Start {
public:
    int index;
    int depth;

    bool operator<(const Start &s) const {
        return depth < s.depth;
    }

    explicit Start(int index = 0, int depth = 0) :
            index(index), depth(depth) {}
};


#endif //EMBEDDED_2022_START_H
