//
// Created by joe on 22-6-10.
//

#ifndef EMBEDDED_2022_LIST_H
#define EMBEDDED_2022_LIST_H

#include "ListNode.h"

namespace simple_dsa {

    enum class SortMethod {
        INSERTION, MERGE, SELECTION, DEFAULT = INSERTION
    };

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
        void Merge(ListNodePosi<T> &, int, List<T> &, ListNodePosi<T>, int);  //归并
        void MergeSort(ListNodePosi<T> &, int);  //对从p开始连续的n个节点归并排序
        void SelectionSort(ListNodePosi<T>, int);  //对从p开始连续的n个节点选择排序
        void InsertionSort(ListNodePosi<T>, int);  //对从p开始连续的n个节点插入排序

    public:
        // 构造函数
        List() { Init(); }                      //默认
        List(List<T> const &L);                 //整体复制列表L
        List(List<T> const &L, Rank r, int n);  //复制列表L中自第r项起的n项
        List(ListNodePosi<T> p, int n);  //复制列表中自位置p起的n项
        // 析构函数
        ~List();  //释放（包含头、尾哨兵在内的）所有节点
        // 只读访问接口
        Rank Size() const { return size_; }        //规模
        bool Empty() const { return size_ <= 0; }  //判空
        T &operator[](Rank r) const;  //重载，支持循秩访问（效率低）
        ListNodePosi<T> First() const { return header_->succ_; }  //首节点位置
        ListNodePosi<T> Last() const { return trailer_->pred_; }  //末节点位置
        //判断位置p是否对外合法
        bool Valid(ListNodePosi<T> p) { return p && (trailer_ != p) && (header_ != p); }//将头、尾节点等同于NULL

        //无序列表查找
        ListNodePosi<T> Find(T const &e) const { return Find(e, size_, trailer_); }

//无序区间查找
        ListNodePosi<T> Find(T const &e, int n, ListNodePosi<T> p) const;

        //有序列表查找
        ListNodePosi<T> Search(T const &e) const { return Search(e, size_, trailer_); }

