//
// Created by joe on 22-6-10.
//

#ifndef EMBEDDED_2022_QUADLIST_H
#define EMBEDDED_2022_QUADLIST_H

#include "QuadlistNode.h"

namespace simple_dsa {
    template<typename T>
    class Quadlist {
    private:
        int _size_;//规模
        QlistNodePosi<T> _header_;//头哨兵
        QlistNodePosi<T> _trailer_;//尾哨兵
    protected:
        void Init();//初始化
        int Clear();//清除所有节点
    public:
        //构造函数
        Quadlist() { Init(); }

        //析构函数
        ~Quadlist() {
            Clear();
            delete _header_;
            delete _trailer_;
        }

        int Size() const { return _size_; }//四链表规模
        bool Empty() const { return _size_ <= 0; }//判空
        QlistNodePosi<T> First() const { return _header_->succ_; }//首节点位置
        QlistNodePosi<T> Last() const { return _trailer_->pred_; }//末节点位置
        //判断位置p是否对外合法
        //p非空且p不是头、尾哨兵
        bool Valid(QlistNodePosi<T> p) { return (nullptr!=p) && (_trailer_ != p) && (_header_ != p); }

        //删除（合法）位置p处的节点，返回被删除节点的数值
        T Remove(QlistNodePosi<T> p);

        //将*e作为p的后继、b的上邻插入
        QlistNodePosi<T> InsertAfterAbove(T const &e, QlistNodePosi<T> p, QlistNodePosi<T> b = nullptr) {
            this->_size_++;
            return p->InsertAsSuccAbove(e, b);//返回新节点位置（below = NULL
        }
    };

    template<typename T>
    void Quadlist<T>::Init() {
        _header_ = new QuadlistNode<T>;//创建头哨兵节点
        _trailer_ = new QuadlistNode<T>;//创建尾哨兵节点
        _header_->succ_ = _trailer_;//沿横向联接哨兵
        _header_->pred_ = nullptr;
        _header_->above_ = nullptr;
        _header_->below_ = nullptr;

        _trailer_->pred_ = _header_;//沿横向联接哨兵
        _trailer_->succ_ = nullptr;
        _trailer_->above_ = nullptr;
        _trailer_->below_ = nullptr;
        _size_ = 0;//记录规模
        //如此构造的四联表，不含任何实质的节点，且暂时与其它四联表相互独立
    }

    template<typename T>
    int Quadlist<T>::Clear() {
        int old_size = this->_size_;
        while (!Empty())
            this->Remove(_header_->succ_);
        return old_size;
    }

    template<typename T>
    T Quadlist<T>::Remove(QlistNodePosi<T> p) {
        p->pred_->succ_=p->succ_;
        p->succ_->pred_=p->pred_;
        _size_--;
        T e = p->entry_;
        delete p;
        return e;
    }
}
#endif //EMBEDDED_2022_QUADLIST_H
