//
// Created by joe on 22-6-4.
//

#ifndef EMBEDDED_2022_ORDERED_LIST_H
#define EMBEDDED_2022_ORDERED_LIST_H

#include "list"
#include "algorithm"

namespace MiuiIsTheBest {
    template<typename T>
    class ordered_list {
    public:
        std::list<T> element;

        void insert(T node) {
            if (element.empty()) {
                element.template emplace_back(node);
                return;
            }
            if (node <= element.front()) {
                element.template emplace_front(node);
            } else if (node >= element.back()) {
                element.template emplace_back(node);
            } else {
                element.template emplace(std::lower_bound(element.begin(), element.end(), node), node);
            }
        }
    };

} // MiuiIsTheBest

#endif //EMBEDDED_2022_ORDERED_LIST_H
