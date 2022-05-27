//
// Created by joe on 22-5-26.
//

#ifndef EMBEDDED_2022_POSITION_H
#define EMBEDDED_2022_POSITION_H
namespace MiuiIsTheBest {
    struct Position {
        short window;
        short region;
        unsigned int cost;
        bool operator<(const Position &p) const {
            return cost < p.cost;
        }
        bool operator==(const Position &p) const {
            return cost == p.cost;
        }
        bool operator>(const Position &p) const {
            return cost > p.cost;
        }
        Position(short window, short region, unsigned int cost) :
                window(window), region(region), cost(cost) {}
    };
}
#endif //EMBEDDED_2022_POSITION_H