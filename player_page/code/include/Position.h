//
// Created by joe on 22-5-26.
//

#ifndef EMBEDDED_2022_POSITION_H
#define EMBEDDED_2022_POSITION_H
namespace MiuiIsTheBest {
    struct Position {
        int window;
        int region;
        unsigned int cost;

        bool operator<(const Position &p) const {
            return cost < p.cost;
        }
        bool operator<=(const Position &p) const {
            return cost <= p.cost;
        }
        bool operator>(const Position &p) const {
            return cost > p.cost;
        }
        bool operator>=(const Position &p) const {
            return cost >= p.cost;
        }
        bool operator==(const Position &p) const {
            return cost == p.cost;
        }
        bool operator!=(const Position &p) const {
            return cost != p.cost;
        }
        Position(){
            window=0;
            region=0;
            cost=0;
        }
        Position(const Position &p){
            window=p.window;
            region=p.region;
            cost=p.cost;
        }
        Position(int window, int region, unsigned int cost) :
                window(window), region(region), cost(cost) {}

    };
}
#endif //EMBEDDED_2022_POSITION_H