        ListNodePosi<T> Search(T const &e, int n, ListNodePosi<T> p) const;  //有序区间查找
        ListNodePosi<T> SelectMax(ListNodePosi<T> p, int n);  //在p及其n-1个后继中选出最大者
        ListNodePosi<T> SelectMax() { return SelectMax(header_->succ_, size_); }  //整体最大者
        // 可写访问接口
        ListNodePosi<T> InsertAsFirst(T const &e);  //将e当作首节点插入
        ListNodePosi<T> InsertAsLast(T const &e);   //将e当作末节点插入
        ListNodePosi<T> InsertA(ListNodePosi<T> p, T const &e);  //将e当作p的后继插入（After）
        ListNodePosi<T> InsertB(ListNodePosi<T> p, T const &e);  //将e当作p的前驱插入（Before）
        T Remove(ListNodePosi<T> p);  //删除合法位置p处的节点,返回被删除节点
        void Merge(List<T> &L) { Merge(First(), Size(), L, L.First(), L.size_); }  //全列表归并
        void Sort(ListNodePosi<T> p, int n, SortMethod method);  //列表区间排序
        void Sort(SortMethod method) { Sort(First(), size_, method); }  //列表整体排序
        void Sort(ListNodePosi<T> p, int n) { Sort(p, n, SortMethod::DEFAULT); }   //列表区间排序
        void Sort() { Sort(SortMethod::DEFAULT); }  //列表整体排序
        int Deduplicate();                          //无序去重
        int Uniquify();                             //有序去重
        void Reverse();                             //前后倒置（习题）
        // 遍历
        void Traverse(void (*)(T &));  //遍历，依次实施visit操作（函数指针，只读或局部性修改）
        //操作器
        template<typename Visit>
        void Traverse(Visit &);  //遍历，依次实施visit操作（函数对象，可全局性修改）
    };                          // List
    /************************构造~析构******************************/
    template<typename T>
    List<T>::List(ListNodePosi<T> p, int n) { CopyNodes(p, n); }

    template<typename T>
    List<T>::List(List<T> const &L) { CopyNodes(L.First(), L.size_); }

    template<typename T>
    List<T>::List(List<T> const &L, Rank r, int n) { CopyNodes(L[r], n); }

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
    T &List<T>::operator[](Rank r) const {  // assert: 0 <= r < size
        ListNodePosi<T> p = First();        //从首节点出发
        while (0 < r--) p = p->succ_;       //顺数第r个节点即是
        return p->data_;  //目标节点，返回其中所存元素
    }

    template<typename T>
    void List<T>::CopyNodes(ListNodePosi<T> p, int n) {
        Init();
        while (n--) {
            InsertAsLast(p->data_);
            p = p->succ_;
        }
    }

    template<typename T>
    int List<T>::Deduplicate() {
        if (size_ < 2) {
            return 0;
        }
        const int oldSize = size_;
        ListNodePosi<T> p = First();
        for (Rank r = 0; p != trailer_; p = p->succ_)  // O(n)
            if (ListNodePosi<T> q = Find(p->data_, r, p))
                Remove(q);  //此时q与p雷同，但删除前者更为简明
            else
                r++;             // r为无重前缀的长度
        return oldSize - size_;  //删除元素总数
    }

    template<typename T>
    int List<T>::Uniquify() {
        if (this->size_ < 2)return 0;
        const int oldSize = this->size_;
        ListNodePosi<T> p = this->First();
        ListNodePosi<T> q;
        while (this->trailer_ != (q = p->succ_)) {
            if (p->data_ != q->data_)p = q;
            else this->Remove(q);
        }
        return oldSize - this->size_;
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

    template<typename T>
    //在无序列表内节点p的n个前驱中，找到等于e的最后者
    ListNodePosi<T> List<T>::Find(T const &e, int n, ListNodePosi<T> p) const {
        while (0 < n--)
            //（0 <= n <= rank(p) < _size）对于p的最近的n个前驱，从右向左
            if (e == (p = p->pred_)->data_)
                return p;  //逐个比对，直至命中或范围越界
        return nullptr;  // p越出左边界意味着区间内不含e，查找失败
    }  //失败时，返回NULL
/**********************************排序******************************/
    template<typename T>
    void List<T>::Sort(ListNodePosi<T> p, int n, SortMethod method) {
        switch (method) {
            case SortMethod::INSERTION:
                InsertionSort(p, n);
                break;
            case SortMethod::MERGE:
                /* code */
                break;
            case SortMethod::SELECTION:
                SelectionSort(p, n);
                break;
            default:
                break;
        }
    }

    template<typename T>
    void List<T>::InsertionSort(ListNodePosi<T> p, int n) {
        for (int r = 0; r < n; r++) {
            InsertA(Search(p->data_, r, p), p->data_);
            p = p->succ_;
            Remove(p->pred_);
        }
    }

    template<typename T>
    void List<T>::SelectionSort(ListNodePosi<T> p, int n) {
        auto head = p->pred_;
        auto tail = p;
        for (int i = 0; i < n; i++) {
            tail = tail->succ_;
        }
        while (1 < n) {
            auto max = this->SelectMax(head->succ_, n);
            this->InsertB(trailer_, this->Remove(max));
            trailer_ = trailer_->pred_;
            n--;
        }
    }

    template<typename T>
    ListNodePosi<T> List<T>::SelectMax(ListNodePosi<T> p, int n) {
        auto m = p;
        for (auto cur = p; 1 < n; n--) {
            cur = cur->succ_;
            if (m->data_ <= cur->data_) m = cur;
        }
        return m;
    }

    template<typename T>
    void List<T>::MergeSort(ListNodePosi<T> &p, int n) {
        if (n < 2) return;
        int m = n >> 1;
        auto q = p;
        for (int i = 0; i < m; i++) q = q->succ_;
        MergeSort(p, m);
        MergeSort(q, n - m);
        Merge(p, m, *this, q, n - m);
    }

    template<typename T>
    void List<T>::Merge(ListNodePosi<T> &p, int n, List<T> &L, ListNodePosi<T> q,
                        int m) {
        auto head = p->pred_;
        while (0 < m) {
            if (0 < n && p->data_ <= q->data_) {
                p = p->succ_;
                if (q == p) break;
                n--;
            } else {
                q = q->succ_;
                InsertB(p, L.Remove(q->pred_));
                m--;
            }
        }
        p = head->succ_;
    }
    /**********************************排序******************************/
    /**********************************遍历******************************/
    template<typename T>
    void List<T>::Traverse(void (*visit)(T &)) {
        for (ListNodePosi<T> p = header_->succ_; p != trailer_; p = p->succ_)
            visit(p->data_);
    }

    template<typename T>
    template<typename Visit>
    void List<T>::Traverse(Visit &visit) {
        for (ListNodePosi<T> p = header_->succ_; p != trailer_; p = p->succ_)
            visit(p->data_);
    }
    /**********************************遍历******************************/

} // simple_dsa

#endif //EMBEDDED_2022_LIST_H
