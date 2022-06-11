//
// Created by joe on 22-6-10.
//

#ifndef EMBEDDED_2022_QUADLISTNODE_H
#define EMBEDDED_2022_QUADLISTNODE_H
namespace simple_dsa {
    template<typename T>
    struct QuadlistNode;
    template<typename T>
    using QlistNodePosi = QuadlistNode<T> *;

    template<typename T>
    struct QuadlistNode {//四链表节点模板类
        T entry_;//所存词条
        QlistNodePosi<T> pred_;//前驱
        QlistNodePosi<T> succ_;//后继
        QlistNodePosi<T> above_;//上邻
        QlistNodePosi<T> below_;//下邻

        //构造器
        QuadlistNode(T e = T(), QlistNodePosi<T> p = nullptr, QlistNodePosi<T> s = nullptr,
                     QlistNodePosi<T> a = nullptr, QlistNodePosi<T> b = nullptr) :
                entry_(e), pred_(p), succ_(s), above_(a), below_(b) {}

        //插入新节点，以当前节点为前驱，以节点b为下邻
        QlistNodePosi<T> InsertAsSuccAbove(T const &e, QlistNodePosi<T> b = nullptr);
    };

    //将e作为当前节点的后继、b的上邻插入Quadlist
    template<typename T>
    QlistNodePosi<T>
    QuadlistNode<T>::InsertAsSuccAbove(const T &e, QlistNodePosi<T> b) {
        QlistNodePosi<T> x = new QuadlistNode<T>(e, this, this->succ_, nullptr, b);//创建新节点
        this->succ_->pred_ = x;//设置水平逆向链接
        this->succ_ = x;//设置水平逆向链接
        if (b)b->above_ = x;//设置垂直逆向链接
        return x;//返回新节点的位置
    }

} // simple_dsa
#endif //EMBEDDED_2022_QUADLISTNODE_H
