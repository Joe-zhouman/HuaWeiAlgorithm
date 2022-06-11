//
// Created by joe on 22-6-10.
//

#ifndef EMBEDDED_2022_LIST_H
#define EMBEDDED_2022_LIST_H

#include "ListNode.h"

namespace simple_dsa {

    template<typename T>
    class List {  //列表模板类

    private:
        int size_;
        ListNodePosi<T> header_;
        ListNodePosi<T> trailer_;  //规模、头哨兵、尾哨兵

    protected:
        void Init();                           //列表创建时的初始化
        int Clear();                           //清除所有节点
        void CopyNodes(ListNodePosi<T>, int);  //复制列表中自位置p起的n项
    public:
        // 构造函数
        List() { Init(); }                      //默认
        List(List<T> const &L);                 //整体复制列表L
        List(List<T> const &L, int r, int n);  //复制列表L中自第r项起的n项
        List(ListNodePosi<T> p, int n);  //复制列表中自位置p起的n项
        // 析构函数
        ~List();  //释放（包含头、尾哨兵在内的）所有节点
        // 只读访问接口
        int Size() const { return size_; }        //规模
        bool Empty() const { return size_ <= 0; }  //判空
        ListNodePosi<T> First() const { return header_->succ_; }  //首节点位置
        ListNodePosi<T> Last() const { return trailer_->pred_; }  //末节点位置
        //判断位置p是否对外合法
        bool Valid(ListNodePosi<T> p) { return p && (trailer_ != p) && (header_ != p); }//将头、尾节点等同于NULL
        // 可写访问接口
        ListNodePosi<T> InsertAsFirst(T const &e);  //将e当作首节点插入
        ListNodePosi<T> InsertAsLast(T const &e);   //将e当作末节点插入
        ListNodePosi<T> InsertA(ListNodePosi<T> p, T const &e);  //将e当作p的后继插入（After）
        ListNodePosi<T> InsertB(ListNodePosi<T> p, T const &e);  //将e当作p的前驱插入（Before）
        T Remove(ListNodePosi<T> p);  //删除合法位置p处的节点,返回被删除节点
    };                          // List
    /************************构造~析构******************************/
    template<typename T>
    List<T>::List(ListNodePosi<T> p, int n) { CopyNodes(p, n); }

    template<typename T>
    List<T>::List(List<T> const &L) { CopyNodes(L.First(), L.size_); }

    template<typename T>
    List<T>::List(List<T> const &L, int r, int n) { CopyNodes(L[r], n); }

    template<typename T>
    void List<T>::Init() {
        header_ = new ListNode<T>;
        trailer_ = new ListNode<T>;
        header_->succ_ = trailer_;
        header_->pred_ = nullptr;
        trailer_->succ_ = nullptr;
        trailer_->pred_ = header_;
        size_ = 0;
    }

//列表析构器
    template<typename T>
    List<T>::~List() {
        Clear();
        delete header_;
        delete trailer_;
    }  //清空列表，释放头、尾哨兵节点
    template<typename T>
    int List<T>::Clear() {
        const int oldSize = size_;
        while (0 < size_) {
            Remove(header_->succ_);
        }
        return oldSize;
    }

    /************************构造~析构******************************/
    template<typename T>
    void List<T>::CopyNodes(ListNodePosi<T> p, int n) {
        Init();
        while (n--) {
            InsertAsLast(p->data_);
            p = p->succ_;
        }
    }
    /**********************************插入******************************/
    template<typename T>
    ListNodePosi<T> List<T>::InsertAsFirst(T const &e) {
        size_++;
        return header_->InsertAsSucc(e);
    }  // e当作首节点插入

    template<typename T>
    ListNodePosi<T> List<T>::InsertAsLast(T const &e) {
        size_++;
        return trailer_->InsertAsPred(e);
    }  // e当作末节点插入

    template<typename T>
    ListNodePosi<T> List<T>::InsertA(ListNodePosi<T> p, T const &e) {
        size_++;
        return p->InsertAsSucc(e);
    }  // e当作p的后继插入（After）

    template<typename T>
    ListNodePosi<T> List<T>::InsertB(ListNodePosi<T> p, T const &e) {
        size_++;
        return p->InsertAsPred(e);
    }  // e当作p的前驱插入（Before）
/**********************************插入******************************/
    template<typename T>
    T List<T>::Remove(ListNodePosi<T> p) {  //删除合法节点p，返回其数值
        T e = p->data_;  //备份待删除节点的数值（假定T类型可直接赋值）
        p->pred_->succ_ = p->succ_;
        p->succ_->pred_ = p->pred_;  //后继、前驱
        delete p;
        size_--;   //释放节点，更新规模
        return e;  //返回备份的数值
    }
} // simple_dsa

#endif //EMBEDDED_2022_LIST_H
