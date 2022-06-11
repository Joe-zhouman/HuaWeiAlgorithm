//
// Created by joe on 22-6-10.
//

#ifndef EMBEDDED_2022_LISTNODE_H
#define EMBEDDED_2022_LISTNODE_H

namespace simple_dsa {
    typedef int Rank;  //秩
    template<typename T>
    struct ListNode;
    template<typename T>
    using ListNodePosi = ListNode<T> *;

    template<typename T>
    struct ListNode {  //列表节点模板类（以双向链表形式实现）
        // 成员
        T data_;
        ListNodePosi<T> pred_;
        ListNodePosi<T> succ_;  //数值、前驱、后继
        // 构造函数
        ListNode() {}        //针对header和trailer的构造
        ListNode(T e, ListNodePosi<T> p = nullptr, ListNodePosi<T> s = nullptr)
                : data_(e), pred_(p), succ_(s) {}  //默认构造器
        // 操作接口
        ListNodePosi<T> InsertAsPred(T const &e);  //紧靠当前节点之前插入新节点
        ListNodePosi<T> InsertAsSucc(T const &e);  //紧随当前节点之后插入新节点
    };

//将e紧靠当前节点之前插入于当前节点所属列表（设有哨兵头节点header）
    template<typename T>
    ListNodePosi<T> ListNode<T>::InsertAsPred(T const &e) {
        ListNodePosi<T> x = new ListNode(e, pred_, this);  //创建新节点
        pred_->succ_ = x;
        pred_ = x;  //设置正向链接
        return x;   //返回新节点的位置
    }

//将e紧随当前节点之后插入于当前节点所属列表（设有哨兵尾节点trailer）
    template<typename T>
    ListNodePosi<T> ListNode<T>::InsertAsSucc(T const &e) {
        ListNodePosi<T> x = new ListNode(e, this, succ_);  //创建新节点
        succ_->pred_ = x;
        succ_ = x;  //设置逆向链接
        return x;   //返回新节点的位置
    }
} // simple_dsa

#endif //EMBEDDED_2022_LISTNODE_H
